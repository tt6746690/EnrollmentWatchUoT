#include <iostream>
#include <curl/curl.h>
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

int Daemon::spawn(){


    pid_t pid, sid;
    int fd[2];
    int nbytes = 1, flag = INIT_FAIL;

    if(pipe(fd) < 0){ return INIT_FAIL; }

    if((pid = fork()) < 0){ return INIT_FAIL; }
    else if(pid > 0){
        /*
         * Parent process exists upon receiving 
         * successful initialization from child
         * and return -1 if child initialization 
         * fails at some point
         */

        if(close(fd[1]) < 0){ return INIT_FAIL; }
        if(read(fd[0], &flag, nbytes) < 0){ return INIT_FAIL; }

        if(flag == INIT_SUCCESS){
            exit(EXIT_SUCCESS);
        } else{
            return INIT_FAIL;
        }
    } else{
        if(close(fd[0]) < 0){ return INIT_FAIL; }
    }    


    if((sid = setsid()) < 0){ return INIT_FAIL; }


    /* 
     * Second fork both proc has fd[0] closed and fd[1] open
     * -- close fd[1] for parent and exists
     * -- write to fd[1] on success from daemon process
     */
    if((pid = fork()) < 0){ return INIT_FAIL; }
    else if(pid > 0){
        exit(EXIT_SUCCESS);
    } else{
        flag = INIT_SUCCESS;
        if(write(fd[1], &flag, nbytes) < 0){ return INIT_FAIL; }
        if(close(fd[1]) < 0){ return INIT_FAIL; }
    }
    return getpid();
}

int Daemon::configure(){

    struct stat buf;
    const char *pidp = (this->pidfile_path + this->pidfile).c_str();

    /* daemon fails if another instance already exists */
    if(stat(pidp, &buf) == 0){ 
        throw std::runtime_error("Daemon already running.");
    }

    if(chdir(this->working_dir.c_str()) < 0){
        return INIT_FAIL;
    }

    umask(this->mask);

    int curfd;
    for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
        fcntl(curfd, F_SETFD, FD_CLOEXEC);
    }
    std::cout << "Here" << std::endl;
    
    /* Saves name.pid, indicating active daemon */
    std::ofstream pid_fs(pidp, std::ofstream::out);
    chmod(pidp, 0644);
    pid_fs << this->pid << std::endl;
    pid_fs.close();

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(this->name.c_str(), LOG_PID, LOG_DAEMON);

    return INIT_SUCCESS;
}




int main(int argc, char **argv){

    unused(argc, argv);

    try{
        Daemon* d = new Daemon();
        std::cout << "daemon with pid = " << d->pid << std::endl;
    } catch(const DaemonRuntimeException& e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}
