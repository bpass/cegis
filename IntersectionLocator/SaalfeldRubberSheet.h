/*
 * File: SaalfeldRubberSheet.h
 * Purpose: This file contains the declaration of a class that
 *          implements the RubberSheetTransform interface using
 *          the rubber sheeting algorithm described in Alan
 *          Saalfeld's paper "A Fast Rubber-Sheeting Transformation
 *          Using Simplicial Coordinates".
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-20-04
 */


#ifndef SAALFELDRUBBERSHEET_H
#define SAALFELDRUBBERSHEET_H


#include "RubberSheetTransform.h"
#include "Error.h"


// Helper struct for keeping track of each triangle's coefficients
// for point in polygon tests and for transformation.
// For a description see Saalfeld.
struct TriangleVal
{
   double A1, A2, B1, B2, C1, C2;
};



/*
 * The SaalfeldRubberSheet class performs rubber sheeting on an
 * already triangulated set of points using Saalfeld's method
 * of converting triangles by simplicial coordinates.
 *
 * void doTransformation( OGRDataSource *pOutDS, 
 *                        OGRDataSource *pInDS,
 *                        const Triangulation &triangles,
 *                        const std::vector<ControlPoint> &points );
 * This function performs the transformation.  
 * Pre: pOutDS is a data source that has already been set up, as 
 *      is pInDS.  triangles should already be filled by a Triangulator
 *      of some kind.  points should already contain a set of 
 *      ControlPoints  to use as the basis for the transformation.
 * Post: pOutDS will contain the vector roads from pInDS but transformed
 *       within the triangles from the triangles argument by
 *       transformations generated based on the ControlPoint's
 *       displacement from the original points.
 */

class SaalfeldRubberSheet : public RubberSheetTransform
{
public:
   virtual ~SaalfeldRubberSheet() {}

   virtual void doTransformation( OGRDataSource *pOutDS, 
                                  OGRDataSource *pInDS,
                                  const Triangulation &triangles,
                                  const std::vector<ControlPoint> &points );

private:
   std::vector<TriangleVal> val;

   void initVals( const Triangulation &triangles,
                  const std::vector<ControlPoint> &points ); 
};



#endif //SAALFELDRUBBERSHEET_H

