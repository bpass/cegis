/*
 * File: ControlPoint.h
 * Purpose: This file contains the declaration of a struct to
 *          hold control points for processing.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-16-04
 */


// Majic numbers for CVS
// $Id: ControlPoint.h,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $


#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "ogrsf_frmts.h"

/* Struct to hold control points and to track their correlation value
 * point is the control point, origPoint is a copy of the point from
 * the dataset (so can be used to find the intersection in the
 * intersection std::map) and correlation is the correlation value
 * that was generated when the point was identified.
 */
struct ControlPoint
{
   OGRPoint point, origPoint;
   double correlation;
};


#endif //CONTROLPOINT_H

