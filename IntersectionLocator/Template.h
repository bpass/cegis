/*
 * File: Template.h
 * Purpose: This file contains the declaration of the Template 
 *          class.  This class contains a two dimensional array
 *          that represents a template of two intersecting roads.
 *
 * Programmer: Ryan Stelzleni
 * Date: 7-22-04
 */


// Majic numbers for CVS
// $Id: Template.h,v 1.2 2004/10/18 22:42:52 rstelzleni Exp $


#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "ogrsf_frmts.h"

#include "Error.h"


/*************************************************************************
*  Class Template
*
*  Description:
*  This class holds a binary grid of values that represent roads coming
*  into an intersection.  The idea is that you pass an OGRPoint to the
*  constructor that represents the center of the intersection, then you
*  pass it points two at a time to add line segments to it.
*
*  To change the pixel size of the template you have to change the 
*  constant PIX_PER_METER in the class above.  This should probably
*  be fixed.
*
*
*    Template();
*  Creates an empty template.
*
*    Template( int xSize, int ySize, const OGRPoint &center );
*  Creates a template with xSize by ySize meters that is centered
*  on the georeferenced point center.  The number of pixels per
*  meter is determined by the member constant PIX_PER_METER.
*
*    void addSegment( const OGRPoint &from, const OGRPoint &dest, 
*                     double width );
*  Adds a line segment between from and dest.  If from and dest are
*  off the template then they will be ignored.  Width is the width
*  of the road to be added in meters.  Road segments that are partially
*  on the template will be drawn partially.  Road segments that are
*  not on the template at all will not be drawn, but note that this
*  is not guaranteed to be any faster than actually drawing the
*  road.
*
*    bool isRoad( int x, int y ) const;
*  Call this function using row major order. To get a sequential read
*  of the raster (across the top row, then the next row, etc) using
*  two nested loops the outer one should be the y values and
*  the inner one should be the x values.
*  returns: false on bad input ( x or y out of range ).
*
*    int xSize() const
*    int ySize() const
*  Returns the size of the template in pixels.
*
*    void dumpToFile( const char *szFilename );
*  Makes a text file called szFilename that contains a grid of 0's and
*  1's.  1's represent roads.  Will Error_Print and return if the file 
*  can't be opened.  This text file is currently being made upside down.
*
**************************************************************************/

class Template
{
public:

   Template();
   Template( int xSize, int ySize, const OGRPoint &center );

   ~Template();
  
   void addSegment( const OGRPoint &from, const OGRPoint &dest, 
                    double width );

   bool isRoad( int x, int y ) const;

   int xSize() const { return mnXSize; }
   int ySize() const { return mnYSize; }

   void dumpToFile( const char *szFilename );

private:

   bool *mpbBuf;     // The buffer
   int mnXSize;      // The buffer's x dimension size in pixels
   int mnYSize;      // The buffer's y dimension size in pixels
   double mdXTrans;  // The value that is subtracted from x coords passed in
   double mdYTrans;  // The value that is subtracted from y coords passed in
   
   // The number of pixels per meter of the raster you want.
   // TODO: This should probably be a class variable.
   enum { PIX_PER_METER = 3 };

   void addWidth( int x, int y, int width, const double &slope );
   void addVerticalLine( int fromX, int fromY, int destX, int destY,
                         int width );


};


#endif // TEMPLATE_H



