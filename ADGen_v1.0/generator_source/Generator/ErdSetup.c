/* 
** Include application header 
*/
#include "ErdSetup.h"

/*****************************************************************************
** Phase 1 : Generator
*****************************************************************************
** FUNCTION NAME
**  main 
** 
** SYNTAX
**
**  main( int argc, char **argv )
**
** ARGUMENTS
**
**  int   argc;
**  char  **argv;
**
** DESCRIPTION
**
**  This is the main application and will parse the eml and set up all
** interaction with the interface. In this phase the application will 
** launch the eml and that is all. There will be no function incorporated 
** into the interface. This application is associated with Generator.eml. 
**
** RETURN VALUE
**
**  None 
**
**
******************************************************************************/

/*
** Global Variables
*/
Eeml_TranslationTable menufunctions[] = 
{ 
	"dismiss",		dismissFunc,
	"cells",		numberCells,
	"format",		formatData,
	"executor",		runAgnps,
	"flood",		runFlood,
	"flow",			runD8,
	"stream",		runGridnet,
	"makeimage",	imageCreation,
	"img2ascii",	imageToAscii,
	"ascii2img",	asciiToImage,
	NULL, NULL, 
};


/*
** Main
*/
main( argc, argv )
     int argc;
     char  **argv;
{
  
	char	  *applicationName =  "AGNPS Data Generator";
	int                                n;
	Emsc_Opaque	              *context;
	char           **buf = (char **)NULL;
	AppContext	           *app = NULL;
	Eerr_ErrorReport         *err;

    Eint_InitToolkitData *erdinit = NULL; 
	/* 
	** Initialize Error macro
	*/
	EERR_INIT( "main", &err, lcl );
  
	/* 
	** Initialize the ERDAS toolkit
	*/
	erdinit=eint_InitToolkit((Emsc_Opaque **)&lcl );
	ES0( lcl, "eint_InitToolkit Failed");
  
	/* 
	** Create and initialize the application context structure
	*/
	app = emsc_New( 1, AppContext );
  
  
  
	/* 
	** Initilize the ERDAS EML Toolkit Package 
	*/
	app->root = eeml_Init( eint_GetInit(), applicationName, argc, argv, &lcl );
	ES0( lcl, "eint_Init");
  
	/* 
	** Parse out the eml with the frame parts 
	 */
	app->result = eeml_ParseVa(ELEX_FILE, "Generator.eml",
					app->root->rootPart, &lcl,
					EEML_PARSE_OPTION_APP_FUNCTIONS, menufunctions,
					EEML_PARSE_OPTION_APP_CONTEXT, (Emsc_Opaque *)app,
					NULL );
	ES0( lcl, "eeml_ParseVa");

	app->frame = eeml_FindPart(app->result, "mainframe", &lcl);
	EERR_CHECK(lcl, EFS0("FindPart mainframe"));
	
  
	/* 
	** Sets up the command to listen to session manager.
	** Exits application on doneFlag.
	*/
	while (!app->root->doneFlag) 
	{
		n = 0;
		eeml_GetNextCommand(app->root, &context, &n, &buf, &lcl );
		EERR_CHECK(lcl , EFS0( "GetNextCommand!" ));
    
		/*
		** We are not interested in any of the commands here, 
		** so free them
		*/
		eeml_FreeCommand(n, buf, &lcl);
		EERR_CHECK(lcl , EFS0( "FreeCommand!" ));
	}

cleanup:
  
	return 0;
}
