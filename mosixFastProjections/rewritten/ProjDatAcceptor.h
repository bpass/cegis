#ifndef USGSMOSIX_PROJDATACCEPTOR_H_
#define USGSMOSIX_PROJDATACCEPTOR_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler 
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : ProjDatAcceptor.h
 *
 * Purpose           : To serve as an interface base class for future jobs 
 * 
 **********************************************************************************************/

namespace USGSMosix {

    template <typename Tdat>
    class ProjDatAcceptor : public InputAcceptor<Tdat> {
      public:
        InputAcceptor(DataSet<T>&  );
        virtual ~InputAcceptor() {}
        virtual void getData(DataSet<Tdat>& dataSet);
        unsigned int getNumWorkUnits();
      private:
        void setNumWorkUnits(workUnitID_t newnumworkunits);
        void insertScanLine(void * scanline, long row);
        
    };

} // namespace USGSMosix

#include "ProjDatAcceptor.hpp" 

#endif // USGSMOSIX_PROJDATACCEPTOR_H_

