/**
 * @file InMemRaster.h
 * Purpose: This file contains the declaration of the InMemRaster 
 *          class.  This class is intended to hold a Raster file
 *          in memory for processing.  Currently it only supports
 *          8 bit word RGB format files.
 *
 * @author Ryan Stelzleni
 * @date 7-9-04
 *
 * This code was written for the United States Geological Survey.
 */




// Majic numbers for CVS
// $Id: InMemRaster.h,v 1.4 2004/11/02 01:20:46 rstelzleni Exp $


#ifndef IN_MEM_RASTER
#define IN_MEM_RASTER

#include <string.h>
#include <stdio.h>
#include "gdal_priv.h"

#include "Error.h"
#include "Classifier.h"

// Forward declaration because of cyclic includes
class Classifier;



/** 
 *  This class will read in a raster from a GDALDataset that is already 
 *  open.  This dataset must be passed in to the constructor.  Once the 
 *  file is open you can manipulate it in various ways, with the end 
 *  result being an image that is classified as road and non-road pixels.
 *  <BR><BR>
 *  <B>Warning:</B> Each instance of this class for a 5000x5000 pixel
 *  tiff uses a ton of memory
 */

class InMemRaster
{

public:

   /**
    *  This is the class' constructor.  After passing in a GDALDataset that
    *  already has an open file the class instance will contain three buffers
    *  which will hold the RGB values for the file.  Note that the constructor
    *  is limited in that it will only open GeoTiffs that have been created 
    *  with blocks that have an X size of one.  Also note that this 
    *  constructor can take quite a while.
    * @param pRaster A GDALDataset to initialize the raster with.
    */
   InMemRaster( GDALDataset *pRaster );
   ~InMemRaster();

   /**
    *  This function is mostly for debugging purposes, it dumps the contents
    *  of the class' internal buffers into a binary formatted file.  If the
    *  class contains RGB values then it will dump all red values, then green
    *  and then blue.  If HSV values it will dumps H's, S's and then V's.
    *  The values are dumped out in row major order from the top left.  The
    *  values are integer sized.
    *  Post: The file called filename contains the output or the function
    *        will Error_Print and return without doing anything.
    *  @param filename A string for the name of the output file, including
    *         the path.
    */  
   void dumpToFile( const char *filename );

   /**
    *  Dumps out 1's or 0's in a binary file that are character sized.  1's
    *  will represent pixels that have been classified as roads.  Outputs in
    *  row major order from the top left in a single band.  
    *  Post: The file called szFilename contains the output or the function
    *        will Error_Print and return without doing anything.
    *  @param szFilename A string for the name of the output file.  Should
    *         include the path.
    */
   void dumpToBinary( const char *szFilename ); // for classified roads

   /**
    *  This function converts the entire image from RGB to HSV values.  This
    *  should be done before using a classifier that has been trained on HSV
    *  type values.  This function can take a while.
    */
   void convertToHSV();

   /**
    *  The classifier passed into this function should already be trained.
    *  Each pixel in the raster will be classified and marked if it is a road.
    *  Note that road pixels won't contain color information after this
    *  function has run.
    *  @param C An already initialized and trained Classifier.
    */
   void classify( const Classifier &C );

   /**
    *  This function is provided in case it improves the location of control
    *  points.  It averages pixels based on whether or not the pixels that
    *  are located up, down, left or right of it are road pixels.  Note that
    *  this can result in a loss of color values for road pixels that 
    *  are averaged to non-road pixels.
    */
   void smooth();

   /**
    *  This function is the same as above, but it uses all pixels in a 5 pixel
    *  wide area around the center to determine if the center is a road or 
    *  not.
    */
   void smoothFive();

   /**
    * Function for converting coordinates.
    *    void raster2Vector( double &x, double &y ) const;<BR>
    *    bool vector2Raster( double &x, double &y ) const;<BR>
    *  These functions convert their arguments from raster coordinates to
    *  vector coordinates or vice verse.  They use the affine transform
    *  coordinates from the raster image.  In order to go from vector
    *  to raster coordinates you have to invert the matrix that is used
    *  for the raster2Vector function, in the case that this matrix has a
    *  determinant of 0 (and hence can't be inverted) this function won't
    *  change the x and y passed in and will return false.
    */
   void raster2Vector( double &x, double &y ) const;
   /**
    * Function for converting coordinates.
    *    void raster2Vector( double &x, double &y ) const;<BR>
    *    bool vector2Raster( double &x, double &y ) const;<BR>
    *  These functions convert their arguments from raster coordinates to
    *  vector coordinates or vice verse.  They use the affine transform
    *  coordinates from the raster image.  In order to go from vector
    *  to raster coordinates you have to invert the matrix that is used
    *  for the raster2Vector function, in the case that this matrix has a
    *  determinant of 0 (and hence can't be inverted) this function won't
    *  change the x and y passed in and will return false.
    */
   bool vector2Raster( double &x, double &y ) const;



   /**
    *  Function for getting hsv values.
    *    int   getHue( int x, int y ) const;<BR>
    *    int   getSat( int x, int y ) const;<BR>
    *    int   getVal( int x, int y ) const;<BR>
    *  These three functions return the Hue, Saturation and Value values at 
    *  the location requested in raster coordinates.  If the image is still
    *  an RGB image then Red corresponds to Hue, Green to Sat, and Blue to
    *  Value.  If the image has been classified already then you may get bogus
    *  values for road classified pixels.
    *  These functions do not do any bound checking so be careful what you
    *  pass as arguments.  x and y should be between 0 and getXSize or 
    *  getYSize.
    */
   int   getHue( int x, int y ) const;
   /**
    *  Function for getting hsv values.
    *    int   getHue( int x, int y ) const;<BR>
    *    int   getSat( int x, int y ) const;<BR>
    *    int   getVal( int x, int y ) const;<BR>
    *  These three functions return the Hue, Saturation and Value values at 
    *  the location requested in raster coordinates.  If the image is still
    *  an RGB image then Red corresponds to Hue, Green to Sat, and Blue to
    *  Value.  If the image has been classified already then you may get bogus
    *  values for road classified pixels.
    *  These functions do not do any bound checking so be careful what you
    *  pass as arguments.  x and y should be between 0 and getXSize or 
    *  getYSize.
    */
   int   getSat( int x, int y ) const;
   /**
    *  Function for getting hsv values.
    *    int   getHue( int x, int y ) const;<BR>
    *    int   getSat( int x, int y ) const;<BR>
    *    int   getVal( int x, int y ) const;<BR>
    *  These three functions return the Hue, Saturation and Value values at 
    *  the location requested in raster coordinates.  If the image is still
    *  an RGB image then Red corresponds to Hue, Green to Sat, and Blue to
    *  Value.  If the image has been classified already then you may get bogus
    *  values for road classified pixels.
    *  These functions do not do any bound checking so be careful what you
    *  pass as arguments.  x and y should be between 0 and getXSize or 
    *  getYSize.
    */
   int   getVal( int x, int y ) const;


   /**
    *  Convenience function for getting hsv values.
    *    int   getHue( double x, double y ) const;<BR>
    *    int   getSat( double x, double y ) const;<BR>
    *    int   getVal( double x, double y ) const;<BR>
    *  If you pass in vector coordinates they will
    *  be converted to raster internally and the correct values will be
    *  returned.  This function doesn't bound check, but if the coordinates
    *  can't be converted by the vector2Raster function above then this
    *  function will return -100.
    */
   int   getHue( double x, double y ) const;
    /**
    *  Convenience function for getting hsv values.
    *    int   getHue( double x, double y ) const;<BR>
    *    int   getSat( double x, double y ) const;<BR>
    *    int   getVal( double x, double y ) const;<BR>
    *  If you pass in vector coordinates they will
    *  be converted to raster internally and the correct values will be
    *  returned.  This function doesn't bound check, but if the coordinates
    *  can't be converted by the vector2Raster function above then this
    *  function will return -100.
    */
   int   getSat( double x, double y ) const;
    /**
    *  Convenience function for getting hsv values.
    *    int   getHue( double x, double y ) const;<BR>
    *    int   getSat( double x, double y ) const;<BR>
    *    int   getVal( double x, double y ) const;<BR>
    *  If you pass in vector coordinates they will
    *  be converted to raster internally and the correct values will be
    *  returned.  This function doesn't bound check, but if the coordinates
    *  can't be converted by the vector2Raster function above then this
    *  function will return -100.
    */
   int   getVal( double x, double y ) const;


   /**
    *  Queries the size of the image.
    *    int getXSize() const;<BR>
    *    int getYSize() const;<BR>
    *  Returns the number of x pixels or y pixels in the raster.
    */
   int getXSize() const;
   /**
    *  Queries the size of the image.
    *    int getXSize() const;<BR>
    *    int getYSize() const;<BR>
    *  Returns the number of x pixels or y pixels in the raster.
    */
   int getYSize() const;


   /**
    *  Tells whether or not a location is a road pixel.
    *    bool isRoad( int x, int y ) const;<BR>
    *    bool isRoad( double x, double y ) const;<BR>
    *  Returns true if x, y is the location of a road pixel.  The integer 
    *  version is for raster coordinates and the double version is for
    *  vector coordinates.  This function does no boundary checking.
    */
   bool isRoad( int x, int y ) const;
   /**
    *  Tells whether or not a location is a road pixel.
    *    bool isRoad( int x, int y ) const;<BR>
    *    bool isRoad( double x, double y ) const;<BR>
    *  Returns true if x, y is the location of a road pixel.  The integer 
    *  version is for raster coordinates and the double version is for
    *  vector coordinates.  This function does no boundary checking.
    */
   bool isRoad( double x, double y ) const;


private:

   int   *mpadHue; //!< The Hue values for this raster
   int   *mpadSat; //!< The Sat values for this raster
   int   *mpadVal; //!< The Val values for this raster

   int mnXSize;    //!< The X size in pixels
   int mnYSize;    //!< The Y size in pixels

   double madAffine[6];     //!< The affine transformation from the input tiff
   char *mpszProjectionWkt; //!< Well Known Text projection string

   /**
    * A helper function for the constructor.  Reads in a band from a tiff
    * image.
    * @param pBand The GDALRasterBand to be read in.
    * @param pDestBuff An integer buffer that will store the values that are
    *        read in.  This should be mnXSize*mnYSize in size.
    * @param pBuff A scratch buffer for reading in data.  Should be 
    *        nXBlockSize * nYBlockSize.
    * @param nYBlocks The number of blocks in the y direction in the 
    *        original raster.
    * @param nXBlocks The number of blocks in the raster in the x 
    *        direction.
    * @param nXBlockSize The size of a block in x direction pixels.
    * @param nYBlockSize The size of a block in y direction pixels.
    */

   void readBand( GDALRasterBand *pBand, int *pDestBuff,
                            unsigned char *pBuff, 
                            int nYBlocks, int nXBlocks,
                            int nXBlockSize, int nYBlockSize );

   /**
    * A helper function for the convertToHSV function.  Changes the values
    * of red, green and blue to hue, saturation and value in place.
    * @param red Should be the red value when input, will be hue on exit.
    * @param green Should be the green value when input, will be 
    *        saturation on exit.
    * @param blue Should be the blue value when input, will be the value
    *        on exit.
    */
   void RGB2HSV( int &red, int &green, int &blue );
   
};



#endif // IN_MEM_RASTER

