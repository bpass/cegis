#ifndef GENERICJOB_H_
#define GENERICJOB_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler (c) 2005 
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : GenericJob.h
 *
 * Purpose           : To serve as an interface base class for future jobs 
 * 
 **********************************************************************************************/

namespace USGSMosix { 

    class <typename InputType, typename OutputType>
    class GenericJob 
    {
      public:
        GenericJob(const OutputGenerator<OutputType>& oG, 
                   const InputAcceptor<InputType>& wA);
        
        virtual ~GenericJob() {} 
        virtual OutputGenerator doJob() = 0;
        virtual OutputGenerator doJob(const InputAcceptor<InputType>& inputAccept) = 0;
    };

} // namespace USGSMosix 
    
#endif //  GENERICJOB_H_

