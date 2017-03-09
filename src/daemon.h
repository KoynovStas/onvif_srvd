#ifndef DAEMON_HEADER
#define DAEMON_HEADER





struct daemon_info_t
{
    //flags
    unsigned int terminated     :1;
    unsigned int daemonized     :1;
    unsigned int no_chdir       :1;
    unsigned int no_close_stdio :1;

    const char *pid_file;
    const char *log_file;
    const char *cmd_pipe;
};


extern volatile struct daemon_info_t daemon_info;





int redirect_stdio_to_devnull(void);
int create_pid_file(const char *pid_file_name);



void daemon_error_exit(const char *format, ...);
void exit_if_not_daemonized(int exit_status);



void daemonize2(void (*optional_init)(void *), void *data);

static inline void daemonize() { daemonize2(NULL, NULL); }





#endif //DAEMON_HEADER
