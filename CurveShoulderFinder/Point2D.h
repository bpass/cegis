/**
 * @file Point2D.h
 * @author Austin Hartman
 *
 * $Id: Point2D.h,v 1.3 2005/10/21 17:41:45 ahartman Exp $
 */

#ifndef AUSTIN_POINT2D_H
#define AUSTIN_POINT2D_H

#include <iostream>
#include <fstream>

template<class T>
class Point2D;

template<class T>
bool operator<(const Point2D<T>& lhs, const Point2D<T>& rhs);

template<class T>
std::ostream& operator<<(std::ostream& os, const Point2D<T>& p);

template<class T>
std::ofstream& operator<<(std::ofstream& ofs, const Point2D<T>& p);

template<class T>
std::istream& operator>>(std::istream& is, Point2D<T>& p);

template<class T>
class Point2D
{
public:
    Point2D();
    Point2D(const T& x, const T& y);

    T& x();
    const T& x() const;

    T& y();
    const T& y() const;

private:
    T m_x;
    T m_y;
};

#include "Point2D.hpp"

#endif

