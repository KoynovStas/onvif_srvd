#ifndef STOOLS_H
#define STOOLS_H

#include <memory>
#include <limits>

#include "soapH.h"





template<typename T>
T* soap_new_ptr(struct soap* soap, T value)
{
    T* ptr = (T*)soap_malloc(soap, sizeof(T));
    if(ptr)
        *ptr = value;

    return ptr;
}



inline std::string* soap_new_std_string(struct soap* soap, const std::string& value)
{
    auto str = soap_new_std__string(soap);
    if(str)
        *str = value;

    return str;
}



inline std::string* soap_new_std_string(struct soap* soap, const char* value)
{
    auto str = soap_new_std__string(soap);
    if(str)
        *str = value;

    return str;
}







#endif // STOOLS_H
