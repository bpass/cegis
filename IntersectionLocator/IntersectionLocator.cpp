/*
 * File: IntersectionLocator.cpp
 * Purpose: This file is intended to test out locating intersections
 *          using OGR to open vector format files.
 * Programmer: Ryan Stelzleni
 * Date: 6-28-04
 */


#pragma warning(disable: 4786)

#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <stdio.h>
#include <string>

#include "IntersectionMap.h"
#include "InMemRaster.h"
#include "Classifier.h"
#include "DelauneyTriangulator.h"
#include "QuarticTriangulator.h"
#include "RecursiveTriangulator.h"
#include "SaalfeldRubberSheet.h"
#include "VMFilter.h"
#include "DistanceFilter.h"

void batch( double percent, Filter *filter, const char *filterType );
void filterTests();

OGRDataSource *createOGRFile(const char *pszFName, char *pszDName)
{
   OGRSFDriver *pDriver = NULL;
   OGRSFDriverRegistrar *pReg = OGRSFDriverRegistrar::GetRegistrar();
   int limit = pReg->GetDriverCount();
   OGRDataSource *pDS = NULL;

   for(int i=0; i < limit; ++i)
   {
      pDriver = pReg->GetDriver(i);
      if( strncmp(pszDName, pDriver->GetName(), strlen(pszDName))==0 )
         break;
   }

   pDS = pDriver->CreateDataSource( pszFName, NULL );

   // pDS could be NULL on failure
   return pDS;
}



int main( int argc, char *argv[] )
{
   filterTests();
   return 0;


   Filter *F = new VMFilter;
   for( double x=0.1; x < 0.6; x += 0.1 )
   {
      printf( "Beginning VMF Runs for percent %02d\n\n\n", 
              static_cast<int>(100*x) );
      batch( x, F, "VMF" );
   }
   delete F;
   F = new DistanceFilter;
   for( double y=0.1; y < 0.6; y += 0.1 )
   {
      printf( "Beginning DF Runs for percent %02d\n\n\n", 
              static_cast<int>(100*y) );
      batch( y, F, "DF" );
   }
   delete F;
   return 0;


   OGRDataSource *pVectorDS, *pControlDS, *pIntersectionsDS;
   OGRDataSource *pSmallDS, *pBigDS;
   GDALDataset *pRaster;
   char szRaster[] = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\data\\FloOrthos\\15syc320960.tif";
   char szVector[] = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\data\\LargeAreaTests\\"
                     "vectorareas\\320960";

   char szSmall[] = "d:\\rstelzleni\\visualstudioprojects\\"
                      "IntersectionLocator\\data\\ControlTest\\"
                      "ControlWithCorrelation\\"
                      "Normal50-90.shp";
   char szBig[] = "d:\\rstelzleni\\visualstudioprojects\\"
                      "IntersectionLocator\\data\\ControlTest\\"
                      "ControlWithCorrelation\\"
                      "Normal60-120.shp";

   char szControl[] = "d:\\rstelzleni\\visualstudioprojects\\"
                      "IntersectionLocator\\data\\ControlTest\\"
                      "ControlWithCorrelation\\"
                      "Normal50-90T60-120.shp";
   char szIntersections[] = "d:\\rstelzleni\\visualstudioprojects\\"
                      "IntersectionLocator\\data\\LargeAreaTests\\"
                      "Demo.shp";
                      
                      //LargeAreaTests\\"
                      //"50-90T60-120_ND_NF\\320945_50-90_ND_NF.shp";

   OGRRegisterAll();
   GDALAllRegister();

   pRaster = static_cast<GDALDataset *>
                                 ( GDALOpen( szRaster, GA_ReadOnly ) );
   if( pRaster == NULL )
   {
      fprintf( stderr, "Failed to open raster file\n" );
      return -1;
   }

   pVectorDS = OGRSFDriverRegistrar::Open( szVector, FALSE, NULL);

   if(pVectorDS == NULL)
   {
      printf("Failed opening vector file\n");
      printf("%d\n", CPLGetLastErrorNo());
      printf(CPLGetLastErrorMsg());
      return -1;
   }
/*
   pControlDS = createOGRFile( szControl, "ESRI Shapefile" );
   if( pControlDS == NULL )
   {
      fprintf( stderr, "pControlDS is NULL\n" );
      exit( -1 );
   }

   pSmallDS = createOGRFile( szSmall, "ESRI Shapefile" );
   if( pSmallDS == NULL )
   {
      fprintf( stderr, "pSmallDS is NULL\n" );
      exit( -1 );
   }   
   pBigDS = createOGRFile( szBig, "ESRI Shapefile" );
   if( pBigDS == NULL )
   {
      fprintf( stderr, "pBigDS is NULL\n" );
      exit( -1 );
   }
*/
   pIntersectionsDS = createOGRFile( szIntersections, "ESRI Shapefile" );
   if( pIntersectionsDS == NULL )
   {
      fprintf( stderr, "pIntersectionsDS is NULL\n" );
      exit( -1 );
   }

   //
   // Finished opening input and output files
   //
   printf( "Files opened successfully\n\n" );

   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText( "d:\\rstelzleni\\visualstudioprojects\\"
                "IntersectionLocator\\data\\onearea\\TwoRdsHist.txt" );

   printf( "Loading raster image\n" );
   InMemRaster Rasta( pRaster );
   printf( "Converting to HSV\n" );
   Rasta.convertToHSV();
   printf( "Classifying road pixels\n\n" );
   Rasta.classify( C );
//   printf( "Smoothing road pixels\n\n" );
//   Rasta.smoothFive();
   
   printf( "Loading intersection data\n" );
   IntersectionMap Intersections( pVectorDS, pRaster );
   printf( "Finding control points\n\n" );

   printf( "\n50-90\n" );
   Intersections.findControlPoints( Rasta, 50, 90 );
//   printf( "Outputting 50-90\n\n" );
//   Intersections.outputControlPoints( pSmallDS );

//   printf( "\n60-120\n" );
//   Intersections.findControlPoints( Rasta, C, 60, 120 );
//   printf( "Outputting 60-120\n\n" );
//   Intersections.outputControlPoints( pBigDS );

//   printf( "\nBoth 50-90 and 60-120\n" );
//  typedef std::pair<int, int> sizes;
//   std::vector<sizes> mySizes;
//   sizes A(50, 90), B(60, 120); //, D(60, 100), E(60, 110);
//   mySizes.push_back( A );
//   mySizes.push_back( B );
 //  mySizes.push_back( D );
 //  mySizes.push_back( A );
//   Intersections.iterativelyFindControlPoints( Rasta, C, mySizes );


//   printf( "Creating intersection shape file\n" );
//   Intersections.outputIntersections( pIntersectionsDS );
//   printf( "Creating control point shape file\n" );
//   Intersections.outputControlPoints( pControlDS );

   printf( "Creating final output file (Filter, Triangulate, Rubber Sheeting)\n" );
   DelauneyTriangulator *triangulator = new RecursiveTriangulator;//QuarticTriangulator;

   RubberSheetTransform *transformer = new SaalfeldRubberSheet;
//   Intersections.triangulate( triangulator, pIntersectionsDS );
   Filter *filter = new DistanceFilter;
   //new DistanceFilter; // or new VMFilter;

   Intersections.adjustToControl( triangulator, transformer,
                                  filter, 0.5, pIntersectionsDS );
   
   delete filter;
   delete transformer;
   delete triangulator;

   return 0;
}


void filterTests()
{
   OGRDataSource *pVectorDS, *pOutputDS, *pLinesDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szLines, szAbbrev;

   std::string szRasterDir = "f:\\rstelzleni\\IntersectionData\\15SYC";
   std::string szVectorDir = "f:\\rstelzleni\\IntersectionData\\MODOT";
   std::string szOutputDir = "f:\\rstelzleni\\IntersectionData\\output\\";
/*   std::string szRasterDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\data\\FloOrthos\\15syc";
   std::string szVectorDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\MoDOT\\AreaCoverages\\";
                     //"IntersectionLocator\\"//MoDOT\\AreaCoverages\\";
                     //"data\\largeareatests\\"
                     //"vectorareas\\";
   std::string szOutputDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\MoDOT\\fixedRun\\";
                      //"IntersectionLocator\\"//MoDOT\\transTest\\";
                      //"data\\FilteredIntersections2\\";
                      //"smooth5\\";
*/

/*   char *aszNames[] = { "305945", "305960", "305975", 
                        "320945", "320960", "320975",
                        "335945", "335960", "335975" };
   int n = 9; // number of names
*/
   char *aszNames[] = { "140750" };
   int n = 1;

   OGRRegisterAll();
   GDALAllRegister();

   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText( "d:\\rstelzleni\\visualstudioprojects\\"
                   "IntersectionLocator\\data\\onearea\\TwoRdsHist.txt" );

   DelauneyTriangulator *Triangulator = new QuarticTriangulator;
   RubberSheetTransform *Transformer = new SaalfeldRubberSheet;

   for( int i=0; i<n; ++i )
   {
      
      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += ".tif";
      szVector = szVectorDir + aszNames[i];


      pRaster = static_cast<GDALDataset *>
                                    ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);

      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }


      printf( "Input files opened successfully\n\n" );

      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );

      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );


      printf( "\n50-90\n" );
      Intersections.findControlPoints( Rasta, 50, 90 );
      printf( "Saving control points\n" );
      std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir + "Diff";
      szOutput += "Unfiltered\\";
      szOutput += aszNames[i];
      szLines = szOutput + "Diff_Lines.shp";
      szOutput += "Diff_Points.shp";

      pOutputDS = createOGRFile( szOutput.c_str(), "ESRI Shapefile" );
      if( pOutputDS == NULL )
      {
         fprintf( stderr, "pOutputDS is NULL\n" );
         exit( -1 );
      }

      pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
      if( pLinesDS == NULL )
      {
         fprintf( stderr, "pLinesDS is NULL\n" );
         exit( -1 );
      }

      printf( "Outputting base control point set and lines\n" );
      Intersections.outputControlPoints( pOutputDS );
      Intersections.triangulateAndAdjust( Triangulator, Transformer,  
                                          pLinesDS ); 

      delete pOutputDS;
      delete pLinesDS;

      for( double r = 0.1; r <= 0.5; r = r + 0.1 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", static_cast<int>(100*r) );
         szAbbrev = temp;

         szOutput = szOutputDir + "Diff";
         szOutput += szAbbrev + "\\";
         szOutput += aszNames[i];
         szLines = szOutput + szAbbrev + "Diff_Lines.shp";
         szOutput += szAbbrev + "Diff_Points.shp";

         pOutputDS = createOGRFile( szOutput.c_str(), "ESRI Shapefile" );
         if( pOutputDS == NULL )
         {
            fprintf( stderr, "pOutputDS is NULL\n" );
            exit( -1 );
         }

         pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
         if( pLinesDS == NULL )
         {
            fprintf( stderr, "pLinesDS is NULL\n" );
            exit( -1 );
         }


         printf( "VMF Filtering and outputting control points\n\n" );
         Filter *filter = new VMFilter;
         Intersections.setControlPoints( originalPoints );
         Intersections.filter( filter, r );
         Intersections.outputControlPoints( pOutputDS );
         Intersections.triangulateAndAdjust( Triangulator, Transformer,
                                             pLinesDS );

         delete filter;
         delete pOutputDS;
         delete pLinesDS;


         sprintf( temp, "_%02dDF", static_cast<int>(100*r) );
         szAbbrev = temp;

         szOutput = szOutputDir + "Diff";
         szOutput += szAbbrev + "\\";
         szOutput += aszNames[i];
         szLines = szOutput + szAbbrev + "Diff_Lines.shp";
         szOutput += szAbbrev + "Diff_Points.shp";

         pOutputDS = createOGRFile( szOutput.c_str(), "ESRI Shapefile" );
         if( pOutputDS == NULL )
         {
            fprintf( stderr, "pOutputDS is NULL\n" );
            exit( -1 );
         }

         pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
         if( pLinesDS == NULL )
         {
            fprintf( stderr, "pLinesDS is NULL\n" );
            exit( -1 );
         }


         printf( "DF Filtering and outputting control points\n\n" );
         filter = new DistanceFilter;
         Intersections.setControlPoints( originalPoints );
         Intersections.filter( filter, r );
         Intersections.outputControlPoints( pOutputDS );
         Intersections.triangulateAndAdjust( Triangulator, Transformer,
                                             pLinesDS );

         delete filter;
         delete pOutputDS;
         delete pLinesDS;
      } // end for r (ratio)
      
      delete pRaster;
      delete pVectorDS;

      printf( "Finished Area %s\n", aszNames[i] );
   } // end for i

   delete Triangulator;
   delete Transformer;

   return;
}


void batch( double percent, Filter *filter, const char *filterType )
{
   OGRDataSource *pVectorDS, *pOutputDS, *pOrigDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szOrigPoints, szAbbrev, szSizes;

   std::string szRasterDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\data\\FloOrthos\\15syc";
   std::string szVectorDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\"//MoDOT\\AreaCoverages\\";
                     "data\\largeareatests\\"
                     "vectorareas\\";
   std::string szOutputDir = "d:\\rstelzleni\\visualstudioprojects\\"
                      "IntersectionLocator\\"//MoDOT\\transTest\\";
                      "data\\FilteredIntersections2\\";
                      //"smooth5\\";

   char temp[10];
   sprintf( temp, "_%02d%s", static_cast<int>(100*percent), filterType );
   szAbbrev = temp;
   szSizes = ""; //"50-90T60-120";

   char *aszNames[] = { "305945", "305960", "305975", 
                        "320945", "320960", "320975",
                        "335945", "335960", "335975" };
   int n = 9; // number of names

   OGRRegisterAll();
   GDALAllRegister();


   for( int i=0; i<n; ++i )
   {

      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += ".tif";
      szVector = szVectorDir + aszNames[i];
      szOutput = szOutputDir + szSizes + "ND";
      szOutput += "_Points" + szAbbrev + "\\";
      szOutput += aszNames[i];
      szOrigPoints = szOutput;
      szOutput += szSizes + szAbbrev + ".shp";
      szOrigPoints += "_Points.shp";

      pRaster = static_cast<GDALDataset *>
                                    ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);

      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }

      pOutputDS = createOGRFile( szOutput.c_str(), "ESRI Shapefile" );
      if( pOutputDS == NULL )
      {
         fprintf( stderr, "pOutputDS is NULL\n" );
         exit( -1 );
      }

      pOrigDS = createOGRFile( szOrigPoints.c_str(), "ESRI Shapefile" );
      if( pOrigDS == NULL )
      {
         fprintf( stderr, "pOrigDS is NULL\n" );
         exit( -1 );
      }

      printf( "Files opened successfully\n\n" );

      Classifier C;
      printf( "Loading classifier training\n\n" );
      C.inputText( "d:\\rstelzleni\\visualstudioprojects\\"
                   "IntersectionLocator\\data\\onearea\\TwoRdsHist.txt" );

      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );
//      printf( "Smoothing road pixels\n\n" );
//      Rasta.smoothFive();

   
      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );


      printf( "\n50-90\n" );
      Intersections.findControlPoints( Rasta, 50, 90 );
      printf( "Outputting 50-90\n\n" );
      Intersections.outputControlPoints( pOrigDS );

      printf( "Filtering and outputting control points\n\n" );
//      Filter *filter = new DistanceFilter;
      Intersections.filter( filter, percent );
      Intersections.outputControlPoints( pOutputDS );

 /*     printf( "\nBoth 50-90 and 60-120\n" );
      typedef std::pair<int, int> sizes;
      std::vector<sizes> mySizes;
      sizes A(50, 90), B(60, 120);
      mySizes.push_back( A );
      mySizes.push_back( B );
      Intersections.iterativelyFindControlPoints( Rasta, C, mySizes );

      printf( "Creating final output file (Filter, Triangulate, Rubber Sheeting)\n" );
      DelauneyTriangulator *triangulator = new QuarticTriangulator;

      RubberSheetTransform *transformer = new SaalfeldRubberSheet;
      //Filter *filter = new VMFilter;
      //new DistanceFilter; // or new VMFilter;

      Intersections.adjustToControl( triangulator, transformer,
                                     filter, percent, pOutputDS );
*/
      delete pVectorDS;
      delete pRaster;
      delete pOutputDS;
      delete pOrigDS;

      //delete triangulator;
      //delete transformer;
      //delete filter;

      printf( "Finished Area %s\n", aszNames[i] );
   } // end for i

   return;
}


//int main( int argc, char *argv[] )
int bla()
{
   OGRDataSource *pDS;
   GDALDataset *pRasterData;
   //GDALDataset *pRasterDataII;

   OGRRegisterAll();
   GDALAllRegister();

   // inserting a test for the Classifier 
   pRasterData = static_cast<GDALDataset *>(
                           GDALOpen( "d:\\rstelzleni\\visualstudioprojects\\"
                           "IntersectionLocator\\data\\onearea\\roads305960.tif",
                           GA_ReadOnly ) );
   if( pRasterData == NULL )
   {
      fprintf( stderr, "Failed opening raster file\n" );
      return -1;
   }
/*
   pRasterDataII = static_cast<GDALDataset *>(
                           GDALOpen( "d:\\rstelzleni\\visualstudioprojects\\"
                           "IntersectionLocator\\data\\onearea\\nonroads320960all.tif",
                           GA_ReadOnly ) );
   if( pRasterDataII == NULL )
   {
      fprintf( stderr, "Failed opening raster file\n" );
      return -1;
   }

   Classifier C;
   fprintf( stderr, "Loading classifier training\n" );
   C.inputText( "d:\\rstelzleni\\visualstudioprojects\\"
                "IntersectionLocator\\data\\onearea\\TwoRdsHist.txt" );

//   fprintf( stderr, "Training classifier\n" );
//   C.addRdTraining( pRasterData );
//   C.addNonRdTraining( pRasterDataII );
   C.setThreshold( 0.0f );
//   fprintf( stderr, "Output histograms\n" );
//   C.outputText( "d:\\rstelzleni\\visualstudioprojects\\"
//                 "IntersectionLocator\\data\\onearea\\TwoRdRGBHist.txt" );

   delete pRasterData;
   delete pRasterDataII;

   fprintf( stderr, "Reading in 15SC320960.tif\n" );
   pRasterData = static_cast<GDALDataset *>(
                           GDALOpen( "d:\\rstelzleni\\visualstudioprojects\\"
                           "IntersectionLocator\\data\\floorthos\\15SYC320960.tif",
                           GA_ReadOnly ) );
   InMemRaster Rasta( pRasterData );
   fprintf( stderr, "Convert to HSV\n" );
   Rasta.convertToHSV();
   fprintf( stderr, "Classifying roads\n" );
   Rasta.classify( C );
   fprintf( stderr, "Smoothing roads\n" );
   Rasta.smoothFive();
   fprintf( stderr, "Dump to rdoverlay file\n" );
   Rasta.dumpToBinary( "d:\\rstelzleni\\visualstudioprojects\\intersectionlocator\\"
                       "data\\onearea\\320960rdovrlay.dat" );
   return 0;
    delete to here to get normal execution back */


   // open the vector file 
   printf(" %d drivers registered\n", OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount());

   pDS = OGRSFDriverRegistrar::Open( "d:\\rstelzleni\\visualstudioprojects\\"
                                     "IntersectionLocator\\data\\OneArea\\finalarea",
                                     FALSE, NULL);

   if(pDS == NULL)
   {
      printf("Failed opening vector file\n");
      printf("%d\n", CPLGetLastErrorNo());
      printf(CPLGetLastErrorMsg());
      return -1;
   }
/*
   // open the raster file 
   pRasterData = static_cast<GDALDataset *>(
                           GDALOpen( "d:\\rstelzleni\\visualstudioprojects\\"
                           "IntersectionLocator\\data\\floorthos\\15SYC320960.tif",
                           GA_ReadOnly ) );
   if( pRasterData == NULL )
   {
      fprintf( stderr, "Failed opening raster file\n" );
      delete pDS;
      return -1;
   }

   fprintf( stdout, "Reading in the file\n" );
   InMemRaster R( pRasterData );
   fprintf( stdout, "Finished reading file, converting to HSV\n" );
   R.convertToHSV();
   fprintf( stdout, "Writing data to file\n" );
   R.dumpToFile( "d:\\rstelzleni\\visualstudioprojects\\intersectionlocator"
                 "\\data\\onearea\\dump.dat" );
   fprintf( stdout, "Finished\n" );
   //R.dumpToFile( "d:\\rstelzleni\\visualstudioprojects\\intersectionlocator"
   //              "\\data\\onearea\\red.txt" );
   return 0;
*/

   /* Initialize the IntersectionMap */
   IntersectionMap intersections( pDS, pRasterData );

   intersections.templateTest( 88, "d:\\rstelzleni\\visualstudioprojects\\"
                                  "intersectionlocator\\data\\templatetest\\"
                                  "twenty" );

   return 0;

   /* Output the intersections */
   OGRDataSource *pOutFile;
   pOutFile = createOGRFile( "d:\\rstelzleni\\visualstudioprojects\\"
                          "intersectionlocator\\data\\OneArea\\Intersections.shp",
                          "ESRI Shapefile" );
   if( pOutFile == NULL )
   {
      fprintf( stderr, "pOutFile is NULL\n" );
      exit( -1 );
   }

   intersections.outputIntersections( pOutFile );


   delete pDS;
   delete pOutFile;
   return 0;
}



