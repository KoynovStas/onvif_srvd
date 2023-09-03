# SPDX-License-Identifier: MIT
#
# Copyright (C) Koynov Stas - skojnov@yandex.ru
#
#
# This module build local gsoap
#
# This code sets the following variables:
#
# GSOAP_VERSION         = gSOAP version
#
# GSOAP_DIR             = "${GSOAP_INSTALL_DIR}/gsoap-2.8/gsoap"
# GSOAP_INCLUDE_DIR     = "${GSOAP_DIR}"
# GSOAP_CUSTOM_DIR      = "${GSOAP_DIR}/custom"
# GSOAP_IMPORT_DIR      = "${GSOAP_DIR}/import"
# GSOAP_PLUGIN_DIR      = "${GSOAP_DIR}/plugin"
# GSOAP_WS_DIR          = "${GSOAP_DIR}/WS"
#
# GSOAP_C_LIBRARY       = "${GSOAP_DIR}/libgsoap.a"
# GSOAP_CK_C_LIBRARY    = "${GSOAP_DIR}/libgsoapck.a"
# GSOAP_SSL_C_LIBRARY   = "${GSOAP_DIR}/libgsoapssl.a"
# GSOAP_CXX_LIBRARY     = "${GSOAP_DIR}/libgsoap++.a"
# GSOAP_CK_CXX_LIBRARY  = "${GSOAP_DIR}/libgsoapck++.a"
# GSOAP_SSL_CXX_LIBRARY = "${GSOAP_DIR}/libgsoapssl++.a"
#
# GSOAP_WSDL2H          = wsdl2h binary
# GSOAP_SOAPCPP2        = soapcpp2 binary



if(NOT DEFINED GSOAP_VERSION)
    set(GSOAP_VERSION 2.8.66)
    message(STATUS "No GSOAP_VERSION specified, use default version: ${GSOAP_VERSION}")
endif()


if(NOT DEFINED GSOAP_INSTALL_DIR)
    set(GSOAP_INSTALL_DIR "${CMAKE_BINARY_DIR}")
    message(STATUS "No GSOAP_INSTALL_DIR specified, use default path: ${GSOAP_INSTALL_DIR}")
endif()


if(NOT DEFINED GSOAP_ARCHIVE_DIR)
    set(GSOAP_ARCHIVE_DIR "${CMAKE_BINARY_DIR}")
    message(STATUS "No GSOAP_ARCHIVE_DIR specified, use default path: ${GSOAP_ARCHIVE_DIR}")
endif()


if(DEFINED GSOAP_CONFIGURE)
    message(STATUS "GSOAP_CONFIGURE: ${GSOAP_CONFIGURE}")
endif()


if(DEFINED GSOAP_PATCHES)
    message(STATUS "GSOAP_PATCHES: ${GSOAP_PATCHES}")
endif()




set(GSOAP_ARCHIVE         "${GSOAP_ARCHIVE_DIR}/gsoap_${GSOAP_VERSION}.zip")
set(GSOAP_DIR             "${GSOAP_INSTALL_DIR}/gsoap-2.8/gsoap")
set(GSOAP_INCLUDE_DIR     "${GSOAP_DIR}")
set(GSOAP_CUSTOM_DIR      "${GSOAP_DIR}/custom")
set(GSOAP_IMPORT_DIR      "${GSOAP_DIR}/import")
set(GSOAP_PLUGIN_DIR      "${GSOAP_DIR}/plugin")
set(GSOAP_WS_DIR          "${GSOAP_DIR}/WS")

set(GSOAP_C_LIBRARY       "${GSOAP_DIR}/libgsoap.a")
set(GSOAP_CK_C_LIBRARY    "${GSOAP_DIR}/libgsoapck.a")
set(GSOAP_SSL_C_LIBRARY   "${GSOAP_DIR}/libgsoapssl.a")
set(GSOAP_CXX_LIBRARY     "${GSOAP_DIR}/libgsoap++.a")
set(GSOAP_CK_CXX_LIBRARY  "${GSOAP_DIR}/libgsoapck++.a")
set(GSOAP_SSL_CXX_LIBRARY "${GSOAP_DIR}/libgsoapssl++.a")

set(GSOAP_SOAPCPP2        "${GSOAP_DIR}/src/soapcpp2")
set(GSOAP_WSDL2H          "${GSOAP_DIR}/wsdl/wsdl2h")



function(gsoap_get_archive)

    if(EXISTS ${GSOAP_ARCHIVE})
        return()
    endif()


    find_package(Wget REQUIRED)
    file(MAKE_DIRECTORY ${GSOAP_ARCHIVE_DIR})


    set(GSOAP_ARCHIVE_URLs
        "https://github.com/SrcBackup/gsoap/releases/download/v2.8.x/gsoap_${GSOAP_VERSION}.zip"
        "https://sourceforge.net/projects/gsoap2/files/gsoap_${GSOAP_VERSION}.zip/download"
    )

    # download archive
    foreach(url ${GSOAP_ARCHIVE_URLs})
        execute_process(COMMAND
            "${WGET_EXECUTABLE}" --continue -O ${GSOAP_ARCHIVE}.tmp ${url}
            WORKING_DIRECTORY ${GSOAP_ARCHIVE_DIR}
            RESULT_VARIABLE exit_code
        )

        if(exit_code EQUAL 0)
            file(RENAME ${GSOAP_ARCHIVE}.tmp ${GSOAP_ARCHIVE})
            break()
        endif()
    endforeach()


    if(NOT EXISTS ${GSOAP_ARCHIVE})
        message(FATAL_ERROR "Can't download ${GSOAP_ARCHIVE}")
    endif()

endfunction()



function(gsoap_unzip_archive)

    if(EXISTS ${GSOAP_DIR}/../README.txt)
        return()
    endif()

    execute_process(COMMAND
        unzip ${GSOAP_ARCHIVE} -d ${GSOAP_INSTALL_DIR}
        WORKING_DIRECTORY ${GSOAP_ARCHIVE_DIR}
        RESULT_VARIABLE exit_code
    )
    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "Can't unzip ${GSOAP_ARCHIVE} to ${GSOAP_INSTALL_DIR}")
    endif()

endfunction()



function(gsoap_add_patches)

    if(NOT DEFINED GSOAP_PATCHES)
        return()
    endif()

    # add patches
    foreach(p ${GSOAP_PATCHES})

        set(patch_done_file ${GSOAP_DIR}/${p}.done)
        if(EXISTS ${patch_done_file})
            continue()
        endif()

        execute_process(COMMAND
            patch -p0 -i ${CMAKE_SOURCE_DIR}/patch/${p}
            WORKING_DIRECTORY ${GSOAP_DIR}/../
            RESULT_VARIABLE exit_code
        )

        if(NOT exit_code EQUAL 0)
            message(FATAL_ERROR "Can't add patch: ${p}")
            break()
        endif()

        file(TOUCH ${patch_done_file})
    endforeach()

endfunction()



function(gsoap_build)

    if(EXISTS ${GSOAP_WSDL2H} AND EXISTS ${GSOAP_SOAPCPP2})
        return()
    endif()

    gsoap_get_archive()
    gsoap_unzip_archive()
    gsoap_add_patches()

    execute_process(COMMAND
        ./configure ${GSOAP_CONFIGURE}
        WORKING_DIRECTORY ${GSOAP_DIR}/..
        RESULT_VARIABLE exit_code
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "Can't configure gSOAP")
    endif()


    execute_process(COMMAND
        make -j1
        WORKING_DIRECTORY ${GSOAP_DIR}/..
        RESULT_VARIABLE exit_code
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "Can't build gSOAP")
    endif()

endfunction()



# process
gsoap_build()

unset(GSOAP_ARCHIVE)

message(STATUS "GSOAP_VERSION         = ${GSOAP_VERSION}")
message(STATUS "GSOAP_DIR             = ${GSOAP_DIR}")
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
