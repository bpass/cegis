#ifndef USGSMOSIX_INPUTACCEPTOR_H_
#define USGSMOSIX_INPUTACCEPTOR_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler 
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : InputAcceptor.h
 *
 * Purpose           : To serve as an interface base class for future jobs 
 * 
 **********************************************************************************************/

namespace USGSMosix {

    template <typename Tdat>
    class InputAcceptor {
      public:
        InputAcceptor();
        virtual ~InputAcceptor() {}
        virtual void getData(DataSet<Tdat>& dataSet) = 0;
    };

} // namespace USGSMosix

#endif // USGSMOSIX_INPUTACCEPTOR_H_


