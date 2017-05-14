#include <iostream>
#include <curl/curl.h>
#include "daemon.h"

using namespace std;

int Daemon::initialize(){

    pid_t pid, sid;
    int curfd;
    int fd[2];
    int nbytes = 1, flag;

    if(pipe(fd) < 0){
        perror("pipe");
        return -1;
    }

    if((pid = fork()) < 0){
        perror("fork");
        return -1;
    } else if(pid > 0){
        /*
         * Parent process exists upon receiving 
         * successful initialization from child
         * and return -1 if child initialization 
         * fails at some point
         */
        close(fd[1]);
        if(read(fd[0], &flag, nbytes) < 0){
            perror("read");
            return -1;
        }
        if(flag == INIT_SUCCESS){
            // cout << "parent " << getpid() << " exited successfully " << endl;
            close(fd[0]);
            exit(EXIT_SUCCESS);
        } else if(flag == INIT_FAIL){
            return -1;
        }
    } else{
        close(fd[0]);
    }    

    if((sid = setsid()) < 0){
        perror("setsid");
        return -1;
    }

    /* 
     * Second fork both proc has fd[0] closed and fd[1] open
     * -- close fd[1] for parent and exists
     * -- write to fd[1] on success from daemon process
     */
    if((pid = fork()) < 0){
        perror("fork");
        return -1;
    } else if(pid > 0){
        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else{
        flag = INIT_SUCCESS;
        if(write(fd[1], &flag, nbytes) < 0){
            perror("write");
            return -1;
        }
        close(fd[1]);
    }

    // for(curfd = sysconf(_SC_OPEN_MAX); curfd >= 0; curfd--){
    //     close(curfd);
    // }
    umask(0);
    if(chdir("/") < 0){
        perror("chdir");
        return -1;
    }
    
    cout << getpid() << " parent: " << getppid() << endl;
    return pid;
}

int main(int argc, char **argv){

    unused(argc, argv);

    Daemon* d = new Daemon();

    cout << " daemon with pid = " << d->pid << endl;
   
    return 0;
}
