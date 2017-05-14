
+ _Cron_ 
    + a daemon that executes scheduled commands
        + starts automatically from `/etc/init.d`
        + parse `crontabs` for contab files, loaded into memory
        + push to event list 
        + sleep and wakes up until next event scheduled
+ `Crontab`
    + install, uninstall, or list tables for driving cron daemon


+ _Daemon_ 
    + steps 
        + fork child, exit parent process, process adopted by `init`
        + `setsid`: 
            + creates new session
            + calling process becomes leader of new session 
            + process group leader of new process group
            + process detached from controlling tty (i.e. shell session termination no longer kills the process)
        + fork again so that parent process terminate to ensure get rid of session leading process
        + ignore/handle signals 
        + `chdir`
        + `umask`
        + `close` all fd inherited from parents
    + [tutorial](http://0pointer.de/public/systemd-man/daemon.html#New-Style%20Daemons)




+ `umask`
    + command determining the settings of a mask that controls how file permissios are set for newly created files in shell
    + `umask -S` displays mask symbolically

+ `mask`
    + a grouping of bits that filters permission during file creation
    + bit set to `1`: disabled; set to `0`: permission determined by program and system 
    + each process has its own mask 

+ `setsid`
    + creates new session, with calling process the session leader with no controlling terminal 
    + returns process group id of new process group on success; -1 and set `errno` on failure
    + motivation 
        + A session is a set of processes which shares a controlling terminal


