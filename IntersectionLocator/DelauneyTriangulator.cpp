/*
 * File: DelauneyTriangulator.cpp
 * Purpose: This file contains the implementation of an interface to an 
 *          object that encapsulates an algorithm for performing a 
 *          Delauney triangulation.  The output function will be required
 *          by all subclasses, so I just put it in the interface.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */

#include "DelauneyTriangulator.h"
#include "Error.h"

void DelauneyTriangulator::outputTriangles( OGRDataSource *pDS, 
                                            OGRSpatialReference &mToSR,
                                            const Triangulation &triangles )
{
   OGRLayer *pLayer = NULL;
   OGRFeature *pFeature = NULL;
   OGRFeatureDefn Def( "TriangleDefinition" );


   char *papszOptions[] = { "SHPT=ARC" };
   pLayer = pDS->CreateLayer( "LayerOne", &mToSR, wkbUnknown, papszOptions );

   for( int i = 0; i < triangles.size(); ++i )
   {
      // A to B
      OGRLineString lineA;
      lineA.addPoint( triangles[i].triangle.pointA().x(), 
                     triangles[i].triangle.pointA().y() );
      lineA.addPoint( triangles[i].triangle.pointB().x(), 
                     triangles[i].triangle.pointB().y() );
      pFeature = OGRFeature::CreateFeature( &Def );
      pFeature->SetGeometry( &lineA );

      if( pLayer->CreateFeature( pFeature ) != OGRERR_NONE )
         Error_Print( "Error creating triangulation edge\n" );
      delete pFeature;

      // B to C
      OGRLineString lineB;
      lineB.addPoint( triangles[i].triangle.pointB().x(), 
                     triangles[i].triangle.pointB().y() );
      lineB.addPoint( triangles[i].triangle.pointC().x(), 
                     triangles[i].triangle.pointC().y() );
      pFeature = OGRFeature::CreateFeature( &Def );
      pFeature->SetGeometry( &lineB );

      if( pLayer->CreateFeature( pFeature ) != OGRERR_NONE )
         Error_Print( "Error creating triangulation edge\n" );
      delete pFeature;

      // A to C
      OGRLineString lineC;
      lineC.addPoint( triangles[i].triangle.pointA().x(), 
                     triangles[i].triangle.pointA().y() );
      lineC.addPoint( triangles[i].triangle.pointC().x(), 
                     triangles[i].triangle.pointC().y() );
      pFeature = OGRFeature::CreateFeature( &Def );
      pFeature->SetGeometry( &lineC );

      if( pLayer->CreateFeature( pFeature ) != OGRERR_NONE )
         Error_Print( "Error creating triangulation edge\n" );
      delete pFeature;
   } // end for i

   //pDS->SyncToDisk();

   if( pDS->SyncToDisk() != OGRERR_NONE )
      Error_Print( "Error syncing to disk\n" );

   return;
}