/*
 * File: RubberSheetTransform.h
 * Purpose: This file contains the declaration of an interface
 *          for rubber sheet transformations.  I did this this
 *          way to allow for several types of rubber sheeting to
 *          be defined, since there seems to be a lot of methods
 *          out there.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-20-04
 */


// Majic numbers for CVS
// $Id: RubberSheetTransform.h,v 1.2 2004/10/18 22:42:52 rstelzleni Exp $


#ifndef RUBBERSHEETTRANSFORM_H
#define RUBBERSHEETTRANSFORM_H

#pragma warning(disable: 4786)

#include <vector>
#include "ogrsf_frmts.h"

#include "DelauneyTriangulator.h"
#include "ControlPoint.h"




/*
 * The RubberSheetTransform is an interface to allow the definition
 * of several different methods of performing the rubber sheeting
 * transformation necessary to this algorithm.  There are many 
 * techniques for rubber sheeting so it seemed logical to make it
 * easy to implement a new one.
 *
 * void doTransformation(                                  
 *                        OGRDataSource *pOutDS, 
 *                        OGRDataSource *pInDS,
 *                        const Triangulation &triangles,
 *                        const std::vector<ControlPoint> &points 
 *                      ) = 0;
 * This is a virtual function that should be overloaded by 
 * concrete sub-classes to perform the rubber sheeting algorithm
 * in whatever manner they are intended to.  The data sources
 * should be already initialized when this function is called, 
 * the triangles should already contain a Triangulation and
 * the points should be a valid ControlPoint set.
 * On return the subclass should put its output into the
 * pOutDS.
 */


class RubberSheetTransform
{
public:
   virtual ~RubberSheetTransform() {}

   virtual void doTransformation( 
                                  OGRDataSource *pOutDS, 
                                  OGRDataSource *pInDS,
                                  const Triangulation &triangles,
                                  const std::vector<ControlPoint> &points 
                                ) = 0;
};


#endif //RUBBERSHEETTRANSFORM_H
