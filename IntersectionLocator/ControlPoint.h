/**
 * @file ControlPoint.h
 * Purpose: This file contains the declaration of a struct to
 *          hold control points for processing.
 *
 * @author Ryan Stelzleni
 * @date 8-16-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: ControlPoint.h,v 1.3 2004/10/22 13:30:26 rstelzleni Exp $


#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "ogrsf_frmts.h"

/**
 * Struct to hold control points and to track their correlation value.
 * point is the control point, origPoint is a copy of the point from
 * the dataset (so can be used to find the intersection in the
 * intersection std::map) and correlation is the correlation value
 * that was generated when the point was identified.
 */
struct ControlPoint
{
   OGRPoint point;      //!< The new point that was identified
   OGRPoint origPoint;  //!< The original point from the data
   double correlation;  //!< The correlation value for the new point
};


#endif //CONTROLPOINT_H

