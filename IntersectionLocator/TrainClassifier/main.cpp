

// This file is a quick program to train a classifier and save the
// training data.  This is just for my convenience, so that I don't
// have to deal with this later on.
// Ryan Stelzleni
// 10/25/04


#include <iostream>
#include <string>
#include "../Classifier.h"

using namespace std;

void outputClassifiedImage( std::string ortho, std::string outputFile,
                            Classifier &C );

int main()
{
   string rd( "D:\\rstelzleni\\VisualStudioProjects\\"
              "IntersectionLocator\\Mo2Quads\\"
              "classifiertraining\\roads260735.tif" );
   string nonrd( "d:\\rstelzleni\\VisualStudioProjects\\"
              "IntersectionLocator\\Mo2Quads\\"
              "classifiertraining\\nonroads260735.tif" );
   string outfile( "d:\\rstelzleni\\VisualStudioProjects\\"
              "IntersectionLocator\\Mo2Quads\\"
              "classifiertraining\\testdblrds.dat" );
   string testOrtho( "d:\\rstelzleni\\VisualStudioProjects\\"
              "IntersectionLocator\\Mo2Quads\\"
              "orthos\\15SYC245735.tif" );
   string testOutput( "d:\\rstelzleni\\VisualStudioProjects\\"
              "IntersectionLocator\\Mo2Quads\\"
              "classifiertraining\\testImages\\class245735testfiverds.dat" );

   cout << "Running\n";

   GDALAllRegister();
   GDALDataset *pRd, *pNonRd;

   pRd = static_cast<GDALDataset *>( GDALOpen( rd.c_str(), GA_ReadOnly ) );
   if( pRd == NULL )
   {
      fprintf( stderr, "Failed to open rd file\n" );
      return -1;
   }
   pNonRd = static_cast<GDALDataset *>( GDALOpen( nonrd.c_str(), GA_ReadOnly ) );
   if( pNonRd == NULL )
   {
      fprintf( stderr, "Failed to open nonrd file\n" );
      return -1;
   }

   // Comment or uncomment lines as you see fit
   Classifier C;
   C.inputText( outfile.c_str() );
   C.addRdTraining( pRd );
   //C.addNonRdTraining( pNonRd );
   //C.outputText( outfile.c_str() );

   //outputClassifiedImage( testOrtho, testOutput, C );

   return 0;
}


// take the image named ortho, classify it with C and put the result
// in a generic binary file called outputFile.  For instructions on
// viewing the output, see the doxygen documentation for the main
// project, on the page called Preparing for new data.
void outputClassifiedImage( std::string ortho, std::string outputFile,
                            Classifier &C )
{
   GDALDataset *image;
   image = static_cast<GDALDataset *>( GDALOpen( ortho.c_str(), GA_ReadOnly ) );
   if( image == NULL )
   {
      fprintf( stderr, "Failed to open image file for test run\n" );
      return;
   }

   InMemRaster rasta( image );
   rasta.convertToHSV();
   rasta.classify( C );
   rasta.dumpToBinary( outputFile.c_str() );

   return;
}


