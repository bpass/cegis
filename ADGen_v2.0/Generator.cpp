///////////////////////////////////////////////////////////////////////
// ADGen v2.0
// This program was originally developed in C by Doug Scheidt and Tim
//    Krupinski of the USGS, among others that did not leave their
//    names.  This version I ported over to C++ and rewrote each line
//    from the code I was given.  The original code was done by so many
//    people that it desperately needed cleaning up.  I have also added
//    comments where I know what's going on and to tell where something
//    is changed from the version I was given.  I will try to
//    distinguish my notes from the original notes, and tell when I 
//    know what the heck is going on.
///////////////////////////////////////////////////////////////////////
// Author: Kyle Williams
// ADGen v2.0
// 6-21-05
// USGS MCMC in Rolla, MO
///////////////////////////////////////////////////////////////////////

#include "ErdSetup.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>

#include<direct.h>

const int MAX_PATH_SIZE       = 100;
const int MAX_FILENAME_SIZE   = 30;
const int NUM_OF_PARAMS       = 22;
const int NUM_OF_LAYERS       = 9;
const __int64 OUTLET_CELL_FLOW= 50000000;

namespace ADGEN {
   enum {E = 1, SE, S, SW, W, NW, N, NE};
}
namespace ARC {
   enum {S = 1, SW = 2, W = 4, NW = 8, N = 16, NE = 32, E = 64, SE = 128};
}
enum SOIL{SAND = 1, SILT, CLAY, PEAT};

enum FERTILIZER{LOW = 1, AVERAGE, HIGH, OTHER};

enum {PARAM_1 = 0, PARAM_2, PARAM_3, PARAM_4, PARAM_5, PARAM_6, PARAM_7, 
      PARAM_8, PARAM_9, PARAM_10, PARAM_11, PARAM_12, PARAM_13, PARAM_14, 
      PARAM_15, PARAM_16, PARAM_17, PARAM_18, PARAM_19, PARAM_20, PARAM_21, 
      PARAM_22};



///////////////////////////////////////////////////////////////////////////////
// The 'moveData' functions are around to directly move bytes of information.
//    Imagine stores their floats backwards from how the machine stores them.
//    (Little Endian vs Big Endian) The code before me had a large amount of
//    code with lots of math to get the data from the array of bytes that
//    Imagine stores it in to a regular float.  I knew it could be simpler, so
//    first I tried 'bit-wise or'ing the data together, but that didn't work,
//    so I moved the data directly to where it should be in the float byte by
//    byte.  I needed to do this often enough with integers also that I created
//    a function for it.  The backwards variable is true for floats and false
//    for integers when dealing with Imagine data.
///////////////////////////////////////////////////////////////////////////////
void moveData(char* dest, const Egda_Byte* src, int numBytes, bool backwards);
void moveData(Egda_Byte* dest, const char* src, int numBytes, bool backwards);


///////////////////////////////////////////////////////////////////////
// Function Name: dismissFunc
// Purpose: Stop the executable from running
// Description: Retrieves the handle of the viewer and image, gets the
//    number of each, and returns to the eml
// Returns: NULL
///////////////////////////////////////////////////////////////////////
// Taken straight from given code.  No idea on how this works yet.
///////////////////////////////////////////////////////////////////////
Estr_StringList* dismissFunc(Eeml_Menu          menu, 
                             Emsc_Opaque*       context, 
                             long               argc, 
                             char**             argv, 
                             Eerr_ErrorReport** err)
{
   AppContext* app = (AppContext*)context;
   EERR_INIT("dismissFunc", err, lcl);

   app->root->doneFlag = EMSC_TRUE;

cleanup:
   if(*err) {}

   return NULL;
}

///////////////////////////////////////////////////////////////////////
// Function Name: numberCells
// Description: Creates a layer where the numbers range from 1 to the
//    number of cells included in the layer.  Each cell has a unique
//    number.
///////////////////////////////////////////////////////////////////////

Estr_StringList* numberCells(Eeml_Menu          menu, 
                             Emsc_Opaque*       context, 
                             long               argc, 
                             char**             argv, 
                             Eerr_ErrorReport** err)
{
   Emet_MeterInfo*         statusBar   = NULL;
   Estr_StringList*        listName    = NULL;
   Estr_StringList*        outputFile  = NULL;
   Eimg_PixelRect*         lineBuffer  = NULL; //These buffers hold one row of
   Eimg_PixelRect*         newBuffer   = NULL; //   the layer at a time.
   Eimg_Layer*             layer       = NULL;
   Eimg_Layer*             newLayer    = NULL;
   Eprj_MapInfo*           mapInfo     = NULL;
   Eprj_ProParameters*     projInfo    = NULL;
   

   char                    pathName[MAX_PATH_SIZE + 1];
   char                    fileName[MAX_FILENAME_SIZE + 1];
   char*                   newLayerName;
   __int64                 counter     = 0;
   const char*             c_byte = (char*)&counter;

   ////////// Temporary variables //////////
   long                    t_height, t_width, t_counter;
   __int64                 t_num;
   float                   t_float;
   char*                   t_char;
   Egda_PixelType          t_type;
   



   EERR_INIT("numberCells", err, lcl);

   //////////////////////////////////////////////
   // Create a status bar
   //////////////////////////////////////////////
   statusBar = emet_MeterInfoCreate(
                        "Reading Cell Data...", 
                        "Numbering Cells",
                        stdout,
                        &lcl
               );
   EERR_CHECK(lcl, EFS0("Error creating status bar"));

   emet_MeterInfoSet(statusBar, 0.0, 100.0, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting status bar info"));

   emet_MeterInfoPrint(statusBar, double(0), &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar info"));

   /////////////////////////////////////////////////////////////////
   // Open a layer
   /////////////////////////////////////////////////////////////////
   listName = eimg_LayerGetNames(argv[0], eint_GetInit(), &lcl);
   EERR_CHECK(lcl, EFS0("Error setting layer name."));

   layer = eimg_LayerOpen(listName->strings[0], eint_GetInit(), &lcl,
                              EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error opening layer."));

   ///////////////////////////////////////////////////////////////////
   // Retrieve the path and file names from the command line and save
   ///////////////////////////////////////////////////////////////////
   estr_GetPathPart(argv[1], pathName, MAX_PATH_SIZE);
   estr_GetNamePart(argv[1], fileName, MAX_FILENAME_SIZE);

   newLayerName = eimg_LayerNameCreate(pathName, fileName, "Layer_1", &lcl);
   EERR_CHECK(lcl, EFS0("Error creating layer."));

   lineBuffer = eimg_PixelRectCreate(layer->width, 1, /*height*/ 
                                     layer->pixelType, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating buffer PixelRect"));

   ///////////////////////////////////////////////////////////////////
   // Read first layer and create map info and projection info
   ///////////////////////////////////////////////////////////////////
   eimg_LayerRead(layer, 0, 0, layer->width, 1, lineBuffer, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading layer."));

   mapInfo = eprj_MapInfoCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating MapInfo structure"));

   eimg_MapInfoRead(layer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading MapInfo"));

   projInfo = eprj_ProParametersCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating projection structure"));

   eimg_ProParametersRead(layer, projInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading projection parameters"));

   ///////////////////////////////////////////////////////////////////
   // Save name of output file.
   ///////////////////////////////////////////////////////////////////
   outputFile = estr_StringListAppend(outputFile, argv[1], &lcl);
   EERR_CHECK(lcl, EFS0("Error creating StringList"));


   ///////////////////////////////////////////////////////////////////
   // Since the layer is numbered from 1 to the number of valid cells,
   //    we need to count the cells.  Here we go through each row.
   //    A cell is valid if it has a value greater than 0.
   ///////////////////////////////////////////////////////////////////
   // I opted for putting the switch case inside the first loop to
   //    improve readability and greatly decrease code at the expense
   //    of run time.
   ///////////////////////////////////////////////////////////////////
   t_height = layer->height;
   t_width  = layer->width;

   ///////////////////////////////////////////////////////////////////
   // Read a row, then go through each value and move the actual bits
   //    into the temporary variable.  Check that variable for
   //    meaningful data (ie. data > 0).
   ///////////////////////////////////////////////////////////////////
   for(int numRow = 0; numRow < t_height; numRow++)
   {
      eimg_LayerRead(layer, 0, numRow, t_width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading layer."));

      switch(layer->pixelType)
      {
         case EGDA_TYPE_U8:
            t_char = (char*)&t_num;
            for(t_counter = 0; t_counter < t_width; ++t_counter)
            {
               t_num = 0;
               *(t_char) = lineBuffer->currentData[t_counter];
               if(t_num > 0)     ++counter;
            }
            break;

         case EGDA_TYPE_U16:
            t_char = (char*)&t_num;
            for(t_counter = 0; t_counter < t_width; t_counter += 2)
            {
               t_num = 0;
               *(t_char+1) = lineBuffer->currentData[t_counter + 1];
               *(t_char) = lineBuffer->currentData[t_counter];
               if(t_num > 0)     ++counter;
            }
            break;

         case EGDA_TYPE_U32:
            t_char = (char*)&t_num;
            for(t_counter = 0; t_counter < t_width; t_counter += 4)
            {
               *(t_char+3) = lineBuffer->currentData[t_counter + 3];
               *(t_char+2) = lineBuffer->currentData[t_counter + 2];
               *(t_char+1) = lineBuffer->currentData[t_counter + 1];
               *(t_char) = lineBuffer->currentData[t_counter];
               if(t_num > 0)     ++counter;
            }
            break;

         case EGDA_TYPE_F32:
            ///////////////////////////////////////////////////////////////////
            // IMPORTANT INFORMATION:
            // Imagine stores their float information opposite that of a normal
            //    float: i.e. 
            //          SEEEEEEE Emmmmmmm mmmmmmmm mmmmmmmm
            //             is instead (in Imagine)
            //          mmmmmmmm mmmmmmmm Emmmmmmm SEEEEEEE
            ///////////////////////////////////////////////////////////////////
            // The original code had about 60 lines of bitshifting and odd
            //    algebra to convert the number.  Instead, I moved the bytes
            //    into a float directly, making sure to reverse the order, then
            //    treat the float as normal.
            ///////////////////////////////////////////////////////////////////
            t_char = (char*)&t_float;
            for(t_counter = 0; t_counter < t_width; t_counter += 4)
            {
               *(t_char+3) = lineBuffer->currentData[t_counter];
               *(t_char+2) = lineBuffer->currentData[t_counter + 1];
               *(t_char+1) = lineBuffer->currentData[t_counter + 2];
               *(t_char) = lineBuffer->currentData[t_counter + 3];
               if(t_float > 0)   ++counter;
            }
            break;

         default:
            *err = eerr_CreateErrorReport("numberCells", (Eerr_ErrorCode)3L, 
                                          EERR_FATALERROR, lcl, 
                                          "Error: Datatype not supported.",
                                          NULL);
            eerr_PrintErrorReport(*err, EERR_ENDUSER, eint_GetInit());
            break;
      };

      emet_MeterInfoPrint(statusBar, (double)numRow/t_height * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing status bar info."));
   }
   emet_MeterInfoPrint(statusBar, (double)100, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar info."));

   ///////////////////////////////////////////////////////////////////
   // Create the new layer.  The datatype must be large enough to
   //    support the number of the last cell.
   ///////////////////////////////////////////////////////////////////
   t_type = EGDA_TYPE_INVALID;
   if(counter <= 0) {}
   else if(counter < 256)
   {
      t_type = EGDA_TYPE_U8;
   }
   else if(counter < 65536)
   {
      t_type = EGDA_TYPE_U16;
   }
   else if(counter < 4294967296)
   {
      t_type = EGDA_TYPE_U32;
   }

   ///// If there is a valid number of pixels, create the new layer /////
   if(t_type != EGDA_TYPE_INVALID)
   {
      newLayer = eimg_LayerCreate(newLayerName, EIMG_THEMATIC_LAYER, 
                                  t_type,
                                  layer->width, layer->height,
                                  eint_GetInit(), &lcl, 
                                  EIMG_LAYER_OPTION_STATS_SKIP_FACTORS,
                                  1, 1,
                                  EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
                                  EIMG_LAYER_OPTION_END);
      EERR_CHECK(lcl, EFS0("Error creating new layer."));
   }
   else
   {
      *err = eerr_CreateErrorReport("numberCells", (Eerr_ErrorCode)3L,
                                    EERR_FATALERROR, lcl,
                                    "Error: No valid pixels", NULL);
      eerr_PrintErrorReport(*err, EERR_ENDUSER, eint_GetInit());
   }

   ///////////////////////////////////////////////////////////////////
   // Number the valid cells
   ///////////////////////////////////////////////////////////////////
   emet_MeterInfoChangeTaskName(statusBar, "Numbering Cells...", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing meter task name"));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar."));

   newBuffer = eimg_PixelRectCreate(t_width, 1, newLayer->pixelType, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating new buffer."));
   
   counter = 0;

   for(int numRow = 0; numRow < t_width; ++numRow)
   {
      eimg_LayerRead(layer, 0, numRow, t_width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading layer."));

      switch(layer->pixelType)
      {
         case EGDA_TYPE_U8:
            for(t_counter = 0; t_counter < t_width; ++t_counter)
            {
               switch(newLayer->pixelType)
               {
                  case EGDA_TYPE_U8:
                     if(lineBuffer->currentData[t_counter] > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter] = __int8(counter);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter] = 0;
                     }
                     break;
   
                  case EGDA_TYPE_U16:
                     if(lineBuffer->currentData[t_counter] > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter*2]     = *c_byte;
                        newBuffer->currentData[t_counter*2 + 1] = *(c_byte+1);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter*2]     = 0;
                        newBuffer->currentData[t_counter*2 + 1] = 0;
                     }
                     break;
                        
                  case EGDA_TYPE_U32:
                     if(lineBuffer->currentData[t_counter] > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter*4]     = *c_byte;
                        newBuffer->currentData[t_counter*4 + 1] = *(c_byte+1);
                        newBuffer->currentData[t_counter*4 + 2] = *(c_byte+2);
                        newBuffer->currentData[t_counter*4 + 3] = *(c_byte+3);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter*4]     = 0;
                        newBuffer->currentData[t_counter*4 + 1] = 0;
                        newBuffer->currentData[t_counter*4 + 2] = 0;
                        newBuffer->currentData[t_counter*4 + 3] = 0;
                     }
                     break;

                  default:
                     break;
               };
            }
            break;

         case EGDA_TYPE_U16:
            t_char = (char*)&t_num;
            for(t_counter = 0; t_counter < t_width * 2; t_counter += 2)
            {
               *(t_char+1) = lineBuffer->currentData[t_counter + 1];
               *(t_char) = lineBuffer->currentData[t_counter];
               switch(newLayer->pixelType)
               {
                  case EGDA_TYPE_U8:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter/2] = counter;
                     }
                     else
                     {
                        newBuffer->currentData[t_counter/2] = 0;
                     }
                     break;

                  case EGDA_TYPE_U16:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter]     = *c_byte;
                        newBuffer->currentData[t_counter + 1] = *(c_byte+1);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter]     = 0;
                        newBuffer->currentData[t_counter + 1] = 0;
                     }
                     break;

                  case EGDA_TYPE_U32:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter*2]     = *c_byte;
                        newBuffer->currentData[t_counter*2 + 1] = *(c_byte+1);
                        newBuffer->currentData[t_counter*2 + 2] = *(c_byte+2);
                        newBuffer->currentData[t_counter*2 + 3] = *(c_byte+3);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter*2]     = 0;
                        newBuffer->currentData[t_counter*2 + 1] = 0;
                        newBuffer->currentData[t_counter*2 + 2] = 0;
                        newBuffer->currentData[t_counter*2 + 3] = 0;
                     }
                     break;

                  default:
                     break;
               };
            }
            break;

         case EGDA_TYPE_U32:
            t_char = (char*)&t_num;
            for(t_counter = 0; t_counter < t_width * 4; t_counter += 4)
            {
               *(t_char+3) = lineBuffer->currentData[t_counter + 3];
               *(t_char+2) = lineBuffer->currentData[t_counter + 2];
               *(t_char+1) = lineBuffer->currentData[t_counter + 1];
               *(t_char) = lineBuffer->currentData[t_counter];
               switch(newLayer->pixelType)
               {
                  case EGDA_TYPE_U8:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter/4] = counter;
                     }
                     else
                     {
                        newBuffer->currentData[t_counter/4] = 0;
                     }
                     break;

                  case EGDA_TYPE_U16:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter/2]     = *c_byte;
                        newBuffer->currentData[t_counter/2 + 1] = *(c_byte+1);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter/2]     = 0;
                        newBuffer->currentData[t_counter/2 + 1] = 0;
                     }
                     break;

                  case EGDA_TYPE_U32:
                     if(t_num > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter]     = *c_byte;
                        newBuffer->currentData[t_counter + 1] = *(c_byte+1);
                        newBuffer->currentData[t_counter + 2] = *(c_byte+2);
                        newBuffer->currentData[t_counter + 3] = *(c_byte+3);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter]     = 0;
                        newBuffer->currentData[t_counter + 1] = 0;
                        newBuffer->currentData[t_counter + 2] = 0;
                        newBuffer->currentData[t_counter + 3] = 0;
                     }
                     break;

                  default:
                     break;
               };
            }
            break;

         case EGDA_TYPE_F32:
            t_char = (char*)&t_float;
            for(t_counter = 0; t_counter < t_width * 4; t_counter += 4)
            {
               *(t_char+3) = lineBuffer->currentData[t_counter];
               *(t_char+2) = lineBuffer->currentData[t_counter + 1];
               *(t_char+1) = lineBuffer->currentData[t_counter + 2];
               *(t_char) = lineBuffer->currentData[t_counter + 3];
               switch(newLayer->pixelType)
               {
                  case EGDA_TYPE_U8:
                     if(t_float > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter/4] = counter;
                     }
                     else
                     {
                        newBuffer->currentData[t_counter/4] = 0;
                     }
                     break;

                  case EGDA_TYPE_U16:
                     if(t_float > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter/2]     = *c_byte;
                        newBuffer->currentData[t_counter/2 + 1] = *(c_byte+1);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter/2]     = 0;
                        newBuffer->currentData[t_counter/2 + 1] = 0;
                     }
                     break;

                  case EGDA_TYPE_U32:
                     if(t_float > 0)
                     {
                        ++counter;
                        newBuffer->currentData[t_counter]     = *c_byte;
                        newBuffer->currentData[t_counter + 1] = *(c_byte+1);
                        newBuffer->currentData[t_counter + 2] = *(c_byte+2);
                        newBuffer->currentData[t_counter + 3] = *(c_byte+3);
                     }
                     else
                     {
                        newBuffer->currentData[t_counter]     = 0;
                        newBuffer->currentData[t_counter + 1] = 0;
                        newBuffer->currentData[t_counter + 2] = 0;
                        newBuffer->currentData[t_counter + 3] = 0;
                     }
                     break;

                  default:
                     break;
               };
            }
            break;

         default:
            break;
      };
      eimg_LayerWrite(newLayer, 0, numRow, newLayer->width, 1, 
                      newBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error writing buffer."));

      emet_MeterInfoPrint(statusBar, double(numRow)/t_height*100, &lcl);
      EERR_CHECK(lcl, EFS0("Error setting progress."));
   } // End of 'row' loop

   eimg_MapInfoWrite(newLayer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error writing MapInfo"));

   eimg_ProParametersWrite(newLayer, projInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error writing projection info."));

   emet_MeterInfoPrint(statusBar, 100, &lcl);
   EERR_CHECK(lcl, EFS0("Error writing to status bar."));

cleanup:
   emet_MeterInfoDelete(statusBar, &lcl);
   statusBar = NULL;

   estr_StringListDelete(listName, &lcl);
   listName = NULL;

   estr_StringListDelete(outputFile, &lcl);
   outputFile = NULL;

   eimg_PixelRectDelete(lineBuffer, &lcl);
   lineBuffer = NULL;

   eimg_PixelRectDelete(newBuffer, &lcl);
   newBuffer = NULL;

   eimg_LayerClose(layer, &lcl);
   layer = NULL;

   eimg_LayerClose(newLayer, &lcl);
   newLayer = NULL;

   eprj_MapInfoFree(&mapInfo);
   mapInfo = NULL;

   eprj_ProParametersFree(&projInfo);
   projInfo = NULL;

   if(newLayerName != NULL)
   {
      free(newLayerName);
      newLayerName = NULL;
   }

   //No memory is allocated to pointers 'c_byte' or 't_char'

   return NULL;
}

///////////////////////////////////////////////////////////////////////
// Function: saveHeaderFlags
// Description: This function saves the AGNPS header flags and 
//    variables in the hflags global variable.
///////////////////////////////////////////////////////////////////////
Estr_StringList* saveHeaderFlags(Eeml_Menu            menu, 
                                 Emsc_Opaque*         context, 
                                 long                 argc, 
                                 char**               argv, 
                                 Eerr_ErrorReport**   err)
{
   EERR_INIT("saveHeaderFlags", err, lcl);

   hflags.errorLog   = atoi(argv[0]);
   hflags.srcAcct    = atoi(argv[1]);
   hflags.sediment   = atoi(argv[2]);
   hflags.hydrology  = atoi(argv[3]);
   hflags.nutrient   = atoi(argv[4]);
   hflags.pesticide  = atoi(argv[5]);
   
   hflags.hyd_calc   = atoi(argv[6]);
   hflags.geomorph   = atoi(argv[7]);
   hflags.k_coeff    = atoi(argv[8]);
   hflags.prepeak    = atoi(argv[9]);
   
   strncpy(hflags.storm_type, argv[10], 5);
   hflags.energy_int = atof(argv[11]);
   hflags.duration   = atof(argv[12]);
   hflags.precip     = atof(argv[13]);
   hflags.nitro_conc = atof(argv[14]);

cleanup:
   return NULL;
}

///////////////////////////////////////////////////////////////////////
// Function: formatData
// Description: This function retrieves the stacked images of AGNPS
//    parameters and creates a data file for AGNPS input.
///////////////////////////////////////////////////////////////////////
Estr_StringList* formatData(Eeml_Menu           menu, 
                            Emsc_Opaque*        context, 
                            long                argc, 
                            char**              argv, 
                            Eerr_ErrorReport**  err)
{
   Emet_MeterInfo*         statusBar   = NULL;
   Estr_StringList*        nameList    = NULL;
   Eimg_LayerStack*        layerStack  = NULL;
   Eimg_PixelRectStack*    lineBuffer  = NULL;
   Eprj_MapInfo*           mapInfo     = NULL;
   Esta_Statistics*        statsInfo   = NULL;

   FILE*                   outFile     = NULL;
   bool*                   validCell   = NULL;
   __int32                 outletCell  = 0;
   

   long                    t_width, t_height;
   
   // 'params' tells which parameters the user chose to include.
   bool                    params[NUM_OF_PARAMS] = {0};

   //Buffer to get the data from 'lineBuffer'
   float**                 data        = NULL;

   //Temporary variables
   int                     param6;
   char*                   t_char = NULL;
   __int32                 t_int = 0;
   float                   t_float = 0;



   EERR_INIT("formatData", err, lcl);

   ///////////////////////////////////////////////////////
   // Create and set a status bar to show progress.
   ///////////////////////////////////////////////////////
   statusBar = emet_MeterInfoCreate("Starting Process", "Convert Image Stack",
                                    stdout, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating status bar."));

   emet_MeterInfoSet(statusBar, 0.0, 100.0, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting status bar info."));

   emet_MeterInfoChangeTaskName(statusBar, 
                     "Converting Image Stack to AGNPS data format.", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing status bar message."));

   emet_MeterInfoPrint(statusBar, 0.0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar info."));

   ///////////////////////////////////////////////////////
   // Get names of all layers in image.
   ///////////////////////////////////////////////////////
   nameList = eimg_LayerGetNames(argv[0], eint_GetInit(), &lcl);
   EERR_CHECK(lcl, EFS0("Error getting layer names."));

   ///////////////////////////////////////////////////////
   // Open a layer stack
   ///////////////////////////////////////////////////////
   layerStack = eimg_LayerStackOpen(nameList, eint_GetInit(), 
                                    &lcl, EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error creating layer stack."));

   ///////////////////////////////////////////////////////
   // Create 1-row buffer
   ///////////////////////////////////////////////////////
   lineBuffer = eimg_PixelRectStackCreate(layerStack->count, 
                                          layerStack->layers[0]->width,
                                          1, //Height
                                          layerStack->layers[0]->pixelType,
                                          &lcl);
   EERR_CHECK(lcl, EFS0("Error creating buffer."));

   ///////////////////////////////////////////////////////
   // Create mapInfo and save map information
   ///////////////////////////////////////////////////////
   mapInfo = eprj_MapInfoCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating mapInfo structure."));

   eimg_MapInfoRead(layerStack->layers[0], mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error retrieving map information."));

   ///////////////////////////////////////////////////////
   // Set width and height variables.
   ///////////////////////////////////////////////////////
   t_width = mapInfo->pixelSize->width;
   t_height = mapInfo->pixelSize->height;

   ///////////////////////////////////////////////////////
   // Create statsInfo and save map statistics
   ///////////////////////////////////////////////////////
   statsInfo = esta_StatisticsCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating statsInfo structure."));

   eimg_StatisticsRead(layerStack->layers[0], statsInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error retrieving statistics information."));


   ///////////////////////////////////////////////////////
   // Populate 'params' array.
   ///////////////////////////////////////////////////////
   for(int counter = 1; counter <= NUM_OF_PARAMS; ++counter)
   {
      params[counter - 1] = atoi(argv[counter]) ? true : false;
   }

   outFile = fopen(argv[23], "w"); //Open file for writing
   outletCell = atoi(argv[24]);


   ///////////////////////////////////////////////////////
   // Begin writing header information.
   ///////////////////////////////////////////////////////
   fprintf(outFile, "AGNPS Version 5.0 format\n");

   fprintf(outFile, "%8i%8i%8i%8i%8i%8i%8i%8i\n",
      hflags.errorLog,
      hflags.srcAcct,
      hflags.hydrology,
      hflags.sediment,
      hflags.nutrient,
      hflags.pesticide,
      0,
      0);

   fprintf(outFile, "Watershed data\n");

   fprintf(outFile, "%i meters\n",
      int(mapInfo->pixelSize->width));

   fprintf(outFile, "%16.2f%8i%8i%8i%8i%8i%8.2f\n",
      t_width*t_height*0.000247, //Area converted from meters to acres
      statsInfo->maximum,
      statsInfo->maximum,
      hflags.hyd_calc,
      hflags.geomorph,
      hflags.k_coeff,
      hflags.prepeak);

   fprintf(outFile, "%16s%8.2f%8.1f%8.2f%8.2f\n",
      hflags.storm_type,
      hflags.energy_int,
      hflags.duration,
      hflags.precip,
      hflags.nitro_conc);

   ///////////////////////////////////////////////////////
   // Allocate memory to 'validCell' and 'data'
   ///////////////////////////////////////////////////////
   // Using 'calloc' because using 'new' causes linker errors
   //    with Imagine - multiply defined symbol
   ///////////////////////////////////////////////////////
   validCell = (bool*)calloc(t_width, sizeof(bool));
   
   data = (float**)calloc(NUM_OF_PARAMS, sizeof(float*));
   for(int counter = 0; counter < NUM_OF_PARAMS; ++counter)
   {
      data[counter] = (float*)calloc(t_width, sizeof(float));
   }

   ///////////////////////////////////////////////////////
   // Go through each row of the layerStack
   ///////////////////////////////////////////////////////
   for(int numRow = 0; numRow < t_height; ++numRow)
   {
      eimg_LayerStackRead(layerStack, 0, numRow, t_width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading layer stack."));

      /////////////////////////////////////////////////////////////////
      // Go through each cell in the row and set all the layer values.
      /////////////////////////////////////////////////////////////////
      // Parameter list:
      //    1  - Cell number - Unique Identifier
      //    2  - Cell division - Zero, no cells divided
      //    3  - Receiving Cell Number
      //    4  - Receiving Cell Division - Set to zero
      //    5  - Flow Direction
      //    6  - SCS Curve Number
      //    7  - Average Land Slope
      //    8  - Slope Shape Factor
      //    9  - Slope Length
      //    10 - Overland Manning's Coefficient
      //    11 - Soil Erodibility Factor
      //    12 - Cropping Factor
      //    13 - Practice Factor - Set to one
      //    14 - Surface Condition Constant
      //    15 - Chemical Oxygen Demand Factor (COD)
      //    16 - Soil Type
      //    17 - Fertilizer Level
      //    18 - Pesticide Type - Set to zero
      //    19 - Number of Point Sources - Set to zero
      //    20 - Additional Erosion Sources - Set to zero
      //    21 - Number of Impoundments - Set to zero
      //    22 - Type of Channel
      /////////////////////////////////////////////////////////////////
      for(int numLayer = 0; numLayer < lineBuffer->numlayers; ++numLayer)
      {
         switch(lineBuffer->datalayer[numLayer]->datatype)
         {
            case EGDA_TYPE_U8:
               t_char = (char*)&t_int;

               for(int counter = 0; 
                   counter < lineBuffer->datalayer[numLayer]->numelements; 
                   ++counter)
               {
                  t_int = 0;
                  *(t_char) = lineBuffer->datalayer[numLayer]->currentData[counter];

                  data[numLayer][counter] = t_int;
               }
               break;
            case EGDA_TYPE_U16:
               t_char = (char*)&t_int;

               for(int counter = 0; 
                   counter < lineBuffer->datalayer[numLayer]->numelements;
                   ++counter)
               {
                  t_int = 0;
                  *(t_char)   = lineBuffer->datalayer[numLayer]->currentData[counter*2];
                  *(t_char+1) = lineBuffer->datalayer[numLayer]->currentData[counter*2+1];

                  data[numLayer][counter] = t_int;
               }
               break;
            case EGDA_TYPE_U32:
               t_char = (char*)&t_int;

               for(int counter = 0;
                   counter < lineBuffer->datalayer[numLayer]->numelements;
                   ++counter)
               {
                  t_int = 0;
                  *(t_char)   = lineBuffer->datalayer[numLayer]->currentData[counter*4];
                  *(t_char+1) = lineBuffer->datalayer[numLayer]->currentData[counter*4+1];
                  *(t_char+2) = lineBuffer->datalayer[numLayer]->currentData[counter*4+2];
                  *(t_char+3) = lineBuffer->datalayer[numLayer]->currentData[counter*4+3];

                  data[numLayer][counter] = t_int;
               }
               break;
            case EGDA_TYPE_F32:
               t_char = (char*)&t_float;

               for(int counter = 0;
                   counter < lineBuffer->datalayer[numLayer]->numelements;
                   ++counter)
               {
                  t_float = 0;
                  *(t_char)   = lineBuffer->datalayer[numLayer]->currentData[counter*4+3];
                  *(t_char+1) = lineBuffer->datalayer[numLayer]->currentData[counter*4+2];
                  *(t_char+2) = lineBuffer->datalayer[numLayer]->currentData[counter*4+1];
                  *(t_char+3) = lineBuffer->datalayer[numLayer]->currentData[counter*4];

                  data[numLayer][counter] = t_float;
               }
               break;
            default:
               break;
         };
      }

      ///////////////////////////////////////////////////////
      // Populate validCell based on data type
      ///////////////////////////////////////////////////////
      switch(layerStack->layers[0]->pixelType)
      {
         case EGDA_TYPE_U8:
            for(int counter = 0; counter < t_width; ++counter)
            {
               validCell[counter] = 
                  (__int8(lineBuffer->datalayer[0]->currentData[counter]) > 0)
                           ? true : false;
            }
            break;
         case EGDA_TYPE_U16:
            t_char = (char*)&t_int;
            for(int counter = 0; counter < t_width*2; counter += 2)
            {
               t_int = 0;
               *(t_char)   = lineBuffer->datalayer[0]->currentData[counter];
               *(t_char+1) = lineBuffer->datalayer[0]->currentData[counter+1];

               validCell[counter/2] = (t_int > 0) ? true : false;
            }
            break;
         case EGDA_TYPE_U32:
            t_char = (char*)&t_int;
            for(int counter = 0; counter < t_width*4; counter += 4)
            {
               t_int = 0;
               *(t_char)   = lineBuffer->datalayer[0]->currentData[counter];
               *(t_char+1) = lineBuffer->datalayer[0]->currentData[counter+1];
               *(t_char+2) = lineBuffer->datalayer[0]->currentData[counter+2];
               *(t_char+3) = lineBuffer->datalayer[0]->currentData[counter+3];

               validCell[counter/4] = (t_int > 0) ? true : false;
            }
            break;
         case EGDA_TYPE_F32:
            t_char = (char*)&t_float;
            for(int counter = 0; counter < t_width*4; counter += 4)
            {
               *(t_char)   = lineBuffer->datalayer[0]->currentData[counter+3];
               *(t_char+1) = lineBuffer->datalayer[0]->currentData[counter+2];
               *(t_char+2) = lineBuffer->datalayer[0]->currentData[counter+1];
               *(t_char+3) = lineBuffer->datalayer[0]->currentData[counter];

               validCell[counter/4] = (t_float > 0) ? true : false;
            }
            break;
         default:
            break;
      };

      for(int numCell = 0; numCell < t_width; ++numCell)
      {
         if(validCell[numCell])
         {
            /////////////////////////////////////////////////////////////////
            // I'm going to try and come back and figure out why these 3 
            //    'if' statements are this way, and what exactly is going on.
            //    As of right now, I'm not entirely sure if the else should
            //    be there, if it needs another else, etc.
            /////////////////////////////////////////////////////////////////

            // If recieving cell is zero, set it to the current cell.
            if(data[PARAM_3][numCell] == 0)
               data[PARAM_3][numCell] = data[PARAM_1][numCell];

            // Force outlet cell to flow out
            if(data[PARAM_1][numCell] == outletCell) 
               data[PARAM_3][numCell] = OUTLET_CELL_FLOW;
            else if(data[PARAM_1][numCell] == data[PARAM_3][numCell]) 
               data[PARAM_5][numCell] = 0;//Cell is a border cell, 
                                    //   set flow direction to 0.

            // Converting parameter 5 from Arc style to ADGen
            switch(int(data[PARAM_5][numCell])) 
            {
               case ARC::E:
                  data[PARAM_5][numCell] = ADGEN::E;
                  break;
               case ARC::SE:
                  data[PARAM_5][numCell] = ADGEN::SE;
                  break;
               case ARC::S:
                  data[PARAM_5][numCell] = ADGEN::S;
                  break;
               case ARC::SW:
                  data[PARAM_5][numCell] = ADGEN::SW;
                  break;
               case ARC::W:
                  data[PARAM_5][numCell] = ADGEN::W;
                  break;
               case ARC::NW:
                  data[PARAM_5][numCell] = ADGEN::NW;
                  break;
               case ARC::N:
                  data[PARAM_5][numCell] = ADGEN::N;
                  break;
               case ARC::NE:
                  data[PARAM_5][numCell] = ADGEN::NE;
                  break;
               default:
                  *err = eerr_CreateErrorReport("formatData", (Eerr_ErrorCode)3L,
                     EERR_FATALERROR, lcl, "Error: Data value not recognized.",
                     NULL);
                  eerr_PrintErrorReport(*err, EERR_ENDUSER, eint_GetInit());
            }

            // For parameter 6, we don't want a 0.  Force a common value.
            if(data[PARAM_6][numCell] == 0)
               param6 = 55.00;
            else if(data[PARAM_16][numCell] == 0) //If it doesn't match up with water
               param6 = 100.00;
            else
               param6 = data[PARAM_6][numCell];

             // If cell is water, force other data to correspond.
            if(data[PARAM_16][numCell] == 0 || data[PARAM_22][numCell] == 0)
            {                                
               data[PARAM_16][numCell] = 0;
               data[PARAM_17][numCell] = 0;
               data[PARAM_18][numCell] = 0;
               data[PARAM_22][numCell] = 0;
            }


            //Print parameter 1
            if(params[PARAM_1])
               fprintf(outFile, "%8.0f", data[PARAM_1][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 2
            if(params[PARAM_2])
               fprintf(outFile, "%8.0f", data[PARAM_2][numCell]);
            else
               fprintf(outFile, "%8s", "000");

            //Print parameter 3
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_3][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 4
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_4][numCell]);
            else
               fprintf(outFile, "%8s", "000");

            //Print parameter 5
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_5][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 6
            if(params[PARAM_3])
               fprintf(outFile, "%8i", param6);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 7
            if(params[PARAM_3])
               fprintf(outFile, "%8.3f", data[PARAM_7][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 8
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_8][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 9
            if(params[PARAM_3])
               fprintf(outFile, "\n%16.0f", data[PARAM_9][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 10
            if(params[PARAM_3])
            {
               if(data[PARAM_16][numCell] == 0) //If it doesn't match up with 
                  fprintf(outFile, "0.99");     //  water, or just for formatting
               else                                     // (I have no idea -Kyle)
                  fprintf(outFile, "%8.3f", data[PARAM_10][numCell]);
            }
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 11
            if(params[PARAM_3])
               fprintf(outFile, "%8.2f", data[PARAM_11][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 12
            if(params[PARAM_3])
               fprintf(outFile, "%8.4f", data[PARAM_12][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 13
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_13][numCell]);
            else
               fprintf(outFile, "%8s", "1.00");

            //Print parameter 14
            if(params[PARAM_3])
               fprintf(outFile, "%8.2f", data[PARAM_14][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 15
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_15][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 16
            if(params[PARAM_3])
               fprintf(outFile, "\n%16.0f", data[PARAM_16][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 17
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_17][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 18
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_18][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 19
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_19][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 20
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_20][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 21
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_21][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            //Print parameter 22
            if(params[PARAM_3])
               fprintf(outFile, "%8.0f", data[PARAM_22][numCell]);
            else
               fprintf(outFile, "%8i", 0);

            fprintf(outFile, "\n");
         }

         ///////////////////////////////////////////////////////
         // Optional soil info
         ///////////////////////////////////////////////////////
         if(data[PARAM_16][numCell] > 0)
         {
            fprintf(outFile, "Soil:%11.4f%8.4f%8.2f%8.2f\n"
                                   "%16.3f%8.3f%8.3f%8.3f",
                          0.0010, 0.0005, 5, 2, 
                          0.05, 0.025, 0.25, 0.25);
            switch(int(data[PARAM_16][numCell]))
            {
               case SAND:
                  fprintf(outFile, "%8i\n", 1);
                  break;
               case SILT:
                  fprintf(outFile, "%8i\n", 2);
                  break;
               case CLAY:
                  fprintf(outFile, "%8i\n", 5);
                  break;
               case PEAT:
                  fprintf(outFile, "%8i\n", 20);
                  break;
            }
         }
         ///////////////////////////////////////////////////////
         // Optional fertilizer info
         ///////////////////////////////////////////////////////
         if(data[PARAM_17][numCell] > 0)
         {
            switch(int(data[PARAM_17][numCell]))
            {
               case LOW:
                  fprintf(outFile, "Fert:%11i%8i", 50, 20);
                  break;
               case AVERAGE:
                  fprintf(outFile, "Fert:%11i%8i", 100, 40);
                  break;
               case HIGH:
                  fprintf(outFile, "Fert:%11i%8i", 200, 80);
                  break;
               case OTHER: //Not sure what data means - use average values
                  fprintf(outFile, "Fert:%11i%8i", 100, 40);
                  break;
            }
            fprintf(outFile, "%8i%8i\n", 60, 60);
         }
         ///////////////////////////////////////////////////////
         // Optional channel info
         ///////////////////////////////////////////////////////
         // The people before me had no idea what this meant,
         //    and because of that, neither do I. In AGNPS 5.0
         //    Input File Format Guide, it says: "The channel
         //    information will exist for every cell in the
         //    watershed."
         ///////////////////////////////////////////////////////
         fprintf(outFile, "Channel:%8.2f%8.4f%8.4f%8.2f%8.4f%8.4f\n"
                          "%16.2f%8.3f%8.4f%8.2f%8.2f\n"
                          "%16.3f%8i%8i%8i%8i\n"
                          "%16i%8i%8i%8i%8i\n",
                 0, 3.425, 0.3151, 0, 0.4537, 0.2192,
                 0, 153, 0.6, 0.1, 1,
                 0.1, 1, 0, 0, 0,
                 1, 1, 1, 1, 1);
      }
      emet_MeterInfoPrint(statusBar, double(numRow)/t_height * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing to status bar"));
   }
   emet_MeterInfoPrint(statusBar, 100, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar"));


cleanup:

   if(outFile != NULL)
      fclose(outFile);
   outFile = NULL;
   
   if(validCell != NULL)
      free(validCell);
   validCell = NULL;

   if(data != NULL)
   {
      for(int counter = 0; counter < NUM_OF_PARAMS; ++counter)
      {
         if(data[counter] != NULL)
            free(data[counter]);
         data[counter] = NULL;
      }
      free(data);
   }
   data = NULL;

   emet_MeterInfoDelete(statusBar, &lcl);
   statusBar = NULL;

   estr_StringListDelete(nameList, &lcl);
   nameList = NULL;

   eimg_LayerStackClose(layerStack, &lcl);
   layerStack = NULL;

   eimg_PixelRectStackDelete(lineBuffer, &lcl);
   lineBuffer = NULL;

   eprj_MapInfoFree(&mapInfo);
   mapInfo = NULL;

   esta_StatisticsDelete(statsInfo, &lcl);
   statsInfo = NULL;

   return NULL;
}


///////////////////////////////////////////////////////////////////////
// Function: runAgnps
// Description: Runs the AGNPS executable.
///////////////////////////////////////////////////////////////////////
// This function was not changed - Kyle
///////////////////////////////////////////////////////////////////////
Estr_StringList* runAgnps(Eeml_Menu            menu, 
                          Emsc_Opaque*         context, 
                          long                 argc, 
                          char**               argv, 
                          Eerr_ErrorReport**   err)
{
   Estr_StringList*  rtnVal = NULL;
   long              t_long;

   EERR_INIT("runAgnps", err, lcl);

   ///////////////////////////////////////////////////////////////////////
   // Line below = temporary solution --- Must change
   ///////////////////////////////////////////////////////////////////////
   _chdir("D:\\Imagine 8.7\\bin\\ntx86");
   t_long = _spawnv(_P_WAIT, argv[0], argv);
   rtnVal = estr_LongToStringList(1, &t_long, &lcl);
   EERR_CHECK(lcl, EFS0("Can't convert 'long' to string"));

cleanup:
   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: runFlood
// Description: Runs the flood executable.
///////////////////////////////////////////////////////////////////////
// This function was not changed - Kyle
///////////////////////////////////////////////////////////////////////
Estr_StringList* runFlood(Eeml_Menu          menu, 
                          Emsc_Opaque*       context, 
                          long               argc, 
                          char**             argv, 
                          Eerr_ErrorReport** err)
{
   Estr_StringList*  rtnVal = NULL;
   long t_long;
   
   ///////////////////////////////////////////////////////////////////////
   // Line below = temporary solution --- Must change
   ///////////////////////////////////////////////////////////////////////
   _chdir("D:\\Imagine 8.7\\bin\\ntx86");
   
   EERR_INIT("runFlood", err, lcl);

   t_long = _spawnv(_P_WAIT, argv[0], argv);
   rtnVal = estr_LongToStringList(1, &t_long, &lcl);
   EERR_CHECK(lcl, EFS0("Cannot convert 'long' to string"));

cleanup:
   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: runD8
// Description: Runs the D8 executable.
///////////////////////////////////////////////////////////////////////
// This function was not changed - Kyle
///////////////////////////////////////////////////////////////////////
Estr_StringList* runD8(Eeml_Menu          menu, 
                       Emsc_Opaque*       context, 
                       long               argc, 
                       char**             argv, 
                       Eerr_ErrorReport** err)
{
   Estr_StringList*  rtnVal = NULL;
   long t_long;

   EERR_INIT("runD8", err, lcl);

   t_long = _spawnv(_P_WAIT, argv[0], argv);
   rtnVal = estr_LongToStringList(1, &t_long, &lcl);
   EERR_CHECK(lcl, EFS0("Cannot convert 'long' to string"));

cleanup:
   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: runGridnet
// Description: Runs the gridnet executable.
///////////////////////////////////////////////////////////////////////
// This function was not changed - Kyle
///////////////////////////////////////////////////////////////////////
Estr_StringList* runGridnet(Eeml_Menu          menu, 
                            Emsc_Opaque*       context, 
                            long               argc, 
                            char**             argv, 
                            Eerr_ErrorReport** err)
{
   Estr_StringList*  rtnVal = NULL;
   long t_long;

   EERR_INIT("runGridnet", err, lcl);

   t_long = _spawnv(_P_WAIT, argv[0], argv);

   rtnVal = estr_LongToStringList(1, &t_long, &lcl);
   EERR_CHECK(lcl, EFS0("Cannot convert 'long' to string"));

cleanup:
   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: imageCreation
// Description: Uses output from AGNPS (*.nps) to make 9 image files,
//    displaying the results of the file on the watershed.
///////////////////////////////////////////////////////////////////////
Estr_StringList* imageCreation(Eeml_Menu          menu, 
                               Emsc_Opaque*       context, 
                               long               argc, 
                               char**             argv, 
                               Eerr_ErrorReport** err)
{
   Emet_MeterInfo*         statusBar      = NULL;
   Eimg_Layer*             layer          = NULL;
   Estr_StringList*        layerName      = NULL;
   Eimg_PixelRect*         lineBuffer     = NULL;
   Eimg_PixelRectStack*    stack[NUM_OF_LAYERS]       = {0};
   Eimg_LayerStack*        layerStack[NUM_OF_LAYERS]  = {0};
   Eimg_LayerNameStack*    nameStack      = NULL;
   Eprj_MapInfo*           mapInfo        = NULL;
   Eprj_ProParameters*     projInfo       = NULL;
   Esta_Statistics*        statsInfo      = NULL;
   Egda_PixelType          pixelType      = EGDA_TYPE_INVALID;

   FILE*                   inFile         = NULL;

   long int                t_width        = 0;
   long int                t_height       = 0;
   int                     t_int          = 0;
   char                    t_char[256]    = {0};
   bool                    t_bool         = false;

   char*                   t_charp        = NULL;
   Egda_Byte*              t_bytep        = NULL;

   // Data variables
   float totalDrain, baseArea, rainfall, intensity;
   int baseCell, cellDivision;
   float volume, rate, yield;
   float sedimentNitro, totalNitro, nitroConcentration, sedimentPhospho;
   float totalPhospho, phosphoConcentration, totalCOD, CODConcentration;

   float areaErosion, gullyErosion;
   int delivery, enrichment;
   float sedimentConcentration, areaYield, sedimentYield;

   int cellNum;
   float drainArea, eqRunoff, upRunoff, upFlow;
   float downRunoff, downFlow, runoffAbove;

   float cellErosion, upSedimentYield, sedimentGenerated;
   int deposition;

   float cellNitro, nitroRunoff;

   float cellPhospho, phosphoRunoff, cellCOD;
   //////////////////////////////////////////////////////
   // Initialize error reporting.
   //////////////////////////////////////////////////////
   EERR_INIT("imageCreation", err, lcl);

   //////////////////////////////////////////////////////
   // Create status bar to show progress.
   //////////////////////////////////////////////////////
   statusBar = emet_MeterInfoCreate("Starting process", 
                                    "Convert NPS to Images", stdout, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating meter info."));

   emet_MeterInfoSet(statusBar, 0.0, 100.0, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting meter info."));

   emet_MeterInfoChangeTaskName(statusBar, "Initializing Layer Stack Images",
                                &lcl);
   EERR_CHECK(lcl, EFS0("Error changing status bar task name."));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing to status bar."));

   //////////////////////////////////////////////////////
   // Retrieve layer names
   //////////////////////////////////////////////////////
   layerName = eimg_LayerGetNames(argv[1], eint_GetInit(), &lcl);
   EERR_CHECK(lcl, EFS0("Error getting layer name."));

   //////////////////////////////////////////////////////
   // Open a layer
   //////////////////////////////////////////////////////
   layer = eimg_LayerOpen(layerName->strings[0], eint_GetInit(), &lcl,
                          EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error opening layer."));

   t_width = layer->width;
   t_height = layer->height;
   pixelType = layer->pixelType;
   //////////////////////////////////////////////////////
   // Create a buffer that will contain one row of data.
   //////////////////////////////////////////////////////
   lineBuffer = eimg_PixelRectCreate(t_width, 1, pixelType, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating buffer"));

   //////////////////////////////////////////////////////////////////////
   // Create mapInfo, projInfo, and statsInfo structures to store data.
   //////////////////////////////////////////////////////////////////////
   mapInfo = eprj_MapInfoCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating map info structure."));

   eimg_MapInfoRead(layer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading map info."));

   projInfo = eprj_ProParametersCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating proj info structure."));

   eimg_ProParametersRead(layer, projInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading projection info."));

   statsInfo = esta_StatisticsCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating stats info structure."));

   eimg_StatisticsRead(layer, statsInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading stats info."));

   ///////////////////////////////////////////////////////////////
   // Create 9 buffers to each hold one row of data from a layer.
   ///////////////////////////////////////////////////////////////
   stack[0] = eimg_PixelRectStackCreate(7, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 1"));

   stack[1] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 2"));

   stack[2] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 3"));

   stack[3] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 4"));

   stack[4] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 5"));

   stack[5] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 6"));

   stack[6] = eimg_PixelRectStackCreate(5, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 7"));

   stack[7] = eimg_PixelRectStackCreate(6, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 8"));

   stack[8] = eimg_PixelRectStackCreate(8, t_width, 1, EGDA_TYPE_F32, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating stack 9"));

   //////////////////////////////////////////////////////
   // Open a file for reading.
   //////////////////////////////////////////////////////
   inFile = fopen(argv[0], "r");

   //////////////////////////////////////////////////////
   // Create 9 layerstacks.  These are the image files
   //    that will be the final product.
   //////////////////////////////////////////////////////
   
   //////////////////////////////////////////////////////
   // Loop to create each of the layerstacks
   //    0 = Hydro
   //    1 = Clay
   //    2 = Silt
   //    3 = SAGG
   //    4 = LAGG
   //    5 = Sand
   //    6 = Total
   //    7 = Nitro
   //    8 = Phospho
   //////////////////////////////////////////////////////
   for(int layerNum = 0; layerNum < NUM_OF_LAYERS; ++layerNum)
   {
      switch(layerNum)
      {
         case 0:
            t_int = 7;
            break;
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
            t_int = 5;
            break;
         case 7:
            t_int = 6;
            break;
         case 8:
            t_int = 8;
      }
      nameStack = eimg_LayerNameStackCreate(t_int, &lcl);
      EERR_CHECK(lcl, EFS0("Error creating name stack."));

      for(int counter = 0; counter < nameStack->count; ++counter)
      {
         nameStack->strings[counter] = estr_Sprintf(NULL, "%s(:Layer_%d)", &lcl,
                                                  argv[layerNum+2], counter+1);
         EERR_CHECK(lcl, EFS0("Error printing names"));
      }

      //////////////////////////////////////////////////////
      // Create the layerstack
      //////////////////////////////////////////////////////
      layerStack[layerNum] = eimg_LayerStackCreate(nameStack, 
                                         EIMG_ATHEMATIC_LAYER, EGDA_TYPE_F32, 
                                         t_width, t_height, eint_GetInit(), &lcl,
                                         EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
                                         EIMG_LAYER_OPTION_END);
      EERR_CHECK(lcl, EFS0("Error creating layer stack."));

      //////////////////////////////////////////////////////
      // Clear nameStack
      //////////////////////////////////////////////////////
      eimg_LayerNameStackDelete(nameStack, &lcl);
      nameStack = NULL;

      emet_MeterInfoPrint(statusBar, double(layerNum+1)/NUM_OF_LAYERS * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing meter data."));
   }
   ///////////////////////////Layer stacks created///////////////////////////

   emet_MeterInfoChangeTaskName(statusBar, 
                  "Converting NPS file to Imagine images... (1 of 2)", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing meter name"));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing progress info."));

   //////////////////////////////////////////////////////
   // Discard useless header info
   //////////////////////////////////////////////////////
   fgets(t_char, 256, inFile);
   fgets(t_char, 256, inFile);
   fgets(t_char, 256, inFile);
   fgets(t_char, 256, inFile);

   //////////////////////////////////////////////////////
   // Collect information
   //////////////////////////////////////////////////////
   fscanf(inFile, "%8f.3%f.2%f.2%f.2", &totalDrain, &baseArea, 
                                        &rainfall, &intensity);
   fscanf(inFile, "%i%i", &baseCell, &cellDivision);
   fscanf(inFile, "%f.2%f.2%f.2", &volume, &rate, &yield);
   fscanf(inFile, "%f.2%f.2%f.2%f.2", &sedimentNitro, &totalNitro,
                                      &nitroConcentration, &sedimentPhospho);
   fscanf(inFile, "%f.2%f.2%f.2%f.2", &totalPhospho, &phosphoConcentration,
                                       &totalCOD, &CODConcentration);

   //////////////////////////////////////////////////////
   // Sediment data
   //////////////////////////////////////////////////////
   fscanf(inFile, "%s", t_char);
   fscanf(inFile, "%s", t_char);
   for(int counter = 0; counter < 6; ++counter)
   {
      fscanf(inFile, "%f.2%f.2%i%i%f.2%f.2%f.2",
                     &areaErosion, &gullyErosion,
                     &delivery, &enrichment,
                     &sedimentConcentration,
                     &areaYield, &sedimentYield);
   }

   fscanf(inFile, "%s", t_char);
   fscanf(inFile, "%s", t_char);
   
   for(int numRow = 0; numRow < t_height; ++numRow)
   {
      eimg_LayerRead(layer, 0, numRow, t_width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading layer"));

      for(int numCell = 0; numCell < t_width; ++numCell)
      {
         t_bool = false;
         switch(layer->pixelType)
         {
            case EGDA_TYPE_U8:
               if(int(lineBuffer->currentData[numCell]) > 0)
                  t_bool = true;
               break;
            case EGDA_TYPE_U16:
               if(int(lineBuffer->currentData[numCell*2] | 
                      lineBuffer->currentData[numCell*2+1]) > 0)
                  t_bool = true;
               break;
            case EGDA_TYPE_U32:
               if(int(lineBuffer->currentData[numCell*4] |
                      lineBuffer->currentData[numCell*4+1] |
                      lineBuffer->currentData[numCell*4+2] |
                      lineBuffer->currentData[numCell*4+3]) > 0)
                  t_bool = true;
               break;
         }
         if(t_bool)
         {
            /////////////////Hydro data/////////////////
            fscanf(inFile, "%i%i%f.2%f.2%f.2%f.2%f.2%f.2%f.1", 
                     &cellNum, &cellDivision, &drainArea, &eqRunoff,
                     &upRunoff, &upFlow, &downRunoff, &downFlow, &runoffAbove);
            moveData((stack[0]->datalayer[0]->currentData)+numCell*4, 
                                                   (char*)&drainArea, 4, true);
            moveData((stack[0]->datalayer[1]->currentData)+numCell*4,
                                                   (char*)&eqRunoff, 4, true);
            moveData((stack[0]->datalayer[2]->currentData)+numCell*4,
                                                   (char*)&upRunoff, 4, true);
            moveData((stack[0]->datalayer[3]->currentData)+numCell*4,
                                                   (char*)&upFlow, 4, true);
            moveData((stack[0]->datalayer[4]->currentData)+numCell*4,
                                                   (char*)&downRunoff, 4, true);
            moveData((stack[0]->datalayer[5]->currentData)+numCell*4,
                                                   (char*)&downFlow, 4, true);
            moveData((stack[0]->datalayer[6]->currentData)+numCell*4,
                                                   (char*)&runoffAbove, 4, true);

            //////////////////////////////////////////////////////
            // Loop to create layers 2 through 7 (indices 1-6)
            //////////////////////////////////////////////////////
            for(int stackNum = 1; stackNum < 7; ++stackNum)
            {
               fscanf(inFile, "%f.2%f.2%f.2%f.2%i",
                           &cellErosion, &upSedimentYield, &sedimentGenerated, 
                           &sedimentYield, &deposition);
               moveData((stack[stackNum]->datalayer[0]->currentData)+numCell*4, 
                                               (char*)&cellErosion, 4, true);
               moveData((stack[stackNum]->datalayer[1]->currentData)+numCell*4,
                                               (char*)&upSedimentYield, 4, true);
               moveData((stack[stackNum]->datalayer[2]->currentData)+numCell*4,
                                               (char*)&sedimentGenerated, 4, true);
               moveData((stack[stackNum]->datalayer[3]->currentData)+numCell*4,
                                               (char*)&sedimentYield, 4, true);
               moveData((stack[stackNum]->datalayer[4]->currentData)+numCell*4,
                                               (char*)&deposition, 4, true);

            }
         }
         else // No valid data
         {
            for(int stackNum = 0; stackNum < 7; ++stackNum)
               for(int dataNum = 0; dataNum < 7; ++dataNum)
                  for(int indexNum = 0; indexNum < 4; ++indexNum)
                     stack[stackNum]->datalayer[dataNum]->currentData[numCell*4 + indexNum] = 0;
         }
      } // End numCell loop

      //////////////////////////////////////////////////////
      // Write the rows stored to the layers.
      //////////////////////////////////////////////////////
      for(int numStack = 0; numStack < 7; ++numStack)
      {
         eimg_LayerStackWrite(layerStack[numStack], 0, numRow, t_width, 1, stack[numStack], &lcl);
         EERR_CHECK(lcl, EFS0("Failed writing layer"));
      }
      //////////////////////////////////////////////////////
      // Update status bar
      //////////////////////////////////////////////////////
      emet_MeterInfoPrint(statusBar, double(numRow)/t_height * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error print status bar info."))
   } // End numRow loop

   //////////////////////////////////////////////////////
   // Close first layer and reopen for layers 8 & 9
   //////////////////////////////////////////////////////
   eimg_LayerClose(layer, &lcl);
   layer = eimg_LayerOpen(layerName->strings[7], eint_GetInit(), &lcl, 
                           EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error opening layer"));

   eimg_LayerRead(layer, 0, 0, t_width, 1, lineBuffer, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading layer"));

   emet_MeterInfoChangeTaskName(statusBar, 
                  "Converting NPS file to Imagine images... (2 of 2)", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing task name"));

   fscanf(inFile, "%s%s", t_char, t_char);

   for(int numRow = 0; numRow < t_height; ++numRow)
   {
      //////////////////////////////////////////////////////
      // Read a row of data
      //////////////////////////////////////////////////////
      eimg_LayerRead(layer, 0, numRow, t_width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading layer"));

      for(int numCell = 0; numCell < t_width; ++numCell)
      {
         t_bool = false;
         switch(layer->pixelType)
         {
            case EGDA_TYPE_U8:
               if(int(lineBuffer->currentData[numCell]) > 0)
                  t_bool = true;
               break;
            case EGDA_TYPE_U16:
               if(int(lineBuffer->currentData[numCell*2] | 
                      lineBuffer->currentData[numCell*2+1]) > 0)
                  t_bool = true;
               break;
            case EGDA_TYPE_U32:
               if(int(lineBuffer->currentData[numCell*4] |
                      lineBuffer->currentData[numCell*4+1] |
                      lineBuffer->currentData[numCell*4+2] |
                      lineBuffer->currentData[numCell*4+3]) > 0)
                  t_bool = true;
               break;
         }
         if(t_bool)
         {
            /////////////////Nitrogen data/////////////////
            fscanf(inFile, "%i%i%f.2%f.2%f.2%f.2%f.2%f.2",
                        &cellNum, &cellDivision, 
                        &drainArea, &cellNitro,
                        &sedimentNitro, &nitroRunoff, 
                        &totalNitro, &nitroConcentration);
            moveData((stack[7]->datalayer[0]->currentData)+numCell*4, 
                                          (char*)&drainArea, 4, true);
            moveData((stack[7]->datalayer[1]->currentData)+numCell*4,
                                          (char*)&cellNitro, 4, true);
            moveData((stack[7]->datalayer[2]->currentData)+numCell*4,
                                          (char*)&sedimentNitro, 4, true);
            moveData((stack[7]->datalayer[3]->currentData)+numCell*4,
                                          (char*)&nitroRunoff, 4, true);
            moveData((stack[7]->datalayer[4]->currentData)+numCell*4,
                                          (char*)&totalNitro, 4, true);
            moveData((stack[7]->datalayer[5]->currentData)+numCell*4,
                                          (char*)&nitroConcentration, 4, true);

            /////////////////Phosphorus and COD data/////////////////
            fscanf(inFile, "%f.2%f.2%f.2%f.2%f.2%f.2%f.2%f.2",
                           &cellPhospho, &sedimentPhospho, &phosphoRunoff, &totalPhospho,
                           &phosphoConcentration, &cellCOD, &totalCOD, &CODConcentration);
            moveData((stack[8]->datalayer[0]->currentData)+numCell*4, 
                                          (char*)&cellPhospho, 4, true);
            moveData((stack[8]->datalayer[1]->currentData)+numCell*4,
                                          (char*)&sedimentPhospho, 4, true);
            moveData((stack[8]->datalayer[2]->currentData)+numCell*4,
                                          (char*)&phosphoRunoff, 4, true);
            moveData((stack[8]->datalayer[3]->currentData)+numCell*4,
                                          (char*)&totalPhospho, 4, true);
            moveData((stack[8]->datalayer[4]->currentData)+numCell*4,
                                          (char*)&phosphoConcentration, 4, true);
            moveData((stack[8]->datalayer[5]->currentData)+numCell*4,
                                          (char*)&cellCOD, 4, true);
            moveData((stack[8]->datalayer[6]->currentData)+numCell*4,
                                          (char*)&totalCOD, 4, true);
            moveData((stack[8]->datalayer[7]->currentData)+numCell*4,
                                          (char*)&CODConcentration, 4, true);
         }
         else // Cells don't have data
         {
            for(int stackNum = 7; stackNum < 9; ++stackNum)
               for(int dataNum = 0; dataNum < 5; ++dataNum)
                  for(int indexNum = 0; indexNum < 4; ++indexNum)
                     stack[stackNum]->datalayer[dataNum]->currentData[numCell*4 + indexNum] = 0;
            for(int indexNum = 0; indexNum < 4; ++indexNum)
            {
               stack[8]->datalayer[6]->currentData[numCell*4 + indexNum] = 0;
               stack[8]->datalayer[7]->currentData[numCell*4 + indexNum] = 0;
            }
         }
      }
      //////////////////////////////////////////////////////
      // Write the row of data
      //////////////////////////////////////////////////////
      eimg_LayerStackWrite(layerStack[7], 0, numRow, t_width, 1, stack[7], &lcl);
      EERR_CHECK(lcl, EFS0("Error writing to layerstack"));

      eimg_LayerStackWrite(layerStack[8], 0, numRow, t_width, 1, stack[8], &lcl);
      EERR_CHECK(lcl, EFS0("Error writing to layerstack"));

      //////////////////////////////////////////////////////
      // Update status bar
      //////////////////////////////////////////////////////
      emet_MeterInfoPrint(statusBar, double(numRow)/t_height * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing to status bar"));
   } // End numRow loop
   emet_MeterInfoChangeTaskName(statusBar, 
                  "Writing map and projection information...", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing task name."));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status"));

   //////////////////////////////////////////////////////
   // Write map information for each layer
   //////////////////////////////////////////////////////
   for(int layerStackNum = 0; layerStackNum < 9; ++layerStackNum)
   {
      for(int layerNum = 0; layerNum < layerStack[layerStackNum]->count; ++layerNum)
      {
         eimg_MapInfoWrite(layerStack[layerStackNum]->layers[layerNum], mapInfo, &lcl);
         EERR_CHECK(lcl, EFS0("Error writing map information"));

         eimg_ProParametersWrite(layerStack[layerStackNum]->layers[layerNum], projInfo, &lcl);
         EERR_CHECK(lcl, EFS0("Error writing projection information"));
      }
      emet_MeterInfoPrint(statusBar, double(layerStackNum)/NUM_OF_LAYERS * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing status information"));
   }

cleanup:
   fclose(inFile);

   estr_StringListDelete(layerName, &lcl);
   layerName = NULL;

   emet_MeterInfoDelete(statusBar, &lcl);
   statusBar = NULL;

   eimg_LayerClose(layer, &lcl);
   layer = NULL;

   eimg_PixelRectDelete(lineBuffer, &lcl);
   lineBuffer = NULL;

   for(int counter = 0; counter < NUM_OF_LAYERS; ++counter)
   {
      eimg_PixelRectStackDelete(stack[counter], &lcl);
      stack[counter] = NULL;

      eimg_LayerStackClose(layerStack[counter], &lcl);
      layerStack[counter] = NULL;
   }

   eimg_LayerNameStackDelete(nameStack, &lcl);
   nameStack = NULL;

   eprj_MapInfoFree(&mapInfo);
   mapInfo = NULL;

   eprj_ProParametersFree(&projInfo);
   projInfo = NULL;

   esta_StatisticsDelete(statsInfo, &lcl);
   statsInfo = NULL;


   return NULL;
}


///////////////////////////////////////////////////////////////////////
// Format for imageToAscii and asciiToImage:
//
// ncols       -> eimg_Layer
// nrows       -> eimg_Layer
// xllcorner   -> eprj_MapInfo
// yllcorner   -> eprj_MapInfo
// cellsize    -> eprj_MapInfo
// nodata_value-> anything
// row1 val val val val val...
// row2 val val val val val...
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Function: imageToAscii
// Description: Converts an Imagine image to arc ascii format
///////////////////////////////////////////////////////////////////////
Estr_StringList* imageToAscii(Eeml_Menu            menu, 
                              Emsc_Opaque*         context, 
                              long                 argc, 
                              char**               argv, 
                              Eerr_ErrorReport**   err)
{
   Emet_MeterInfo*         statusBar   = NULL;
   Estr_StringList*        layerName   = NULL;
   Estr_StringList*        rtnVal      = NULL;
   Eimg_Layer*             layer       = NULL;
   Eimg_PixelRect*         lineBuffer  = NULL;
   Eprj_MapInfo*           mapInfo     = NULL;

   char*                   asciiFile   = NULL;
   FILE*                   outFile     = NULL;

   float                   t_float     = 0;
   //////////////////////////////////////////////////////
   // Initialize error reporting
   //////////////////////////////////////////////////////
   EERR_INIT("imageToAscii", err, lcl);

   //////////////////////////////////////////////////////
   // Create a progress bar.
   //////////////////////////////////////////////////////
   statusBar = emet_MeterInfoCreate("Starting process", "Convert Image", 
                                             stdout, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating progress bar."));

   emet_MeterInfoSet(statusBar, 0.0, 100.0, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting meter info."));

   emet_MeterInfoChangeTaskName(statusBar, 
                           "Converting image to ARC ascii format...", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing status bar."));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing to status bar."));

   //////////////////////////////////////////////////////
   // Retrieve layer names and open a layer type.
   //////////////////////////////////////////////////////
   layerName = eimg_LayerGetNames(argv[0], eint_GetInit(), &lcl);
   EERR_CHECK(lcl, EFS0("Error getting layer names."));

   layer = eimg_LayerOpen(layerName->strings[0], eint_GetInit(), &lcl,
                              EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error getting layer."));

   if(layer == NULL)
      exit(1);

   //////////////////////////////////////////////////////
   // Create a 1-row buffer
   //////////////////////////////////////////////////////
   lineBuffer = eimg_PixelRectCreate(layer->width, 1, layer->pixelType, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating buffer."));

   //////////////////////////////////////////////////////
   // Save map information
   //////////////////////////////////////////////////////
   mapInfo  = eprj_MapInfoCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating map info."));

   eimg_LayerRead(layer, 0, 0, layer->width, 1, lineBuffer, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading layer"));

   eimg_MapInfoRead(layer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading map info."));

   ///////////////////////////////////////////////////////////////////////////
   // Create the new file by changing the image in the input file to ascii.
   ///////////////////////////////////////////////////////////////////////////
   asciiFile = (char*)malloc(128);

   if(strlen(argv[0]) > 127)
   {
      *err = eerr_CreateErrorReport("imageToAscii", (Eerr_ErrorCode)3L,
                                    EERR_FATALERROR, lcl, 
                                    "Error: filename was too long.\n"
                                    "File names must be less than 128"
                                    " characters", NULL);
      eerr_PrintErrorReport(*err, EERR_ENDUSER, eint_GetInit());
   }
   strcpy(asciiFile, argv[0]);
   strncpy(asciiFile + strlen(asciiFile) - 4, ".asc", 4);

   outFile = fopen(asciiFile, "w");
   errno;
   
   if(outFile == NULL) exit(1);
   //////////////////////////////////////////////////////
   // Set the return value to be the name of the file.
   //////////////////////////////////////////////////////
   rtnVal = estr_StringListAppend(rtnVal, asciiFile, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting string list."));

   //////////////////////////////////////////////////////
   // Create file header.
   //////////////////////////////////////////////////////
   fprintf(outFile, "ncols %i\n", layer->width);
   fprintf(outFile, "nrows %i\n", layer->height);
   fprintf(outFile, "xllcorner %f\n", mapInfo->upperLeftCenter->x);
   fprintf(outFile, "yllcorner %f\n", mapInfo->lowerRightCenter->y);
   fprintf(outFile, "cellsize %i\n", mapInfo->pixelSize->width);
   fprintf(outFile, "nodata_value -1\n");

   for(int numRow = 0; numRow < layer->height; ++numRow)
   {
      eimg_LayerRead(layer, 0, numRow, layer->width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error reading from layer."));

      for(int numCell = 0; numCell < layer->width; ++numCell)
      {
         switch(layer->pixelType)
         {
            case EGDA_TYPE_U8:
               fprintf(outFile, "%i ", lineBuffer->currentData[numCell]);
               break;
            case EGDA_TYPE_U16:
               fprintf(outFile, "%i ", 
                  lineBuffer->currentData[numCell*2] | 
                  lineBuffer->currentData[numCell*2+1]<<8);
               break;
            case EGDA_TYPE_U32:
               fprintf(outFile, "%i ",
                  lineBuffer->currentData[numCell*4]       |
                  lineBuffer->currentData[numCell*4+1]<<8  |
                  lineBuffer->currentData[numCell*4+2]<<16 |
                  lineBuffer->currentData[numCell*4+3]<<24);
               break;
            case EGDA_TYPE_F32:
               t_float = 0;
               moveData((char*)&t_float, &lineBuffer->currentData[numCell*4], 4, true);
               fprintf(outFile, "%f ", t_float);
               break;
            default:
               *err = eerr_CreateErrorReport("imageToAscii", (Eerr_ErrorCode)3L,
                           EERR_FATALERROR, lcl, "Error: Datatype not supported.", NULL);
               eerr_PrintErrorReport(*err, EERR_ENDUSER, eint_GetInit());
         }
         fprintf(outFile, "\n");

         emet_MeterInfoPrint(statusBar, double(numRow)/layer->height * 100, &lcl);
         EERR_CHECK(lcl, EFS0("Error printing status info."));
      }
      emet_MeterInfoPrint(statusBar, 100, &lcl);
      EERR_CHECK(lcl, EFS0("Erorr printing to status bar."));

      fflush(outFile);
   }

cleanup:
   fclose(outFile);
   free(asciiFile);
   asciiFile = NULL;

   emet_MeterInfoDelete(statusBar, &lcl);
   statusBar = NULL;

   estr_StringListDelete(layerName, &lcl);
   layerName = NULL;

   eimg_LayerClose(layer, &lcl);
   layer = NULL;

   eimg_PixelRectDelete(lineBuffer, &lcl);
   lineBuffer = NULL;

   eprj_MapInfoFree(&mapInfo);
   mapInfo = NULL;
   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: asciiToImage
// Description: Creates an Imagine image from a tardem ascii file.
///////////////////////////////////////////////////////////////////////
Estr_StringList* asciiToImage(Eeml_Menu            menu, 
                              Emsc_Opaque*         context, 
                              long                 argc, 
                              char**               argv, 
                              Eerr_ErrorReport**   err)
{
   Emet_MeterInfo*         statusBar   = NULL;
   Estr_StringList*        layerName   = NULL;
   Estr_StringList*        rtnVal      = NULL;
   Eimg_Layer*             layer       = NULL;
   Eimg_Layer*             newLayer    = NULL;
   Eimg_PixelRect*         lineBuffer  = NULL;
   Eprj_MapInfo*           mapInfo     = NULL;
   Eprj_ProParameters*     projInfo    = NULL;
   Esta_Statistics*        statsInfo   = NULL;

   FILE*                   inFile            = NULL;
   char*                   pathname          = NULL;
   char*                   filename          = NULL;
   char*                   file              = NULL;
   char*                   imageFile         = NULL;
   char*                   newLayerName      = NULL;
   long int                nodata_value;
   long int                data;

   char                    c_temp[5000];

   //////////////////////////////////////////////////////
   // Initialize error reporting
   //////////////////////////////////////////////////////
   EERR_INIT("asciiToImage", err, lcl);

   //////////////////////////////////////////////////////
   // Create a progress bar
   //////////////////////////////////////////////////////
   statusBar = emet_MeterInfoCreate("Starting process", 
                        "Convert ASCII to Imagine image.", stdout, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating status bar."));

   emet_MeterInfoSet(statusBar, 0, 100, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting status bar."));

   emet_MeterInfoChangeTaskName(statusBar, "Initializing...", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing task name."));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing to status bar."));

   //////////////////////////////////////////////////////
   // Get layer names and retrieve a layer.
   //////////////////////////////////////////////////////
   layerName = eimg_LayerGetNames(argv[1], eint_GetInit(), &lcl);
   EERR_CHECK(lcl, EFS0("Error getting layer names."));

   layer = eimg_LayerOpen(layerName->strings[0], eint_GetInit(), &lcl,
                           EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error opening layer."));

   //////////////////////////////////////////////////////
   // Save map, projection, and statistics info.
   //////////////////////////////////////////////////////
   mapInfo = eprj_MapInfoCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating map info structure."));

   eimg_MapInfoRead(layer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading map info."));

   projInfo = eprj_ProParametersCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating projection info structure."));

   eimg_ProParametersRead(layer, projInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading projection info."));

   statsInfo = esta_StatisticsCreate(&lcl);
   EERR_CHECK(lcl, EFS0("Error creating statistics info structure."));

   eimg_StatisticsRead(layer, statsInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error reading statistics."));

   //////////////////////////////////////////////////////
   // Create a buffer to hold a row of data.
   //////////////////////////////////////////////////////
   lineBuffer = eimg_PixelRectCreate(layer->width, 1, EGDA_TYPE_U8, &lcl);
   EERR_CHECK(lcl, EFS0("Error creating pixel rect."));

   //////////////////////////////////////////////////////
   // Open the ascii file for reading.
   //////////////////////////////////////////////////////
   inFile = fopen(argv[0], "r");
   if(!inFile) exit(1);

   imageFile      = (char*)malloc(1024); 
   pathname       = (char*)malloc(128); 
   filename       = (char*)malloc(64); 
   newLayerName   = (char*)malloc(500); 
   file           = (char*)malloc(150);

   //////////////////////////////////////////////////////
   // Get path and filename needed for layer creation
   //////////////////////////////////////////////////////
   estr_GetPathPart(argv[0], pathname, 127);
   estr_GetNamePart(argv[0], filename, 63);

   strncpy(filename + strlen(filename) - 4, ".img", 4);

   //////////////////////////////////////////////////////
   // If the file exists, delete it.
   //////////////////////////////////////////////////////
   strcpy(file, pathname);
   strcat(file, filename);

   _unlink(file);

   //////////////////////////////////////////////////////
   // Set the layer's name.
   //////////////////////////////////////////////////////
   sprintf(newLayerName, "%s%s(:Layer_1)\0", pathname, filename);

   int t_width = layer->width;
   int t_height = layer->height;
   Eint_InitToolkitData* blah = eint_GetInit();
   newLayer = eimg_LayerCreate(newLayerName, EIMG_ATHEMATIC_LAYER, EGDA_TYPE_U8, 
                              t_width, t_height,
                              blah, err,
                              EIMG_LAYER_OPTION_COMPUTE_STATS_ON_CLOSE,
                              EIMG_LAYER_OPTION_END);
   EERR_CHECK(lcl, EFS0("Error creating layer."));

   emet_MeterInfoChangeTaskName(statusBar, 
                           "Converting ascii file to Imagine image...", &lcl);
   EERR_CHECK(lcl, EFS0("Error changing status bar name."));

   emet_MeterInfoPrint(statusBar, 0, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing to status bar."));

   strncpy(imageFile, argv[0], 191);
   strncpy(imageFile + strlen(imageFile) - 4, ".img", 192 - strlen(imageFile) + 4);
   rtnVal = estr_StringListAppend(rtnVal, imageFile, &lcl);
   EERR_CHECK(lcl, EFS0("Error setting string."));

   //////////////////////////////////////////////////////
   // We will already have the header info from the DEM
   //////////////////////////////////////////////////////
   fgets(c_temp, 255, inFile);
   fgets(c_temp, 255, inFile);
   fgets(c_temp, 255, inFile);
   fgets(c_temp, 255, inFile);
   fgets(c_temp, 255, inFile);

   //////////////////////////////////////////////////////
   // Get nodata_value
   //////////////////////////////////////////////////////
   fscanf(inFile, "%s", c_temp);
   fscanf(inFile, "%i", &nodata_value);

   //////////////////////////////////////////////////////
   // Go through each cell
   //////////////////////////////////////////////////////
   for(int numRow = 0; numRow < layer->height; ++numRow)
   {
      for(int numCell = 0; numCell < layer->width; ++numCell)
      {
         fscanf(inFile, "%i", &data);

         if(data != nodata_value)
            lineBuffer->currentData[numCell] = data;
         else
            lineBuffer->currentData[numCell] = 1;
      }

      eimg_LayerWrite(newLayer, 0, numRow, layer->width, 1, lineBuffer, &lcl);
      EERR_CHECK(lcl, EFS0("Error writing to layer."));

      emet_MeterInfoPrint(statusBar, double(numRow)/layer->height * 100, &lcl);
      EERR_CHECK(lcl, EFS0("Error printing status bar."));
   }
   emet_MeterInfoPrint(statusBar, 100, &lcl);
   EERR_CHECK(lcl, EFS0("Error printing status bar."));

   //////////////////////////////////////////////////////
   // Put info from the DEM into the new image.
   //////////////////////////////////////////////////////
   eimg_MapInfoWrite(newLayer, mapInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error writing map info."));

   eimg_ProParametersWrite(newLayer, projInfo, &lcl);
   EERR_CHECK(lcl, EFS0("Error writing projection info."));

cleanup:
   fclose(inFile);
   inFile = NULL;

   free(pathname);
   free(filename);
   free(newLayerName);
   free(file);
   free(imageFile);
   

   emet_MeterInfoDelete(statusBar, &lcl);
   statusBar = NULL;

   estr_StringListDelete(layerName, &lcl);
   layerName = NULL;

   eimg_LayerClose(layer, &lcl);
   layer = NULL;

   eimg_LayerClose(newLayer, &lcl);
   newLayer = NULL;

   eimg_PixelRectDelete(lineBuffer, &lcl);
   lineBuffer = NULL;

   eprj_MapInfoFree(&mapInfo);
   mapInfo = NULL;

   eprj_ProParametersFree(&projInfo);
   projInfo = NULL;

   esta_StatisticsDelete(statsInfo, &lcl);
   statsInfo = NULL;

   return rtnVal;
}

///////////////////////////////////////////////////////////////////////
// Function: deleteFiles
// Description: renames any existing image files to include "_copy" so
//    that Imagine will not error when it sees that they are already
//    created.  If files with "_copy" already exist, they are deleted.
///////////////////////////////////////////////////////////////////////
Estr_StringList* deleteFiles(Eeml_Menu            menu, 
                             Emsc_Opaque*         context, 
                             long                 argc, 
                             char**               argv, 
                             Eerr_ErrorReport**   err)
{
   char*    path = argv[0];
   char*    old_Name;
   char*    new_Name;
   char*    temp;
   int      error = errno;

   old_Name = (char*)malloc(MAX_PATH_SIZE);
   new_Name = (char*)malloc(MAX_PATH_SIZE + 5);

   for(int counter = 1; counter <= 22; ++counter)
   {
      sprintf(old_Name, "%sparameter", path);
      if(counter < 10) sprintf(old_Name, "%s0", old_Name);
      sprintf(old_Name, "%s%i", old_Name, counter);
      
      sprintf(new_Name, "%s_copy.img", old_Name);
      sprintf(old_Name, "%s.img", old_Name);

      _unlink(new_Name);
      rename(old_Name, new_Name);
   }

   sprintf(old_Name, "%sImageStack.img", path);
   _unlink(old_Name);

   free(old_Name);
   free(new_Name);
   
   errno = error;
   return NULL;
}








void moveData(char *dest, const Egda_Byte *src, int numBytes, bool backwards)
{
   for(int counter = 0; counter < numBytes; ++counter)
   {
      if(backwards)
         *(dest + counter) = *(src + (numBytes - 1) - counter);
      else
         *(dest + counter) = *(src + counter);
   }
}
void moveData(Egda_Byte* dest, const char* src, int numBytes, bool backwards)
{
   for(int counter = 0; counter < numBytes; ++counter)
   {
      if(backwards)
         *(dest + counter) = *(src + (numBytes - 1) - counter);
      else
         *(dest + counter) = *(src + counter);
   }
}
