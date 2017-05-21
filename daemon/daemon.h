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
#include <memory>           // unique pointer
        
// Suppress unused warning 
template <typename... Args> inline void unused(Args&&...) {}


#define SYSCALL_SUCCESS 1
#define SYSCALL_FAIL 0
#define SYSLOG_OUT "/var/log/system.log" 


/*
 * Trims str leading and trailing whitespaces
 */
std::string trim(std::string& str);

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
 * Daemon's job 
 */
void job();

/*
 * Handles signal interrupts for daemon
 * -- SIGUP: restart daemon with new config
 * -- SIGTERM: terminate daemon despite resapwn flag
 * -- SIGUSR1: wakes up from sleep and perform action
 */
void dsig_handler(int sig);



/*
 * Daemon configuration struct type
 * -- name: identifier for logging 
 * -- pidfile_path: path for which pidfile is stored
 * -- conf_path: custom configuration file path
 * -- working_dir: directory to which the daemon lives
 * -- mask: permission mask 
 * -- respawn: if daemon respawn upon termination
 * Type made public to be populated and passed into constructor
 * even though config_ is private
 */ 
struct dconfig
{
    static char CONF_DELIM;

    std::string name_ = "enrolwatchd";
    std::string pidfile_path_ = "/Users/markwang/github/EnrollmentWatchUofT/daemon/";
    std::string conf_path_ = "/Users/markwang/github/EnrollmentWatchUofT/daemon/.daemon.conf";
    std::string working_dir_ = "/";
    mode_t mask_ = S_IRWXU | S_IRWXG | S_IRWXO;
    bool respawn_ = true;
    /*
     * Parses config file stream and 
     * returns a map of configuration with 
     * keys as corresponding member of Daemon class
     */ 
    static std::unordered_map<std::string, std::string> parse_conf(std::ifstream& fs);
};
/*
 * A Daemon that is active in the background
 */
class Daemon{
    public: 
        /* Type definition */
        typedef struct dconfig dconfig;
        typedef void (*djob)(); 

        /* member */
        int pid_;
        /*
         * Execute a given function 
         */
        void work_on(djob job);
        /*
         * Get the singleton daemon
         * -- instantiate a unique pointer with corresponding constructor if daemon_ is empty
         * -- return daemon_ otherwise
         */
        static Daemon& get_daemon();
        static Daemon& get_daemon(std::string& conf_path);
        static Daemon& get_daemon(dconfig& config);

        /*
         * Terminate daemon
         * -- terminate: respect respawnable_ during termination
         * -- force_terminate: ignore respawnable_ during termination
         */
        static void terminate();
        static void force_terminate();
        /*
         * Respawns a daemon 
         * -- terminate daemon_ 
         * -- let get_daemon create a new daemon_ instance
         * Postcondition
         * -- new daemon gets a new pid after respawn
         */
        static Daemon& respawn();
        /* public destructor necessary */
        ~Daemon();
    private:
        /* singleton pointer */
        static std::unique_ptr<Daemon> daemon_;
        static bool respawnable_;
        /* Constructor 
         * -- (): spawn and configures a daemon
         * -- (std::string path): initialize daemon with given config path string 
         * -- (Daemon::dconfig config): initialize daemon with dconfig struct
         */
        Daemon();   
        Daemon(std::string& conf_path);
        Daemon(dconfig& config);
        /*
         * private member:
         * -- config_: configuration for daemon
         * -- job_: function invoked 
         * ---- on calling work_on(job)
         * ---- right after respawn
         */
        dconfig config_;
        djob job_;
        /* Spawns a daemon 
         * -- fork: create child process 
         * -- setsid: child leads new session 
         * ---- detaches from controlling tty
         * -- fork: detach from calling terminal
         * Returns
         * -- pid of daemon on success 
         * -- SYSCALL_FAIL on syscall failure
         * Note the calling process is exited properly
         * and a new process is returned with pid
         */
        int spawn() const;
        /*
         * respawns daemon and does job if expr is evaluated to true
         */
        void respawn_if(bool expr) const;
        /*
         * Configures a daemon 
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
        int configure() const;
        /*
         * Configures a daemon based on file given at conf_path
         * -- load config_ with those provided by conf_path
         * -- call configure()
         */
        int configure(std::string& conf_path);
        /*
         * Destroys a daemon 
         * -- remove pidfile from pidpath
         * -- close syslog 
         * Return 
         * -- SYSCALL_SUCCESS on success
         * -- SYSCALL_FAILURE on failed syscalls
         */
         void destroy();
};

bool Daemon::respawnable_ = true;
std::unique_ptr<Daemon> Daemon::daemon_ = nullptr;

char Daemon::dconfig::CONF_DELIM = '=';

#endif
