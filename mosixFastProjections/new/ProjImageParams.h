#ifndef _USGSMOSIX_INPUTREADER_H_
#define _USGSMOSIX_INPUTREADER_H_

/*!
 *
 * \author Mark Schisler
 *
 * \file InputReader.h
 *
 * \date $Date: 2005/07/06 23:16:45 $
 *
 * \version 0.1
 * 
 * \brief ProjImageParams is to take care of the business of accepting
 * and dealing with input parameters for the MOSIX fast projections 
 * project.
 *
 */

#include <iostream>
#include <ProjectionIO/ProjectionReader.h>
#include <ProjectionIO/ProjectionWriter.h>
#include <DRect.h>

namespace USGSMosix 
{
using std::istream;
using std::ostream;

class ProjImageParams  
{
    public:
        enum FileType { INPUT = 0, OUTPUT };
        
        ProjImageParams() {} 
        ProjImageParams( std::string paramFilename, FileType ft ); 
        virtual ~ProjImageParams();

        friend istream& operator>>( istream& in, ProjImageParams& params );
        friend ostream& operator<<( ostream& out,const ProjImageParams& params);
        
        std::string getFilename()const { return m_imageFilename; }
        DRect getBounds()const  { return m_bounds;  }
        const Projection * getProjection()const { return m_projection; } 
       
    private:
        Projection * constructProjection(istream& in);
        FileType m_fileType;  
        std::string m_imageFilename;
        DRect m_bounds;
        Projection * m_projection; 
};

} // namespace


#endif // _USGSMOSIX_INPUTREADER_H_
