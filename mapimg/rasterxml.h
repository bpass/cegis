// $Id: rasterxml.h,v 1.2 2005/01/20 17:28:50 rbuehler Exp $


/*! \mainpage RasterXML class for mapimg
*
*	\section desc_sec Description
*	The RasterXML class is used to store the raster file information 
*	generated by mapimg. It can load the information
*	from XML files, as well as write information out to XML files.
*	Note: all member functions of RasterXML throw XMLException upon error.

*/
#ifndef RASTERXML_H
#define RASTERXML_H
#include <iostream>
#include <tinyxml.h>

//!Image Class.
/*!
  This class stores all of the image information used by mapimg.
  Currently, it is designed to load this information from XML files
  that describe the specific features of the image. It can aslo be 
  used to generate new XML based image information files.
*/
class RasterXML {
	//!Overloaded output operator.
	/*!
		Outputs a formatted(non-XML) description of the 
		image attributes to stream "os".
	*/
	friend std::ostream& operator<<(std::ostream& os, const RasterXML& i);
public:

	//!Default Constructor.
	RasterXML();

	//!Constructor used to load an xml file.
	/*!
		When this constructor is used the file referred to by filename
		will be parsed and all image values will be loaded into the appropriate
		class member variables.
	*/
	RasterXML(const char* filename);

	//!Copy Constructor.
	RasterXML(const RasterXML& c);

	//!Destructor.
	~RasterXML();

	//!Get Bit depth of image.
	int getBits() const;

	//!Get GCTP number for current projection.
	int getProjNumber() const;

	//!Get GCTP units number for current units.
	int getUnitsNumber() const;

	//!Get UTM Zone.
	int getZone() const;

	//!Get GCTP Datum number for current datum.
	int getDatumNumber() const;

	//!Get number of rows in the image.
	int getRows() const;

	//!Get number of columns in the image.
	int getCols() const;
	
	//!Get pixel size of the image.
	double getPixelSize() const;

	//!Get upper-left X coordinate of the image.
	double getUlx() const;

	//!Get upper-left Y coordinate of the image.
	double getUly() const;

	//!Get GCTP parameter indexed by "index".
	double getGCTPParam(int index) const;

	//!Get the pixel fill value for the image.
	double getFillValue() const;

	//!Get Name of current projection.
	/*! 
		If projection name is not set this returns NULL.
	*/
	char* getProjName() const;
	
	//!Get name of current datum.
	/*!
		If datum name is not set, this returns NULL.
	*/
	char* getDatumName() const;
	
	//!Get the name of the current units.
	/*!
		If units name is not set, this returns NULL.
	*/
	char* getUnitsName() const;
	
	//!Get the data type of the image.
	/*!
		If the data type has not been set, this returns NULL.
	*/
	char* getDataType() const;
	
	//!Get the filename of the xml file associated with this image.
	/*!
		If filename has not been set, this returns NULL.
	*/
	char* getFilename() const;
	
	//!Get the name of the image author.
	char* getAuthorName() const;

	//!Get the company of the image author.
	char* getAuthorCompany() const;

	//!Get the email address of the image author.
	char* getAuthorEmail() const;

	//!Check if the image is signed or not.
	/*!
		Returns 1 if signed, 0 otherwise.
	*/
	int isSigned() const;

	//!Set the number of rows for this image.
	void setRows(int rows);

	//!Set the number of columns for this image.
	void setCols(int cols);

	//!Set the pixel size of this image.
	void setPixelSize(double pixelSize);

	//!Set the bit-depth of this image.
	void setBits(int bits);

	//!Set the GCTP Projection number for this image.
	void setProjNumber(int num);

	//!Set the GCTP units number for this image.
	void setUnitsNumber(int num);
	
	//!Set the name of the units for this image.
	void setUnitsName(const char* name);
	
	//!Set the UTM zone for this image.
	void setZone(int zone);

	//!Set the GCTP Datum number for this image.
	void setDatumNumber(int num);

	//!Set the coordinates for the upper-left corner of the image.
	void setUlCorner(double ulx, double uly);

	//!Set GCTP Parameter "index" for this image.
	void setParam(int index, double value, const char* comment = NULL);

	//!Set the data type for this image.
	void setDataType(const char* type);

	//!Set the projection name for this image.
	void setProjName(const char* name);

	//!Set the datum name for this image.
	void setDatumName(const char* name);

	//!Set the sign of the data in this image.
	/*!
		1 for signed
		0 for unsigned
	*/
	void setSigned(bool isSigned);

	//!Load an XML configuration file for this image.
	/*!
		Returns true if load was sucessful, false otherwise.
	*/
	void loadFile(const char* filename);
	
	//!Save the image data out to an XML file.
	/*!
		In order to use this function you must have passed
		a filename to the constructor when you instantiated 
		the object.
	*/
	bool save();
	
	//!Save image data out to an XML file named "filename"
	/*!
		Use this function if you did not pass a filename when 
		you instantiated the image object, or if you wish to 
		save the current image configuration out to another file.
	*/
	bool save(const char* filename);
	
	//!Set the pixel fill value for the image.
	void setFillValue(double val);

	//!Set the name of the image author.
	/*!
		If NULL is passed, the value "Unknown"
		will be used.
	*/
	void setAuthorName(const char* name);
	
	//!Set the company of the image author.
	/*!
		If NULL is passed, the value "Unknown"
		will be used.
	*/
	void setAuthorCompany(const char* company);

	//!Set the email of the image author.
	/*!
		If NULL is passed, the value "Unknown"
		will be used.
	*/
	void setAuthorEmail(const char* email);
	 

private:
		int m_bits;
		int m_projNumber;
		int m_unitsNumber;
		int m_zone;
		int m_datumNumber;
		int m_rows;
		int m_cols;
		double m_pixelSize;
		double m_ulx;
		double m_uly;
		double m_fillValue;
      //double m_noDataValue; .setNoDataValue(double) .getNoDataValue()
		double* m_GCTPParams;
		char* m_projName;
		char* m_datumName;
		char* m_unitsName;
		char* m_dataType;
		char* m_authorName;
		char* m_authorCompany;
		char* m_authorEmail;
		char* m_filename;
		char* m_tempFilename;
		int m_signed;

		TiXmlDocument* m_doc;

		void setFilename(const char* filename);
		void initInternalDocument(const char* filename=NULL);
};

//!Image exception class.
/*!
	This class is used to generate exceptions
	when parsing the XML information files.
*/
class XMLException {
public:
	
	//!Constructor.
	/*!
		"message" is the error that has been generated.
		There is a 99 character limit on the error messages.
		Anything longer will be truncated.
	*/
	XMLException(const char* message) {
		if(message) {
			if(strlen(message) > 99) {
				for(int i = 0; i < 99; i++)
					m_message[i] = message[i];
				m_message[i] = '\0';
			}
			else {
				for(unsigned int i = 0; i < strlen(message); i++)
					m_message[i] = message[i];
				m_message[i] = '\0';
			}
		}
			
		else
			for(int i = 0; i < 100; i++)
				m_message[i] = '\0';
		
	}

	//!Overloaded output operator.
	/*!
		Outputs error message to stream "os"
	*/
	friend std::ostream& operator<<(std::ostream& os, XMLException e);
private:
	 char m_message[100];
};


#endif

