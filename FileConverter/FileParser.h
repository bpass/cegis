/* 
 * Programmer: Ryan Stelzleni
 * Date: 6-3-04
 *
 * Purpose: This file contains the definition of a class that is intended
 *          to parse several types of input files and create an output file.
 *          For a detailed description of it's methods see the end of this 
 *          file.
 */

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include <fstream>

class FileParser
{
public:
   FileParser(char *InputFile, char *OutputFile, double &dOffset);
   int WasSuccess();

   static enum {GOOD, BADFILENAME, UNRECOGNIZED};
private:
   enum {NONE, NUM, DAT, CSV};

   int type;
   double dOffset;
   std::ifstream input;
   std::ofstream output;
   std::string rawInput;
   std::string theString;
   std::vector<std::string> theVector;
   bool unrecognizedFile;
   bool badFile;

   void ConvertNumType();
   void ConvertDatType();
   void ConvertCsvType();
   void DetermineFileType();
   bool IsNumber(const std::string &word) const;
   int FirstWord(const std::string &word) const;
   int TrimWhiteSpace();
   int FindAndReplace(const char gone, const char here);
   void ExtractValues();
   bool GetNextLine();
   void InitInputFile();
};

#endif

/*
 * The FileParser class currently handles three file types, *.csv Levelogger 
 * generated files, *.dat ezq files that contain all numbers, and *.dat 
 * WaterLog sensor generated files.
 *
 ************************  PUBLIC MEMBERS
 *
 *** FileParser(char *InputFile, char *OutputFile, double &dOffset);
 * This constructor is the main entry point for the code in this class.
 * The arguments are an input file name, and output file name and a 
 * double offset value that is only used if the input file is a comma
 * separated value file.
 * Pre Conditions: InputFile should contain the name of a file that exists
 *                 dOffset only needs to contain an offset if the input
 *                 file is a .csv file.
 * Post Condition: The file named OutputFile is created and contains the
 *                 parsed version of InputFile.
 *
 *** int WasSuccess();
 * This function should be called by an already instantiated object (obviously)
 * and will return the status of the parse operation.
 * Return Values: This can return FileParser::GOOD for success,
 *                FileParser::BADFILENAME for an error opening either the
 *                input or output file, or FileParser::UNRECOGNIZED if there
 *                was an error with the format of the file to be parsed.
 *
 *
 ************************  PRIVATE MEMBERS
 *
 *** void ConvertNumType();
 * This function takes an already initialized .dat file that contains 
 * only numbers and converts it into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 *
 *** void ConvertDatType();
 * This function takes an already initialized .dat file and converts it
 * into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 *
 *** void ConvertCsvType();
 * This function takes an already initialized .csv file and converts it
 * into it's output format.
 * Pre Condition: InitInputFile() should have been called already.
 * Post Condition: The file has either been parsed or it was 
 *                 unrecognizable.
 *
 *** void DetermineFileType();
 * This function populates type with a code to indicate what type of file
 * it is.
 * Pre Condition: the input file should be open before calling 
 *                this function.
 * Post Condition: type will have a meaningful value
 *
 *** bool IsNumber(const std::string &word) const;
 * Return Value: true if word contains only digits 
 *
 *** int FirstWord(const std::string &word) const;
 * This function returns the numeric code for the type of file that starts
 * with the word passed in.
 * Return Value: NONE if the word isn't recognized, DAT for "WaterLOG, 
 *               CSV for Instrument and NUM if word is a number.
 *
 *** int TrimWhiteSpace();
 * This function changes the input string so that it has at most one space
 * between any two non-whitespace characters.  It ignores line 
 * breaks and removes all tabs.
 * Return value: returns the number of spaces removed from the string.
 *
 *** int FindAndReplace(const char gone, const char here);
 * This function finds each instance of gone in the input string and replaces
 * it with here.
 * Return value: returns the number of replacements performed.  A tab will be
 *               reported as one replacement.
 *
 *** void ExtractValues();
 * This function takes a string of single space separated tokens and splits
 * it up into a vector of tokens.
 * Pre Condition: The theString member variable should contain tokens 
 *                separated by single spaces and should have no more than 
 *                one leading space.
 * Post Condition: The theVector member variable will contain all of the 
 *                 tokens from the input string.
 *
 *** bool GetNextLine();
 * Reads in the next line or returns false if you are at the end of the file.
 * Pre Condition: the input file should be open
 * Post Condition: theVector contains all the tokens from the next input line
 *                 in the input file.
 * Return Value: Returns false when the end of the file has been reached.
 *
 *** void InitInputFile();
 * This function seeks to a location in a starting file where the next 
 * GetNextLine will be data.  The function will echo everything it reads
 * up to the data into the output file.
 * Pre Condition: the file should be open and DetermineFileTypes should
 *                have already been called.
 * Post Condition: the file will be ready to read data from and theVector
 *                 will contain the parsed string immediately before the 
 *                 data.
 */
