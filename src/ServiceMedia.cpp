/*
 --------------------------------------------------------------------------
 ServiceMedia.cpp

 Implementation of functions (methods) for the service:
 ONVIF media.wsdl server side
-----------------------------------------------------------------------------
*/

#include "soapMediaBindingService.h"
#include "ServiceContext.h"
#include "smacros.h"
#include "stools.h"





int MediaBindingService::GetServiceCapabilities(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse &trt__GetServiceCapabilitiesResponse)
{
    UNUSED(trt__GetServiceCapabilities);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    trt__GetServiceCapabilitiesResponse.Capabilities = ctx->getMediaServiceCapabilities(this->soap);


    return SOAP_OK;
}



int MediaBindingService::GetVideoSources(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse &trt__GetVideoSourcesResponse)
{
    UNUSED(trt__GetVideoSources);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();


    for( auto it = profiles.cbegin(); it != profiles.cend(); ++it )
    {
        trt__GetVideoSourcesResponse.VideoSources.push_back(it->second.get_video_src(this->soap));
    }


    return SOAP_OK;
}



int MediaBindingService::GetProfile(_trt__GetProfile *trt__GetProfile, _trt__GetProfileResponse &trt__GetProfileResponse)
{
    DEBUG_MSG("Media: %s   get profile:%s\n", __FUNCTION__, trt__GetProfile->ProfileToken.c_str());

    int ret = SOAP_FAULT;

    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    auto profiles       = ctx->get_profiles();
    auto it             = profiles.find(trt__GetProfile->ProfileToken);


    if( it != profiles.end() )
    {
        trt__GetProfileResponse.Profile = it->second.get_profile(this->soap);
        ret = SOAP_OK;
    }


    return ret;
}



int MediaBindingService::GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse &trt__GetProfilesResponse)
{
    UNUSED(trt__GetProfiles);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();


    for( auto it = profiles.cbegin(); it != profiles.cend(); ++it )
    {
        trt__GetProfilesResponse.Profiles.push_back(it->second.get_profile(this->soap));
    }


    return SOAP_OK;
}



int MediaBindingService::GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse &trt__GetStreamUriResponse)
{
    DEBUG_MSG("Media: %s   for profile:%s\n", __FUNCTION__, trt__GetStreamUri->ProfileToken.c_str());


    int ret = SOAP_FAULT;

    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    auto profiles       = ctx->get_profiles();
    auto it             = profiles.find(trt__GetStreamUri->ProfileToken);


    if( it != profiles.end() )
    {
        trt__GetStreamUriResponse.MediaUri = soap_new_tt__MediaUri(this->soap);
        trt__GetStreamUriResponse.MediaUri->Uri = ctx->get_stream_uri(it->second.get_url(), htonl(this->soap->ip));
        ret = SOAP_OK;
    }


    return ret;
}



int MediaBindingService::GetSnapshotUri(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse &trt__GetSnapshotUriResponse)
{
    DEBUG_MSG("Media: %s   for profile:%s\n", __FUNCTION__, trt__GetSnapshotUri->ProfileToken.c_str());


    int ret = SOAP_FAULT;

    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    auto profiles       = ctx->get_profiles();
    auto it             = profiles.find(trt__GetSnapshotUri->ProfileToken);

    if( it != profiles.end() )
    {
        trt__GetSnapshotUriResponse.MediaUri = soap_new_tt__MediaUri(this->soap);
        trt__GetSnapshotUriResponse.MediaUri->Uri = ctx->get_snapshot_uri(it->second.get_snapurl(), htonl(this->soap->ip));
        ret = SOAP_OK;
    }


    return ret;
}



int MediaBindingService::GetVideoSourceConfigurations(
    _trt__GetVideoSourceConfigurations         *trt__GetVideoSourceConfigurations,
    _trt__GetVideoSourceConfigurationsResponse &trt__GetVideoSourceConfigurationsResponse)
{
    UNUSED(trt__GetVideoSourceConfigurations);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();

    for(auto& p : profiles)
    {
        tt__VideoSourceConfiguration *vsc = p.second.get_video_src_cnf(this->soap);
        trt__GetVideoSourceConfigurationsResponse.Configurations.push_back(vsc);
    }

    return SOAP_OK;
}



int MediaBindingService::GetVideoEncoderConfigurations(
    _trt__GetVideoEncoderConfigurations         *trt__GetVideoEncoderConfigurations,
    _trt__GetVideoEncoderConfigurationsResponse &trt__GetVideoEncoderConfigurationsResponse)
{
    UNUSED(trt__GetVideoEncoderConfigurations);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();

    for(auto& p : profiles)
    {
        tt__VideoEncoderConfiguration *vec = p.second.get_video_enc_cfg(this->soap);
        trt__GetVideoEncoderConfigurationsResponse.Configurations.push_back(vec);
    }

    return SOAP_OK;
}



int MediaBindingService::GetVideoSourceConfiguration(
    _trt__GetVideoSourceConfiguration         *trt__GetVideoSourceConfiguration,
    _trt__GetVideoSourceConfigurationResponse &trt__GetVideoSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();

    for(auto& p : profiles)
    {
        if (trt__GetVideoSourceConfiguration->ConfigurationToken == p.second.get_video_src_cnf(this->soap)->token)
        {
            tt__VideoSourceConfiguration *vsc = p.second.get_video_src_cnf(this->soap);
            trt__GetVideoSourceConfigurationResponse.Configuration = vsc;
            break;
        }
    }

    return SOAP_OK;
}



int MediaBindingService::GetGuaranteedNumberOfVideoEncoderInstances(
    _trt__GetGuaranteedNumberOfVideoEncoderInstances         *trt__GetGuaranteedNumberOfVideoEncoderInstances,
    _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse &trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
    UNUSED(trt__GetGuaranteedNumberOfVideoEncoderInstances);
    DEBUG_MSG("Media: %s\n", __FUNCTION__);

    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();

    int instancesNumber     = 3;
    int jpegInstancesNumber = 0;
    int mpeg4InstancesNumber= 0;
    int h264InstancesNumber = 0;
    int totalNumber         = 0;

    for(auto& p : profiles)
    {
        if (p.second.get_type() == static_cast<int>(tt__VideoEncoding::JPEG))
        {
            jpegInstancesNumber += instancesNumber;
            totalNumber         += instancesNumber;
        }
        else if (p.second.get_type() == static_cast<int>(tt__VideoEncoding::MPEG4))
        {
            mpeg4InstancesNumber += instancesNumber;
            totalNumber          += instancesNumber;
        }
        else if (p.second.get_type() == static_cast<int>(tt__VideoEncoding::H264))
        {
            h264InstancesNumber += instancesNumber;
            totalNumber         += instancesNumber;
        }
    }

    trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse.TotalNumber = totalNumber;
    trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse.JPEG  = soap_new_ptr(soap, jpegInstancesNumber);
    trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse.MPEG4 = soap_new_ptr(soap, mpeg4InstancesNumber);
    trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse.H264  = soap_new_ptr(soap, h264InstancesNumber);

    return SOAP_OK;
}



SOAP_EMPTY_HANDLER(MediaBindingService, trt, StartMulticastStreaming)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, StopMulticastStreaming)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetSynchronizationPoint)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioSources)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioOutputs)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, CreateProfile)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddVideoEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddVideoSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddAudioEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddAudioSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddPTZConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddVideoAnalyticsConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddMetadataConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddAudioOutputConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, AddAudioDecoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveVideoEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveVideoSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveAudioEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveAudioSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemovePTZConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveVideoAnalyticsConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveMetadataConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveAudioOutputConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, RemoveAudioDecoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, DeleteProfile)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioSourceConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioEncoderConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoAnalyticsConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetMetadataConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioOutputConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioDecoderConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoAnalyticsConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetMetadataConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioOutputConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioDecoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleVideoEncoderConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleVideoSourceConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleAudioEncoderConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleAudioSourceConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleVideoAnalyticsConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleMetadataConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleAudioOutputConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetCompatibleAudioDecoderConfigurations)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetVideoSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetVideoEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetAudioSourceConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetAudioEncoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetVideoAnalyticsConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetMetadataConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetAudioOutputConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetAudioDecoderConfiguration)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoSourceConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoEncoderConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioSourceConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioEncoderConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetMetadataConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioOutputConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetAudioDecoderConfigurationOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetVideoSourceModes)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetVideoSourceMode)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetOSDs)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetOSD)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, GetOSDOptions)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, SetOSD)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, CreateOSD)
SOAP_EMPTY_HANDLER(MediaBindingService, trt, DeleteOSD)
