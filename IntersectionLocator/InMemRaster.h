/*
 * File: InMemRaster.h
 * Purpose: This file contains the declaration of the InMemRaster 
 *          class.  This class is intended to hold a Raster file
 *          in memory for processing.  Currently it only supports
 *          8 bit word RGB format files.
 *
 * Programmer: Ryan Stelzleni
 * Date: 7-9-04
 */

// Warning: Each instance of this class for a 5000x5000 pixel
// tiff uses a ton of memory


// Majic numbers for CVS
// $Id: InMemRaster.h,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $


#ifndef IN_MEM_RASTER
#define IN_MEM_RASTER

#include <string.h>
#include <stdio.h>
#include "gdal_priv.h"

#include "Error.h"
#include "Classifier.h"

// Forward declaration because of cyclic includes
class Classifier;



/* 
 *  For a detailed description of the functions of this class,
 *  see the end of this file.
 */

class InMemRaster
{

public:

   InMemRaster( GDALDataset *pRaster );
   ~InMemRaster();

   void dumpToFile( const char *filename );
   void dumpToBinary( const char *szFilename ); // for classified roads

   void convertToHSV();
   void classify( const Classifier &C );
   void smooth();
   void smoothFive();

   void raster2Vector( double &x, double &y ) const;
   bool vector2Raster( double &x, double &y ) const;

   int   getHue( int x, int y ) const;
   int   getSat( int x, int y ) const;
   int   getVal( int x, int y ) const;

   int   getHue( double x, double y ) const;
   int   getSat( double x, double y ) const;
   int   getVal( double x, double y ) const;

   int getXSize() const;
   int getYSize() const;

   bool isRoad( int x, int y ) const;
   bool isRoad( double x, double y ) const;

private:

   int   *mpadHue;
   int   *mpadSat;
   int   *mpadVal;

   int mnXSize, mnYSize;

   double madAffine[6];
   char *mpszProjectionWkt;

   void readBand( GDALRasterBand *pBand, int *pDestBuff,
                            unsigned char *pBuff, 
                            int nYBlocks, int nXBlocks,
                            int nXBlockSize, int nYBlockSize );
   void RGB2HSV( int &red, int &green, int &blue );
   
};



#endif // IN_MEM_RASTER


/*************************************************************************
*  Class InMemRaster
*
*  Description:
*  This class will read in a raster from a GDALDataset that is already 
*  open.  This dataset must be passed in to the constructor.  Once the 
*  file is open you can manipulate it in various ways, with the end 
*  result being an image that is classified as road and non-road pixels.
*  
*    InMemRaster( GDALDataset *pRaster );
*  This is the class' constructor.  After passing in a GDALDataset that
*  already has an open file the class instance will contain three buffers
*  which will hold the RGB values for the file.  Note that the constructor
*  is limited in that it will only open GeoTiffs that have been created 
*  with blocks that have an X size of one.  Also note that this 
*  constructor can take quite a while.
*
*    ~InMemRaster();
*  The destructor, this frees the allocated memory in the class instance.
*
*    void dumpToFile( const char *filename );
*  This function is mostly for debugging purposes, it dumps the contents
*  of the class' internal buffers into a binary formatted file.  If the
*  class contains RGB values then it will dump all red values, then green
*  and then blue.  If HSV values it will dumps H's, S's and then V's.
*  The values are dumped out in row major order from the top left.  The
*  values are integer sized.
*  Post: The file called filename contains the output or the function
*        will Error_Print and return without doing anything.
*  
*    void dumpToBinary( const char *szFilename ); // for classified roads
*  Dumps out 1's or 0's in a binary file that are character sized.  1's
*  will represent pixels that have been classified as roads.  Outputs in
*  row major order from the top left in a single band.  
*  Post: The file called szFilename contains the output or the function
*        will Error_Print and return without doing anything.
*
*    void convertToHSV();
*  This function converts the entire image from RGB to HSV values.  This
*  should be done before using a classifier that has been trained on HSV
*  type values.  This function can take a while.
*
*    void classify( const Classifier &C );
*  The classifier passed into this function should already be trained.
*  Each pixel in the raster will be classified and marked if it is a road.
*  Note that road pixels won't contain color information after this
*  function has run.
*
*    void smooth();
*  This function is provided in case it improves the location of control
*  points.  It averages pixels based on whether or not the pixels that
*  are located up, down, left or right of it are road pixels.  Note that
*  this can result in a loss of color values for road pixels that 
*  are averaged to non-road pixels.
*
*    void smoothFive();
*  This function is the same as above, but it uses all pixels in a 5 pixel
*  wide area around the center to determine if the center is a road or 
*  not.
*
*    void raster2Vector( double &x, double &y ) const;
*    bool vector2Raster( double &x, double &y ) const;
*  These functions convert their arguments from raster coordinates to
*  vector coordinates or vice verse.  They use the affine transform
*  coordinates from the raster image.  In order to go from vector
*  to raster coordinates you have to invert the matrix that is used
*  for the raster2Vector function, in the case that this matrix has a
*  determinant of 0 (and hence can't be inverted) this function won't
*  change the x and y passed in and will return false.
*
*    int   getHue( int x, int y ) const;
*    int   getSat( int x, int y ) const;
*    int   getVal( int x, int y ) const;
*  These three functions return the Hue, Saturation and Value values at 
*  the location requested in raster coordinates.  If the image is still
*  an RGB image then Red corresponds to Hue, Green to Sat, and Blue to
*  Value.  If the image has been classified already then you may get bogus
*  values for road classified pixels.
*  These functions do not do any bound checking so be careful what you
*  pass as arguments.  x and y should be between 0 and getXSize or 
*  getYSize.
*
*    int   getHue( double x, double y ) const;
*    int   getSat( double x, double y ) const;
*    int   getVal( double x, double y ) const;
*  These do the same as the above functions but are provided for a 
*  convenience function.  If you pass in vector coordinates they will
*  be converted to raster internally and the correct values will be
*  returned.  This function doesn't bound check, but if the coordinates
*  can't be converted by the vector2Raster function above then this
*  function will return -100.
*
*    int getXSize() const;
*    int getYSize() const;
*  Returns the number of x pixels or y pixels in the raster.
*
*    bool isRoad( int x, int y ) const;
*    bool isRoad( double x, double y ) const;
*  Returns true if x, y is the location of a road pixel.  The integer 
*  version is for raster coordinates and the double version is for
*  vector coordinates.  This function does no boundary checking.
*
**************************************************************************/
