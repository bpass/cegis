#ifndef USGSMOSIX_UCSCANLINE_H_
#define USGSMOSIX_UCSCANLINE_H_

/******************************************************************************
 * Programmer        : Mark Schisler 
 *                     United States Geological Survey
 *
 *                     Mid-Continent Mapping Center
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : ScanlineSet.h
 * 
 * Purpose           : To serve as an interface base class for future datasets 
 * 
 ******************************************************************************/

#include <iostream>
#include <deque>

namespace USGSMosix {

    template <>
    class ScanLineSet<unsigned char> : public DataSet<unsigned char> 
    {
      public:
        DataSet() {}
        virtual ~DataSet() {}
        virtual Tdat getTop();
        virtual Tdat popTop();
        virtual Tdat getNext();
        virtual bool isEnd();
        virtual void appendTail(const unsigned char& newElement);
        
        friend std::ostream & os 
        operator<< ( std::ostream & os, const ScanLineSet<unsigned char>& sls);
        
        friend std::ostream & os
        operator>> ( const std::ostream & os, ScanLineSet<unsigned char>& sls);
        
      private:
        std::deque<unsigned char> m_setScanlines;
    };

} // namespace USGSMosix

#endif //  USGSMOSIX_UCSCANLINESET_H_








