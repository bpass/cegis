/*
 * File: SaalfeldRubberSheet.cpp
 * Purpose: This file contains the implementation of a class that
 *          implements the RubberSheetTransform interface using
 *          the rubber sheeting algorithm described in Alan
 *          Saalfeld's paper "A Fast Rubber-Sheeting Transformation
 *          Using Simplicial Coordinates".
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-20-04
 */


#include "SaalfeldRubberSheet.h"



// Note that it would be a problem if the pInDS wasn't a layer of
// OGRLineStrings.
void SaalfeldRubberSheet::doTransformation( OGRDataSource *pOutDS,
                                  OGRDataSource *pInDS,
                                  const Triangulation &triangles,
                                  const std::vector<ControlPoint> &points )
{
   OGRFeature *pFeature = NULL;
   OGRLayer *pInLayer = NULL, *pOutLayer = NULL;
   OGRLineString *pLine = NULL;
   OGRPoint thePoint;
   int numFeatures, numPoints, numTriangles, triIndex;
   double s1, s2, s3;  // simplicial coordinates for points

   initVals( triangles, points );

   numTriangles = triangles.size();

   // Init the input layer
   pInLayer = pInDS->GetLayer( 0 ); // Get the map to be transformed
   numFeatures = pInLayer->GetFeatureCount();

   // Init the output layer
   char *papszOptions[] = { "SHPT=ARC" };
   pOutLayer = pOutDS->CreateLayer( "LayerOne", pInLayer->GetSpatialRef(), 
                                    wkbUnknown, papszOptions );

   
   // Grab each line and perform the necessary transformations
   pInLayer->ResetReading();
   for( int i=0; i<numFeatures; ++i )
   {
      pFeature = pInLayer->GetNextFeature();
      pLine = static_cast<OGRLineString *>( pFeature->StealGeometry() );
      
      // Process and adjust each point in the line
      numPoints = pLine->getNumPoints();
      for( int j=0; j<numPoints; ++j )
      {
         pLine->getPoint( j, &thePoint );
         
         // find the triangle that contains this point
         // and do some useful calculation in the meantime
         for( triIndex=0; triIndex<numTriangles; ++triIndex )
         {
            s1 = val[triIndex].A1*thePoint.getY() + 
                 val[triIndex].B1*thePoint.getX() + val[triIndex].C1;
            s2 = val[triIndex].A2*thePoint.getY() +
                 val[triIndex].B2*thePoint.getX() + val[triIndex].C2;

            if( s1 >= 0 && s2 >= 0 && (s1 + s2) <= 1 )
               break;
         }

         if( triIndex == numTriangles )
         {
            Error_Print( "This point isn't contained in the"
                         " triangulation,\n" );
            continue;
         }

         // Now we know what triangle to look in, we can transform
         s3 = 1.0 - s1 - s2;

         thePoint.setX( s1*points[ triangles[triIndex].a ].point.getX() +
                        s2*points[ triangles[triIndex].b ].point.getX() +
                        s3*points[ triangles[triIndex].c ].point.getX() );
         thePoint.setY( s1*points[ triangles[triIndex].a ].point.getY() +
                        s2*points[ triangles[triIndex].b ].point.getY() +
                        s3*points[ triangles[triIndex].c ].point.getY() );

         // Place the transformed point back in the linestring
         pLine->setPoint( j, &thePoint );
      } // end for j

      // Write the modified feature to the output layer
      pFeature->SetGeometryDirectly( pLine );
      if( pOutLayer->CreateFeature( pFeature ) != OGRERR_NONE )
      {
         Error_Print( "Error creating adjusted output feature.\n" );
      }
         
      delete pFeature;
   } // end for i

   // Save the new file
   if( pOutDS->SyncToDisk() != OGRERR_NONE )
      Error_Print( "Error syncing to disk\n" );

   return;
} // end function doTransformation


void SaalfeldRubberSheet::initVals( const Triangulation &triangles,
                                    const std::vector<ControlPoint> &points)
{
   int n = triangles.size();
   double T;

   val.clear();
   val.resize( n );

   for( int i=0; i < n; ++i )
   {
      // Find the A's, B's and C's suggested by Saalfeld for each triangle
      // I called the array val because I wasn't sure exactly what to call
      // these numbers.  They are related to some calculations with
      // simplicial coordinates.
      Point pOne( points[ triangles[i].a ].origPoint.getX(), 
                  points[ triangles[i].a ].origPoint.getY() );
      Point pTwo( points[ triangles[i].b ].origPoint.getX(),
                  points[ triangles[i].b ].origPoint.getY() );
      Point pThr( points[ triangles[i].c ].origPoint.getX(),
                  points[ triangles[i].c ].origPoint.getY() );

      T = pOne.x()*pTwo.y() + pTwo.x()*pThr.y() + pThr.x()*pOne.y() -
          pThr.x()*pTwo.y() - pTwo.x()*pOne.y() - pOne.x()*pThr.y();

      val[i].A1 = (pThr.x() - pTwo.x()) / T;
      val[i].B1 = (pTwo.y() - pThr.y()) / T;
      val[i].C1 = (pTwo.x()*pThr.y() - pThr.x()*pTwo.y()) / T;

      val[i].A2 = (pOne.x() - pThr.x()) / T;
      val[i].B2 = (pThr.y() - pOne.y()) / T;
      val[i].C2 = (pThr.x()*pOne.y() - pOne.x()*pThr.y()) / T;
   }

   return;
} // end function initVals