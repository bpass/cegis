#ifndef USGSMOSIX_TIFFGENERATOR_H_
#define USGSMOSIX_TIFFGENERATOR_H_

/***********************************************************************************************
 * Programmer        : Mark Schisler 
 *                     United States Geological Survey
 *                     Mid-Continent Mapping Center
 *
 * Date of Creation  : Wed Mar  2 10:27:38 CST 2005
 * File              : OutputGenerator.h
 *
 * Purpose           : To serve as a specific type of OutputGenerator.  In this
 * case, we are concerned with outputing TIFF data from new projections.
 * 
 **********************************************************************************************/

namespace USGSMosix {
    
    template <typename Tdat>
    class TIFFGenerator<Tdat> : public OutputGenerator<Tdat>
    {
      public:
        OutputGenerator(ScanLineSet<Tdat>& scanLine) : m_scanLineSet(scanLine) {}
        virtual ~OutputGenerator() {}
        virtual void getChunk (const DataSet<Tdat>& dataSet);
      private:
        ScanLineSet<Tdat>& m_scanLineSet;
    };

} // namespace USGSMosix

#endif //  USGSMOSIX_TIFFGENERATOR_H_

