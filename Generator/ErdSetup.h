/*
** Toolkit Includes
*/

#ifndef ERDSETUP_H
#define ERDSETUP_H

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

#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>


/*
** Error Macros 
*/
#define EFS0(m) EERR_FATAL_SET_0( __LINE__,m)
#define ES0(e,m) EERR_SHOW_0(e,EERR_DEBUG,__LINE__,m)

/* 
** Definition of structure
*/
typedef struct 
{
    /* frame parts */	  
	Eui_Root			*root;
	Eml_ParseResult		*result;
	Eui_BasePart		*frame;

	char imgLayerName[ESTR_MAX_PATH_LEN];
 
}AppContext;

/*
** Declare Globals 
*/

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
Eeml_ApplicationFunction getAttributes;

static void selectCB(Emsc_Opaque *,  Esel_SelectionTable *, Emsc_Opaque *);

#endif