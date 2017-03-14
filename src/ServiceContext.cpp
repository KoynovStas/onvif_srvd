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



std::string ServiceContext::getServerIpFromClientIp(uint32_t client_ip)
{
    char server_ip[INET_ADDRSTRLEN];


    for(size_t i = 0; i < eth_ifs.size(); ++i)
    {
        uint32_t if_ip, if_mask;
        eth_ifs[i].get_ip(&if_ip);
        eth_ifs[i].get_mask(&if_mask);

        if( (if_ip & if_mask) == (client_ip & if_mask) )
        {
            eth_ifs[i].get_ip(server_ip);
            return server_ip;
        }
    }


    return "127.0.0.1";  //localhost
}



tds__DeviceServiceCapabilities *ServiceContext::getDeviceServiceCapabilities(soap *soap)
{
    tds__DeviceServiceCapabilities *capabilities = soap_new_tds__DeviceServiceCapabilities(soap);

    capabilities->Network = soap_new_tds__NetworkCapabilities(soap);

    capabilities->Network->IPFilter            = soap_new_ptr(soap, false);
    capabilities->Network->ZeroConfiguration   = soap_new_ptr(soap, false);
    capabilities->Network->IPVersion6          = soap_new_ptr(soap, false);
    capabilities->Network->DynDNS              = soap_new_ptr(soap, false);
    capabilities->Network->Dot11Configuration  = soap_new_ptr(soap, false);
    capabilities->Network->Dot1XConfigurations = soap_new_ptr(soap, 0);
    capabilities->Network->HostnameFromDHCP    = soap_new_ptr(soap, false);
    capabilities->Network->NTP                 = soap_new_ptr(soap, 0);
    capabilities->Network->DHCPv6              = soap_new_ptr(soap, false);


    capabilities->Security = soap_new_tds__SecurityCapabilities(soap);

    capabilities->Security->TLS1_x002e0          = soap_new_ptr(soap, false);
    capabilities->Security->TLS1_x002e1          = soap_new_ptr(soap, false);
    capabilities->Security->TLS1_x002e2          = soap_new_ptr(soap, false);
    capabilities->Security->OnboardKeyGeneration = soap_new_ptr(soap, false);
    capabilities->Security->AccessPolicyConfig   = soap_new_ptr(soap, false);
    capabilities->Security->DefaultAccessPolicy  = soap_new_ptr(soap, false);
    capabilities->Security->Dot1X                = soap_new_ptr(soap, false);
    capabilities->Security->RemoteUserHandling   = soap_new_ptr(soap, false);
    capabilities->Security->X_x002e509Token      = soap_new_ptr(soap, false);
    capabilities->Security->SAMLToken            = soap_new_ptr(soap, false);
    capabilities->Security->KerberosToken        = soap_new_ptr(soap, false);
    capabilities->Security->UsernameToken        = soap_new_ptr(soap, false);
    capabilities->Security->HttpDigest           = soap_new_ptr(soap, false);
    capabilities->Security->RELToken             = soap_new_ptr(soap, false);
    capabilities->Security->MaxUsers             = soap_new_ptr(soap, 0);
    capabilities->Security->MaxUserNameLength    = soap_new_ptr(soap, 0);
    capabilities->Security->MaxPasswordLength    = soap_new_ptr(soap, 0);


    capabilities->System = soap_new_tds__SystemCapabilities(soap);

    capabilities->System->DiscoveryResolve       = soap_new_ptr(soap, true);
    capabilities->System->DiscoveryBye           = soap_new_ptr(soap, true);
    capabilities->System->RemoteDiscovery        = soap_new_ptr(soap, true);
    capabilities->System->SystemBackup           = soap_new_ptr(soap, false);
    capabilities->System->SystemLogging          = soap_new_ptr(soap, false);
    capabilities->System->FirmwareUpgrade        = soap_new_ptr(soap, false);
    capabilities->System->HttpFirmwareUpgrade    = soap_new_ptr(soap, false);
    capabilities->System->HttpSystemBackup       = soap_new_ptr(soap, false);
    capabilities->System->HttpSystemLogging      = soap_new_ptr(soap, false);
    capabilities->System->HttpSupportInformation = soap_new_ptr(soap, false);
    capabilities->System->StorageConfiguration   = soap_new_ptr(soap, false);


    return capabilities;
}

