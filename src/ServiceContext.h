#ifndef SERVICECONTEXT_H
#define SERVICECONTEXT_H


#include <string>
#include <vector>

#include "soapH.h"





class ServiceContext
{
    public:
        ServiceContext();


        int         port;
        std::string user;
        std::string password;


        //Device Information
        std::string manufacturer;
        std::string model;
        std::string firmware_version;
        std::string serial_number;
        std::string hardware_id;

        std::vector<std::string> scope;
};





#endif // SERVICECONTEXT_H
