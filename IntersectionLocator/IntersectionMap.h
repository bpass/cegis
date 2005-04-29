/**
 * @file IntersectionMap.h
 * Purpose: This file contains the declaration of the IntersectionMap 
 *          class.  This class is used to extract intersections from
 *          a vector format file and relocate those intersections to
 *          match a raster format file.  The file formats must be
 *          supported by gdal.
 * @author Ryan Stelzleni
 * @date 7-7-04
 *
 * This code was written for the United States Geological Survey.
 */



// Majic numbers for CVS
// $Id: IntersectionMap.h,v 1.7 2005/04/29 16:27:36 ahartman Exp $


#ifndef INTERSECTION_MAP_H
#define INTERSECTION_MAP_H

// Stop MSVC from creating lots of unreadable warnings about the
// stl classes I used.
#ifdef _MSC_VER
#if _MSC_VER < 1300
#pragma warning(disable: 4786)
#endif
#endif

#include <map>
#include <vector>
#include <string>
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




/**
 * Comparison type for OGRPoints.  
 */
struct OGRPointComp : public std::binary_function<OGRPoint, OGRPoint, bool>
{
   /**
     * returns true if P1 is less than P2, which is if P1 has a lesser x
     *         value or, if the x's are equal, a lesser Y value.
     */
   bool operator() (const OGRPoint &P1, const OGRPoint &P2) const
   {
      bool ret = false;
      if( const_cast<OGRPoint&>(P1).getX() < const_cast<OGRPoint&>(P2).getX() )
         ret = true;
      else if( const_cast<OGRPoint&>(P1).getX() == const_cast<OGRPoint&>(P2).getX() )
      {
         if( const_cast<OGRPoint&>(P1).getY() < const_cast<OGRPoint&>(P2).getY() )
            ret = true;
      }
      return ret;
   }
};


/** 
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
 */
class IntersectionMap
{
public:
   /// Typedef'd pair to Indexes because std::vector<std::pair<int, int> > 
   /// is kind of funny looking.
   typedef std::pair<int, int> Indexes;

   /**
    * The constructor for this class keeps a reference to pInDS and
    * pRasterIn, so don't delete these datasets until you are finished 
    * using the IntersectionMap you initialized with them.  This
    * function will do some analysis on both datasets and will extract
    * a set of intersection points from the pInDS.
    * @pre pInDS and pRasterIn should be valid, open, overlapping datasets.
    * @post The IntersectionMap will have the set of intersections stored
    *       and will have pointers to these datasets to be used for
    *       further analysis.
    * @param pInDS The vector formatted file that contains the road map.
    * @param pRasterIn The raster image to use to correct the vector roads.
    */
   IntersectionMap( OGRDataSource *pInDS, GDALDataset *pRasterIn );

   ~IntersectionMap();

   /**
    * This function locates control points to use when adjusting the 
    * intersections.  The function will attempt to locate a control point
    * for every intersection in the IntersectMap, so some of these control
    * points will be much better than others.  rasta is the InMemRaster
    * that you would like to use to look for intersections, it should be
    * already opened and classified as road or non-road.  tempSize is
    * the size in meters of the Template to be created, and rasterDim
    * is the size of the area over the rasta to move the template.
    * @pre tempSize and rasterDim are nonnegative, rasta is classified
    *      as road and non-road.
    * @post The IntersectionMap will have identified a control point
    *       for each intersection.  These are stored in the member
    *       variable vControl.
    * @param rasta A raster image that has been classified as road and
    *        non road
    * @param tempSize The size of the template to make from the vector
    *        data.  The template will be a square with each side's length
    *        equal to this parameter in meters.
    * @param rasterDim The length of a side of the square area that the
    *        template should be moved around on.
    */
   void findControlPoints( InMemRaster &rasta,
                           const double &tempSize, 
                           const double &rasterDim);

   /**
    * This function does the same as findControlPoints but it performs
    * the template intersection matching for every pair of 
    * pair->first = template size in meters and
    * pair->second = area to move template over in meters.
    * After each size template is applied to the entire area the results
    * from that template are compared to the overall results so far.
    * if any control point has a higher normalized correlation value
    * then it will replace the corresponding control point in the
    * vControl member variable.
    * @pre Each pair in size has no negative values, and the rasta is
    *      already classified as road or non-road.
    * @post The vControl member variable is populated with a hopefully
    *       improved set of control points for the intersections.
    * @param rasta The image to use for correction.
    * @param size A vector of pairs of integers. The first integer in
    *        each pair will be used as the size of a template and the
    *        second integer will be the area to move the template over.
    *        These sizes will be used to identify one corrected 
    *        intersection for each intersection in the original data,
    *        and after the function has run only the best intersections
    *        found will have been kept in the vControl vector.
    */
   void iterativelyFindControlPoints( InMemRaster &rasta, 
                           std::vector<Indexes> &size );

   void addBoundingControlPoints();

   /**
    * This function will perform the rubber sheeting process on the
    * pRoadsDS that was passed in in the constructor.  The output
    * will be saved in the pOutDS DataSource, which should be open 
    * and available for writing when passed in.  If either Tri or
    * Tra is null the function will return without doing anything.
    * @pre Tri and Tra point to valid data and pOutDS is open for
    *      writing.  A findControlPoints function should have already
    *      been called as well.
    * @post The data in pInDS from the constructor is rubber sheeted
    *       using the control points obtained previously with a
    *       findControlPoints function and the output is placed
    *       into pOutDS.
    * @param Tri A pointer to a DelauneyTriangulator to use for
    *        creating a triangulation.
    * @param Tra A pointer to a RubberSheetTransform for transforming
    *        the data.
    * @param pOutDS A pointer to an open OGRDataSource that is 
    *        writable for containing the output of the function.
    */
   void triangulateAndAdjust( DelauneyTriangulator *Tri,
                              RubberSheetTransform *Tra,
                              OGRDataSource *pOutDS );

   /**
    * This function performs the whole process of creating the output
    * with the exception of finding control points.  The control
    * points will be filtered using filter with a ratio or ratio,
    * triangulated using triangulator and rubber sheeted using transformer.
    * If filter is NULL then no filtration is performed, if triangulator
    * or transformer is NULL then the function does nothing.
    * @pre transformer and triangulator are not NULL, pOutDS is a valid
    *      pointer to an OGRDataSource open for writing, and ratio is
    *      in the range [0,1].  Also the IntersectionMap should contain
    *      some control points, so a findControlPoints function should
    *      have been called.
    * @post The dataset pInDS from the constructor is adjusted and 
    *       saved in pOutDS.
    * @param triangulator A pointer to an object that can triangulate
    *        the data points.  Is triangulate a verb?
    * @param transformer A pointer to an object taht can transform
    *        the data.
    * @param filter A pointer to a filter that can filter out control
    *        points.
    * @param ratio The ratio of points to be removed by the filter.
    * @param pOutDS A pointer to an open and writable OGRDataSource
    *        to write the output to.
    */
   void adjustToControl( DelauneyTriangulator *triangulator,
                         RubberSheetTransform *transformer,
                         Filter *filter,
                         const double &ratio,
                         OGRDataSource *pOutDS );

   /**
    * This function will filter the identified control points using
    * filter F with a ratio of ratio.  This is mostly so that you can
    * perform multiple filters if you would like to.
    * @pre A findControlPoints function should have already been called.
    * @post The control points in vControl have been filtered by the
    *       filter F.
    * @param F A filter to use on the control points.
    * @param ratio The ratio of points to filter out.
    */
   void filter( Filter *F, const double &ratio );

   /**
    * This function returns a copy of vControl.  This is useful
    * if you want to save the state of the intersection map
    * after identifying control points.
    * @return A copy of vControl.
    */
   std::vector<ControlPoint> getControlPoints() const;

   /** 
    * This is the inverse of getControlPoints, it sets the vControl
    * vector from newPoints.  This is useful to restore the state of
    * the IntersectionMap after identifying points.
    * @param newPoints The set of points to copy into vControl.
    */
   void setControlPoints( const std::vector<ControlPoint> newPoints );


   // From here down is basically useless crap that was just for testing

   /**
    * Outputs the currently contained intersections to the writable 
    * datasource provided in the pDS variables.
    * @pre pDS is a writable OGRDataSource.
    * @post The intersections are dumped to pDS.
    * @param pDS An open and writable OGRDataSource for the output.
    */
   void outputIntersections( OGRDataSource *pDS );

   /**
    * Outputs the currently contained control points to the writable 
    * datasource provided in the pDS variables.
    * @pre pDS is a writable OGRDataSource.
    * @post The control points are dumped to pDS.
    * @param pDS An open and writable OGRDataSource for the output.
    */
   void outputControlPoints( OGRDataSource *pDS );


   /**
    * This function creates a template for every control point currently
    * identified and outputs that template to a text file that is
    * a grid of ones and zeros, where each represents one pixel and
    * a one is a road and a zero isn't.
    * @pre A findControlPoint function should have already been called.
    * @post A lot of text files will be created, one for each intersection
    *       The text files will be named 
    *       szFilename + intersectionNumber + .txt
    *       Don't forget to include a path in szFilename
    * @param i Doesn't do anything. (This function is a hack for testing
    *        purposes)
    * @param szFilename The base file name for the output text files.
    *        This should really include a path so that you know where
    *        your output is going.
    */
   void templateTest( int i, const char *szFilename );

   /**
    * This function will perform a triangulation on the currently 
    * identified control points and then will output that 
    * triangulation as lines into pDS.  Note that the output isn't
    * very clean, and each triangle outputs its borders each time, so
    * there are many lines that are three lines stacked up.
    * @pre pDS is a writable datasource.
    * @post The triangulation is output to the datasource, note that
    *       the triandulation isn't saved in the IntersectionMap.
    * @param triangulator An object that can triangulate the dataset.
    * @param pDS An open and writable OGRDataSource to contain the
    *        output.
    */
   void triangulateAndOutput( DelauneyTriangulator *triangulator, 
                              OGRDataSource *pDS );

private:

   /// Number of roads that must participate in an intersection before it
   /// will be recognized as an intersection
   enum { INTERSECT_ROAD_LIMIT = 2 };

   /** @defgroup roadtypes Road Type Widths
    * Road type widths in meters.
    * These are Tiger Cfcc code road types.  For instance, an A01 will 
    * be assumed to have width A0SIZE.
    * Note that if there is a divided road type in the overall type then
    * the divided type will just use half this value.  Also some road
    * types will have 0 width reguardless of the value set here, dirt paths
    * for instance.
    */
   //@{
   enum { A0SIZE = 5 };  // 5
   enum { A1SIZE = 25 }; //25
   enum { A2SIZE = 20 }; //20
   enum { A3SIZE = 15 }; //15
   enum { A4SIZE = 7 };  // 7
   enum { A5SIZE = 5 };  // 5
   enum { A6SIZE = 5 };  // 5
   enum { DEFAULT = 5 };
   //@}

   OGRSpatialReference *mpFromSR; //!< The spatial reference of the input
   OGRSpatialReference mToSR;     //!< The spatial reference of the raster
   OGRDataSource *pRoadsDS;       //!< The vector roads dataset
   GDALDataset *pRasterDS;        //!< The raster image file
   /// A transform for converting from the vector coordinate system to
   /// the raster.
   OGRCoordinateTransformation *pTransform;

   /// A std::map containing all of the points in the input road map.  
   /// Any point with more than INTERSECT_ROAD_LIMIT lines containing
   /// it is considered an intersection.
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp> mIntersectMap;
   std::vector<ControlPoint> vControl; //!< For control points

   // PRIVATE FUNCTIONS

   /** Returns line i from the pRoadsDS's first layer.  type will be
    * the road's Cfcc code after the function returns.  Note that
    * you must delete the OGRLineString that is returned.
    * @param i The index of the line in the dataset.
    * @param type A string representing the type of road as a Cfcc code.
    */
   OGRLineString *getLine( int i, std::string &type );

   /** Returns the roadWidth in meters for a road of Cfcc type type.
    *  @param type The Cfcc code for the road type.
    */
   double roadWidth( std::string type );

   /** This function will create a Template in T centered on intersection
    * and fill the template with any roads that participate in that
    * intersection.
    * @param T An empty template to fill with an image.
    * @param intersection The intersection point that will be the
    *        center of the template.
    */
   void fillTemplate( Template &T, const OGRPoint &intersection );

   /** This function determines the correlation factor of Template T
    * overlaid on Raster rasta with its top left corner at rasta
    * coordinate (X, Y).
    * @param rasta The raster image to correlate the template with.
    * @param T The template to compare to the raster.
    * @param X The upper left starting point's x coordinate for the
    *        template's location on the raster.
    * @param Y The upper left starting point's y coordinate for the
    *        template's location on the raster.
    */
   double correlate( InMemRaster &rasta, Template &T, int X, int Y );

   /** This function takes a vector of ControlPoints vNewVals that
    * contains points selected in a different manner than the
    * points currently in vControl.  It then normalizes and compares
    * the correlation values for each point and places the point
    * with the greater correlation in the vControl member variable.
    * @param vNewVals The vector of values to compare against the
    *        vControl vector.
    */
   void pickHighestCorrelation( std::vector<ControlPoint> &vNewVals );


}; // end class IntersectionMap


#endif // INTERSECTION_MAP_H
