/*
 * file Classifier.cpp
 * Purpose: This file contains the implementation of the Classifier 
 *          class.  This class uses a Naive Bayes classification
 *          scheme to group road and non road pixels.
 * author Ryan Stelzleni
 * date 7-16-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Classifier.cpp,v 1.5 2004/11/23 20:50:30 rstelzleni Exp $


#include "Classifier.h"



Classifier::Classifier()
: miTotalPixels( 0 ),
  miRoadPixels( 0 ),
  miNonRoadPixels( 0 ),
  mdThresh( 0.0 )
{
   mpaiRoadHist[ Hue ] = new int[361];
   mpaiRoadHist[ Sat ] = new int[256];
   mpaiRoadHist[ Val ] = new int[256];

   mpaiNonRoadHist[ Hue ] = new int[361];
   mpaiNonRoadHist[ Sat ] = new int[256];
   mpaiNonRoadHist[ Val ] = new int[256];

   zeroArray( mpaiRoadHist[ Hue ], 361 );
   zeroArray( mpaiNonRoadHist[ Hue ], 361 );
   zeroArray( mpaiRoadHist[ Sat ], 256 );
   zeroArray( mpaiNonRoadHist[ Sat ], 256 );
   zeroArray( mpaiRoadHist[ Val ], 256 );
   zeroArray( mpaiNonRoadHist[ Val ], 256 );
}


Classifier::Classifier( GDALDataset *RdTraining, 
                        GDALDataset *NonRdTraining,
                        double dThresh )
: miTotalPixels( 0 ),
  miRoadPixels( 0 ),
  miNonRoadPixels( 0 ),
  mdThresh( dThresh )
{
   mpaiRoadHist[ Hue ] = new int[361];
   mpaiRoadHist[ Sat ] = new int[256];
   mpaiRoadHist[ Val ] = new int[256];

   mpaiNonRoadHist[ Hue ] = new int[361];
   mpaiNonRoadHist[ Sat ] = new int[256];
   mpaiNonRoadHist[ Val ] = new int[256];

   zeroArray( mpaiRoadHist[ Hue ], 361 );
   zeroArray( mpaiNonRoadHist[ Hue ], 361 );
   zeroArray( mpaiRoadHist[ Sat ], 256 );
   zeroArray( mpaiNonRoadHist[ Sat ], 256 );
   zeroArray( mpaiRoadHist[ Val ], 256 );
   zeroArray( mpaiNonRoadHist[ Val ], 256 );

   addRdTraining( RdTraining );
   addNonRdTraining( NonRdTraining );
}

Classifier::~Classifier()
{
   for( int i=0; i<3; ++i )
   {
      delete [] mpaiRoadHist[ i ];
      delete [] mpaiNonRoadHist[ i ];
   }
}


void Classifier::zeroArray( int array[], int size )
{
   for( int i=0; i<size; ++i )
      array[i] = 0;

   return;
}

void Classifier::addRdTraining( GDALDataset *RdTraining )
{
   InMemRaster R( RdTraining );
   int h, s, v;

   R.convertToHSV();
   
   for( int y=0; y < R.getYSize(); ++y )
   {
      for( int x=0; x < R.getXSize(); ++x )
      {
         h = R.getHue( x, y );
         s = R.getSat( x, y );
         v = R.getVal( x, y );

         // ignore pixels that are pure black so hopefully I can
         // use an imagine generated subset.
         if( !( h == 0 && s == 0 && v == 0 ) )
         {
            ++( mpaiRoadHist[Hue][h] );
            ++( mpaiRoadHist[Sat][s] );
            ++( mpaiRoadHist[Val][v] );

            ++miTotalPixels;
            ++miRoadPixels;
         }
      } // for x
   } // for y

   return;
} // End function addRdTraining


void Classifier::addNonRdTraining( GDALDataset *NonRdTraining )
{
   InMemRaster R( NonRdTraining );
   int h, s, v;

   R.convertToHSV();
   
   for( int y=0; y < R.getYSize(); ++y )
   {
      for( int x=0; x < R.getXSize(); ++x )
      {
         h = R.getHue( x, y );
         s = R.getSat( x, y );
         v = R.getVal( x, y );

         // ignore pixels that are pure black so hopefully I can
         // use an imagine generated subset.
         if( !( h == 0 && s == 0 && v == 0 ) )
         {
            ++( mpaiNonRoadHist[Hue][h] );
            ++( mpaiNonRoadHist[Sat][s] );
            ++( mpaiNonRoadHist[Val][v] );

            ++miTotalPixels;
            ++miNonRoadPixels;
         }
      } // for x
   } // for y

   return;
} // End function addNonRdTraining


void Classifier::setThreshold( const double &dThresh )
{
   if( dThresh >=0 && dThresh <= 1 )
      mdThresh = dThresh;
   return;
}

bool Classifier::isRoad( int h, int s, int v ) const
{
   // p for probability, not pointer
   double pRoad, pNonRoad;

   // Find pXi by counting the training examples that fall into
   // Xi and dividing by the size of the training set.
   // Then multiply that by the probabilities generated from
   // the other criteria that is present.
   // Naive Bayes classification assumes that the individual 
   // criteria of classification are independent of each other,
   // which means that in this section I can just multiply
   // together the probabilities generated for each criteria,
   // i.e. Hue, Sat and Val.

   /*pRoad = ( mpaiRoadHist[Hue][h] / miRoadPixels ) *
           ( mpaiRoadHist[Sat][s] / miRoadPixels ) *
           ( mpaiRoadHist[Val][v] / miRoadPixels ) *
           ( miRoadPixels / miTotalPixels );
   pNonRoad = ( mpaiNonRoadHist[Hue][h] / miNonRoadPixels ) *
              ( mpaiNonRoadHist[Sat][s] / miNonRoadPixels ) *
              ( mpaiNonRoadHist[Val][v] / miNonRoadPixels ) *
              ( miNonRoadPixels / miTotalPixels );
   */
   // This should be faster, although it is very ugly
   pRoad = ( static_cast<double>(mpaiRoadHist[Hue][h]) * 
             static_cast<double>(mpaiRoadHist[Sat][s]) * 
             static_cast<double>(mpaiRoadHist[Val][v])  )
           / (static_cast<double>(miRoadPixels) * 
              static_cast<double>(miRoadPixels) * 
              static_cast<double>(miTotalPixels) );
   pNonRoad = ( static_cast<double>(mpaiNonRoadHist[Hue][h]) * 
                static_cast<double>(mpaiNonRoadHist[Sat][s]) * 
                static_cast<double>(mpaiNonRoadHist[Val][v])  )
              / (static_cast<double>(miNonRoadPixels) * 
                 static_cast<double>(miNonRoadPixels) * 
                 static_cast<double>(miTotalPixels) );

   return ( (pRoad > pNonRoad) && (pRoad > mdThresh) );
} // End function isRoad


void Classifier::outputText( const char *filename )
{
   FILE *outfile = fopen( filename, "wt" );
   int i;

   if( outfile == NULL )
   {
      Error_Print( "Failed open file for output from classifier.\n" );
      return;
   }

   fprintf( outfile, "%i\t Pixels classified\n"
            "%i\t Road Pixels\n%i\t NonRoad Pixels\n\n",
            miTotalPixels, miRoadPixels, miNonRoadPixels );

   fprintf( outfile, "Hue\tRoad\tNon Road\n" );
   for( i=0; i<361; ++i )
   {
      fprintf( outfile, "%i\t%i\t%i\n", i, mpaiRoadHist[Hue][i],
               mpaiNonRoadHist[Hue][i] );
   }

   fprintf( outfile, "\nSat\tRoad\tNon Road\n" );
   for( i=0; i<256; ++i )
   {
      fprintf( outfile, "%i\t%i\t%i\n", i, mpaiRoadHist[Sat][i],
               mpaiNonRoadHist[Sat][i] );
   }

   fprintf( outfile, "\nVal\tRoad\tNon Road\n" );
   for( i=0; i<256; ++i )
   {
      fprintf( outfile, "%i\t%i\t%i\n", i, mpaiRoadHist[Val][i],
               mpaiNonRoadHist[Val][i] );
   }

   fclose( outfile );
   return;
}


void Classifier::inputText( const char *szFilename )
{
   FILE *input = fopen( szFilename, "rt" );
   int r, nr, temp, i; // road, nonroad, temp and counter

   if( input == NULL )
   {
      Error_Print( "Failed opening input file for classifier\n" );
      return;
   }

   fscanf( input, "%i\t Pixels classified\n"
           "%i\t Road Pixels\n%i\t NonRoad Pixels\n\n",
           &r, &nr, &temp );

   miTotalPixels = r;
   miRoadPixels = nr;
   miNonRoadPixels = temp;

   fscanf( input, "Hue\tRoad\tNon Road\n" );

   for( i=0; i<361; ++i )
   {
      fscanf( input, "%i\t%i\t%i\n", &temp, &r, &nr );
      mpaiRoadHist[Hue][i] = r;
      mpaiNonRoadHist[Hue][i] = nr;
   }

   fscanf( input, "\nSat\tRoad\tNon Road\n" );
   for( i=0; i<256; ++i )
   {
      fscanf( input, "%i\t%i\t%i\n", &temp, &r, &nr );
      mpaiRoadHist[Sat][i] = r;
      mpaiNonRoadHist[Sat][i] = nr;
   }

   fscanf( input, "\nVal\tRoad\tNon Road\n" );
   for( i=0; i<256; ++i )
   {
      fscanf( input, "%i\t%i\t%i\n", &temp, &r, &nr );
      mpaiRoadHist[Val][i] = r;
      mpaiNonRoadHist[Val][i] = nr;
   }

   fclose( input );
   return;
}

