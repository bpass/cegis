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



/* This class contains a list of each road that has a point in each 
 * intersection.  These are stored in a list of pairs ordered by 
 * intersection point.  The first value in the pair is the road's index
 * in the layer, and the second is the index of the line point that 
 * touches the intersection.
 */
class IntersectionMap
{
   // Typedef'd pair to Indexes because std::vector<std::pair<int, int>> 
   // wouldn't compile in the map declaration.
   typedef std::pair<int, int> Indexes;

   // Number of roads that must participate in an intersection before it
   // will be recognized as an intersection
   enum { INTERSECT_ROAD_LIMIT = 2 };

   // Road type widths in meters.  
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

   OGRSpatialReference *mpFromSR;
   OGRSpatialReference mToSR;
   OGRDataSource *pRoadsDS;
   GDALDataset *pRasterDS;
   OGRCoordinateTransformation *pTransform;

   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp> mIntersectMap;
   std::vector<ControlPoint> vControl; // For control points

   double madAffine[6];

   OGRLineString *getLine( int i, std::string &type );
   double roadWidth( std::string type );
   void fillTemplate( Template &T, const OGRPoint &intersection );
   double correlate( InMemRaster &rasta, Template &T, int X, int Y );
   void pickHighestCorrelation( std::vector<ControlPoint> &vNewVals );

public:
   IntersectionMap( OGRDataSource *pInDS, GDALDataset *pRasterIn );
   ~IntersectionMap();

   void findControlPoints( InMemRaster &rasta, Classifier &classify,
                           const double &tempSize, const double &rasterDim);
   void iterativelyFindControlPoints( InMemRaster &rasta, 
                           Classifier &classify, 
                           std::vector<Indexes> &size );
   void triangulate( DelauneyTriangulator *triangulator, OGRDataSource *pDS );
   void adjustToControl( DelauneyTriangulator *triangulator,
                         RubberSheetTransform *transformer,
                         Filter *filter,
                         const double &ratio,
                         OGRDataSource *pOutDS );
   void filter( Filter *F, const double &ratio );

   // From here down is basically useless crap that was just for testing

   void outputIntersections( OGRDataSource *pDS );
   void outputControlPoints( OGRDataSource *pDS );

   void Raster2Vector( double &x, double &y );
   bool Vector2Raster( double &x, double &y );

   void templateTest( int i, const char *szFilename );

}; // end class IntersectionMap


#endif // INTERSECTION_MAP_H
