/*
 * File: Template.cpp
 * Purpose: This file contains the implementation of the Template 
 *          class.  This class contains a two dimensional array
 *          that represents a template of two intersecting roads.
 * Programmer: Ryan Stelzleni
 * Date: 7-22-04
 *
 * This code was written for the United States Geological Survey.
 */

// Notes:
// The raster is currently being built upside down.  The isRoad
// function takes this into account when you call it, so it isn't
// really a problem at the moment, but this is really kind of a
// ghetto fix.  
//
// another note, a 60x60 template looks really nice when converted
// in imagine, but a 20x20 could be just as useful to the algorithm.
// try both, and a few in between.



// Majic numbers for CVS
// $Id: Template.cpp,v 1.4 2004/11/23 20:50:30 rstelzleni Exp $


#include "Template.h"

Template::Template()
: mpbBuf( NULL ),
  mnXSize( 0 ),
  mnYSize( 0 )
{
}


// xSize and ySize should be in meters
Template::Template( int xSize, int ySize, const OGRPoint &center )
: mnXSize( xSize * PIX_PER_METER ),
  mnYSize( ySize * PIX_PER_METER )
{
   mpbBuf = new bool[ mnXSize * mnYSize ];

   for( int i=0; i<(mnXSize*mnYSize); ++i )
   {
      mpbBuf[i] = false;
   }

   // The center point minus half the size of the raster should work
   // as an offset for transforming projection coordinates onto the
   // raster.  I don't think my template images will ever be very big.
   // Currently I'm thinking 60 meters tops.
   mdXTrans = const_cast<OGRPoint&>(center).getX() - mnXSize/2;
   mdYTrans = const_cast<OGRPoint&>(center).getY() - mnYSize/2;

} // End Constructor


Template::~Template()
{
   if( mpbBuf )
   {
      delete [] mpbBuf;
   }
} // End Destructor


// Expects a width in meters
void Template::addSegment( const OGRPoint &from, const OGRPoint &dest, 
                           double width )
{
   int fromX, fromY, destX, destY;  // begin and end coordinates
   int x, y;                        // iterators
   int xSign, ySign, slopeSign;     // keep direction information
   double slopeCount;               // ghetto slope counter
   double slope;
   int iWidth = static_cast<int>( width * PIX_PER_METER );

   // Determine what direction the line is going.
   if( const_cast<OGRPoint&>(dest).getY() > const_cast<OGRPoint&>(from).getY() )
      ySign = 1;
   else
      ySign = -1;

   if( const_cast<OGRPoint&>(dest).getX() > const_cast<OGRPoint&>(from).getX() )
      xSign = 1;
   else
      xSign = -1;


   // Get starting and ending points in the raster coordinates.
   fromX = static_cast<int>(const_cast<OGRPoint&>(from).getX() - mdXTrans);
   fromY = static_cast<int>(const_cast<OGRPoint&>(from).getY() - mdYTrans);
   destX = static_cast<int>(const_cast<OGRPoint&>(dest).getX() - mdXTrans);
   destY = static_cast<int>(const_cast<OGRPoint&>(dest).getY() - mdYTrans);


   // Calculate the slope.  If the line is vertical then its 
   // slope is undefined so I need a different method.  For
   // that case I use the addVerticalLine function.
   if( const_cast<OGRPoint&>(dest).getX() - const_cast<OGRPoint&>(from).getX() != 0 )
   {
      slope = (const_cast<OGRPoint&>(dest).getY() - const_cast<OGRPoint&>(from).getY()) / 
              (const_cast<OGRPoint&>(dest).getX() - const_cast<OGRPoint&>(from).getX());
      if( slope >= 0 )
         slopeSign = 1;
      else
         slopeSign = -1;
   }
   else
   {
      addVerticalLine( fromX, fromY, destX, destY, iWidth );
      return;
   }


   // Start plotting the line
   x = fromX;
   y = fromY;
   if( x < mnXSize && x >= 0 && y < mnYSize && y >= 0 )
   {
      addWidth( x, y, iWidth, slope );
   }

   // Plot the line, basically this just counts out rise over
   // run like a fifth grader with some graph paper.
   slopeCount = 0;
   while( x != destX && y != destY )
   {
      if( slopeCount*slopeSign >= 1 )
      {
         y += ySign;
         if( x < mnXSize && y < mnYSize )
         {
            addWidth( x, y, iWidth, slope );
         }
         slopeCount -= slopeSign;
      }
      else
      {
         x += xSign;
         if( x < mnXSize && x >= 0 && y < mnYSize && y >= 0 )
         {
            addWidth( x, y, iWidth, slope );
         }
         slopeCount += slope;
      }
   }

   // make sure the end point didn't get skipped over
   if( destX < mnXSize && destX >= 0 && destY < mnYSize && destY >= 0 )
   {
      addWidth( destX, destY, iWidth, slope );
   }

   return;
} // End Function addSegment



// This function should be incapable of causing a seg fault, although
// I think that some of the calling functions might pass in some
// bad locations.
void Template::addWidth( int x, int y, int width, const double &slope )
{
   int i;
   int xOff, yOff;

   // If the line is more horizontal than vertical then 
   // its slope is between -1 and 1, or if it
   // is diagonal [( rise == run ) => slope == 1]
   // then the width will be added vertically
   // otherwise the width will be added horizontally
   if( slope >= -1 && slope <= 1 )
   {
      xOff = 0;
      yOff = 1;
   }
   else
   {
      xOff = 1;
      yOff = 0;
   }

   // Check to see if every offset location is within the buffer
   // and if so, add a road point.  Note that i is an int so odd
   // widths will be rounded to one less than half and will lose
   // one pixel.
   for( i = -(width/2); i<(width/2); ++i )
   {
      // This check ensures that neither x nor y is out of bounds,
      // it also checks to make sure that the x's don't wrap.
      if( (x + xOff*i) < mnXSize && (x + xOff*i) >= 0 &&
          (y + yOff*i) < mnYSize && (y + yOff*i) >= 0 )
      {
         mpbBuf[ (x + xOff*i) + (y + yOff*i)*mnXSize ] = true;
      }
   }

   return;
} // End Function addWidth



// This function expects an integer width, not a meters width
void Template::addVerticalLine( int fromX, int fromY, 
                                int destX, int destY, int width )
{
   int ySign;
   int x = fromX, y = fromY;

   if( destY > fromY )
      ySign = 1;
   else
      ySign = -1;

   while( ySign*y <= destY )
   {
      if( x > 0 && x < mnXSize && y > 0 && y < mnYSize )
      {
         // the slope is 2 so that it will be considered more
         // vertical than horizontal.
         addWidth( x, y, width, 2 );
      }
      y += ySign;
   }

   if( destX > 0 && destX < mnXSize && destY > 0 && destY < mnYSize )
   {
      addWidth( destX, destY, width, 2 );
   }

   return;
} // End Function addVerticalLine



bool Template::isRoad( int x, int y ) const
{
   if( x < 0 || y < 0 || x >= mnXSize || y >= mnYSize )
   {
      return false;
   }

   // reverse the y coordinates of the values returned
   return mpbBuf[ x + (mnYSize-y-1)*mnXSize ];
} // end function isRoad


void Template::dumpToFile( const char *szFilename )
{
   FILE *outfile = fopen( szFilename, "wt" );

   if( !outfile )
   {
      Error_Print( "Failed opening output file for Template\n" );
      return;
   }

   for( int i=0; i<mnXSize*mnYSize; ++i )
   {
      if( i != 0 && i % mnXSize == 0 )
         fprintf( outfile, "\n" );
      fprintf( outfile, "%i ", mpbBuf[i] );
   }

   fclose( outfile );
   return;
} // End Function dumpToFile



