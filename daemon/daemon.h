#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <cstdlib>          // exit
#include <unistd.h>         // fork
#include <sys/stat.h>       // umask

#include <iostream>

// Suppress unused warning 
template <typename... Args> inline void unused(Args&&...) {}

using namespace std;

#define INIT_SUCCESS 1
#define INIT_FAIL 0

class Daemon{
    public: 
        Daemon() {
            cout << getpid() << " parent: " << getppid() << endl;
            this->initialize();
            cout << getpid() << " parent: " << getppid() << endl;
        }
        ~Daemon();

        // pid of this daemon process
        int pid;
    private:
        /* Initilizes a daemon 
         * -- fork: create child process 
         * -- setsid: child leads new session 
         * -- fork: detach from calling terminal
         * -- umask: change file mask 
         * -- chdir: to default directory
         * -- close: all open fds 
         * Returns
         * -- pid of daemon on success 
         * -- -1 for sys call failure
         */
        int initialize();
};






#endif
