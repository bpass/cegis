#ifndef USGSMOSIX_DATASET_H_
#define USGSMOSIX_DATASET_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler (c) 2005  
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *             
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : DataSet.h
 * Purpose           : To serve as an interface base class for future datasets 
 * 
 **********************************************************************************************/

namespace USGSMosix {

    template <typename Tdat>
    class DataSet 
    {
      public:
        DataSet() {}
        virtual ~DataSet() {}
        virtual Tdat getTop() = 0;
        virtual Tdat popTop() = 0;
        virtual Tdat getNext() = 0;
        virtual bool isEnd() = 0;
        virtual void appendTail(const Tdat& newElement) = 0;
    };

} // namespace USGSMosix

#endif //  USGSMOSIX_GENERICJOB_H_


