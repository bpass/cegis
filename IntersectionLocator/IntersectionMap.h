/*
 * File: IntersectionMap.h
 * Purpose: This file contains the declaration of the IntersectionMap 
 *          class.  This class is used to extract intersections from
 *          a vector format file and relocate those intersections to
 *          match a raster format file.  The file formats must be
 *          supported by gdal.
 * Programmer: Ryan Stelzleni
 * Date: 7-7-04
 */


#ifndef INTERSECTION_MAP_H
#define INTERSECTION_MAP_H

// Stop MSVC from creating lots of unreadable warnings about the
// stl classes I used.
#pragma warning(disable: 4786)

#include <map>
#include <vector>
#include <functional>
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <stdio.h>
#include <math.h>  // For sqrt in correlate function

#include "Error.h"
#include "Template.h"
#include "InMemRaster.h"
#include "Classifier.h"
#include "DelauneyTriangulator.h"
#include "ControlPoint.h"
#include "SaalfeldRubberSheet.h"
#include "Filter.h"




/* Comparison type for OGRPoints.  
 * returns true if P1 is less than P2, which is if P1 has a lesser x
 *         value or, if the x's are equal, a lesser Y value.
 */
struct OGRPointComp : public std::binary_function<OGRPoint, OGRPoint, bool>
{
   bool operator() (const OGRPoint &P1, const OGRPoint &P2) const
   {
      bool ret = false;
      if( P1.getX() < P2.getX() )
         ret = true;
      else if( P1.getX() == P2.getX() )
      {
         if( P1.getY() < P2.getY() )
            ret = true;
      }
      return ret;
   }
};



/* 
 * IntersectionMap
 *
 * This class contains a list of each road that has a point in each 
 * intersection.  These are stored in a std::map of pairs ordered by 
 * intersection point.  The first value in the Indexes pair is the road's 
 * index in the layer, and the second is the index of the line point that 
 * touches the intersection.
 *
 * This is the class where most of the work takes place.  When passed
 * a vector and raster datasource in its constructor it will locate the
 * intersections to be corrected, and get some data about the raster 
 * image.  The next step is to filter the control points, then 
 * perform a triangulation, and finally adjust the control points.
 *
 *
 * PUBLIC FUNCTIONS
 *
 * IntersectionMap( OGRDataSource *pInDS, GDALDataset *pRasterIn )
 * The constructor for this class keeps a reference to pInDS and
 * pRasterIn, so don't delete these datasets until you are finished 
 * using the IntersectionMap you initialized with them.  This
 * function will do some analysis on both datasets and will extract
 * a set of intersection points from the pInDS.
 * Pre: pInDS and pRasterIn should be valid, open, overlapping datasets.
 * Post: The IntersectionMap will have the set of intersections stored
 *       and will have pointers to these datasets to be used for
 *       further analysis.
 *
 * void findControlPoints( InMemRaster &rasta,
 *                         const double &tempSize, 
 *                         const double &rasterDim )
 * This function locates control points to use when adjusting the 
 * intersections.  The function will attempt to locate a control point
 * for every intersection in the IntersectMap, so some of these control
 * points will be much better than others.  rasta is the InMemRaster
 * that you would like to use to look for intersections, it should be
 * already opened and classified as road or non-road.  tempSize is
 * the size in meters of the Template to be created, and rasterDim
 * is the size of the area over the rasta to move the template.
 * Pre: tempSize and rasterDim are nonnegative, rasta is classified
 *      as road and non-road.
 * Post: The IntersectionMap will have identified a control point
 *       for each intersection.  These are stored in the member
 *       variable vControl.
 *
 * void iterativelyFindControlPoints( InMemRaster &rasta, 
 *                         std::vector<Indexes> &size );
 * This function does the same as the above function but it performs
 * the template intersection matching for every pair of 
 * pair->first = template size in meters and
 * pair->second = area to move template over in meters.
 * After each size template is applied to the entire area the results
 * from that template are compared to the overall results so far.
 * if any control point has a higher normalized correlation value
 * then it will replace the corresponding control point in the
 * vControl member variable.
 * Pre: Each pair in size has no negative values, and the rasta is
 *      already classified as road or non-road.
 * Post: The vControl member variable is populated with a hopefully
 *       improved set of control points for the intersections.
 *
 * void triangulateAndAdjust( DelauneyTriangulator *Tri,
 *                            RubberSheetTransform *Tra,
 *                            OGRDataSource *pOutDS );
 * This function will perform the rubber sheeting process on the
 * pRoadsDS that was passed in in the constructor.  The output
 * will be saved in the pOutDS DataSource, which should be open 
 * and available for writing when passed in.  If either Tri or
 * Tra is null the function will return without doing anything.
 * Pre: Tri and Tra point to valid data and pOutDS is open for
 *      writing.  A findControlPoints function should have already
 *      been called as well.
 * Post: The data in pInDS from the constructor is rubber sheeted
 *       using the control points obtained previously with a
 *       findControlPoints function and the output is placed
 *       into pOutDS.
 *
 * void adjustToControl( DelauneyTriangulator *triangulator,
 *                       RubberSheetTransform *transformer,
 *                       Filter *filter,
 *                       const double &ratio,
 *                       OGRDataSource *pOutDS );
 * This function performs the whole process of creating the output
 * with the exception of finding control points.  The control
 * points will be filtered using filter with a ratio or ratio,
 * triangulated using triangulator and rubber sheeted using transformer.
 * If filter is NULL then no filtration is performed, if triangulator
 * or transformer is NULL then the function does nothing.
 * Pre: transformer and triangulator are not NULL, pOutDS is a valid
 *      pointer to an OGRDataSource open for writing, and ratio is
 *      in the range [0,1].  Also the IntersectionMap should contain
 *      some control points, so a findControlPoints function should
 *      have been called.
 * Post: The dataset pInDS from the constructor is adjusted and 
 *       saved in pOutDS.
 *
 * void filter( Filter *F, const double &ratio );
 * This function will filter the identified control points using
 * filter F with a ratio of ratio.  This is mostly so that you can
 * perform multiple filters if you would like to.
 * Pre: A findControlPoints function should have already been called.
 * Post: The control points in vControl have been filtered by the
 *       filter F.
 *
 * void outputIntersections( OGRDataSource *pDS );
 * void outputControlPoints( OGRDataSource *pDS );
 * These functions output the currently contained intersections and
 * control points, respectively to the writable datasources provided
 * in the pDS variables.
 * Pre: pDS is a writable OGRDataSource.
 * Post: The intersections or control points are dumped to that file.
 *
 * void triangulateAndOutput( DelauneyTriangulator *triangulator, 
 *                            OGRDataSource *pDS );
 * This function will perform a triangulation on the currently 
 * identified control points and then will output that 
 * triangulation as lines into pDS.  Note that the output isn't
 * very clean, and each triangle outputs its borders each time, so
 * there are many lines that are three lines stacked up.
 * Pre: pDS is a writable datasource.
 * Post: The triangulation is output to the datasource, note that
 *       the triandulation isn't saved in the IntersectionMap.
 *
 * void templateTest( int i, const char *szFilename );
 * This function creates a template for every control point currently
 * identified and outputs that template to a text file that is
 * a grid of ones and zeros, where each represents one pixel and
 * a one is a road and a zero isn't.
 * Pre: A findControlPoint function should have already been called.
 * Post: A lot of text files will be created, one for each intersection
 *       The text files will be named 
 *       szFilename + intersectionNumber + .txt
 *       Don't forget to include a path in szFilename
 */


class IntersectionMap
{
public:
   // Typedef'd pair to Indexes because std::vector<std::pair<int, int> > 
   // is kind of funny looking.
   typedef std::pair<int, int> Indexes;

   IntersectionMap( OGRDataSource *pInDS, GDALDataset *pRasterIn );
   ~IntersectionMap();

   void findControlPoints( InMemRaster &rasta,
                           const double &tempSize, 
                           const double &rasterDim);
   void iterativelyFindControlPoints( InMemRaster &rasta, 
                           std::vector<Indexes> &size );
   void triangulateAndAdjust( DelauneyTriangulator *Tri,
                              RubberSheetTransform *Tra,
                              OGRDataSource *pOutDS );
   void adjustToControl( DelauneyTriangulator *triangulator,
                         RubberSheetTransform *transformer,
                         Filter *filter,
                         const double &ratio,
                         OGRDataSource *pOutDS );
   void filter( Filter *F, const double &ratio );

   std::vector<ControlPoint> getControlPoints() const;
   void setControlPoints( const std::vector<ControlPoint> newPoints );

   // From here down is basically useless crap that was just for testing

   void outputIntersections( OGRDataSource *pDS );
   void outputControlPoints( OGRDataSource *pDS );

   void templateTest( int i, const char *szFilename );

   void triangulateAndOutput( DelauneyTriangulator *triangulator, 
                              OGRDataSource *pDS );

private:

   // Number of roads that must participate in an intersection before it
   // will be recognized as an intersection
   enum { INTERSECT_ROAD_LIMIT = 2 };

   // Road type widths in meters.
   // These are Tiger Cfcc code road types.
   // Note that if there is a divided road type in the overall type then
   // the divided type will just use half this value.  Also some road
   // types will have 0 width reguardless of the value set here, dirt paths
   // for instance.
   enum { A0SIZE = 5 };  // 5
   enum { A1SIZE = 25 }; //25
   enum { A2SIZE = 20 }; //20
   enum { A3SIZE = 15 }; //15
   enum { A4SIZE = 7 };  // 7
   enum { A5SIZE = 5 };  // 5
   enum { A6SIZE = 5 };  // 5
   enum { DEFAULT = 5 };

   OGRSpatialReference *mpFromSR;
   OGRSpatialReference mToSR;
   OGRDataSource *pRoadsDS;
   GDALDataset *pRasterDS;
   OGRCoordinateTransformation *pTransform;

   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp> mIntersectMap;
   std::vector<ControlPoint> vControl; // For control points

   // PRIVATE FUNCTIONS

   // Returns line i from the pRoadsDS's first layer.  type will be
   // the road's Cfcc code after the function returns.  Note that
   // you must delete the OGRLineString that is returned.
   OGRLineString *getLine( int i, std::string &type );

   // Returns the roadWidth in meters for a road of Cfcc type type.
   double roadWidth( std::string type );

   // This function will create a Template in T centered on intersection
   // and fill the template with any roads that participate in that
   // intersection.
   void fillTemplate( Template &T, const OGRPoint &intersection );

   // This function determines the correlation factor of Template T
   // overlaid on Raster rasta with its top left corner at rasta
   // coordinate (X, Y).
   double correlate( InMemRaster &rasta, Template &T, int X, int Y );

   // This function takes a vector of ControlPoints vNewVals that
   // contains points selected in a different manner than the
   // points currently in vControl.  It then normalizes and compares
   // the correlation values for each point and places the point
   // with the greater correlation in the vControl member variable.
   void pickHighestCorrelation( std::vector<ControlPoint> &vNewVals );


}; // end class IntersectionMap


#endif // INTERSECTION_MAP_H
