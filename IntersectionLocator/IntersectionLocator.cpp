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
// $Id: IntersectionLocator.cpp,v 1.6 2004/11/15 16:45:12 rstelzleni Exp $

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


/** This is a test comment.  Second sentence.  Typing is fun.
 * */
int main( /*int argc, char *argv[]*/ )
{
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

   std::string szRaster, szVector, szOutput, szLines, szAbbrev;

   std::string szRasterDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\Mo2Quads\\Orthos\\15SYC";
   std::string szVectorDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\Mo2Quads\\OriginalAreas\\";
   std::string szOutputDir = "d:\\rstelzleni\\visualstudioprojects\\"
                     "IntersectionLocator\\Mo2Quads\\Tests\\";

   double templateSize = 50, areaSize = 65;

//   char *aszNames[] = { "305945", "305960", "305975", 
//                        "320945", "320960", "320975",
//                        "335945", "335960", "335975" };
   char *aszNames[] = { /*"200675", "200690", "200705",
                        "200720", "200735", "200750",
                        "200765",
                        "215675", "215690", "215705",
                        "215720", "215735", "215750",
                        "215765",
                        "230675", "230690", "230705",
                        "230720", "230735", "230750",
                        "230765",
                        "245675", "245690", "245705",
                        "245720", */"245735"/*, "245750",
                        "245765",
                        "260675", "260690", "260705",
                        "260720", "260735", "260750",
                        "260765",
                        "275675"*/
                        };

   int n = 1;//36; // number of names



   OGRRegisterAll();
   GDALAllRegister();

   Classifier C;
   printf( "Loading classifier training\n\n" );
   C.inputText( "d:\\rstelzleni\\visualstudioprojects\\"
                   "IntersectionLocator\\Mo2Quads\\"
                   "classifiertraining\\testdblrds.dat" );

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
      Intersections.findControlPoints( Rasta, templateSize, areaSize );
      printf( "Saving control points\n" );
      std::vector<ControlPoint> originalPoints = 
                                        Intersections.getControlPoints();

      // Got the control points, now start manipulating the data

      // First dump the unfiltered data and lines
      szOutput = szOutputDir;
      szOutput += "Unfiltered\\";
      szOutput += aszNames[i];
      szLines = szOutput + "_Lines.shp";
      szOutput += "_Points.shp";

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

      // Now filter the points using all filters and combinations
      for( double r = 0.1; r <= 0.5; r = r + 0.1 )
      {
         char temp[10];
         sprintf( temp, "_%02dVMF", static_cast<int>(100*r) );
         szAbbrev = temp;

         szOutput = szOutputDir + "Filter" ;
         szOutput += szAbbrev + "\\";
         szOutput += aszNames[i];
         szLines = szOutput + szAbbrev + "_Lines.shp";
         szOutput += szAbbrev + "_Points.shp";

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

         szOutput = szOutputDir + "Filter";
         szOutput += szAbbrev + "\\";
         szOutput += aszNames[i];
         szLines = szOutput + szAbbrev + "_Lines.shp";
         szOutput += szAbbrev + "_Points.shp";

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

