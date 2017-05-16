#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <cstdlib>          // exit
#include <cstdio>           // remove
#include <unistd.h>         // fork, usleep
#include <fcntl.h>          // close fd
#include <sys/stat.h>       // umask / flags
#include <syslog.h>       

#include <iostream>
#include <fstream>          
#include <string>            
#include <stdexcept>        // runtime_error
#include <cstring>          // strerror 

// Suppress unused warning 
template <typename... Args> inline void unused(Args&&...) {}


#define SYSCALL_SUCCESS 1
#define SYSCALL_FAIL 0
#define SYSLOG_OUT "/var/log/system.log" 

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
        /* Spawns a daemon 
         * -- fork: create child process 
         * -- setsid: child leads new session 
         * -- fork: detach from calling terminal
         * Returns
         * -- pid of daemon on success 
         * -- SYSCALL_FAIL for sys call failure
         * Note processes are exited properly during initialization
         */
        int spawn();
        /*
         * Configures daemon
         * -- check for pidfile at pidfile_path
         * ---- throws runtime exception if another instance of daemon is present
         * ---- continue otherwise
         * -- change directory 
         * -- clears umask for explicit file creation
         * -- close all open fds
         * -- saves daemon pid to pidfile at pidfile_path
         * -- setup syslog facilities 
         * Returns 
         * -- SYSCALL_SUCCESS on success
         * -- SYSCALL_FAIL on failure
         */
        int configure();
        /*
         * Default configuration member
         * -- pidfile + pidfile_path: file where pid of daemon is written to
         * -- working_dir: working directory for daemon
         * -- mask: umask for daemon process
         * -- respawn: if daemon restart on termination
         */
        std::string name = "enrolwatchd";
        std::string pidfile = this->name + ".pid";
        std::string pidfile_path = "/tmp/";
        std::string working_dir = "/";
        mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
        bool respawn = false;

        /*
         * Destroys a daemon 
         * -- remove pidfile from pidpath
         * -- close syslog 
         * Return 
         * -- SYSCALL_SUCCESS for success
         * -- SYSCALL_FAILURE for failed syscals
         */
        int destroy();
    public: 
        int pid;

        /* Constructor spawns a daemon and configures the process */
        Daemon(): pid(this->spawn()) {
            throw_last_err(this->pid == SYSCALL_FAIL, 
                    "Daemon initialization failed.");
            throw_last_err(this->configure() == SYSCALL_FAIL, 
                    "Daemon configuration failed.");
            syslog(LOG_INFO, "daemon starts running...");
        };

        ~Daemon(){
            throw_last_err(this->destroy() == SYSCALL_FAIL, 
                    "Daemon destruction failed.");
        };
};

#endif
