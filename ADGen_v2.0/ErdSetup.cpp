#include "ErdSetup.h"

///////////////////////////////////////////////////////////////////////
// Main function
// Description: The main application will parse the eml provided in 
//    'GeneratorV2.eml' and will set up interactions with the interface.
//    The application will launch the eml and wait.
///////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////
Eeml_TranslationTable menufunctions[] = 
{ 
   "dismiss"      ,     dismissFunc    ,
   "cells"        ,     numberCells    ,
   "format"       ,     formatData     ,
   "executor"     ,     runAgnps       ,
   "flood"        ,     runFlood       ,
   "flow"         ,     runD8          ,
   "stream"       ,     runGridnet     ,
   "makeimage"    ,     imageCreation  ,
   "img2ascii"    ,     imageToAscii   ,
   "ascii2img"    ,     asciiToImage   ,
   "deleteFiles"  ,     deleteFiles    ,
   
   NULL           ,     NULL           , 
};




//////////////////////////////////////////////////
// Main application - ADGen
//////////////////////////////////////////////////
int main(int argc, char *argv[])
{
   char                 *applicationName  = "AGNPS Data Generator";
   char                 **buffer          = NULL;
   int                  counter           = 0;
   AppContext           *app              = NULL;
   Emsc_Opaque          *context          = NULL;
   Eerr_ErrorReport     *err              = NULL;
   Eint_InitToolkitData *erdInit          = NULL;

   //////////////////////////////////////////////////
   // Initialization
   //////////////////////////////////////////////////
   EERR_INIT("main", &err, lcl);                         //Error Macro

   erdInit = eint_InitToolkit((Emsc_Opaque**) &lcl);     //Toolkit
   ES0(lcl, "Failed to initialize the toolkit");

   app = emsc_New(1, AppContext);                        //Context Structure

                                                         //EML Toolkit package
   app->root = eeml_Init(eint_GetInit(), applicationName, argc, argv, &lcl);
   ES0(lcl, "Failed to initialize EML toolkit package");

   //////////////////////////////////////////////////
   // Parse EML with frame parts
   //////////////////////////////////////////////////
   app->result = eeml_ParseVa(
                        ELEX_FILE,                          "GeneratorV2.eml",
                        app->root->rootPart,                &lcl, 
                        EEML_PARSE_OPTION_APP_FUNCTIONS,    menufunctions, 
                        EEML_PARSE_OPTION_APP_CONTEXT,      (Emsc_Opaque *)app,
                        NULL);
   ES0(lcl, "Failed to parse EML");

   app->frame = eeml_FindPart(app->result, "mainframe", &lcl); // Load
   EERR_CHECK(lcl, EFS0("Couldn't find 'mainframe' in EML"));  // 'mainframe'

   //////////////////////////////////////////////////
   // Listen to the session manager for next command
   // Exit when doneFlag is true
   //////////////////////////////////////////////////
   while(!app->root->doneFlag)
   {
      counter = 0;
      eeml_GetNextCommand(app->root, &context, &counter, &buffer, &lcl);
      EERR_CHECK(lcl, EFS0("Couldn't get next command"));

      ///////////////////////////////////////////////////////////////////
      // We are not interested in the rest of the commands, so free them
      ///////////////////////////////////////////////////////////////////

      eeml_FreeCommand(counter, buffer, &lcl);
      EERR_CHECK(lcl, EFS0("Couldn't free command"));
   }

cleanup:

   return 0;
}














