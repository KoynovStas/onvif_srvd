# SPDX-License-Identifier: MIT
#
# Copyright (C) Koynov Stas - skojnov@yandex.ru
#
#
# This module try find the system gsoap
#
# This code sets the following variables:
#
# GSOAP_VERSION         = gSOAP version
#
# GSOAP_INCLUDE_DIR     = Full path to the main gSOAP header stdsoap2.h
# GSOAP_CUSTOM_DIR      = Full path to gSOAP custom serializers for various data types
# GSOAP_IMPORT_DIR      = Full path to gSOAP import-ed header files for soapcpp2 compiler
# GSOAP_PLUGIN_DIR      = Full path to gSOAP plug-ins for advanced features
# GSOAP_WS_DIR          = Full path to gSOAP WS-typemap.dat and WS-* protocol schemas for plug-in development
#
# GSOAP_C_LIBRARY       = Full path to libgsoap.a
# GSOAP_CK_C_LIBRARY    = Full path to libgsoapck.a
# GSOAP_SSL_C_LIBRARY   = Full path to libgsoapssl.a
# GSOAP_CXX_LIBRARY     = Full path to libgsoap++.a
# GSOAP_CK_CXX_LIBRARY  = Full path to libgsoapck++.a
# GSOAP_SSL_CXX_LIBRARY = Full path to libgsoapssl++.a
#
# GSOAP_WSDL2H          = Full path to wsdl2h binary tool
# GSOAP_SOAPCPP2        = Full path to soapcpp2 binary tool
#
# GSOAP_FOUND           = set to true if gsoap was found successfully



message(STATUS "Searching gSOAP")



find_path(GSOAP_INCLUDE_DIR
    NAMES stdsoap2.h
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/include ${GSOAP_DIR}/include/*
    DOC "The main gSOAP header"
)


find_path(GSOAP_CUSTOM_DIR
    NAMES chrono_duration.cpp
    HINTS ${GSOAP_DIR}/custom /usr/share/gsoap/custom
    DOC "gSOAP custom serializers for various data types"
)


find_path(GSOAP_IMPORT_DIR
    NAMES wsa.h
    HINTS ${GSOAP_DIR}/import /usr/share/gsoap/import
    DOC "gSOAP import-ed header files for soapcpp2 compiler"
)


find_path(GSOAP_PLUGIN_DIR
    NAMES wsaapi.h
    HINTS ${GSOAP_DIR}/plugin /usr/share/gsoap/plugin
    DOC "gSOAP plug-ins for advanced features"
)


find_path(GSOAP_WS_DIR
    NAMES typemap.dat
    HINTS ${GSOAP_DIR}/WS /usr/share/gsoap/WS
    DOC "gSOAP WS-typemap.dat and WS-* protocol schemas for plug-in development"
)



# user want use only static library
if(DEFINED USE_GSOAP_STATIC_LIB)
    set(CMAKE_FIND_LIBRARY_SUFFIXES_OLD ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()


# C Library
find_library(GSOAP_C_LIBRARY
    NAMES gsoap
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)


find_library(GSOAP_CK_C_LIBRARY
    NAMES gsoapck
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)


find_library(GSOAP_SSL_C_LIBRARY
    NAMES gsoapssl
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)



# C++ Library
find_library(GSOAP_CXX_LIBRARY
    NAMES gsoap++
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)


find_library(GSOAP_CK_CXX_LIBRARY
    NAMES gsoapck++
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)


find_library(GSOAP_SSL_CXX_LIBRARY
    NAMES gsoapssl++
    HINTS ${GSOAP_DIR} ${GSOAP_DIR}/lib ${GSOAP_DIR}/lib64
          ${GSOAP_DIR}/lib32
)


# restore old value for CMAKE_FIND_LIBRARY_SUFFIXES
if(DEFINED USE_GSOAP_STATIC_LIB)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_OLD})
    unset(CMAKE_FIND_LIBRARY_SUFFIXES_OLD)
endif()



# gSOAP tools
find_program(GSOAP_WSDL2H
    NAMES wsdl2h
    HINTS ${GSOAP_DIR}/wsdl ${GSOAP_DIR}/bin
)


find_program(GSOAP_SOAPCPP2
    NAMES soapcpp2
    HINTS ${GSOAP_DIR}/src ${GSOAP_DIR}/bin
)



# Try get gSOAP version
execute_process(COMMAND ${GSOAP_SOAPCPP2} "-V"
    OUTPUT_VARIABLE GSOAP_STRING_VERSION
    ERROR_VARIABLE  GSOAP_STRING_VERSION
)
string(REGEX MATCH "[0-9]*\\.[0-9]*\\.[0-9]*" GSOAP_VERSION ${GSOAP_STRING_VERSION})
unset(GSOAP_STRING_VERSION)



# Show find results
message(STATUS "GSOAP_VERSION         = ${GSOAP_VERSION}")
if(DEFINED GSOAP_DIR)
    message(STATUS "GSOAP_DIR             = ${GSOAP_DIR}")
endif()
message(STATUS "GSOAP_INCLUDE_DIR     = ${GSOAP_INCLUDE_DIR}")
message(STATUS "GSOAP_CUSTOM_DIR      = ${GSOAP_CUSTOM_DIR}")
message(STATUS "GSOAP_IMPORT_DIR      = ${GSOAP_IMPORT_DIR}")
message(STATUS "GSOAP_PLUGIN_DIR      = ${GSOAP_PLUGIN_DIR}")
message(STATUS "GSOAP_WS_DIR          = ${GSOAP_WS_DIR}")

message(STATUS "GSOAP_C_LIBRARY       = ${GSOAP_C_LIBRARY}")
message(STATUS "GSOAP_CK_C_LIBRARY    = ${GSOAP_CK_C_LIBRARY}")
message(STATUS "GSOAP_SSL_C_LIBRARY   = ${GSOAP_SSL_C_LIBRARY}")
message(STATUS "GSOAP_CXX_LIBRARY     = ${GSOAP_CXX_LIBRARY}")
message(STATUS "GSOAP_CK_CXX_LIBRARY  = ${GSOAP_CK_CXX_LIBRARY}")
message(STATUS "GSOAP_SSL_CXX_LIBRARY = ${GSOAP_SSL_CXX_LIBRARY}")

message(STATUS "GSOAP_SOAPCPP2        = ${GSOAP_SOAPCPP2}")
message(STATUS "GSOAP_WSDL2H          = ${GSOAP_WSDL2H}")



# -----------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set GSOAP_FOUND to TRUE if
# all listed variables are TRUE
# -----------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gSOAP DEFAULT_MSG
    GSOAP_C_LIBRARY
    GSOAP_SSL_C_LIBRARY
    GSOAP_CXX_LIBRARY
    GSOAP_SSL_CXX_LIBRARY
    GSOAP_INCLUDE_DIR
    GSOAP_CUSTOM_DIR
    GSOAP_IMPORT_DIR
    GSOAP_PLUGIN_DIR
    GSOAP_SOAPCPP2
    GSOAP_WSDL2H
)

mark_as_advanced(
    GSOAP_C_LIBRARY
    GSOAP_CK_C_LIBRARY
    GSOAP_SSL_C_LIBRARY
    GSOAP_CXX_LIBRARY
    GSOAP_CK_CXX_LIBRARY
    GSOAP_SSL_CXX_LIBRARY
    GSOAP_INCLUDE_DIR
    GSOAP_CUSTOM_DIR
    GSOAP_IMPORT_DIR
    GSOAP_PLUGIN_DIR
    GSOAP_SOAPCPP2
    GSOAP_WSDL2H
)
