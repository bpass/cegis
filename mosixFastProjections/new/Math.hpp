
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file Math.hpp
 * 
 * \brief A collection of random, yet useful math functions. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

namespace USGSMosix 
{

/*****************************************************************************/

template<typename T>
inline bool Math<T>::inInclusiveRange( T val, T beginRange, T endRange )
    throw( std::domain_error )
{
    if ( beginRange < endRange ) return ( val>=beginRange && val<=endRange );
    else throw std::domain_error("begin and end range and incorrect");       
}

/*****************************************************************************/

template<typename T>
inline bool Math<T>::inExclusiveRange( T val, T beginRange, T endRange )
    throw( std::domain_error ) 
{
    if ( beginRange < endRange ) return ( val>beginRange && val<endRange );
    else throw std::domain_error("begin and end range and incorrect");
}

/*****************************************************************************/

template<typename T>
inline T Math<T>::abs(T val) throw()
{
    return ( val < 0 ) ? (-(val)):(val);
}

/*****************************************************************************/

template<typename T>
inline T Math<T>::max(T a, T b) throw()
{
    return ( a > b ) ? (a) : (b);
}

/*****************************************************************************/

template<typename T>
inline T Math<T>::min(T a, T b) throw()
{
    return ( a < b ) ? (a) : (b);
}

/*****************************************************************************/

template<typename T>
inline T Math<T>::floor(double a) throw()
{
    return static_cast<T>(static_cast<long int>(a));
}

/*****************************************************************************/

template<typename T>
inline T Math<T>::ceil(double b) throw()
{
    return static_cast<T>(static_cast<long int>(b + 0.5f));
}

/*****************************************************************************/

template<typename T>
inline double Math<T>::truncate(double a, double prec) throw(std::domain_error)
{
    if ( prec < 1 && prec > 0 ) return (floor(a/prec) * prec);
    else throw std::domain_error();
}

/*****************************************************************************/

template<typename T>
inline int Math<T>::truncate(int a, double prec) throw()
{
    (void)prec;
    return a;
}

/*****************************************************************************/

template<typename T> 
inline T Math<T>::getMin( const std::list<T>& l ) throw()
{
   T _min = *(l.begin());
   
   for( typename std::list<T>::const_iterator i = ++l.begin(); 
        i != l.end(); ++i)
        if ( *i < _min ) _min = *i;

   return _min;
}

/*****************************************************************************/

template<typename T> 
inline T Math<T>::getMax( const std::list<T>& l ) throw()
{
    T _max = *(l.begin());

    for( typename std::list<T>::const_iterator i = ++l.begin(); 
         i != l.end(); ++i) 
        if ( *i > _max ) _max = *i;

    return _max;
}

/*****************************************************************************/

} // namespace

