/**
 * @file Template.h
 * Purpose: This file contains the declaration of the Template 
 *          class.  This class contains a two dimensional array
 *          that represents a template of two intersecting roads.
 *
 * @author Ryan Stelzleni
 * @date 7-22-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Template.h,v 1.3 2004/11/02 01:20:46 rstelzleni Exp $


#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "ogrsf_frmts.h"

#include "Error.h"


/**
 *  This class holds a binary grid of values that represent roads coming
 *  into an intersection.  The idea is that you pass an OGRPoint to the
 *  constructor that represents the center of the intersection, then you
 *  pass it points two at a time to add line segments to it.
 *
 *  To change the pixel size of the template you have to change the 
 *  constant PIX_PER_METER in the class above.  This should probably
 *  be fixed.
 */
class Template
{
public:

   /**
   *  Creates an empty template.
   */
   Template();

   /**
    * Creates a template with xSize by ySize meters that is centered
    * on the georeferenced point center.  The number of pixels per
    * meter is determined by the member constant PIX_PER_METER.
    * @param xSize The width of the template in meters.
    * @param ySize The height of the template in meters.
    * @param center The intersection to center the template on.
    */
   Template( int xSize, int ySize, const OGRPoint &center );

   ~Template();
  
   /**
   * Adds a line segment between from and dest.  If from and dest are
   * off the template then they will be ignored.  Width is the width
   * of the road to be added in meters.  Road segments that are partially
   * on the template will be drawn partially.  Road segments that are
   * not on the template at all will not be drawn, but note that this
   * is not guaranteed to be any faster than actually drawing the
   * road.
   * @param from The starting point of the line.
   * @param dest The ending point of the line.
   * @param width The width of the line in meters.]
   */
   void addSegment( const OGRPoint &from, const OGRPoint &dest, 
                    double width );

   /**
   * Call this function using row major order. To get a sequential read
   * of the raster (across the top row, then the next row, etc) using
   * two nested loops the outer one should be the y values and
   * the inner one should be the x values.
   * returns: false on bad input ( x or y out of range ).
   * @param x The x coordinate to query.
   * @param y The y coordinate to query.
   */
   bool isRoad( int x, int y ) const;

   /**
    * Returns the width of the template in pixels.
    */
   int xSize() const { return mnXSize; }

   /**
    * Returns the height of the template in pixels.
    */
   int ySize() const { return mnYSize; }

   /**
    * Makes a text file called szFilename that contains a grid of 0's and
    * 1's.  1's represent roads.  Will Error_Print and return if the file 
    * can't be opened.  This text file is currently being made upside down.
    * @param szFilename The file to print to.  Don't forget the path.
    */
   void dumpToFile( const char *szFilename );

private:

   bool *mpbBuf;    //!< The buffer
   int mnXSize;     //!< The buffer's x dimension size in pixels
   int mnYSize;     //!< The buffer's y dimension size in pixels
   double mdXTrans; //!< The value that is subtracted from x coords passed in
   double mdYTrans; //!< The value that is subtracted from y coords passed in
   
   /// The number of pixels per meter of the raster you want.
   /// TODO: This should probably be a class variable.
   enum { PIX_PER_METER = 3 };

   /**
    * This function marks an area as road surrounding a point that has
    * been drawn into the line.  
    * @param x The x coordinate of a point on the line.
    * @param y The y coordinate of a point on the line.
    * @param width the width to draw in pixels.
    * @param slope The slope of the line being drawn.
    */
   void addWidth( int x, int y, int width, const double &slope );

   /**
    * This function is a helper.  Since I am drawing lines based on
    * slope I can't draw a vertical one (the slope is 1/0).  This
    * draws a vertical line.
    * @param fromX The x coordinate to start from.
    * @param fromY The y coordinate to start from.
    * @param destX The x coordinate to end at.
    * @param destY The y coordinate to end at.
    * @param width The width of the road in pixels.
    */
   void addVerticalLine( int fromX, int fromY, int destX, int destY,
                         int width );

};


#endif // TEMPLATE_H



