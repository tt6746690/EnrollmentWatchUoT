#include <iostream>
#include <curl/curl.h>
#include "daemon.h"

using namespace std;

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

int Daemon::initialize(){

    pid_t pid, sid;
    int curfd;
    int fd[2];
    int nbytes = 1, flag = INIT_FAIL;

    if(pipe(fd) < 0){ return INIT_FAIL; }

    if((pid = fork()) < 0){
        return INIT_FAIL;
    } else if(pid > 0){
        /*
         * Parent process exists upon receiving 
         * successful initialization from child
         * and return -1 if child initialization 
         * fails at some point
         */
        close(fd[1]);
        if(read(fd[0], &flag, nbytes) < 0){
            return INIT_FAIL;
        }
        if(flag == INIT_SUCCESS){
            close(fd[0]);
            exit(EXIT_SUCCESS);
        } else{
            return INIT_FAIL;
        }
    } else{
        close(fd[0]);
    }    
    if((sid = setsid()) < 0){
        return INIT_FAIL;
    }

    /* 
     * Second fork both proc has fd[0] closed and fd[1] open
     * -- close fd[1] for parent and exists
     * -- write to fd[1] on success from daemon process
     */
    if((pid = fork()) < 0){
        return INIT_FAIL;
    } else if(pid > 0){
        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else{
        flag = INIT_SUCCESS;
        if(write(fd[1], &flag, nbytes) < 0){
            return INIT_FAIL;
        }
        close(fd[1]);
    }

    // for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
    //     close(curfd);
    // }
    umask(0);
    if(chdir("/") < 0){
        return INIT_FAIL;
    }
    return getpid();
}

int main(int argc, char **argv){

    unused(argc, argv);

    try{
        Daemon* d = new Daemon();
        cout << "daemon with pid = " << d->pid << endl;
    } catch(const DaemonRuntimeException& e){
        cout << e.what() << endl;
    }

    return 0;
}
