#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <thread>


#include "daemon.h"
#include "smacros.h"
#include "ServiceContext.h"

// ---- gsoap ----
#include "DeviceBinding.nsmap"
#include "soapDeviceBindingService.h"
#include "soapMediaBindingService.h"
#include "soapwsddService.h"
#include "soapRemoteDiscoveryBindingService.h"





static const char *help_str =
        " ===============  Help  ===============\n"
        " Daemon name:  " DAEMON_NAME          "\n"
        " Daemon  ver:  " DAEMON_VERSION_STR   "\n"
#ifdef  DEBUG
        " Build  mode:  debug\n"
#else
        " Build  mode:  release\n"
#endif
        " Build  date:  " __DATE__ "\n"
        " Build  time:  " __TIME__ "\n\n"
        "Options:                      description:\n\n"
        "       --no_chdir             Don't change the directory to '/'\n"
        "       --no_fork              Don't do fork\n"
        "       --no_close             Don't close standart IO files\n"
        "       --pid_file     [value] Set pid file name\n"
        "       --log_file     [value] Set log file name\n\n"
        "       --port         [value] Set socket port for Services   (default = 1000)\n"
        "       --user         [value] Set user name for Services     (default = admin)\n"
        "       --password     [value] Set user password for Services (default = admin)\n"
        "       --model        [value] Set model device for Services  (default = Model)\n"
        "       --scope        [value] Set scope for Services         (default don't set)\n"
        "       --ifs          [value] Set Net interfaces for work    (default don't set)\n"
        "       --hardware_id  [value] Set Hardware ID of device      (default = HardwareID)\n"
        "       --serial_num   [value] Set Serial number of device    (default = SerialNumber)\n"
        "       --firmware_ver [value] Set firmware version of device (default = FirmwareVersion)\n"
        "       --manufacturer [value] Set manufacturer for Services  (default = Manufacturer)\n\n"
        "       --name         [value] Set Name for Profile Media Services\n"
        "       --width        [value] Set Width for Profile Media Services\n"
        "       --height       [value] Set Height for Profile Media Services\n"
        "       --url          [value] Set URL (or template URL) for Profile Media Services\n"
        "                              in template mode %s will be changed to IP of interfase (see opt ifs)\n"
        "       --type         [value] Set Type for Profile Media Services (JPEG|MPEG4|H264)\n"
        "                              It is also a sign of the end of the profile parameters\n\n"
        "  -v,  --version              Display daemon version\n"
        "  -h,  --help                 Display this help\n\n";




// indexes for long_opt function
namespace LongOpts
{
    enum
    {
        version = 'v',
        help    = 'h',

        //daemon options
        no_chdir = 1,
        no_fork,
        no_close,
        pid_file,
        log_file,

        //ONVIF Service options (context)
        port,
        user,
        password,
        manufacturer,
        model,
        firmware_ver,
        serial_num,
        hardware_id,
        scope,
        ifs,

        //Media Profile for ONVIF Media Service
        name,
        width,
        height,
        url,
        type
    };
}



static const char *short_opts = "hv";


static const struct option long_opts[] =
{
    { "version",      no_argument,       NULL, LongOpts::version       },
    { "help",         no_argument,       NULL, LongOpts::help          },

    //daemon options
    { "no_chdir",     no_argument,       NULL, LongOpts::no_chdir      },
    { "no_fork",      no_argument,       NULL, LongOpts::no_fork       },
    { "no_close",     no_argument,       NULL, LongOpts::no_close      },
    { "pid_file",     required_argument, NULL, LongOpts::pid_file      },
    { "log_file",     required_argument, NULL, LongOpts::log_file      },

    //ONVIF Service options (context)
    { "port",         required_argument, NULL, LongOpts::port          },
    { "user",         required_argument, NULL, LongOpts::user          },
    { "password",     required_argument, NULL, LongOpts::password      },
    { "manufacturer", required_argument, NULL, LongOpts::manufacturer  },
    { "model",        required_argument, NULL, LongOpts::model         },
    { "firmware_ver", required_argument, NULL, LongOpts::firmware_ver  },
    { "serial_num",   required_argument, NULL, LongOpts::serial_num    },
    { "hardware_id",  required_argument, NULL, LongOpts::hardware_id   },
    { "scope",        required_argument, NULL, LongOpts::scope         },
    { "ifs",          required_argument, NULL, LongOpts::ifs           },

    //Media Profile for ONVIF Media Service
    { "name",          required_argument, NULL, LongOpts::name         },
    { "width",         required_argument, NULL, LongOpts::width        },
    { "height",        required_argument, NULL, LongOpts::height       },
    { "url",           required_argument, NULL, LongOpts::url          },
    { "type",          required_argument, NULL, LongOpts::type         },

    { NULL,           no_argument,       NULL,  0                      }
};





#define FOREACH_SERVICE(APPLY, soap)                    \
        APPLY(DeviceBindingService, soap)               \
        APPLY(MediaBindingService, soap)                \


#define FOREACH_DISCOVERY_SERVICE(APPLY, soap)          \
        APPLY(RemoteDiscoveryBindingService, soap)      \
        APPLY(wsddService, soap)                        \

/*
 * If you need support for other services,
 * add the desired option to the macro FOREACH_SERVICE.
 *
 * Note: Do not forget to add the gsoap binding class for the service,
 * and the implementation methods for it, like for DeviceBindingService



        APPLY(ImagingBindingService, soap)               \
        APPLY(PTZBindingService, soap)                   \
        APPLY(RecordingBindingService, soap)             \
        APPLY(ReplayBindingService, soap)                \
        APPLY(SearchBindingService, soap)                \
        APPLY(ReceiverBindingService, soap)              \
        APPLY(DisplayBindingService, soap)               \
        APPLY(EventBindingService, soap)                 \
        APPLY(PullPointSubscriptionBindingService, soap) \
        APPLY(NotificationProducerBindingService, soap)  \
        APPLY(SubscriptionManagerBindingService, soap)   \
*/


#define DECLARE_SERVICE(service, soap) service service ## _inst(soap);

#define DISPATCH_SERVICE(service, soap)                                  \
                else if (service ## _inst.dispatch() != SOAP_NO_METHOD) {\
                    soap_send_fault(soap);                               \
                    soap_stream_fault(soap, std::cerr);                  \
                }




static struct soap *soap;
static struct soap *soap_discovery;

ServiceContext service_ctx;





void daemon_exit_handler(int sig)
{
    //Here we release resources

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);


    unlink(daemon_info.pid_file);


    exit(EXIT_SUCCESS); // good job (we interrupted (finished) main loop)
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
    signal(SIGHUP,  SIG_IGN);
}



void processing_cmd(int argc, char *argv[])
{
    int opt;

    StreamProfile  profile;


    while( (opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1 )
    {
        switch( opt )
        {

            case LongOpts::help:
                        puts(help_str);
                        exit_if_not_daemonized(EXIT_SUCCESS);
                        break;

            case LongOpts::version:
                        puts(DAEMON_NAME "  version  " DAEMON_VERSION_STR "\n");
                        exit_if_not_daemonized(EXIT_SUCCESS);
                        break;


                 //daemon options
            case LongOpts::no_chdir:
                        daemon_info.no_chdir = 1;
                        break;

            case LongOpts::no_fork:
                        daemon_info.no_fork = 1;
                        break;

            case LongOpts::no_close:
                        daemon_info.no_close_stdio = 1;
                        break;

            case LongOpts::pid_file:
                        daemon_info.pid_file = optarg;
                        break;

            case LongOpts::log_file:
                        daemon_info.log_file = optarg;
                        break;


            //ONVIF Service options (context)
            case LongOpts::port:
                        service_ctx.port = atoi(optarg);
                        break;

            case LongOpts::user:
                        service_ctx.user = optarg;
                        break;

            case LongOpts::password:
                        service_ctx.password = optarg;
                        break;

            case LongOpts::manufacturer:
                        service_ctx.manufacturer = optarg;
                        break;

            case LongOpts::model:
                        service_ctx.model = optarg;
                        break;

            case LongOpts::firmware_ver:
                        service_ctx.firmware_version = optarg;
                        break;

            case LongOpts::serial_num:
                        service_ctx.serial_number = optarg;
                        break;

            case LongOpts::hardware_id:
                        service_ctx.hardware_id = optarg;
                        break;

            case LongOpts::scope:
                        service_ctx.scopes.push_back(optarg);
                        break;

            case LongOpts::ifs:
                        service_ctx.eth_ifs.push_back(Eth_Dev_Param());

                        if( service_ctx.eth_ifs.back().open(optarg) != 0 )
                            daemon_error_exit("Can't open ethernet interface: %s - %m\n", optarg);

                        break;


            //Media Profile for ONVIF Media Service
            case LongOpts::name:
                        if( !profile.set_name(optarg) )
                            daemon_error_exit("Can't set name for Profile: %s\n", profile.get_cstr_err());

                        break;


            case LongOpts::width:
                        if( !profile.set_width(optarg) )
                            daemon_error_exit("Can't set width for Profile: %s\n", profile.get_cstr_err());

                        break;


            case LongOpts::height:
                        if( !profile.set_height(optarg) )
                            daemon_error_exit("Can't set height for Profile: %s\n", profile.get_cstr_err());

                        break;


            case LongOpts::url:
                        if( !profile.set_url(optarg) )
                            daemon_error_exit("Can't set URL for Profile: %s\n", profile.get_cstr_err());

                        break;


            case LongOpts::type:
                        if( !profile.set_type(optarg) )
                            daemon_error_exit("Can't set type for Profile: %s\n", profile.get_cstr_err());

                        if( !service_ctx.add_profile(profile) )
                            daemon_error_exit("Can't add Profile: %s\n", service_ctx.get_cstr_err());

                        profile.clear(); //now we can add new profile (just uses one variable)

                        break;


            default:
                        puts("for more detail see help\n\n");
                        exit_if_not_daemonized(EXIT_FAILURE);
                        break;
        }
    }
}



void check_service_ctx(void)
{
    if(service_ctx.eth_ifs.empty())
        daemon_error_exit("Error: not set no one ehternet interface more details see opt --ifs\n");


    if(service_ctx.scopes.empty())
        daemon_error_exit("Error: not set scopes more details see opt --scope\n");


    if(service_ctx.get_profiles().empty())
        daemon_error_exit("Error: not set no one profile more details see --help\n");
}

void init_gsoap(void)
{
    soap = soap_new();

    if(!soap)
        daemon_error_exit("Can't get mem for SOAP\n");


    soap->bind_flags = SO_REUSEADDR;

    if( !soap_valid_socket(soap_bind(soap, NULL, service_ctx.port, 10)) )
    {
        soap_stream_fault(soap, std::cerr);
        exit(EXIT_FAILURE);
    }


    soap->send_timeout = 3; // timeout in sec
    soap->recv_timeout = 3; // timeout in sec


    //save pointer of service_ctx in soap
    soap->user = (void*)&service_ctx;
}

void init_gsoap_discovery(void) {
    unsigned char loop = 1;
    struct ip_mreq mreqcon;
    soap_discovery = soap_new();

    if (!soap_discovery)
        daemon_error_exit("Can't get mem for soap discovery\n");

    soap_discovery->connect_flags = SO_BROADCAST;
    soap_discovery->port = 3702;

    soap_set_namespaces(soap_discovery, namespaces);
    soap_discovery->omode = SOAP_IO_UDP;
    soap_discovery->bind_flags = SO_REUSEADDR;

    if (!soap_valid_socket(soap_bind(soap_discovery, NULL, soap_discovery->port, 100))) {
        soap_stream_fault(soap_discovery, std::cerr);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < service_ctx.eth_ifs.size(); ++i)
    {
        service_ctx.eth_ifs[i].set_gateway("-net 224.0.0.0 netmask 224.0.0.0");
    }

    if(setsockopt(soap_discovery->master, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)))
    {
        perror("setsockopt");
    }
    mreqcon.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mreqcon.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(soap_discovery->master, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqcon, sizeof(mreqcon)))
    {
        perror("setsockopt");
    }

    //save pointer of service_ctx in soap
    soap_discovery->user = (void*)&service_ctx;
}

void discovery_handler()
{
    FOREACH_DISCOVERY_SERVICE(DECLARE_SERVICE, soap_discovery)

    while( true )
    {
        if(!soap_valid_socket(soap_accept(soap_discovery)))
        {
            soap_stream_fault(soap_discovery, std::cerr);
            return;
        }

        // process service
        if( soap_begin_serve(soap_discovery) )
        {
            soap_stream_fault(soap_discovery, std::cerr);
        }
        FOREACH_DISCOVERY_SERVICE(DISPATCH_SERVICE, soap)
        else
        {
            DEBUG_MSG("Unknown service\n");
        }
    }
}

void init(void *data)
{
    init_signals();
    check_service_ctx();
    init_gsoap_discovery();
    init_gsoap();
}



int main(int argc, char *argv[])
{
    processing_cmd(argc, argv);
    daemonize2(init, NULL);

    std::thread thread_discovery(discovery_handler);

    FOREACH_SERVICE(DECLARE_SERVICE, soap)

    while( true )
    {
        // wait new client
        if( !soap_valid_socket(soap_accept(soap)) )
        {
            soap_stream_fault(soap, std::cerr);
            return EXIT_FAILURE;
        }


        // process service
        if( soap_begin_serve(soap) )
        {
            soap_stream_fault(soap, std::cerr);
        }
        FOREACH_SERVICE(DISPATCH_SERVICE, soap)
        else
        {
            DEBUG_MSG("Unknown service\n");
        }
    }

    thread_discovery.join();

    return EXIT_SUCCESS; // good job (we interrupted (finished) main loop)
}
