/*
 * File: Classifier.h
 * Purpose: This file contains the declaration of the Classifier 
 *          class.  This class uses a Naive Bayes classification
 *          scheme to group road and non road pixels.
 *
 * Programmer: Ryan Stelzleni
 * Date: 7-16-04
 */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <stdio.h>

#include "gdal_priv.h"
#include "InMemRaster.h"

// Forward declaration because of cyclic includes
class InMemRaster;


/*************************************************************************
* 
*  Class Classifier
*
*  Description:
*  This class is a naive bayesian classifier for identifying road and
*  non-road pixels based on their Hue, Saturation and Value.  To use
*  this classifier you provide it with either training sets or with
*  saved training data.  Once the classifier is trained the isRoad
*  function can be used to identify good candidates for road pixels.
*
*
*    Classifier();
*  This constructor creates a classifier with no training yet.  This
*  classifier constructor will give the classifier a threshhold value
*  of zero.
*
*    Classifier( GDALDataset *RdTraining, GDALDataset *NonRdTraining,
*                double dThresh = 0.0 );
*  This constructor takes raster datasets to represent roads and non-roads
*  and trains itself based on that data.  More data can be added later on.
*  
*    void addRdTraining( GDALDataset *RdTraining );
*    void addNonRdTraining( GDALDataset *NonRdTraining );
*  These functions add training files to the classifier.  The files must
*  be something that can be read by GDAL and should fulfill whatever other
*  requirements the InMemRaster class imposes, since the classifier uses
*  that class to get at the values and convert them to HSV.  You can add
*  as many of these training sets as you would like.  When creating these
*  files you should try to make sure that there are only road pixels in 
*  in the RdTraining dataset and only non-road pixels in the other.
*
*    void setThreshold( const double &dThresh );
*  The threshhold was added as an experiment, and I haven't decided if I 
*  should take it out yet.  The Bayesian classifier works by assigning
*  an is road probability and an is non-road probability and taking the
*  higher of the two.  The idea of the threshhold was that roads wouldn't
*  be identified unless their is road probability was higher than the
*  is non-road probability and also was higher than the threshhhold.  My
*  initial tests with this yielded poor results though, so it is probably
*  best to not use this idea.
*
*    bool isRoad( int h, int s, int v ) const;
*  This function performs the classification.  If it is more likely that a 
*  pixel is a road than a non-road based on the available training data 
*  then the function returns true.
*
*    void outputText( const char *filename );
*  Places the currently available training data in an output file called
*  filename.  The data should be readable and can be cut and pasted into
*  a spreadsheet to look at histograms or what have you.
*  If the file can't be opened the function will Error_Print and return.
*
*    void inputText( const char *szFilename );
*  This function will read in a file that was output by outputText and
*  use the training data located in that file.  Any previous training 
*  will be erased, but you can add more training from GDAL datasets
*  after you call this function if you would like.
*
**************************************************************************/

class Classifier
{
public:

   Classifier();
   Classifier( GDALDataset *RdTraining, GDALDataset *NonRdTraining,
               double dThresh = 0.0 );
   ~Classifier();

   void addRdTraining( GDALDataset *RdTraining );
   void addNonRdTraining( GDALDataset *NonRdTraining );
   void setThreshold( const double &dThresh );

   bool isRoad( int h, int s, int v ) const;

   void outputText( const char *filename );
   void inputText( const char *szFilename );

private:

   int *mpaiRoadHist[3];
   int *mpaiNonRoadHist[3];
   int miTotalPixels;
   int miRoadPixels;
   int miNonRoadPixels;
   double mdThresh;

   enum { Hue = 0, Sat, Val };

   void zeroArray( int array[], int size );

};


#endif // CLASSIFIER_H





