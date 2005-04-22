/**
 * @file RealDouble.h
 * @author Austin Hartman
 * $Id: RealDouble.h,v 1.3 2005/04/22 16:51:01 ahartman Exp $
 */

#ifndef AUSTIN_REALDOUBLE_H
#define AUSTIN_REALDOUBLE_H

#include <cmath>

class RealDouble
{
public:
    RealDouble();

    template<class U>
    RealDouble(const U& u);

    operator double&();
    operator const double&() const;
    
    static const double& getEpsilon();
    static void setEpsilon(const double& newEpsilon);
    
    friend bool operator<(const RealDouble& lhs, const RealDouble& rhs);
    friend bool operator>(const RealDouble& lhs, const RealDouble& rhs);
    friend bool operator==(const RealDouble& lhs, const RealDouble& rhs);
    friend bool operator!=(const RealDouble& lhs, const RealDouble& rhs);
    friend bool operator<=(const RealDouble& lhs, const RealDouble& rhs);
    friend bool operator>=(const RealDouble& lhs, const RealDouble& rhs);

private:
    static double epsilon;
    double value_;
};

inline
RealDouble::RealDouble()
    : value_(0)
{}

template<class U>
inline
RealDouble::RealDouble(const U& u)
    : value_(u)
{}

inline
RealDouble::operator double&()
{
    return value_;
}

inline
RealDouble::operator const double&() const
{
    return value_;
}

inline
const double&
RealDouble::getEpsilon()
{
    return RealDouble::epsilon;
}

inline
void
RealDouble::setEpsilon(const double& newEpsilon)
{
    epsilon = std::abs(newEpsilon);
}

inline
bool
operator<(const RealDouble& lhs, const RealDouble& rhs)
{
    return lhs.value_ - rhs.value_ < -RealDouble::epsilon;    
}

inline
bool
operator>(const RealDouble& lhs, const RealDouble& rhs)
{
    return lhs.value_ - rhs.value_ > RealDouble::epsilon;
}

inline
bool 
operator==(const RealDouble& lhs, const RealDouble& rhs)
{
    return std::abs(lhs.value_ - rhs.value_) < RealDouble::epsilon;
}

inline
bool
operator!=(const RealDouble& lhs, const RealDouble& rhs)
{
    return !(lhs == rhs);
}

inline
bool
operator<=(const RealDouble& lhs, const RealDouble& rhs)
{
    return lhs.value_ - rhs.value_ < RealDouble::epsilon;
}

inline
bool
operator>=(const RealDouble& lhs, const RealDouble& rhs)
{
    return lhs.value_ - rhs.value_ > -RealDouble::epsilon;
}

#endif

