/**
 * @file LittleRiver.cpp
 *
 * $Id: LittleRiver.cpp,v 1.14 2005/09/29 23:47:46 ahartman Exp $
 */

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ImgReader.h"
#include "Point2D.h"

//#define PRINT_RESOLUTIONS
//#define PRINT_FILENAMES
//#define PRINT_SUFFIXES
//#define PAUSE_AT_END

typedef unsigned int Resolution_t;
typedef std::vector<Resolution_t> Resolutions_t;
typedef std::vector<std::vector<std::string> > Filenames_t;

Resolutions_t createResolutions();
Filenames_t createFilenames();
std::map<std::string, size_t> createFileTypesMap();
void littleRiverPrintPoints();

const std::map<std::string, size_t> fileTypes = createFileTypesMap();

int main()
{
    std::cout << std::fixed;
    std::cout.precision(4);
    littleRiverPrintPoints();

#ifdef PAUSE_AT_END
    std::cout << "Press ENTER to continue...";
    std::string blah;
    std::getline(std::cin, blah);
#endif
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

Resolutions_t
createResolutions()
{
    // Set up the numbers for each resolution that will be part of each of
    // folder and file names
    const Resolutions_t::value_type resolutionsAsArray[] = 
        { 30, 60, 120, 210, 240, 480, 960, 1920 };
    const size_t numResolutions = 
        sizeof(resolutionsAsArray) / sizeof(Resolutions_t::value_type);
    Resolutions_t resolutions(resolutionsAsArray, 
                              resolutionsAsArray + numResolutions);

#ifdef PRINT_RESOLUTIONS
    // Test to make sure the resolutions were set up correctly
    std::cout << "Resolutions:\n";
    for(Resolutions_t::const_iterator i = resolutions.begin();
        i != resolutions.end(); ++i)
    {
        std::cout << '\t' << *i << '\n';
    }
#endif

    return resolutions;
}

Filenames_t
createFilenames()
{
    // Set up the main data directory
#ifdef WIN32
    const std::string dataDir = "L:/sdir_snap/AGNPSOutputAnalysis/Data/LittleRiver/NAWQA/Output/2005-08-17/7.30in/";
#else
    const std::string dataDir = "/snap/AGNPSOutputAnalysis/Data/LittleRiver/NAWQA/Output/2005-08-17/7.30in/";
#endif

    const Resolutions_t resolutions = createResolutions();
    const std::string prefix = "lr";

    // Set up the suffixes for each filename
    const std::string suffixesAsArray[] =
    {
        "_clay",
        "_hydro",
        "_largeagg",
        "_nitro",
        "_phospho",
        "_sand",
        "_silt",
        "_smallagg",
        "_total"
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

    const std::string extension = ".img";

    // Set up all the filenames
    Filenames_t filenames(resolutions.size(), 
                          std::vector<std::string>(numSuffixes));
    for(size_t i = 0; i < resolutions.size(); ++i)
    {
        std::ostringstream oss;
        oss << resolutions[i];
        const std::string resolutionAsString = oss.str();
        for(size_t j = 0; j < numSuffixes; ++j)
        {
            filenames[i][j] = dataDir + prefix + resolutionAsString +
                              suffixes[j] + extension;
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

    return filenames;
}

void littleRiverPrintPoints()
{
    // Read the data from various points at each resolution
    typedef float Data_t;
    typedef ImgReader<Data_t> ImgReader_t;
    typedef Point2D<ImgReader_t::UTMCoordinateType> Point_t;
    typedef std::vector<Point_t> Points_t;

    // Set up the points we want to read
    Points_t points;
    points.push_back(Point_t(244338.17235, 3509943.05796));
    points.push_back(Point_t(243876.45384, 3509519.99026));
    points.push_back(Point_t(244925.32144, 3507531.60852));
    points.push_back(Point_t(250344.48237, 3499648.11321));
    points.push_back(Point_t(241920.86317, 3514708.05963));
    points.push_back(Point_t(256122.17418, 3487125.20807));
    points.push_back(Point_t(254514.00120, 3485869.88143));
    points.push_back(Point_t(254419.30251, 3489880.48902));
//    points.push_back(Point_t(249064.067797, 3499322.525424));
//    points.push_back(Point_t(254786.440678, 3485895.745763));
//    points.push_back(Point_t(237540.000000, 3516595.420200));
//    points.push_back(Point_t(260580.000000, 3516595.420200));
//    points.push_back(Point_t(237540.000000, 3485911.000000));
//    points.push_back(Point_t(260580.000000, 3485911.000000));

    // Set up the data files we want to read
//    const std::string myFileTypes[] = { "clay", "hydro", "largeagg", "nitro",
//                                        "phospho", "sand", "silt", "smallagg",
//                                        "total" };
    const std::string myFileTypes[] = { "hydro" };
    const size_t numMyFileTypes = sizeof(myFileTypes) / sizeof(myFileTypes[0]);

    // Set up the bands we want to read
    typedef std::vector<size_t> BandsList_t;
    typedef std::map<std::string, BandsList_t> Bands_t;
    Bands_t myBands;

    const size_t myClayBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t myClayBands(
                      myClayBandsAsArray, myClayBandsAsArray + 
                      sizeof(myClayBandsAsArray) / sizeof(size_t));

    const size_t myHydroBandsAsArray[] = { 1, 2, 3, 4, 5, 6, 7 };
    const BandsList_t myHydroBands(
                      myHydroBandsAsArray, myHydroBandsAsArray + 
                      sizeof(myHydroBandsAsArray) / sizeof(size_t));

    const size_t myLargeAggBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t myLargeAggBands(
                      myLargeAggBandsAsArray, myLargeAggBandsAsArray + 
                      sizeof(myLargeAggBandsAsArray) / sizeof(size_t));

    const size_t myNitroBandsAsArray[] = { 1, 2, 3, 4, 5, 6 };
    const BandsList_t myNitroBands(
                      myNitroBandsAsArray, myNitroBandsAsArray + 
                      sizeof(myNitroBandsAsArray) / sizeof(size_t));

    const size_t myPhosphoBandsAsArray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const BandsList_t myPhosphoBands(
                      myPhosphoBandsAsArray, myPhosphoBandsAsArray + 
                      sizeof(myPhosphoBandsAsArray) / sizeof(size_t));

    const size_t mySandBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t mySandBands(
                      mySandBandsAsArray, mySandBandsAsArray + 
                      sizeof(mySandBandsAsArray) / sizeof(size_t));

    const size_t mySiltBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t mySiltBands(
                      mySiltBandsAsArray, mySiltBandsAsArray + 
                      sizeof(mySiltBandsAsArray) / sizeof(size_t));

    const size_t mySmallAggBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t mySmallAggBands(
                      mySmallAggBandsAsArray, mySmallAggBandsAsArray + 
                      sizeof(mySmallAggBandsAsArray) / sizeof(size_t));

    const size_t myTotalBandsAsArray[] = { 1, 2, 3, 4, 5 };
    const BandsList_t myTotalBands(
                      myTotalBandsAsArray, myTotalBandsAsArray + 
                      sizeof(myTotalBandsAsArray) / sizeof(size_t));

    myBands["clay"] = myClayBands;
    myBands["hydro"] = myHydroBands;
    myBands["largeagg"] = myLargeAggBands;
    myBands["nitro"] = myNitroBands;
    myBands["phospho"] = myPhosphoBands;
    myBands["sand"] = mySandBands;
    myBands["silt"] = mySiltBands;
    myBands["smallagg"] = mySmallAggBands;
    myBands["total"] = myTotalBands;

    // Create the ImgReaders for the files we want
    const Resolutions_t resolutions = createResolutions();
    const Filenames_t filenames = createFilenames();
    std::vector<std::vector<ImgReader_t> > imgReaders(resolutions.size());
    for(size_t i = 0; i < resolutions.size(); ++i)
    {
        for(size_t j = 0; j < numMyFileTypes; ++j)
        {
            const size_t indexInFilenames = 
                (fileTypes.find(myFileTypes[j]))->second;
            imgReaders[i].push_back
                (ImgReader_t(filenames[i][indexInFilenames]));
        }
    }

    for(size_t myFileType = 0; myFileType < numMyFileTypes; ++myFileType)
    {
        std::cout << "Values for " << myFileTypes[myFileType] << '\n';
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

                for(size_t resIndex = 0; resIndex < resolutions.size(); 
                    ++resIndex)
                {
                    const Resolution_t resolution = resolutions[resIndex];
                    std::cout << "\t\t\t" << std::setw(4) 
                              << resolution << ": ";
                    try
                    {
                        const Data_t value = 
                            imgReaders[resIndex][myFileType].
                                getValue(point->x(), point->y(), *band);
                        std::cout << std::setw(10) 
                                  << value
                                  << std::setw(10) 
                                  << value / resolution
                                  << std::setw(10) 
                                  << value / (resolution * resolution)
                                  << '\n';
                    }
                    catch(...)
                    {
                        std::cout << "Error getting value\n";
                    }
                }
            }
        }
    }
}

