// $Id: main.cpp,v 1.21 2007/08/31 14:55:38 oschnarr Exp $


#include <iostream>
using namespace std;

/*	This program is very simple. Now that you (should) have compiled and run it
	please do the following (see Jason or Austin if you have any troubles):


	1) Add your name to the list in the cout statement below

	2) Compile and run the program to make sure that it works as expected

	Windows Users:
		3) Go to where you have main.cpp stored, right-click on it and 
			choose "CVS Diff..." 

		4) When prompted for an external diff program, you can select WinDiff.
			This is usually found in C:\Program Files\Microsoft Visual Studio\Common\Tools\
			or WinMerge if you have installed it

		5) Review the diff and make sure that what it displays makes "sense"

		6) Right-click on the FOLDER containing main.cpp and choose "CVS Update"
			This will get the newest version of all files in the project and merge them
			with yours

		7) Right-click on the FOLDER containing main.cpp and choose "CVS Commit..."
			This will commit your file changes to the repository

		8) When the checkin dialog appears enter a comment and click ok

	Linux Users:
		3a) Run Cervisia choose File -> Open Sandbox; then point it at the folder that
			contains main.cpp

		4a) Right-click on main.cpp and choose "Difference to Repository (BASE)"
			Review the diff and make sure it makes "sense"

		5a) In Cervisia choose File -> Update
			This will get the newest version of all files in the project and merge them
			with yours

		6a) In Cervisia choose File -> Commit
			This will commit your file changes to the repository

		7a) When the checkin dialog appears enter a comment and click ok

*/



int main()
{
   cout << "CVS Demo\n";
   cout << "Others who have checked out:\n"
        << "\tJason Trent\n"
        << "\tAustin Hartman\n"
        << "\tRyan Stelzleni\n"
        << "\tJames Muo\n"
        << "\tBob Buehler\n"
        << "\tJimmy Hoffa\n"
        << "\tMike Williams\n"
  	    << "\tKyle Williams\n"
        << "\tMark Schisler\n"
		<< "\tJosh Wilkerson\n"
		<< "\tThe Notorious P.A.T.\n";
		<< "\tOtto Schnarr the III.\n";

   return 0;
}
