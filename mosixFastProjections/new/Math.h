
#ifndef _USGSMOSIX_MATH_H_
#define _USGSMOSIX_MATH_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file Math.h
 * 
 * \brief The ProjImage object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include<list>
#include<stdexcept>
#include<exception>

namespace USGSMosix 
{
    template <typename T>
    class Math
    {
        public:
            
        static T abs(T val) throw();
        static T max(T a, T b) throw();
        static T min(T a, T b) throw();
        static T floor(double a) throw();
        static T ceil(double b) throw();
        
        static bool inInclusiveRange( T val, T beginRange, T endRange )
            throw( std::domain_error );
        
        static bool inExclusiveRange( T val, T beginRange, T endRange )
            throw( std::domain_error );
        
        static double truncate(double a, double prec) throw(std::domain_error);
        static int    truncate(int    a, double prec) throw();
        
        static T getMin( const std::list<T>& l ) throw();
        static T getMax( const std::list<T>& l ) throw();
    };
}

#include "Math.hpp"

#endif // _USGSMOSIX_MATH_H_

