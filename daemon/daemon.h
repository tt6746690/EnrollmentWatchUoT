#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <cstdlib>          // exit
#include <unistd.h>         // fork
#include <sys/stat.h>       // umask

#include <iostream>
#include <string>            
#include <stdexcept>        // runtime_error
#include <cstring>          // strerror 

// Suppress unused warning 
template <typename... Args> inline void unused(Args&&...) {}


#define INIT_SUCCESS 1
#define INIT_FAIL 0

/*
 * formats errno, associated interpretation, 
 * and user provided error message
 * Returns 
 * -- string
 */
std::string format_err_msg(int err, const std::string& msg);


/*
 * A runtime exception thrown for sys call failure 
 * during Daemon initialization 
 */
class DaemonRuntimeException: public std::runtime_error{
    private:    
        int _err;
    public: 
        explicit DaemonRuntimeException(int err, const std::string& msg)
            : std::runtime_error(format_err_msg(err, msg)), _err(err){}
        int get_err() const { return _err; }
};


/* if expr evaluates to True, 
 * Throws DaemonRuntimeError for the newest error 
 * specified by errno with given msg 
 */
void throw_last_err(bool expr, const std::string& msg);


/*
 * A Daemon that is active in the background
 */
class Daemon{
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
         * -- INIT_FAIL for sys call failure
         * Note processes are exited properly during initialization
         */
        int initialize();
    public: 
        int pid;
        Daemon(): pid(this->initialize()){
            throw_last_err(this->pid == INIT_FAIL, "Daemon initialization failed.");
        }
        ~Daemon();
};

#endif
