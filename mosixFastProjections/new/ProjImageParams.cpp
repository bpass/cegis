
/*!
 *
 * \author Mark Schisler
 *
 * \file ProjImageParams.h
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \brief Implementation file for ProjImageParams class. 
 *
 */

#include "ProjImageParams.h"
#include <ProjUtil.h>
#include <fstream>

namespace USGSMosix {

/*****************************************************************************/
    
ProjImageParams::ProjImageParams( std::string paramFilename, FileType ft )
    : m_paramFilename(paramFilename),
      m_projection(NULL)
{
    WRITE_DEBUG ( "Creating Parameter file: " << paramFilename << std::endl );
    m_fileType = ft;
    std::ifstream in(paramFilename.c_str());
    in >> *this;
    in.close();
}

/*****************************************************************************/

ProjImageParams::~ProjImageParams()
{
    delete m_projection; 
}

/*****************************************************************************/

ProjImageParams::ProjImageParams( const ProjImageParams & params)
    : SerializableInterface(), 
      m_fileType( params.m_fileType ),
      m_imageFilename( params.m_imageFilename ),
      m_bounds( params.m_bounds ),
      m_outHeight( params.m_outHeight ),
      m_outWidth( params.m_outWidth ) 
{
    m_projection = params.m_projection->clone();    
}

/*****************************************************************************/

ProjImageParams 
ProjImageParams::createFromSocket( ClientSocket & socket )
{
    unsigned int len;
    int i = 0;
    char * msg = NULL;
    std::string strMsg; 
    FileType ft; 
    socket.receive( &len, sizeof(len) );
    if ( len > 0 ) 
    {
        msg = new (std::nothrow)char[len + 1]; 
        if ( msg == NULL )
            throw GeneralException("New allocation failed.");
    } else
        throw GeneralException("Error: Cannot make array of size less than 1");
    socket.receive( msg, len );
    msg[len] = '\0';
    strMsg = msg; 
    delete msg;
    socket.receive( &i, sizeof(i) );
    ft = static_cast<FileType>(i);
    return ProjImageParams( strMsg, ft );
            
}

/*****************************************************************************/

void ProjImageParams::exportToSocket( ClientSocket & socket )const
{
    unsigned int len = m_paramFilename.length(); 
    const char * _cstr = m_paramFilename.c_str();
    int i = static_cast<int>(m_fileType); // cannot transport enums 
    socket.appendToBuffer( &len, sizeof(len) );
    socket.appendToBuffer( _cstr, len );
    socket.appendToBuffer( &i, sizeof(i) );
    socket.sendFromBuffer();

    return;
}

/*****************************************************************************/

std::pair<long int, long int> ProjImageParams::getHeightWidthPair()
{
    if ( m_fileType == OUTPUT ) 
    {
        std::pair<long int, long int> hw(m_outHeight,m_outWidth);
        return hw;
    } else
        throw GeneralException("Error: no height width data for input params.");
}

/*****************************************************************************/

istream& operator>>( istream& in, ProjImageParams& params )
{
    // get input data
    in >> params.m_imageFilename;

    if ( params.m_fileType == ProjImageParams::INPUT )
    {
        in >> params.m_bounds.left
        >> params.m_bounds.right
        >> params.m_bounds.top
        >> params.m_bounds.bottom;
    } else 
    {
        in >> params.m_outWidth
           >> params.m_outHeight;
            
    }

    params.m_projection = params.constructProjection( in );
    return in;
    
}

/*****************************************************************************/

ostream& operator<<( ostream& out, const ProjImageParams& params )
{
    if ( params.m_projection != NULL )
    {
        out << "Image Information-------------------------------- " << std::endl
            << "Image file: " << params.m_imageFilename << std::endl;

        if ( params.m_fileType == ProjImageParams::INPUT )
        {
            out <<"File type: Input" << std::endl
                <<"Left Bound (deg.): "<<params.m_bounds.left<< std::endl
                <<"Right Bound (deg.): "<<params.m_bounds.right<< std::endl
                <<"Top Bound (deg.): "<< params.m_bounds.top << std::endl
                <<"Bottom Bound (deg.): "<<params.m_bounds.bottom<< std::endl
                << std::endl;
        } else 
        {
            out <<"File type: Output" << std::endl
                <<"Output Height (pixels): " << params.m_outHeight << std::endl
                <<"Output Width  (pixels): " << params.m_outWidth << std::endl;
        }

        out << "Projection Information--------------------------- "<< std::endl
            << params.m_projection->toString() << std::endl;
    } else
        out << "No input projection created." << std::endl;

    return out;
}

/*****************************************************************************/

Projection * ProjImageParams::constructProjection(istream& in)
{
    std::string projtype, sdatum, sunit;  // projection type, datum and unit
    double StdParallel1 = 0.0;            // projection options
    double StdParallel2 = 0.0;
    double NatOriginLong = 0.0;
    double NatOriginLat = 0.0;
    double FalseEasting = 0.0;
    double FalseNorthing = 0.0;
    double CenterLong = 0.0;
    double CenterLat = 0.0;
    double ScaleAtNatOrigin = 1.0;
    double StraightVertPoleLong = 0.0;
    double SemiMajorAxis = 0.0; // MS: addition Thu Jun  9 2005
    double SemiMinorAxis = 0.0; // MS: addition Thu Jun  9 2005
    double SphereRadius = 0.0;  // MS: addition Thu Jun  9 2005

    int zone = 0;
    Projection * proj = NULL; //the projection to return

    try
    {
        in >> projtype; //read the projection type

        if (projtype == std::string("GEO"))
        {
            in >> sdatum;
            in >> sunit;
            if (!(proj = new (std::nothrow) GeographicProjection
                         (GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("UTM"))
        {
            in >> sdatum;
            in >> sunit;
            in >> zone;

            if (!(proj = new (std::nothrow) UTMProjection(zone,
                         GetDatum(sdatum),
                         GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("SPCS"))
        {
            in >> sdatum
            >> sunit
            >> zone;

            if (sdatum == std::string("NAD83"))
                StatePlaneProjection::setNAD83ParameterFilename
                (std::string("./nad83sp"));
            else
            {
                if (sdatum == std::string("NAD27"))
                    StatePlaneProjection::setNAD83ParameterFilename
                    (std::string("./nad27sp"));
                else
                {
                    return NULL;  // not a valid state plane
                }
            }
            // create the state plane projection
            // here I am assuming that the zone will be
            // a correct state plane zone
            if (!(proj = new (std::nothrow) StatePlaneProjection
                         (zone, GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("ALBERS"))
        {
            in >> sdatum
            >> sunit
            >> StdParallel1
            >> StdParallel2
            >> NatOriginLat
            >> CenterLong
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj =  new (std::nothrow) AlbersConicProjection
                         ( ConvertToDMS(StdParallel1),
                           ConvertToDMS(StdParallel2),
                           SemiMajorAxis, 0.0, ConvertToDMS(CenterLong),
                           ConvertToDMS(NatOriginLat),
                           FalseEasting, FalseNorthing, GetDatum(sdatum),
                           GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("AZMEQD"))
        {
            in >> sdatum
            >> sunit
            >> CenterLong
            >> CenterLat
            >> FalseEasting
            >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new (std::nothrow) AzimuthalEquidistantProjection
                        ( ConvertToDMS(CenterLong),
                          ConvertToDMS(CenterLat),
                          FalseEasting, FalseNorthing, 
                          SphereRadius, GetDatum(sdatum),
                          GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("GNOMON"))
        {
            in >> sdatum >> sunit >> CenterLong >> CenterLat
            >> FalseEasting >> FalseNorthing >> SphereRadius;

            if(!(proj = new(std::nothrow) GnomonicProjection
                        ( ConvertToDMS(CenterLong),
                          ConvertToDMS(CenterLat),
                          FalseEasting,FalseNorthing,
                          SphereRadius, GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("LAMAZ"))
        {
            in >> sdatum
            >> sunit
            >> CenterLong
            >> CenterLat
            >> FalseEasting
            >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new(std::nothrow) LambertAzimuthalProjection
                        ( ConvertToDMS(CenterLong), ConvertToDMS(CenterLat),
                          FalseEasting, FalseNorthing, SphereRadius,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("ORTHO"))
        {
            in >> sdatum
            >> sunit
            >> CenterLong
            >> CenterLat
            >> FalseEasting
            >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new(std::nothrow) OrthographicProjection
                        ( ConvertToDMS(CenterLong),
                          ConvertToDMS(CenterLat),
                          FalseEasting,FalseNorthing, SphereRadius,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("STEREO"))
        {
            in >> sdatum >> sunit >> CenterLong
            >> CenterLat >> FalseEasting >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new(std::nothrow) StereographicProjection
                        (ConvertToDMS(CenterLong),
                         ConvertToDMS(CenterLat),
                         FalseEasting, FalseNorthing,
                         SphereRadius, GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("MILLER"))
        {
            in >> sdatum >> sunit >> CenterLong
            >> FalseEasting >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new (std::nothrow) MillerCylindricalProjection
                        ( SphereRadius, ConvertToDMS(CenterLong),
                          FalseEasting, FalseNorthing,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("ROBIN"))
        {
            in >> sdatum >> sunit >> CenterLong
            >> FalseEasting >> FalseNorthing
            >> SphereRadius;

            if(!(proj = new(std::nothrow) RobinsonProjection
                        ( SphereRadius, ConvertToDMS(CenterLong),
                          FalseEasting, FalseNorthing,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("SNSOID"))
        {
            in >> sdatum >> sunit >> CenterLong
            >> FalseEasting >> FalseNorthing
            >> SphereRadius;
            
            if(!(proj = new(std::nothrow) SinusoidalProjection
                        ( SphereRadius, ConvertToDMS(CenterLong),
                          FalseEasting,
                          FalseNorthing, GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("EQUIDC"))
        {
            in >> sdatum >> sunit >> StdParallel1
            >> StdParallel2;

            if ( StdParallel1 == StdParallel2 )
            {
                in >> NatOriginLat >> CenterLong >> CenterLat
                >> FalseEasting >> FalseNorthing
                >> SemiMajorAxis >> SemiMinorAxis;
               
                if(!(proj =  new(std::nothrow) EquidistantConicProjection
                             ( ConvertToDMS(CenterLat),
                               SemiMajorAxis,
                               SemiMinorAxis,
                               ConvertToDMS(CenterLong),
                               ConvertToDMS(NatOriginLat),
                               FalseEasting,
                               FalseNorthing,
                               GetDatum(sdatum), GetUnit(sunit))))
                    throw std::bad_alloc();

            } else 
            {
                in >> NatOriginLat >> CenterLong >> FalseEasting
                >> FalseNorthing >> SemiMajorAxis >> SemiMinorAxis;
                
                if(!(proj = new(std::nothrow) EquidistantConicProjection
                            ( ConvertToDMS(StdParallel1),
                              ConvertToDMS(StdParallel2),
                              SemiMajorAxis,
                              SemiMinorAxis,
                              ConvertToDMS(CenterLong),
                              ConvertToDMS(NatOriginLat),
                              FalseEasting,
                              FalseNorthing,
                              GetDatum(sdatum), GetUnit(sunit))))
                    throw std::bad_alloc();
            }
            
        } else if (projtype == std::string("EQRECT"))
        {
            in >> sdatum >> sunit >> CenterLong >> CenterLat
            >> FalseEasting >> FalseNorthing >> SphereRadius;

            if(!(proj =  new(std::nothrow) EquirectangularProjection
                         ( ConvertToDMS(CenterLat),
                           SphereRadius,
                           ConvertToDMS(CenterLong),
                           FalseEasting,FalseNorthing,
                           GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("HOM"))
        {
            double Latitude1 = 0.0;     // MS: addition Thu Jun  9 2005
            double Longitude1 = 0.0;    // MS: addition Thu Jun  9 2005
            double Latitude2 = 0.0;     // MS: addition Thu Jun  9 2005
            double Longitude2 = 0.0;    // MS: addition Thu Jun  9 2005

            in >> sdatum
            >> sunit
            >> CenterLong
            >> CenterLat
            >> ScaleAtNatOrigin
            >> Latitude1
            >> Longitude1
            >> Latitude2
            >> Longitude2
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj = new (std::nothrow) HotineObliqueMercatorProjection
                        ( ScaleAtNatOrigin,
                          ConvertToDMS(Longitude1),
                          ConvertToDMS(Latitude1),
                          ConvertToDMS(Longitude2),
                          ConvertToDMS(Latitude2),
                          SemiMajorAxis,
                          SemiMinorAxis,
                          ConvertToDMS(CenterLat),
                          FalseEasting,
                          FalseNorthing,
                          GetDatum(sdatum),
                          GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("LAMCC"))
        {
            in >> sdatum
            >> sunit
            >> StdParallel1
            >> StdParallel2
            >> NatOriginLong
            >> CenterLong
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj = new(std::nothrow) LambertConformalConicProjection
                        ( ConvertToDMS(StdParallel1),
                          ConvertToDMS(StdParallel2),
                          SemiMajorAxis,
                          SemiMinorAxis,
                          ConvertToDMS(NatOriginLong),
                          ConvertToDMS(CenterLong),
                          FalseEasting,
                          FalseNorthing,
                          GetDatum(sdatum),
                          GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("MERCAT"))
        {
            in >> sdatum
            >> sunit
            >> NatOriginLat
            >> CenterLong
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj = new(std::nothrow) MercatorProjection
                        ( SemiMajorAxis, SemiMinorAxis, 
                          ConvertToDMS(CenterLong),
                          ConvertToDMS(NatOriginLat),
                          FalseEasting, FalseNorthing,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("POLYC"))
        {

            in >> sdatum
            >> sunit
            >> CenterLong
            >> CenterLat
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj = new(std::nothrow) PolyconicProjection
                        ( SemiMajorAxis, SemiMinorAxis, 
                          ConvertToDMS(CenterLong),
                          ConvertToDMS(CenterLat), FalseEasting,
                          FalseNorthing, GetDatum(sdatum),
                          GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("PS"))
        {
            in >> sdatum
            >> sunit
            >> NatOriginLat          // latitude at true scale
            >> StraightVertPoleLong  // longitude down below poll of map
            >> FalseEasting
            >> FalseNorthing
            >> SemiMajorAxis
            >> SemiMinorAxis;

            if(!(proj = new(std::nothrow) PolarStereographicProjection
                        (ConvertToDMS(StraightVertPoleLong),
                         ConvertToDMS(NatOriginLat),
                         SemiMajorAxis,
                         SemiMinorAxis,
                         FalseEasting,
                         FalseNorthing,
                         GetDatum(sdatum),
                         GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("ALASKA"))
        {
            in >> sdatum >> sunit >> FalseEasting
            >> FalseNorthing >> SemiMajorAxis >> SemiMinorAxis;

            if(!(proj =  new(std::nothrow) AlaskaConformalProjection
                         (SemiMajorAxis, SemiMinorAxis, FalseEasting, 
                          FalseNorthing,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("TM"))
        {
            in >> sdatum >> sunit >> ScaleAtNatOrigin
            >> CenterLong >> NatOriginLat
            >> FalseEasting >> FalseNorthing
            >> SemiMajorAxis >> SemiMinorAxis;

            if(!(proj = new(std::nothrow) TransverseMercatorProjection
                        (ScaleAtNatOrigin, SemiMajorAxis, SemiMinorAxis,
                         ConvertToDMS(CenterLong),
                         ConvertToDMS(NatOriginLat), FalseEasting,
                         FalseNorthing,
                         GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("VGRINT"))
        {
            in >> sdatum >> sunit >> CenterLat
            >> CenterLong >> FalseEasting
            >> FalseNorthing;
            if(!(proj = new(std::nothrow) VanDerGrintenProjection
                        ( ConvertToDMS(CenterLat), 0.0,
                          ConvertToDMS(CenterLong),
                          FalseEasting,FalseNorthing,
                          GetDatum(sdatum), GetUnit(sunit))))
                throw std::bad_alloc();
            
        } else if (projtype == std::string("GOOD"))
        {
            in >> sdatum >> sunit >> SphereRadius;
            if (!(proj = new (std::nothrow) GoodeHomolosineProjection
                         ( SphereRadius,
                           GetDatum(sdatum),
                           GetUnit(sunit))))
                throw std::bad_alloc();
            
        } 

    }
    catch(...)
    {
        if (proj)
            delete proj;
        proj = NULL;
    }

    m_projection = proj;
    return m_projection;
}

/*****************************************************************************/

} // namespace USGSMosix
