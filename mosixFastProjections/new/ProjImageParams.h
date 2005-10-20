#ifndef __USGSMOSIX_PROJIMAGEPARAMS_H_
#define __USGSMOSIX_PROJIMAGEPARAMS_H_

/*!
 *
 * \author Mark Schisler
 *
 * \file ProjImageParams.h 
 *
 * \date $Date: 2005/10/20 23:21:15 $
 *
 * \version 0.1
 * 
 * \brief Header file for ProjImageParams class.
 *
 */

#include <iostream>
#include <utility>
#include <ProjectionIO/ProjectionReader.h>
#include <ProjectionIO/ProjectionWriter.h>
#include <DRect.h>
#include "SerializableInterface.h"
#include "GeneralException.h"
#include "ClientSocket.h"

namespace USGSMosix {

using std::istream;
using std::ostream;

/// The ProjImageParams object contains encapsulates the 
/// process of gathering boundary data, image data, and creating
/// a projection from a parameter file.
class ProjImageParams : public SerializableInterface {
    
    public:
        /// \brief A enumerated type indicating what type of parameters
        /// are inside this ProjImageParams object, whether they are input
        /// or output parameters.
        enum FileType { INPUT = 0, OUTPUT };
        
        ProjImageParams() {} 
        ProjImageParams( std::string paramFilename, FileType ft );
        ProjImageParams( const ProjImageParams & params );
        virtual ~ProjImageParams();
        
        /// \param socket The socket which the data for creating the 
        /// ProjImageParam object is waiting on.
        ///
        /// \pre  The data waiting in the forwarded socket is that which 
        /// was exported by a function call to exportFromSocket().
        ///
        /// \post The aforementioned data waiting in the socket will be 
        /// used to create a ProjImageParam object which will be 
        /// returned from the function.
        static ProjImageParams createFromSocket( ClientSocket & socket );
       
        /// \param socket The destination socket.
        ///
        /// \brief This function exports the pertinent data members of
        /// this ProjImageParam object which are needed to recreate 
        /// the same object on the other end of the forwarded socket.
        virtual void exportToSocket( ClientSocket & socket )const;

        /// \param in The stream which has the data for a parameter 
        /// file, as prescribed by program documentation, waiting on it.
        ///
        /// \param params The object which will have the data from the 
        /// aforementioned parameter file inputted into it.
        ///
        /// \brief Inputs data from a parameter file waiting on a istream,
        /// in, into params.
        friend istream& operator>>( istream& in, ProjImageParams& params );
        
        /// \param out The stream which the data for params should
        /// be outputted to in text form.
        ///
        /// \param params The object which will have its data outputted
        /// to the ostream, in.
        ///
        /// \brief Outputs the data in params in a pretty text style to 
        /// the ostream out.
        friend ostream& operator<<( ostream& out,const ProjImageParams& params);
      
        /// \brief returns the filename of the parameter file which
        /// created this object.
        std::string getParamFilename()const     { return m_paramFilename; }
        
        /// \brief returns the filename of the image file.
        std::string getFilename()const          { return m_imageFilename; }

        /// \brief returns the bounds in degrees of the image file associated
        /// with these parameters.
        DRect getBounds()const                  { return m_bounds;  }

        /// \brief returns the projection which was created from the 
        /// parameter file that initialized this object. 
        const Projection * getProjection()const { return m_projection; } 
        
        /// \brief Returns a std::pair representing the height and 
        /// width of the image associated with this parameter file 
        /// in pixels. 
        std::pair<long int, long int> getHeightWidthPair(); 
       
    private:
        
        /// \param The input stream.
        /// \brief Returns a constructed projection alloc'ed by new 
        /// which was created from projection parameters waiting on 
        /// the input stream in.
        Projection * constructProjection(istream& in);
       
        /// The name of the parameter file.
        std::string m_paramFilename;
        
        /// Input or output file.
        FileType m_fileType;  

        /// Name of the image which we are gathering info about.
        std::string m_imageFilename;

        /// The bounds of the aforementioned image in degrees.
        DRect m_bounds;

        /// Where the projection will be created. 
        Projection * m_projection; 

        /// The width and height of the image in pixels.
        long int m_outHeight, m_outWidth;
};

} // namespace

#endif // __USGSMOSIX_PROJIMAGEPARAMS_H_
