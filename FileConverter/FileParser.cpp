/* 
 * Programmer: Ryan Stelzleni
 * Date: 6-3-04
 *
 * Purpose: This file contains the implementation of a class that is intended
 *          to parse several types of input files and create an output file.
 */

#include "stdafx.h"
#include "FileParser.h"

/* This constructor is the main entry point for the code in this class.
 * The arguments are an input file name, and output file name and a 
 * double offset value that is only used if the input file is a comma
 * separated value file.
 * Pre Conditions: InputFile should contain the name of a file that exists
 *                 dOffset only needs to contain an offset if the input
 *                 file is a .csv file.
 * Post Condition: The file named OutputFile is created and contains the
 *                 parsed version of InputFile.
 */
FileParser::FileParser(char *InputFile, char *OutputFile, double &dOffset)
: type( NONE ),
input( InputFile ),
output( OutputFile ),
badFile(false),
unrecognizedFile(false),
dOffset(dOffset)
{
   if( !input || !output || input.eof() )
   {
      badFile = true;
   }
   else
   {
      DetermineFileType();
      InitInputFile();

      switch(type)
      {
      case NUM:
         ConvertNumType();
         break;
      case DAT:
         ConvertDatType();
         break;
      case CSV:
         ConvertCsvType();
         break;
      case NONE:
         unrecognizedFile = true;
      }
   }

   input.close();
   output.close();
}


/* This function should be called by an already instantiated object (obviously)
 * and will return the status of the parse operation.
 * Return Values: This can return FileParser::GOOD for success,
 *                FileParser::BADFILENAME for an error opening either the
 *                input or output file, or FileParser::UNRECOGNIZED if there
 *                was an error with the format of the file to be parsed.
 */
int FileParser::WasSuccess()
{
   if(badFile)
      return BADFILENAME;
   else if(unrecognizedFile)
      return UNRECOGNIZED;
   
   return GOOD;
}

/* This function takes an already initialized .csv file and converts it
 * into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 */
void FileParser::ConvertCsvType()
{
   double dVal= 0.0f;
   char szTemp[64];
   std::string temp;

   while( GetNextLine() && !theString.empty() )
   {
      if( theVector.size() != 5 )
      {
         unrecognizedFile = true;
         break;
      }

      if( theVector[0].size() == 1 )
      {
         theVector[0].insert( theVector[0].begin(), '0' );
      }
      output <<theVector[0] <<',' <<theVector[1] <<',' <<theVector[2] 
             <<',';
      
      dVal = atof( theVector[3].c_str() );
      dVal = dOffset - dVal;
      sprintf(szTemp, "%.2f", dVal);
      temp.assign( szTemp );

      output <<temp <<',' <<theVector[4] <<std::endl;   
   }

   return;
}

/* This function takes an already initialized .dat file and converts it
 * into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 */
void FileParser::ConvertDatType()
{
   while( GetNextLine() && !theString.empty() )
   {
      if( theVector.size() != 6 )
      {
         unrecognizedFile = true;
         break;
      }

      std::string temp(theVector[0], 0, 7);
      temp.append(theVector[0], 9, 3);

      output <<temp <<" " <<rawInput.substr(13) <<std::endl;
   }
   return;
}


/* This function takes an already initialized .dat file that contains 
 * only numbers and converts it into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 */
void FileParser::ConvertNumType()
{
   double Total = 0, Average = 0;
   char szTemp[64];
   std::string temp;

   while( GetNextLine() && !theString.empty() )
   {
      if( theVector.size() != 25 )
      {
         unrecognizedFile = true;
         break;
      }
      Total = 0;
      output <<theVector[0] <<" " <<theVector[1] <<" "
             <<theVector[2].substr(2,2) <<"  " <<theVector[3]
             <<" " <<theVector[4] <<" " <<theVector[5] <<"  ";
      Total += atof( theVector[13].c_str() );
      output <<theVector[13] <<"  ";
      Total += atof( theVector[17].c_str() );
      output <<theVector[17] <<"  ";
      Total += atof( theVector[21].c_str() );
      output <<theVector[21] <<"  ";

      Average = Total / 3.0f;
      sprintf(szTemp, "%.3f", Average);
      temp.assign(szTemp);
      output <<temp <<std::endl;
   }
   return;
}

/* This function seeks to a location in a starting file where the next 
 * GetNextLine will be data.  The function will echo everything it reads
 * up to the data into the output file.
 * Pre Condition: the file should be open and DetermineFileTypes should
 *                have already been called.
 * Post Condition: the file will be ready to read data from and theVector
 *                 will contain the parsed string immediately before the 
 *                 data.
 */
void FileParser::InitInputFile()
{
   std::string stopSeekWord = "";
   input.seekg(0, std::ios::beg);

   switch(type)
   {
   case DAT:
      stopSeekWord = "\"Date\"";
      break;
   case CSV:
      stopSeekWord = "No.";
   }
   
   /* if there is a word to seek and we aren't at the end of the file */
   if( !stopSeekWord.empty() && GetNextLine() )
   {
      do
      {
         output <<rawInput<<std::endl;
      }while( stopSeekWord != theVector[0] && GetNextLine() );
   }
   return;
}

/* This function populates type with a code to indicate what type of file
 * it is.
 * Pre Condition: the input file should be open before calling 
 *                this function.
 * Post Condition: type will have a meaningful value
 */
void FileParser::DetermineFileType()
{
   do
   {
      std::getline(input, theString, '\n');
   }while(theString == "");
   TrimWhiteSpace();
   ExtractValues();
   type = FirstWord(theVector[0]);

   return;
}


/* This function returns the numeric code for the type of file that starts
 * with the word passed in.
 * Return Value: NONE if the word isn't recognized, DAT for "WaterLOG, 
 *               CSV for Instrument and NUM if word is a number.
 */
int FileParser::FirstWord(const std::string &word) const
{
   int type = NONE;

   if(word == "\"WaterLOG")
      type = DAT;
   else if(word == "Instrument")
      type = CSV;
   else if( IsNumber(word) )
      type = NUM;

   return type;
}


/* Return Value: true if word contains only digits */
bool FileParser::IsNumber(const std::string &word) const
{
   bool isNum = true;
   for(int i=0; i<word.size(); ++i)
      if(isdigit(word[i]) == 0)
         isNum = false;
   return isNum;
}
      

/* This function changes the input string so that it has at most one space
 * between any two non-whitespace characters.  It ignores line 
 * breaks and removes all tabs.
 * Return value: returns the number of spaces removed from the string.
 */
int FileParser::TrimWhiteSpace()
{
   int count = 0;
   char lastChar = '?';
   std::string output;

   for(int i=0; i<theString.size(); ++i)
   {
      /* Replace tabs with spaces */
      if(theString[i] == '\t')
         theString[i] = ' ';

      /* Remove any two adjacent spaces */
      if(!(lastChar == ' ' && theString[i] == ' '))
      {
         output += theString[i];
         ++count;
         lastChar = theString[i];
      }
   }

   count = theString.size() - count;
   theString.assign(output);
   return count;
}

/* This function finds each instance of gone in the input string and replaces
 * it with here.
 * Return value: returns the number of replacements performed.  A tab will be
 *               reported as one replacement.
 */
int FileParser::FindAndReplace(const char gone, const char here)
{
   int count = 0;
   size_t loc = theString.find(gone);

   while(loc != std::string::npos)
   {
      ++count;
      theString.replace(loc, 1, 1, here);
      loc = theString.find(gone, loc);
   }

   return count;
}

/* This function takes a string of single space separated tokens and splits
 * it up into a vector of tokens.
 * Pre Condition: The theString member variable should contain tokens 
 *                separated by single spaces and should have no more than 
 *                one leading space.
 * Post Condition: The theVector member variable will contain all of the 
 *                 tokens from the input string.
 */
void FileParser::ExtractValues()
{   
   size_t loc = 0, lastloc = 0;
   const char delim = ' ';

   theVector.clear();

   /* Skip one leading whitespace if it's there */
   if(theString[lastloc] == delim)
   {
      ++lastloc;
      ++loc;
   }

   /* Parse out each space separated token */
   loc = theString.find(delim, loc);
   while(loc != std::string::npos)
   {
      theVector.push_back( theString.substr(lastloc, loc-lastloc) );
      lastloc = loc + 1;
      loc = theString.find(delim, lastloc);
   }
   theVector.push_back( theString.substr(lastloc, loc-lastloc) );

   return;
}


/* Reads in the next line or returns false if you are at the end of the file.
 * Pre Condition: the input file should be open
 * Post Condition: theVector contains all the tokens from the next input line
 *                 in the input file.
 * Return Value: Returns false when the end of the file has been reached.
 */
bool FileParser::GetNextLine()
{
   bool ret = input.eof();

   if( !ret )
   {
      std::getline(input, rawInput, '\n');
      theString.assign(rawInput);

      /* Remove commas to simplify parsing (replace with spaces) */
      FindAndReplace(',', ' ');
      TrimWhiteSpace();
      ExtractValues();
   }

   return !ret;
}
