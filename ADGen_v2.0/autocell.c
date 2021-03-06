/*************************************************************************
 Application : applTemplate

 Purpose: This will provide the template for all applications.

 Notes:
 This template is the outline for how an Application is to be set up. This 
 example will initialize, parse and launch an EML. On "Ok" the application 
 will display on the EML "HELLO WORLD".
 
*************************************************************************
*************************************************************************/

/*
** Toolkit Includes
*/
#define bool int
#define true 1 
#define false 0

#include <stdio.h>
#include <eint.h>
#include <eerr.h>
#include <eeml.h>
#include <eeml_dialog.h>
#include <estr.h>
#include <esmg.h>
#include <edsc.h>
#include <ehfa.h>
#include <eimg.h>
#include <esta.h>
#include <emsc.h>

#include <process.h>

/*
** Error Macros 
*/
#define EFS0( m )	EERR_FATAL_SET_0( __LINE__, m )
#define EWS0( m )	EERR_WARNING_SET_0(__LINE__, m )
#define ES0( e, m )	EERR_SHOW_0( e, EERR_DEBUG, __LINE__, m )
#define MESSAGE_COMMAND  "-message"
/*
** Application Function declarations
*/
Eeml_ApplicationFunction determineCell;
Eeml_ApplicationFunction okFunc;

/* 
** Menu Functions Declared
*/
static Eeml_TranslationTable menufunctions[] = 
{
  "determineCell",          determineCell,
   "exitB",					okFunc,
   NULL,NULL,  
};

//Global Functions
void movPos(long *, long *, int direction, int dataType);

bool checkAndSet(Eimg_PixelRect *pixCellNum, Eimg_PixelRect *pixFlowDir, long colPos, int *direction, 
				 long *outCellNum, int dataType);

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
	root = eeml_Init( erdinit, "Determine Outlet Cell", argc, argv, &lclerr );
	EERR_CHECK( lclerr,  EFS0("Eeml_Init Error"));
  
	/* 
	** Parse out the eml with the frame parts 
	*/
	result = eeml_ParseVa(ELEX_FILE, "autoCell.eml",
				 root->rootPart, &lclerr,
				 EEML_PARSE_OPTION_APP_FUNCTIONS, menufunctions,
				 EEML_PARSE_OPTION_APP_CONTEXT, (Emsc_Opaque *)root,
				 NULL );
	EERR_CHECK( lclerr,  EFS0("eeml_ParseVa Error"));
  
 
	/*
	** Process the command line
	*/
	//processCommand(argc, argv, &lclerr);
	//ES0(lclerr ,  "Unable to processCommand!" );

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
		
		processCommand(n, buf, &lclerr);
		ES0(lclerr ,  "Unable to processCommand!" );*/
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



//****************************************************************************
// FUNCTION NAME
// determineCell
//	follows the major streams in the watershed until you get to the outlet cell
//  (pour point)
//
// SYNTAX
// extern Estr_StringList *
//		getAttributes(menu, context, argc, argv, err)
//
// ARGUMENTS
//
//	Eeml_Menu menu;
//	Emsc_Opaque *context
//	long argc;
//	char **argv
//	Eerr_ErrorReport **err
//
// DESCRIPTION
//	For this function to work, it takes a 3-layer stacked image generated by the
//  Cell-Stack mdl.  Layer 1 is Cell Numbering, Layer 2 is Flow Direction, and Layer
//  3 is Strahler Stream Order.  The function begins in the middle of the watershed 
//  and follows the path specified by Layer 2.  When it reaches the boundary, it returns
//  the value of the last valid numbered cell.
// for its models to run.
//
// RETURN VALUE
// 
//	Estr_StringList *
//
//   
//****************************************************************************


extern Estr_StringList *
determineCell(
	Eeml_Menu menu,
	Emsc_Opaque *context,
	long argc,
	char **argv,
	Eerr_ErrorReport **outerr
)
{
	Estr_StringList*			listNames = NULL;
	Estr_StringList*			retVal = NULL;

	Eimg_Layer*					cellNum = NULL;
	Eimg_Layer*					flowDir = NULL;
	Eimg_Layer*					strahler = NULL;

	Eimg_PixelRect*				pixCellNum = NULL;
	Eimg_PixelRect*				pixFlowDir = NULL;
	Eimg_PixelRect*				pixStrahler = NULL;
	
	Esta_Statistics*			stats = NULL;

	char*						temp = NULL;

	long						maxStream = 0, outnum = 0,
								rowPos = 0, colPos = 0, colBegin = 0,
								colEnd = 0, outCellNum = 0;
	
	int							rows = 0, cols = 0, debugctr=0, direction = 0;
	
	bool						outletFound = false;


	EERR_INIT("determineCell", outerr, lclerr);
	
	//Get the list of layer names for the stacked image
	listNames = eimg_LayerGetNames(argv[0], eint_GetInit(), &lclerr);
	EERR_CHECK(lclerr, EFS0("Error Getting Name"));


	//Open layer 1 of the stacked image
	cellNum = eimg_LayerOpen(listNames->strings[0], eint_GetInit(), &lclerr, 
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lclerr, EFS0("Error opening layerstack"));
	
	//Create a structure to hold a row of pixel values
	pixCellNum = eimg_PixelRectCreate(cellNum->width, 1, cellNum->pixelType, &lclerr);
	EERR_CHECK(lclerr, EFS0("Error creating pixel rect"));

	//Read in Row 0 of pixel values to pixCellNum
	eimg_LayerRead(cellNum, 0, 0, cellNum->width, 1, pixCellNum, &lclerr);
	EERR_CHECK(lclerr, EFS0("Error reading row of pixels"));

	//set the row position value to halfway down the data set to make sure
	//our first data value is valid
	rowPos = (((long)cellNum->height) / 2);



	//*****************************************************
	//8 Bit Unsigned Integer
	//*****************************************************
	if(cellNum->pixelType==3)
	{
		//find middle of valid range in current row of data
		eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
		EERR_CHECK(lclerr, EFS0("Error reading pixels"));

		
		for(cols = 0; cols < (cellNum->width); cols=cols+1) 
		{


			outnum = pixCellNum->currentData[cols];

			if(outnum != 0)
			{
				colBegin = (cols);
				break;
			}	
		}

		for(cols = colBegin; cols < (cellNum->width); cols=cols+1)
		{

			outnum = pixCellNum->currentData[cols];
			if(outnum == 0)
			{
				colEnd = (cols-1);
				break;
			}
		}

		//Find the middle column of the current row
		colPos = ((colBegin + colEnd) / 2);

		//Just standard output so i know this thing works
		outCellNum = ((pixCellNum->currentData[colPos + 1]<<8) | pixCellNum->currentData[colPos]);

		///////////////////////////////////////////////////////
		//Get max stream order from *.gord.img
		///////////////////////////////////////////////////////
		strahler = eimg_LayerOpen(listNames->strings[2], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);
		EERR_CHECK(lclerr, EFS0("Error opening Layer 3 of Stacked Image!"));

		stats = eimg_StatisticsRead(strahler, NULL, &lclerr);
		EERR_CHECK(lclerr, EFS0("Stat Read on Strahler Failed!"));

		maxStream = (long)stats->maximum;
		

		///////////////////////////////////////////////////////////////////////////
		//Open FlowDirection image (parameter 5) and find value at current pixel
		//then move the current position accordingly
		///////////////////////////////////////////////////////////////////////////

		flowDir = eimg_LayerOpen(listNames->strings[1], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);

		pixFlowDir = eimg_PixelRectCreate(flowDir->width, 1, flowDir->pixelType, &lclerr);
		EERR_CHECK(lclerr, EFS0("PixRectCreat for Layer 2 Failed!"));

		eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
		EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

		direction = pixFlowDir->currentData[colPos];
		

		//Loop that calls movPos and checkAndSet to continually follow the flow direction
		while(!outletFound)
		{
			movPos(&rowPos, &colPos, direction, cellNum->pixelType);
			
			//Read the new row of cell numbers
			eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 1 failed!"));

			//Read the new row of Flow Directions
			eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

			outletFound = checkAndSet(pixCellNum, pixFlowDir, colPos, &direction, &outCellNum, cellNum->pixelType);
			
			//Debug / Runtime stuff
			debugctr++;
			esmg_LogMessage(ESMG_LOG_VERBOSE, &lclerr, "Counter: %i", debugctr);
		}

		//The next to function calls are just a funky thing you need to do to get
		//numbers to be returned properly to the EML
		temp = estr_Sprintf(NULL, "%d", &lclerr, outCellNum);
		EERR_SHOW(lclerr, EERR_DEBUG);

		retVal = estr_StringListAppend(retVal, temp, &lclerr);
		EERR_CHECK(lclerr, EFS0("StringListAppend Failed!"));

		emsc_Free(temp);

	}


	//*****************************************************
	//16 Bit Unsigned Integer
	//*****************************************************
	if(cellNum->pixelType == 5)
	{
		//find middle of valid range in current row of data
		eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
		EERR_CHECK(lclerr, EFS0("Error reading pixels"));

		
		for(cols = 0; cols < (cellNum->width)*2; cols=cols+2) 
		{

			/*******************************
			SPECIAL NOTE:  For 16-Bit data in .img files, the current number (in binary) actually takes 
			two places in the array to represent.  For example, to get the number for column 10, you 
			need to read the data in column 11, shift it left by 8 bits, then bit-wise OR it with data 
			in column 10.  Thats why you see the following syntax to get the current cell number.
			*********************************/
			outnum = ((pixCellNum->currentData[cols+1] << 8) | pixCellNum->currentData[cols]);

			if(outnum != 0)
			{
				colBegin = (cols);
				break;
			}	
		}

		for(cols = colBegin*2; cols < (cellNum->width)*2; cols=cols+2)
		{

			outnum = ((pixCellNum->currentData[cols+1] << 8) | pixCellNum->currentData[cols]);
			if(outnum == 0)
			{
				colEnd = (cols-2);
				break;
			}
		}

		//Find the middle column of the current row
		colPos = ((colBegin + colEnd) / 2);

		//Check to make sure we are at an even number, so as not to mess up our 'bit-wise or'
		//operations
		if((colPos % 2) != 0)
		{
			colPos = (colPos - 1);
		}

		//Just standard output so i know this thing works
		outCellNum = ((pixCellNum->currentData[colPos + 1]<<8) | pixCellNum->currentData[colPos]);

		///////////////////////////////////////////////////////
		//Get max stream order from *.gord.img
		///////////////////////////////////////////////////////
		strahler = eimg_LayerOpen(listNames->strings[2], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);
		EERR_CHECK(lclerr, EFS0("Error opening Layer 3 of Stacked Image!"));

		stats = eimg_StatisticsRead(strahler, NULL, &lclerr);
		EERR_CHECK(lclerr, EFS0("Stat Read on Strahler Failed!"));

		maxStream = (long)stats->maximum;
		

		///////////////////////////////////////////////////////////////////////////
		//Open FlowDirection image (parameter 5) and find value at current pixel
		//then move the current position accordingly
		///////////////////////////////////////////////////////////////////////////

		flowDir = eimg_LayerOpen(listNames->strings[1], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);

		pixFlowDir = eimg_PixelRectCreate(flowDir->width, 1, flowDir->pixelType, &lclerr);
		EERR_CHECK(lclerr, EFS0("PixRectCreat for Layer 2 Failed!"));

		eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
		EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

		direction = pixFlowDir->currentData[colPos];
		

		//Loop that calls movPos and checkAndSet to continually follow the flow direction
		while(!outletFound)
		{
			movPos(&rowPos, &colPos, direction, cellNum->pixelType);
			
			//Read the new row of cell numbers
			eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 1 failed!"));

			//Read the new row of Flow Directions
			eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

			outletFound = checkAndSet(pixCellNum, pixFlowDir, colPos, &direction, &outCellNum, cellNum->pixelType);
			
			//Debug / Runtime stuff
			debugctr++;
			esmg_LogMessage(ESMG_LOG_VERBOSE, &lclerr, "Counter: %i", debugctr);
		}

		//The next to function calls are just a funky thing you need to do to get
		//numbers to be returned properly to the EML
		temp = estr_Sprintf(NULL, "%d", &lclerr, outCellNum);
		EERR_SHOW(lclerr, EERR_DEBUG);

		retVal = estr_StringListAppend(retVal, temp, &lclerr);
		EERR_CHECK(lclerr, EFS0("StringListAppend Failed!"));

		emsc_Free(temp);
	}

	//*************************************************
	//32 Bit Unsigned Integer
	//*************************************************

	else if(cellNum->pixelType==7)
	{
		//find middle of valid range in row rowPos of data
		eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
		EERR_CHECK(lclerr, EFS0("Error reading pixels"));
		


		for(cols = 0; cols < (cellNum->width)*4; cols=cols+4) 
		{

			/*******************************
			SPECIAL NOTE:  For 32 bit integer, we need to 'bit-wise OR' four sepearate
			positions with data to obtain the number.  Much the same way we did it 
			with the 16 Bit Integers, except now we do it with a factor of 2 more
			(That is, we multiply our counters by 4, and add or subtract 4 when moving
			columns)
			*********************************/
			outnum = ((pixCellNum->currentData[cols+3]<<24) | 
						(pixCellNum->currentData[cols+2]<<16) |
						(pixCellNum->currentData[cols+1]<< 8) | 
						pixCellNum->currentData[cols]);

			if(outnum != 0)
			{
				colBegin = (cols);
				break;
			}	
		}

		for(cols = colBegin; cols < (cellNum->width)*4; cols=cols+4)
		{
			outnum = ((pixCellNum->currentData[cols+3]<<24) | 
						(pixCellNum->currentData[cols+2]<<16) |
						(pixCellNum->currentData[cols+1]<< 8) | 
						pixCellNum->currentData[cols]);

			if(outnum == 0)
			{
				colEnd = (cols-4);
				break;
			}
		}

		//Establish middle of valid data in middle row
		colPos = ((colBegin + colEnd) / 2);

		//Check to make sure we are at a number divisible by 4, so as not to 
		//mess up our 4-part 'bit-wise or' operations  
		if((colPos % 4) == 1)
		{
			colPos = (colPos - 1);
		}
		else if((colPos % 4) == 2)
		{
			colPos = (colPos-2);
		}
		else if((colPos%4)==3)
		{
			colPos = (colPos-3);
		}


		//Just standard output so i know this thing works
		outCellNum = ((pixCellNum->currentData[colPos+3]<<24) | 
						(pixCellNum->currentData[colPos+2]<<16) |
						(pixCellNum->currentData[colPos+1]<< 8) | 
						pixCellNum->currentData[colPos]);

		///////////////////////////////////////////////////////
		//Get max stream order from *.gord.img
		///////////////////////////////////////////////////////
		strahler = eimg_LayerOpen(listNames->strings[2], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);
		EERR_CHECK(lclerr, EFS0("Error opening Layer 3 of Stacked Image!"));

		stats = eimg_StatisticsRead(strahler, NULL, &lclerr);
		EERR_CHECK(lclerr, EFS0("Stat Read on Strahler Failed!"));

		maxStream = (long)stats->maximum;

		///////////////////////////////////////////////////////////////////////////
		//Open FlowDirection image (parameter 5) and find value at current pixel
		//then move the current position accordingly
		///////////////////////////////////////////////////////////////////////////

		flowDir = eimg_LayerOpen(listNames->strings[1], eint_GetInit(), &lclerr, EIMG_LAYER_OPTION_END);

		pixFlowDir = eimg_PixelRectCreate(flowDir->width, 1, flowDir->pixelType, &lclerr);
		EERR_CHECK(lclerr, EFS0("PixRectCreat for Layer 2 Failed!"));

		eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
		EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

		direction = pixFlowDir->currentData[colPos];
		esmg_LogMessage(ESMG_LOG_VERBOSE, &lclerr, "Current Flow Direction Is: %i", direction);
		

		////////////////////////////////////
		//move position
		///////////////////////////////////

		//Loop that calls movPos and checkAndSet to continually follow the flow direction
		while(!outletFound)
		{
			movPos(&rowPos, &colPos, direction, cellNum->pixelType);
			
			//Read in the new rows of cell numbers and flow directions
			eimg_LayerRead(cellNum, 0, rowPos, cellNum->width, 1, pixCellNum, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 1 failed!"));

			eimg_LayerRead(flowDir, 0, rowPos, flowDir->width, 1, pixFlowDir, &lclerr);
			EERR_CHECK(lclerr, EFS0("LayerRead for Layer 2 failed!"));

			outletFound = checkAndSet(pixCellNum, pixFlowDir, colPos, &direction, &outCellNum, cellNum->pixelType);
			
			//Debug / Runtime stuff
			debugctr++;
			esmg_LogMessage(ESMG_LOG_VERBOSE, &lclerr, "Counter: %i", debugctr);
		}


		//Method of returning a number to EML
		temp = estr_Sprintf(NULL, "%d", &lclerr, outCellNum);
		EERR_SHOW(lclerr, EERR_DEBUG);

		retVal = estr_StringListAppend(retVal, temp, &lclerr);
		EERR_CHECK(lclerr, EFS0("StringListAppend Failed!"));

		emsc_Free(temp);

	}

	//*****************************************************
	//32 Bit Float
	//*****************************************************
	if(cellNum->pixelType==9)
	{

	}

//Close everything we opened
cleanup:
	eimg_PixelRectDelete(pixCellNum, &lclerr);
	pixCellNum = NULL;

	eimg_PixelRectDelete(pixFlowDir, &lclerr);
	pixCellNum = NULL;

	eimg_LayerClose(cellNum, &lclerr);
	cellNum = NULL;

	eimg_LayerClose(flowDir, &lclerr);
	cellNum = NULL;

	eimg_LayerClose(strahler, &lclerr);
	cellNum = NULL;

	estr_StringListDelete(listNames, &lclerr);
	listNames = NULL;

	return retVal;
}

/******************************************************************************
/** FUNCTION: movPos
/** ARGUMENTS: Pointers to rowPos and colPos, as well as 'direction' and 'dataType'
/** DESCRIPTION: This function reads the current flowdirection, and then moves
/** the current position accordingly.  
/*****************************************************************************/
void movPos(long *rowPos, long *colPos, int direction, int dataType)
{
	//Unsigned 8 Bit Integer
	if(dataType == 3)
	{
		if(direction == 1)
		{
			*colPos = *colPos + 1;
		}
		
		if(direction == 2)
		{
			*colPos = *colPos + 1;
			*rowPos = *rowPos + 1;
		}

		if(direction == 4)
		{
			*rowPos = *rowPos + 1;
		}

		if(direction == 8)
		{
			*colPos = *colPos - 1;
			*rowPos = *rowPos + 1;
		}

		if(direction == 16)
		{
			*colPos = *colPos - 1;
		}

		if(direction == 32)
		{
			*colPos = *colPos - 1;
			*rowPos = *rowPos - 1;
		}

		if(direction == 64)
		{
			*rowPos = *rowPos - 1;
		}

		if(direction == 128)
		{
			*colPos = *colPos + 1;
			*rowPos = *rowPos - 1;
		}
	}

	//Unsigned 16 Bit Integer
	if(dataType == 5)
	{
		if(direction == 1)
		{
			*colPos = *colPos + 2;
		}
		
		if(direction == 2)
		{
			*colPos = *colPos + 2;
			*rowPos = *rowPos + 1;
		}

		if(direction == 4)
		{
			*rowPos = *rowPos + 1;
		}

		if(direction == 8)
		{
			*colPos = *colPos - 2;
			*rowPos = *rowPos + 1;
		}

		if(direction == 16)
		{
			*colPos = *colPos - 2;
		}

		if(direction == 32)
		{
			*colPos = *colPos - 2;
			*rowPos = *rowPos - 1;
		}

		if(direction == 64)
		{
			*rowPos = *rowPos - 1;
		}

		if(direction == 128)
		{
			*colPos = *colPos + 2;
			*rowPos = *rowPos - 1;
		}
	}

	//Unsigned 32 Bit Integer
	if(dataType == 7)
	{
		if(direction == 1)
		{
			*colPos = *colPos + 4;
		}
		
		else if(direction == 2)
		{
			*colPos = *colPos + 4;
			*rowPos = *rowPos + 1;
		}

		else if(direction == 4)
		{
			*rowPos = *rowPos + 1;
		}

		else if(direction == 8)
		{
			*colPos = *colPos - 4;
			*rowPos = *rowPos + 1;
		}

		else if(direction == 16)
		{
			*colPos = *colPos - 4;
		}

		else if(direction == 32)
		{
			*colPos = *colPos - 4;
			*rowPos = *rowPos - 1;
		}

		else if(direction == 64)
		{
			*rowPos = *rowPos - 1;
		}

		else if(direction == 128)
		{
			*colPos = *colPos + 4;
			*rowPos = *rowPos - 1;
		}
	}

}

/******************************************************************************
/** FUNCTION: checkAndSet
/** ARGUMENTS: Pointers to pixCellNum, pixFlowDir, direction, and outCellNum
/** in addition to colPos and dataType
/** DESCRIPTION: This function checks whether it has reached the outlet cell by 
/** checking whether or not the new position has a non-zero cell number.  If the 
/** position's number is zero, then true is returned and the while loop stops. 
/*****************************************************************************/

bool checkAndSet(Eimg_PixelRect *pixCellNum, Eimg_PixelRect *pixFlowDir, long colPos, int *direction, 
					long *outCellNum, int dataType)
{
	long temporary;

	//Different method for each different data type to get the cell number
	if(dataType==3)
	{
		temporary = pixCellNum->currentData[colPos];
	}
	else if(dataType == 5)
	{
		temporary = ((pixCellNum->currentData[colPos + 1]<<8) | pixCellNum->currentData[colPos]);
	}
	else if(dataType==7)
	{
		temporary = ((pixCellNum->currentData[colPos+3]<<24) | 
						(pixCellNum->currentData[colPos+2]<<16) |
						(pixCellNum->currentData[colPos+1]<< 8) | 
						pixCellNum->currentData[colPos]);
	}


	//If we've moved into a position with zero as the cell number, return true
	//or else keep going until we hit a zero.
	if(temporary == 0)
	{
		return true;
	}
	else if(temporary!=0)
	{
		*outCellNum = temporary;
		*direction = pixFlowDir->currentData[colPos];
		return false;
	}

	return false;
}
	
/*
** Delete and set done flag
*/ 
extern Estr_StringList *
okFunc(
	Eeml_Menu menu,
	Emsc_Opaque *context,
	long argc,
	char **argv,
	Eerr_ErrorReport **outerr
)
{
	EERR_INIT("okFunc", outerr, lclerr);

	Eui_Root *root = (Eui_Root *)context;

	root->doneFlag = EMSC_TRUE;

	return NULL;
}