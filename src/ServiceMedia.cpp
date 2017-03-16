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





int MediaBindingService::GetServiceCapabilities(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse &trt__GetServiceCapabilitiesResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;
    trt__GetServiceCapabilitiesResponse.Capabilities = ctx->getMediaServiceCapabilities(this->soap);


    return SOAP_OK;
}



int MediaBindingService::GetVideoSources(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse &trt__GetVideoSourcesResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();


    for( auto it = profiles.cbegin(); it != profiles.cend(); ++it )
    {
        trt__GetVideoSourcesResponse.VideoSources.push_back(it->second.get_video_src(this->soap));
    }


    return SOAP_OK;
}



int MediaBindingService::GetAudioSources(_trt__GetAudioSources *trt__GetAudioSources, _trt__GetAudioSourcesResponse &trt__GetAudioSourcesResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioOutputs(_trt__GetAudioOutputs *trt__GetAudioOutputs, _trt__GetAudioOutputsResponse &trt__GetAudioOutputsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::CreateProfile(_trt__CreateProfile *trt__CreateProfile, _trt__CreateProfileResponse &trt__CreateProfileResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
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
    DEBUG_MSG("Media: %s\n", __FUNCTION__);


    ServiceContext* ctx = (ServiceContext*)this->soap->user;

    auto profiles = ctx->get_profiles();


    for( auto it = profiles.cbegin(); it != profiles.cend(); ++it )
    {
        trt__GetProfilesResponse.Profiles.push_back(it->second.get_profile(this->soap));
    }


    return SOAP_OK;
}



int MediaBindingService::AddVideoEncoderConfiguration(_trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, _trt__AddVideoEncoderConfigurationResponse &trt__AddVideoEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddVideoSourceConfiguration(_trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, _trt__AddVideoSourceConfigurationResponse &trt__AddVideoSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddAudioEncoderConfiguration(_trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, _trt__AddAudioEncoderConfigurationResponse &trt__AddAudioEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddAudioSourceConfiguration(_trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, _trt__AddAudioSourceConfigurationResponse &trt__AddAudioSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddPTZConfiguration(_trt__AddPTZConfiguration *trt__AddPTZConfiguration, _trt__AddPTZConfigurationResponse &trt__AddPTZConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddVideoAnalyticsConfiguration(_trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, _trt__AddVideoAnalyticsConfigurationResponse &trt__AddVideoAnalyticsConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddMetadataConfiguration(_trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, _trt__AddMetadataConfigurationResponse &trt__AddMetadataConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddAudioOutputConfiguration(_trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, _trt__AddAudioOutputConfigurationResponse &trt__AddAudioOutputConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::AddAudioDecoderConfiguration(_trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, _trt__AddAudioDecoderConfigurationResponse &trt__AddAudioDecoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveVideoEncoderConfiguration(_trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, _trt__RemoveVideoEncoderConfigurationResponse &trt__RemoveVideoEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveVideoSourceConfiguration(_trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, _trt__RemoveVideoSourceConfigurationResponse &trt__RemoveVideoSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveAudioEncoderConfiguration(_trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, _trt__RemoveAudioEncoderConfigurationResponse &trt__RemoveAudioEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveAudioSourceConfiguration(_trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, _trt__RemoveAudioSourceConfigurationResponse &trt__RemoveAudioSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemovePTZConfiguration(_trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, _trt__RemovePTZConfigurationResponse &trt__RemovePTZConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveVideoAnalyticsConfiguration(_trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, _trt__RemoveVideoAnalyticsConfigurationResponse &trt__RemoveVideoAnalyticsConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveMetadataConfiguration(_trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, _trt__RemoveMetadataConfigurationResponse &trt__RemoveMetadataConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveAudioOutputConfiguration(_trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, _trt__RemoveAudioOutputConfigurationResponse &trt__RemoveAudioOutputConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::RemoveAudioDecoderConfiguration(_trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, _trt__RemoveAudioDecoderConfigurationResponse &trt__RemoveAudioDecoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::DeleteProfile(_trt__DeleteProfile *trt__DeleteProfile, _trt__DeleteProfileResponse &trt__DeleteProfileResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, _trt__GetVideoSourceConfigurationsResponse &trt__GetVideoSourceConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoEncoderConfigurations(_trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, _trt__GetVideoEncoderConfigurationsResponse &trt__GetVideoEncoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioSourceConfigurations(_trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, _trt__GetAudioSourceConfigurationsResponse &trt__GetAudioSourceConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioEncoderConfigurations(_trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, _trt__GetAudioEncoderConfigurationsResponse &trt__GetAudioEncoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoAnalyticsConfigurations(_trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, _trt__GetVideoAnalyticsConfigurationsResponse &trt__GetVideoAnalyticsConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetMetadataConfigurations(_trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, _trt__GetMetadataConfigurationsResponse &trt__GetMetadataConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioOutputConfigurations(_trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, _trt__GetAudioOutputConfigurationsResponse &trt__GetAudioOutputConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioDecoderConfigurations(_trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, _trt__GetAudioDecoderConfigurationsResponse &trt__GetAudioDecoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoSourceConfiguration(_trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, _trt__GetVideoSourceConfigurationResponse &trt__GetVideoSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoEncoderConfiguration(_trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, _trt__GetVideoEncoderConfigurationResponse &trt__GetVideoEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioSourceConfiguration(_trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, _trt__GetAudioSourceConfigurationResponse &trt__GetAudioSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioEncoderConfiguration(_trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, _trt__GetAudioEncoderConfigurationResponse &trt__GetAudioEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoAnalyticsConfiguration(_trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, _trt__GetVideoAnalyticsConfigurationResponse &trt__GetVideoAnalyticsConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetMetadataConfiguration(_trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, _trt__GetMetadataConfigurationResponse &trt__GetMetadataConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioOutputConfiguration(_trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, _trt__GetAudioOutputConfigurationResponse &trt__GetAudioOutputConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioDecoderConfiguration(_trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, _trt__GetAudioDecoderConfigurationResponse &trt__GetAudioDecoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleVideoEncoderConfigurations(_trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, _trt__GetCompatibleVideoEncoderConfigurationsResponse &trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleVideoSourceConfigurations(_trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, _trt__GetCompatibleVideoSourceConfigurationsResponse &trt__GetCompatibleVideoSourceConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleAudioEncoderConfigurations(_trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, _trt__GetCompatibleAudioEncoderConfigurationsResponse &trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleAudioSourceConfigurations(_trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, _trt__GetCompatibleAudioSourceConfigurationsResponse &trt__GetCompatibleAudioSourceConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleVideoAnalyticsConfigurations(_trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, _trt__GetCompatibleVideoAnalyticsConfigurationsResponse &trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleMetadataConfigurations(_trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, _trt__GetCompatibleMetadataConfigurationsResponse &trt__GetCompatibleMetadataConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleAudioOutputConfigurations(_trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, _trt__GetCompatibleAudioOutputConfigurationsResponse &trt__GetCompatibleAudioOutputConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetCompatibleAudioDecoderConfigurations(_trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, _trt__GetCompatibleAudioDecoderConfigurationsResponse &trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetVideoSourceConfiguration(_trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, _trt__SetVideoSourceConfigurationResponse &trt__SetVideoSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetVideoEncoderConfiguration(_trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, _trt__SetVideoEncoderConfigurationResponse &trt__SetVideoEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetAudioSourceConfiguration(_trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, _trt__SetAudioSourceConfigurationResponse &trt__SetAudioSourceConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetAudioEncoderConfiguration(_trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, _trt__SetAudioEncoderConfigurationResponse &trt__SetAudioEncoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetVideoAnalyticsConfiguration(_trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, _trt__SetVideoAnalyticsConfigurationResponse &trt__SetVideoAnalyticsConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetMetadataConfiguration(_trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, _trt__SetMetadataConfigurationResponse &trt__SetMetadataConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetAudioOutputConfiguration(_trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, _trt__SetAudioOutputConfigurationResponse &trt__SetAudioOutputConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetAudioDecoderConfiguration(_trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, _trt__SetAudioDecoderConfigurationResponse &trt__SetAudioDecoderConfigurationResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, _trt__GetVideoSourceConfigurationOptionsResponse &trt__GetVideoSourceConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoEncoderConfigurationOptions(_trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, _trt__GetVideoEncoderConfigurationOptionsResponse &trt__GetVideoEncoderConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioSourceConfigurationOptions(_trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, _trt__GetAudioSourceConfigurationOptionsResponse &trt__GetAudioSourceConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioEncoderConfigurationOptions(_trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, _trt__GetAudioEncoderConfigurationOptionsResponse &trt__GetAudioEncoderConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, _trt__GetMetadataConfigurationOptionsResponse &trt__GetMetadataConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioOutputConfigurationOptions(_trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, _trt__GetAudioOutputConfigurationOptionsResponse &trt__GetAudioOutputConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetAudioDecoderConfigurationOptions(_trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, _trt__GetAudioDecoderConfigurationOptionsResponse &trt__GetAudioDecoderConfigurationOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetGuaranteedNumberOfVideoEncoderInstances(_trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse &trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
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



int MediaBindingService::StartMulticastStreaming(_trt__StartMulticastStreaming *trt__StartMulticastStreaming, _trt__StartMulticastStreamingResponse &trt__StartMulticastStreamingResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::StopMulticastStreaming(_trt__StopMulticastStreaming *trt__StopMulticastStreaming, _trt__StopMulticastStreamingResponse &trt__StopMulticastStreamingResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetSynchronizationPoint(_trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, _trt__SetSynchronizationPointResponse &trt__SetSynchronizationPointResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetSnapshotUri(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse &trt__GetSnapshotUriResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetVideoSourceModes(_trt__GetVideoSourceModes *trt__GetVideoSourceModes, _trt__GetVideoSourceModesResponse &trt__GetVideoSourceModesResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetVideoSourceMode(_trt__SetVideoSourceMode *trt__SetVideoSourceMode, _trt__SetVideoSourceModeResponse &trt__SetVideoSourceModeResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetOSDs(_trt__GetOSDs *trt__GetOSDs, _trt__GetOSDsResponse &trt__GetOSDsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetOSD(_trt__GetOSD *trt__GetOSD, _trt__GetOSDResponse &trt__GetOSDResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::GetOSDOptions(_trt__GetOSDOptions *trt__GetOSDOptions, _trt__GetOSDOptionsResponse &trt__GetOSDOptionsResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::SetOSD(_trt__SetOSD *trt__SetOSD, _trt__SetOSDResponse &trt__SetOSDResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::CreateOSD(_trt__CreateOSD *trt__CreateOSD, _trt__CreateOSDResponse &trt__CreateOSDResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}



int MediaBindingService::DeleteOSD(_trt__DeleteOSD *trt__DeleteOSD, _trt__DeleteOSDResponse &trt__DeleteOSDResponse)
{
    DEBUG_MSG("Media: %s\n", __FUNCTION__);
    return SOAP_OK;
}
