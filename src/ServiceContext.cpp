#include <arpa/inet.h>

#include "ServiceContext.h"





ServiceContext::ServiceContext():
    port     ( 1000    ),
    user     ( "admin" ),
    password ( "admin" ),


    //Device Information
    manufacturer     ( "Manufacturer"   ),
    model            ( "Model"          ),
    firmware_version ( "FirmwareVersion"),
    serial_number    ( "SerialNumber"   ),
    hardware_id      ( "HardwareId"     )
{
}

