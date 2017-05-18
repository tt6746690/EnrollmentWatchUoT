#include <iostream>
#include <chrono>
#include <thread>
#include "daemon.h"

std::string format_err_msg(int err, const std::string& msg)
{
    std::string errn = std::to_string(err);
    std::string errstr(std::strerror(err));
    return errn + "-" + errstr + ": " + msg; 
}

void throw_last_err(bool expr, const std::string& msg)
{
    if(expr)
    {
        throw DaemonRuntimeException(errno, msg);
    }
}

void dsig_handler(int sig)
{
    switch(sig)
    {
        case SIGHUP:
        {
            syslog(LOG_NOTICE, "Reload Config on SIGHUP");
            exit(EXIT_SUCCESS);
            break;
        }
        case SIGTERM:
        {
            syslog(LOG_NOTICE, "Exit daemon on SIGTERM");
            exit(EXIT_SUCCESS);
            break;
        }
    }
}


std::string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
}

std::unordered_map<std::string, std::string> Daemon::dconfig::parse_conf(std::ifstream& fs)
{

    std::unordered_map<std::string, std::string> conf;
    std::string conf_line;

    while(std::getline(fs, conf_line))
    {
        for(auto iter = conf_line.begin(); iter != conf_line.end(); iter++)
        {
            if(*iter == CONF_DELIM && iter + 1 != conf_line.end())
            {
                std::string key = std::string(conf_line.begin(), iter);
                std::string t_key = trim(key);

                std::string val = std::string(iter + 1, conf_line.end());
                std::string t_val = trim(val);

                conf[t_key] = t_val;
                break;
            }
        }
    }
    return conf;
}



int Daemon::spawn() const
{

    if(getppid()==1)
    {
        throw std::runtime_error("Already a daemon");
    }

    pid_t pid, sid;
    int fd[2];
    int nbytes = 1, flag = SYSCALL_FAIL;

    if(pipe(fd) < 0){ return SYSCALL_FAIL; }

    if((pid = fork()) < 0){ return SYSCALL_FAIL; }
    else if(pid > 0){
        /*
         * Parent process exists upon receiving 
         * successful initialization from child
         * and return -1 if child initialization 
         * fails at some point
         */

        if(close(fd[1]) < 0){ return SYSCALL_FAIL; }
        if(read(fd[0], &flag, nbytes) < 0){ return SYSCALL_FAIL; }

        if(flag == SYSCALL_SUCCESS)
        {
            exit(EXIT_SUCCESS);
        } else
        {
            return SYSCALL_FAIL;
        }
    } else
    {
        if(close(fd[0]) < 0){ return SYSCALL_FAIL; }
    }    


    if((sid = setsid()) < 0){ return SYSCALL_FAIL; }


    /* 
     * Second fork both proc has fd[0] closed and fd[1] open
     * -- close fd[1] for parent and exists
     * -- write to fd[1] on success from daemon process
     */
    if((pid = fork()) < 0){ return SYSCALL_FAIL; }
    else if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    } else {
        flag = SYSCALL_SUCCESS;
        if(write(fd[1], &flag, nbytes) < 0){ return SYSCALL_FAIL; }
        if(close(fd[1]) < 0){ return SYSCALL_FAIL; }
    }
    return getpid();
}


int Daemon::configure() const
{

    struct stat buf;
    const char *pidp = (config_.pidfile_path + config_.name + ".pid").c_str();

    /* daemon fails if another instance already exists */
    if(stat(pidp, &buf) == 0)
    { 
        throw std::runtime_error("Daemon already running.");
    }

    if(chdir(config_.working_dir.c_str()) < 0)
    {
        return SYSCALL_FAIL;
    }

    umask(config_.mask);

    // int curfd;
    // for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
    //     close(curfd);
    // }
   
    if(signal(SIGCHLD, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGTSTP, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGHUP, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGTERM, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    

    /* Saves name.pid, indicating active daemon */
    std::ofstream pid_fs(pidp, std::ofstream::out);
    if(chmod(pidp, 0644) < 0){ return SYSCALL_FAIL; };
    pid_fs << pid_ << std::endl;
    pid_fs.close();

    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(config_.name.c_str(), LOG_PID, LOG_DAEMON);

    return SYSCALL_SUCCESS;
}


int Daemon::configure(std::string& conf_path)
{

    std::ifstream conf_fs(conf_path, std::ofstream::in);
    std::unordered_map<std::string, std::string> conf;

    if(conf_fs.is_open())
    {
        conf = dconfig::parse_conf(conf_fs);

        for(auto it: conf)
        {
            if(it.first == "name"){ config_.name = it.second; continue;}
            if(it.first == "pidfile_path"){ config_.pidfile_path = it.second; continue; }
            if(it.first == "conf_path"){ config_.conf_path = it.second; continue; }
            if(it.first == "working_dir"){ config_.working_dir = it.second; continue; }
            if(it.first == "mask"){ config_.mask = std::stoi(it.second); continue; }
            if(it.first == "respawn"){ config_.respawn = (it.second == "1") ? true:false; continue; }
        }
             
        } else {

        throw std::runtime_error("Daemon configuration file does not exists");
    }

    return configure();
}


int Daemon::destroy() const
{
    struct stat buf;
    const char *pidp = (config_.pidfile_path + config_.name + ".pid").c_str();

    if(stat(pidp, &buf) < 0){ return SYSCALL_FAIL; }

    if(std::remove(pidp) < 0)
    { 
        throw DaemonRuntimeException(errno, "Cannot remove daemon pidfile.");
    }

    closelog();
    return SYSCALL_SUCCESS;
}


Daemon::Daemon(): pid_(spawn()) 
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure() == SYSCALL_FAIL, "Daemon configuration failed.");
    syslog(LOG_NOTICE, "daemon starts running...");
};

Daemon::Daemon(std::string& conf_path): pid_(spawn()) 
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure(conf_path) == SYSCALL_FAIL, "Daemon configuration failed.");
    syslog(LOG_NOTICE, "daemon starts running with given conf_path...");
};

Daemon::Daemon(dconfig& config): pid_(spawn()), config_(config)
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure() == SYSCALL_FAIL, "Daemon configuration failed.");
    syslog(LOG_NOTICE, "daemon starts running with dconfig struct...");
};


Daemon::~Daemon()
{
    syslog(LOG_NOTICE, "daemon terminated...");
    throw_last_err(destroy() == SYSCALL_FAIL, 
            "Daemon destruction failed.");
};


int main(int argc, char **argv)
{

    unused(argc, argv);

    try{
        std::string conf_p(".daemon.conf");
        std::auto_ptr <Daemon> d(new Daemon(conf_p));
        int i = 0;

        while(i++ < 20)
        {
            syslog(LOG_NOTICE, "Loop # %d", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            // raise(SIGHUP);
       }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    } catch(const DaemonRuntimeException& e){
        syslog(LOG_ERR, "runtime exception -> %s", e.what());
    }

    exit(EXIT_SUCCESS);
}
