#ifndef USGSMOSIX_SCANLINESET_H_
#define USGSMOSIX_SCANLINESET_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler 
 *                     United States Geological Survey
 *
 *                     Mid-Continent Mapping Center
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : ScanlineSet.h
 * 
 * Purpose           : To serve as an interface base class for future datasets 
 * 
 **********************************************************************************************/

#include <deque>

namespace USGSMosix {

    template <typename Tdat>
    class ScanLineSet : public DataSet<T> 
    {
      public:
        DataSet() {}
        virtual ~DataSet() {}
        virtual Tdat getTop() = 0;
        virtual Tdat popTop() = 0;
        virtual Tdat getNext() = 0;
        virtual bool isEnd() = 0;
        virtual void appendTail(const Tdat& newElement) = 0;
      private:
        std::deque<Tdat> m_setScanlines;
    };

} // namespace USGSMosix

#endif //  USGSMOSIX_SCANLINESET_H_

