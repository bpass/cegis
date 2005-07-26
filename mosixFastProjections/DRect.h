#ifndef DRECT_H
#define DRECT_H

/***************************************************************************
                          DRect.h  -  description
                             -------------------
    begin                : Thu Oct 12 2000
    copyright            : (C) 2000 by Chris Bilderback
    email                : cbilderback@usgs.gov
 ***************************************************************************/

#include <iostream>

class DRect
{
public:
    //main constructor for the DRect class
    DRect() : left(0), right(0), top(0), bottom(0)
    {}
   
    // MS 5/26/2005: added for simplicity in construction
    DRect(double t, double r, double b, double l) : 
        left(l), right(r), top(t), bottom(b) {} 

    // MS 7/21/2005: added 
    friend std::ostream& operator<<(std::ostream& os, const DRect & r)
    {
        os  << "left   : " << r.left << std::endl
            << "right  : " << r.right << std::endl
            << "top    : " << r.top << std::endl
            << "bottom : " << r.bottom << std::endl;

        return os;
    }
    
    double left, right, top, bottom;
};

#endif // DRECT_H
