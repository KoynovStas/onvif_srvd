/*
 --------------------------------------------------------------------------
 ServiceDiscovery.cpp
 
 Implementation of functions (methods) for the service:
 ONVIF discovery.wsdl server side
-----------------------------------------------------------------------------
*/

#include "soapwsddService.h"
#include "soapRemoteDiscoveryBindingService.h"
#include "ServiceContext.h"
#include "smacros.h"

static int soap_send___wsdd__ProbeMatches(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{	
  struct __wsdd__ProbeMatches soap_tmp___wsdd__ProbeMatches;
  if (soap_action == NULL)
    soap_action ="http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";

  soap_begin(soap);
  soap->encodingStyle = NULL;
  soap_tmp___wsdd__ProbeMatches.wsdd__ProbeMatches = wsdd__ProbeMatches;
  soap_serializeheader(soap);
  soap_serialize___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches);

  if (soap_begin_count(soap))
    return soap->error;

  if (soap->mode & SOAP_IO_LENGTH)
  {	if (soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL)
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap))
    return soap->error;
  }

  if (soap_end_count(soap))
    return soap->error;

  if (soap_connect(soap, soap_endpoint, soap_action)
      || soap_response(soap, SOAP_OK)
      || soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL)
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap)
      || soap_end_send(soap))
    return soap_closesock(soap);

  return SOAP_OK;
}

int wsddService::Hello(struct wsdd__HelloType *wsdd__Hello)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int wsddService::Bye(struct wsdd__ByeType *wsdd__Bye)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int wsddService::Probe(struct wsdd__ProbeType *wsdd__Probe)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
	char macaddr[6];
	char _HwId[1024];


	ServiceContext* ctx = (ServiceContext*)this->soap->user;
	memset(macaddr, 0, sizeof(macaddr));
    ctx->eth_ifs[0].get_hwaddr(macaddr);
    sprintf(_HwId,"urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X",macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

    std::string scopes_items;
    scopes_items = "onvif://www.onvif.org/type/NetworkVideoTransmitter";
    scopes_items += " onvif://www.onvif.org/name/" "TestDevices";
    scopes_items += " onvif://www.onvif.org/hardware/" + ctx->hardware_id;
    scopes_items += " onvif://www.onvif.org/version/" + ctx->firmware_version;
    scopes_items += " onvif://www.onvif.org/serial/" + ctx->serial_number;
    scopes_items += " onvif://www.onvif.org/location/";

    wsdd__ScopesType *scopes = soap_new_set_wsdd__ScopesType(soap, (char *)scopes_items.c_str(), NULL);

    char **any = (char **)soap_malloc(soap, sizeof(char*) * 8);
    any[0] = (char *)soap_malloc(soap, sizeof(char) * 8);
    strcpy(any[0], "Any");

    wsa5__ReferenceParametersType *parameters = soap_new_set_wsa5__ReferenceParametersType(soap, NULL, 0, any, (char *)"Attribute");

    wsa5__MetadataType *metadata = soap_new_set_wsa5__MetadataType(soap,
            0, any, (char*)"Attribute");

    wsa5__EndpointReferenceType *endpoint = soap_new_set_wsa5__EndpointReferenceType(soap,
            _HwId, parameters, metadata, 0, any, (char *)"Attribute");

    char *xAddrs = (char *)soap_malloc(soap, sizeof(char) * 256);
    strcpy(xAddrs, ctx->getXAddr(soap).c_str());
	wsdd__ProbeMatchType *probematch = soap_new_set_wsdd__ProbeMatchType(soap,
	        *endpoint, wsdd__Probe->Types, scopes, xAddrs, 1);


	wsdd__ProbeMatchesType *probeMatches = soap_new_set_wsdd__ProbeMatchesType(soap,
	        1, probematch);

    soap->header->wsa5__To = (char *)"http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
	soap->header->wsa5__RelatesTo = soap_new_set_wsa5__RelatesToType(soap,
	        soap->header->wsa5__MessageID, NULL, NULL);
    soap->header->wsa5__MessageID = _HwId+4;

    soap_send___wsdd__ProbeMatches(soap, "http://", NULL, probeMatches);
    return SOAP_OK;
}

int wsddService::ProbeMatches(struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int wsddService::Resolve(struct wsdd__ResolveType *wsdd__Resolve)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int wsddService::ResolveMatches(struct wsdd__ResolveMatchesType *wsdd__ResolveMatches)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int RemoteDiscoveryBindingService::Hello(const struct wsdd__HelloType& tdn__Hello, struct wsdd__ResolveType &tdn__HelloResponse)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int RemoteDiscoveryBindingService::Bye(const struct wsdd__ByeType& tdn__Bye, struct wsdd__ResolveType &tdn__ByeResponse)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

int RemoteDiscoveryBindingService::Probe(const struct wsdd__ProbeType& tdn__Probe, struct wsdd__ProbeMatchesType &tdn__ProbeResponse)  {
    DEBUG_MSG("Device: %s\n", __PRETTY_FUNCTION__);
    return SOAP_OK; 
}

