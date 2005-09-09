#ifndef ERDSETUP_H
#define ERDSETUP_H

//////////////////////////////////////////////////
// Toolkit Includes
//////////////////////////////////////////////////
#include <eint.h>
#include <eerr.h>
#include <eeml.h>
#include <eeml_frame.h>
#include <emsc.h>
#include <estr.h>
#include <eeml_dialog.h>
#include <efga.h>
#include <eeml_part.h>
#include <eimg.h>
#include <esta.h>
#include <eprj.h>
#include <egda.h>

//////////////////////////////////////////////////
// Standard Includes
//////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////
// Error Macros
//////////////////////////////////////////////////
#define EFS0(message) EERR_FATAL_SET_0( __LINE__,message)
#define ES0(error,message) EERR_SHOW_0(error,EERR_DEBUG,__LINE__,message)

//////////////////////////////////////////////////
// Structs
//////////////////////////////////////////////////
struct AppContext
{
   // frame parts //
   Eui_Root          *root;
   Eml_ParseResult   *result;
   Eui_BasePart      *frame;

   char imgLayerName[ESTR_MAX_PATH_LEN];
};


struct HeaderFlags
{
   /* Line 2 */
   int errorLog;
   int srcAcct;
   int hydrology;
   int sediment;
   int nutrient;
   int pesticide;

   /* Line 5 */
   int hyd_calc;
   int geomorph;
   int k_coeff;
   double prepeak;

   /* Line 6 */
   char storm_type[5];
   double energy_int;
   double duration;
   double precip;
   double nitro_conc;
};

//////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////
static HeaderFlags hflags;
Eeml_ApplicationFunction dismissFunc;
Eeml_ApplicationFunction numberCells;
Eeml_ApplicationFunction formatData;
Eeml_ApplicationFunction runAgnps;
Eeml_ApplicationFunction runFlood;
Eeml_ApplicationFunction runD8;
Eeml_ApplicationFunction runGridnet;
Eeml_ApplicationFunction imageCreation;
Eeml_ApplicationFunction imageToAscii;
Eeml_ApplicationFunction asciiToImage;
Eeml_ApplicationFunction saveHeaderFlags;
Eeml_ApplicationFunction deleteFiles;

static void selectCB(Emsc_Opaque *,  Esel_SelectionTable *, Emsc_Opaque *);



#endif
