// classifier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ImageClassifier.h"
#include "ImageClassException.h"
#include <stdlib.h>
#include <string>
int _tmain(int argc, _TCHAR* argv[])
{
	int numClasses = 5;
	
	if(argc < 3) {
		printf("\nUsage: classifier inputFilename dataType [numClasses] \n");
		printf("[] denotes optional parameters\n");
		printf("outputFilename refers to the xml file that the classification data is saved to\n");
		printf("If no filename is specified, output will be saved to \"report.xml\"\n");
		printf("\nAccepted dataType values are:\n");
		printf("Unsigned 8-bit Data -> \"u8\"\n");
		printf("Signed 8-bit Data -> \"8\"\n");
		printf("Unsigned 16-bit Data -> \"u16\"\n");
		printf("Signed 16-bit Data -> \"16\"\n");
		printf("Unsigned 32-bit Data -> \"u32\"\n");
		printf("Signed 32-bit Data -> \"32\"\n");
		printf("32-bit Floating Point Data -> \"float\"\n");
		printf("64-bit Floating Point Data -> \"double\"\n");
		return(0);
	}

	
	try {
		
		//build xml filename
		std::string outputFilename(argv[1]);
		outputFilename.append(".xml");

		if(argc == 4)
			numClasses = atoi(argv[3]);
				
		if(strcmp(argv[2], "u8") == 0) {
			ImageClassifier<unsigned char> classifier(argv[1], numClasses);
			
			classifier.classify();
			classifier.printReport();
			classifier.setClassColor(2, 0xF0, 0xA2, 0x34);
			classifier.saveReport(outputFilename.c_str());
			
		}
		
		else if(strcmp(argv[2], "u16") == 0) {
			ImageClassifier<unsigned int> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}

		else if(strcmp(argv[2], "16")==0) {
			ImageClassifier<int> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}

		else if(strcmp(argv[2], "u32")==0) {
			ImageClassifier<unsigned long> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}

		else if(strcmp(argv[2], "32")==0) {
			ImageClassifier<long> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}

		else if(strcmp(argv[2], "float64")==0) {
			ImageClassifier<double> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}
		else if(strcmp(argv[2], "float") == 0) {
			ImageClassifier<float> classifier(argv[1], numClasses);
			classifier.classify();
			classifier.printReport();
			classifier.saveReport(outputFilename.c_str());
		}

		else {
			printf("\nInvalid data type. Accepted values are:\n");
			printf("Unsigned 8-bit Data -> \"u8\"\n");
			printf("Signed 8-bit Data -> \"8\"\n");
			printf("Unsigned 16-bit Data -> \"u16\"\n");
			printf("Signed 16-bit Data -> \"16\"\n");
			printf("Unsigned 32-bit Data -> \"u32\"\n");
			printf("Signed 32-bit Data -> \"32\"\n");
			printf("32-bit Floating Point Data -> \"float\"\n");
			printf("64-bit Floating Point Data -> \"double\"\n");
		}
		return(0);
	}

	catch(ImageClassException e) {
		e.printMessage();
	}
	return(0);
}

