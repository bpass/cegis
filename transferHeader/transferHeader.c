
//*************************************************************************************
//*Project: headerTransfer
//*Programmer: Michael Williams USGS
//*Date: June, 2004
//*
//*Description: This program extracts the Ephemeris header information from an
//*imported ASTER scene and transfers it to another Imagine .img file.
//*The reason this program was developed was because, when importing ASTER
//*data in Imagine, there is a certain amount of ephemeris data that put into
//*the Imagine file.  However, the imorted ASTER files are always imported in
//*geographic coordinates, and if you use Imagine's built-in reprojection
//*functionality, the ephemeris data that was present in the original file is lost.
//*This program extracts that data from the 
//*orignally imported file, and transfers it over to the reprojected file.
//*************************************************************************************



#include <string.h>
#include "transferHeader.h"

/*
** Error Macros 
*/
#define EFS0( m )	EERR_FATAL_SET_0( __LINE__, m )
#define EWS0( m )	EERR_WARNING_SET_0(__LINE__, m )
#define ES0( e, m )	EERR_SHOW_0( e, EERR_DEBUG, __LINE__, m )

Eeml_ApplicationFunction transferFunc;

//Does nothing 
static void processCommand(int, char **,Eerr_ErrorReport **);




/* 
** Menu Functions Declared
*/
static Eeml_TranslationTable menufunctions[] = 
{
  "doTransfer", transferFunc,
   NULL,NULL,  
};

/*
** The main application 
*/
int
main(
     int argc,
     char  **argv
)
{
	EERR_INIT("main", &lclerr, lclerr);
  	
	int						n;
	Emsc_Opaque				*context	= NULL;
	char					**buf		= (char **)NULL;
	Eint_InitToolkitData	*erdinit	= NULL;
	Eui_Root				*root		= NULL;
	Eml_ParseResult			*result		= NULL;
	int rc                              = -1;

	/* 
	** Initialize the ERDAS toolkit
	*/
	erdinit=eint_InitToolkit((Emsc_Opaque **)&lclerr );
	EERR_CHECK( lclerr,  EFS0("eint_InitToolkit Failed"));
 
	/* 
	** Initilize the ERDAS EML Toolkit Package 
	*/
	root = eeml_Init( erdinit, "Reproject Aster", argc, argv, &lclerr );
	EERR_CHECK( lclerr,  EFS0("Eeml_Init Error"));
  
	/* 
	** Parse out the eml with the frame parts 
	*/
	result = eeml_ParseVa(ELEX_FILE, "..\\..\\scripts\\transferHeader.eml",
				 root->rootPart, &lclerr,
				 EEML_PARSE_OPTION_APP_FUNCTIONS, menufunctions,
				 EEML_PARSE_OPTION_APP_CONTEXT, (Emsc_Opaque *)root,
				 NULL );
	EERR_CHECK( lclerr,  EFS0("eeml_ParseVa Error"));
  
 
	/*
	** Process the command line
	*/
	processCommand(argc, argv, &lclerr);
	ES0(lclerr ,  "Unable to processCommand!" );

	/* 
	** Sets up the command to listen to session manager.
	** Exits application on doneFlag.
	*/
	while (!root->doneFlag) 
	{
		n = 0;
		eeml_GetNextCommand(root, &context, &n, &buf, &lclerr );
		ES0(lclerr ,  "Unable to get next command!" );

		/*
		** Got a command from Session Manager, process it.
		*/
		processCommand(n, buf, &lclerr);
		ES0(lclerr ,  "Unable to processCommand!" );
		/*
		** We are not interested in any of the commands here, 
		** so free them
		*/
		eeml_FreeCommand(n, buf, &lclerr);
		ES0( lclerr ,"Unable to free up command!" );
	}

	rc = 0;
cleanup:
	ES0(lclerr, "Errors reported by main");
	eeml_ParseResultFree(result);

	return rc;
}


//*************************************************************
//*Function: transferFunc
//*Description: This function takes 2 filenames as its input 
//*arguments and transfers the band pass data from
//*file1 to file2
//*************************************************************
extern Estr_StringList* transferFunc(
	  Eeml_Menu menu,
	Emsc_Opaque *context,
	long argc,
	char **argv,
	Eerr_ErrorReport **outerr
)
{
	
	static bandPass bp; //store band pass data;
	static HDFBandAttributes ba; //store HDFBandAttributes data
	static HDFGlobalAttributes ga; //store HDFGlobalAttributes data

	Ehfa_File*		inFile = NULL; //import files
	Ehfa_File*		outputFile = NULL; //output file
	Estr_StringList*  namelist = NULL; //stores layer names of input file
	Estr_StringList*  newNameList = NULL; //stores layer names of output file
	Eint_InitToolkitData* erdinit = NULL; //toolkit initialization data

	Emif_Design* bandPassObjectDesign = NULL;//design of bandPass object
	Emif_Design* bandAttributesObjectDesign = NULL; //design of HDFBandAttributes object
	Emif_Design* globalAttributesObjectDesign = NULL; //design for HDFGlobalAttributes

	Emif_Design* temp = NULL;  //used to check for the existance of a bandPAss object.
	Emif_Dictionary* inputDict = NULL; //dictionary for import file
	Emif_Dictionary* outputDict = NULL; //dictionary for reprojected file
	char* bandPassPath = NULL; //node path to band pass data
	char* bandAttrPath = NULL; //band attributes path.
	char* tempstring = NULL;
	char* bandAttrDesignName = NULL; //design name for HDFBandAttributes is different for each layer, store it here.
	char* bandAttrDesignNameOut = NULL;
	char* tempstring2 = NULL;

	long i = 0; //counter, but you should already know that.
	long j = 0;
	int k = 0;
	int numInputFiles = 0; //How many input images are we using?
	long numLayers = 0; //number of layers in the input file.
	long totalInputLayers = 0; //total number of input layers in all of the input images
	long numLayersOut = 0; //number of layers in the output file
	long currentLayerNumber = 0;
	char* files[3] = {NULL, NULL, NULL};
	char* outFile = NULL; //reprojected file filename
	int* matchingLayerNameLocations = NULL; //stores numerical indices of the array numbers in the input file
									        //that match with array numbers in the output file. This exists in case
											//the user removed some layers from the output file, but still wants to 
											//transfer the data over to those that remain.
	
	//init error checking
	EERR_INIT("transferFunc", &lclerr, lclerr);

	
	if(argc != 4) {
		eeml_DialogMessage(NULL, "Error: not enough input files", &lclerr);
		return NULL;
	}

	//get command line args
	files[0] = estr_StringCopy(files[0], argv[0]);
	files[1] = estr_StringCopy(files[1], argv[1]);
	files[2] = estr_StringCopy(files[2], argv[2]);
	outFile = estr_StringCopy(outFile, argv[3]);
	
	//make sure there's at least 1 input and output file.	
	if(!strcmp(files[0], "") || !strcmp(outFile, "")) {
		eeml_DialogMessage(NULL, "Error: You must specify at least one input file, and an output file.", &lclerr);
		return NULL;
	}

	//check how many valid input files we have
	for(i = 0; i < 3; i++) {
		if(strcmp(files[i], ""))
			numInputFiles++;
	}

	erdinit = eint_GetInit();

	

	newNameList = eimg_LayerGetNames(outFile, erdinit, &lclerr);
	EERR_CHECK(lclerr, EFS0("Error getting output file layer names"));

	numLayersOut = newNameList->count;
	EERR_CHECK(numLayersOut <=0, EFS0("No Layers found in the output file"));
	
	//check that the total number of input layers in all images, 
	//corresponds to total number of layers in the output image.
	for(k = 0; k < numInputFiles; k++) {

		//get layer names for current input file
		namelist = eimg_LayerGetNames(files[k], erdinit,&lclerr);
		EERR_CHECK(lclerr, EFS0("Error getting input file layer names.")); 

		numLayers = namelist->count;
		EERR_CHECK(numLayers <=0,EFS0("No layers found in the input image file"));

		totalInputLayers += numLayers;
	}

	if(totalInputLayers != numLayersOut) {
		eeml_DialogMessage(NULL, "Error: total number of layers from input files does not match number of layers in output file.", &lclerr);
		return NULL;
	}
	
	
	
	outputFile = ehfa_FileOpen(outFile, "r+", erdinit, &lclerr);
	ES0(lclerr, "file open failed");
	

	//get output dictionary
	outputDict = outputFile->dictionary;

	for(k = 0; ((k < numInputFiles) && (currentLayerNumber < numLayersOut)); k++) {
		//get layer names for current input file
		namelist = eimg_LayerGetNames(files[k], erdinit,&lclerr);
		EERR_CHECK(lclerr, EFS0("Error getting input file layer names.")); 

		//get number of layers in this image
		numLayers = namelist->count;
		EERR_CHECK(numLayers <=0,EFS0("No layers found in the input image file"));

		//open current input file
		inFile = ehfa_FileOpen(files[k], "r+", erdinit, &lclerr);
		ES0(lclerr, "file open failed");

		//get current dictionary
		inputDict = inFile->dictionary;

		//Get band pass object design from input dictionary
		bandPassObjectDesign = emif_DictionarySearchByName(inputDict, "BandPass", &lclerr);
		ES0(lclerr, "Error locating BandPass Design in input Dictionary");

	
		if(bandPassObjectDesign==NULL) {
			eeml_DialogMessage(NULL, "Error: Band Pass information missing from input file", &lclerr);
			return NULL;
		}

		

		if(k == 0) {
			//check for presence of header data
			temp = emif_DictionarySearchByName(outputDict, "BandPass", &lclerr);
			ES0(lclerr, "Error in dictionary search");
			//check for presence of header data
			temp = emif_DictionarySearchByName(outputDict, "ASTERHeader", &lclerr);
			ES0(lclerr, "Error in dictionary search");
	

			if(temp != NULL) {
				eeml_DialogMessage(NULL, "Error: image already contains the header information", &lclerr);
				return NULL;
			}

			//add BandPass design to output dictionary, only needs to be done once,
			//since it is universal.
			emif_DictionaryAddDesign(outputDict, bandPassObjectDesign, &lclerr);
			ES0(lclerr, "Error adding band pass design to output file dictionary");
		}

		//only add the global attributes if we are using 1 input file.
		if(numInputFiles == 1) {
			globalAttributesObjectDesign = emif_DictionarySearchByName(inputDict, "ASTERHeader", &lclerr);
			ES0(lclerr, "Error locating BandPass Design in input Dictionary");

			if(globalAttributesObjectDesign == NULL) {
				eeml_DialogMessage(NULL, "Error: HDFGlobalAttribute information missing from input file", &lclerr);
				return NULL;
			}
			emif_DictionaryAddDesign(outputDict, globalAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error adding band pass design to output file dictionary");
		
			//Read Global Attribute info
			ehfa_ObjectRead(inFile, ":HDFGlobalAttributes", "HDFGlobalAttributes", EHFA_SEEK_PATH, (void*)&ga, globalAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error reading global attributes");

			//write global attribute data
			ehfa_ObjectWrite(outputFile, ":", "HDFGlobalAttributes", "ASTERHeader", EHFA_WRITE_FIRST_CHILD, (void*)&ga, globalAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error writing global attributes");
		}

		//loop for each layer in input image
		for(i = 0; i < numLayers; i++) {
		
			//pull out node path from layer name
			eimg_LayerNameParse(namelist->strings[i], NULL, NULL, &tempstring, &lclerr);
			ES0(lclerr, "Error in LayerNameParse");
		
			//build paths
			bandPassPath = estr_StringCat(bandPassPath, tempstring, ":BandPass");
			bandAttrPath = estr_StringCat(bandAttrPath, tempstring, ":HDFBandAttributes");

			//the band attribute object names differ by layer, and are of the form
			//<layer name>Header. Here we remove the ':' character from the layer path
			//returned by LayerNameParse
			tempstring2 = removeChar(tempstring, ':');
			bandAttrDesignName = estr_StringCat(bandAttrDesignName, tempstring2, "Header");

			//get layer attribute object design for this layer.
			bandAttributesObjectDesign = emif_DictionarySearchByName(inputDict, bandAttrDesignName, &lclerr);
			ES0(lclerr, "Error locating BandPass Design in input Dictionary");
	
			if(bandAttributesObjectDesign == NULL) {
				eeml_DialogMessage(NULL, "Error: Missing Band atrribute information", &lclerr);
				return NULL;
			}
		
			emsc_Free(tempstring);
			tempstring = NULL;
			emsc_Free(tempstring2);
			tempstring2 = NULL;

			//get layer name of output file
			eimg_LayerNameParse(newNameList->strings[i], NULL, NULL, &tempstring, &lclerr);
			ES0(lclerr, "Parsing layer name");

			//Here we're building the band attribute node names relative to the output file,
			//this fixes a bug that causes files that have just had their data replenished with 
			//this utility to be unable to be used as source files for additional transfers.
			tempstring = removeChar(tempstring, ':');
			tempstring2 = estr_StringCat(tempstring2, tempstring, "Header");

			//change node design name to be relative to the output file.
			if(bandAttributesObjectDesign->name != NULL)
				emsc_Free(bandAttributesObjectDesign->name);
			bandAttributesObjectDesign->name = estr_Duplicate(tempstring2);
			bandAttrDesignNameOut = estr_Duplicate(tempstring2);

			if(k == 0) {
				//Check to see if layer attribute info allready exists in output file.
				temp = emif_DictionarySearchByName(outputDict, tempstring2, &lclerr);
				ES0(lclerr, "Error locating BandPass Design in input Dictionary");

				if(temp != NULL) {
					eeml_DialogMessage(NULL, "Error: layer attribute info allready exists in output file", &lclerr);
					return NULL;
				}
			}

			//Read in band pass and layer attribute data
			ehfa_ObjectRead(inFile, bandPassPath, "BandPass", EHFA_SEEK_PATH, (void*)&bp, bandPassObjectDesign, &lclerr);
			ES0(lclerr, "Error reading band pass object");
			ehfa_ObjectRead(inFile, bandAttrPath, "HDFBandAttributes", EHFA_SEEK_PATH, (void*)&ba, bandAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error reading band pass object");
		

			//add the band attribute design for this layer
			emif_DictionaryAddDesign(outputDict, bandAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error adding band pass design to output file dictionary");

			emsc_FreeFunc(tempstring2);
			tempstring2 = NULL;

			//read the name of the first layer in the output file, because it
			//may not match the layer name of the input file.
			eimg_LayerNameParse(newNameList->strings[currentLayerNumber], NULL, NULL, &tempstring2, &lclerr);
			ES0(lclerr, "Error in LayerNameParse");

			//write band pass and layer attribute data 
			ehfa_ObjectWrite(outputFile, tempstring2, "BandPass", "BandPass", EHFA_WRITE_FIRST_CHILD, (void*)&bp, bandPassObjectDesign, &lclerr);
			ES0(lclerr, "Error writing band pass object");
			ehfa_ObjectWrite(outputFile, tempstring2, "HDFBandAttributes", bandAttrDesignNameOut, EHFA_WRITE_LAST_CHILD, (void*)&ba, bandAttributesObjectDesign, &lclerr);
			ES0(lclerr, "Error writing band pass object");

		
			//free memory and reinitialize strings
			emsc_FreeFunc(tempstring);
			tempstring = NULL;
		
			emsc_FreeFunc(bandPassPath);
			bandPassPath = NULL;

			emsc_FreeFunc(bandAttrPath);
			bandAttrPath = NULL;	

			emsc_FreeFunc(tempstring2);
			tempstring2 = NULL;

			emsc_FreeFunc(bandAttrDesignName);
			bandAttrDesignName = NULL;

			emsc_FreeFunc(bandAttrDesignNameOut);
			bandAttrDesignNameOut = NULL;

			currentLayerNumber++;
		}

		ehfa_FileClose(inFile, &lclerr);
		EERR_CHECK(lclerr, EFS0("Error closing file"));
	}
	ehfa_FileClose(outputFile, &lclerr);
	EERR_CHECK(lclerr, EFS0("Error closing file"));

	eeml_DialogMessage(NULL, "Transfer Complete", &lclerr);

//do garbage collection
cleanup:
	if(namelist)
		estr_StringListDelete(namelist, &lclerr);
	if(newNameList)
		estr_StringListDelete(newNameList, &lclerr);
	for(i = 0; i < 3; i++) {
		if(files[i] != NULL) 
			emsc_FreeFunc(files[i]);
	}
	if(outFile)
		emsc_FreeFunc(outFile);
	if(bandPassPath)
		emsc_FreeFunc(bandPassPath);
	if(bandAttrPath)
		emsc_FreeFunc(bandAttrPath);
	if(tempstring)
		emsc_FreeFunc(tempstring);
	if(bandAttrDesignName)
		emsc_FreeFunc(bandAttrDesignName);
	if(tempstring)
		emsc_FreeFunc(tempstring);
	if(tempstring2)
		emsc_FreeFunc(tempstring2);
	
	return NULL;
}





//Does nothing
static void
processCommand(int argc, char **argv, Eerr_ErrorReport **outerr)
{
	
}


//************************************************************
//*Function: removeChar
//*Description: This function takes an input string and 
//*returns a pointer to a new string, without the character
//*specified by toRemove
//***********************************************************
static char* removeChar(const char* input, char toRemove) {
	int i = 0;
	int pos = 0;
	int  len = 0;
	char* ret = NULL;

	if(input == NULL)
		return NULL;
	
	len = strlen(input);
	ret = (char*)malloc(len+1);
	
	for(i = 0; i < len; i++)
		ret[i] = '\0';

	for(i = 0; (i < len && pos < len); i++) {
		if(input[i] == toRemove)
			continue;
		else {
			ret[pos] = input[i];
			pos++;
		}
	}
	return ret;
}




