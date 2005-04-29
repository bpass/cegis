/*
 * File: IntersectionMap.cpp
 * Purpose: This file contains the implementation of the IntersectionMap
 *          class.  This class is used to extract intersections from
 *          a vector format file and relocate those intersections to
 *          match a raster format file.  The file formats must be
 *          supported by gdal.
 * Programmer: Ryan Stelzleni
 * Date: 7-7-04
 *
 * This code was written for the United States Geological Survey.
 */


// Notes:
//
// The control points function uses hard coded threes for transforming
// from meters to pixels.  I should probably fix that.



// Majic numbers for CVS
// $Id: IntersectionMap.cpp,v 1.7 2005/04/29 16:27:36 ahartman Exp $


#include "IntersectionMap.h"
#include <sstream> // for ostringstream


// Locates the intersections and stores intersection information about what
// roads participate in an intersection in the map.
IntersectionMap::IntersectionMap( OGRDataSource *pInDS, GDALDataset *pRasterIn )
{
   OGRFeature *pFeature = NULL;
   OGRLayer *pLayer = NULL;
   OGRLineString *pLine = NULL;
   OGRCoordinateTransformation *poCT = NULL;
   OGRPoint point;      // Creates a point with (0, 0) coordinates

   int numFeatures = 0;
   int numPoints = 0;
   double x, y;
   char *pszWktString = NULL, *pszStringBase;

   pLayer = pInDS->GetLayer( 0 );

   // Store pointers to the data sources
   pRoadsDS = pInDS;
   pRasterDS = pRasterIn;

   // Store the spatial reference information for later.
   mpFromSR = pLayer->GetSpatialRef();
   pszWktString = new char[ 1 + strlen( pRasterIn->GetProjectionRef() ) ];
   pszStringBase = pszWktString;
   strcpy( pszStringBase, pRasterIn->GetProjectionRef() );
   mToSR.importFromWkt( &pszStringBase );

   // This was done because the OGRSpatialReference function moves the string
   // pointer that is passed to it, so I needed to keep track of the beginning
   // of the string so that I could delete it.
   delete [] pszWktString;

   // Create the transformation
   if( mpFromSR != NULL  )
   {
      poCT = OGRCreateCoordinateTransformation( mpFromSR,
                                                &mToSR );
   }

   pTransform = poCT;

   // Extract the points and insert them in the std::map
   numFeatures = pLayer->GetFeatureCount();
   for(int i=0; i < numFeatures; ++i)
   {
      pFeature = pLayer->GetNextFeature();
      pLine = static_cast<OGRLineString *>(pFeature->GetGeometryRef());
      // XXX shouldn't this be a dynamic cast?
      numPoints = pLine->getNumPoints();

      for(int j=0; j < numPoints; ++j)
      {
         pLine->getPoint( j, &point );

         // Transform the coordinates from starting system to end system
         x = point.getX();
         y = point.getY();

         if( poCT != NULL && !poCT->Transform( 1, &x, &y ) )
            Error_Print( "Error transforming coordinates\n" );

         point.setX( x );
         point.setY( y );

         // Insert this street's endpoint to the std::map
         mIntersectMap[point].push_back( std::pair<int, int>(i, j) );
      } // end for j
      
      delete pFeature;
   } // end for i

} // end constructor


IntersectionMap::~IntersectionMap()
{
   delete pTransform;
}


std::vector<ControlPoint> IntersectionMap::getControlPoints() const
{
   return vControl;
}

// Not a reference so that a copy is made (why did I do this?)
void IntersectionMap::setControlPoints( const std::vector<ControlPoint> 
                                        newPoints )
{
   vControl.clear();
   vControl = newPoints;
   return;
}


void IntersectionMap::iterativelyFindControlPoints( InMemRaster &rasta,
                                    std::vector<Indexes> &size )
{
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp>::iterator i;
   std::vector<Indexes>::iterator j;
   std::vector<ControlPoint> vTemp;
   ControlPoint thePoint;
   bool bFirstRun = true;
   int areaWidth, areaHeight; // limits for the area to check
   int x, y, restartX;      // for loop counters
   double startX, startY;   // Top left of the area to sweep over
   double dC, dHiC = -1;    // Current and Highest correlation so far
   int HiCX=0, HiCY=0;      // x and y for the highest 
                            // correlation point in the process.

   // Make sure the vector of control points is empty to start with
   vControl.clear();

   for( j = size.begin(); j != size.end(); ++j )
   {
      // Get the raster area to check in pixels instead of meters.
      // The madAffine positions are according to the documentation for
      // the gdal library.  Actually the affine coefficents don't work
      // like this.  I"m not sure why, the gdal documentation does say
      // that this is how it goes.
      areaWidth = static_cast<int>(j->second * 3);  // / madAffine[1]);
      areaHeight = static_cast<int>(j->second * 3);  // / madAffine[5]);

// Also crap
      // Status counters
      // Now using status as a counter for the control vector
      int status=0, total = mIntersectMap.size();
      //
// End Also crap

      // empty vTemp in case we've already used it once
      vTemp.clear();

      for( i = mIntersectMap.begin(); i != mIntersectMap.end(); ++i )
      {
         // I'm a Hack, this would be much better without a continue
         if( i->second.size() <= INTERSECT_ROAD_LIMIT )
            continue;

// <crap>
          // Debug status print
         printf( "Processing intersection %i of %i\n", status, total );

         // Stop after 20
//         if( status == 20 )
//            break;
// </crap>

         // Set up a template centered on the current point
         // j->first is the size that was passed in in the vector size
         Template T( j->first, j->first, i->first );

         // Populate the template with the roads in the intersection
         fillTemplate( T, static_cast<OGRPoint>(i->first) );

         // Get the starting point and iterate over the area, comparing
         // the template at each location and keeping track of the greatest
         // correlation factor.
         startX = const_cast<OGRPoint&>(i->first).getX();
         startY = const_cast<OGRPoint&>(i->first).getY();
         rasta.vector2Raster( startX, startY );

         // Reset these variables for this iteration.
         dHiC = -(T.xSize()*T.ySize());
         HiCX = 0;
         HiCY = 0;

         // Make sure our template's area to move in is within the bounds
         // of the raster.  The correlate function should handle the case
         // of going over the top of the raster.
         if( (startY - areaHeight/2) > 0 )
            y = static_cast<int>(startY - areaHeight/2);
         else
            y = 0;
         if( (startX - areaWidth/2) > 0 )
            restartX = static_cast<int>(startX - areaWidth/2);
         else
            restartX = 0;

         // These loop limits are to guarantee that the template stays within
         // the raster area being inspected.
         for( ; y < static_cast<int>( startY + areaHeight - T.ySize() ); ++y )
         {
            for( x = restartX; 
                 x < static_cast<int>( startX + areaWidth - T.xSize() ); ++x )
            {
               // correlate is n^2 in the size of the template, this is the
               // part of the code that is really slow.
               dC = correlate( rasta, T, x, y );

               if( dC > dHiC )
               {
                  dHiC = dC;
                  HiCX = x;
                  HiCY = y;
               }
            }
         }

         // Get the location of the center of the template in projected
         // coordinates
         startX = static_cast<double>(HiCX) + T.xSize()/2; //areaWidth/2;
         startY = static_cast<double>(HiCY) + T.ySize()/2; //areaHeight/2;
         rasta.raster2Vector( startX, startY );
      
         // Create the control point
         thePoint.point.setX( startX );
         thePoint.point.setY( startY );
         thePoint.origPoint = static_cast<OGRPoint>(i->first);
         thePoint.correlation = dHiC;

         // First time through just populate the vector, second time look
         // for better correlations.
         if( bFirstRun )
            vControl.push_back( thePoint );
         else 
            vTemp.push_back( thePoint );

         ++status;
      } // End for i
      
      if( bFirstRun )
         bFirstRun = false;
      else
         pickHighestCorrelation( vTemp );

   } // End for j (the size vector iterator)
   
   return;
} // End Function iterativelyFindControlPoints



void IntersectionMap::pickHighestCorrelation( 
                                     std::vector<ControlPoint> &vNewVals )
{
   double dTotalCControl = 0, dTotalCNew = 0;
   unsigned int i;

   for( i = 0; i<vControl.size(); ++i )
   {
      dTotalCControl += vControl[i].correlation;
      dTotalCNew += vNewVals[i].correlation;
   }

   for( i = 0; i<vControl.size(); ++i )
   {
      // This normalizes the newval's correlation value, since the
      // total of correlation values can vary.
      if( ( vNewVals[i].correlation * dTotalCControl / dTotalCNew )  > 
            vControl[i].correlation )
      {
         vControl[i] = vNewVals[i];
      }
   } // End for i

   return;
} // End function pickHighestCorrelation



void IntersectionMap::findControlPoints( InMemRaster &rasta,
                                         const double &tempSize,
                                         const double &rasterDim)
{
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp>::iterator i;
   ControlPoint thePoint;
   int areaWidth, areaHeight; // limits for the area to check
   int x, y, restartX;      // for loop counters
   double startX, startY;   // Top left of the area to sweep over
   double dC, dHiC = -1;    // Current and Highest correlation so far
   int HiCX=0, HiCY=0;      // x and y for the highest 
                            // correlation point in the process.

   // Get the raster area to check in pixels instead of meters.
   // The madAffine positions are according to the documentation for
   // the gdal library.  Actually the affine coefficents don't work
   // like this.  I"m not sure why, the gdal documentation does say
   // that this is how it goes.
   areaWidth = static_cast<int>(rasterDim * 3);  //madAffine[1]);
   areaHeight = static_cast<int>(rasterDim * 3);  //madAffine[5]);

   // Status counters
   int status=0, total = mIntersectMap.size();
   //

   // Make sure the vector of control points is clear
   vControl.clear();

   for( i = mIntersectMap.begin(); i != mIntersectMap.end(); ++i )
   {
      // I'm a Hack, this would be much better without a continue
      if( i->second.size() <= INTERSECT_ROAD_LIMIT )
         continue;

// <crap>
       // Debug status print
      printf( "Processing intersection %i of %i\n", ++status, total );
// </crap>

      // Set up a template centered on the current point
      Template T( static_cast<int>(tempSize), static_cast<int>(tempSize), i->first );

      // Populate the template with the roads in the intersection
      fillTemplate( T, static_cast<OGRPoint>(i->first) );

      // Get the starting point and iterate over the area, comparing
      // the template at each location and keeping track of the greatest
      // correlation factor.
      startX = const_cast<OGRPoint&>(i->first).getX();
      startY = const_cast<OGRPoint&>(i->first).getY();
      rasta.vector2Raster( startX, startY );

      // Reset these variables for this iteration.
      // dHiC is set to the minimum possible value if all points on the
      // template result in a -1 modifier to the correlation
      dHiC = -(T.xSize()*T.ySize());
      HiCX = 0;
      HiCY = 0;

      // Make sure our template's area to move in is within the bounds
      // of the raster.  The correlate function should handle the case
      // of going over the top of the raster.
      if( (startY - areaHeight/2) > 0 )
         y = static_cast<int>(startY - areaHeight/2);
      else
         y = 0;
      if( (startX - areaWidth/2) > 0 )
         restartX = static_cast<int>(startX - areaWidth/2);
      else
         restartX = 0;

      // These loop limits are to guarantee that the template stays within
      // the raster area being inspected.
      for( ; y < ( startY + areaHeight/2 - T.ySize()/2 ); y = y + 4 )
      {
         for( x = restartX; x < ( startX + areaWidth/2 - T.xSize()/2 ); 
              x = x + 4 )
         {
            // correlate has O(templatewidth*templateheight) time complexity
            // this is the part of the code that is really slow.
            dC = correlate( rasta, T, x, y );

            if( dC > dHiC )
            {
               dHiC = dC;
               HiCX = x;
               HiCY = y;
            }
         }
      }

      // Get the location of the center of the template in projected
      // coordinates
      startX = static_cast<double>(HiCX) + 
               static_cast<double>(T.xSize())/2;
      startY = static_cast<double>(HiCY) + 
               static_cast<double>(T.ySize())/2;
      rasta.raster2Vector( startX, startY );
      
      // Create the control point
      thePoint.point.setX( startX );
      thePoint.point.setY( startY );
      thePoint.origPoint = static_cast<OGRPoint>(i->first);
      thePoint.correlation = dHiC;
      vControl.push_back( thePoint );
   } // End for i  
   
   return;
} // End Function findControlPoints



double IntersectionMap::correlate( InMemRaster &rasta, Template &T, 
                                   int X, int Y )
{
   double numMatch = 0, numDiff = 0;

   if( X < 0 || Y < 0 || X >= rasta.getXSize() || Y >= rasta.getYSize() )
      return 0;

   for( int y=0; y < T.ySize(); ++y )
   {
      for( int x=0; x < T.xSize(); ++x )
      {
         if( (Y + y) < rasta.getYSize() && (X + x) < rasta.getXSize() )
         {
            if( T.isRoad( x, y ) && rasta.isRoad( X+x, Y+y ) )
               ++numMatch;
         }
      } // End for x
   } // End for y

   return numMatch + numDiff;
}

// Commented this out and went with the simpler version above.  They seem
// to give the same output, but I saved this one in case some oddity comes
// up.  
// Denominator stuff is commented out to help solve a problem with identifying
// control points near image borders.

/*   // The numerator, denominator for the template information, and
   // the denominator for the raster information
   double iNumer = 0;//, iDenomT = 0, iDenomI = 0;
   // The overall denominator
   //double iDenom;

   // So we don't have to worry about these locations on rasta
   if( X < 0 || Y < 0 || X >= rasta.getXSize() || Y >= rasta.getYSize() )
      return 0;

   for( int y=0; y < T.ySize(); ++y )
   {
      for( int x=0; x < T.xSize(); ++x )
      {
         if( (Y + y) < rasta.getYSize() && (X + x) < rasta.getXSize() )
         {
           // if( T.isRoad( x, y ) )
           //    ++iDenomT;
           // if( rasta.isRoad( X+x, Y+y ) )
           //    ++iDenomI;
            if( T.isRoad( x, y ) && rasta.isRoad( X+x, Y+y ) )
               ++iNumer;
         }
      } // End for x
   } // End for y

   // If either of the denominator's factors are zero then we return 0.
   // Also, if the numerator is 0 then the fraction is also zero.
//   if( iDenomT == 0 || iDenomI == 0 || iNumer == 0 )
//      return 0; 

//   iDenomT *= iDenomT;
//   iDenomI *= iDenomI;
//   iDenom = sqrt( iDenomT*iDenomI );

   return ( iNumer );/// iDenom );
} // End Function correlate
*/


void IntersectionMap::triangulateAndOutput( 
                                   DelauneyTriangulator *triangulator,
                                   OGRDataSource *pDS )
{
   Triangulation edges;
   triangulator->triangulate( vControl, edges );
   triangulator->outputTriangles( pDS, mToSR, edges );
   return;
}


void IntersectionMap::adjustToControl( DelauneyTriangulator *triangulator,
                                       RubberSheetTransform *transformer,
                                       Filter *filter,
                                       const double &ratio,
                                       OGRDataSource *pOutDS )
{
   Triangulation edges;
   if( !triangulator || !transformer )
      return;

   if( filter )
      filter->filter( vControl, ratio );

   triangulator->triangulate( vControl, edges );
   transformer->doTransformation( pOutDS, pRoadsDS, edges, vControl );
   return;
}


void IntersectionMap::filter( Filter *F, const double &ratio )
{
   if( F )
      F->filter( vControl, ratio );

   return;
}

void IntersectionMap::triangulateAndAdjust( DelauneyTriangulator *Tri,
                                            RubberSheetTransform *Tra,
                                            OGRDataSource *pOutDS )
{
   if( !Tri || !Tra )
      return;

   Triangulation edges;
   Tri->triangulate( vControl, edges );
   Tra->doTransformation( pOutDS, pRoadsDS, edges, vControl );
   return;
}

// outputs intersections to a file, this is for testing purposes.
void IntersectionMap::outputIntersections( OGRDataSource *pDS )
{
   // This is ugly, but I need an iterator
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp>::iterator iter;
   OGRLayer *pLayer = NULL;
   OGRFeature *pFeature = NULL;
   OGRFeatureDefn Def( "IntersectionDefinition" );
//   OGRCoordinateTransformation *poCT = NULL;


   char *papszOptions[] = { "SHPT=POINT" };
   pLayer = pDS->CreateLayer( "LayerOne", &mToSR, wkbUnknown, papszOptions );

   for( iter = mIntersectMap.begin(); iter != mIntersectMap.end(); ++iter )
   {
      if( iter->second.size() > INTERSECT_ROAD_LIMIT )
      {
         pFeature = OGRFeature::CreateFeature( &Def );
         // iter->first is the key (OGRPoint)
         // This function only copies the geometry, so it is safe to 
         // cast from const to non-const.
         pFeature->SetGeometry( (OGRPoint *)(&(iter->first)) );

         if( pLayer->CreateFeature( pFeature ) != OGRERR_NONE )
            Error_Print( "Error creating intersection point\n" );
         
         delete pFeature;
      }
   } // end for iter

   pDS->SyncToDisk();

   if( pDS->SyncToDisk() != OGRERR_NONE )
      Error_Print( "Error syncing to disk\n" );
} // end outputIntersections



// outputs control points to a file, this is for testing purposes.
void IntersectionMap::outputControlPoints( OGRDataSource *pDS )
{
   // This is ugly, but I need an iterator
   std::vector<ControlPoint>::iterator iter;
   //std::string textfile;
   //int count = 0;
   //FILE *textout;
   OGRLayer *pLayer = NULL;
   OGRFeature *pFeature = NULL;
   OGRFeatureDefn Def( "IntersectionDefinition" );
   //OGRCoordinateTransformation *poCT = NULL;

 /*  textfile = pDS->GetName();
   textfile += ".txt";
   textout = fopen( textfile.c_str(), "wt" );
   if( !textout )
   {
      Error_Print( "Failed opening correlation text output file\n" );
      return;
   }
*/

   char *papszOptions[] = { "SHPT=POINT" };
   pLayer = pDS->CreateLayer( "LayerOne", &mToSR, wkbUnknown, papszOptions );

   for( iter = vControl.begin(); iter != vControl.end(); ++iter )
   {
      pFeature = OGRFeature::CreateFeature( &Def );
      // This function only copies the geometry, so it is safe to 
      // cast from const to non-const.
      pFeature->SetGeometry( &(iter->point) );
      
      if( pLayer->CreateFeature( pFeature ) != OGRERR_NONE )
         Error_Print( "Error creating intersection point\n" );
      
      delete pFeature;

      //fprintf( textout, "%i: %f\n", count++, iter->correlation );
   } // end for iter

   //pDS->SyncToDisk();

   if( pDS->SyncToDisk() != OGRERR_NONE )
      Error_Print( "Error syncing to disk\n" );
} // end outputControlPoints


// This assumes that we have a single layer vector file
// Also note that you should delete the OGRLineString that is returned.
OGRLineString *IntersectionMap::getLine( int i, std::string &type )
{
   OGRLayer *pLayer = pRoadsDS->GetLayer( 0 );
   OGRFeature * pFeature;
   OGRLineString *pLine;

   if( pLayer->GetFeatureCount() <= i || i < 0 )
   {
      Error_Print( "Line number out of bounds in getLine.\n" );
      return NULL;
   }

   // Make sure we start on the first feature
   pLayer->ResetReading();

   // Unfortunately the only way that OGR provides for getting a specific
   // feature from a layer is to iterate through it, and the only way to
   // iterate through returns memory that you have to delete.  Hence this
   // crappy loop.
   for( int j=0; j<i; ++j )
   {
      pFeature = pLayer->GetNextFeature();
      delete pFeature;
   }
   pFeature = pLayer->GetNextFeature();

   // Get the road's type
   type = pFeature->GetFieldAsString( pFeature->GetFieldIndex( "Cfcc" ) );

   pLine = static_cast<OGRLineString *>(pFeature->StealGeometry());
   delete pFeature;

   return pLine;//static_cast<OGRLineString *>(pFeature->GetGeometryRef());
} // End Function getLine


// the type string should be a TIGER line Cfcc code
double IntersectionMap::roadWidth( std::string type )
{
   double width = DEFAULT; // width is in meters

   // If it is a road type
   if( type[0] == 'A' )
   {
      switch( type[1] )
      {
      // Unkown road type
      case '0':  
         width = A0SIZE; 
         break;
      // Primary highway with limited access
      case '1':  
         if( type[2] == '0' || type[2] == '1' || 
             type[2] == '4' || type[2] == '9' )
         {       // unseparated highway or bridge
            width = A1SIZE;
         }
         else if( type[2] == '5' || type[2] =='8' )
         {       // separated highway
            width = static_cast<double>(A1SIZE) / 2;
         }
         // Cfcc's 2,3,6,7 are for tunnels and underpasses,so these
         // will be given widths of 0.
         break;
      // Primary road without limited access
      case '2':  
         if( type[2] == '0' || type[2] == '1' || 
             type[2] == '4' || type[2] == '9' )
         {       // unseparated highway or bridge
            width = A2SIZE;
         }
         else if( type[2] == '5' || type[2] =='8' )
         {       // separated highway
            width = static_cast<double>(A2SIZE) / 2;
         }
         // Cfcc's 2,3,6,7 are for tunnels and underpasses,so these
         // will be given widths of 0.
         break;
      // Secondary and connecting road
      case '3':  
        if( type[2] == '0' || type[2] == '1' || 
            type[2] == '4' || type[2] == '9' )
         {       // unseparated highway or bridge
            width = A3SIZE;
         }
         else if( type[2] == '5' || type[2] =='8' )
         {       // separated highway
            width = static_cast<double>(A3SIZE) / 2;
         }
         // Cfcc's 2,3,6,7 are for tunnels and underpasses,so these
         // will be given widths of 0.
         break;
      // Local, neighborhood and rural road
      case '4':  
         width = A4SIZE;
         break;
      // Vehicular trail
      case '5':  
         width = A5SIZE;
         break;
      // Road with special characteristics
      case '6':  
         if( type[2] != '6' && type[2] != '7' )
         {              // 6 and 7 are obstuctions, like gated barriers
            width = A6SIZE;  // Probably a highway ramp
         }
         break;
      // This would be a foot trail or other non paved route.
      // case '7': 
      } // End switch case

   } // End if

   return width;
} // End function roadWidth


// The template should have been initialized with a point on this line
// (preferably the intersection) or it will most likely not show.
void IntersectionMap::fillTemplate( Template &T, 
                                    const OGRPoint &intersection )
{
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp>::iterator iter;
   std::string type;
   OGRLineString *pLine;
   OGRPoint from, dest;

   iter = mIntersectMap.find( intersection );

   for( unsigned int x=0; x<iter->second.size(); ++x )
   {
      // iter->second[x].first is the number of the line
      // This funcion will put a string into type to identify the road type
      pLine = getLine( iter->second[x].first, type );

      // Grab and plot each line segment in the template image
      pLine->getPoint( 0, &from );
      for( int y=1; y<pLine->getNumPoints(); ++y )
      {
         pLine->getPoint( y, &dest );
         T.addSegment( from, dest, roadWidth( type ) );
         from.setX( dest.getX() );
         from.setY( dest.getY() );
      }
      delete pLine;
   }

   return;
} // End function fillTemplate


void IntersectionMap::templateTest( int i, const char *szFilename )
{
   std::map<OGRPoint, std::vector<Indexes>, OGRPointComp>::iterator iter;
   std::string file;
   std::string type;

   std::string temp;
   int count = 0;
 
   OGRLineString *pLine;
   OGRPoint from, dest;

   for( iter = mIntersectMap.begin(); iter != mIntersectMap.end(); ++iter )
   {
      if( iter->second.size() > INTERSECT_ROAD_LIMIT )
      {
         printf( "%f, %f are the point's coordinates\n", 
                 const_cast<OGRPoint&>(iter->first).getX(), 
                 const_cast<OGRPoint&>(iter->first).getY() );
         printf( "%i lines in this intersection\n", iter->second.size() );
         Template T( 20, 20, iter->first );

         for( unsigned int x=0; x<iter->second.size(); ++x )
         {
            // iter->second[x].first is the number of the line
            pLine = getLine( iter->second[x].first, type );

            fprintf( stderr, "\nLine # %i has %i points\n", x+1,
                     pLine->getNumPoints() );

            // Grab and plot each line segment in the template image
            pLine->getPoint( 0, &from );
            for( int y=1; y<pLine->getNumPoints(); ++y )
            {
               pLine->getPoint( y, &dest );
               T.addSegment( from, dest, roadWidth( type ) );
               from.setX( dest.getX() );
               from.setY( dest.getY() );
            }
            fprintf( stderr,  "Done printing line\n\n" );
            delete pLine;
         }
         file = szFilename;
         {
             std::ostringstream output_string;
             output_string << count;
             temp = output_string.str();
         }
         
         file += temp;
         file += ".txt";
         T.dumpToFile( file.c_str() );
         if( count == 0 )
         {
            file = szFilename;
            file += "0reverse.txt";
            FILE *out = fopen( file.c_str(), "wt" );

            for( int q=0; q<60; ++q )
            {
               for( int w=0; w<60; ++w )
               {
                  fprintf( out, "%i ", T.isRoad( w, q ) );
               }
               fprintf( out, "\n" );
            }
         }
         ++count;
      }

   } // end for iter

   return;
} // End function templateTest

class LessControlPointX
{
public:
    bool operator()(const ControlPoint& p1, const ControlPoint& p2) const;
};

class LessControlPointY
{
public:
    bool operator()(const ControlPoint& p1, const ControlPoint& p2) const;
};

bool
LessControlPointX::operator()(const ControlPoint& p1, 
                              const ControlPoint& p2) const
{
    return p1.point.getX() < p2.point.getX();
}

bool
LessControlPointY::operator()(const ControlPoint& p1, 
                              const ControlPoint& p2) const
{
    return p1.point.getY() < p2.point.getY();
}

void
IntersectionMap::addBoundingControlPoints()
{
    // get some iterators that point to the elements in vControl that have
    // the minimum and maximum x- and y-coordinates
    const std::vector<ControlPoint>::iterator maxXPoint = 
        std::max_element( vControl.begin(), vControl.end(), 
                          LessControlPointX() );
    const std::vector<ControlPoint>::iterator maxYPoint = 
        std::max_element( vControl.begin(), vControl.end(), 
                          LessControlPointY() );
    const std::vector<ControlPoint>::iterator minXPoint = 
        std::min_element( vControl.begin(), vControl.end(), 
                          LessControlPointX() );
    const std::vector<ControlPoint>::iterator minYPoint = 
        std::min_element( vControl.begin(), vControl.end(), 
                          LessControlPointY() );

    // get those max and min x and y values
    const double maxX = maxXPoint->point.getX();
    const double maxY = maxYPoint->point.getY();
    const double minX = minXPoint->point.getX();
    const double minY = minYPoint->point.getY();

    const double dX = maxX - minX;
    const double dY = maxY - minY;

    // find x and y coordinates that will bound all of those points
    const double maxBoundingX = minX + dX * 4;
    const double maxBoundingY = minY + dY * 4;
    const double minBoundingX = minX - dX;
    const double minBoundingY = minY - dY;

    // create the new control points and add them to the vector
    const double correlationValue = 1.0; // XXX not sure what should go here
    
    ControlPoint cp1;
    cp1.point.setX( minBoundingX ); 
    cp1.point.setY( minBoundingY );
    cp1.origPoint = cp1.point;
    cp1.correlation = correlationValue;
    vControl.push_back( cp1 );

    ControlPoint cp2;
    cp2.point.setX( maxBoundingX ); 
    cp2.point.setY( minBoundingY );
    cp2.origPoint = cp2.point;
    cp2.correlation = correlationValue;
    vControl.push_back( cp2 );

    ControlPoint cp3;
    cp3.point.setX( minBoundingX ); 
    cp3.point.setY( maxBoundingY );
    cp3.origPoint = cp3.point;
    cp3.correlation = correlationValue;
    vControl.push_back( cp3 );
}

