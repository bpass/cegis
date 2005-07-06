
#ifndef _USGSMOSIX_PROJECTORINTERFACE_H_
#define _USGSMOSIX_PROJECTORINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjectorInterface.h 
 * 
 * \brief This Interface is designed to lessen the tight-coupledness
 * of the frequent inheritence from the Projector class encountered
 * in the MOSIX Fast Projections project.  
 *
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"
#include<ProjectorException.h>

namespace USGSMosix 
{

class ProjectorInterface 
{
    public:

        ProjectorInterface() {}
        virtual ~ProjectorInterface() {} 
        virtual const ProjLib::Projection * getOutputProjection()const = 0;
        virtual void getNewExtents() = 0;
        virtual scanlines_t project( long unsigned int beginLine, 
                                     long unsigned int endLine ) = 0;
        virtual void project() = 0;   
};

} // namespace

#endif // _USGSMOSIX_PROJECTORINTERFACE_H_
