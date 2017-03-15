#include <arpa/inet.h>

#include <sstream>


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



std::string ServiceContext::getXAddr(soap *soap)
{
    std::ostringstream os;

    os << "http://" << getServerIpFromClientIp(htonl(soap->ip)) << ":" << port;

    return os.str();
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



trt__Capabilities *ServiceContext::getMediaServiceCapabilities(soap *soap)
{
    trt__Capabilities *capabilities = soap_new_trt__Capabilities(soap);

    capabilities->ProfileCapabilities = soap_new_trt__ProfileCapabilities(soap);
    capabilities->ProfileCapabilities->MaximumNumberOfProfiles =  soap_new_ptr(soap, 1);

    capabilities->StreamingCapabilities = soap_new_trt__StreamingCapabilities(soap);
    capabilities->StreamingCapabilities->RTPMulticast = soap_new_ptr(soap, false);


    return capabilities;
}




// ------------------------------- StreamProfile -------------------------------




bool StreamProfile::set_name(const char *new_val)
{
    if(!new_val)
    {
        str_err = "Name is empty";
        return false;
    }


    name = new_val;
    return true;
}



bool StreamProfile::set_width(const char *new_val)
{

    std::istringstream ss(new_val);
    int tmp_val;
    ss >> tmp_val;


    if( (tmp_val < 100) || (tmp_val >= 10000) )
    {
        str_err = "width is bad, correct range: 100-10000";
        return false;
    }


    width = tmp_val;
    return true;
}



bool StreamProfile::set_height(const char *new_val)
{
    std::istringstream ss(new_val);
    int tmp_val;
    ss >> tmp_val;


    if( (tmp_val < 100) || (tmp_val >= 10000) )
    {
        str_err = "height is bad, correct range: 100-10000";
        return false;
    }


    height = tmp_val;
    return true;
}



bool StreamProfile::set_url(const char *new_val)
{
    if(!new_val)
    {
        str_err = "URL is empty";
        return false;
    }


    url = new_val;
    return true;
}



bool StreamProfile::set_type(const char *new_val)
{
    std::string new_type(new_val);


    if( new_type == "JPEG" )
        type = tt__VideoEncoding__JPEG;
    else if( new_type == "MPEG4" )
        type = tt__VideoEncoding__MPEG4;
    else if( new_type == "H264" )
        type = tt__VideoEncoding__H264;
    else
    {
        str_err = "type dont support";
        return false;
    }


    return true;
}



void StreamProfile::clear()
{
    name.clear();
    url.clear();

    width  = -1;
    height = -1;
    type   = -1;
}



bool StreamProfile::is_valid()
{
    return ( !name.empty()  &&
             !url.empty()   &&
             (width  != -1) &&
             (height != -1) &&
             (type   != -1)
           );
}
