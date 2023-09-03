cmake_minimum_required(VERSION 3.15)


message(STATUS "Generator is set to: ${CMAKE_GENERATOR}")
#set(CMAKE_VERBOSE_MAKEFILE ON)


set(DAEMON_NAME          onvif_srvd)
set(DAEMON_MAJOR_VERSION 2)
set(DAEMON_MINOR_VERSION 0)
set(DAEMON_PATCH_VERSION patch)
# variants: patch - set PATCH_VERSION == number of commits(patches) between head and last tag
# variants: xxx   - set PATCH_VERSION == xxx (your variant)
set(DAEMON_PID_FILE_NAME  ${DAEMON_NAME}.pid)
set(DAEMON_LOG_FILE_NAME  ${DAEMON_NAME}.log)
set(DAEMON_NO_CHDIR       1)
set(DAEMON_NO_CLOSE_STDIO 0)


project(${DAEMON_NAME} LANGUAGES CXX)


set(CMAKE_CXX_STANDARD          11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
set(CMAKE_CXX_FLAGS         "-O2")


add_compile_options(
    -Wall
    -Wextra
)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "No CMAKE_BUILD_TYPE specified, force set to: ${CMAKE_BUILD_TYPE}")
endif()




list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


if(USE_SYSTEM_GSOAP)
    #set(GSOAP_DIR "${CMAKE_BINARY_DIR}/gsoap-2.8/gsoap")
    find_package(gSOAP REQUIRED)
else()
    set(GSOAP_VERSION     2.8.92)
    set(GSOAP_INSTALL_DIR "${CMAKE_SOURCE_DIR}")
    set(GSOAP_ARCHIVE_DIR "${CMAKE_SOURCE_DIR}/SDK")
    set(GSOAP_PATCHES     soapcpp2_lex.l.patch)
    set(GSOAP_CONFIGURE   --disable-c-locale)
    include(build_gsoap)
endif()



add_compile_definitions(
    DAEMON_NAME="${DAEMON_NAME}"
    DAEMON_PID_FILE_NAME="${DAEMON_PID_FILE_NAME}"
    DAEMON_LOG_FILE_NAME="${DAEMON_LOG_FILE_NAME}"
    DAEMON_NO_CHDIR=${DAEMON_NO_CHDIR}
    DAEMON_NO_CLOSE_STDIO=${DAEMON_NO_CLOSE_STDIO}
)



set(COMMON_DIR    "${CMAKE_SOURCE_DIR}/src")
set(WSDL_DIR      "${CMAKE_SOURCE_DIR}/wsdl")
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")

file(GLOB WSDL_FILES "${WSDL_DIR}/*.wsdl" "${WSDL_DIR}/*.xsd")



include_directories(
    ${COMMON_DIR}
    ${GENERATED_DIR}
    ${GSOAP_INCLUDE_DIR}
    ${GSOAP_CUSTOM_DIR}
    ${GSOAP_PLUGIN_DIR}
    ${GSOAP_IMPORT_DIR}
)



# To build a daemon with WS-Security support,
# call make with the WSSE_ON=1 parameter
# see manual: https://www.genivia.com/doc/wsse/html/wsse.html
# example:
# cmake -B build . -DWSSE_ON=1
if(WSSE_ON)
    add_compile_definitions(WITH_OPENSSL WITH_DOM)

    set(WSSE_SOURCES
        ${GSOAP_PLUGIN_DIR}/wsseapi.c
        ${GSOAP_PLUGIN_DIR}/mecevp.c
        ${GSOAP_PLUGIN_DIR}/smdevp.c
        ${GSOAP_PLUGIN_DIR}/wsaapi.c
    )

    set(WSSE_HEADERS
        ${GSOAP_PLUGIN_DIR}/wsseapi.h
        ${GSOAP_PLUGIN_DIR}/mecevp.h
        ${GSOAP_PLUGIN_DIR}/smdevp.h
        ${GSOAP_PLUGIN_DIR}/wsaapi.h
    )

    set(WSSE_IMPORT "${CMAKE_COMMAND}" -E echo "\#import \"wsse.h\"" >> ${GENERATED_DIR}/onvif.h)
endif()



set(SOAP_SOURCES
    ${GENERATED_DIR}/soapC.cpp
    ${GENERATED_DIR}/soapDeviceBindingService.cpp
    ${GENERATED_DIR}/soapMediaBindingService.cpp
    ${GENERATED_DIR}/soapPTZBindingService.cpp

    ${GSOAP_CUSTOM_DIR}/duration.c
)

if(NOT USE_SYSTEM_GSOAP AND NOT USE_GSOAP_STATIC_LIB)
    set(SOAP_SOURCES
        ${SOAP_SOURCES}
        ${GSOAP_DIR}/stdsoap2.cpp
        ${GSOAP_DIR}/dom.cpp)
endif()



set(SOURCES
    ${COMMON_DIR}/daemon.c
    ${COMMON_DIR}/${DAEMON_NAME}.cpp
    ${COMMON_DIR}/eth_dev_param.cpp
    ${COMMON_DIR}/ServiceContext.cpp
    ${COMMON_DIR}/ServiceDevice.cpp
    ${COMMON_DIR}/ServiceMedia.cpp
    ${COMMON_DIR}/ServicePTZ.cpp

    ${SOAP_SOURCES}
    ${WSSE_SOURCES}
)


#process all sources with C++
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)



set(SOAP_HEADERS
    ${GSOAP_INCLUDE_DIR}/stdsoap2.h
    ${GSOAP_CUSTOM_DIR}/duration.h
    ${GENERATED_DIR}/onvif.h
    ${GENERATED_DIR}/soapH.h
    ${GENERATED_DIR}/soapStub.h
    ${GENERATED_DIR}/soapDeviceBindingService.h
    ${GENERATED_DIR}/soapMediaBindingService.h
    ${GENERATED_DIR}/soapPTZBindingService.h
)



set(HEADERS
    ${COMMON_DIR}/daemon.h
    ${COMMON_DIR}/stools.h
    ${COMMON_DIR}/smacros.h
    ${COMMON_DIR}/eth_dev_param.h
    ${COMMON_DIR}/ServiceContext.h

    ${GENERATED_DIR}/version.h

    ${SOAP_HEADERS}
    ${WSSE_HEADERS}
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})



#Create a cmake target that generate gsoap files
add_custom_command(
    OUTPUT ${GENERATED_DIR}/onvif.h
    COMMAND "${CMAKE_COMMAND}" -E make_directory ${GENERATED_DIR}
    COMMAND ${GSOAP_WSDL2H} -d -t ${WSDL_DIR}/typemap.dat
            -o ${GENERATED_DIR}/onvif.h  ${WSDL_FILES}
    COMMAND ${WSSE_IMPORT}
    VERBATIM
    COMMENT "Creating gSOAP binding file"
)


add_custom_command(
    OUTPUT ${GENERATED_DIR}/soapC.cpp
    COMMAND ${GSOAP_SOAPCPP2} -j -L -x -S -d ${GENERATED_DIR}
            -I${GSOAP_INCLUDE_DIR}:${GSOAP_IMPORT_DIR} ${GENERATED_DIR}/onvif.h
    DEPENDS ${GENERATED_DIR}/onvif.h
    COMMENT "Creating gSOAP stubs and glue code"
)


add_custom_target(gsoap_generation
    DEPENDS ${GENERATED_DIR}/soapC.c
)


set_source_files_properties(
    ${GENERATED_DIR}/soapC.cpp
    ${GENERATED_DIR}/soapDeviceBindingService.cpp
    ${GENERATED_DIR}/soapMediaBindingService.cpp
    ${GENERATED_DIR}/soapPTZBindingService.cpp
    ${GENERATED_DIR}/onvif.h
    ${GENERATED_DIR}/soapH.h
    ${GENERATED_DIR}/soapStub.h
    ${GENERATED_DIR}/soapDeviceBindingService.h
    ${GENERATED_DIR}/soapMediaBindingService.h
    ${GENERATED_DIR}/soapPTZBindingService.h
    ${GENERATED_DIR}/version.h
    PROPERTIES GENERATED TRUE
)



add_custom_target(generate_version ALL)

add_custom_command(
    TARGET generate_version
    COMMAND ${CMAKE_COMMAND}
            -DWORK_DIR="${CMAKE_SOURCE_DIR}"
            -DIN_FILE="${CMAKE_SOURCE_DIR}/cmake/version.h.in"
            -DOUT_FILE="${GENERATED_DIR}/version.h"
            -DDAEMON_MAJOR_VERSION=${DAEMON_MAJOR_VERSION}
            -DDAEMON_MINOR_VERSION=${DAEMON_MINOR_VERSION}
            -DDAEMON_PATCH_VERSION=${DAEMON_PATCH_VERSION}
            -P "${CMAKE_SOURCE_DIR}/cmake/version.cmake"
    DEPENDS ${SOURCES}
)

add_dependencies(${PROJECT_NAME} generate_version)



if(USE_SYSTEM_GSOAP OR USE_GSOAP_STATIC_LIB)
    if(WSSE_ON)
        target_link_libraries(${PROJECT_NAME} ${GSOAP_SSL_CXX_LIBRARY})
    else()
        target_link_libraries(${PROJECT_NAME} ${GSOAP_CXX_LIBRARY})
    endif()
endif()


if(WSSE_ON)
    target_link_libraries(${PROJECT_NAME} ssl crypto z)
endif()
