// $Id: rasterxml.cpp,v 1.2 2005/01/28 17:59:07 jtrent Exp $


/*  To Do:
		2.) implement get/set author functions
		3.) rework constuctors to account for new design
		4.) rework loadFile() to account for new design
		6.) test test test
		7.)finish reworking set functions
*/



/******************************************************
*File: RasterXML.cpp
*Description: Implementation file for Image class
*Programmer: Michael Williams, USGS 
*Date: 09/24/2004
*******************************************************/

#include "rasterxml.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
using namespace std;


//Default Constructor
RasterXML::RasterXML(): 
m_bits(0), 
m_projNumber(0),
m_unitsNumber(0),
m_zone(0),
m_datumNumber(0),
m_rows(0),
m_cols(0),
m_pixelSize(0.0),
m_ulx(0.0),
m_uly(0.0),
m_fillValue(0.0),
m_projName(NULL),
m_datumName(NULL),
m_unitsName(NULL),
m_dataType(NULL),
m_authorName(NULL),
m_authorCompany(NULL),
m_authorEmail(NULL),
m_filename(NULL),
m_tempFilename(NULL),
m_signed(false),
m_doc(NULL)
{
	m_GCTPParams = new double[15];
	for(int i = 0; i < 15; i++)
		m_GCTPParams[i] = 0.0;
	
	//initialize internal document tree
	initInternalDocument();
	
	
}

//Constructor
//Function: the XML file referred to by filename
//is loaded.
RasterXML::RasterXML(const char* filename): 
m_bits(0), 
m_projNumber(0),
m_unitsNumber(0),
m_zone(0),
m_datumNumber(0),
m_ulx(0.0),
m_uly(0.0),
m_fillValue(0.0),
m_projName(NULL),
m_datumName(NULL),
m_unitsName(NULL),
m_dataType(NULL),
m_authorName(NULL),
m_authorCompany(NULL),
m_authorEmail(NULL),
m_doc(NULL),
m_filename(NULL),
m_signed(false),
m_rows(0),
m_cols(0),
m_pixelSize(0.0)
{
	if(!filename) {
		throw(XMLException("NULL filename"));
		exit(1);
	}

	m_GCTPParams = new double[15];
	for(int i = 0; i < 15; i++)
		m_GCTPParams[i] = 0.0;

	loadFile(filename);

	
}

//Copy Constructor
RasterXML::RasterXML(const RasterXML& c):
m_bits(0), 
m_projNumber(0),
m_unitsNumber(0),
m_zone(0),
m_datumNumber(0),
m_ulx(0.0),
m_uly(0.0),
m_fillValue(0.0),
m_projName(NULL),
m_datumName(NULL),
m_unitsName(NULL),
m_dataType(NULL),
m_filename(NULL),
m_authorName(NULL),
m_authorCompany(NULL),
m_authorEmail(NULL),
m_signed(false),
m_rows(0),
m_cols(0),
m_pixelSize(0.0),
m_doc(NULL)
{
	//initialize internal document tree
	//so that as we do sets, the tree gets 
	//updated and eventually is an exact
	//copy of the other tree.
	initInternalDocument(c.m_filename);

	TiXmlDocument* tempDoc = c.m_doc;
	TiXmlNode* paramComment = tempDoc->FirstChild("rasterInformation")->FirstChild("GCTPParams")->FirstChild();
	m_GCTPParams = new double[15];
	for(unsigned int i = 0; i < 15; i++) {
		setParam(i, c.m_GCTPParams[i], paramComment->Value());
		paramComment = paramComment->NextSibling();
		paramComment = paramComment->NextSibling();
	}

	
	setAuthorName(c.m_authorName);
	setAuthorCompany(c.m_authorCompany);
	setAuthorEmail(c.m_authorEmail);
	setProjName(c.m_projName);
	setDatumName(c.m_datumName);
	setUnitsName(c.m_unitsName);
	setDataType(c.m_dataType);
	setFilename(c.m_filename);
	setRows(c.m_rows);
	setCols(c.m_cols);
	setPixelSize(c.m_pixelSize);
	setFillValue(c.m_fillValue);
	setBits(c.m_bits);

	if(c.m_signed)
		setSigned(true);
	else if(!c.m_signed)
		setSigned(false);

	setDatumNumber(c.m_datumNumber);
	setProjNumber(c.m_projNumber);
	setUnitsNumber(c.m_unitsNumber);
	setZone(c.m_zone);
	setUlCorner(c.m_ulx,c.m_uly);
	setFillValue(m_fillValue);
	
}

RasterXML::~RasterXML() {
	if(m_GCTPParams)
		delete[] m_GCTPParams;
	if(m_projName)
		delete[] m_projName;
	if(m_datumName)
		delete[] m_datumName;
	if(m_unitsName)
		delete[] m_unitsName;
	if(m_dataType)
		delete[] m_dataType;
	if(m_filename)
		delete[] m_filename;
	if(m_authorName)
		delete[] m_authorName;
	if(m_authorCompany)
		delete[] m_authorCompany;
	if(m_authorEmail)
		delete[] m_authorEmail;
	if(m_doc)
		delete m_doc;
}

//Initialize internal document tree
void RasterXML::initInternalDocument(const char* filename) {
	if(m_doc)
		delete m_doc;

	//if we were given a filename, use it, otherwise
	//don't
	if(filename)
		m_doc = new TiXmlDocument(filename);
	else
		m_doc = new TiXmlDocument();
	
	//******************header comment string*********************
	const char* headerCommentString = "This is the information for a generic binary formatted\n"
	"raster image generated by mapimg. Image storage format\n"
	"should be band interleave by line (BIL). Generic binary\n"
	"information should be in native byte order of current\n"
	"operation system\n"
	"Send all questions and comments to mapimg@usgs.gov.\n"
	"Created by U.S. Geological Survey 2005\0";

	//Create each element for the internal document tree
	TiXmlText unknownV("Unknown");
	TiXmlDeclaration decl("1.0", NULL, "yes");
	TiXmlElement rootE("rasterInformation");

	//************Declare Comment Object***************
	TiXmlComment headerComment;

	TiXmlElement authorE("author");
	TiXmlElement authorNameE("name");
	TiXmlElement authorCompanyE("company");
	TiXmlElement authorEmailE("email");
	TiXmlElement areaE("area");
	TiXmlElement ulCornerE("ulCorner");
	TiXmlElement dimensionsE("dimensions");
	TiXmlElement pixelDescE("pixelDescription");
	TiXmlElement pixelBitsE("bits");
	TiXmlElement pixelSignE("sign");
	TiXmlElement pixelSizeE("pixelSize");
	TiXmlElement pixelTypeE("type");
	TiXmlElement pixelFillValueE("fillValue");
	TiXmlElement projE("projection");
	TiXmlElement projNameE("name");
	TiXmlElement projZoneE("zone");
	TiXmlElement projDatumE("datum");
	TiXmlElement projUnitsE("units");
	TiXmlElement paramsRootE("GCTPParams");
	TiXmlElement paramE("param");



	
	//********************init header comment******************
	headerComment.SetValue(headerCommentString);

	//create attributes for <projection>, <units>, <datum>, <dimensions>, and <ulcorner>
	projE.SetAttribute("number", "-1");
	projUnitsE.SetAttribute("number", "-1");
	projDatumE.SetAttribute("number", "-1");
	
	//<author>
	authorNameE.InsertEndChild(unknownV);
	authorCompanyE.InsertEndChild(unknownV);
	authorEmailE.InsertEndChild(unknownV);
	authorE.InsertEndChild(authorNameE);
	authorE.InsertEndChild(authorCompanyE);
	authorE.InsertEndChild(authorEmailE);

	//<area>
	ulCornerE.SetAttribute("ulx", "0.0");
	ulCornerE.SetAttribute("uly", "0.0");
	dimensionsE.SetAttribute("rows", "-1");
	dimensionsE.SetAttribute("columns", "-1");
	areaE.InsertEndChild(ulCornerE);
	areaE.InsertEndChild(dimensionsE);

	//<pixelDescription> 
	pixelBitsE.InsertEndChild(TiXmlText("0"));
	pixelSignE.InsertEndChild(unknownV);
	pixelTypeE.InsertEndChild(unknownV);
	pixelSizeE.InsertEndChild(unknownV);
	pixelFillValueE.InsertEndChild(TiXmlText("0.0"));
	pixelDescE.InsertEndChild(pixelSignE);
	pixelDescE.InsertEndChild(pixelBitsE);
	pixelDescE.InsertEndChild(pixelTypeE);
	pixelDescE.InsertEndChild(pixelSizeE);
	pixelDescE.InsertEndChild(pixelFillValueE);


    //<projection>
	projNameE.InsertEndChild(unknownV);
	projUnitsE.InsertEndChild(unknownV);
	projZoneE.InsertEndChild(TiXmlText("62"));
	projDatumE.InsertEndChild(unknownV);
	projE.InsertEndChild(projNameE);
	projE.InsertEndChild(projZoneE);
	projE.InsertEndChild(projDatumE);
	projE.InsertEndChild(projUnitsE);

	paramE.InsertEndChild(TiXmlText("0.0"));

	//<GCTPParams>
	for(int i = 0; i < 15; i++) {
		TiXmlComment paramComment;
		paramComment.SetValue("Unused");
		paramsRootE.InsertEndChild(paramComment);
		paramsRootE.InsertEndChild(paramE);
	}
	
	rootE.InsertEndChild(headerComment);
	rootE.InsertEndChild(authorE);
	rootE.InsertEndChild(areaE);
	rootE.InsertEndChild(pixelDescE);
	rootE.InsertEndChild(projE);
	rootE.InsertEndChild(paramsRootE);


	//add declaration i.e. <?xml verison=....
	m_doc->InsertEndChild(decl);

	//insert root node.
	m_doc->InsertEndChild(rootE);
}

//Load an XML image file 
void RasterXML::loadFile(const char* filename) {
	int bits = 0;
	int datumNumber = 0;
	double pixelSize = 0;
	double fillValue = 0;
	int rows = 0;
	int cols = 0;
	int sign = -2;
	int unitsNumber = 0;
	int projNumber = 0;
	int zone = -1;
	double ulx = 0;
	double uly = 0;
	double GCTPParam = 0;
	
	const char* unitsName = NULL;
	const char* datumName = NULL;
	const char* projName = NULL;
	const char* typeName = NULL;
	const char* authorName = NULL;
	const char* authorCompany = NULL;
	const char* authorEmail = NULL;

	//Create the document object and load the file.
	TiXmlDocument doc;
	if(!doc.LoadFile(filename)) 
		throw(XMLException("Error loading file"));
	

	//Initialize the internal document tree with the current filename.
	initInternalDocument(filename);
	
	TiXmlHandle handle(&doc);

	//Element pointerts for the top level tags.
	TiXmlElement* rootE = handle.FirstChild("rasterInformation").Element();
	TiXmlElement* authorE =  handle.FirstChild("rasterInformation").FirstChild("author").Element();
	TiXmlElement* pixelDescE =  handle.FirstChild("rasterInformation").FirstChild("pixelDescription").Element();
	TiXmlElement* projE = handle.FirstChild("rasterInformation").FirstChild("projection").Element();
	TiXmlElement* areaE = handle.FirstChild("rasterInformation").FirstChild("area").Element();
	TiXmlElement* paramsRootE = handle.FirstChild("rasterInformation").FirstChild("GCTPParams").Element();
	
	//element pointers for the lower level tags.
	TiXmlNode* currentParam = NULL;
	TiXmlNode* currentComment = NULL;
	TiXmlElement* bitsE= NULL;
	TiXmlElement* signedE = NULL;
	TiXmlElement* unitsE = NULL;
	TiXmlElement* typeE = NULL;
	TiXmlElement* projNameE = NULL;
	TiXmlElement* zoneE = NULL;
	TiXmlElement* datumE = NULL;
	TiXmlElement* authorNameE = NULL;
	TiXmlElement* authorCompanyE = NULL;
	TiXmlElement* authorEmailE = NULL;
	TiXmlElement* fillValueE = NULL;
	TiXmlElement* ulCornerE = NULL;
	TiXmlElement* dimensionsE = NULL;
	TiXmlElement* pixelSizeE = NULL;


	//********************************************************
	//*Check to see if any of the top-level tags are missing
	//********************************************************
	if(!rootE) 
		throw(XMLException("Error: missing or malformed <rasterInformation> tag"));
	

	if(!pixelDescE) 
		throw(XMLException("Error, missing or malformed <pixelDescription> tag."));
	

	if(!projE) 
		throw(XMLException("Error, missing or malformed <projection> tag."));
	

	if(!areaE) 
		throw(XMLException("Error, missing or malformed <area> tag."));
	

	if(!paramsRootE) 
		throw(XMLException("Error, missing or malformed <GCTPParams> tag."));
	
	
	//If none of the tags are missing, instantiate the lower
	//level tag element objects.
	bitsE= pixelDescE->FirstChild("bits")->ToElement();
	signedE = pixelDescE->FirstChild("sign")->ToElement();
	typeE = pixelDescE->FirstChild("type")->ToElement();
	fillValueE = pixelDescE->FirstChild("fillValue")->ToElement();
	pixelSizeE = pixelDescE->FirstChild("pixelSize")->ToElement();
	datumE = projE->FirstChild("datum")->ToElement();
	projNameE = projE->FirstChild("name")->ToElement();
	zoneE = projE->FirstChild("zone")->ToElement();
	unitsE = projE->FirstChild("units")->ToElement();
	ulCornerE = areaE->FirstChild("ulCorner")->ToElement();
	dimensionsE = areaE->FirstChild("dimensions")->ToElement();
	authorNameE = authorE->FirstChild("name")->ToElement();
	authorCompanyE = authorE->FirstChild("company")->ToElement();
	authorEmailE = authorE->FirstChild("email")->ToElement();

	//*********************************************************************
	//*Check for the existence of lower-level tags
	//*********************************************************************
	if(!bitsE) 
		throw(XMLException("Error, missing or malformed <bits> tag under <pixelDescription>."));

	if(!signedE) 
		throw(XMLException("Error, missing or malformed <signed> tag under <pixelDescription>."));
	

	if(!unitsE) 
		throw(XMLException("Error, missing or malformed <units> tag under <projection>."));
	

	if(!typeE) 
		throw(XMLException("Error, missing or malformed <type> tag under <pixelDescription>."));
	

	if(!datumE) 
		throw(XMLException("Error, missing or malformed <datum> tag under <projection>."));
	
	if(!fillValueE)
		throw(XMLException("Error, missing or malformed <fillValue> tag under <pixelDescription>."));

	if(!pixelSizeE)
		throw(XMLException("Error, missing or malformed <pixelSize> tag under <pixelDescription>."));

	if(!projNameE) 
		throw(XMLException("Error, missing or malformed <name> tag under <projection>."));
	
	if(!ulCornerE) 
		throw(XMLException("Error, missing or malformed <ulCorner> tag under <area>."));

	if(!dimensionsE) 
		throw(XMLException("Error, missing or malformed <dimensions> tag under <area>."));

	if(!zoneE) 
		throw(XMLException("Error, missing or malformed <zone> tag under <projection>."));
	
	if(!authorNameE) 
		throw(XMLException("Error, missing or malformed <name> tag under <author>"));

	if(!authorCompanyE) 
		throw(XMLException("Error, missing or malformed <company> tag under <author>"));

	if(!authorEmailE) 
		throw(XMLException("Error, missing or malformed <email> tag under <author>"));


	if(authorNameE->FirstChild()) 
		authorName = authorNameE->FirstChild()->Value();
	
	else
		throw(XMLException("Error, missing value for <name> tag under <author> please put \"unknown\" if you do not know the value."));

	if(authorCompanyE->FirstChild()) 
		authorCompany = authorCompanyE->FirstChild()->Value();
	
	else
		throw(XMLException("Error, missing value for <company> tag under <author> please put \"unknown\" if you do not know the value."));

	if(authorEmailE->FirstChild()) 
		authorEmail = authorEmailE->FirstChild()->Value();
	
	else
		throw(XMLException("Error, missing value for <email> tag under <author> please put \"unknown\" if you do not know the value."));

	//Get rows attribute from <image>
	if(!dimensionsE->Attribute("rows", &rows)) 
		throw(XMLException("Error: missing rows attribute in <dimensions> please put \"-1\" if you do not have a value"));
	

	//Get columns attribute from <image>
	if(!dimensionsE->Attribute("columns", &cols)) 
		throw(XMLException("Error: missing columns attribute in <image> please put \"-1\" if you do not have a value"));
		
	//If there is a value inside <bits>
	if(bitsE->FirstChild()) 
		bits = atoi(bitsE->FirstChild()->Value());

	else 
		throw(XMLException("Error: bits value not present, please put \"0\" if you do not have a value"));
	
	if(fillValueE->FirstChild())
		fillValue = atof(fillValueE->FirstChild()->Value());
	
	else
		throw(XMLException("Error, missing value for <fillValue> under <pixelDescription>, please use -2 if it is unknown"));

	if(pixelSizeE->FirstChild())
		pixelSize = atof(pixelSizeE->FirstChild()->Value());
	
	else
		throw(XMLException("Error, missing value for <pixelSize> under <pixelDescription>, please use -1 if it is unknown"));

	//if there is a value inside <sign>
	if(signedE->FirstChild()) {
		char temp[30] = {'\0'};
		strcpy(temp, signedE->FirstChild()->Value());
		for(unsigned int i = 0; i < strlen(temp); i++)
			temp[i] = toupper(temp[i]);

		if(!strcmp(temp, "UNKNOWN"))
			sign = -1;

		else if(!strcmp(temp, "SIGNED"))
			sign = 1;

		else
			sign = 0;
	}

	else 
		throw(XMLException("Error: signed value not present, please put \"-1\" if you do not have a value"));
	

	//Load units number from "number" attribute in <units>
	if(!unitsE->Attribute("number", &unitsNumber)) 
		throw(XMLException("Error: number attribute missing from <units> please put \"-1\" if you do not have a value"));
	
	//if there is a value inside <units>
	if(unitsE->FirstChild()) 
		unitsName = unitsE->FirstChild()->Value();

	else 
		throw(XMLException("Error: units value not present, please put \"unknown\" if you do not have a value"));
	

	//if there is a value inside <type>
	if(typeE->FirstChild())
		typeName =typeE->FirstChild()->Value();

    else 
		throw(XMLException("Error: type value not present, please put \"unknown\" if you do not have a value"));
	

	//get number attribute from <projection>
	if(!projE->Attribute("number", &projNumber)) 
		throw(XMLException("Error: number attribute missing from <projection> please put \"-1\" if you do not have a value"));
	

	//if there is a value inside <name>
	if(projNameE->FirstChild())
		projName = projNameE->FirstChild()->Value();
	
	else 
		throw(XMLException("Error: name value not present for <name> under <projection>, please put \"unknown\" if you do not have a value"));
	

	//if there is a value inside <zone>
	if(zoneE->FirstChild())
		zone = atoi(zoneE->FirstChild()->Value());

	else 
		throw(XMLException("Error: zone value not present, please put \"-1\" if you do not have a value"));
	

	//get number attribute from <datum>
	if(!datumE->Attribute("number", &datumNumber)) 
		throw(XMLException("Error: missing number attribute from <datum>  please put \"-1\" if you do not have a value."));
	
	
	//if there is a value inside <datum>
	if(datumE->FirstChild())
		datumName = datumE->FirstChild()->Value();

	else 
		throw(XMLException("Error: name value not present for <name> under <datum>, please put \"unknown\" if you do not have a value"));

	//Get ulx attribute from <boundaries>
	if(!ulCornerE->Attribute("ulx", &ulx)) 
		throw(XMLException("Error: ulx attribute missing from <boundaries> please put \"-1\" if you do not have a value."));
	

	//Get uly attribute from <boundaries>
	if(!ulCornerE->Attribute("uly", &uly)) 
		throw(XMLException("Error: uly attribute missing from <boundaries> please put \"-1\" if you do not have a value."));
	
	
	//Set member variables.
	setAuthorName(authorName);
	setAuthorEmail(authorEmail);
	setAuthorCompany(authorCompany);
	setRows(rows);
	setCols(cols);
	setBits(bits);
	setFillValue(fillValue);
	setPixelSize(pixelSize);
	if(sign == 1)
		setSigned(true);
	else if(sign == 0)
		setSigned(false);
	setDatumName(datumName);
	setDatumNumber(datumNumber);
	setUnitsName(unitsName);
	setProjName(projName);
	setDataType(typeName);
	setProjNumber(projNumber);
	setUnitsNumber(unitsNumber);
	setZone(zone);
	setUlCorner(ulx,uly);
	setFilename(filename);

	//Set the 15 GCTPParameters values
	currentComment = paramsRootE->FirstChild();
	currentParam = paramsRootE->FirstChild()->NextSibling();
	for(int j = 0; j < 15; j++) {
		
		if(!currentComment || !currentParam) 
			throw(XMLException("Error: missing <param> or comment tag under <GCTPParameters>, there must be 15 parameters and 15 comments."));
		
		if(currentParam->FirstChild()) {
			GCTPParam = atof(currentParam->FirstChild()->Value());
			setParam(j, GCTPParam, currentComment->Value());
		}
		else
			throw(XMLException("Error, missing parameter value, use 0.0 if unknown"));
		
		if(j != 14) {
			currentParam = currentParam->NextSibling()->NextSibling();
			currentComment = currentComment->NextSibling()->NextSibling();
		}
		
	}
}

//Save image info to XML file named filename
bool RasterXML::save(const char* filename) {
	if(!m_filename && filename)
		setFilename(filename);
	if(m_doc && filename) {
		if(m_doc->SaveFile(filename)) 
			return true;
	}
	else
		return false;

return true;
}

//Save image info to XML file with the current filename
bool RasterXML::save() {
	if(!m_filename) {
		throw(XMLException("Error: filename not set, please use RasterXML::save(\"filename\")"));
		return false;
	}
	if(m_doc && m_filename) {
		if(m_doc->SaveFile(m_filename))
			return true;
	}

	else
		return false;

return false;
}

int RasterXML::getRows() const{
	return(m_rows);
}

int RasterXML::getCols() const{
	return(m_cols);
}

int RasterXML::getBits() const{
	return(m_bits);
}

int RasterXML::getProjNumber() const{
	return(m_projNumber);
}

int RasterXML::getZone() const{
	return(m_zone);
}

int RasterXML::getUnitsNumber() const{
	return(m_unitsNumber);
}

int RasterXML::getDatumNumber() const{
	return(m_datumNumber);
}

double RasterXML::getPixelSize() const{
	return(m_pixelSize);
}

double RasterXML::getUlx() const{
	return(m_ulx);
}

double RasterXML::getUly() const{
	return(m_uly);
}

double RasterXML::getGCTPParam(int index) const{
	return(m_GCTPParams[index]);
}

char* RasterXML::getProjName() const{
	return(m_projName);
}

char* RasterXML::getDatumName() const{
	return(m_datumName);
}

char* RasterXML::getUnitsName() const{
	return(m_unitsName);
}

char* RasterXML::getDataType() const{
	return(m_dataType);
}

char* RasterXML::getFilename() const{
	return(m_filename);
}

char* RasterXML::getAuthorName() const {
	return(m_authorName);
}

char* RasterXML::getAuthorCompany() const {
	return(m_authorCompany);
}

char* RasterXML::getAuthorEmail() const {
	return(m_authorEmail);
}

int RasterXML::isSigned() const{
	
	return(m_signed);
}

double RasterXML::getFillValue() const {
	return m_fillValue;
}

void RasterXML::setFillValue(double val) {

	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* fillValueT = handle.FirstChild("rasterInformation").FirstChild("pixelDescription").FirstChild("fillValue").FirstChild().Text();
		if(fillValueT) {
			char temp[20] = {'\0'};
			sprintf(temp, "%f", val);
			fillValueT->SetValue(temp);
		}

		else 
			throw(XMLException("Error: <fillValue> value not present"));

		m_fillValue = val;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

void RasterXML::setAuthorName(const char* name) {
	if(m_authorName)
		delete[] m_authorName;

	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* authorNameT = handle.FirstChild("rasterInformation").FirstChild("author").FirstChild("name").FirstChild().Text();
		if(authorNameT) {
			if(name) {
				authorNameT->SetValue(name);
				m_authorName = new char[strlen(name)+1];
				strcpy(m_authorName, name);
				m_authorName[strlen(name)] = '\0';
			}

			else {
				authorNameT->SetValue("Unknown");
				m_authorName = new char[8];
				strcpy(m_authorName, "Unknown\0");
			}
		}

		else 
			throw(XMLException("Error: <name> value not present under <author>"));

		 
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

void RasterXML::setAuthorCompany(const char* company) {
	if(m_authorCompany)
		delete[] m_authorCompany;

	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* authorCompanyT = handle.FirstChild("rasterInformation").FirstChild("author").FirstChild("company").FirstChild().Text();
		if(authorCompanyT) {
			if(company) {
				authorCompanyT->SetValue(company);
				m_authorCompany = new char[strlen(company)+1];
				strcpy(m_authorCompany, company);
				m_authorCompany[strlen(company)] = '\0';
			}

			else {
				authorCompanyT->SetValue("Unknown");
				m_authorCompany = new char[8];
				strcpy(m_authorCompany, "Unknown\0");
			}
		}

		else 
			throw(XMLException("Error: <company> value not present under <author>"));

		 
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

void RasterXML::setAuthorEmail(const char* email) {
	if(m_authorEmail)
		delete[] m_authorEmail;

	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* authorEmailT = handle.FirstChild("rasterInformation").FirstChild("author").FirstChild("email").FirstChild().Text();
		if(authorEmailT) {
			if(email) {
				authorEmailT->SetValue(email);
				m_authorEmail = new char[strlen(email)+1];
				strcpy(m_authorEmail, email);
				m_authorEmail[strlen(email)] = '\0';
			}

			else {
				authorEmailT->SetValue("Unknown");
				m_authorEmail = new char[8];
				strcpy(m_authorEmail, "Unknown\0");
			}
		}

		else 
			throw(XMLException("Error: <company> value not present under <author>"));

		 
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

//Set bits value in <image>
void RasterXML::setBits(int bits) {
	

	//Update internal document tree with new bits value.
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* bitsT = handle.FirstChild("rasterInformation").FirstChild("pixelDescription").FirstChild("bits").FirstChild().Text();
		if(bitsT) {
			char temp[20] = {'\0'};
			sprintf(temp, "%d", bits);
			bitsT->SetValue(temp);
			
		}

		else 
			throw(XMLException("Error: <bits> tag value missing"));

		m_bits = bits;
		
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
	
	

}

//Set projection number
void RasterXML::setProjNumber(int num) {
	

	//Update internal document tree with new projection number
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlElement* projE = handle.FirstChild("rasterInformation").FirstChild("projection").Element();
		if(projE) 
			projE->SetAttribute("number", num);
		else
			throw(XMLException("Error: <projection> tag missing"));

		m_projNumber = num;
	}
	else 
		throw(XMLException("Error: internal document tree not initialized"));
	
}

//Set units number
void RasterXML::setUnitsNumber(int num) {
	

	//Update internal document tree with new units number value
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlElement* unitsE = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("units").Element();
		if(unitsE)
			unitsE->SetAttribute("number", num);
		else
			throw(XMLException("Error: <units> tag missing"));

		m_unitsNumber = num;
	}
	else 
		throw(XMLException("Error: internal document tree not initialized"));

	
}			

//Set units name
void RasterXML::setUnitsName(const char* name) {
	
	if(m_unitsName)
		delete[] m_unitsName;
	

	//update internal document tree with new units name value
	if(m_doc) {
		
			TiXmlHandle handle(m_doc);
			TiXmlText* unitsT = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("units").FirstChild().Text();
			if(unitsT) {
				if(name) {
					unitsT->SetValue(name);
					m_unitsName = new char[strlen(name)+1];
					strcpy(m_unitsName, name);
					m_unitsName[strlen(name)] = '\0';
				}
				else {
					unitsT->SetValue("Unknown");
					m_unitsName = new char[8];
					strcpy(m_unitsName, "Unknown\0");
				}
			}
			else
				throw(XMLException("Error: <units> tag missing value"));
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));


}		

//Set zone
void RasterXML::setZone(int zone) {

	//Update internal document tree with new zone number
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* zoneT = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("zone").FirstChild().Text();
		if(zoneT) {
			char temp[20] = {'\0'};
			sprintf(temp, "%d", zone);
			zoneT->SetValue(temp);
		}

		else 
			throw(XMLException("Error <zone> tag missing value"));

		m_zone = zone;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));

	
	
}

//Set datum number
void RasterXML::setDatumNumber(int num) {
	
	//update internal document tree with new datum number value
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlElement* datumE = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("datum").Element();
		if(datumE) 
			datumE->SetAttribute("number", num);
		else
			throw(XMLException("Error: <datum> tag missing"));
		m_datumNumber = num;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));

	
}

//Set upper-left corner coordinates.
void RasterXML::setUlCorner(double ulx, double uly) {
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlElement* ulCornerE = handle.FirstChild("rasterInformation").FirstChild("area").FirstChild("ulCorner").Element();
		if(ulCornerE) {
			char tempX[40] = {'\0'};
			char tempY[40] = {'\0'};
			sprintf(tempX, "%f", ulx);
			sprintf(tempY, "%f", uly);
			ulCornerE->SetAttribute("ulx", tempX);
			ulCornerE->SetAttribute("uly", tempY);
		}
		else 
			throw(XMLException("Error: <ulCorner> tag missing"));

		m_ulx = ulx;
		m_uly = uly;
	}
	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

//Set a GCTPParam value
void RasterXML::setParam(int index, double value, const char* comment) {
	if(m_GCTPParams) {
		
		//update internal document tree with new param value
		if(m_doc) {

			if((index >= 0) && (index < 15)) {
				TiXmlHandle handle(m_doc);
				TiXmlNode* paramsRoot = handle.FirstChild("rasterInformation").FirstChild("GCTPParams").FirstChild().Node();
				if(paramsRoot) {
					char newVal[100] = {'\0'};
					sprintf(newVal, "%f", value);
					for(int i = 0; i < index*2; i++) 
						paramsRoot = paramsRoot->NextSibling();
					
					//get comment for this parameter
					TiXmlComment* commentText = paramsRoot->ToComment();
					
					//get parameter
					TiXmlText* paramText = paramsRoot->NextSiblingElement()->FirstChild()->ToText();
					
					//if comment tag exists, set it accordingly
					if(commentText) {
						if(comment) 
							commentText->SetValue(comment);
						else {
							char temp[12] = {'\0'};
							sprintf(temp, "Parameter %i", index);
							commentText->SetValue(temp);
						}
					}
					
					else
						throw(XMLException("Error: Parameter comment tag missing"));
					
					//if parameter tag exists, set its value
					if(paramText) 
						paramText->SetValue(newVal);
					
					else
						throw(XMLException("Error: <param> value missing"));

					m_GCTPParams[index] = value;
				}
				else
					throw(XMLException("Error: <param> tag missing"));
			}

			else 
				throw(XMLException("GCTP Parameter index out of bounds"));
		}
	}

	else
		throw(XMLException("Error: GCTPParams Array not initialized"));
}

//Set data type
void RasterXML::setDataType(const char* type) {

	if(m_dataType)
		delete[] m_dataType;
	
	//Update internal document tree with new data type value
	if(m_doc) {
		
			TiXmlHandle handle(m_doc);
			TiXmlText* dataTypeT = handle.FirstChild("rasterInformation").FirstChild("pixelDescription").FirstChild("type").FirstChild().Text();
			if(dataTypeT) {
				if(type) {
					dataTypeT->SetValue(type);
					m_dataType = new char[strlen(type)+1];
					strcpy(m_dataType, type);
					m_dataType[strlen(type)] = '\0';
				}
				else {
					dataTypeT->SetValue("Unknown");
					m_dataType = new char[8];
					strcpy(m_dataType, "Unknown\0");
				}
			}
			else
				throw(XMLException("Error: <type> tag missing value"));
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

//Set filename
//Should I keep this?
void RasterXML::setFilename(const char* filename) {
	

	if(m_filename) 
		delete[] m_filename;

	if(filename) {
		m_filename = new char[strlen(filename)+1];
		strcpy(m_filename, filename);
		m_filename[strlen(filename)] = '\0';
	}
	

}

//Set projection name
void RasterXML::setProjName(const char* name) {
	
	if(m_projName)
		delete[] m_projName;

	if(m_doc) {
		
			TiXmlHandle handle(m_doc);
			TiXmlText* projNameT = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("name").FirstChild().Text();
			if(projNameT) {
				if(name) {
					projNameT->SetValue(name);
					m_projName = new char[strlen(name)+1];
					strcpy(m_projName, name);
					m_projName[strlen(name)] = '\0';
				}
				else {
					projNameT->SetValue("Unknown");
					m_projName = new char[8];
					strcpy(m_projName, "Unknown\0");
				}
			}
			else
				throw(XMLException("Error: <name> tag missing value under <projection>"));
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));
}

//Set datum name
void RasterXML::setDatumName(const char* name) {
	if(m_datumName)
		delete[] m_datumName;

	if(m_doc) {
		
			TiXmlHandle handle(m_doc);
			TiXmlText* datumNameT = handle.FirstChild("rasterInformation").FirstChild("projection").FirstChild("datum").FirstChild().Text();
			if(datumNameT) {
				if(name) {
					datumNameT->SetValue(name);
					m_datumName = new char[strlen(name)+1];
					strcpy(m_datumName, name);
					m_datumName[strlen(name)] = '\0';
				}
				else {
					datumNameT->SetValue("Unknown");
					m_datumName = new char[8];
					strcpy(m_datumName, "Unknown\0");
				}
			}
			else
				throw(XMLException("Error: <name> tag missing value under <projection>"));
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));	
}

//Set rows
void RasterXML::setRows(int rows) {

	//update internal document tree with new rows value
	if(m_doc){
		TiXmlHandle handle(m_doc);
		TiXmlElement* dimensionsE = handle.FirstChild("rasterInformation").FirstChild("area").FirstChild("dimensions").Element();
		if(dimensionsE) 
			dimensionsE->SetAttribute("rows", rows);
		else
			throw(XMLException("<dimensions> tag missing"));

		m_rows = rows;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));	

}

//Set columns
void RasterXML::setCols(int cols) {

	
	//update internal document tree with new columns value
	if(m_doc){
		TiXmlHandle handle(m_doc);
		TiXmlElement* dimensionsE = handle.FirstChild("rasterInformation").FirstChild("area").FirstChild("dimensions").Element();
		if(dimensionsE) 
			dimensionsE->SetAttribute("columns", cols);
		else
			throw(XMLException("<dimensions> tag missing"));

		m_cols = cols;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));	
}

//Set pixel size
void RasterXML::setPixelSize(double pixelSize) {
	
	
	//Update internal document tree with new pixel size value
	if(m_doc){
		TiXmlHandle handle(m_doc);
		TiXmlText* pixelSizeT = handle.FirstChild("rasterInformation").FirstChild("pixelDescription").FirstChild("pixelSize").FirstChild().Text();
		if(pixelSizeT) {
			char temp[100] = {'\0'};
			sprintf(temp, "%f", pixelSize);
			pixelSizeT->SetValue(temp);
		}
		else
			throw(XMLException("<pixelSize> tag missing"));

		m_pixelSize = pixelSize;
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));	
}

//Set sign
void RasterXML::setSigned(bool isSigned) {

	
	//Update internal document with new sign value
	if(m_doc) {
		TiXmlHandle handle(m_doc);
		TiXmlText* signT = handle.FirstChild("rasterInformation").FirstChild("pixelDescription").FirstChild("sign").FirstChild().Text();
		if(signT) {
			if(isSigned)
				signT->SetValue("signed");
			else
				signT->SetValue("unsigned");
		}

		else 
			throw(XMLException("Error: <sign> tag value missing"));

		m_signed = isSigned;
		
	}

	else 
		throw(XMLException("Error: internal document tree not initialized"));	
}

//Overloaded output operator for RasterXML class
std::ostream& operator<<(std::ostream& os, const RasterXML& i) {
	os << "File: " << i.getFilename() << endl;
	os << "Author: " << i.getAuthorName() << endl;
	os << "\tCompany: " << i.getAuthorCompany() << endl;
	os << "\tEmail: " << i.getAuthorEmail() << endl;
	os << "Rows: " << i.getRows() << endl;
	os << "Columns: " << i.getCols() << endl;
	os << "Data Type: ";
	const char* signedS = "signed\0";
	const char* unsignedS = "unsigned\0";
	
	if(i.isSigned()) 
		os << signedS << " ";
	else
		os << unsignedS << " ";

	os << i.getBits() << " bit " << i.getDataType() << endl;

	os << "Units: " << i.getUnitsName() << endl;
	os << "Fill Value: " << i.getFillValue() << endl;
	os << "Pixel Size: " << i.m_pixelSize << endl;
	os << "Boundaries:   ulx=" << i.getUlx() << "   uly=" << i.getUly() << endl;
	os << "Projection: " << i.getProjName() << "     Zone(if UTM):" << i.getZone() << endl;
	os << "Datum: " << i.getDatumName() << endl;
	os << "GCTP Parameters: ";
	for(int j = 0; j < 15; j++) 
		os << i.getGCTPParam(j) << " ";
	os << endl;

	return(os);
}

//Overloaded output operator for ImageException class
std::ostream& operator<<(std::ostream& os, XMLException e) {
	os << e.m_message;
	return(os);
}


