/**
 * @file Classifier.h
 * Purpose: This file contains the declaration of the Classifier 
 *          class.  This class uses a Naive Bayes classification
 *          scheme to group road and non road pixels.
 *
 * @author Ryan Stelzleni
 * @date 7-16-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Classifier.h,v 1.3 2004/10/22 13:30:26 rstelzleni Exp $


#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdio.h>

#include "gdal_priv.h"
#include "InMemRaster.h"

// Forward declaration because of cyclic includes
class InMemRaster;


/**
*  This class is a naive bayesian classifier for identifying road and
*  non-road pixels based on their Hue, Saturation and Value.  To use
*  this classifier you provide it with either training sets or with
*  saved training data.  Once the classifier is trained the isRoad
*  function can be used to identify good candidates for road pixels.
*/
class Classifier
{
public:

   /**
    *  This constructor creates a classifier with no training yet.  This
    *  classifier constructor will give the classifier a threshhold value
    *  of zero.
    */
   Classifier();

   /**
    *  This constructor takes raster datasets to represent roads and non-roads
    *  and trains itself based on that data.  More data can be added later on.
    *  @param RdTraining Pointer to a GDALDataset the contains only a raster
    *         image of roads.
    *  @param NonRdTraining Pointer to a GDALDataset that contains only a
    *         raster image on non-roads.
    *  @param dThresh double to represent the minimum probability threshold
    *         before a pixel is identified as a road.  It seems best to
    *         leave this zero.
    */  
   Classifier( GDALDataset *RdTraining, GDALDataset *NonRdTraining,
               double dThresh = 0.0 );


   ~Classifier();

   /**
   *  These functions add training files to the classifier.  The files must
   *  be something that can be read by GDAL and should fulfill whatever other
   *  requirements the InMemRaster class imposes, since the classifier uses
   *  that class to get at the values and convert them to HSV.  You can add
   *  as many of these training sets as you would like.  When creating these
   *  files you should try to make sure that there are only road pixels in 
   *  in the RdTraining dataset and only non-road pixels in the other.
   *  @param RdTraining A pointer to a dataset of road pixels.
   *  @see addNonRdTraining
   */
   void addRdTraining( GDALDataset *RdTraining );

    /**
   *  These functions add training files to the classifier.  The files must
   *  be something that can be read by GDAL and should fulfill whatever other
   *  requirements the InMemRaster class imposes, since the classifier uses
   *  that class to get at the values and convert them to HSV.  You can add
   *  as many of these training sets as you would like.  When creating these
   *  files you should try to make sure that there are only road pixels in 
   *  in the RdTraining dataset and only non-road pixels in the other.
   *  @param NonRdTraining A pointer to a dataset of non-road pixels.
   *  @see addRdTraining
   */
   void addNonRdTraining( GDALDataset *NonRdTraining );

   /**
   *  The threshhold was added as an experiment, and I haven't decided if I 
   *  should take it out yet.  The Bayesian classifier works by assigning
   *  an is road probability and an is non-road probability and taking the
   *  higher of the two.  The idea of the threshhold was that roads wouldn't
   *  be identified unless their is road probability was higher than the
   *  is non-road probability and also was higher than the threshhhold.  My
   *  initial tests with this yielded poor results though, so it is probably
   *  best to not use this idea.
   *  @param dThresh The new threshold value.
   */
   void setThreshold( const double &dThresh );

   /**
    *  This function performs the classification.  If it is more likely that a
    *  pixel is a road than a non-road based on the available training data 
    *  then the function returns true.
    *  @param h A hue value taken from a pixel
    *  @param s A saturation value taken from a pixel
    *  @param v A value um.. value taken from a pixel
    *  @return true if those values are identified as a road.
    */
   bool isRoad( int h, int s, int v ) const;

   /**
    *  Places the currently available training data in an output file called
    *  filename.  The data should be readable and can be cut and pasted into
    *  a spreadsheet to look at histograms or what have you.
    *  If the file can't be opened the function will Error_Print and return.
    *  @param filename The name of the file to output to, including the path.
    */
   void outputText( const char *filename );

   /**
    *  This function will read in a file that was output by outputText and
    *  use the training data located in that file.  Any previous training 
    *  will be erased, but you can add more training from GDAL datasets
    *  after you call this function if you would like.
    *  @param szFilename The name of the file to read in training from.
    */
   void inputText( const char *szFilename );

private:

   int *mpaiRoadHist[3];    //!< A table of hsv values so far for roads
   int *mpaiNonRoadHist[3]; //!< A table of hsv values so far for non-roads
   int miTotalPixels;       //!< Total number of pixels trained from so far
   int miRoadPixels;        //!< Total number of road pixels trained from
   int miNonRoadPixels;     //!< Total number of non-road pixels trained from
   double mdThresh;         //!< The threshold values

   /**
    * Anonymous enum for Hue, Sat and Val.
    * The Hue, Sat and Val enumerations can be used as indexes into the
    * tables of histograms used in this class.  This is defined for clarity.
    */
   enum { Hue = 0, Sat, Val };

   /**
    * Zeros out an array, this is just a convenience function used in
    * the classes constructor.
    * @param array A pointer to a buffer to be zero'd.
    * @param size The number of items in the array
    */
   void zeroArray( int array[], int size );

};


#endif // CLASSIFIER_H





