/**
 * @file LittleRiver.cpp
 *
 * $Id: LittleRiver.cpp,v 1.1 2005/08/09 19:19:13 ahartman Exp $
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "BSQReader.h"
#include "Point2D.h"

typedef std::vector<std::vector<std::string> > filenames_t;
filenames_t createFilenames();
void littleRiverPrintPoints();

int main()
{
    littleRiverPrintPoints();
}

filenames_t createFilenames()
{
    // Set up the main data directory
    const std::string dataDir = "D:/Data/Little_River/";

    // Set up the numbers for each resolution that will be part of each of
    // folder and file names
    const int resolutionsAsArray[] = 
        { 30, 60, 120, 210, 240, 480, 960, 1920 };
    const size_t numResolutions = 
        sizeof(resolutionsAsArray) / sizeof(int);
    const std::vector<int> 
        resolutions(resolutionsAsArray, resolutionsAsArray + numResolutions);

    // Test to make sure the resolutions were set up correctly
    std::cout << "Resolutions:\n";
    for(std::vector<int>::const_iterator i = resolutions.begin();
        i != resolutions.end(); ++i)
    {
        std::cout << '\t' << *i << '\n';
    }

    const std::string prefix = "n";

    // Set up the suffixes for each filename
    const std::string suffixesAsArray[] =
    {
        "little_clay",
        "little_hydro",
        "little_lagg",
        "little_nitro",
        "little_phospho",
        "little_sagg",
        "little_sand",
        "little_silt",
        "little_total"
    };
    const size_t numSuffixes = sizeof(suffixesAsArray) / sizeof(std::string);
    const std::vector<std::string> 
        suffixes(suffixesAsArray, suffixesAsArray + numSuffixes);

    // Test to make sure the suffixes were set up correctly
    std::cout << "Suffixes:\n";
    for(std::vector<std::string>::const_iterator i = suffixes.begin();
        i != suffixes.end(); ++i)
    {
        std::cout << '\t' << *i << '\n';
    }

    const std::string extension = ".bsq";

    // Set up all the filenames
    filenames_t filenames(numResolutions, 
                          std::vector<std::string>(numSuffixes));
    for(size_t i = 0; i < numResolutions; ++i)
    {
        std::ostringstream oss;
        oss << resolutions[i];
        const std::string resolutionAsString = oss.str();
        for(size_t j = 0; j < numSuffixes; ++j)
        {
            filenames[i][j] = dataDir + resolutionAsString + '/' + 
                              prefix + resolutionAsString + suffixes[j] +
                              extension;
        }
    }

    // Test to see if the filenames were set up correctly
    std::cout << "Filenames:\n";
    for(size_t i = 0; i < filenames.size(); ++i)
    {
        for(size_t j = 0; j < filenames[i].size(); ++j)
        {
            std::cout << "\t\"" << filenames[i][j] << "\"\n";
        }
    }
    std::cout.flush();

    return filenames;
}

void littleRiverPrintPoints()
{
    filenames_t filenames = createFilenames();

    typedef float Data_t;
    typedef Point2D<BSQReader<Data_t>::UTMCoordinateType> Point_t;
    typedef std::vector<Point_t> Points_t;
    Points_t points;
    points.push_back(Point_t(249064.067797, 3499322.525424));
}

