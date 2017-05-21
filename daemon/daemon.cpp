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
            syslog(LOG_NOTICE, "dsig_handler:SIGHUP respawn a new daemon");

            Daemon& d = Daemon::respawn();
            d.work_on(job);
            
            syslog(LOG_NOTICE, "dsig_handler:SIGHUP respawn a new daemon after");

            exit(EXIT_SUCCESS);
            break;
        }
        case SIGTERM:
        {
            syslog(LOG_NOTICE, "dsig_handler:SIGTERM daemon terminated unconditionally");

            Daemon::force_terminate();
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



void Daemon::work_on(djob job)
{
    job_ = job;
    job();
}


int Daemon::spawn() const
{

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

void Daemon::respawn_if(bool expr) const
{
    if(expr)
    {
        Daemon& d = Daemon::respawn();
        d.work_on(job_);
        Daemon::terminate();
    }
}


int Daemon::configure() const
{

    struct stat buf;
    const char *pidp = (config_.pidfile_path_ + config_.name_ + ".pid").c_str();

    /* daemon fails if another instance already exists */
    if(stat(pidp, &buf) == 0)
    { 
        throw std::runtime_error("Daemon already running.");
    }

    if(chdir(config_.working_dir_.c_str()) < 0)
    {
        return SYSCALL_FAIL;
    }

    umask(config_.mask_);

    // int curfd;
    // for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
    //     close(curfd);
    // }
   
    if(signal(SIGCHLD, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGTSTP, SIG_IGN) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGHUP, dsig_handler) == SIG_ERR){ return SYSCALL_FAIL; }
    if(signal(SIGTERM, dsig_handler) == SIG_ERR){ return SYSCALL_FAIL; }

    /* Saves name.pid, indicating active daemon */
    std::ofstream pid_fs(pidp, std::ofstream::out);
    if(chmod(pidp, 0644) < 0){ return SYSCALL_FAIL; };
    pid_fs << pid_ << std::endl;
    pid_fs.close();

    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(config_.name_.c_str(), LOG_PID, LOG_DAEMON);

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
            if(it.first == "name"){ config_.name_ = it.second; continue;}
            if(it.first == "pidfile_path"){ config_.pidfile_path_ = it.second; continue; }
            if(it.first == "conf_path"){ config_.conf_path_ = it.second; continue; }
            if(it.first == "working_dir"){ config_.working_dir_ = it.second; continue; }
            if(it.first == "mask"){ config_.mask_ = std::stoi(it.second); continue; }
            if(it.first == "respawn"){ config_.respawn_ = (it.second == "1") ? true:false; continue; }
        }
             
        } else {

        throw std::runtime_error("Daemon configuration file does not exists");
    }

    // static flag updated when loading non-default configuration
    respawnable_ = config_.respawn_;

    return configure();
}

void Daemon::destroy()
{
    const char *pidp = (config_.pidfile_path_ + config_.name_ + ".pid").c_str();

    if(std::remove(pidp) < 0)
    { 
        throw DaemonRuntimeException(errno, "Cannot remove daemon pidfile.");
    }

    closelog();
}


Daemon& Daemon::get_daemon()
{

    if(!daemon_)
    {
        syslog(LOG_NOTICE, "Daemon::get_daemon  instantiating daemon_ %p", &daemon_);
        std::cout << "Daemon::get_daemon instantiating daemon_" << &daemon_ << std::endl;
        daemon_ = std::unique_ptr<Daemon>(new Daemon());
    } else {
        syslog(LOG_NOTICE, "Daemon::get_daemon  get daemon_ %p", &daemon_);
        std::cout << "Daemon::get_daemon get daemon_" << &daemon_ << std::endl;
    }

    return *daemon_;
}

Daemon& Daemon::get_daemon(std::string& conf_path)
{
    if(!daemon_)
    {
        syslog(LOG_NOTICE, "Daemon::get_daemon  instantiating daemon_ %p", &daemon_);
        daemon_ = std::unique_ptr<Daemon>(new Daemon(conf_path));
    } else {
        syslog(LOG_NOTICE, "Daemon::get_daemon  get daemon_ %p", &daemon_);
    }
    return *daemon_;
}

Daemon& Daemon::get_daemon(dconfig& config)
{
    if(!daemon_)
    {
        syslog(LOG_NOTICE, "Daemon::get_daemon  instantiating daemon_ %p", &daemon_);
        daemon_ = std::unique_ptr<Daemon>(new Daemon(config));
    } else {
        syslog(LOG_NOTICE, "Daemon::get_daemon  get daemon_ %p", &daemon_);
    }
    return *daemon_;
}


void Daemon::terminate()
{
    daemon_.reset(nullptr);
}

void Daemon::force_terminate()
{
    respawnable_ = false;
    daemon_.reset(nullptr);
}


Daemon& Daemon::respawn()
{
    if(daemon_)
    {
        Daemon::force_terminate();
    }
    return  Daemon::get_daemon();
}


Daemon::Daemon(): pid_(spawn()) 
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure() == SYSCALL_FAIL, "Daemon configuration failed.");

    syslog(LOG_NOTICE, "Daemon::Daemon %p", this);
    syslog(LOG_NOTICE, "Daemon::Daemon respawnable %d", respawnable_);

}

Daemon::Daemon(std::string& conf_path): pid_(spawn()) 
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure(conf_path) == SYSCALL_FAIL, "Daemon configuration failed.");

    syslog(LOG_NOTICE, "Daemon::Daemon(conf_path) %p", this);
}

Daemon::Daemon(dconfig& config): pid_(spawn()), config_(config)
{
    throw_last_err(pid_ == SYSCALL_FAIL, "Daemon initialization failed.");
    throw_last_err(configure() == SYSCALL_FAIL, "Daemon configuration failed.");

    syslog(LOG_NOTICE, "Daemon::Daemon(config) %p", this);
}



Daemon::~Daemon()
{
    syslog(LOG_NOTICE, "Daemon::~Daemon %p", this);

    destroy();
    respawn_if(respawnable_);
} 


void job()
{
    int i = 0;
    while(i++ < 5)
    {
        syslog(LOG_NOTICE, "Loop # %d", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


int main(int argc, char **argv)
{
    unused(argc, argv);

    setlogmask(LOG_UPTO(LOG_INFO));
    openlog("/Users/markwang/github/EnrollmentWatchUofT/daemon/", LOG_PID, LOG_DAEMON);

    try
    {
        Daemon& d = Daemon::get_daemon();
        d.work_on(job);
    } 
    catch(const DaemonRuntimeException& e)
    {
        syslog(LOG_ERR, "%s", e.what());
    }

    std::cout << "program exiting successfully" << std::endl;

    exit(EXIT_SUCCESS);
}
