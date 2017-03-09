#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>


#include "daemon.h"

// ---- gsoap ----
#include "DeviceBinding.nsmap"
#include "soapDeviceBindingService.h"





static const char *help_str =
        " ===============  Help  ===============\n"
        " Daemon name:  %s\n"
        " Daemon  ver:  %d.%d.%d\n"
#ifdef  DEBUG
        " Build  mode:  debug\n"
#else
        " Build  mode:  release\n"
#endif
        " Build  date:  "__DATE__"\n"
        " Build  time:  "__TIME__"\n\n"
        "Options:                      description:\n\n"
        "       --no_chdir             Don't change the directory to '/'\n"
        "       --no_close             Don't close standart IO files\n"
        "       --pid_file [value]     Set pid file name\n"
        "       --log_file [value]     Set log file name\n"
        "  -v   --version              Display daemon version information\n"
        "  -h,  --help                 Display this information\n\n";



static const char *short_opts = "hv";


static const struct option long_opts[] =
{
    { "version",      no_argument,       NULL, 'v' },
    { "help",         no_argument,       NULL, 'h' },
    { "no_chdir",     no_argument,       NULL,  1  },
    { "no_close",     no_argument,       NULL,  2  },
    { "pid_file",     required_argument, NULL,  3  },
    { "log_file",     required_argument, NULL,  4  },

    { NULL,           no_argument,       NULL,  0  }
};





void daemon_exit_handler(int sig)
{

    //Here we release resources


    unlink(daemon_info.pid_file);

    _exit(EXIT_FAILURE);
}



void init_signals(void)
{

    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = daemon_exit_handler;
    if( sigaction(SIGTERM, &sa, NULL) != 0 )
        daemon_error_exit("Can't set daemon_exit_handler: %m\n");



    signal(SIGCHLD, SIG_IGN); // ignore child
    signal(SIGTSTP, SIG_IGN); // ignore tty signals
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
}



void processing_cmd(int argc, char *argv[])
{

    int opt, long_index;



    opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
    while( opt != -1 )
    {
        switch( opt )
        {

            case 'v':
                        printf("%s  version  %d.%d.%d\n", DAEMON_NAME, DAEMON_MAJOR_VERSION,
                                                                       DAEMON_MINOR_VERSION,
                                                                       DAEMON_PATCH_VERSION);
                        exit_if_not_daemonized(EXIT_SUCCESS);
                        break;

            case 'h':

                        printf(help_str, DAEMON_NAME, DAEMON_MAJOR_VERSION,
                                                      DAEMON_MINOR_VERSION,
                                                      DAEMON_PATCH_VERSION);
                        exit_if_not_daemonized(EXIT_SUCCESS);
                        break;

            case '?':
            case ':':
                        printf("for more detail see help\n\n");
                        exit_if_not_daemonized(EXIT_FAILURE);
                        break;

//            case 0:     // long options
//                        if( strcmp( "name_options", long_opts[long_index].name ) == 0 )
//                        {
//                            //Processing of "name_options"
//                            break;
//                        }

            case 1:     // --no_chdir
                        daemon_info.no_chdir = 1;
                        break;

            case 2:     // --no_close
                        daemon_info.no_close_stdio = 1;
                        break;

            case 3:     // --pid_file
                        daemon_info.pid_file = optarg;
                        break;

            case 4:     // --log_file
                        daemon_info.log_file = optarg;
                        break;

            default:
                  break;
        }

        opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
    }

}



void init(void *data)
{
    init_signals();


    //Here is your code to initialize
}



int main(int argc, char *argv[])
{

    processing_cmd(argc, argv);
    daemonize2(init, NULL);



    while( !daemon_info.terminated )
    {

        // Here а routine of daemon

        printf("%s: daemon is run\n", DAEMON_NAME);
        sleep(10);
    }



    return EXIT_SUCCESS; // good job (we interrupted (finished) main loop)
}
