/**
 * @file SaalfeldRubberSheet.h
 * Purpose: This file contains the declaration of a class that
 *          implements the RubberSheetTransform interface using
 *          the rubber sheeting algorithm described in Alan
 *          Saalfeld's paper "A Fast Rubber-Sheeting Transformation
 *          Using Simplicial Coordinates".
 *
 * @author Ryan Stelzleni
 * @date 8-20-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: SaalfeldRubberSheet.h,v 1.4 2004/12/03 17:27:26 ahartman Exp $


#ifndef SAALFELDRUBBERSHEET_H
#define SAALFELDRUBBERSHEET_H


#include "RubberSheetTransform.h"
#include "Error.h"

/**
 * Helper struct for keeping track of each triangle's coefficients
 * for point in polygon tests and for transformation.
 * For a description see Saalfeld.
 */
struct TriangleVal
{
   double A1, A2, B1, B2, C1, C2;
};



/**
 * The SaalfeldRubberSheet class performs rubber sheeting on an
 * already triangulated set of points using Saalfeld's method
 * of converting triangles by simplicial coordinates.
 */

class SaalfeldRubberSheet : public RubberSheetTransform
{
public:
   virtual ~SaalfeldRubberSheet() {}

   /**
    * This function performs the transformation.  
    * @pre pOutDS is a data source that has already been set up, as 
    *      is pInDS.  triangles should already be filled by a Triangulator
    *      of some kind.  points should already contain a set of 
    *      ControlPoints  to use as the basis for the transformation.
    * @post pOutDS will contain the vector roads from pInDS but transformed
    *       within the triangles from the triangles argument by
    *       transformations generated based on the ControlPoint's
    *       displacement from the original points.
    * @param pOutDS The datasource to put the resulting dataset in.
    * @param pInDS The datasource to be transformed.
    * @param triangles The triangulation to use.
    * @param points The set of ControlPoints to use to correct the dataset.
    */
   virtual void doTransformation( OGRDataSource *pOutDS, 
                                  OGRDataSource *pInDS,
                                  const Triangulation &triangles,
                                  const std::vector<ControlPoint> &points );

private:
   /**
    * This vector contains the coefficients for the transformation for
    * each triangle.
    */
   std::vector<TriangleVal> val;

   /**
    * This function populates the val vector with the coefficients for
    * the transformation.  One set of values will be generated for each
    * triangle in triangles.
    * @param triangles The triangulation to generate values for.
    * @param points The ControlPoints that will be used to create the
    *        coefficients.
    */
   void initVals( const Triangulation &triangles,
                  const std::vector<ControlPoint> &points ); 
};



#endif //SAALFELDRUBBERSHEET_H

