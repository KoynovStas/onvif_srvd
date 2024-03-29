# ONVIF Device(IP camera) Service server


## Description

**onvif_srvd** - ONVIF Device(IP camera) Service server (Linux daemon)

This server (Service) has a minimal implementation. Use this server as a template for writing your ONVIF service for an IP camera.


ONVIF official website: [https://www.onvif.org](https://www.onvif.org)
and their [github presence](https://github.com/onvif/).

The web services data binding is generated using [gSOAP](https://www.genivia.com)




## Build

#### Dependencies
Most Linux systems for building this project require the following packages to be installed: `make m4 flex bison byacc yacc`

If you need support for encryption and WS-Security then you also need: `openssl zlib libcrypto`


For example, on ubuntu 22.04, you needed to install:
```console
sudo apt install flex bison byacc make cmake m4

#for support encryption and WS-Security
sudo apt install openssl libssl-dev zlib1g-dev libcrypto++8
```

For build use cmake for [CMakeLists.txt](./CMakeLists.txt):

You have 4 build variants:

1. By default, the build takes place in the old style (when there was a `Makefile`)
  We build our own version of `gSOAP` and use it (see [build_gsoap.cmake](./cmake/build_gsoap.cmake)).
  At the same time, we compile the necessary `gSOAP` functions with the project.

```console
cd repo_dir
cmake -B build .
cmake --build build
```


2. Analogue of the 1st, but we use the static library `gSOAP` (we link it)

```console
cd repo_dir
cmake -B build . -DUSE_GSOAP_STATIC_LIB=1
cmake --build build
```


3. We use the system `gSOAP`, for this we use the search module([FindgSOAP.cmake](./cmake/FindgSOAP.cmake)),
 this module should find the `gSOAP` in the system that we will use.

```console
cd repo_dir
cmake -B build . -DUSE_SYSTEM_GSOAP=1
cmake --build build
```

For example, in Ubuntu 22.04, to install `gSOAP`, you need to install the following packages:

```console
sudo apt install gsoap libgsoap-dev
```


4. Analogue of the 3rd, but we tell the [FindgSOAP.cmake](./cmake/FindgSOAP.cmake) search module to look for static libraries instead of shared ones.
  This will allow you not to depend on the `gSOAP` system libraries.

```console
cd repo_dir
cmake -B build . -DUSE_SYSTEM_GSOAP=1 -DUSE_GSOAP_STATIC_LIB=1
cmake --build build
```

> **Note**: If you need WS-Security support, you need to call make with the `WSSE_ON=1` parameter.

Show how enable support WS-Security:
```console
cmake -B build . -DWSSE_ON=1
```

If before make was done without WS-Security support, **must regenerate** (We need to rebuild the gsoap with `openssl` support)



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

[GPLv2](./LICENSE).
