/*
 --------------------------------------------------------------------------
 ServiceDevice.cpp

 Implementation of functions (methods) for the service:
 ONVIF devicemgmt.wsdl server side
-----------------------------------------------------------------------------
*/

#include <ctime>

#include "soapDeviceBindingService.h"
#include "ServiceContext.h"
#include "smacros.h"
#include "stools.h"





int DeviceBindingService::GetServices(
    _tds__GetServices         *tds__GetServices,
    _tds__GetServicesResponse &tds__GetServicesResponse)
{
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    std::string XAddr = ctx->getXAddr(this->soap);



    //Device Service
    tds__GetServicesResponse.Service.push_back(soap_new_tds__Service(this->soap));
    tds__GetServicesResponse.Service.back()->Namespace  = "http://www.onvif.org/ver10/device/wsdl";
    tds__GetServicesResponse.Service.back()->XAddr      = XAddr;
    tds__GetServicesResponse.Service.back()->Version    = soap_new_req_tt__OnvifVersion(this->soap, 2, 5);
    if( tds__GetServices->IncludeCapability )
    {
        tds__GetServicesResponse.Service.back()->Capabilities        = soap_new__tds__Service_Capabilities(this->soap);
        tds__DeviceServiceCapabilities *capabilities                 = ctx->getDeviceServiceCapabilities(this->soap);
        tds__GetServicesResponse.Service.back()->Capabilities->__any = soap_dom_element(this->soap, NULL, "tds:Capabilities", capabilities, capabilities->soap_type());
    }


    tds__GetServicesResponse.Service.push_back(soap_new_tds__Service(this->soap));
    tds__GetServicesResponse.Service.back()->Namespace  = "http://www.onvif.org/ver10/media/wsdl";
    tds__GetServicesResponse.Service.back()->XAddr      = XAddr;
    tds__GetServicesResponse.Service.back()->Version    = soap_new_req_tt__OnvifVersion(this->soap, 2, 6);
    if (tds__GetServices->IncludeCapability)
    {
        tds__GetServicesResponse.Service.back()->Capabilities        = soap_new__tds__Service_Capabilities(this->soap);
        trt__Capabilities *capabilities                              = ctx->getMediaServiceCapabilities(this->soap);
        tds__GetServicesResponse.Service.back()->Capabilities->__any = soap_dom_element(this->soap, NULL, "trt:Capabilities", capabilities, capabilities->soap_type());
    }


    if (ctx->get_ptz_node()->enable) {
        tds__GetServicesResponse.Service.push_back(soap_new_tds__Service(this->soap));
        tds__GetServicesResponse.Service.back()->Namespace  = "http://www.onvif.org/ver20/ptz/wsdl";
        tds__GetServicesResponse.Service.back()->XAddr      = XAddr;
        tds__GetServicesResponse.Service.back()->Version    = soap_new_req_tt__OnvifVersion(this->soap, 2, 4);
        if (tds__GetServices->IncludeCapability)
        {
            tds__GetServicesResponse.Service.back()->Capabilities        = soap_new__tds__Service_Capabilities(this->soap);
            tptz__Capabilities *capabilities                             = ctx->getPTZServiceCapabilities(this->soap);
            tds__GetServicesResponse.Service.back()->Capabilities->__any = soap_dom_element(this->soap, NULL, "tptz:Capabilities", capabilities, capabilities->soap_type());
        }
    }


    return SOAP_OK;
}



int DeviceBindingService::GetServiceCapabilities(
    _tds__GetServiceCapabilities         *tds__GetServiceCapabilities,
    _tds__GetServiceCapabilitiesResponse &tds__GetServiceCapabilitiesResponse)
{
    UNUSED(tds__GetServiceCapabilities);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    tds__GetServiceCapabilitiesResponse.Capabilities = ctx->getDeviceServiceCapabilities(this->soap);

    return SOAP_OK;
}



int DeviceBindingService::GetDeviceInformation(
    _tds__GetDeviceInformation         *tds__GetDeviceInformation,
    _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse)
{
    UNUSED(tds__GetDeviceInformation);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    tds__GetDeviceInformationResponse.Manufacturer    = ctx->manufacturer;
    tds__GetDeviceInformationResponse.Model           = ctx->model;
    tds__GetDeviceInformationResponse.FirmwareVersion = ctx->firmware_version;
    tds__GetDeviceInformationResponse.SerialNumber    = ctx->serial_number;
    tds__GetDeviceInformationResponse.HardwareId      = ctx->hardware_id;

    return SOAP_OK;
}



int DeviceBindingService::GetSystemDateAndTime(
    _tds__GetSystemDateAndTime         *tds__GetSystemDateAndTime,
    _tds__GetSystemDateAndTimeResponse &tds__GetSystemDateAndTimeResponse)
{
    UNUSED(tds__GetSystemDateAndTime);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    auto ctx = (ServiceContext*)this->soap->user;

    tds__GetSystemDateAndTimeResponse.SystemDateAndTime = ctx->get_SystemDateAndTime(this->soap);

    return SOAP_OK;
}



int DeviceBindingService::GetScopes(
    _tds__GetScopes         *tds__GetScopes,
    _tds__GetScopesResponse &tds__GetScopesResponse)
{
    UNUSED(tds__GetScopes);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    for(size_t i = 0; i < ctx->scopes.size(); ++i)
    {
        tds__GetScopesResponse.Scopes.push_back(soap_new_req_tt__Scope(soap, tt__ScopeDefinition::Fixed, ctx->scopes[i]));
    }

    return SOAP_OK;
}



int DeviceBindingService::GetWsdlUrl(
    _tds__GetWsdlUrl         *tds__GetWsdlUrl,
    _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse)
{
    UNUSED(tds__GetWsdlUrl);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    std::string url = soap->endpoint;
    tds__GetWsdlUrlResponse.WsdlUrl = url;

    return SOAP_OK;
}



int DeviceBindingService::GetUsers(
    _tds__GetUsers         *tds__GetUsers,
    _tds__GetUsersResponse &tds__GetUsersResponse)
{
    UNUSED(tds__GetUsers);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    if( !ctx->user.empty() )
    {
        tds__GetUsersResponse.User.push_back(soap_new_tt__User(this->soap));
        tds__GetUsersResponse.User.back()->Username = ctx->user;
    }

    return SOAP_OK;
}



int DeviceBindingService::GetCapabilities(
    _tds__GetCapabilities         *tds__GetCapabilities,
    _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    std::string XAddr = ctx->getXAddr(this->soap);



    tds__GetCapabilitiesResponse.Capabilities = soap_new_tt__Capabilities(this->soap);
    std::vector<tt__CapabilityCategory>& categories(tds__GetCapabilities->Category);
    if(categories.empty())
    {
        categories.push_back(tt__CapabilityCategory::All);
    }


    for(tt__CapabilityCategory category : categories)
    {
        if(!tds__GetCapabilitiesResponse.Capabilities->Device && ( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Device) ) )
        {
            tds__GetCapabilitiesResponse.Capabilities->Device = soap_new_tt__DeviceCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Device->XAddr = XAddr;
            tds__GetCapabilitiesResponse.Capabilities->Device->System = soap_new_tt__SystemCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Device->System->SupportedVersions.push_back(soap_new_req_tt__OnvifVersion(this->soap, 2, 0));
            tds__GetCapabilitiesResponse.Capabilities->Device->Network = soap_new_tt__NetworkCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Device->Security = soap_new_tt__SecurityCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Device->IO = soap_new_tt__IOCapabilities(this->soap);
        }


        if(!tds__GetCapabilitiesResponse.Capabilities->Media && ( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Media) ) )
        {
            tds__GetCapabilitiesResponse.Capabilities->Media  = soap_new_tt__MediaCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Media->XAddr = XAddr;
            tds__GetCapabilitiesResponse.Capabilities->Media->StreamingCapabilities = soap_new_tt__RealTimeStreamingCapabilities(this->soap);
            tds__GetCapabilitiesResponse.Capabilities->Media->StreamingCapabilities->RTPMulticast = soap_new_ptr(soap, false);
            tds__GetCapabilitiesResponse.Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = soap_new_ptr(soap, false);
            tds__GetCapabilitiesResponse.Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_new_ptr(soap, true);
        }

        if (ctx->get_ptz_node()->enable) {
            if(!tds__GetCapabilitiesResponse.Capabilities->PTZ && ( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::PTZ) ) )
            {
                tds__GetCapabilitiesResponse.Capabilities->PTZ  = soap_new_tt__PTZCapabilities(this->soap);
                tds__GetCapabilitiesResponse.Capabilities->PTZ->XAddr = XAddr;
            }
        }

    }


    return SOAP_OK;
}



int DeviceBindingService::GetNetworkInterfaces(
    _tds__GetNetworkInterfaces         *tds__GetNetworkInterfaces,
    _tds__GetNetworkInterfacesResponse &tds__GetNetworkInterfacesResponse)
{
    UNUSED(tds__GetNetworkInterfaces);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;


    for(size_t i = 0; i < ctx->eth_ifs.size(); ++i)
    {

        char tmp_buf[20];

        tds__GetNetworkInterfacesResponse.NetworkInterfaces.push_back(soap_new_tt__NetworkInterface(this->soap));
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->Enabled = true;
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->Info = soap_new_tt__NetworkInterfaceInfo(this->soap);
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->Info->Name = soap_new_std__string(this->soap);
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->Info->Name->assign(ctx->eth_ifs[i].dev_name());

        ctx->eth_ifs[i].get_hwaddr(tmp_buf);
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->Info->HwAddress = tmp_buf;

        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->IPv4 = soap_new_tt__IPv4NetworkInterface(this->soap);
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->IPv4->Config = soap_new_tt__IPv4Configuration(this->soap);
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->IPv4->Config->Manual.push_back(soap_new_tt__PrefixedIPv4Address(this->soap));

        ctx->eth_ifs[i].get_ip(tmp_buf);

        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->IPv4->Config->Manual.back()->Address = tmp_buf;
        tds__GetNetworkInterfacesResponse.NetworkInterfaces.back()->IPv4->Config->Manual.back()->PrefixLength = ctx->eth_ifs[i].get_mask_prefix();

    }


    return SOAP_OK;
}



SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetSystemDateAndTime)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetSystemFactoryDefault)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, UpgradeSystemFirmware)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SystemReboot)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, RestoreSystem)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetSystemBackup)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetSystemLog)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetSystemSupportInformation)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetScopes)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, AddScopes)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, RemoveScopes)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDiscoveryMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetDiscoveryMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetRemoteDiscoveryMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetRemoteDiscoveryMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDPAddresses)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetEndpointReference)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetRemoteUser)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetRemoteUser)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, CreateUsers)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, DeleteUsers)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetUser)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetDPAddresses)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetHostname)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetHostname)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetHostnameFromDHCP)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDNS)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetDNS)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetNTP)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetNTP)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDynamicDNS)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetDynamicDNS)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetNetworkInterfaces)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetNetworkProtocols)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetNetworkProtocols)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetNetworkDefaultGateway)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetNetworkDefaultGateway)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetZeroConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetZeroConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetIPAddressFilter)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetIPAddressFilter)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, AddIPAddressFilter)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, RemoveIPAddressFilter)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetAccessPolicy)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetAccessPolicy)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, CreateCertificate)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetCertificates)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetCertificatesStatus)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetCertificatesStatus)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, DeleteCertificates)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetPkcs10Request)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, LoadCertificates)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetClientCertificateMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetClientCertificateMode)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetRelayOutputs)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetRelayOutputSettings)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetRelayOutputState)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SendAuxiliaryCommand)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetCACertificates)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, LoadCertificateWithPrivateKey)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetCertificateInformation)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, LoadCACertificates)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, CreateDot1XConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetDot1XConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDot1XConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDot1XConfigurations)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, DeleteDot1XConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDot11Capabilities)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetDot11Status)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, ScanAvailableDot11Networks)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetSystemUris)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, StartFirmwareUpgrade)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, StartSystemRestore)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetStorageConfigurations)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, CreateStorageConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetStorageConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetStorageConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, DeleteStorageConfiguration)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, GetGeoLocation)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, SetGeoLocation)
SOAP_EMPTY_HANDLER(DeviceBindingService, tds, DeleteGeoLocation)
