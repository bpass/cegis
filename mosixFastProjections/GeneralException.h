#ifndef USGSMOSIX_GENERALEXCEPTION_H_
#define USGSMOSIX_GENERALEXCEPTION_H_ 

/******************************************************************************
 *
 * Programmer : Mark Schisler
 * Date       : Fri Mar 11 20:34:23 CST 2005
 * File       : GeneralException.h
 *
 * Purpose    : To be the interface for general types of exceptions for
 *              the MOSIX project. 
 *
 *****************************************************************************/

#include <string>
#include "Exception.h"

namespace USGSMosix 
{
    class GeneralException : public Exception 
    {
        public:
            GeneralException(std::string descrip) : m_description(descrip) {}
            virtual ~GeneralException() {}
            virtual std::string toString() { return m_description; }
        private: 
            std::string m_description;
    };
}

#endif 
