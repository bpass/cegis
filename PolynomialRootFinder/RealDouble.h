/**
 * @file RealDouble.h
 * @author Austin Hartman
 * $Id: RealDouble.h,v 1.2 2005/04/22 16:44:31 ahartman Exp $
 */

#ifndef AUSTIN_REALDOUBLE_H
#define AUSTIN_REALDOUBLE_H

#include <cmath>

class RealDouble
{
public:
    RealDouble();
    RealDouble(const double& d);
    RealDouble(const float& f);
    RealDouble(const int& i);
    RealDouble(const unsigned& u);

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

inline
RealDouble::RealDouble(const double& d)
    : value_(d)
{}

inline
RealDouble::RealDouble(const float& f)
    : value_(f)
{}

inline
RealDouble::RealDouble(const int& i)
    : value_(i)
{}

inline
RealDouble::RealDouble(const unsigned& u)
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

