/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageData.h 
 * 
 * \brief Implementation file for ProjImageData class. 
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
:  m_proj( proj ),
   m_file( file ),
   m_boundaries( bounds ),
   m_photo(0), 
   m_bps(0),
   m_spp(0),
   m_width(0),
   m_height(0),
   m_filename("")
{

}

/******************************************************************************/

ProjImageData::~ProjImageData()
{
    delete m_file;
    std::list<PmeshLib::ProjectionMesh*>::iterator it = m_meshes.begin();
    for(; it != m_meshes.end(); ++it)
        delete *it;
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
    {
        throw GeneralException(filename + "is not a Filename.");
    }

    return extension;
}

/******************************************************************************/

const PmeshLib::ProjectionMesh & ProjImageData::setupMesh(
        const ProjLib::Projection & toProjection,
        unsigned int divisions, 
        MathLib::InterpolatorType interp )const  
 { 
    PmeshLib::ProjectionMesh * mesh; 
    mesh = new(std::nothrow) PmeshLib::ProjectionMesh;
    m_meshes.push_back(mesh);    
    try { 
    
        if ( m_proj == NULL || mesh == NULL  ) 
            throw GeneralException("NULL in setting up Mesh.");
    
        // the mesh boundaries are for the current image, since that
        // is where we are coming from.
        mesh->setSourceMeshBounds( getLeftBound(),
                                   getBottomBound(),
                                   getRightBound(),
                                   getTopBound() );
    
        // set the number of divisions in the x and y directions.
        mesh->setMeshSize( divisions, divisions );

        // set the type of interpolation to be used in the mesh 
        mesh->setInterpolator( interp );

        // create the mesh.
        mesh->calculateMesh( *(m_proj), toProjection );
        
    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
        throw; 
    }
   
    return *(mesh);
 } 

/*****************************************************************************/

const PmeshLib::ProjectionMesh & ProjImageData::setupReverseMesh(
        const ProjLib::Projection & fromProjection,
        const DRect & boundaries,
        unsigned int divisions, 
        MathLib::InterpolatorType interp )const
 {
    PmeshLib::ProjectionMesh * mesh; 
    mesh = new(std::nothrow) PmeshLib::ProjectionMesh;
    m_meshes.push_back(mesh);
    try { 
    
        if ( m_proj == NULL || mesh == NULL  ) 
            throw GeneralException("NULL in setting up Mesh.");
        
        // the mesh boundaries are for the destination image,
        // since that is where we are coming from.
        mesh->setSourceMeshBounds( boundaries.left,
                                   boundaries.bottom,
                                   boundaries.right,
                                   boundaries.top );
        
        // set the number of divisions in the x and y directions.
        mesh->setMeshSize( divisions, divisions );
        
        // set the type of interpolation to be used.
        mesh->setInterpolator( interp );

        // create the mesh.
        mesh->calculateMesh( fromProjection , *(m_proj));
        
    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
        throw;
    }
   
    return *(mesh);
 } 

/*****************************************************************************/

} // namespace USGSMosix
