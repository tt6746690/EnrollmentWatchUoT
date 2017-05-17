#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <cstdlib>          // exit
#include <cstdio>           // remove
#include <cstring>          // strerror 
#include <csignal>          // signal
#include <unistd.h>         // fork, usleep
#include <fcntl.h>          // close fd
#include <sys/stat.h>       // umask / flags
#include <syslog.h>       

#include <iostream>
#include <unordered_map>
#include <sstream>          // split string
#include <fstream>          
#include <string>            
#include <stdexcept>        // runtime_error

// Suppress unused warning 
template <typename... Args> inline void unused(Args&&...) {}


#define SYSCALL_SUCCESS 1
#define SYSCALL_FAIL 0
#define SYSLOG_OUT "/var/log/system.log" 

#define CONF_DELIM '='

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
 * Handles signal interrupts for daemon
 * -- SIGUP: restart daemon with new config
 * -- SIGTERM: terminate daemon despite resapwn flag
 * -- SIGUSR1: wakes up from sleep and perform action
 */
void dsig_handler(int sig);


/*
 * Daemon configuration struct 
 * -- name: identifier for logging 
 * -- pidfile: a file for safekeeping running daemon instance
 * -- pidfile_path: path for which pidfile is stored
 * -- conf_path: custom configuration file path
 * -- working_dir: directory to which the daemon lives
 * -- mask: permission mask 
 * -- respawn: if daemon respawn upon termination
 */ 
typedef struct dconfig{
    std::string name = "enrolwatchd";
    std::string pidfile = this->name + ".pid";
    std::string pidfile_path = "/tmp/";
    std::string conf_path = ".daemon.conf";
    std::string working_dir = "/";
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    bool respawn = false;
} dconfig;

/*
 * A Daemon that is active in the background
 */
class Daemon{
    private:
        dconfig config;
        /* Spawns a daemon 
         * -- fork: create child process 
         * -- setsid: child leads new session 
         * ---- detaches from controlling tty
         * -- fork: detach from calling terminal
         * Returns
         * -- pid of daemon on success 
         * -- SYSCALL_FAIL on syscall failure
         * Note processes are exited properly during initialization
         */
        int spawn();
        /*
         * Parses config file stream and 
         * returns a map of configuration with 
         * keys as corresponding member of Daemon class
         */ 
        std::unordered_map<std::string, std::string> parse_conf(std::ifstream& fs);
        /*
         * Configures daemon 
         * -- check for pidfile at pidfile_path
         * ---- throws runtime exception if another instance of daemon is present
         * ---- continue otherwise
         * -- change directory 
         * -- clears umask for explicit file creation
         * -- close all open fds
         * -- signal handling 
         * ---- SIG_IGN: 
         * ------ SIGCHLD: signal from child termination signal
         * ------ SIGTSTP: keyboard interrupts
         * ------ SIGTTIN: background read attempted from terminal
         * ---- dsig_handler:
         * ------ SIGTTUOT: background write attempted from terminal
         * ------ SIGTERM: terminates
         * ------ SIGUSR1: terminates and if respawn is true restarts
         * -- saves daemon pid to pidfile at pidfile_path
         * -- setup syslog facilities 
         * Returns 
         * -- SYSCALL_SUCCESS on success
         * -- SYSCALL_FAILURE on failed syscalls
         */
        int configure();
        /*
         * Configures daemon based on file given at conf_path
         */
        int configure(std::string conf_path);
        /*
         * Destroys a daemon 
         * -- remove pidfile from pidpath
         * -- close syslog 
         * Return 
         * -- SYSCALL_SUCCESS on success
         * -- SYSCALL_FAILURE on failed syscalls
         */
        int destroy();
    public: 
        int pid;

        /* Constructor 
         * -- spawns a daemon
         * -- configures the process 
         */
        Daemon();   
        /* Destructor */
        ~Daemon();
};

#endif
