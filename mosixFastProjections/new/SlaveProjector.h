#ifndef __USGSMOSIX_SLAVEPROJECTOR_H_
#define __USGSMOSIX_SLAVEPROJECTOR_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 20:56:34 $
 *
 * \version 0.1
 * 
 * \file SlaveProjector.h
 * 
 * \brief Built as a wrapper for an FromMultiGeoProjector object,
 * this class will redirect all output from it usual file ouput, 
 * to being network controlled --i.e., all output will be
 * directed toward a socket via a forwarded descriptor.
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
    class SlaveProjector : public ProjectorInterface
    {
    public:
        
        SlaveProjector( ProjImageInInterface & inInterface,
                        ProjImageOutInterface & imgOutInterface,
                        std::pair<unsigned long, unsigned long> scanlineRange ); 
        SlaveProjector( ProjectorInterface & interface, 
                        std::pair<unsigned long, unsigned long> scanlineRange );
        
        virtual ~SlaveProjector();
        
        virtual const ProjLib::Projection * getOutputProjection()const;
        virtual bool setupOutput();
        virtual scanlines_t project( long unsigned int beginLine, 
                                     long unsigned int endLine );
        
        void setScanlines(scanlines_t sl); 
        
        virtual void project();   

        virtual const ProjImageInInterface * getProjImageIn()const;
        virtual const ProjImageOutInterface * getProjImageOut()const;

        virtual void exportToSocket( ClientSocket & client )const;
        static SlaveProjector createFromSocket( ClientSocket & client );
        
    private:
        void cleanupScanlines();

        static ProjectorFactory m_projFactory;
        static long int m_id;  
        ProjectorInterface * m_projInterface;
        
        std::pair<long int, long int> m_scanlineRange;
        unsigned int m_noScanlines;
        scanlines_t m_scanlines;
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

inline 
void SlaveProjector::setScanlines(scanlines_t sl)
{ 
    m_scanlines = sl; 
}

/******************************************************************************/

} // namespace 

/******************************************************************************/

#endif // __USGSMOSIX_SLAVEPROJECTOR_H_
