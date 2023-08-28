# Change Log

[**onvif_srvd**](https://github.com/KoynovStas/onvif_srvd) - is Linux daemon for ONVIF Device(IP camera) Service server.

ONVIF official website: [http://www.onvif.org](http://www.onvif.org)

The web services data binding is generated using [gSOAP](http://www.genivia.com)


License: [GPLv2](./LICENSE).

Copyright (c) 2016, Koynov Stas - skojnov@yandex.ru



---



## [v2.0](https://github.com/KoynovStas/onvif_srvd/tree/v2.0) (2023-08-28)
[Full Changelog](https://github.com/KoynovStas/onvif_srvd/compare/v1.1...v2.0)


### New

 - **changed** license to **GPLv2** (gsoap requirements) see: [issue 21](https://github.com/KoynovStas/onvif_srvd/issues/21)
 - update `gsoap` to ver `2.8.92` (was `2.8.55`)
 - Added support for `snapshot` and `PTZ service` see details [PR 13](https://github.com/KoynovStas/onvif_srvd/pull/13) Thank for [roleo](https://github.com/roleoroleo)
 - add [`get_time_zone` method and `enum TimeZoneFormat`](https://github.com/KoynovStas/onvif_srvd/commit/c4b54531a694bf501516bd5cbaa50eb9feafc411)
 - add option `--tz_format` for set `TimeZoneFormat`
 - add `ServiceContext::get_SystemDateAndTime`
 - add `set_sig_handler` function
 - add process Ctrl-C in terminal(`SIGINT` signal) for debug
 - add various `PATCH_VERSION` processing options
 - add `systemd` script
 - add `SOAP_EMPTY_HANDLER` macros


### Refactoring

 - now we can set extern toolchain (compiller)
 - update mirrors for `gSOAP` arhive
 - PTZ:Ref: use `soap_new_req_tt__Vector2D/1D` (malloc + init params)
 - PTZ:Ref: use `soap_new_req_tt__FloatRange` (malloc + init params)
 - PTZ:Ref: use `auto` from C++11


### BugFix

 - Closed [issue 19](https://github.com/KoynovStas/onvif_srvd/issues/19)
   (Memory leak) We must delete all dynamically-allocated memory managed by soap context
 - add `soapcpp2_lex.l.patch` For more details see: https://sourceforge.net/p/gsoap2/bugs/1269/




## [v1.1](https://github.com/KoynovStas/onvif_srvd/tree/v1.1) (2017-11-23)
[Full Changelog](https://github.com/KoynovStas/onvif_srvd/compare/v1.0...v1.1)


### New

 - uses new ver of `gsoap 2.8.55` (was `2.8.44`)
 - update daemon.c/h to ver 1.1
 - update smacros.h  to ver 1.1


### BugFix

 - fix: del debug output




## [v1.0](https://github.com/KoynovStas/onvif_srvd/tree/v1.0) (2017-04-04)
