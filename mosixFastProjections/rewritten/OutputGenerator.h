#ifndef USGSMOSIX_OUTPUTGENERATOR_H_
#define USGSMOSIX_OUTPUTGENERATOR_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler (c) 2005 
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *                     
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : OutputGenerator.h
 *
 * Purpose           : To serve as an interface base class for future jobs 
 * 
 **********************************************************************************************/

namespace USGSMosix {
   
    template <typename Tdat>
    class OutputGenerator  
    {
      public:
        OutputGenerator() {}
        virtual ~OutputGenerator() {}
        virtual void getChunk (const DataSet<Tdat>& dataSet) = 0;
    };

} // namespace USGSMosix

#endif //  USGSMOSIX_GENERICJOB_H_


