/*
** Doug Scheidt
** US Geological Survey
** July 2001
** Last Updated April 2002
*/
#include "ErdSetup.h"
#include <process.h>
#include <string.h>

void floatToBinary(float arg); // Function located at bottom of this file.
void intToBinary16(int arg);     // Function after that.
void intToBinary32(int arg);     // Ditto.
int array[4]={0};                // Used in above functions, numberCells and
                                 //   imageCreation function.

/*****************************************************************************
** FUNCTION NAME
** dismissFunc
**    Stops the executable from running. 
** 
** SYNTAX
**  extern Estr_StringList *
**     dismissFunc( menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
** This function will get the displayhandle of the viewer (if present) and the 
** layerhandle of the image. With this information it will obtain the 
** viewer number and image name and return it to the EML.
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**       
**
******************************************************************************/
extern Estr_StringList *dismissFunc( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	AppContext *app = (AppContext *)context;
	EERR_INIT( "dismissfunc", err, lcl );

	app->root->doneFlag = EMSC_TRUE;
	
cleanup:

	if(*err)
	{
	}

	return (NULL);
}



/*****************************************************************************
** FUNCTION NAME
** numberCells
**    Number the cells of a watershed cutout
** 
** SYNTAX
**  extern Estr_StringList *
**     numberCells(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *numberCells( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	Estr_StringList						*listname=NULL;
	Estr_StringList						*newlist=NULL;
	Eimg_Layer							*layer=NULL;
	Eimg_Layer							*newlayer=NULL;
	Eimg_PixelRect						*pixelrect=NULL;
	Eimg_PixelRect						*newpixelrect=NULL;
	Eprj_MapInfo						*mapinfo=NULL;
	Eprj_ProParameters					*projinfo=NULL;
	Emet_MeterInfo						*meter = (Emet_MeterInfo*)NULL;
	char								*newname=NULL;
	char								pathname[101]="",filename[31]="";
	unsigned char						sign,exponent;
	int									i,j,rows,k;
	long int							outnum=0,counter=0;
	float								fullmantissa,s,e,f;
	float								mantissa[3]={0};
	int temp;

	
	EERR_INIT( "numbercells", err, lcl );

	// Create and set a meter to show percentage done to the user.
	meter = emet_MeterInfoCreate("Reading Cell Data...", 
		"Parameter 1 Creation",stdout, &lcl);
	EERR_CHECK( lcl, EFS0("Error creating the meter."));

	emet_MeterInfoSet( meter, 0.0, 100.0, &lcl);
	EERR_CHECK( lcl, EFS0("Error setting the meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// For DEM cutout file.
	listname = eimg_LayerGetNames(argv[0], eint_GetInit(),&lcl);
	EERR_CHECK(lcl, EFS0("Error getting name"));
	
	// Open a Layer type (provides the correct structure)
	layer = eimg_LayerOpen(listname->strings[0], eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layerstack"));

	// Functions to get path and filename needed for layer creation.
	estr_GetPathPart(argv[1],pathname,100);

	estr_GetNamePart(argv[1],filename,30);
	
	// Set up the name for the layer needed to create the layer.
	newname = eimg_LayerNameCreate(pathname,filename,"Layer_1",&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack")); 

	// Create a buffer that will contain one full row at a time 
	pixelrect = eimg_PixelRectCreate(layer->width, 1,
		layer->pixelType, &lcl);	
	EERR_CHECK(lcl, EFS0("Error creating pixel rect."));
	
	// Read one full row of data for the layer.
	eimg_LayerRead(layer, 0, 0, layer->width, 1,
		pixelrect, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

	// Create a structure to hold map information
	mapinfo = eprj_MapInfoCreate( &lcl);
	EERR_CHECK(lcl, EFS0("Error Creating MapInfo Structure."));

	// Read the map information of the layer.
	eimg_MapInfoRead(layer, mapinfo, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading MapInfo."));

	// Same with projection information
	projinfo = eprj_ProParametersCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Map Projection Structure."));

	eimg_ProParametersRead(layer,projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading ProParameters."));

	// Name of the output file is function's return value.
	rtnVal = estr_StringListAppend(rtnVal,argv[1],&lcl);
	EERR_CHECK(lcl, EFS0("Error creating StringList"));

	// Time to determine the number of cells we have in order
	// to know what the pixelType of the output file (P1) should be.
	if(layer->pixelType==3 )  // unsigned 8 bit integer 
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer."));

			for( i=0; i<layer->width; i++)
			{	
				if(pixelrect->currentData[i] > 0)
					++counter;
			}			

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}	
	}
	else if(layer->pixelType==5 )  // unsigned 16 bit integer
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer."));

			for( i=0; i<(layer->width)*2; i=i+2)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum = ((pixelrect->currentData[i+1]<<8) | 
					pixelrect->currentData[i]);

				if(outnum > 0)
					++counter;			
			}
				
			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		}
	}
	else if(layer->pixelType==7 )  // unsigned 32 bit integer
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum= ((pixelrect->currentData[i+3]<<24) |
					(pixelrect->currentData[i+2]<<16) |
					(pixelrect->currentData[i+1]<<8) |
					pixelrect->currentData[i]);

				if(outnum>0)
					++counter;							
			}

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		}
	}
	else if(layer->pixelType==9)   // 32 bit float
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{
				/* Base equation for a binary 4 byte real number:
				 *		x = (-1)^S * m * 2^(E-B)  
				 * Where S is the sign of the number, 
				 *		 m is what's behind the decimal point, and
				 *		 E is the exponent
				 *		 B is the offset (127 for 4 byte reals)
				 *
				 * In binary format:
				 *		SEEEEEEE Emmmmmmm mmmmmmmm mmmmmmmm
				 *
				 * With this format use the equation:
				 *		x = -1^S * 1.m * 2^(E-B)
				 ***												  ***
				 *** IMPORTANT: Imagine data is stored like this:     ***
				 ***	mmmmmmmm mmmmmmmm Emmmmmmm SEEEEEEE			  ***
				 ***												  ***
				 *
				 * Use same equation, but careful how you get the data.
				 *
				 */

				// Special case for a value of zero (won't catch it otherwise)
				if(pixelrect->currentData[i]==0
					&& pixelrect->currentData[i+1]==0
					&& pixelrect->currentData[i+2]==0
					&& pixelrect->currentData[i+3]==0 )
				{
				}
				else
				{
					// Bit shift everything off but the sign bit.
					// This becomes S (bit 1) in the above equation
					sign = (pixelrect->currentData[i+3])>>7;  

					// Bit shift off the sign bit and bitwise "or" with the
					//    second byte everything but the exponent bit.
					// This will be E (bits 2-9) in the above equation.
					exponent = (pixelrect->currentData[i+3]<<1) 
								| (pixelrect->currentData[i+2]>>7);

					// All of these together will be m in the above equation.
					mantissa[0] = 0;	// bits 10-16
					mantissa[1] = 0;	// bits 17-24
					mantissa[2] = 0;	// bits 25-32

					// Bit shift off numbers to the left, then to the right, then see if it exists.
					//    If so, convert it to decimal and add to mantissa.
					for(j=7;j>0;j--)
					{ 
						if( ((((pixelrect->currentData[i+2])<<(8-j))>>7) & 1) == 1) 
							mantissa[0]+= (1/(pow(2,9-j))); 
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i+1])<<(8-j))>>7) & 1) == 1) 
							mantissa[1]+= (1/(pow(2,17-j)));
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i])<<(8-j))>>7) & 1) == 1) 
							mantissa[2]+= (1/(pow(2,25-j)));
					}

					fullmantissa = 2*(mantissa[0]+mantissa[1]+mantissa[2]);		// m
				
					s = pow(-1,sign);
					e = pow(2,exponent-127);									// B = 127
					f = 1+fullmantissa;
					outnum = (int)(s * f * e);

					if(outnum > 0)
						++counter;					

				}   // Number created				
								
			}		// Row created	

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}
	}
	else
	{
		// Error: Cannot handle anything other than unsigned 8,16, and 32 bit 
		//        integers or 32 bit floats.
		*err = eerr_CreateErrorReport("numberCells", (Eerr_ErrorCode)3L,
			EERR_FATALERROR,lcl,"Error. Data type not supported.",NULL);
		eerr_PrintErrorReport(*err,EERR_ENDUSER,eint_GetInit());
	}

	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Create the layer based on number of watershed cells.
	if(counter<255)  // 8-bit
	{
		newlayer = eimg_LayerCreate(newname,1,3,layer->width,
			layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_STATS_SKIP_FACTORS,1,1,
			EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
			EIMG_LAYER_OPTION_END);
		EERR_CHECK(lcl, EFS0("Error creating layer stack"));
	}
	else if(counter<65535)  // 16-bit
	{
		newlayer = eimg_LayerCreate(newname,1,5,layer->width,
			layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_STATS_SKIP_FACTORS,1,1,
			EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
			EIMG_LAYER_OPTION_END);
		EERR_CHECK(lcl, EFS0("Error creating layer stack"));
	}
	else if(counter<4294967295)  // 32-bit
	{
		newlayer = eimg_LayerCreate(newname,1,7,layer->width,
			layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_STATS_SKIP_FACTORS,1,1,
			EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
			EIMG_LAYER_OPTION_END);
		EERR_CHECK(lcl, EFS0("Error creating layer stack"));
	}
	else
	{
		// Error: Cannot handle numbers higher than 32 bit.
		*err = eerr_CreateErrorReport("numberCells", (Eerr_ErrorCode)3L,
			EERR_FATALERROR,lcl,"Error. Data type not supported.",NULL);
		eerr_PrintErrorReport(*err,EERR_ENDUSER,eint_GetInit());
	}
		
	emet_MeterInfoChangeTaskName(meter,
		"Numbering Cells...", &lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Create a buffer that will contain one full row at a time 
	newpixelrect = eimg_PixelRectCreate(newlayer->width, 1,
		newlayer->pixelType, &lcl);	
	EERR_CHECK(lcl, EFS0("Error creating pixel rect."));

	counter=0;


	if(layer->pixelType==3 )  // * INPUT FILE *   Unsigned 8-bit int
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer."));

			for( i=0; i<layer->width; i++)  // For each cell.
			{	
				if(newlayer->pixelType==3)    // * OUTPUT FILE *
				{					
					if(pixelrect->currentData[i] > 0)
					{
						++counter;
						newpixelrect->currentData[i]=counter;
					}
					else
						newpixelrect->currentData[i]=0;				
				}
				else if(newlayer->pixelType==5)  
				{
					if(pixelrect->currentData[i]>0)
					{
						++counter;
						intToBinary16(counter);
						newpixelrect->currentData[i*2] = array[1];
						newpixelrect->currentData[(i*2)+1] = array[0];
					}
					else
					{
						newpixelrect->currentData[i*2] = 0;
						newpixelrect->currentData[(i*2)+1] = 0;
					}
				}
				else if(newlayer->pixelType==7)  
				{
					if(pixelrect->currentData>0)
					{
						++counter;
						intToBinary32(counter);
						newpixelrect->currentData[i*4]   = array[3];
						newpixelrect->currentData[(i*4)+1] = array[2];
						newpixelrect->currentData[(i*4)+2] = array[1];
						newpixelrect->currentData[(i*4)+3] = array[0];
					}
					else
					{
						newpixelrect->currentData[i*4]   = 0;
						newpixelrect->currentData[(i*4)+1] = 0;
						newpixelrect->currentData[(i*4)+2] = 0;
						newpixelrect->currentData[(i*4)+3] = 0;
					}
				}				
			}

			// Write the row stored in the pixelrect to the layer.
			eimg_LayerWrite(newlayer,0,rows,newlayer->width,1,newpixelrect,&lcl);
			EERR_CHECK(lcl,EFS0("Error writing newlayer"));

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((i*100)/newlayer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}	
	}
	else if(layer->pixelType==5 )  // * INPUT FILE *  unsigned 16 bit int
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer."));

			for( i=0; i<(layer->width)*2; i=i+2)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum = ((pixelrect->currentData[i+1]<<8) | 
					pixelrect->currentData[i]);

				if(newlayer->pixelType==3)    // * OUTPUT FILE *
				{
					if(outnum>0)
					{
						++counter;
						newpixelrect->currentData[i/2]=counter;
					}					
					else
						newpixelrect->currentData[i/2]=0;
				}
				else if(newlayer->pixelType==5)
				{
					/* If outnum >0 */
					/* Number to binary, fill pixel rect */
					if(outnum>0)
					{
						++counter;
						intToBinary16(counter);
						newpixelrect->currentData[i] = array[1];
						newpixelrect->currentData[i+1] = array[0];
					}
					else
					{
						newpixelrect->currentData[i] = 0;
						newpixelrect->currentData[i+1] = 0;
					}								
				}
				else if(newlayer->pixelType==7)
				{
					if(outnum>0)
					{
						++counter;
						intToBinary32(counter);
						newpixelrect->currentData[i*2]   = array[3];
						newpixelrect->currentData[(i*2)+1] = array[2];
						newpixelrect->currentData[(i*2)+2] = array[1];
						newpixelrect->currentData[(i*2)+3] = array[0];
					}
					else
					{
						newpixelrect->currentData[i*2]   = 0;
						newpixelrect->currentData[(i*2)+1] = 0;
						newpixelrect->currentData[(i*2)+2] = 0;
						newpixelrect->currentData[(i*2)+3] = 0;
					}
				}
			}

			// Write the row stored in the pixelrect to the layer.
			eimg_LayerWrite(newlayer,0,rows,newlayer->width,1,newpixelrect,&lcl);
			EERR_CHECK(lcl,EFS0("Error writing newlayer"));

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		}
	}
	else if(layer->pixelType==7 )  // * INPUT FILE *    unsigned 32 bit integer
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum= ((pixelrect->currentData[i+3]<<24) |
					(pixelrect->currentData[i+2]<<16) |
					(pixelrect->currentData[i+1]<<8) |
					pixelrect->currentData[i]);

				if(newlayer->pixelType==3)   // * OUTPUT FILE *
				{
					if(outnum>0)
					{
						++counter;
						newpixelrect->currentData[i/4]=counter;
					}
					else
						newpixelrect->currentData[i/4]=0;
				}
				else if(newlayer->pixelType==5)
				{
					if(outnum>0)
					{
						++counter;
						intToBinary16(counter);
						newpixelrect->currentData[i/2] = array[1];
						newpixelrect->currentData[(i/2)+1] = array[0];
					}
					else
					{
						newpixelrect->currentData[i/2] = 0;
						newpixelrect->currentData[(i/2)+1] = 0;
					}
				}
				else if(newlayer->pixelType==7)
				{
					/* If outnum >0 */
					/* Number to binary, fill pixel rect */
					if(outnum>0)
					{
						++counter;
						intToBinary32(counter);
						newpixelrect->currentData[i]   = array[3];
						newpixelrect->currentData[i+1] = array[2];
						newpixelrect->currentData[i+2] = array[1];
						newpixelrect->currentData[i+3] = array[0];
					}
					else
					{
						newpixelrect->currentData[i]   = 0;
						newpixelrect->currentData[i+1] = 0;
						newpixelrect->currentData[i+2] = 0;
						newpixelrect->currentData[i+3] = 0;
					}								
				}
			}
			
			// Write the row stored in the pixelrect to the layer.
			eimg_LayerWrite(newlayer,0,rows,newlayer->width,1,newpixelrect,&lcl);
			EERR_CHECK(lcl,EFS0("Error writing newlayer"));

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
			
		}
	}
	else if(layer->pixelType==9)   //  * INPUT FILE *     32 bit float
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{
				/* Base equation for a binary 4 byte real number:
				 *		x = (-1)^S * m * 2^(E-B)  
				 * Where S is the sign of the number, 
				 *		 m is what's behind the decimal point, and
				 *		 E is the exponent
				 *		 B is the offset (127 for 4 byte reals)
				 *
				 * In binary format:
				 *		SEEEEEEE Emmmmmmm mmmmmmmm mmmmmmmm
				 *
				 * With this format use the equation:
				 *		x = -1^S * 1.m * 2^(E-B)
				 ***												  ***
				 *** IMPORTANT: Imagine data is stored like this:     ***
				 ***	mmmmmmmm mmmmmmmm Emmmmmmm SEEEEEEE			  ***
				 ***												  ***
				 *
				 * Use same equation, but careful how you get the data.
				 *
				 */

				// Special case for a value of zero (won't catch it otherwise)
				if(pixelrect->currentData[i]==0
					&& pixelrect->currentData[i+1]==0
					&& pixelrect->currentData[i+2]==0
					&& pixelrect->currentData[i+3]==0 )
				{
					if(newlayer->pixelType==3)
					{
						newpixelrect->currentData[i/4]=0;
					}
					else if(newlayer->pixelType==5)
					{
						newpixelrect->currentData[i/2]=0;
						newpixelrect->currentData[(i/2)+1]=0;
					}
					else if(newlayer->pixelType==7)
					{
						newpixelrect->currentData[i]   =0;
						newpixelrect->currentData[i+1] =0;
						newpixelrect->currentData[i+2] =0;
						newpixelrect->currentData[i+3] =0;
					}
				}
				else
				{
					// Bit shift everything off but the sign bit.
					// This becomes S (bit 1) in the above equation
					sign = (pixelrect->currentData[i+3])>>7;  

					// Bit shift off the sign bit and bitwise "or" with the
					//    second byte everything but the exponent bit.
					// This will be E (bits 2-9) in the above equation.
					exponent = (pixelrect->currentData[i+3]<<1) 
								| (pixelrect->currentData[i+2]>>7);

					// All of these together will be m in the above equation.
					mantissa[0] = 0;	// bits 10-16
					mantissa[1] = 0;	// bits 17-24
					mantissa[2] = 0;	// bits 25-32

					// Bit shift off numbers to the left, then to the right, then see if it exists.
					//    If so, convert it to decimal and add to mantissa.
					for(j=7;j>0;j--)
					{ 
						if( ((((pixelrect->currentData[i+2])<<(8-j))>>7) & 1) == 1) 
							mantissa[0]+= (1/(pow(2,9-j))); 
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i+1])<<(8-j))>>7) & 1) == 1) 
							mantissa[1]+= (1/(pow(2,17-j)));
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i])<<(8-j))>>7) & 1) == 1) 
							mantissa[2]+= (1/(pow(2,25-j)));
					}

					fullmantissa = 2*(mantissa[0]+mantissa[1]+mantissa[2]);		// m
				
					s = pow(-1,sign);
					e = pow(2,exponent-127);									// B = 127
					f = 1+fullmantissa;
					outnum = (int)(s * f * e);

					if(newlayer->pixelType==3)    // * OUTPUT FILE *
					{
						if(outnum>0)
						{
							++counter;
							newpixelrect->currentData[i/4]=counter;
						}
						else
							newpixelrect->currentData[i/4]=0;
					}
					else if(newlayer->pixelType==5)
					{
						if(outnum>0)
						{
							++counter;
							intToBinary16(counter);
							newpixelrect->currentData[i/2] = array[1];
							newpixelrect->currentData[(i/2)+1] = array[0];
						}
						else
						{
							newpixelrect->currentData[i/2] = 0;
							newpixelrect->currentData[(i/2)+1] = 0;
						}
					}
					else if(newlayer->pixelType==7)
					{
					
						/* If outnum >0 */
						/* Number to binary, fill pixel rect */
						if(outnum>0)
						{
							++counter;
							intToBinary32(counter);
							newpixelrect->currentData[i]   = array[3];
							newpixelrect->currentData[i+1] = array[2];
							newpixelrect->currentData[i+2] = array[1];
							newpixelrect->currentData[i+3] = array[0];
						}
						else
						{
							newpixelrect->currentData[i]   = 0;
							newpixelrect->currentData[i+1] = 0;
							newpixelrect->currentData[i+2] = 0;
							newpixelrect->currentData[i+3] = 0;
						}								
					}							

				}   // Number created				
								
			}		// Row created
			
			// Write the row stored in the pixelrect to the layer.
			eimg_LayerWrite(newlayer,0,rows,newlayer->width,1,newpixelrect,&lcl);
			EERR_CHECK(lcl,EFS0("Error writing newlayer"));

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}
	}

	// Put same info from dem image to the new image.
	// Stats will be computed when layer is closed.
	eimg_MapInfoWrite(newlayer,mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(newlayer,projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing projection info"));

	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));


cleanup:

	newname = NULL;

	emet_MeterInfoDelete(meter, &lcl);
	meter = NULL;

	eprj_MapInfoFree( &mapinfo);
	mapinfo = NULL;

	eprj_ProParametersFree(&projinfo);
	projinfo=NULL;

	eimg_PixelRectDelete(pixelrect, &lcl);
	pixelrect = NULL;

	eimg_PixelRectDelete(newpixelrect, &lcl);
	newpixelrect = NULL;

	eimg_LayerClose(layer,&lcl);
	layer = NULL;

	eimg_LayerClose(newlayer,&lcl);
	newlayer = NULL;
	
	estr_StringListDelete(listname,&lcl);
	listname = NULL;

	estr_StringListDelete(newlist,&lcl);
	newlist = NULL;

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** saveHeaderFlags
**    Saves AGNPS header flags and variables
** 
** SYNTAX
**  extern Estr_StringList *
**     saveHeaderFlags(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
** This function stores AGNPS header flags and variables in Struct hflags
** hflags is accessed by the formatdata
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**
**
******************************************************************************/

extern Estr_StringList *saveHeaderFlags( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList		*rtnVal=NULL;

	EERR_INIT ("saveheaderflags", err, lcl);

	//Function to save AGNPS Header Flags (Lines 2, 5, and 6)

	//Line 2 
	hflags.errorLog = atoi(argv[0]);
	hflags.srcAcct = atoi(argv[1]);
	hflags.sediment = atoi(argv[2]);
	hflags.hydrology = atoi(argv[3]);
	hflags.nutrient = atoi(argv[4]);
	hflags.pesticide = atoi(argv[5]);

	//Line 5
	hflags.hyd_calc = atoi(argv[6]);
	hflags.geomorph = atoi(argv[7]);
	hflags.k_coeff = atoi(argv[8]);
	hflags.prepeak = atof(argv[9]);

	//Line 6
	//hflags.storm_type = "abc"; //argv[10];
	strcpy(hflags.storm_type, argv[10]);
	hflags.energy_int = atof(argv[11]);
	hflags.duration = atof(argv[12]);
	hflags.precip = atof(argv[13]);
	hflags.nitro_conc = atof(argv[14]);

cleanup:

	return rtnVal;
}


/*****************************************************************************
** FUNCTION NAME
** formatData
**    Formats the data for input into AGNPS version 5.0.
** 
** SYNTAX
**  extern Estr_StringList *
**     formatData(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
** This function grabs the stacked image of AGNPS parameters and creates
** the data file for AGNPS input.
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**       char viewid;
**       char filename;
**
**
******************************************************************************/


extern Estr_StringList *formatData( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	AppContext *app = (AppContext *)context;
	Estr_StringList						*listnames=NULL;
	Eimg_LayerStack						*layerstack=NULL;
	Eimg_PixelRectStack					*pixelrect=NULL;
	Eprj_MapInfo						*mapinfo=NULL;
	Esta_Statistics						*stats=NULL;
	Emet_MeterInfo						*meter = (Emet_MeterInfo*)NULL;
	float								**number=NULL;
	int									*validcell=NULL;
	char								*outfile=NULL;
	float								fullmantissa;
	unsigned char						sign,exponent;
	float								s,e,f;
	float								mantissa[3]={0};
	int									i,j,n,rows,current;
	int 								arraylist[23],secondary[23];
	int									zero = 0, exitloop=0,k=0;
	FILE								*fptr;


	//**** Header Variables ****//
	//Subject to change depending on info from Dave Bosch.
	//Don't remember him saying anything about these however.
	//May have to ask him about this if he sends other info
	//  without sending header information.
	char *watershed_name=NULL;
	int num_cells, total_cells, hyd_calc=1, geomorph=0, k_coeff=1;
	double prepeak=366.00, energy_int=160.00, duration=0.0;
	double area;
	double precip=7.30, nitro_conc=1.10;
	char *storm_type="3",outletcell;
				
	//****  Soil Info Variables   ****//
	// Calculate based on lookup table and soil type.
	double soil_N, soil_P, pore_N, pore_P;
	double N_runoff, P_runoff, N_leech, P_leech;
	int org_matter;

	//****  Fertilizer Variables  ****//
	// Calculate based on lookup table and fertilizer level.
	int N_app, P_app, N_avail, P_avail;

	//**** Channel Info Variables ****//
	// Taken from sample data from AGNPS program as per Dave Bosch's suggestion.
	double width=0,wid_coef=3.425,wid_exp=0.3151,depth=0,dep_coef=0.4537,dep_exp=0.2192;
	double length=0,len_coef=153,len_exp=0.6,chSlope=0.1,sideSlope=1.0;
	double chManning=0.1;
	int decay_indic=1,nitro_decay=0,phospho_decay=0,CODecay=0;
	int clay=1,silt=1,small_agg=1,large_agg=1,sand=1;


	EERR_INIT( "formatdata", err, lcl );

	// Create and set a meter to show percentage done to the user.
	meter = emet_MeterInfoCreate("Starting process", 
		"Convert Image Stack",stdout, &lcl);
	EERR_CHECK( lcl, EFS0("Error creating the meter."));

	emet_MeterInfoSet( meter, 0.0, 100.0, &lcl);
	EERR_CHECK( lcl, EFS0("Error setting the meter info!"));

	emet_MeterInfoChangeTaskName(meter,
		"Converting image stack to AGNPS data format...", &lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Get the names of all the layers in the image.
	listnames = eimg_LayerGetNames(argv[0], eint_GetInit(), &lcl);
	EERR_CHECK(lcl, EFS0("Error getting layer names"));

	// Open a LayerStack type (provides the correct structure)
	layerstack = eimg_LayerStackOpen(listnames, eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layerstack"));

	// Create a buffer that will contain one full row at a time 
	pixelrect = eimg_PixelRectStackCreate(layerstack->count,
		layerstack->layers[0]->width, 1,
		layerstack->layers[0]->pixelType, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect."));

	// Read one full row of data for each layer.
	eimg_LayerStackRead(layerstack, 0, 0, layerstack->layers[0]->width, 1,
		pixelrect, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

	// Create a structure to hold map information
	mapinfo = eprj_MapInfoCreate( &lcl);
	EERR_CHECK(lcl, EFS0("Error Creating MapInfo Structure."));

	// Read the map information of the top layer.
	eimg_MapInfoRead(layerstack->layers[0],mapinfo, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading MapInfo."));

	// Create a structure to hold map statistics.
	stats = esta_StatisticsCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Statistics Structure."));

	// Read the map statistics of the top layer
	eimg_StatisticsRead(layerstack->layers[0],stats,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Statistics"));

	//Convert from square meters to acres
	area = (mapinfo->pixelSize->width)*(mapinfo->pixelSize->height)*0.000247;

	num_cells = stats->maximum;
	total_cells = num_cells;

	// Allocate memory to accomodate 22 parameters for the width of the layer.
	number = (float **)malloc( 22 * sizeof(float *));
	for(i=0;i<22;i++)
	{
		number[i]=(float *) malloc( layerstack->layers[0]->width * sizeof(float));
	}

	// Allocate memory for an array to determine whether a cell is in the 
	//    watershed or not.
	validcell = calloc(layerstack->layers[0]->width,sizeof(int));

	// Arraylist will hold which parameters the user included.
	for(i=1; i<23; i++)
	{
		arraylist[i] = atoi(argv[i]);
	}

	// Output file (.dat)
	outfile = argv[23];
	outletcell = atoi(argv[24]);
	
	fptr = fopen(outfile,"w");  // Write only

	/******** FILE HEADER ********/
	fprintf(fptr, "AGNPS Version 5.0 format\n");

	//for(i=0; i<6; i++)
	//fprintf(fptr, "%8i",atoi(argv[i+25])); //header flags (Line 2)

	/* Header Flags (Line 2) */
	fprintf(fptr, "%8i", hflags.errorLog);
	fprintf(fptr, "%8i", hflags.srcAcct);
	fprintf(fptr, "%8i", hflags.hydrology);
	fprintf(fptr, "%8i", hflags.sediment);
	fprintf(fptr, "%8i", hflags.nutrient);
	fprintf(fptr, "%8i", hflags.pesticide);

	for(i=0; i<2; i++)
		fprintf(fptr, "%8i", zero);
	fprintf(fptr, "\n");
	fprintf(fptr, "Watershed data\n");
	fprintf(fptr, "%i meters\n",(int)mapinfo->pixelSize->width);


	/* Header Flags (Line 5) */
	hyd_calc = hflags.hyd_calc;
	geomorph = hflags.geomorph;
	k_coeff = hflags.k_coeff;
	prepeak = hflags.prepeak;

	/* Header Flags (Line 6) */
	storm_type = hflags.storm_type;
	energy_int = hflags.energy_int;
	duration = hflags.duration;
	precip = hflags.precip;
	nitro_conc = hflags.nitro_conc;

	/******** WATERSHED INFORMATION ***********/
	fprintf(fptr, "%16.2f%8i%8i",area,num_cells,total_cells);
	fprintf(fptr, "%8i%8i%8i%8.2f",hyd_calc,geomorph,k_coeff,prepeak);
	fprintf(fptr, "\n");
	fprintf(fptr, "%16s%8.2f%8.1f%8.2f%8.2f",storm_type,energy_int,duration,precip,nitro_conc);
	fprintf(fptr, "\n");


	// The Mother of All Loops (Goes through stacked image one row at a time)
	for(rows=0; rows<layerstack->layers[0]->height; rows++)
	{
		// Read in one row of data.
		eimg_LayerStackRead(layerstack, 0, rows, layerstack->layers[0]->width, 1,
		pixelrect, &lcl);
		EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

		// A copy of the arraylist telling which parameters were used.
		// The secondary array gets zeroed out, thus it is recreated every
		//   time.
		for(i=1;i<23;i++)
			secondary[i]=arraylist[i];				
		
		// Loop to get a row of pixel data (for all the layers)
		for( n=0; n<pixelrect->numlayers; n++)
		{
			// Loop to search for next valid cell in the row.
			exitloop=0;
			for( i=1; i<23; i++)
			{
				if(secondary[i]!=0 && exitloop==0)
				{
					current = i-1;
					secondary[i]=0;
					exitloop=1;
				}
			}

			// Loop to create each floating point number from a binary number
			//   for each row.
			// Have to check or grab bits individually by bit shifting
			for( i=0; i<(layerstack->layers[n]->width)*4; i=i+4)
			{
				/* Base equation for a binary 4 byte real number:
				 *		x = (-1)^S * m * 2^(E-B)  
				 * Where S is the sign of the number, 
				 *		 m is what's behind the decimal point, and
				 *		 E is the exponent
				 *		 B is the offset (127 for 4 byte reals)
				 *
				 * In binary format:
				 *		SEEEEEEE Emmmmmmm mmmmmmmm mmmmmmmm
				 *
				 * With this format use the equation:
				 *		x = -1^S * 1.m * 2^(E-B)
				 ***												  ***
				 *** IMPORTANT: Imagine data is stored like this:     ***
				 ***	mmmmmmmm mmmmmmmm Emmmmmmm SEEEEEEE			  ***
				 ***												  ***
				 *
				 * Use same equation, but careful how you get the data.
				 *
				 */

				// Special case for a value of zero (won't catch it otherwise)
				if(pixelrect->datalayer[n]->currentData[i]==0
					&& pixelrect->datalayer[n]->currentData[i+1]==0
					&& pixelrect->datalayer[n]->currentData[i+2]==0
					&& pixelrect->datalayer[n]->currentData[i+3]==0 )
					number[current][i/4] = 0;
				else
				{
					// Bit shift everything off but the sign bit.
					// This becomes S (bit 1) in the above equation
					sign = (pixelrect->datalayer[n]->currentData[i+3])>>7;  

					// Bit shift off the sign bit and bitwise "or" with the
					//    second byte everything but the exponent bit.
					// This will be E (bits 2-9) in the above equation.
					exponent = (pixelrect->datalayer[n]->currentData[i+3]<<1) 
								| (pixelrect->datalayer[n]->currentData[i+2]>>7);

					// All of these together will be m in the above equation.
					mantissa[0] = 0;	// bits 10-16
					mantissa[1] = 0;	// bits 17-24
					mantissa[2] = 0;	// bits 25-32

					// Bit shift off numbers to the left, then to the right, then see if it exists.
					//    If so, convert it to decimal and add to mantissa.
					for(j=7;j>0;j--)
					{ 
						if( ((((pixelrect->datalayer[n]->currentData[i+2])<<(8-j))>>7) & 1) == 1) 
							mantissa[0]+= (1/(pow(2,9-j))); 
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->datalayer[n]->currentData[i+1])<<(8-j))>>7) & 1) == 1) 
							mantissa[1]+= (1/(pow(2,17-j)));
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->datalayer[n]->currentData[i])<<(8-j))>>7) & 1) == 1) 
							mantissa[2]+= (1/(pow(2,25-j)));
					}

					fullmantissa = 2*(mantissa[0]+mantissa[1]+mantissa[2]);		// m
				
					s = pow(-1,sign);
					e = pow(2,exponent-127);									// B = 127
					f = 1+fullmantissa;
					number[current][i/4] = s * f * e;

					if(n==0 && number[n][i/4]>0)  
					{
						validcell[i/4]=1;  // This cell is a watershed cell.						
					}

				}   // Number created
								
			}		// Row created	

		}			// Layer created (But still inside major loop)
		
		/******* Output of parameters ********/

		// Scan the row
		for( i=0; i<(layerstack->layers[0]->width); i=i++)
		{
			if(validcell[i]>0)  // Parameter 1 has a value (It's a watershed cell)
			{
				for(j=1; j<23; j++)  // Go through each parameter
				{
					if(arraylist[j]>0)  // If parameter was used.
					{
						if( j==5 )
						{
							// Hardcoded recode of parameter 5.
							// Arc's flow direction not the same as AGNPS's.
							if(number[j-1][i]==64) number[j-1][i]=1;
							else if(number[j-1][i]==128) number[j-1][i]=2;
							else if(number[j-1][i]==1) number[j-1][i]=3;
							else if(number[j-1][i]==2) number[j-1][i]=4;
							else if(number[j-1][i]==4) number[j-1][i]=5;
							else if(number[j-1][i]==8) number[j-1][i]=6;
							else if(number[j-1][i]==16) number[j-1][i]=7;
							else if(number[j-1][i]==32) number[j-1][i]=8;
						}

						if( j==1 || j==3 )   // Parameters 1 & 3
						{
							// Don't want p3 to be zero, make it equal p1
							if(number[2][i]==0)
							{
								number[2][i] = number[0][i];
							}

							// Force outlet cell to flow out
							if(number[0][i]==outletcell)         // Flows out
							{
								number[2][i] = (float)total_cells+10;
							}
							else if(number[0][i]==number[2][i])  // Border cell
							{
								number[4][i]=0;                  // P5 is zero
							}

							fprintf(fptr,"%8.0f",number[j-1][i]);
						}
						else if( j==6 )         // Parameter 6, SCS Curve
						{
							// Don't want a zero, force a common value
							if(number[j-1][i]==0)
								fprintf(fptr,"%8s","55.00");
							else if(number[15][i]==0) //If it doesn't match up with water.
								fprintf(fptr,"%8s","100.00");
							else
								fprintf(fptr,"%8.2f",number[j-1][i]);
						}
						else if( j==7 )    // Parameter 7, Slope output format
						{
							// Just necessary for formatting purposes.
							fprintf(fptr,"%8.3f",number[j-1][i]);
						}
						else if( j==10 )   // Parameter 10, Mannings
						{
							// If it doesn't match up with water.
							//    or just for formatting.
							if(number[j+5][i]==0)   // Parameter 16, Soil
								fprintf(fptr,"%8s","0.99");
							else
								fprintf(fptr,"%8.3f",number[j-1][i]);
						} 
						else if( j==12 )   // Parameter 12, C-factor
						{
							// More special formatting
							fprintf(fptr,"%8.4f",number[j-1][i]);
						}
						else if( j==16 )   // Parameter 16, Soil
						{
							// If it is water, force others to correspond.
							if(number[j-1][i]==0)
							{
								number[j][i]=0;     // Parameter 17
								number[j+1][i]=0;   // Parameter 18
								number[j+5][i]=0;   // Parameter 22
							}
							else
							{
								if(number[j+5][i]==0)   // Parameter 22
								{
									number[j-1][i]=0;   // 16   
									number[j][i]=0;     // 17
									number[j+1][i]=0;   // 18
								}
							}

							fprintf(fptr,"\n%16.0f",number[j-1][i]);
						}						
						else if( j==9 )                 // Parameter 9 output formatting
							fprintf(fptr,"\n%16.0f",number[j-1][i]);
						else if( j==11 || j==14 )       // Parameter 11 & 14 more formatting
							fprintf(fptr,"%8.2f",number[j-1][i]);
						else                            // All others have this format
							fprintf(fptr,"%8.0f",number[j-1][i]);

						if( j == 22 )
							fprintf(fptr,"\n");
					}
					else      //Layers not given that need hardcoded format or value.
					{
						if( j==2 || j==4 )
						{
							fprintf(fptr,"%8s","000");
						}
						//Hardcoded recode of parameter 13.
						else if( j == 13 )
						{
							fprintf(fptr,"%8s","1.00");
						}
						else
							fprintf(fptr,"%8i",zero);
					}
				}

				//***** Optional Soil Info *****/
				//Use lookup table to set optional soil info
				if(number[15][i]>0 )  //if not water
				{
					if(number[15][i]==1)       //Sand
					{
						soil_N = 0.0010;
						soil_P = 0.0005;
						pore_N = 5.00;
						pore_P = 2.00;
						N_runoff = 0.050;
						P_runoff = 0.025;
						N_leech = 0.250;
						P_leech = 0.250;
						org_matter = 1;
					}
					else if(number[15][i]==2)  //Silt
					{
						soil_N = 0.0010;
						soil_P = 0.0005;
						pore_N = 5.00;
						pore_P = 2.00;
						N_runoff = 0.050;
						P_runoff = 0.025;
						N_leech = 0.250;
						P_leech = 0.250;
						org_matter = 2;
					}
					else if(number[15][i]==3)  //Clay
					{
						soil_N = 0.0010;
						soil_P = 0.0005;
						pore_N = 5.00;
						pore_P = 2.00;
						N_runoff = 0.050;
						P_runoff = 0.025;
						N_leech = 0.250;
						P_leech = 0.250;
						org_matter = 5;
					}
					else if(number[15][i]==4)  //Peat
					{
						soil_N = 0.0010;
						soil_P = 0.0005;
						pore_N = 5.00;
						pore_P = 2.00;
						N_runoff = 0.050;
						P_runoff = 0.025;
						N_leech = 0.250;
						P_leech = 0.250;
						org_matter = 20;
					}

					fprintf(fptr,"Soil:%11.4f%8.4f%8.2f%8.2f\n",
						soil_N,soil_P,pore_N,pore_P);
					fprintf(fptr,"%16.3f%8.3f%8.3f%8.3f%8i\n",
						N_runoff,P_runoff,N_leech,P_leech,org_matter);
				}
				/***** End Optional Soil Info *****/

				/***** Optional Fertilizer Information *****/
				if(number[16][i]>0)   //if there is application
				{
					if(number[16][i]==1)        //Low
					{
						N_app = 50;
						P_app = 20;
						N_avail = 60;
						P_avail = 60;
					}
					else if(number[16][i]==2)   //Average
					{
						N_app = 100;
						P_app = 40;
						N_avail = 60;
						P_avail = 60;
					}
					else if(number[16][i]==3)   //High
					{
						N_app = 200;
						P_app = 80;
						N_avail = 60;
						P_avail = 60;
					}
					else if(number[16][i]==4)   //User supplied amounts
					{
					//No idea what they want. Set to average level.
						N_app = 100;
						P_app = 40;
						N_avail = 60;
						P_avail = 60;
					}
				
					fprintf(fptr,"Fert:%11i%8i%8i%8i\n",
						N_app,P_app,N_avail,P_avail);
				}
				/***** End Optional Fertilizer Information *****/

				/***** Optional Channel Information ****/
				// No idea yet on what to do with it!!!
				// In AGNPS 5.0 Input File Format Guide, it says:
				// "The channel information will exist for every cell
				//   in the watershed!"
				//if(number[21][i]>0)
				//{
				fprintf(fptr,"Channel:%8.2f%8.4f%8.4f%8.2f%8.4f%8.4f\n",
					width,wid_coef,wid_exp,depth,dep_coef,dep_exp);
				fprintf(fptr,"%16.2f%8.3f%8.4f%8.2f%8.2f\n",
					length,len_coef,len_exp,chSlope,sideSlope);
				fprintf(fptr,"%16.3f%8i%8i%8i%8i\n",
					chManning,decay_indic,nitro_decay,phospho_decay,CODecay);
				fprintf(fptr,"%16i%8i%8i%8i%8i\n",
					clay,silt,small_agg,large_agg,sand);
				//}

			}  // Done processing one cell.

		}  // Done processing one row.

		// Reset valid cells for next run of major loop.
		for(k=0;k<layerstack->layers[0]->width;k++)
			validcell[k]=0;
		
		// Keep track of progress.
		emet_MeterInfoPrint(meter, (double)((rows*100)/layerstack->layers[0]->height), &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

    }	   // Done with big loop
	
	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

// Free up all memory and crap.
cleanup:

	fclose(fptr);
	outfile=NULL;

	for(i=0;i<22 ;i++)
		free(number[i]);
	free(number);
	number = NULL;

	emet_MeterInfoDelete(meter, &lcl);
	meter = NULL;

	free(validcell);
	validcell = NULL;

	estr_StringListDelete(listnames,&lcl);
	listnames = NULL;

	eprj_MapInfoFree( &mapinfo);
	mapinfo = NULL;

	esta_StatisticsDelete( stats, &lcl);
	stats = NULL;

	eimg_PixelRectStackDelete(pixelrect, &lcl);
	pixelrect = NULL;

	eimg_LayerStackClose(layerstack,&lcl);
	layerstack = NULL;	

	return listnames;
}



/*****************************************************************************
** FUNCTION NAME
** runAgnps
**    runs the Agnps executable
** 
** SYNTAX
**  extern Estr_StringList *
**     runAgnps(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *runAgnps( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	long value[1]={0};

	EERR_INIT( "runagnps", err, lcl );
	
	value[0] = _spawnv(_P_WAIT, argv[0], argv); 

	rtnVal = estr_LongToStringList(1,value,&lcl);
	EERR_CHECK(lcl,EFS0("Can't convert long to string"));

cleanup:

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** runFlood
**    runs the Flood executable
** 
** SYNTAX
**  extern Estr_StringList *
**     runFlood(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *runFlood( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	long value[1]={0};

	EERR_INIT( "runflood", err, lcl );
	
	value[0] = _spawnv(_P_WAIT, argv[0], argv); 

	rtnVal = estr_LongToStringList(1,value,&lcl);
	EERR_CHECK(lcl,EFS0("Can't convert long to string"));

cleanup:

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** runD8
**    runs the d8 executable
** 
** SYNTAX
**  extern Estr_StringList *
**     runD8(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *runD8( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	long value[1]={0};

	EERR_INIT( "rund8", err, lcl );
	
	value[0] = _spawnv(_P_WAIT, argv[0], argv); 

	rtnVal = estr_LongToStringList(1,value,&lcl);
	EERR_CHECK(lcl,EFS0("Can't convert long to string"));

cleanup:

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** runGridnet
**    runs the Gridnet executable
** 
** SYNTAX
**  extern Estr_StringList *
**     runGridnet(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *runGridnet( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	long value[1]={0};

	EERR_INIT( "rungridnet", err, lcl );
	
	value[0] = _spawnv(_P_WAIT, argv[0], argv); 

	rtnVal = estr_LongToStringList(1,value,&lcl);
	EERR_CHECK(lcl,EFS0("Can't convert long to string"));

cleanup:

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** imageCreation
**    create images from agnps output
** 
** SYNTAX
**  extern Estr_StringList *
**     imageCreation(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  Uses the output from AGNPS (.nps) to make 9 image files displaying the
**  results of the file on the watershed.
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *imageCreation( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList			*rtnVal=NULL;
	Estr_StringList			*name=NULL;
	Eimg_Layer				*layer=NULL;
	Eimg_LayerNameStack		*allnames=NULL;
	Eimg_PixelRect			*p1pixelrect=NULL;
	Eimg_PixelRectStack		*stack1=NULL;
	Eimg_PixelRectStack		*stack2=NULL;
	Eimg_PixelRectStack		*stack3=NULL;
	Eimg_PixelRectStack		*stack4=NULL;
	Eimg_PixelRectStack		*stack5=NULL;
	Eimg_PixelRectStack		*stack6=NULL;
	Eimg_PixelRectStack		*stack7=NULL;
	Eimg_PixelRectStack		*stack8=NULL;
	Eimg_PixelRectStack		*stack9=NULL;
	Eimg_LayerStack			*layerstack1=NULL;
	Eimg_LayerStack			*layerstack2=NULL;
	Eimg_LayerStack			*layerstack3=NULL;
	Eimg_LayerStack			*layerstack4=NULL;
	Eimg_LayerStack			*layerstack5=NULL;
	Eimg_LayerStack			*layerstack6=NULL;
	Eimg_LayerStack			*layerstack7=NULL;
	Eimg_LayerStack			*layerstack8=NULL;
	Eimg_LayerStack			*layerstack9=NULL;
	Eprj_MapInfo			*mapinfo=NULL;
	Eprj_ProParameters		*projinfo=NULL;
	Esta_Statistics			*stats=NULL;
	Emet_MeterInfo			*meter = (Emet_MeterInfo*)NULL;
	long					value[1]={0};
	char					*useless="";
	int						i, j, k=0,p=0;
	FILE					*fptr;
		
	
	/*** Initial Watershed Nutrient Data ***/
	float totDrain, baseArea, rainfall, intensity;
	int baseCell, cellDivision;
	float volume, rate, yield;
	float sedNitro, totalNitro, nitroConc, sedPhospho, totalPhospho;
	float phosphoConc, totalCOD, CODConc;

	/*** Sediment Data ***/
	float areaErosion, gullyErosion;
	int delivery, enrichment;
	float sedConc, areaYield, sedYield;

	/*** Soil Loss Data ***/
	//Hydrology Data
	int cellNum;      //Re-use cellDivision variable
	float drainArea, eqRunoff, upRunoff, upFlow;
	float downRunoff, downFlow, runoffAbove;

	//Rest of the 6 data sections for soil loss
	float cellErosion, upSedYield, sedGenerated;  //Re-use sedYield variable
	int deposition;

	/*** Nutrient Data ***/
	//Re-use cellNum and cellDivision
	//Re-use drainArea, sedNitro, totalNitro, nitroConc
	float cellNitro, nitroRunoff;
	//Re-use sedPhospho, totalPhospho, phosphoConc, totalCOD, CODConc;
	float cellPhospho, phosphoRunoff, cellCOD;
	
	/*** Pesticide Data ***/
	//Skip for now.  No pesticide data for Little River Watershed.

	EERR_INIT( "imageCreation", err, lcl );

	// Create and set a meter to show percentage done to the user.
	meter = emet_MeterInfoCreate("Starting process", 
		"Convert NPS to Images",stdout, &lcl);
	EERR_CHECK( lcl, EFS0("Error creating the meter."));

	emet_MeterInfoSet( meter, 0.0, 100.0, &lcl);
	EERR_CHECK( lcl, EFS0("Error setting the meter info!"));

	emet_MeterInfoChangeTaskName(meter,
		"Initializing Layer Stack Images", &lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// For parameter 1 file.
	name = eimg_LayerGetNames(argv[1], eint_GetInit(),&lcl);
	EERR_CHECK(lcl, EFS0("Error getting name"));
	
	// Open a Layer type (provides the correct structure)
	layer = eimg_LayerOpen(name->strings[0], eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layer"));

	// Create a buffer that will contain one full row at a time 
	p1pixelrect = eimg_PixelRectCreate( layer->width, 1,
			layer->pixelType, &lcl);	
	EERR_CHECK(lcl, EFS0("Error creating pixel rect."));

	// Read one full row of data for each layer.
	eimg_LayerRead(layer, 0, 0, layer->width, 1, 
		p1pixelrect, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Layer."));

	// Create structure to hold map information.
	mapinfo = eprj_MapInfoCreate( &lcl);
	EERR_CHECK(lcl, EFS0("Error Creating MapInfo Structure."));

	// Read the map information.
	eimg_MapInfoRead(layer,mapinfo, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading MapInfo."));

	// Same with projection information
	projinfo = eprj_ProParametersCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Map Projection Structure."));

	eimg_ProParametersRead(layer,projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading ProParameters."));

	// And same with map statistics
	stats = esta_StatisticsCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Statistics Structure."));

	eimg_StatisticsRead(layer,stats,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Statistics"));

	// Create 9 pixel rects to hold one row of data
	//   for each layer of each rect.
	stack1 = eimg_PixelRectStackCreate(7,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 1"));

	stack2 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 2"));

	stack3 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 3"));

	stack4 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 4"));

	stack5 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 5"));

	stack6 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 6"));

	stack7 = eimg_PixelRectStackCreate(5,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 7"));

	stack8 = eimg_PixelRectStackCreate(6,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 8"));

	stack9 = eimg_PixelRectStackCreate(8,layer->width,1,
		9, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating pixel rect stack 9"));

	fptr = fopen(argv[0],"r");

	// Need to create 9 layerstacks
	// These are the actual image files that will be the final
	//    product.
	/***********  Begin Layerstack Creation  ********************/

	/********* 1 Hydro **********/
	allnames = eimg_LayerNameStackCreate(7,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack")); 

	// Add the names of the layers to the layernamestack.
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[2],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	

	// Create the layerstack
	layerstack1 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack"));

	// Get rid of it so next layerstack has empty namestack to use.
	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL; 

	emet_MeterInfoPrint(meter, (double)11, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
    
	/********* 2 Clay *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[3],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack2 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)22, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 3 Silt *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[4],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack3 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)33, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 4 SAGG *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[5],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack4 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)44, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 5 LAGG *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[6],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack5 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)55, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 6 Sand *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[7],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack6 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)66, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 7 Total *********/
	allnames = eimg_LayerNameStackCreate(5,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[8],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack7 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)77, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 8 Nitro *********/
	allnames = eimg_LayerNameStackCreate(6,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[9],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack8 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)88, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/********* 9 Phospho *********/
	allnames = eimg_LayerNameStackCreate(8,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack"));

	
	for(i=0;i<allnames->count;i++)
	{
		allnames->strings[i]=estr_Sprintf(NULL,"%s%s%d%s",&lcl,
			argv[10],"(:Layer_",i+1,")");
		EERR_CHECK(lcl,EFS0("Error making names"));
		
	}	
	layerstack9 = eimg_LayerStackCreate(allnames,1,9,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack")); 

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;

	emet_MeterInfoPrint(meter, (double)100, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/*************** End Layer Stack Creation *******************/

	emet_MeterInfoChangeTaskName(meter,"Converting NPS file to Imagine images... (1 of 2)",
		&lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Header Info
	for(i=0; i<4; i++)
	{
		fgets(useless,256,fptr);				
	}
	
	// Initial Watershed Nutrient Data
	fscanf(fptr,"%8f.3",&totDrain);
	fscanf(fptr,"%f.2",&baseArea);
	fscanf(fptr,"%f.2",&rainfall);
	fscanf(fptr,"%f.2",&intensity);
	
	fscanf(fptr,"%i",&baseCell);
	fscanf(fptr,"%i",&cellDivision);

	fscanf(fptr,"%f.2",&volume);
	fscanf(fptr,"%f.2",&rate);
	fscanf(fptr,"%f.2",&yield);

	fscanf(fptr,"%f.2",&sedNitro);
	fscanf(fptr,"%f.2",&totalNitro);
	fscanf(fptr,"%f.2",&nitroConc);
	fscanf(fptr,"%f.2",&sedPhospho);

	fscanf(fptr,"%f.2",&totalPhospho);
	fscanf(fptr,"%f.2",&phosphoConc);
	fscanf(fptr,"%f.2",&totalCOD);
	fscanf(fptr,"%f.2",&CODConc);
	
	// Sediment Data
	fscanf(fptr,"%s",useless);
	fscanf(fptr,"%s",useless);
	for(i=0;i<6;i++)
	{
		fscanf(fptr,"%f.2",&areaErosion);
		fscanf(fptr,"%f.2",&gullyErosion);
		fscanf(fptr,"%i",&delivery);
		fscanf(fptr,"%i",&enrichment);
		fscanf(fptr,"%f.2",&sedConc);
		fscanf(fptr,"%f.2",&areaYield);
		fscanf(fptr,"%f.2",&sedYield);
	}

	//Bring in Soil Loss Data
	fscanf(fptr,"%s",useless);
	fscanf(fptr,"%s",useless);

	for(i=0; i<layer->height; i++)
	{
		//Scan pixel rect for positive integers
		//For each positive integer, create a corresponding
		//  entry in the new layer (pixelrectstack) with the next bit of data
		//  from the nps file.
		k=0;  // If 16-bit, need to keep track of both bytes.
		p=0;  // If 32-bit, need to keep track of 4 bytes.
		for(j=0; j<(layer->width)*4; j+=4) 
		{
			if( (layer->pixelType==3 && p1pixelrect->currentData[j/4]>0) || 
			    (layer->pixelType==5 && (p1pixelrect->currentData[(j/4)+k]>0 ||
				                         p1pixelrect->currentData[(j/4)+k+1]>0) ) ||
				(layer->pixelType==7 && (p1pixelrect->currentData[(j/4)+p]>0 ||
										 p1pixelrect->currentData[(j/4)+p+1]>0 ||
										 p1pixelrect->currentData[(j/4)+p+2]>0 ||
										 p1pixelrect->currentData[(j/4)+p+3]>0) ) )
			{
				/***** Hydro data *****/
				fscanf(fptr,"%i",&cellNum);				
				fscanf(fptr,"%i",&cellDivision);
				fscanf(fptr,"%f.2",&drainArea);
				fscanf(fptr,"%f.2",&eqRunoff);
				fscanf(fptr,"%f.2",&upRunoff);
				fscanf(fptr,"%f.2",&upFlow);
				fscanf(fptr,"%f.2",&downRunoff);
				fscanf(fptr,"%f.2",&downFlow);
				fscanf(fptr,"%f.1",&runoffAbove);

				// Fill array values (a global variable).
				floatToBinary(drainArea);

				// Fill pixel rects values with those values.
				stack1->datalayer[0]->currentData[j]=array[3];
				stack1->datalayer[0]->currentData[j+1]=array[2];
				stack1->datalayer[0]->currentData[j+2]=array[1];
				stack1->datalayer[0]->currentData[j+3]=array[0];
				
				floatToBinary(eqRunoff);
				stack1->datalayer[1]->currentData[j]=array[3];
				stack1->datalayer[1]->currentData[j+1]=array[2];
				stack1->datalayer[1]->currentData[j+2]=array[1];
				stack1->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(upRunoff);
				stack1->datalayer[2]->currentData[j]=array[3];
				stack1->datalayer[2]->currentData[j+1]=array[2];
				stack1->datalayer[2]->currentData[j+2]=array[1];
				stack1->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(upFlow);
				stack1->datalayer[3]->currentData[j]=array[3];
				stack1->datalayer[3]->currentData[j+1]=array[2];
				stack1->datalayer[3]->currentData[j+2]=array[1];
				stack1->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(downRunoff);
				stack1->datalayer[4]->currentData[j]=array[3];
				stack1->datalayer[4]->currentData[j+1]=array[2];
				stack1->datalayer[4]->currentData[j+2]=array[1];
				stack1->datalayer[4]->currentData[j+3]=array[0];

				floatToBinary(downFlow);
				stack1->datalayer[5]->currentData[j]=array[3];
				stack1->datalayer[5]->currentData[j+1]=array[2];
				stack1->datalayer[5]->currentData[j+2]=array[1];
				stack1->datalayer[5]->currentData[j+3]=array[0];

				floatToBinary(runoffAbove);
				stack1->datalayer[6]->currentData[j]=array[3];
				stack1->datalayer[6]->currentData[j+1]=array[2];
				stack1->datalayer[6]->currentData[j+2]=array[1];
				stack1->datalayer[6]->currentData[j+3]=array[0];

				/******* Clay data ******/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);
				
				floatToBinary(cellErosion);
				stack2->datalayer[0]->currentData[j]=array[3];
				stack2->datalayer[0]->currentData[j+1]=array[2];
				stack2->datalayer[0]->currentData[j+2]=array[1];
				stack2->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack2->datalayer[1]->currentData[j]=array[3];
				stack2->datalayer[1]->currentData[j+1]=array[2];
				stack2->datalayer[1]->currentData[j+2]=array[1];
				stack2->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack2->datalayer[2]->currentData[j]=array[3];
				stack2->datalayer[2]->currentData[j+1]=array[2];
				stack2->datalayer[2]->currentData[j+2]=array[1];
				stack2->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack2->datalayer[3]->currentData[j]=array[3];
				stack2->datalayer[3]->currentData[j+1]=array[2];
				stack2->datalayer[3]->currentData[j+2]=array[1];
				stack2->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack2->datalayer[4]->currentData[j]=array[3];
				stack2->datalayer[4]->currentData[j+1]=array[2];
				stack2->datalayer[4]->currentData[j+2]=array[1];
				stack2->datalayer[4]->currentData[j+3]=array[0];

				/****** Silt data ******/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);

				floatToBinary(cellErosion);
				stack3->datalayer[0]->currentData[j]=array[3];
				stack3->datalayer[0]->currentData[j+1]=array[2];
				stack3->datalayer[0]->currentData[j+2]=array[1];
				stack3->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack3->datalayer[1]->currentData[j]=array[3];
				stack3->datalayer[1]->currentData[j+1]=array[2];
				stack3->datalayer[1]->currentData[j+2]=array[1];
				stack3->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack3->datalayer[2]->currentData[j]=array[3];
				stack3->datalayer[2]->currentData[j+1]=array[2];
				stack3->datalayer[2]->currentData[j+2]=array[1];
				stack3->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack3->datalayer[3]->currentData[j]=array[3];
				stack3->datalayer[3]->currentData[j+1]=array[2];
				stack3->datalayer[3]->currentData[j+2]=array[1];
				stack3->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack3->datalayer[4]->currentData[j]=array[3];
				stack3->datalayer[4]->currentData[j+1]=array[2];
				stack3->datalayer[4]->currentData[j+2]=array[1];
				stack3->datalayer[4]->currentData[j+3]=array[0];

				/****** SAGG data ******/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);

				floatToBinary(cellErosion);
				stack4->datalayer[0]->currentData[j]=array[3];
				stack4->datalayer[0]->currentData[j+1]=array[2];
				stack4->datalayer[0]->currentData[j+2]=array[1];
				stack4->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack4->datalayer[1]->currentData[j]=array[3];
				stack4->datalayer[1]->currentData[j+1]=array[2];
				stack4->datalayer[1]->currentData[j+2]=array[1];
				stack4->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack4->datalayer[2]->currentData[j]=array[3];
				stack4->datalayer[2]->currentData[j+1]=array[2];
				stack4->datalayer[2]->currentData[j+2]=array[1];
				stack4->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack4->datalayer[3]->currentData[j]=array[3];
				stack4->datalayer[3]->currentData[j+1]=array[2];
				stack4->datalayer[3]->currentData[j+2]=array[1];
				stack4->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack4->datalayer[4]->currentData[j]=array[3];
				stack4->datalayer[4]->currentData[j+1]=array[2];
				stack4->datalayer[4]->currentData[j+2]=array[1];
				stack4->datalayer[4]->currentData[j+3]=array[0];

				/***** LAGG data ******/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);

				floatToBinary(cellErosion);
				stack5->datalayer[0]->currentData[j]=array[3];
				stack5->datalayer[0]->currentData[j+1]=array[2];
				stack5->datalayer[0]->currentData[j+2]=array[1];
				stack5->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack5->datalayer[1]->currentData[j]=array[3];
				stack5->datalayer[1]->currentData[j+1]=array[2];
				stack5->datalayer[1]->currentData[j+2]=array[1];
				stack5->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack5->datalayer[2]->currentData[j]=array[3];
				stack5->datalayer[2]->currentData[j+1]=array[2];
				stack5->datalayer[2]->currentData[j+2]=array[1];
				stack5->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack5->datalayer[3]->currentData[j]=array[3];
				stack5->datalayer[3]->currentData[j+1]=array[2];
				stack5->datalayer[3]->currentData[j+2]=array[1];
				stack5->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack5->datalayer[4]->currentData[j]=array[3];
				stack5->datalayer[4]->currentData[j+1]=array[2];
				stack5->datalayer[4]->currentData[j+2]=array[1];
				stack5->datalayer[4]->currentData[j+3]=array[0];

				/******  Sand data *****/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);

				floatToBinary(cellErosion);
				stack6->datalayer[0]->currentData[j]=array[3];
				stack6->datalayer[0]->currentData[j+1]=array[2];
				stack6->datalayer[0]->currentData[j+2]=array[1];
				stack6->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack6->datalayer[1]->currentData[j]=array[3];
				stack6->datalayer[1]->currentData[j+1]=array[2];
				stack6->datalayer[1]->currentData[j+2]=array[1];
				stack6->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack6->datalayer[2]->currentData[j]=array[3];
				stack6->datalayer[2]->currentData[j+1]=array[2];
				stack6->datalayer[2]->currentData[j+2]=array[1];
				stack6->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack6->datalayer[3]->currentData[j]=array[3];
				stack6->datalayer[3]->currentData[j+1]=array[2];
				stack6->datalayer[3]->currentData[j+2]=array[1];
				stack6->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack6->datalayer[4]->currentData[j]=array[3];
				stack6->datalayer[4]->currentData[j+1]=array[2];
				stack6->datalayer[4]->currentData[j+2]=array[1];
				stack6->datalayer[4]->currentData[j+3]=array[0];

				/****** Total data ******/

				fscanf(fptr,"%f.2",&cellErosion);
				fscanf(fptr,"%f.2",&upSedYield);
				fscanf(fptr,"%f.2",&sedGenerated);
				fscanf(fptr,"%f.2",&sedYield);
				fscanf(fptr,"%i",&deposition);

				floatToBinary(cellErosion);
				stack7->datalayer[0]->currentData[j]=array[3];
				stack7->datalayer[0]->currentData[j+1]=array[2];
				stack7->datalayer[0]->currentData[j+2]=array[1];
				stack7->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(upSedYield);
				stack7->datalayer[1]->currentData[j]=array[3];
				stack7->datalayer[1]->currentData[j+1]=array[2];
				stack7->datalayer[1]->currentData[j+2]=array[1];
				stack7->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedGenerated);
				stack7->datalayer[2]->currentData[j]=array[3];
				stack7->datalayer[2]->currentData[j+1]=array[2];
				stack7->datalayer[2]->currentData[j+2]=array[1];
				stack7->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(sedYield);
				stack7->datalayer[3]->currentData[j]=array[3];
				stack7->datalayer[3]->currentData[j+1]=array[2];
				stack7->datalayer[3]->currentData[j+2]=array[1];
				stack7->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(deposition);
				stack7->datalayer[4]->currentData[j]=array[3];
				stack7->datalayer[4]->currentData[j+1]=array[2];
				stack7->datalayer[4]->currentData[j+2]=array[1];
				stack7->datalayer[4]->currentData[j+3]=array[0];
			}
			else   // No data for that cell.
			{
				stack1->datalayer[0]->currentData[j]=0;
				stack1->datalayer[0]->currentData[j+1]=0;
				stack1->datalayer[0]->currentData[j+2]=0;
				stack1->datalayer[0]->currentData[j+3]=0;

				stack1->datalayer[1]->currentData[j]=0;
				stack1->datalayer[1]->currentData[j+1]=0;
				stack1->datalayer[1]->currentData[j+2]=0;
				stack1->datalayer[1]->currentData[j+3]=0;

				stack1->datalayer[2]->currentData[j]=0;
				stack1->datalayer[2]->currentData[j+1]=0;
				stack1->datalayer[2]->currentData[j+2]=0;
				stack1->datalayer[2]->currentData[j+3]=0;

				stack1->datalayer[3]->currentData[j]=0;
				stack1->datalayer[3]->currentData[j+1]=0;
				stack1->datalayer[3]->currentData[j+2]=0;
				stack1->datalayer[3]->currentData[j+3]=0;

				stack1->datalayer[4]->currentData[j]=0;
				stack1->datalayer[4]->currentData[j+1]=0;
				stack1->datalayer[4]->currentData[j+2]=0;
				stack1->datalayer[4]->currentData[j+3]=0;

				stack1->datalayer[5]->currentData[j]=0;
				stack1->datalayer[5]->currentData[j+1]=0;
				stack1->datalayer[5]->currentData[j+2]=0;
				stack1->datalayer[5]->currentData[j+3]=0;

				stack1->datalayer[6]->currentData[j]=0;
				stack1->datalayer[6]->currentData[j+1]=0;
				stack1->datalayer[6]->currentData[j+2]=0;
				stack1->datalayer[6]->currentData[j+3]=0;

				stack2->datalayer[0]->currentData[j]=0;
				stack2->datalayer[0]->currentData[j+1]=0;
				stack2->datalayer[0]->currentData[j+2]=0;
				stack2->datalayer[0]->currentData[j+3]=0;

				stack2->datalayer[1]->currentData[j]=0;
				stack2->datalayer[1]->currentData[j+1]=0;
				stack2->datalayer[1]->currentData[j+2]=0;
				stack2->datalayer[1]->currentData[j+3]=0;

				stack2->datalayer[2]->currentData[j]=0;
				stack2->datalayer[2]->currentData[j+1]=0;
				stack2->datalayer[2]->currentData[j+2]=0;
				stack2->datalayer[2]->currentData[j+3]=0;

				stack2->datalayer[3]->currentData[j]=0;
				stack2->datalayer[3]->currentData[j+1]=0;
				stack2->datalayer[3]->currentData[j+2]=0;
				stack2->datalayer[3]->currentData[j+3]=0;

				stack2->datalayer[4]->currentData[j]=0;
				stack2->datalayer[4]->currentData[j+1]=0;
				stack2->datalayer[4]->currentData[j+2]=0;
				stack2->datalayer[4]->currentData[j+3]=0;

				stack3->datalayer[0]->currentData[j]=0;
				stack3->datalayer[0]->currentData[j+1]=0;
				stack3->datalayer[0]->currentData[j+2]=0;
				stack3->datalayer[0]->currentData[j+3]=0;

				stack3->datalayer[1]->currentData[j]=0;
				stack3->datalayer[1]->currentData[j+1]=0;
				stack3->datalayer[1]->currentData[j+2]=0;
				stack3->datalayer[1]->currentData[j+3]=0;

				stack3->datalayer[2]->currentData[j]=0;
				stack3->datalayer[2]->currentData[j+1]=0;
				stack3->datalayer[2]->currentData[j+2]=0;
				stack3->datalayer[2]->currentData[j+3]=0;

				stack3->datalayer[3]->currentData[j]=0;
				stack3->datalayer[3]->currentData[j+1]=0;
				stack3->datalayer[3]->currentData[j+2]=0;
				stack3->datalayer[3]->currentData[j+3]=0;

				stack3->datalayer[4]->currentData[j]=0;
				stack3->datalayer[4]->currentData[j+1]=0;
				stack3->datalayer[4]->currentData[j+2]=0;
				stack3->datalayer[4]->currentData[j+3]=0;

				stack4->datalayer[0]->currentData[j]=0;
				stack4->datalayer[0]->currentData[j+1]=0;
				stack4->datalayer[0]->currentData[j+2]=0;
				stack4->datalayer[0]->currentData[j+3]=0;

				stack4->datalayer[1]->currentData[j]=0;
				stack4->datalayer[1]->currentData[j+1]=0;
				stack4->datalayer[1]->currentData[j+2]=0;
				stack4->datalayer[1]->currentData[j+3]=0;

				stack4->datalayer[2]->currentData[j]=0;
				stack4->datalayer[2]->currentData[j+1]=0;
				stack4->datalayer[2]->currentData[j+2]=0;
				stack4->datalayer[2]->currentData[j+3]=0;

				stack4->datalayer[3]->currentData[j]=0;
				stack4->datalayer[3]->currentData[j+1]=0;
				stack4->datalayer[3]->currentData[j+2]=0;
				stack4->datalayer[3]->currentData[j+3]=0;

				stack4->datalayer[4]->currentData[j]=0;
				stack4->datalayer[4]->currentData[j+1]=0;
				stack4->datalayer[4]->currentData[j+2]=0;
				stack4->datalayer[4]->currentData[j+3]=0;

				stack5->datalayer[0]->currentData[j]=0;
				stack5->datalayer[0]->currentData[j+1]=0;
				stack5->datalayer[0]->currentData[j+2]=0;
				stack5->datalayer[0]->currentData[j+3]=0;

				stack5->datalayer[1]->currentData[j]=0;
				stack5->datalayer[1]->currentData[j+1]=0;
				stack5->datalayer[1]->currentData[j+2]=0;
				stack5->datalayer[1]->currentData[j+3]=0;

				stack5->datalayer[2]->currentData[j]=0;
				stack5->datalayer[2]->currentData[j+1]=0;
				stack5->datalayer[2]->currentData[j+2]=0;
				stack5->datalayer[2]->currentData[j+3]=0;

				stack5->datalayer[3]->currentData[j]=0;
				stack5->datalayer[3]->currentData[j+1]=0;
				stack5->datalayer[3]->currentData[j+2]=0;
				stack5->datalayer[3]->currentData[j+3]=0;

				stack5->datalayer[4]->currentData[j]=0;
				stack5->datalayer[4]->currentData[j+1]=0;
				stack5->datalayer[4]->currentData[j+2]=0;
				stack5->datalayer[4]->currentData[j+3]=0;

				stack6->datalayer[0]->currentData[j]=0;
				stack6->datalayer[0]->currentData[j+1]=0;
				stack6->datalayer[0]->currentData[j+2]=0;
				stack6->datalayer[0]->currentData[j+3]=0;

				stack6->datalayer[1]->currentData[j]=0;
				stack6->datalayer[1]->currentData[j+1]=0;
				stack6->datalayer[1]->currentData[j+2]=0;
				stack6->datalayer[1]->currentData[j+3]=0;

				stack6->datalayer[2]->currentData[j]=0;
				stack6->datalayer[2]->currentData[j+1]=0;
				stack6->datalayer[2]->currentData[j+2]=0;
				stack6->datalayer[2]->currentData[j+3]=0;

				stack6->datalayer[3]->currentData[j]=0;
				stack6->datalayer[3]->currentData[j+1]=0;
				stack6->datalayer[3]->currentData[j+2]=0;
				stack6->datalayer[3]->currentData[j+3]=0;

				stack6->datalayer[4]->currentData[j]=0;
				stack6->datalayer[4]->currentData[j+1]=0;
				stack6->datalayer[4]->currentData[j+2]=0;
				stack6->datalayer[4]->currentData[j+3]=0;

				stack7->datalayer[0]->currentData[j]=0;
				stack7->datalayer[0]->currentData[j+1]=0;
				stack7->datalayer[0]->currentData[j+2]=0;
				stack7->datalayer[0]->currentData[j+3]=0;

				stack7->datalayer[1]->currentData[j]=0;
				stack7->datalayer[1]->currentData[j+1]=0;
				stack7->datalayer[1]->currentData[j+2]=0;
				stack7->datalayer[1]->currentData[j+3]=0;

				stack7->datalayer[2]->currentData[j]=0;
				stack7->datalayer[2]->currentData[j+1]=0;
				stack7->datalayer[2]->currentData[j+2]=0;
				stack7->datalayer[2]->currentData[j+3]=0;

				stack7->datalayer[3]->currentData[j]=0;
				stack7->datalayer[3]->currentData[j+1]=0;
				stack7->datalayer[3]->currentData[j+2]=0;
				stack7->datalayer[3]->currentData[j+3]=0;

				stack7->datalayer[4]->currentData[j]=0;
				stack7->datalayer[4]->currentData[j+1]=0;
				stack7->datalayer[4]->currentData[j+2]=0;
				stack7->datalayer[4]->currentData[j+3]=0;
			}
			k++;       // 16 bit checking
			p=p+3;     // 32 bit checking
		}

		// Write the row stored in the pixelrect to the layer.
		eimg_LayerStackWrite(layerstack1,0,i,layer->width,1,stack1,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 1"));

		eimg_LayerStackWrite(layerstack2,0,i,layer->width,1,stack2,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 2"));

		eimg_LayerStackWrite(layerstack3,0,i,layer->width,1,stack3,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 3"));

		eimg_LayerStackWrite(layerstack4,0,i,layer->width,1,stack4,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 4"));

		eimg_LayerStackWrite(layerstack5,0,i,layer->width,1,stack5,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 5"));

		eimg_LayerStackWrite(layerstack6,0,i,layer->width,1,stack6,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 6"));

		eimg_LayerStackWrite(layerstack7,0,i,layer->width,1,stack7,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 7"));

		// Read the next layer.
		eimg_LayerRead(layer, 0, i+1, layer->width, 1,
					   p1pixelrect, &lcl);
		EERR_CHECK(lcl, EFS0("Error Reading More of Layer."));

		// Keep track of progress.
		emet_MeterInfoPrint(meter, (double)((i*100)/layer->height), &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
	}

	// Done with First 7 Images
	// Close the p1 Layer
	eimg_LayerClose(layer,&lcl);
	layer = NULL;

	//Reopen it for Images 8 and 9
	layer = eimg_LayerOpen(name->strings[0], eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layer"));

	// Read one full row of data for each layer.
	eimg_LayerRead(layer, 0, 0, layer->width, 1,
		p1pixelrect, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Layer."));

	emet_MeterInfoChangeTaskName(meter,"Converting NPS file to Imagine images... (2 of 2)",
		&lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	// Nutrient Data
	fscanf(fptr,"%s",useless);
	fscanf(fptr,"%s",useless);

	for(i=0; i<layer->height; i++)
	{
		//Scan pixel rect for positive integers
		//For each positive integer, create a corresponding
		//  entry in the new layer (pixelrectstack)with the next bit of data
		//  from the nps file.
		k=0;  // If 16-bit, need to keep track of both bytes.
		p=0;  // If 132-bit, need to keep track of 4 bytes.
		for(j=0; j<(layer->width)*4; j+=4) 
		{
			if( (layer->pixelType==3 && p1pixelrect->currentData[j/4]>0) || 
			    (layer->pixelType==5 && (p1pixelrect->currentData[(j/4)+k]>0 ||
				                         p1pixelrect->currentData[(j/4)+k+1]>0) ) ||
				(layer->pixelType==7 && (p1pixelrect->currentData[(j/4)+p]>0 ||
										 p1pixelrect->currentData[(j/4)+p+1]>0 ||
										 p1pixelrect->currentData[(j/4)+p+2]>0 ||
										 p1pixelrect->currentData[(j/4)+p+3]>0) ) )
			{
				/***** Nitrogen data *****/
				fscanf(fptr,"%i",&cellNum);
				fscanf(fptr,"%i",&cellDivision);
				fscanf(fptr,"%f.2",&drainArea);
				fscanf(fptr,"%f.2",&cellNitro);
				fscanf(fptr,"%f.2",&sedNitro);
				fscanf(fptr,"%f.2",&nitroRunoff);
				fscanf(fptr,"%f.2",&totalNitro);
				fscanf(fptr,"%f.2",&nitroConc);
				
				floatToBinary(drainArea);
				stack8->datalayer[0]->currentData[j]=array[3];
				stack8->datalayer[0]->currentData[j+1]=array[2];
				stack8->datalayer[0]->currentData[j+2]=array[1];
				stack8->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(cellNitro);
				stack8->datalayer[1]->currentData[j]=array[3];
				stack8->datalayer[1]->currentData[j+1]=array[2];
				stack8->datalayer[1]->currentData[j+2]=array[1];
				stack8->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(sedNitro);
				stack8->datalayer[2]->currentData[j]=array[3];
				stack8->datalayer[2]->currentData[j+1]=array[2];
				stack8->datalayer[2]->currentData[j+2]=array[1];
				stack8->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(nitroRunoff);
				stack8->datalayer[3]->currentData[j]=array[3];
				stack8->datalayer[3]->currentData[j+1]=array[2];
				stack8->datalayer[3]->currentData[j+2]=array[1];
				stack8->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(totalNitro);
				stack8->datalayer[4]->currentData[j]=array[3];
				stack8->datalayer[4]->currentData[j+1]=array[2];
				stack8->datalayer[4]->currentData[j+2]=array[1];
				stack8->datalayer[4]->currentData[j+3]=array[0];

				floatToBinary(nitroConc);
				stack8->datalayer[5]->currentData[j]=array[3];
				stack8->datalayer[5]->currentData[j+1]=array[2];
				stack8->datalayer[5]->currentData[j+2]=array[1];
				stack8->datalayer[5]->currentData[j+3]=array[0];

				/***** Phosphorous and COD data ****/

				fscanf(fptr,"%f.2",&cellPhospho);
				fscanf(fptr,"%f.2",&sedPhospho);
				fscanf(fptr,"%f.2",&phosphoRunoff);
				fscanf(fptr,"%f.2",&totalPhospho);
				fscanf(fptr,"%f.2",&phosphoConc);
				fscanf(fptr,"%f.2",&cellCOD);
				fscanf(fptr,"%f.2",&totalCOD);
				fscanf(fptr,"%f.2",&CODConc);

				floatToBinary(cellPhospho);
				stack9->datalayer[0]->currentData[j]=array[3];
				stack9->datalayer[0]->currentData[j+1]=array[2];
				stack9->datalayer[0]->currentData[j+2]=array[1];
				stack9->datalayer[0]->currentData[j+3]=array[0];

				floatToBinary(sedPhospho);
				stack9->datalayer[1]->currentData[j]=array[3];
				stack9->datalayer[1]->currentData[j+1]=array[2];
				stack9->datalayer[1]->currentData[j+2]=array[1];
				stack9->datalayer[1]->currentData[j+3]=array[0];

				floatToBinary(phosphoRunoff);
				stack9->datalayer[2]->currentData[j]=array[3];
				stack9->datalayer[2]->currentData[j+1]=array[2];
				stack9->datalayer[2]->currentData[j+2]=array[1];
				stack9->datalayer[2]->currentData[j+3]=array[0];

				floatToBinary(totalPhospho);
				stack9->datalayer[3]->currentData[j]=array[3];
				stack9->datalayer[3]->currentData[j+1]=array[2];
				stack9->datalayer[3]->currentData[j+2]=array[1];
				stack9->datalayer[3]->currentData[j+3]=array[0];

				floatToBinary(phosphoConc);
				stack9->datalayer[4]->currentData[j]=array[3];
				stack9->datalayer[4]->currentData[j+1]=array[2];
				stack9->datalayer[4]->currentData[j+2]=array[1];
				stack9->datalayer[4]->currentData[j+3]=array[0];

				floatToBinary(cellCOD);
				stack9->datalayer[5]->currentData[j]=array[3];
				stack9->datalayer[5]->currentData[j+1]=array[2];
				stack9->datalayer[5]->currentData[j+2]=array[1];
				stack9->datalayer[5]->currentData[j+3]=array[0];

				floatToBinary(totalCOD);
				stack9->datalayer[6]->currentData[j]=array[3];
				stack9->datalayer[6]->currentData[j+1]=array[2];
				stack9->datalayer[6]->currentData[j+2]=array[1];
				stack9->datalayer[6]->currentData[j+3]=array[0];

				floatToBinary(CODConc);
				stack9->datalayer[7]->currentData[j]=array[3];
				stack9->datalayer[7]->currentData[j+1]=array[2];
				stack9->datalayer[7]->currentData[j+2]=array[1];
				stack9->datalayer[7]->currentData[j+3]=array[0];

			}
			else  // No values for that cell.
			{
				stack8->datalayer[0]->currentData[j]=0;
				stack8->datalayer[0]->currentData[j+1]=0;
				stack8->datalayer[0]->currentData[j+2]=0;
				stack8->datalayer[0]->currentData[j+3]=0;

				stack8->datalayer[1]->currentData[j]=0;
				stack8->datalayer[1]->currentData[j+1]=0;
				stack8->datalayer[1]->currentData[j+2]=0;
				stack8->datalayer[1]->currentData[j+3]=0;

				stack8->datalayer[2]->currentData[j]=0;
				stack8->datalayer[2]->currentData[j+1]=0;
				stack8->datalayer[2]->currentData[j+2]=0;
				stack8->datalayer[2]->currentData[j+3]=0;

				stack8->datalayer[3]->currentData[j]=0;
				stack8->datalayer[3]->currentData[j+1]=0;
				stack8->datalayer[3]->currentData[j+2]=0;
				stack8->datalayer[3]->currentData[j+3]=0;

				stack8->datalayer[4]->currentData[j]=0;
				stack8->datalayer[4]->currentData[j+1]=0;
				stack8->datalayer[4]->currentData[j+2]=0;
				stack8->datalayer[4]->currentData[j+3]=0;

				stack8->datalayer[5]->currentData[j]=0;
				stack8->datalayer[5]->currentData[j+1]=0;
				stack8->datalayer[5]->currentData[j+2]=0;
				stack8->datalayer[5]->currentData[j+3]=0;

				stack9->datalayer[0]->currentData[j]=0;
				stack9->datalayer[0]->currentData[j+1]=0;
				stack9->datalayer[0]->currentData[j+2]=0;
				stack9->datalayer[0]->currentData[j+3]=0;

				stack9->datalayer[1]->currentData[j]=0;
				stack9->datalayer[1]->currentData[j+1]=0;
				stack9->datalayer[1]->currentData[j+2]=0;
				stack9->datalayer[1]->currentData[j+3]=0;

				stack9->datalayer[2]->currentData[j]=0;
				stack9->datalayer[2]->currentData[j+1]=0;
				stack9->datalayer[2]->currentData[j+2]=0;
				stack9->datalayer[2]->currentData[j+3]=0;

				stack9->datalayer[3]->currentData[j]=0;
				stack9->datalayer[3]->currentData[j+1]=0;
				stack9->datalayer[3]->currentData[j+2]=0;
				stack9->datalayer[3]->currentData[j+3]=0;

				stack9->datalayer[4]->currentData[j]=0;
				stack9->datalayer[4]->currentData[j+1]=0;
				stack9->datalayer[4]->currentData[j+2]=0;
				stack9->datalayer[4]->currentData[j+3]=0;

				stack9->datalayer[5]->currentData[j]=0;
				stack9->datalayer[5]->currentData[j+1]=0;
				stack9->datalayer[5]->currentData[j+2]=0;
				stack9->datalayer[5]->currentData[j+3]=0;

				stack9->datalayer[6]->currentData[j]=0;
				stack9->datalayer[6]->currentData[j+1]=0;
				stack9->datalayer[6]->currentData[j+2]=0;
				stack9->datalayer[6]->currentData[j+3]=0;

				stack9->datalayer[7]->currentData[j]=0;
				stack9->datalayer[7]->currentData[j+1]=0;
				stack9->datalayer[7]->currentData[j+2]=0;
				stack9->datalayer[7]->currentData[j+3]=0;
			}
			k++;        // 16 bit checking
			p=p+3;      // 32 bit checking
		}

		// Write data from pixelrect to layer for this row.
		eimg_LayerStackWrite(layerstack8,0,i,layer->width,1,stack8,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 8"));

		eimg_LayerStackWrite(layerstack9,0,i,layer->width,1,stack9,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing layer stack 9"));


		// Read one full row of data for each layer.
		eimg_LayerRead(layer, 0, i+1, layer->width, 1,
			p1pixelrect, &lcl);
		EERR_CHECK(lcl, EFS0("Error Reading Layer."));

		// Keep track of progress.
		emet_MeterInfoPrint(meter, (double)((i*100)/layer->height), &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
	}


	emet_MeterInfoChangeTaskName(meter,"Writing map and projection information...",
		&lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	/****  Write map info and projection info for each layer of each image *****/

	eimg_MapInfoWrite(layerstack1->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[4],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[5],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack1->layers[6],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack1->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[4],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[5],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack1->layers[6],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)11, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack2->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack2->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack2->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack2->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack2->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack2->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack2->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack2->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack2->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack2->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)22, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack3->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack3->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack3->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack3->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack3->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack3->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack3->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack3->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack3->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack3->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)33, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack4->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack4->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack4->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack4->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack4->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack4->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack4->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack4->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack4->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack4->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)44, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack5->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack5->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack5->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack5->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack5->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack5->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack5->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack5->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack5->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack5->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)55, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack6->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack6->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack6->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack6->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack6->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack6->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack6->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack6->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack6->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack6->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)66, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack7->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack7->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack7->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack7->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack7->layers[4],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack7->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack7->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack7->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack7->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack7->layers[4],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)77, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack8->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack8->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack8->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack8->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack8->layers[4],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack8->layers[5],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack8->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack8->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack8->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack8->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack8->layers[4],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack8->layers[5],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	emet_MeterInfoPrint(meter, (double)88, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	eimg_MapInfoWrite(layerstack9->layers[0],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[1],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[2],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[3],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[4],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[5],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[6],mapinfo,&lcl);
	eimg_MapInfoWrite(layerstack9->layers[7],mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(layerstack9->layers[0],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[1],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[2],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[3],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[4],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[5],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[6],projinfo,&lcl);
	eimg_ProParametersWrite(layerstack9->layers[7],projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));
	
	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

// Clear up memory and all that jazz.		
cleanup:

	fclose(fptr);

	estr_StringListDelete(name,&lcl);
	name = NULL;

	emet_MeterInfoDelete(meter, &lcl);
	meter = NULL;

	eprj_MapInfoFree( &mapinfo);
	mapinfo = NULL;

	eprj_ProParametersFree(&projinfo);
	projinfo=NULL;

	esta_StatisticsDelete( stats, &lcl);
	stats = NULL;

	eimg_PixelRectDelete(p1pixelrect, &lcl);
	p1pixelrect = NULL;

	eimg_PixelRectStackDelete(stack1, &lcl);
	stack1 = NULL;

	eimg_PixelRectStackDelete(stack2, &lcl);
	stack2 = NULL;

	eimg_PixelRectStackDelete(stack3, &lcl);
	stack3 = NULL;

	eimg_PixelRectStackDelete(stack4, &lcl);
	stack4 = NULL;

	eimg_PixelRectStackDelete(stack5, &lcl);
	stack5 = NULL;

	eimg_PixelRectStackDelete(stack6, &lcl);
	stack6 = NULL;

	eimg_PixelRectStackDelete(stack7, &lcl);
	stack7 = NULL;

	eimg_PixelRectStackDelete(stack8, &lcl);
	stack8 = NULL;

	eimg_PixelRectStackDelete(stack9, &lcl);
	stack9 = NULL;

	eimg_LayerStackClose(layerstack1,&lcl);
	layerstack1=NULL;

	eimg_LayerStackClose(layerstack2,&lcl);
	layerstack2=NULL;

	eimg_LayerStackClose(layerstack3,&lcl);
	layerstack3=NULL;

	eimg_LayerStackClose(layerstack4,&lcl);
	layerstack4=NULL;

	eimg_LayerStackClose(layerstack5,&lcl);
	layerstack5=NULL;

	eimg_LayerStackClose(layerstack6,&lcl);
	layerstack6=NULL;

	eimg_LayerStackClose(layerstack7,&lcl);
	layerstack7=NULL;

	eimg_LayerStackClose(layerstack8,&lcl);
	layerstack8=NULL;

	eimg_LayerStackClose(layerstack9,&lcl);
	layerstack9=NULL;

	eimg_LayerNameStackDelete(allnames,&lcl);
	allnames = NULL;	

	eimg_LayerClose(layer,&lcl);
	layer = NULL;

	return rtnVal;
}


/*****************************************************************************
** FUNCTION NAME
** imageToAscii
**    Convert an Imagine image to arc ascii format
** 
** SYNTAX
**  extern Estr_StringList *
**     imageToAscii(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *imageToAscii( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList						*rtnVal=NULL;
	Estr_StringList						*listname=NULL;
	Eimg_Layer							*layer=NULL;
	Eimg_PixelRect						*pixelrect=NULL;
	Eprj_MapInfo						*mapinfo=NULL;
	Emet_MeterInfo						*meter = (Emet_MeterInfo*)NULL;
	char								*asciifile=NULL;
	unsigned char						sign,exponent;
	int									i,j,rows,k;
	long int							outnum;
	float								fullmantissa,s,e,f;
	float								mantissa[3]={0};
	FILE								*fptr;

	
	// Image to convert : argv[0]
	// Set up like formatData
	/* Will need:
		ncols        ->   eimg_Layer
		nrows        ->   eimg_Layer
		xllcorner	 ->   eprj_mapinfo
		yllcorner	 ->	  eprj_mapinfo
		cellsize     ->   eprj_mapinfo
		nodata_value ->   make up
		row1 val val val val val val ........
		row2 val val ....
		.
		.
		.
    */

	EERR_INIT( "imagetoascii", err, lcl );

	// Create and set a meter to show percentage done to the user.
	meter = emet_MeterInfoCreate("Starting process", 
		"Convert Image",stdout, &lcl);
	EERR_CHECK( lcl, EFS0("Error creating the meter."));

	emet_MeterInfoSet( meter, 0.0, 100.0, &lcl);
	EERR_CHECK( lcl, EFS0("Error setting the meter info!"));

	emet_MeterInfoChangeTaskName(meter,
		"Converting image to Arc ASCII format...", &lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// For parameter 1 file.
	listname = eimg_LayerGetNames(argv[0], eint_GetInit(),&lcl);
	EERR_CHECK(lcl, EFS0("Error getting name"));
	
	// Open a Layer type (provides the correct structure)
	layer = eimg_LayerOpen(listname->strings[0], eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layerstack"));

	// Create a buffer that will contain one full row at a time 
	pixelrect = eimg_PixelRectCreate(layer->width, 1,
		layer->pixelType, &lcl);	
	EERR_CHECK(lcl, EFS0("Error creating pixel rect."));

	// Read one full row of data for each layer.
	eimg_LayerRead(layer, 0, 0, layer->width, 1,
		pixelrect, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

	// Create a structure to hold map information
	mapinfo = eprj_MapInfoCreate( &lcl);
	EERR_CHECK(lcl, EFS0("Error Creating MapInfo Structure."));

	// Read the map information of the top layer.
	eimg_MapInfoRead(layer, mapinfo, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading MapInfo."));

	// Create the new file name by changing the img from the
	//    input file(argv[0]) to asc.
	asciifile="";
	strcpy(asciifile,argv[0]);
	strncpy(asciifile + (strlen(asciifile)-4),".asc",4);
	fptr = fopen(asciifile,"w");  // Write only

	// Name of the file is function's return value.
	rtnVal = estr_StringListAppend(rtnVal,asciifile,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating StringList"));

	// Create the header of the file.
	fprintf(fptr,"ncols %i\n", layer->width);
	fprintf(fptr,"nrows %i\n", layer->height);
	fprintf(fptr,"xllcorner %f\n", (float)mapinfo->upperLeftCenter->x);
	fprintf(fptr,"yllcorner %f\n", (float)mapinfo->lowerRightCenter->y);
	fprintf(fptr,"cellsize %i\n", (int)mapinfo->pixelSize->width);
	fprintf(fptr,"nodata_value -9999\n");

	if(layer->pixelType==3 )  // unsigned 8 bit integer 
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<layer->width; i++)
			{			
				fprintf(fptr, "%i ",pixelrect->currentData[i]);
			}

			fprintf(fptr,"\n");

		// Keep track of progress.
		emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}	
	}
	else if(layer->pixelType==5 )  // unsigned 16 bit integer
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*2; i=i+2)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum = ((pixelrect->currentData[i+1]<<8) | 
					pixelrect->currentData[i]);

				fprintf(fptr, "%i ",outnum);				
			}

			fprintf(fptr,"\n");
				
			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		}
	}
	else if(layer->pixelType==7 )  // unsigned 32 bit integer
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{	
				// "Bit-wise or" all of the bits together to make the number.
				outnum= ((pixelrect->currentData[i+3]<<24) |
					(pixelrect->currentData[i+2]<<16) |
					(pixelrect->currentData[i+1]<<8) |
					pixelrect->currentData[i]);

				fprintf(fptr, "%i ",outnum);				
			}

			fprintf(fptr,"\n");				

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		}
	}
	else if(layer->pixelType==9)   // 32 bit float
	{
		for( rows=0; rows<(layer->height); rows++)
		{
			// Read in one row of data.
			eimg_LayerRead(layer, 0, rows, layer->width, 1, pixelrect, &lcl);
			EERR_CHECK(lcl, EFS0("Error Reading Layer Stack."));

			for( i=0; i<(layer->width)*4; i=i+4)
			{
				/* Base equation for a binary 4 byte real number:
				 *		x = (-1)^S * m * 2^(E-B)  
				 * Where S is the sign of the number, 
				 *		 m is what's behind the decimal point, and
				 *		 E is the exponent
				 *		 B is the offset (127 for 4 byte reals)
				 *
				 * In binary format:
				 *		SEEEEEEE Emmmmmmm mmmmmmmm mmmmmmmm
				 *
				 * With this format use the equation:
				 *		x = -1^S * 1.m * 2^(E-B)
				 ***												  ***
				 *** IMPORTANT: Imagine data is stored like this:     ***
				 ***	mmmmmmmm mmmmmmmm Emmmmmmm SEEEEEEE			  ***
				 ***												  ***
				 *
				 * Use same equation, but careful how you get the data.
				 *
				 */

				// Special case for a value of zero (won't catch it otherwise)
				if(pixelrect->currentData[i]==0
					&& pixelrect->currentData[i+1]==0
					&& pixelrect->currentData[i+2]==0
					&& pixelrect->currentData[i+3]==0 )
					fprintf(fptr,"0 ");
				else
				{
					// Bit shift everything off but the sign bit.
					// This becomes S (bit 1) in the above equation
					sign = (pixelrect->currentData[i+3])>>7;  

					// Bit shift off the sign bit and bitwise "or" with the
					//    second byte everything but the exponent bit.
					// This will be E (bits 2-9) in the above equation.
					exponent = (pixelrect->currentData[i+3]<<1) 
								| (pixelrect->currentData[i+2]>>7);

					// All of these together will be m in the above equation.
					mantissa[0] = 0;	// bits 10-16
					mantissa[1] = 0;	// bits 17-24
					mantissa[2] = 0;	// bits 25-32

					// Bit shift off numbers to the left, then to the right, then see if it exists.
					//    If so, convert it to decimal and add to mantissa.
					for(j=7;j>0;j--)
					{ 
						if( ((((pixelrect->currentData[i+2])<<(8-j))>>7) & 1) == 1) 
							mantissa[0]+= (1/(pow(2,9-j))); 
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i+1])<<(8-j))>>7) & 1) == 1) 
							mantissa[1]+= (1/(pow(2,17-j)));
					}
					for(j=8;j>0;j--)
					{
						if( ((((pixelrect->currentData[i])<<(8-j))>>7) & 1) == 1) 
							mantissa[2]+= (1/(pow(2,25-j)));
					}

					fullmantissa = 2*(mantissa[0]+mantissa[1]+mantissa[2]);		// m
				
					s = pow(-1,sign);
					e = pow(2,exponent-127);									// B = 127
					f = 1+fullmantissa;
					fprintf(fptr,"%i ", (int)(s * f * e));
					

				}   // Number created				
								
			}		// Row created	

			fprintf(fptr,"\n");

			// Keep track of progress.
			emet_MeterInfoPrint(meter, (double)((rows*100)/layer->height), &lcl);
			EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

		}
	}
	else
	{
		// Error: Cannot handle anything other than unsigned 8,16, and 32 bit 
		//        integers or 32 bit floats.
		*err = eerr_CreateErrorReport("imagetoascii", (Eerr_ErrorCode)3L,
			EERR_FATALERROR,lcl,"Error. Data type not supported.",NULL);
		eerr_PrintErrorReport(*err,EERR_ENDUSER,eint_GetInit());
	}
	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
		


cleanup:

	fclose(fptr);
	asciifile=NULL;

	emet_MeterInfoDelete(meter, &lcl);
	meter = NULL;

	eprj_MapInfoFree( &mapinfo);
	mapinfo = NULL;

	eimg_PixelRectDelete(pixelrect, &lcl);
	pixelrect = NULL;

	eimg_LayerClose(layer,&lcl);
	layer = NULL;
	
	estr_StringListDelete(listname,&lcl);
	listname = NULL;

	return rtnVal;
}

/*****************************************************************************
** FUNCTION NAME
** asciiToImage
**    create images from arc ascii file
** 
** SYNTAX
**  extern Estr_StringList *
**     asciiToImage(menu, context, argc, argv, err )
**
** ARGUMENTS
**
**  Eeml_Menu menu;
**	Emsc_Opaque *context;
**	long argc;
**	char **argv;
**	Eerr_ErrorReport **err;
**
** DESCRIPTION
**  Uses the output from tardem (.asc) to make Imagine image.
**  
**
** RETURN VALUE
**
**  Estr_StringList *
**  
**
******************************************************************************/
extern Estr_StringList *asciiToImage( menu, context, argc, argv, err )
Eeml_Menu menu;
Emsc_Opaque *context;
long argc;
char **argv;
Eerr_ErrorReport **err;
{
	Estr_StringList			*rtnVal=NULL;
	Estr_StringList			*name=NULL;
	Eimg_Layer				*layer=NULL;
	Eimg_Layer				*newlayer=NULL;
	Eimg_PixelRect			*newpixelrect=NULL;
	Eprj_MapInfo			*mapinfo=NULL;
	Eprj_ProParameters		*projinfo=NULL;
	Esta_Statistics			*stats=NULL;
	Emet_MeterInfo			*meter = (Emet_MeterInfo*)NULL;
	char					useless[255]="",imagefile[255]="";
	char					pathname[101]="",filename[31]="",*newname=NULL;
	char					checkfile[132]="";
	int						i, j, strahler, no_value;
	FILE					*fptr;
	

	EERR_INIT( "asciitoimage", err, lcl );

	// Create and set a meter to show percentage done to the user.
	meter = emet_MeterInfoCreate("Starting process", 
		"Convert ASCII to Image",stdout, &lcl);
	EERR_CHECK( lcl, EFS0("Error creating the meter."));

	emet_MeterInfoSet( meter, 0.0, 100.0, &lcl);
	EERR_CHECK( lcl, EFS0("Error setting the meter info!"));

	emet_MeterInfoChangeTaskName(meter,
		"Initializing...", &lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// For dem file.
	name = eimg_LayerGetNames(argv[1], eint_GetInit(),&lcl);
	EERR_CHECK(lcl, EFS0("Error getting name"));
	
	// Open a Layer type (provides the correct structure)
	layer = eimg_LayerOpen(name->strings[0], eint_GetInit(), &lcl,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error opening layer"));

	// Create structure to hold map information.
	mapinfo = eprj_MapInfoCreate( &lcl);
	EERR_CHECK(lcl, EFS0("Error Creating MapInfo Structure."));

	// Read the map information.
	eimg_MapInfoRead(layer,mapinfo, &lcl);
	EERR_CHECK(lcl, EFS0("Error Reading MapInfo."));

	// Same with projection information
	projinfo = eprj_ProParametersCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Map Projection Structure."));

	eimg_ProParametersRead(layer,projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading ProParameters."));

	// And same with map statistics
	stats = esta_StatisticsCreate(&lcl);
	EERR_CHECK(lcl, EFS0("Error Creating Statistics Structure."));

	eimg_StatisticsRead(layer,stats,&lcl);
	EERR_CHECK(lcl, EFS0("Error Reading Statistics"));

	// Create pixel rect to hold one row of strahler data
	newpixelrect = eimg_PixelRectCreate(layer->width,1,3, &lcl);
	EERR_CHECK(lcl, EFS0("Error creating new pixel rect"));	

	// Open the ascii file for reading.
	fptr = fopen(argv[0],"r");

	// Functions to get path and filename needed for layer creation.
	estr_GetPathPart(argv[0],pathname,100);

	estr_GetNamePart(argv[0],filename,30);
	strncpy(filename + (strlen(filename)-4),".img",4);

	strcpy(checkfile,pathname);
	strcat(checkfile,filename);

	// If the file exists, delete it. If it exists, eimg_LayerCreate errors.
	_unlink(checkfile);	
	
    // Set up the name for the layer needed to create the layer.
	newname = eimg_LayerNameCreate(pathname,filename,"Layer_1",&lcl);
	EERR_CHECK(lcl, EFS0("Error creating name stack")); 

	// Create the layer
	newlayer = eimg_LayerCreate(newname,1,3,layer->width,
		layer->height,eint_GetInit(),&lcl,EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
		EIMG_LAYER_OPTION_END);
	EERR_CHECK(lcl, EFS0("Error creating layer stack"));

	// Get rid of the layer name, don't neet it.
	newname = NULL;  	

	emet_MeterInfoChangeTaskName(meter,"Converting asc file to Imagine image...",
		&lcl);
	EERR_CHECK( lcl, EFS0("Error changing name of meter info!"));

	emet_MeterInfoPrint(meter, (double)0, &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Name of the new file is function's return value.
	// Create the new file name by changing the asc from the
	//    input ascii file(argv[0]) to img.
	//	imagefile="";
	strcpy(imagefile,argv[0]);
	strncpy(imagefile + (strlen(imagefile)-4),".img",4);
	rtnVal = estr_StringListAppend(rtnVal,imagefile,&lcl);
	EERR_CHECK(lcl, EFS0("Error creating StringList"));

	// Header Info (will already have from the dem image)
	for(i=0; i<5; i++)
	{
		fgets(useless,256,fptr);
	}	

	// Get the nodata_value.
	fscanf(fptr,"%s",&useless);
	fscanf(fptr,"%i",&no_value);
	
	// For all the rows
	for(i=0; i<layer->height; i++)
	{
		// For all the columns
		for(j=0; j<(layer->width); j++) 
		{
			fscanf(fptr,"%i",&strahler);  //Bring in the number
					
			if(strahler != no_value)
			{
				// Fill pixel rect with converted number.
				newpixelrect->currentData[j]=strahler;
			}
			else
			{
				// No definitive channel.
				newpixelrect->currentData[j]=1;
			}
		}

		// Write the row stored in the pixelrect to the layer.
		eimg_LayerWrite(newlayer,0,i,layer->width,1,newpixelrect,&lcl);
		EERR_CHECK(lcl,EFS0("Error writing newlayer"));

		// Keep track of progress.
		emet_MeterInfoPrint(meter, (double)((i*100)/layer->height), &lcl);
	    EERR_CHECK( lcl, EFS0("Error printing the meter info!"));
	}

	emet_MeterInfoPrint(meter, (double)100, &lcl);
	EERR_CHECK( lcl, EFS0("Error printing the meter info!"));

	// Put same info from dem image to the new image.
	// Stats will be computed when layer is closed.
	eimg_MapInfoWrite(newlayer,mapinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing map info"));

	eimg_ProParametersWrite(newlayer,projinfo,&lcl);
	EERR_CHECK(lcl, EFS0("Error writing projection info"));	

cleanup:

	fclose(fptr);

	emet_MeterInfoDelete(meter, &lcl);
	meter = NULL;

	eprj_MapInfoFree( &mapinfo);
	mapinfo = NULL;

	eprj_ProParametersFree(&projinfo);
	projinfo=NULL;

	esta_StatisticsDelete( stats, &lcl);
	stats = NULL;

	eimg_PixelRectDelete(newpixelrect, &lcl);
	newpixelrect = NULL;

	// Close the Layers
	eimg_LayerClose(layer,&lcl);
	layer = NULL;

	eimg_LayerClose(newlayer,&lcl);
	newlayer = NULL;

	return rtnVal;
}

/*********************************************************************\
**
** Function: floatToBinary( float )
**
** Descripition:  Converts a number to a 4-byte floating point Imagine
**                value stored as 4 single byte integers in a global
**                array.
**
** Returns:  Nothing.  Needed values put into global array.  Okay, so
**           maybe global values are messy, but that's the way it is
**           right now.
**
** Future Concerns: 2 things.
**           1.  If Imagine changes their image format.... it's screwed.
**					This isn't very likely anytime soon, though.
**					I say that now.....
**           2.  If AGNPS changes the accuracy of their output,
**                  this will not be quite so accurate.  This goes
**					along with the version 5.0 where there is at most
**					2 digits following the decimal.
**
\*********************************************************************/
void floatToBinary(float arg)
{
	//long double				fandb,back=0,backbin=0;
	//int						e=0,s=0,b=127;
	//int						front,place=0,rev=0,store,first,second;
	//int						third=0,fourth=0,convfirst=0,convsec=0;
	//int						convthird=0,convfourth=0;
	//int						bins[24]={0},ordbins[24]={0};
	//double					frontbin=0;
	//long					ebin=0;
	//long					trial,trial2,trial3;
	//float					trial4;
	//int						k,reorder=0,prereq=0;
	unsigned char *bytes;


	// Clear out global array each time function is called.
	array[0]=0;
	array[1]=0;
	array[2]=0;
	array[3]=0;

	if(arg!=0)  // No need for this if it's zero.
	{
		bytes = (unsigned char*)&arg;  //Create a pointer to the value

		array[0]=*(bytes+3);           //Grab each byte and store it.
		array[1]=*(bytes+2);
		array[2]=*(bytes+1);
		array[3]=*bytes;
	}


	/**
	 * The code below works perfectly, and was my original method
	 * of getting the arg in byte arrays for Imagine, but a young man
	 * by the name of Chris Bilderback helped me shorten it a ton
	 * by suggesting I use a pointer to the number and type-cast 
	 * it as an unsigned char to maintain the byte storage.
	 */

	/*if(arg!=0)  // No need for this if it's zero.
	{
		// Grab the integer part of the number.
		front = (long)arg;

		// Multiply by 100.  These were test variables to
		//    take care of floating point errors and they
		//    worked, so I never renamed them.
		trial = front * pow(10,2);

		// Multiply original by 100 and round to avoid
		//     floating point screwups.
		// These floating point fixes assume that the
		//     incoming AGNPS data has at most 2 numbers
		//     behind the decimal.  This is the case for
		//     version 5.0.
		trial2 = (long)ceil(arg * pow(10,2)); 

		// What's behind the decimal, but as an integer.
		trial3 = trial2 - trial;
	
		// Back to a decimal.
		trial4 = trial3 * pow(10,-2);

		if(front<0) 
		{
			s=1;            //Sign
			front*=-1;
		}

		// Convert front of the number to "binary"
		//    Front meaning in front of the decimal.
		// This just makes an integer representation
		//    and an array versionof the binary number.
		//    Both are used later.
		while( front > 0 )
		{
			// If divisible by two
			if(front%2!=0)
			{
				// Put a 1 in the correct binary spot.
				frontbin = frontbin+(pow(10,place));
				bins[place]=1;
			}
			else
				bins[place]=0;

			// Divide by two, and truncate any remainder.
			front=(long)front/2;

			// Keep track of how "binary" number is.
			place++;					
		}
			
		// Above method has array set up backwards.
		// Put "binary" in correct order in ordbins array.
		place--;
		while(place >= 0)
		{
			ordbins[rev]=bins[place];
			rev++;
			bins[place]=0;
			place--;
		}

		// Decimal of the number to "binary".
		// Fix for floating point errors by using the trial 
		//	  variables.
		place=-1;
		while( ( trial3 != 0) && (place > -25) ) 
		{
			if( trial3 != 100)
				trial3 *= 2;

			// If greater than 100, subtract off 100 and
			//    keep track of binary representation.
			if(trial3>=100)
			{
				trial3-=100;
				backbin = backbin + (pow(10,place));
				bins[abs(place)-1]=1;
			}
			else
				bins[abs(place)-1]=0;
			place--;
		}
		
		// Full binary representation
		fandb=frontbin+backbin;

		// Add decimal part to front part in array.
		store=0;
		while( (store<(abs(place)-1)) && (rev < 24) )
		{
			ordbins[rev]=bins[store]; 
			store++;
			rev++;
		}
				

		// Calculate Exponent
		if(fandb>1)
		{
			// Normalize fandb and keep track of how much
			//    shifting was required (This is E).
			while( (fandb/10) > 1 )
			{
				fandb = fandb/10;
				e++;
			}
		}
		else  // Numbers smaller than 1 are handled differently
		{
			// Normalize fandb and keep track of how much
			//    shifting was required (This is E).
			while( (fandb) < 1 )
			{
				fandb = fandb*10;
				e--;
			}

			//Normalize decimal array
			for(k=0;k<24;k++)
			{
				// Find first instance of 1.
				if(ordbins[k]==1 && prereq==0)
				{
					ordbins[reorder]=1;
					prereq=1;
					reorder++;
				}
				else if(prereq==1)  // Go on from there.
				{
					ordbins[reorder]=ordbins[k];
					reorder++;
				}					
			}

			//Clear out any extra junk
			for(k=reorder;k<24;k++)
			{
				ordbins[reorder]=0;
			}
		}

		e=e+b;   // Final E with offset B.

		place=0;
		// Exponent to binary
		while( e > 0 )
		{
			// If divisible by two, add 1 to correct binary place
			if(e%2!=0)
			{
				ebin = ebin+(pow(10,place)); 
			}
			e=(long)e/2; 
			place++;					
		}

		// Create first as integer representation of binary number
		// Using sign and first 7 numbers of ebin.
		first = s*(pow(10,7));
		first = first + (ebin/10);

		// Create second "binary" number using last number of ebin
		//   and then use ordbins 0-6 for last 7 "bits".
		if( ebin%2 == 0)
			second = 0;
		else
			second = 1*(pow(10,7));  // Make as front "bit".

		for(k=6; k>=0; k--)
		{
			second = second + (ordbins[7-k])*(pow(10,k));
		}

		// Create third "binary" by using positions 7-14 as the
		//    8 "bits" of the number.
		for(k=14; k>=7; k--)
		{
			third = third + (ordbins[22-k])*(pow(10,k-7));
		}

		// Same with the fourth.
		for(k=22; k>=15; k--)
		{
			fourth = fourth + (ordbins[38-k])*(pow(10,k-15));
		}

		// Now convert each integer representation, or "byte", 
		//   of the binary number to the actual integer
		//   value of that byte. 
		//   (e.g. 10000011 = 128 + 2 + 1 = 131)
		k=0;
		while(first>0)
		{
			// If divisible by ten, add it's correct binary
			//    placement.
			if(first%10 != 0)
			{
				convfirst = convfirst + pow(2,k);
			}

			// Then divide by ten, truncating the remainder.
			first=first/10;
			k++; 
		}

		k=0;
		while(second>0)
		{
			if(second%10 != 0)
			{
				convsec = convsec + pow(2,k);
			}
			second=second/10;
			k++;
		}

		k=0;
		while(third>0)
		{
			if(third%10 != 0)
			{
				convthird = convthird + pow(2,k);
			}
			third=third/10;
			k++;
		}

		k=0;
		while(fourth>0)
		{
			if(fourth%10 != 0)
			{
				convfourth = convfourth + pow(2,k);
			}
			fourth=fourth/10;
			k++;
		}

		// Set the global arrays to these integers.
		array[0] = convfirst;
		array[1] = convsec;
		array[2] = convthird;
		array[3] = convfourth;
	}
*/
	return;
}

/*********************************************************************\
**
** Function: intToBinary16( int )
**
** Descripition:  Converts a number to a 2-byte integer Imagine
**                value stored as 2 single byte integers in a global
**                array.
**
** Returns:  Nothing.  Needed values put into global array.  Okay, so
**           maybe global values are messy, but that's the way it is
**           right now.
**
** Future Concerns: 
**           1.  If Imagine changes their image format.... it's screwed.
**					This isn't very likely anytime soon, though.
**					I say that now.....
**
\*********************************************************************/
void intToBinary16(int arg)
{
	//int						front,place=0,rev=0,first=0,second=0;
	//int						convfirst=0,convsec=0;
	//int						bins[16]={0},ordbins[16]={0};
	//double					frontbin=0;
	//int						k;
	unsigned char * bytes;

	// Clear out global array each time function is called.
	array[0]=0;
	array[1]=0;
	array[2]=0;
	array[3]=0;

	bytes = (unsigned char*)&arg;

	array[0]=*(bytes+1);
	array[1]=*bytes;
/*
	// Grab the integer part of the number.
	front = (long)arg;

	// This just makes an integer representation
	//    and an array versionof the binary number.
	//    Both are used later.
	while( front > 0 )
	{
		// If divisible by two
		if(front%2!=0)
		{
			// Put a 1 in the correct binary spot.
			frontbin = frontbin+(pow(10,place));
			bins[15-place]=1;
		}
		else
			bins[15-place]=0;
			// Divide by two, and truncate any remainder.
		front=(long)front/2;
			// Keep track of how "binary" number is.
		place++;					
	}
			

	// Create first "binary" number using 
	//   bins 0-7 for last 7 "bits".
	for(k=7; k>=0; k--)
	{
		first = first + (bins[7-k])*(pow(10,k));
	}
	
	// Create second "binary" by using positions 8-15 as the
	//    8 "bits" of the number.
	for(k=15; k>=8; k--)
	{
		second = second + (bins[23-k])*(pow(10,k-8));
	}
	
		
	// Now convert each integer representation, or "byte", 
	//   of the binary number to the actual integer
	//   value of that byte. 
	//   (e.g. 10000011 = 128 + 2 + 1 = 131)
	k=0;
	while(first>0)
	{
		// If divisible by ten, add it's correct binary
		//    placement.
		if(first%10 != 0)
		{
			convfirst = convfirst + pow(2,k);
		}
			
		// Then divide by ten, truncating the remainder.
		first=first/10;
		k++; 
	}
	
	k=0;
	while(second>0)
	{
		if(second%10 != 0)
		{
			convsec = convsec + pow(2,k);
		}
		second=second/10;
		k++;
	}
	
	
	// Set the global arrays to these integers.
	array[0] = convfirst;
	array[1] = convsec;
*/
return;
}



/*********************************************************************\
**
** Function: intToBinary32( int )
**
** Descripition:  Converts a number to a 4-byte integer Imagine
**                value stored as 4 single byte integers in a global
**                array.
**
** Returns:  Nothing.  Needed values put into global array.  Okay, so
**           maybe global values are messy, but that's the way it is
**           right now.
**
** Future Concerns: 
**           1.  If Imagine changes their image format.... it's screwed.
**					This isn't very likely anytime soon, though.
**					I say that now.....
**
\*********************************************************************/
void intToBinary32(int arg)
{
	//int						front,place=0,rev=0,first=0,second=0;
	//int						third=0,fourth=0,convfirst=0,convsec=0;
	//int						convthird=0,convfourth=0;
	//int						bins[32]={0},ordbins[32]={0};
	//double					frontbin=0;
	//int						k;
    unsigned char * bytes;

	// Clear out global array each time function is called.
	array[0]=0;
	array[1]=0;
	array[2]=0;
	array[3]=0;

    bytes = (unsigned char*)&arg;

	array[0]=*(bytes+3);
	array[1]=*(bytes+2);
	array[2]=*(bytes+1);
	array[3]=*bytes;

/*
	// Grab the integer part of the number.
	front = (long)arg;

	// This just makes an integer representation
	//    and an array versionof the binary number.
	//    Both are used later.
	while( front > 0 )
	{
		// If divisible by two
		if(front%2!=0)
		{
			// Put a 1 in the correct binary spot.
			frontbin = frontbin+(pow(10,place));
			bins[31-place]=1;
		}
		else
			bins[31-place]=0;
			// Divide by two, and truncate any remainder.
		front=(long)front/2;
			// Keep track of how "binary" number is.
		place++;					
	}
			

	// Create first "binary" number using 
	//   bins 0-7 for last 7 "bits".
	for(k=7; k>=0; k--)
	{
		first = first + (bins[7-k])*(pow(10,k));
	}
	
	// Create second "binary" by using positions 8-15 as the
	//    8 "bits" of the number.
	for(k=15; k>=8; k--)
	{
		second = second + (bins[23-k])*(pow(10,k-8));
	}

	// Create third "binary" by using positions 16-23 as the
	//    8 "bits" of the number.
	for(k=23; k>=16; k--)
	{
		third = third + (bins[39-k])*(pow(10,k-16));
	}

	// Create second "binary" by using positions 24-31 as the
	//    8 "bits" of the number.
	for(k=31; k>=24; k--)
	{
		fourth = fourth + (bins[55-k])*(pow(10,k-24));
	}
	
		
	// Now convert each integer representation, or "byte", 
	//   of the binary number to the actual integer
	//   value of that byte. 
	//   (e.g. 10000011 = 128 + 2 + 1 = 131)
	k=0;
	while(first>0)
	{
		// If divisible by ten, add it's correct binary
		//    placement.
		if(first%10 != 0)
		{
			convfirst = convfirst + pow(2,k);
		}
			
		// Then divide by ten, truncating the remainder.
		first=first/10;
		k++; 
	}
	
	k=0;
	while(second>0)
	{
		if(second%10 != 0)
		{
			convsec = convsec + pow(2,k);
		}
		second=second/10;
		k++;
	}

	k=0;
	while(third>0)
	{
		if(third%10 != 0)
		{
			convthird = convthird + pow(2,k);
		}
		third=third/10;
		k++;
	}

	k=0;
	while(fourth>0)
	{
		if(fourth%10 != 0)
		{
			convfourth = convfourth + pow(2,k);
		}
		fourth=fourth/10;
		k++;
	}
	
	
	// Set the global arrays to these integers.
	array[0] = convfirst;
	array[1] = convsec;
	array[2] = convthird;
	array[3] = convfourth;
*/
return;
}