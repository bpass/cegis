/*!
 *
 * \author Mark Schisler
 *
 * \file driver.cpp
 *
 * \date $date$
 *
 * \brief Purpose is to be a temporary test driver for the mosix
 * fast projections project.
 *
 *
 */

#include <iostream>
#include <string>
#include <list>
#include <ProjectionIO/ProjectionReader.h>
#include <ProjectionIO/ProjectionWriter.h>
#include "ProjImageIn.h"
#include "ProjImageInInterface.h"
#include "ProjImageParams.h"
#include "ProjImageFactory.h"
#include "FromMultiGeoProjector.h"
using namespace USGSMosix;

void cleanupPtrList( std::list<ProjImageParams* > & l ) 
{
    for(std::list<ProjImageParams*>::iterator i = l.begin(); i != l.end(); ++i)
        if ( *i != NULL ) delete *i;

    return;
}

void projectInformation() 
{
     std::cout << "---------------------------------------------" << std::endl
               << "USGS: MOSIX Fast-Reprojections Project" << std::endl
               << "version 0.1b" << std::endl
               << std::endl
               << "Authors: Chris Bilderback, Ha Thi Dam, " << std::endl
               << "Mary Beth Danuser, Mark Schisler" << std::endl
               << std::endl
               << "Mid-Continent Mapping Center, 2005" << std::endl
               << "(Consult COPYING for license information)." << std::endl
               << "---------------------------------------------" 
               << std::endl << std::endl;
     
     return;
}

std::string usageInformation()
{
    return "usage: reproject [input parameter file(s)] [output parameter file]";
}

int main(int argc, char** argv )
{
    ProjectorInterface * projector = NULL;
    ProjImageParams * outputFileParams = NULL;
    std::list<ProjImageParams* > inputFiles;  // contains parameter filenames
    ProjImageInInterface * input = NULL;
   
    try { 
        // objects for projector
        ProjIOLib::ProjectionReader projReader;
        ProjIOLib::ProjectionWriter projWriter;
        USGSMosix::ProjImageFactory imageFactory(projReader, projWriter);
        USGSMosix::ProjImageInInterface* inputImageList;
        
        // factory for creating 

        // output program information and usage information.
        projectInformation();    
       
        // no parameters forwarded or only one.  Neither
        // make any sense.
        if ( argc < 3 ) 
            throw GeneralException(usageInformation()); 
        
        // iterate through the input files
        for ( int i = 1; i < argc - 1; ++i )
        {
           inputFiles.push_back( new ProjImageParams( std::string(argv[i]), 
                                                      ProjImageParams::INPUT ));

           input = new ProjImageIn( *(*inputFiles.begin()), projReader ); 
           
        }

        // assign output parameter files
        outputFileParams = new ProjImageParams( std::string(argv[argc - 1]), 
                                                ProjImageParams::OUTPUT );
        
        inputImageList = imageFactory.makeProjImageInList(inputFiles); 
       
        if ( inputImageList != NULL  && outputFileParams != NULL )
        {
            projector = new FromMultiGeoProjector( projWriter,
                                                   projReader,
                                                   *outputFileParams,
                                                   *input
                                                   /* WAS: *inputImageList */ );
            projector->project();
        }

    } catch ( GeneralException & e )
    {
        std::cout << e.toString() << std::endl;
    }

    if ( projector != NULL ) delete projector;
    if ( outputFileParams != NULL ) delete outputFileParams;
    cleanupPtrList(inputFiles);

    return 0;
}
