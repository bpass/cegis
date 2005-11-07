/**
 * @file ComparePrecipitation.cpp
 * @author Austin Hartman
 * 
 * $Id: ComparePrecipitation.cpp,v 1.2 2005/11/07 17:20:32 ahartman Exp $
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ImgReader.h"
#include "Point2D.h"
#include "RasterReader.h"

typedef float Data_t;
typedef RasterReader<Data_t>::UTMCoordinateType UTMCoordinateType;

typedef Point2D<UTMCoordinateType> Point_t;
typedef std::vector<Point_t> Points_t;

typedef size_t Band_t;
typedef std::vector<Band_t> Bands_t;

typedef std::string Precipitation_t;
typedef std::vector<Precipitation_t> Precipitations_t;

typedef std::string Resolution_t;
typedef std::vector<Resolution_t> Resolutions_t;

/**
 * This class stores all the values from a single .img file for the requested
 * points and bands.
 */
class ImgValues
{
public:
    /**
     * Constructor to specify the file, points, and bands whose values
     * we want to store
     *
     * @param filename The name of the .img file to read from
     * @param points The set of points for which values will be stored
     * for access later
     * @param bands The set of bands for which values will be stored 
     * for access later
     */
    ImgValues(const std::string& filename,
              const Points_t& points,
              const Bands_t& bands);

    /**
     * Gets the value from the specified point and band.  The point and band
     * must have been part of the set that was passed into the constructor
     *
     * @param point The point at which to get data
     * @param band The band at which to get data
     *
     * @exception InvalidPoint Thrown if the specified point was not part of
     * the set passed into the constructor
     * @exception InvalidBand Thrown if the specified band was not part of
     * the set passed into the constructor
     */
    const Data_t& getValue(const Point_t& point, const Band_t& band) const;

    /**
     * Exception class to be used when the user asks for data for a point
     * that the object does not have data for
     */
    class InvalidPoint
    {};

    /**
     * Exception class to be used when the user asks for data for a band
     * that the object does not have data for
     */
    class InvalidBand
    {};

private:
    /**
     * Data is stored in a hierarchy of std::maps in following way:
     * point->band->value
     */
    //@{
    typedef std::map<Band_t, Data_t> BandMap_t;
    typedef std::map<Point_t, BandMap_t> PointMap_t;
    PointMap_t values;
    //@}
};

ImgValues::ImgValues(const std::string& filename,
                     const Points_t& points,
                     const Bands_t& bands)
{
    ImgReader<Data_t> imgReader(filename);

    // for every point, get the value at each band
    for(Points_t::const_iterator i = points.begin();
        i != points.end(); ++i)
    {
        // insert into the map of points a key for the new point and a
        // default-constructed value which will be modified next
        std::pair<ImgValues::PointMap_t::iterator, bool> pointMapInsertValue =
            values.insert(make_pair(*i, ImgValues::BandMap_t()));

        // if this was a point that had not been previously inserted into
        // the map
        if(pointMapInsertValue.second == true)
        {
            // get the band map from the iterator returned from the insertion
            ImgValues::BandMap_t& bandMap = pointMapInsertValue.first->second;

            // now go through each band and get the value in the .img file
            for(Bands_t::const_iterator j = bands.begin();
                j != bands.end(); ++j)
            {
                // get the value at the correct x- and y-coordinate in
                // the correct band
                Data_t value = imgReader.getValue(i->x(), i->y(), *j);

                // insert that value into the map
                bandMap.insert(std::make_pair(*j, value));
            }
        }
        // else do nothing since the point had already been inserted
        // and its associated values had already been found
    }
}

const Data_t&
ImgValues::getValue(const Point_t& point, const Band_t& band) const
{
    // find the point we're looking for
    ImgValues::PointMap_t::const_iterator i = values.find(point);
    if(i == values.end())
    {
        throw ImgValues::InvalidPoint();
    }

    // find the band we're looking for
    const ImgValues::BandMap_t& bandMap = i->second;
    const ImgValues::BandMap_t::const_iterator j = bandMap.find(band);
    if(j == bandMap.end())
    {
        throw ImgValues::InvalidBand();
    }

    // get the value
    return j->second;
}


/**
 * This is a simple functor to create filenames of the type I'm using in
 * this project, which are of the form
 * <path-to-folder-above-precipitation>/<precipitation-value, such as 6.00>in/
 * lr<resolution-value, such as 1920>_hydro.img
 * Obviously, this isn't very extendable, but it's suitable for my purposes.
 */
class CreateFilename
{
public:
    CreateFilename(const std::string& path, const std::string& prefix,
                   const std::string& suffix, const std::string& extension);
    std::string operator()(const std::string& precipitation,
                           const std::string& resolution) const;
private:
    const std::string m_path, m_prefix, m_suffix, m_extension;
};

CreateFilename::CreateFilename(const std::string& path, 
                               const std::string& prefix,
                               const std::string& suffix, 
                               const std::string& extension)
    : m_path(path), m_prefix(prefix), m_suffix(suffix), m_extension(extension)
{}

std::string
CreateFilename::operator()(const std::string& precipitation,
                           const std::string& resolution) const
{
    return m_path + precipitation + m_prefix + resolution + 
           m_suffix + m_extension;
}

/**
 * This class stores values from multiple .img files in such a way that
 * they are accessible by specifying their precipitation amount and
 * resolution.
 */
class AllValues
{
public:
    AllValues(const std::string& path, 
              const std::string& prefix,
              const std::string& suffix, 
              const std::string& extension,
              const Resolutions_t& resolutions, 
              const Precipitations_t& precipitations,
              const Points_t& points,
              const Bands_t& bands);
    Data_t getValue(const Resolution_t& resolution, 
                    const Precipitation_t& precipitation,
                    const Point_t& point, 
                    const Band_t& band) const;

    /**
     * Exception class to be used when the user asks for data for a resolution
     * that the object does not have data for
     */
    class InvalidResolution
    {};

    /**
     * Exception class to be used when the user asks for data for a 
     * precipitation that the object does not have data for
     */
    class InvalidPrecipitation
    {};

    // A couple typedefs used so that the user does not need to use the
    // ImgValues:: scope
    typedef ImgValues::InvalidPoint InvalidPoint;
    typedef ImgValues::InvalidBand InvalidBand;

private:
    /**
     * These typedefs will be used to create objects that will store data
     * that we can access by specifying a resolution, precipitation, point,
     * and band
     */
    //@{
    /**
     * A map that points from a precipitation value to an ImgValues object
     */
    typedef std::map<Precipitation_t, ImgValues> PrecipitationImgValues_t;

    /**
     * A map that points from a resolution value to the map that points
     * from a precipitation value to an Img Values object
     */
    typedef std::map<Resolution_t, PrecipitationImgValues_t> 
        ResolutionPrecipitation_t;
    //@}

    /**
     * The main data structure to hold all the data we want
     */
    ResolutionPrecipitation_t resPrecip;
};

AllValues::AllValues(const std::string& path, 
                     const std::string& prefix,
                     const std::string& suffix, 
                     const std::string& extension,
                     const Resolutions_t& resolutions, 
                     const Precipitations_t& precipitations,
                     const Points_t& points,
                     const Bands_t& bands)
{
    CreateFilename createFilename(path, prefix, suffix, extension);

    // Read in the data
    // go through each resolution
    for(Resolutions_t::const_iterator i = resolutions.begin(); 
        i != resolutions.end(); ++i)
    {
        // insert the resolution into our map, along with a 
        // default-constructed value of what it maps to, and get
        // its return value
        std::pair<ResolutionPrecipitation_t::iterator, bool> 
        resPrecipInsertValue = 
            resPrecip.insert(std::make_pair(*i, PrecipitationImgValues_t()));

        // go through each precipitation
        for(Precipitations_t::const_iterator j = precipitations.begin(); 
            j != precipitations.end(); ++j)
        {
            // create the filename based off the precipitation and resolution
            const std::string filename = createFilename(*j, *i);

            // insert the corresponding ImgValues into the map
            // First, point to the map that maps precipitation to an 
            // ImgValues object
            PrecipitationImgValues_t& precipImgValues = 
                resPrecipInsertValue.first->second;

            precipImgValues.
                insert(std::make_pair(*j, ImgValues(filename, points, bands)));
        }
    }
}

Data_t 
AllValues::getValue(const Resolution_t& resolution, 
                    const Precipitation_t& precipitation,
                    const Point_t& point, 
                    const Band_t& band) const
{
    // get the iterator to the map keyed on precipitation values
    ResolutionPrecipitation_t::const_iterator resIter =
        resPrecip.find(resolution);
    // make sure we have data for the specified resolution
    if(resIter == resPrecip.end())
    {
        throw AllValues::InvalidResolution();
    }

    // get the iterator to the ImgValues object for this resolution
    // and precipitation
    PrecipitationImgValues_t::const_iterator precipIter =
        resIter->second.find(precipitation);
    // make sure we have data for the specified precipitation
    if(precipIter == resIter->second.end())
    {
        throw AllValues::InvalidPrecipitation();
    }

    // get the correct value from the ImgValues object
    return precipIter->second.getValue(point, band);
}

int main()
{
    std::cout.precision(6);
    std::cout << std::fixed;

    const Precipitation_t precipitationsArray[] =
    {
        "1.00",
        "2.00",
        "3.00",
        "4.00",
        "5.00",
        "6.00",
        "7.00",
        "7.30",
        "8.00",
        "9.00"
    };
    const Precipitations_t
    precipitations(precipitationsArray,
                   precipitationsArray + sizeof(precipitationsArray)/
                                         sizeof(precipitationsArray[0]));

    const Resolution_t resolutionsArray[] =
    {
        "30",
        "60",
        "120",
        "210",
        "240",
        "480",
        "960",
        "1920"
    };
    const Resolutions_t
    resolutions(resolutionsArray,
                resolutionsArray + sizeof(resolutionsArray)/
                                   sizeof(resolutionsArray[0]));

    const Point_t pointsArray[] =
    {
        Point_t(244338.17235, 3509943.05796),
        Point_t(243876.45384, 3509519.99026),
        Point_t(244925.32144, 3507531.60852),
        Point_t(250344.48237, 3499648.11321),
        Point_t(241920.86317, 3514708.05963),
        Point_t(256122.17418, 3487125.20807),
        Point_t(254514.00120, 3485869.88143),
        Point_t(254419.30251, 3489880.48902)
    };
    const Points_t points(pointsArray,
                          pointsArray + sizeof(pointsArray)/
                                        sizeof(pointsArray[0]));

    const Band_t bandsArray[] =
    {
        1,
        2,
        3,
        4,
        5,
        6,
        7
    };
    const Bands_t bands(bandsArray,
                        bandsArray + sizeof(bandsArray)/
                                     sizeof(bandsArray[0]));

    // We'll be creating filenames of the following form
    const std::string path = "L:/sdir_snap/AGNPSOutputAnalysis/Data/"
                             "LittleRiver/NAWQA/Output/2005-08-17/";
    const std::string prefix = "in/lr";
    const std::string suffix = "_hydro";
    const std::string extension = ".img";

    AllValues allValues(path, prefix, suffix, extension,
                        resolutions, precipitations, points, bands);

    std::ofstream ofs("output.csv");
    ofs.precision(6);
    ofs << std::fixed;

    ofs << "Resolution,Point X-Coord,Point Y-Coord,Band,Precipitation,Value\n";
    for(Resolutions_t::const_iterator resIter = resolutions.begin();
        resIter != resolutions.end(); ++resIter)
    {
        for(Points_t::const_iterator pointsIter = points.begin();
            pointsIter != points.end(); ++pointsIter)
        {
            for(Bands_t::const_iterator bandsIter = bands.begin();
                bandsIter != bands.end(); ++bandsIter)
            {
                for(Precipitations_t::const_iterator precipIter =
                        precipitations.begin();
                    precipIter != precipitations.end(); ++precipIter)
                {
                    const Data_t value =
                        allValues.getValue(*resIter, *precipIter,
                                           *pointsIter, *bandsIter);
                    ofs << *resIter << ',' << pointsIter->x() << ',' 
                        << pointsIter->y() << ',' << *bandsIter
                        << ',' << *precipIter << ',' << value << '\n';
                }
            }
        }
    }
}

