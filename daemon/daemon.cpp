#include <iostream>
#include <chrono>
#include <thread>
#include "daemon.h"

std::string format_err_msg(int err, const std::string& msg){
    std::string errn = std::to_string(err);
    std::string errstr(std::strerror(err));
    return errn + "-" + errstr + ": " + msg; 
}

void throw_last_err(bool expr, const std::string& msg){
    if(expr){
        throw DaemonRuntimeException(errno, msg);
    }
}

void dsig_handler(int sig){
    switch(sig){
        case SIGHUP:{
            syslog(LOG_NOTICE, "Reload Config on SIGHUP");
            exit(EXIT_SUCCESS);
            break;
        }
        case SIGTERM:{
            syslog(LOG_NOTICE, "Exit daemon on SIGTERM");
            exit(EXIT_SUCCESS);
            break;
         }
    }
}

int Daemon::spawn(){

    if(getppid()==1){
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

        if(flag == SYSCALL_SUCCESS){
            exit(EXIT_SUCCESS);
        } else{
            return SYSCALL_FAIL;
        }
    } else{
        if(close(fd[0]) < 0){ return SYSCALL_FAIL; }
    }    


    if((sid = setsid()) < 0){ return SYSCALL_FAIL; }


    /* 
     * Second fork both proc has fd[0] closed and fd[1] open
     * -- close fd[1] for parent and exists
     * -- write to fd[1] on success from daemon process
     */
    if((pid = fork()) < 0){ return SYSCALL_FAIL; }
    else if(pid > 0){
        exit(EXIT_SUCCESS);
    } else{
        flag = SYSCALL_SUCCESS;
        if(write(fd[1], &flag, nbytes) < 0){ return SYSCALL_FAIL; }
        if(close(fd[1]) < 0){ return SYSCALL_FAIL; }
    }
    return getpid();
}


/*
 * Parses config file stream and 
 * returns a map of configuration with 
 * keys as corresponding member of Daemon class
 */ 
std::unordered_map<std::string, std::string> Daemon::parse_conf(std::ifstream& fs){

    std::unordered_map<std::string, std::string> conf;

    std::string conf_line;
    while(std::getline(fs, conf_line)){

        for(auto iter = conf_line.begin(); iter != conf_line.end(); iter++){
            if(*iter == CONF_DELIM && iter + 1 != conf_line.end()){
                conf[std::string(conf_line.begin(), iter)] = std::string(iter + 1, conf_line.end());
                break;
            }
        }

    }
    return conf;
}

int Daemon::configure(std::string conf_path){

    std::ifstream conf_fs(conf_path, std::ofstream::in);
    std::unordered_map<std::string, std::string> conf;

    if(conf_fs.is_open()){
        conf = parse_conf(conf_fs);

        for(auto it: conf)
            std::cout << it.first << ": " << it.second << std::endl;
    } else {
        throw std::runtime_error("Daemon configuration file does not exists");
    }
    return SYSCALL_SUCCESS;
}


int Daemon::configure(){

    dconfig& config = this->config;

    this->configure(config.conf_path);


    struct stat buf;
    const char *pidp = (config.pidfile_path + config.pidfile).c_str();

    /* daemon fails if another instance already exists */
    if(stat(pidp, &buf) == 0){ 
        throw std::runtime_error("Daemon already running.");
    }

    if(chdir(config.working_dir.c_str()) < 0){
        return SYSCALL_FAIL;
    }

    umask(config.mask);

    // int curfd;
    // for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
    //     close(curfd);
    // }
   
    signal(SIGCHLD, SIG_IGN); /* child terminate signal */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGHUP, dsig_handler);
    signal(SIGTERM, dsig_handler);

    /* Saves name.pid, indicating active daemon */
    std::ofstream pid_fs(pidp, std::ofstream::out);
    chmod(pidp, 0644);
    pid_fs << this->pid << std::endl;
    pid_fs.close();

    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(config.name.c_str(), LOG_PID, LOG_DAEMON);

    return SYSCALL_SUCCESS;
}

int Daemon::destroy(){
    struct stat buf;
    const char *pidp = (this->config.pidfile_path + this->config.pidfile).c_str();

    if(stat(pidp, &buf) < 0){ return SYSCALL_FAIL; }

    if(std::remove(pidp) < 0){ 
        throw DaemonRuntimeException(errno, "Cannot remove daemon pidfile.");
    }

    closelog();
    return SYSCALL_SUCCESS;
}


Daemon::Daemon(): pid(this->spawn()) {
    throw_last_err(this->pid == SYSCALL_FAIL, 
            "Daemon initialization failed.");
    throw_last_err(this->configure() == SYSCALL_FAIL, 
            "Daemon configuration failed.");
    syslog(LOG_NOTICE, "daemon starts running...");
};


Daemon::~Daemon(){
    syslog(LOG_NOTICE, "daemon terminated...");
    throw_last_err(this->destroy() == SYSCALL_FAIL, 
            "Daemon destruction failed.");
};


int main(int argc, char **argv){

    unused(argc, argv);

    try{
        std::auto_ptr <Daemon> d(new Daemon);
        int i = 0;
        while(i++ < 20){
            syslog(LOG_NOTICE, "Loop # %d", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            raise(SIGHUP);
       }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    } catch(const DaemonRuntimeException& e){
        syslog(LOG_ERR, "runtime exception -> %s", e.what());
    }

    exit(EXIT_SUCCESS);
}
