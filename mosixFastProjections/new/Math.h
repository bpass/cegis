
#ifndef _USGSMOSIX_MATH_H_
#define _USGSMOSIX_MATH_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file Math.h
 * 
 * \brief A collection of random, yet useful math functions that 
 * are useful to have around.  
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include<list>
#include<stdexcept>
#include<exception>

namespace USGSMosix {
    
    template <typename T>
    class Math {
        
        public:
        
        /// \brief Returns the absolute value of val.
        static T abs(T val) throw();

        /// \brief Returns the maximum value of a and b using the 
        /// less than operator.
        static T max(T a, T b) throw();

        /// \brief Returns the minumum value of a and b using the 
        /// less than operator.
        static T min(T a, T b) throw();

        /// \brief Returns the floor value of a.
        static T floor(double a) throw();

        /// \brief returns the ceiling of b.
        static T ceil(double b) throw();
        
        /// \param val The test value.
        /// \param beginRange The beginning of the inclusive range.
        /// \param endRange The end of the inclusive range.
        /// \brief Returns true if (beingRange <= val <= endRange). 
        static bool inInclusiveRange( T val, T beginRange, T endRange )
            throw( std::domain_error );
        
        /// \param val The test value.
        /// \param beginRange The beginning of the exclusive range.
        /// \param endRange The end of the exclusive range.
        /// \brief Returns true if (beginRange < val < endRange).
        static bool inExclusiveRange( T val, T beginRange, T endRange )
            throw( std::domain_error );
        
        /// \param a A value. 
        /// \param prec The precision to which we would like to save value a.
        /// This value must be between 0 and 1.
        /// \brief This function truncates a floating pointer value at a 
        /// certain precision.  For instance, if prec = 0.01 and a = 2.096, 
        /// then the value returned will be 2.093.
        static double truncate(double a, double prec) throw(std::domain_error);
        static int    truncate(int    a, double prec) throw();
       
        /// \param l A list of values.
        /// \brief Returns the least value in the list of values.  Uses
        /// the operator< to determine which is lesser than which.
        static T getMin( const std::list<T>& l ) throw();

        /// \param l A list of values.
        /// \brief Returns the maximum value in the list of values.  Uses
        /// the operator> to determine which is more than which.
        static T getMax( const std::list<T>& l ) throw();
    };
}

#include "Math.hpp"

#endif // _USGSMOSIX_MATH_H_

