# ONVIF Device(IP camera) Service server


## Description

**onvif_srvd** - ONVIF Device(IP camera) Service server (Linux daemon)

This server (Service) has a minimal implementation. Use this server as a template for writing your ONVIF service for an IP camera.


ONVIF official website: [http://www.onvif.org](http://www.onvif.org)

The web services data binding is generated using [gSOAP](http://www.genivia.com)




## Build

To start build you have to choose your compiler (or toolchain) in the [Makefile](./Makefile) (see variable `$GCC`).

For build daemon in release and debug mode:
```console
make all
```

For build daemon in release mode (strip):
```console
make release
```

For build daemon in debug mode (`#define DEBUG 1`):
```console
make debug
```

Show all supported commands(targets) for  [Makefile](./Makefile):
```console
make help
```

> **Note**: If you need WS-Security support, you need to call make with the `WSSE_ON=1` parameter.

Show how enable support WS-Security:
```console
make WSSE_ON=1
```

If before make was done without WS-Security support, it is necessary to perform the cleanup:
```console
make clean
```



## Usage

To start the daemon, you have to give him the parameters that are needed for work:

```console
./onvif_srvd  --ifs eth0 --scope onvif://www.onvif.org/name/TestDev --scope onvif://www.onvif.org/Profile/S --name RTSP --width 800 --height 600 --url rtsp://%s:554/unicast --type JPEG"
```
The `--url` option can contain or static IP or dynamic (template parameter `%s`) that will be replaced with the IP address of the interface, see the option `--ifs`.

For more details see help:
```console
./onvif_srvd --help
```

Or you can use [S90onvif_srvd](./start_scripts/S90onvif_srvd) script to start the daemon (for old init.d script style):
```console
./start_scripts/S90onvif_srvd start
```

If You use systemd see:
[onvif_srvd.service](./start_scripts/onvif_srvd.service)



## Testing

For testing daemon you need client application.


#### Linux:
1. [ONVIF Device Tool (GUI)](http://lingodigit.com/onvif_nvc.html)

> **Note**:
> 1. ONVIF Device Tool at me this application falls when show the first frame of RTSP. Sad :(.
> 2. This application requires support for **WS-Security**
> 3. This application requires support for [**WS-Discovery**](https://github.com/KoynovStas/wsdd)



2. [gsoap-onvif](https://github.com/tonyhu/gsoap-onvif)

Only the console client, useful for debugging.

Usage:
```console
./ipconvif 127.0.0.1:1000
```


#### Windows:
1. [ONVIF Device Manager](https://sourceforge.net/projects/onvifdm/)



## License

[BSD-3-Clause](./LICENSE).
