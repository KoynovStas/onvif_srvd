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

    auto ctx   = (ServiceContext*)soap->user;
    auto XAddr = ctx->getXAddr(soap);


    //Device Service
    auto dev_svc = soap_new_tds__Service(soap);
    if(dev_svc)
    {
        dev_svc->soap_default(soap);
        dev_svc->Namespace = SOAP_NAMESPACE_OF_tds;
        dev_svc->XAddr     = XAddr;
        dev_svc->Version   = soap_new_req_tt__OnvifVersion(soap, 16, 12);

        if( tds__GetServices->IncludeCapability )
        {
            auto dev_caps         = ctx->getDeviceServiceCapabilities(soap);
            auto svc_caps         = soap_new_req__tds__GetServiceCapabilitiesResponse(soap, dev_caps);
            dev_svc->Capabilities = soap_new_req__tds__Service_Capabilities(soap);

            if(dev_svc->Capabilities)
                dev_svc->Capabilities->__any.set(svc_caps, SOAP_TYPE_tds__DeviceServiceCapabilities);
        }
    }
    tds__GetServicesResponse.Service.emplace_back(dev_svc);


    //Media Service
    auto med_svc = soap_new_tds__Service(soap);
    if(med_svc)
    {
        med_svc->soap_default(soap);
        med_svc->Namespace = SOAP_NAMESPACE_OF_trt;
        med_svc->XAddr     = XAddr;
        med_svc->Version   = soap_new_req_tt__OnvifVersion(soap, 2, 6);

        if( tds__GetServices->IncludeCapability )
        {
            auto med_caps         = ctx->getMediaServiceCapabilities(soap);
            auto svc_caps         = soap_new_req__trt__GetServiceCapabilitiesResponse(soap, med_caps);
            med_svc->Capabilities = soap_new_req__tds__Service_Capabilities(soap);

            if(med_svc->Capabilities)
                med_svc->Capabilities->__any.set(svc_caps, SOAP_TYPE_tds__DeviceServiceCapabilities);
        }
    }
    tds__GetServicesResponse.Service.emplace_back(med_svc);


    if(ctx->get_ptz_node()->enable)
        return SOAP_OK;

    //PTZ Service
    auto ptz_svc = soap_new_tds__Service(soap);
    if(ptz_svc)
    {
        ptz_svc->soap_default(soap);
        ptz_svc->Namespace = SOAP_NAMESPACE_OF_tptz;
        ptz_svc->XAddr     = XAddr;
        ptz_svc->Version   = soap_new_req_tt__OnvifVersion(soap, 2, 4);

        if( tds__GetServices->IncludeCapability )
        {
            auto ptz_caps         = ctx->getPTZServiceCapabilities(soap);
            auto svc_caps         = soap_new_req__tptz__GetServiceCapabilitiesResponse(soap, ptz_caps);
            ptz_svc->Capabilities = soap_new_req__tds__Service_Capabilities(soap);

            if(ptz_svc->Capabilities)
                ptz_svc->Capabilities->__any.set(svc_caps, SOAP_TYPE_tds__DeviceServiceCapabilities);
        }
    }
    tds__GetServicesResponse.Service.emplace_back(ptz_svc);


    return SOAP_OK;
}



int DeviceBindingService::GetServiceCapabilities(
    _tds__GetServiceCapabilities         *tds__GetServiceCapabilities,
    _tds__GetServiceCapabilitiesResponse &tds__GetServiceCapabilitiesResponse)
{
    UNUSED(tds__GetServiceCapabilities);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    auto ctx = (ServiceContext*)soap->user;
    tds__GetServiceCapabilitiesResponse.Capabilities = ctx->getDeviceServiceCapabilities(soap);

    return SOAP_OK;
}



int DeviceBindingService::GetDeviceInformation(
    _tds__GetDeviceInformation         *tds__GetDeviceInformation,
    _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse)
{
    UNUSED(tds__GetDeviceInformation);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);


    auto ctx = (ServiceContext*)soap->user;
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

    auto ctx = (ServiceContext*)soap->user;

    tds__GetSystemDateAndTimeResponse.SystemDateAndTime = ctx->get_SystemDateAndTime(soap);

    return SOAP_OK;
}



int DeviceBindingService::GetScopes(
    _tds__GetScopes         *tds__GetScopes,
    _tds__GetScopesResponse &tds__GetScopesResponse)
{
    UNUSED(tds__GetScopes);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    auto ctx = (ServiceContext*)soap->user;

    for(auto& scope : ctx->scopes)
    {
        tds__GetScopesResponse.Scopes.push_back(soap_new_req_tt__Scope(soap, tt__ScopeDefinition::Fixed, scope));
    }

    return SOAP_OK;
}



int DeviceBindingService::GetWsdlUrl(
    _tds__GetWsdlUrl         *tds__GetWsdlUrl,
    _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse)
{
    UNUSED(tds__GetWsdlUrl);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    tds__GetWsdlUrlResponse.WsdlUrl = soap->endpoint;

    return SOAP_OK;
}



int DeviceBindingService::GetUsers(
    _tds__GetUsers         *tds__GetUsers,
    _tds__GetUsersResponse &tds__GetUsersResponse)
{
    UNUSED(tds__GetUsers);
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    auto ctx = (ServiceContext*)soap->user;

    if( !ctx->user.empty() )
    {
        auto rsp_user = soap_new_req_tt__User(soap, ctx->user, tt__UserLevel::User);
        tds__GetUsersResponse.User.push_back(rsp_user);
    }

    return SOAP_OK;
}



int DeviceBindingService::GetCapabilities(
    _tds__GetCapabilities         *tds__GetCapabilities,
    _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
    DEBUG_MSG("Device: %s\n", __FUNCTION__);

    auto ctx   = (ServiceContext*)soap->user;
    auto XAddr = ctx->getXAddr(soap);


    tds__GetCapabilitiesResponse.Capabilities = soap_new_req_tt__Capabilities(soap);
    if(!tds__GetCapabilitiesResponse.Capabilities)
        return SOAP_FAULT;

    if(tds__GetCapabilities->Category.empty())
        tds__GetCapabilities->Category.push_back(tt__CapabilityCategory::All); //ONVIF TestTool can skip it


    for(auto category : tds__GetCapabilities->Category)
    {

        if( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Device) )
        {
            tds__GetCapabilitiesResponse.Capabilities->Device = ctx->getDeviceCapabilities(soap, XAddr);
        }


        if( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Media) )
        {
            tds__GetCapabilitiesResponse.Capabilities->Media = ctx->getMediaCapabilities(soap, XAddr);
        }


        if( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::PTZ) )
        {
            tds__GetCapabilitiesResponse.Capabilities->PTZ = ctx->getPTZCapabilities(soap, XAddr);
        }

        if( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Events) )
        {
            tds__GetCapabilitiesResponse.Capabilities->Events = soap_new_req_tt__EventCapabilities(
                soap, XAddr, false, true, false);
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

    auto ctx = (ServiceContext*)soap->user;

    for(const auto& eth_if : ctx->eth_ifs)
    {
        auto net_if = ctx->getNetworkInterface(soap, eth_if);

        if(net_if)
            tds__GetNetworkInterfacesResponse.NetworkInterfaces.push_back(net_if);
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
