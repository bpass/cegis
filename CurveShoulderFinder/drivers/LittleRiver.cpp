/**
 * @file LittleRiver.cpp
 *
 * $Id: LittleRiver.cpp,v 1.3 2005/08/11 21:25:05 ahartman Exp $
 */

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "BSQReader.h"
#include "Point2D.h"

//#define PRINT_RESOLUTIONS
//#define PRINT_FILENAMES
//#define PRINT_SUFFIXES

//typedef std::vector<std::vector<std::string> > filenames_t;
//filenames_t createFilenames();
std::map<std::string, size_t> createFileTypesMap();
void littleRiverPrintPoints();

//enum FileTypes { clay = 0, hydro, lagg, nitro, 
//                 phospho, sagg, sand, silt, total };
const std::map<std::string, size_t> fileTypes = createFileTypesMap();

int main()
{
    std::cout << std::fixed;
    std::cout.precision(4);
    littleRiverPrintPoints();
}

std::map<std::string, size_t>
createFileTypesMap()
{
    const std::string fileTypes[] =
    {
        "clay",
        "hydro",
        "lagg",
        "nitro",
        "phospho",
        "sagg",
        "sand",
        "silt",
        "total"
    };
    const size_t numFileTypes = sizeof(fileTypes) / sizeof(std::string);

    std::map<std::string, size_t> fileTypesMap;
    for(size_t i = 0; i < numFileTypes; ++i)
    {
        fileTypesMap.insert(make_pair(fileTypes[i], i));
    }
    return fileTypesMap;
}

void littleRiverPrintPoints()
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

#ifdef PRINT_RESOLUTIONS
    // Test to make sure the resolutions were set up correctly
    std::cout << "Resolutions:\n";
    for(std::vector<int>::const_iterator i = resolutions.begin();
        i != resolutions.end(); ++i)
    {
        std::cout << '\t' << *i << '\n';
    }
#endif

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

#ifdef PRINT_SUFFIXES
    // Test to make sure the suffixes were set up correctly
    std::cout << "Suffixes:\n";
    for(std::vector<std::string>::const_iterator i = suffixes.begin();
        i != suffixes.end(); ++i)
    {
        std::cout << '\t' << *i << '\n';
    }
#endif

    const std::string extension = ".bsq";

    // Set up all the filenames
    typedef std::vector<std::vector<std::string> > filenames_t;
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

#ifdef PRINT_FILENAMES
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
#endif

    // Read the data from various points at each resolution
    typedef float Data_t;
    typedef BSQReader<Data_t> BSQReader_t;
    typedef Point2D<BSQReader_t::UTMCoordinateType> Point_t;
    typedef std::vector<Point_t> Points_t;

    // Set up the points we want to read
    Points_t points;
    points.push_back(Point_t(249064.067797, 3499322.525424));

    // Set up the data files we want to read
    const std::string myFileTypes[] = { "clay", "nitro" };
    const size_t numMyFileTypes = sizeof(myFileTypes) / sizeof(myFileTypes[0]);

    // Set up the bands we want to read
    typedef std::vector<size_t> BandsList_t;
    typedef std::map<std::string, BandsList_t> Bands_t;
    Bands_t myBands;

    const size_t myClayBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t myClayBands(
                      myClayBandsAsArray, myClayBandsAsArray + 
                      sizeof(myClayBandsAsArray) / sizeof(size_t));

    const size_t myNitroBandsAsArray[] = { 1, 2, 3, 4, 5, 6};
    const BandsList_t myNitroBands(
                      myNitroBandsAsArray, myNitroBandsAsArray + 
                      sizeof(myNitroBandsAsArray) / sizeof(size_t));

    myBands["clay"] = myClayBands;
    myBands["nitro"] = myNitroBands;

    // Create the BSQReaders for the files we want
    std::vector<std::vector<BSQReader_t> > bsqReaders(numResolutions);
    for(size_t i = 0; i < numResolutions; ++i)
    {
        for(size_t j = 0; j < numMyFileTypes; ++j)
        {
            const size_t indexInFilenames = 
                (fileTypes.find(myFileTypes[j]))->second;
            bsqReaders[i].push_back
                (BSQReader_t(filenames[i][indexInFilenames]));
        }
    }

    for(size_t myFileType = 0; myFileType < numMyFileTypes; ++myFileType)
    {
        std::cout << "Values for " << myFileTypes[myFileType] << ":\n";
        for(Points_t::const_iterator point = points.begin();
            point != points.end(); ++point)
        {
            std::cout << "\tValues at " << *point << '\n';

            const BandsList_t& bands = 
                myBands.find(myFileTypes[myFileType])->second;
            for(BandsList_t::const_iterator band = bands.begin(); 
                band != bands.end(); ++band)
            {
                std::cout << "\t\tValues for band " << *band << '\n';

                for(size_t resolution = 0; resolution < numResolutions; 
                    ++resolution)
                {
                    const Data_t value = 
                        bsqReaders[resolution][myFileType].
                            getValue(point->x(), point->y(), *band);
                    std::cout << "\t\t\t" << std::setw(4) 
                              << resolutions[resolution]
                              << ": " << value << '\n';
                }
            }
        }
    }
}

