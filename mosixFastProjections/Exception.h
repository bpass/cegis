#ifndef USGSMOSIX_EXCEPTION_H_
#define USGSMOSIX_EXCEPTION_H_ 

/******************************************************************************
 *
 * Programmer : Mark Schisler
 * Date       : Fri Mar 11 20:34:23 CST 2005
 * File       : Exception.h
 *
 * Purpose    : To be the interface for general types of exceptions for
 *              the MOSIX project. 
 *
 *****************************************************************************/

#include <string>

namespace USGSMosix 
{
    class Exception 
    {
        public:
            virtual ~Exception() {}
            virtual std::string toString() = 0;
    };
}

#endif 
