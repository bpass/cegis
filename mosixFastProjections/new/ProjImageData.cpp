/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageData.h 
 * 
 * \brief The ProjImageData object is meant exploit commonality
 * between ProjImage classes and store the data and implementations
 * of many of the methods used by them.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <iostream>
#include "ProjImageData.h"
#include "GeneralException.h"

namespace USGSMosix {

/******************************************************************************/

ProjImageData::ProjImageData( DRect bounds, 
                              USGSImageLib::ImageFile * file, 
                              const ProjLib::Projection * proj )
:  m_file( file ),
   m_proj( proj ),
   m_boundaries( bounds )
{

}

/******************************************************************************/

ProjImageData::~ProjImageData()
{
    std::cout << "proj image data destructor called." << std::endl;
    if ( m_file != NULL )
    {
        std::cout << "deleting m_file" << std::endl;
        delete m_file;
    }
}

/******************************************************************************/

std::string ProjImageData::getFileExtension( const std::string& filename )
{
    std::string extension;
    std::string::size_type pos;
    
    if ((pos = filename.find(".")) != std::string::npos)
    {
       extension = filename.substr(++pos,std::string::npos);
    } else 
        throw GeneralException("Not a Filename.");

    return extension;
}

/*****************************************************************************/
const PmeshLib::ProjectionMesh & ProjImageData::setupMesh(
        const ProjLib::Projection & secondProjection,
        unsigned int divisions, 
        MathLib::InterpolatorType interp ) 
 { 
    PmeshLib::ProjectionMesh* mesh=
        new(std::nothrow) PmeshLib::ProjectionMesh;
    
    try { 
    
        if ( m_proj == NULL || mesh == NULL  ) 
            throw GeneralException("NULL in setting up Mesh.");
        
        mesh->setSourceMeshBounds( getLeftBound(),
                                   getBottomBound(),
                                   getRightBound(),
                                   getTopBound() );
        
        mesh->setMeshSize( divisions, divisions );
        mesh->setInterpolator( interp );
        mesh->calculateMesh( *(m_proj), secondProjection );
        
    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
    }
   
    return *(mesh);
 } 

} // namespace USGSMosix
