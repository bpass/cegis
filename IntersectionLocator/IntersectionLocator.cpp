/**
 * @file IntersectionLocator.cpp
 * Description: This file is intended to test out locating intersections
 *              using OGR to open vector format files.
 * @author Ryan Stelzleni
 * @date 6-28-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: IntersectionLocator.cpp,v 1.17 2005/05/17 18:48:25 ahartman Exp $

#ifdef _MSC_VER
#if _MSC_VER < 1300
#pragma warning(disable: 4786)
#endif
#endif

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
void recursiveTriangulatorTest();
void triangleOutputTest();
void profilerTest();


/**
 * This function was created because OGR doesn't offer a clear way to
 * select the type of a new data source that it creates.  
 * @param pszFName The name of the new datasource.
 * @param pszDName The name of the driver for the filetype.  
 *        "ESRI Shapefile" is the name for Shapefiles.
 * 
 */
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


/** The main function here is just to help run tests on the library as 
 *  a whole.  If you get rid of the main function (basically of this 
 *  whole file) then you can build the project as a static library 
 *  easily, in case you want the thing as a library for linking it to
 *  a gui or other interface without worrying about what the code in
 *  this project is.
 * */
int main( /*int argc, char *argv[]*/ )
{
//   // Run the function to profile this project
//   profilerTest();
//   return 0;

   // Test the triangle output
   triangleOutputTest();
   return 0;
    
   // Test of the RecursiveTriangulator
   recursiveTriangulatorTest();
   return 0;

   // New way of testing filters
   filterTests();
   return 0;


   // old way of testing filters
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
}


/**
 * This function runs the vector median filter and the magnitude only
 * filter on the datasets named in aszNames.  The file naming and stuff
 * is kind of a pain, and I'm not going to bother describing it in this
 * comment.  Read the source if you need to know.
 * This function is basically a hack to make it easier to run
 * tests.
 */
void filterTests()
{
   OGRDataSource *pVectorDS, *pOutputDS, *pLinesDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szPoints, szLines, szAbbrev;

   std::string szRasterDir = "L:\\cartoresearch\\data-integration\\gjaromack\\STL\\orthoimages\\15SYC";
   std::string szVectorDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\OriginalRoads\\";
   std::string szOutputDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\CorrectedRoads_10-90\\";

   double templateSize = 50, areaSize = 65;


   char *aszNames[] = {
        "065630",
        "065645",
        "065660",
        "065675",
        "065690",
        "065705",
        "065720",
        "065735",
        "065750",
        "065765",
        "065780",
        //"080630",
        "080645",
        "080660",
        "080675",
        "080690",
        "080705",
        "080720",
        "080735",
        "080750",
        "080765",
        "080780",
        "095630",
        "095645",
        "095660",
        "095675",
        "095690",
        "095705",
        "095720",
        "095735",
        "095750",
        "095765",
        "095780",
        "110630",
        "110645",
        "110660",
        "110675",
        "110690",
        "110705",
        "110720",
        "110735",
        "110750",
        "110765",
        //"110780",
        "125630",
        "125645",
        "125660",
        "125675",
        "125690",
        "125705",
        "125720",
        "125735",
        "125750",
        "125765",
        "125780",
        "140630",
        "140645",
        "140660",
        "140675",
        "140690",
        "140705",
        "140720",
        "140735",
        "140750",
        "140765",
        "140780",
        "155630",
        "155645",
        "155660",
        "155675",
        "155690",
        "155705",
        "155720",
        "155735",
        "155750",
        "155765",
        "155780",
        "170630",
        "170645",
        "170660",
        "170675",
        "170690",
        "170705",
        "170720",
        "170735",
        "170750",
        "170765",
        "170780",
        "185630",
        "185645",
        "185660",
        "185675",
        "185690",
        "185705",
        "185720",
        "185735",
        "185750",
        "185765",
        "185780",
        "200630",
        "200645",
        "200660",
        "200675",
        "200690",
        "200705",
        "200720",
        "200735",
        "200750",
        "200765",
        "200780",
        "215630",
        "215645",
        "215660",
        "215675",
        "215690",
        "215705",
        "215720",
        "215735",
        "215750",
        "215765",
        "215780",
        "230630",
        "230645",
        "230660",
        "230675",
        "230690",
        "230705",
        "230720",
        "230735",
        "230750",
        "230765",
        "230780",
        "245630",
        "245645",
        "245660",
        "245675",
        "245690",
        "245705",
        "245720",
        "245735",
        "245750",
        "245765",
        "245780",
        "260630",
        "260645",
        "260660",
        "260675",
        "260690",
        "260705",
        "260720",
        "260735",
        "260750",
        "260765",
        "260780",
        "275630",
        "275645",
        "275660",
        "275675",
        "275690",
        "275705",
        "275720",
        "275735",
        "275750",
        "275765",
        "275780"
    };

   int n = 163; // number of names



   OGRRegisterAll();
   GDALAllRegister();

   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText("L:\\sdir_snap\\rstelzleni\\Mo2Quads\\classifiertraining\\training.dat");

   DelauneyTriangulator *Triangulator = new QuarticTriangulator;
   //DelauneyTriangulator *Triangulator = new RecursiveTriangulator;
   RubberSheetTransform *Transformer = new SaalfeldRubberSheet;

   for( int i=0; i<n; ++i )
   {
      // Figure out the names of the raster and vector files to open
      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += ".TIF";
      szVector = szVectorDir + aszNames[i];

      // Open the raster file and check for errors
      pRaster = static_cast<GDALDataset *>
                                    ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      // Open the vector file and check for errors
      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);
      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }


      printf( "Input files opened successfully\n\n" );

      // Run the classifier on the raster file
      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );

      // Create the IntersectionMap
      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );

      // Get the original control points
      printf( "\n10-90\n" );
      Intersections.findControlPoints( Rasta, templateSize, areaSize );
      printf( "Saving control points\n" );
      std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir + "Unfiltered\\";
      szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
      szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";

      // Open the file to output the original points
      pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
      if( pOutputDS == NULL )
      {
         fprintf( stderr, "pOutputDS is NULL\n" );
         exit( -1 );
      }

      // Open the file to output the original lines
      pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
      if( pLinesDS == NULL )
      {
         fprintf( stderr, "pLinesDS is NULL\n" );
         exit( -1 );
      }

      // Output the original points
      printf( "Outputting base control point set and lines\n" );
      Intersections.outputControlPoints( pOutputDS );

      // Do the triangulation and transform
      Intersections.triangulateAndAdjust( Triangulator, Transformer,  
                                          pLinesDS ); 

      delete pOutputDS;
      delete pLinesDS;




      // Now filter the points using all filters and combinations
      for( int r = 10; r <= 90; r = r + 10 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", r );
         szAbbrev = temp;

         szOutput = szOutputDir + "Filter" ;
         szOutput += szAbbrev + "\\";
         szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
         szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";

         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
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
         Intersections.filter( filter, static_cast<double>(r)/100 );
         Intersections.outputControlPoints( pOutputDS );
         Intersections.triangulateAndAdjust( Triangulator, Transformer,
                                             pLinesDS );

         delete filter;
         delete pOutputDS;
         delete pLinesDS;

//         sprintf( temp, "_%02dDF", r );
//         szAbbrev = temp;
//
//         szOutput = szOutputDir + "Filter";
//         szOutput += szAbbrev + "\\";
//         szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
//         szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";
//
//         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
//         if( pOutputDS == NULL )
//         {
//            fprintf( stderr, "pOutputDS is NULL\n" );
//            exit( -1 );
//         }
//
//         pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
//         if( pLinesDS == NULL )
//         {
//            fprintf( stderr, "pLinesDS is NULL\n" );
//            exit( -1 );
//         }
//
//
//         printf( "DF Filtering and outputting control points\n\n" );
//         filter = new DistanceFilter;
//         Intersections.setControlPoints( originalPoints );
//         Intersections.filter( filter, static_cast<double>(r)/100 );
//         Intersections.outputControlPoints( pOutputDS );
//         Intersections.triangulateAndAdjust( Triangulator, Transformer,
//                                             pLinesDS );
//
//         delete filter;
//         delete pOutputDS;
//         delete pLinesDS;
      } // end for r (ratio)
      
      delete pRaster;
      delete pVectorDS;

      printf( "Finished Area %s\n", aszNames[i] );
   } // end for i

   delete Triangulator;
   delete Transformer;

   return;
}


/** This function locates control points and runs uses the passed
 *  in filter to filter out control points before correcting the lines.
 *  The process is run on all the datasets inthe aszNames array.
 *  @param percent The percent of points to filter out.
 *  @param filter the filtering method to use.
 *  @param filterType A string to use in nameing the output files.  The
 *         string should refer to the type of filter.
 */
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

void recursiveTriangulatorTest()
{
   OGRDataSource *pVectorDS, *pOutputDS, *pLinesDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szPoints, szLines, szAbbrev;

   std::string szRasterDir = "L:\\cartoresearch\\data-integration\\gjaromack\\STL\\orthoimages\\15SYC";
   std::string szVectorDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\OriginalRoads\\";
   //std::string szOutputDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\CorrectedRoads_10-90\\";
   std::string szOutputDir = "D:\\Data\\Output\\RecursiveTriangulatorTest\\";

   double templateSize = 50, areaSize = 65;


   char *aszNames[] = {
        "125645",
    };

   int n = 1; // number of names



   OGRRegisterAll();
   GDALAllRegister();

   // load the classifier training
   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText("L:\\sdir_snap\\rstelzleni\\Mo2Quads\\classifiertraining\\training.dat");

   // create the objects that will do the triangulation and transformation
   DelauneyTriangulator *Triangulator = new RecursiveTriangulator;
   RubberSheetTransform *Transformer = new SaalfeldRubberSheet;

   // go through each vector/raster combination
   for( int i=0; i<n; ++i )
   {
      
      // calculate where to store the output
      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += ".TIF";
      szVector = szVectorDir + aszNames[i];


      // open a GDAL file for the raster
      pRaster = static_cast<GDALDataset *>
                   ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      // open an OGR file for the vector
      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);

      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }


      printf( "Input files opened successfully\n\n" );

      // load the raster, convert it to HSV, and classify it
      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );

      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );


      // find the control points for this vector/raster combination
      printf( "\n10-90\n" );
      Intersections.findControlPoints( Rasta, templateSize, areaSize );
      printf( "Saving control points\n" );
      std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir + "Unfiltered\\";
      szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
      szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";

      pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
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

      // Temporary stopping point (1/31/05)   
      delete Triangulator;
      delete Transformer;
      return;


      // Now filter the points using all filters and combinations
      for( int r = 10; r <= 90; r = r + 10 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", r );
         szAbbrev = temp;

         szOutput = szOutputDir + "Filter" ;
         szOutput += szAbbrev + "\\";
         szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
         szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";

         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
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
         Intersections.filter( filter, static_cast<double>(r)/100 );
         Intersections.outputControlPoints( pOutputDS );
         Intersections.triangulateAndAdjust( Triangulator, Transformer,
                                             pLinesDS );

         delete filter;
         delete pOutputDS;
         delete pLinesDS;

//         sprintf( temp, "_%02dDF", r );
//         szAbbrev = temp;
//
//         szOutput = szOutputDir + "Filter";
//         szOutput += szAbbrev + "\\";
//         szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
//         szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";
//
//         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
//         if( pOutputDS == NULL )
//         {
//            fprintf( stderr, "pOutputDS is NULL\n" );
//            exit( -1 );
//         }
//
//         pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
//         if( pLinesDS == NULL )
//         {
//            fprintf( stderr, "pLinesDS is NULL\n" );
//            exit( -1 );
//         }
//
//
//         printf( "DF Filtering and outputting control points\n\n" );
//         filter = new DistanceFilter;
//         Intersections.setControlPoints( originalPoints );
//         Intersections.filter( filter, static_cast<double>(r)/100 );
//         Intersections.outputControlPoints( pOutputDS );
//         Intersections.triangulateAndAdjust( Triangulator, Transformer,
//                                             pLinesDS );
//
//         delete filter;
//         delete pOutputDS;
//         delete pLinesDS;
      } // end for r (ratio)
      
      delete pRaster;
      delete pVectorDS;

      printf( "Finished Area %s\n", aszNames[i] );
   } // end for i

   delete Triangulator;
   delete Transformer;

   return;
}

void triangleOutputTest()
{
   OGRDataSource *pVectorDS, *pOutputDS, *pLinesDS, *pTrianglesDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szPoints, szLines, szAbbrev,
               szTriangles;

//   std::string szRasterDir = "L:\\cartoresearch\\data-integration\\gjaromack\\STL\\orthoimages\\15SYC";
//   std::string szVectorDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\OriginalRoads\\";
//   //std::string szOutputDir = "L:\\sdir_snap\\rstelzleni\\Mo2Quads\\CorrectedRoads_10-90\\";
//   std::string szOutputDir = "D:\\Data\\Output\\boundingSmallSquare\\"; 

   const std::string szRasterDir = "/snap/ahartman/Data/STL/orthoimages/15SYC";
   const std::string szVectorDir = "/snap/rstelzleni/Mo2Quads/OriginalRoads/";
   const std::string szOutputDir = "/snap/ahartman/Data/Output/boundingLargeSquare/";
   const std::string szTiffExtension = ".tif";

   const double templateSize = 50, areaSize = 65;


   const char * const aszNames[] = {
        //"065630",
        //"065645",
        //"065660",
        //"065675",
        //"065690",
        //"065705",
        //"065720",
        //"065735",
        //"065750",
        //"065765",
        //"065780",
        ////"080630",
        //"080645",
        //"080660",
        //"080675",
        //"080690",
        //"080705",
        //"080720",
        //"080735",
        //"080750",
        //"080765",
        //"080780",
        //"095630",
        //"095645",
        //"095660",
        //"095675",
        //"095690",
        //"095705",
        //"095720",
        //"095735",
        //"095750",
        //"095765",
        //"095780",
        //"110630",
        //"110645",
        //"110660",
        //"110675",
        //"110690",
        //"110705",
        //"110720",
        //"110735",
        //"110750",
        //"110765",
        ////"110780",
        //"125630",
        //"125645",
        //"125660",
        //"125675",
        //"125690",
        //"125705",
        //"125720",
        //"125735",
        //"125750",
        //"125765",
        //"125780",
        //"140630",
        //"140645",
        //"140660",
        //"140675",
        //"140690",
        //"140705",
        //"140720",
        //"140735",
        //"140750",
        //"140765",
        //"140780",
        //"155630",
        //"155645",
        //"155660",
        //"155675",
        //"155690",
        //"155705",
        //"155720",
        //"155735",
        //"155750",
        //"155765",
        //"155780",
        //"170630",
        //"170645",
        //"170660",
        //"170675",
        //"170690",
        //"170705",
        //"170720",
        //"170735",
        //"170750",
        //"170765",
        //"170780",
        //"185630",
        //"185645",
        //"185660",
        //"185675",
        //"185690",
        //"185705",
        //"185720",
        //"185735",
        //"185750",
        //"185765",
        //"185780",
        //"200630",
        //"200645",
        //"200660",
        //"200675",
        //"200690",
        //"200705",
        //"200720",
        //"200735",
        //"200750",
        //"200765",
        //"200780",
        //"215630",
        //"215645",
        //"215660",
        //"215675",
        //"215690",
        //"215705",
        //"215720",
        //"215735",
        //"215750",
        //"215765",
        //"215780",
        //"230630",
        //"230645",
        //"230660",
        //"230675",
        //"230690",
        //"230705",
        "230720",
        "230735",
        "230750",
        //"230765",
        //"230780",
        //"245630",
        //"245645",
        //"245660",
        //"245675",
        //"245690",
        //"245705",
        "245720",
        "245735",
        "245750",
        //"245765",
        //"245780",
        //"260630",
        //"260645",
        //"260660",
        //"260675",
        //"260690",
        //"260705",
        "260720",
        "260735",
        "260750"
        //"260765",
        //"260780",
        //"275630",
        //"275645",
        //"275660",
        //"275675",
        //"275690",
        //"275705",
        //"275720",
        //"275735",
        //"275750",
        //"275765",
        //"275780"
    };

   const int n = 9; // number of names



   OGRRegisterAll();
   GDALAllRegister();

   Classifier C;
   printf( "Loading classifier training\n\n" );
   //C.inputText("L:\\sdir_snap\\rstelzleni\\Mo2Quads\\classifiertraining\\training.dat");
   C.inputText("/snap/rstelzleni/Mo2Quads/classifiertraining/training.dat");

   DelauneyTriangulator *Triangulator = new QuarticTriangulator;
   RubberSheetTransform *Transformer = new SaalfeldRubberSheet;

   for( int i=0; i<n; ++i )
   {
      // Figure out the names of the raster and vector files to open
      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += szTiffExtension;
      szVector = szVectorDir + aszNames[i];

      // Open the raster file and check for errors
      pRaster = static_cast<GDALDataset *>
                                    ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      // Open the vector file and check for errors
      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);
      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }


      printf( "Input files opened successfully\n\n" );

      // Run the classifier on the raster file
      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );

      // Create the IntersectionMap
      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );

      // Get the original control points
      Intersections.findControlPoints( Rasta, templateSize, areaSize );

      // add the bounding control points
      Intersections.addBoundingControlPoints();
      
      printf( "Saving control points\n" );
      std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir + "Unfiltered/";
      szPoints = szOutput /*+ "Points\\"*/ + aszNames[i] + "_Points.shp";
      szLines = szOutput /*+ "Lines\\"*/ + aszNames[i] + "_Lines.shp";
      szTriangles = szOutput + aszNames[i] + "_Triangles.shp";

      // Open the file to output the original points
      pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
      if( pOutputDS == NULL )
      {
         fprintf( stderr, "pOutputDS is NULL\n" );
         exit( -1 );
      }

      // Open the file to output the original lines
      pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
      if( pLinesDS == NULL )
      {
         fprintf( stderr, "pLinesDS is NULL\n" );
         exit( -1 );
      }

      // Open the file to output the triangles
      pTrianglesDS = createOGRFile( szTriangles.c_str(), "ESRI Shapefile" );
      if( pTrianglesDS == NULL )
      {
         fprintf( stderr, "pTrianglesDS is NULL\n" );
         exit( -1 );
      }

      // Output the original points
      printf( "Outputting base control point set and lines\n" );
      Intersections.outputControlPoints( pOutputDS );

      // Output the triangles
      printf( "Outputting the triangles\n" );
      Intersections.triangulateAndOutput( Triangulator, pTrianglesDS );
      
      // Do the triangulation and transform
      Intersections.triangulateAndAdjust( Triangulator, Transformer,  
                                          pLinesDS ); 

      delete pOutputDS;
      delete pLinesDS;
      delete pTrianglesDS;

      // Now filter the points using all filters and combinations
      for( int r = 50; r <= 50; r += 10 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", r );
         szAbbrev = temp;

         szOutput = szOutputDir + "Filter" ;
         szOutput += szAbbrev + "/";
         szPoints = szOutput /*+ "Points\\"*/ + aszNames[i] + "_Points.shp";
         szLines = szOutput /*+ "Lines\\"*/ + aszNames[i] + "_Lines.shp";
         szTriangles = szOutput + aszNames[i] + "_Triangles.shp";

         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
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

         // Open the file to output the triangles
         pTrianglesDS = createOGRFile( szTriangles.c_str(), "ESRI Shapefile" );
         if( pTrianglesDS == NULL )
         {
            fprintf( stderr, "pTrianglesDS is NULL\n" );
            exit( -1 );
         }


         printf( "VMF Filtering and outputting control points\n\n" );
         Filter *filter = new VMFilter;
         Intersections.setControlPoints( originalPoints );
         Intersections.filter( filter, static_cast<double>(r)/100 );
         Intersections.outputControlPoints( pOutputDS );

         // Output the triangles
         printf( "Outputting the triangles\n" );
         Intersections.triangulateAndOutput( Triangulator, pTrianglesDS );
      
         Intersections.triangulateAndAdjust( Triangulator, Transformer,
                                             pLinesDS );

         delete filter;
         delete pOutputDS;
         delete pLinesDS;

//         sprintf( temp, "_%02dDF", r );
//         szAbbrev = temp;
//
//         szOutput = szOutputDir + "Filter";
//         szOutput += szAbbrev + "\\";
//         szPoints = szOutput + "Points\\" + aszNames[i] + "_Points.shp";
//         szLines = szOutput + "Lines\\" + aszNames[i] + "_Lines.shp";
//
//         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
//         if( pOutputDS == NULL )
//         {
//            fprintf( stderr, "pOutputDS is NULL\n" );
//            exit( -1 );
//         }
//
//         pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
//         if( pLinesDS == NULL )
//         {
//            fprintf( stderr, "pLinesDS is NULL\n" );
//            exit( -1 );
//         }
//
//
//         printf( "DF Filtering and outputting control points\n\n" );
//         filter = new DistanceFilter;
//         Intersections.setControlPoints( originalPoints );
//         Intersections.filter( filter, static_cast<double>(r)/100 );
//         Intersections.outputControlPoints( pOutputDS );
//         Intersections.triangulateAndAdjust( Triangulator, Transformer,
//                                             pLinesDS );
//
//         delete filter;
//         delete pOutputDS;
//         delete pLinesDS;
      } // end for r (ratio)
      
      delete pRaster;
      delete pVectorDS;

      printf( "Finished Area %s\n", aszNames[i] );
   } // end for i

   delete Triangulator;
   delete Transformer;

   return;
}

void profilerTest()
{
   OGRDataSource *pVectorDS, *pOutputDS, *pLinesDS, *pTrianglesDS;
   GDALDataset *pRaster;

   std::string szRaster, szVector, szOutput, szPoints, szLines, szAbbrev,
               szTriangles;

   const std::string szRasterDir = "/snap/ahartman/Data/STL/orthoimages/15SYC";
   const std::string szVectorDir = "/snap/rstelzleni/Mo2Quads/OriginalRoads/";
   const std::string szOutputDir = "/work/Data/Output/profilerTest/";
   const std::string szTiffExtension = ".tif";

   double templateSize = 50, areaSize = 65;


   const char * const aszNames[] = {
//        "230720",
//        "230735",
//        "230750",
//        "245720",
//        "245735",
//        "245750",
        "260720"//,
//        "260735",
//        "260750"
    };

   const int n = 1; // number of names



   OGRRegisterAll();
   GDALAllRegister();

   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText("/snap/rstelzleni/Mo2Quads/classifiertraining/training.dat");

   DelauneyTriangulator *Triangulator = new QuarticTriangulator;
   RubberSheetTransform *Transformer = new SaalfeldRubberSheet;

   for( int i=0; i<n; ++i )
   {
      // Figure out the names of the raster and vector files to open
      printf( "\n\nBeginning area %s\n\n", aszNames[i] );
      szRaster = szRasterDir + aszNames[i];
      szRaster += szTiffExtension;
      szVector = szVectorDir + aszNames[i];

      // Open the raster file and check for errors
      pRaster = static_cast<GDALDataset *>
                                    ( GDALOpen( szRaster.c_str(), GA_ReadOnly ) );
      if( pRaster == NULL )
      {
         fprintf( stderr, "Failed to open raster file\n" );
         exit( -1 );
      }

      // Open the vector file and check for errors
      pVectorDS = OGRSFDriverRegistrar::Open( szVector.c_str(), FALSE, NULL);
      if( pVectorDS == NULL )
      {
         printf("Failed opening vector file\n");
         printf("%d\n", CPLGetLastErrorNo());
         printf(CPLGetLastErrorMsg());
         exit( -1 );
      }


      printf( "Input files opened successfully\n\n" );

      // Run the classifier on the raster file
      printf( "Loading raster image\n" );
      InMemRaster Rasta( pRaster );
      printf( "Converting to HSV\n" );
      Rasta.convertToHSV();
      printf( "Classifying road pixels\n\n" );
      Rasta.classify( C );

      // Create the IntersectionMap
      printf( "Loading intersection data\n" );
      IntersectionMap Intersections( pVectorDS, pRaster );
      printf( "Finding control points\n\n" );

      // Get the original control points
      Intersections.findControlPoints( Rasta, templateSize, areaSize );

      // add the bounding control points
      Intersections.addBoundingControlPoints();
      
      printf( "Saving control points\n" );
      const std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir + "Unfiltered/";
      szPoints = szOutput /*+ "Points\\"*/ + aszNames[i] + "_Points.shp";
      szLines = szOutput /*+ "Lines\\"*/ + aszNames[i] + "_Lines.shp";
      szTriangles = szOutput + aszNames[i] + "_Triangles.shp";

      // Open the file to output the original points
      pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
      if( pOutputDS == NULL )
      {
         fprintf( stderr, "pOutputDS is NULL\n" );
         exit( -1 );
      }

      // Open the file to output the original lines
      pLinesDS = createOGRFile( szLines.c_str(), "ESRI Shapefile" );
      if( pLinesDS == NULL )
      {
         fprintf( stderr, "pLinesDS is NULL\n" );
         exit( -1 );
      }

      // Open the file to output the triangles
      pTrianglesDS = createOGRFile( szTriangles.c_str(), "ESRI Shapefile" );
      if( pTrianglesDS == NULL )
      {
         fprintf( stderr, "pTrianglesDS is NULL\n" );
         exit( -1 );
      }

      // Output the original points
      printf( "Outputting base control point set and lines\n" );
      Intersections.outputControlPoints( pOutputDS );

      // Output the triangles
      printf( "Outputting the triangles\n" );
      Intersections.triangulateAndOutput( Triangulator, pTrianglesDS );
      
      // Do the triangulation and transform
      Intersections.triangulateAndAdjust( Triangulator, Transformer,  
                                          pLinesDS ); 

      delete pOutputDS;
      delete pLinesDS;
      delete pTrianglesDS;

      // Now filter the points using all filters and combinations
      for( int r = 50; r <= 50; r += 10 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", r );
         szAbbrev = temp;

         szOutput = szOutputDir + "Filter" ;
         szOutput += szAbbrev + "/";
         szPoints = szOutput /*+ "Points\\"*/ + aszNames[i] + "_Points.shp";
         szLines = szOutput /*+ "Lines\\"*/ + aszNames[i] + "_Lines.shp";
         szTriangles = szOutput + aszNames[i] + "_Triangles.shp";

         pOutputDS = createOGRFile( szPoints.c_str(), "ESRI Shapefile" );
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

         // Open the file to output the triangles
         pTrianglesDS = createOGRFile( szTriangles.c_str(), "ESRI Shapefile" );
         if( pTrianglesDS == NULL )
         {
            fprintf( stderr, "pTrianglesDS is NULL\n" );
            exit( -1 );
         }


         printf( "VMF Filtering and outputting control points\n\n" );
         Filter *filter = new VMFilter;
         Intersections.setControlPoints( originalPoints );
         Intersections.filter( filter, static_cast<double>(r)/100 );
         Intersections.outputControlPoints( pOutputDS );

         // Output the triangles
         printf( "Outputting the triangles\n" );
         Intersections.triangulateAndOutput( Triangulator, pTrianglesDS );
      
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
