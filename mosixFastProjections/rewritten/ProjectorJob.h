#ifndef GENERICJOB_H_
#define GENERICJOB_H_

/******************************************************************************
 *  Programmer       : Mark Schisler (c) 2005
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *                     
 *  Date of Creation : Thu Mar  3 09:16:36 CST 2005
 *  File             : ProjectorJob.h
 *  
 *  Purpose          : To define a specific type of job.  In this case we are 
 *  mainly concerned with MOSIX's ability to do projections over the cluster.  
 *  To accomplish this we use the USGS "Projector".
 *  
 ******************************************************************************/

#include "Projector.h"
#include "WorkUnit.h"
#include "Stitcher.h"
#include "StitcherNode.h"

namespace USGSMosix { 

    template<typename InputType, typename OutputType>
    class ProjectorJob<InputType, OutputType> : public GenericJob<InputType, OutputType> 
    {
      public:
        ProjectorJob(const OutputGenerator<InputType>& oG, const WorkAcceptor<OutputType>& wA);
        virtual ~GenericJob();   
        virtual OutputGenerator doJob();
      private:
        OutputGenerator<InputType>& m_outputGenerator;
        WorkAcceptor<OutputType>&  m_workAcceptor; 
        
    };

} // namespace USGSMosix 

#endif //  GENERICJOB_H_












