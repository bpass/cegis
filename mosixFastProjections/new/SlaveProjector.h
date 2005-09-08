#ifndef __USGSMOSIX_SLAVEPROJECTOR_H_
#define __USGSMOSIX_SLAVEPROJECTOR_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file SlaveProjector.h
 * 
 * \brief Header file for SlaveProjector class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"
#include "GeneralException.h"
#include "ProjImageParams.h"
#include "ProjectorInterface.h"
#include "ProjImageInInterface.h"
#include "ProjImageOutInterface.h"
#include "ProjImageDataOutInterface.h"
#include "ProjectorFactory.h"

namespace USGSMosix
{
    /// A wrapper for the GeneralProjector object which re-directs all of 
    /// its output to the network instead of directly back to a file. 
    class SlaveProjector : public ProjectorInterface
    {
    public:
       
        /// \param inInterface The ProjImage interface from which the input 
        /// data will come.
        /// \param imgOutInterface The ProjImage interface from which the
        /// ouput data will come.
        /// \param scanlineRange The inclusive range of scanlines (zero-based)
        /// for which this Projector is responsible.
        SlaveProjector( ProjImageInInterface & inInterface,
                        ProjImageOutInterface & imgOutInterface,
                        std::pair<unsigned long, unsigned long> scanlineRange); 

        /// \param interface The Projector that the SlaveProjector will be
        /// composed of and use to carry out a projection.
        /// \param scanlineRange The inclusive range of scanlines (zero-based)
        /// for which this Projector is responsible.
        SlaveProjector( ProjectorInterface & interface, 
                        std::pair<unsigned long, unsigned long> scanlineRange );
        
        virtual ~SlaveProjector();
        
        /// \brief Returns the output projection that the projector is using.
        virtual const ProjLib::Projection * getOutputProjection()const;

        /// \brief Constructs an object for image output if it doesn't exist 
        /// in the child Projector to this object.  Sets up various output 
        /// parameters.
        virtual bool setupOutput();

        /// \brief Reprojects the scanlines in the inclusive range [ beginLine,
        /// endLine] from the source projection to the destination projection.
        virtual ProjImageOutPiece projectPiece( long unsigned int beginLine,
                                                long unsigned int endLine );
        
        virtual ProjImageOutPiece projectPiece();    
        
        /// \brief Reprojects all scanlines in the range given to the 
        /// projector.
        virtual void project();   

        /// \brief Returns a const pointer to the ProjImageInInterface 
        /// reference.
        virtual const ProjImageInInterface * getProjImageIn()const;

        /// \brief Returns a const pointer to the ProjImageOutInterface 
        /// reference.
        virtual const ProjImageOutInterface * getProjImageOut()const;

        /// \param client The socket 
        /// \brief Serializes all member data to a client socket so that
        /// a duplicate object may be created elsewhere.
        virtual void exportToSocket( ClientSocket & client )const;

        
        static SlaveProjector createFromSocket( ClientSocket & client );
        
    private:

        /// The factory necessary for creating Projectors from the
        /// createFromSocket() function.
        static ProjectorFactory m_projFactory;

        /// The id number for this projector. 
        static long int m_id;  

        /// The child projector which is used to carry out the projection.
        ProjectorInterface * m_projInterface;
        
        /// The inclusive range of scanlines for which this projector is 
        /// responsible.
        std::pair<long int, long int> m_scanlineRange;

    };

/******************************************************************************/

inline
const ProjImageInInterface * SlaveProjector::getProjImageIn()const
{
    return m_projInterface->getProjImageIn();
}

/******************************************************************************/

inline
const ProjImageOutInterface * SlaveProjector::getProjImageOut()const
{
    return m_projInterface->getProjImageOut();
}

/******************************************************************************/

} // namespace 

/******************************************************************************/

#endif // __USGSMOSIX_SLAVEPROJECTOR_H_
