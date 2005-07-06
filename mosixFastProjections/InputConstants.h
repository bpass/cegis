#ifndef _INPUTCONSTANTS_H_
#define _INPUTCONSTANTS_H_

/*!
 *
 * \author Mark Schisler
 * \file   InputConstants.h
 * \date   $date$ 
 * \brief  File used to coordinate the use of constants and
 * projection parameters in use in the MOSIX fast projections 
 * project.
 *
 */ 

namespace USGSMosix {

    enum ELLIPSOID { CLARKE_1866 = 0, 
                     CLARKE_1880, 
                     BESSEL, 
                     INTERNATIONAL_1967,
                     INTERNATIONAL_1909,
                     WGS_72_ELLIPSOID, 
                     EVEREST, 
                     WGS_66, 
                     GRS_1980, 
                     AIRY, 
                     MODIFIED_EVEREST, 
                     MODIFIED_AIRY, 
                     WGS_84_ELLIPSOID, 
                     SOUTHEAST_ASIA, 
                     AUSTRALIAN_NATIONAL, 
                     KRASSOVSKY, 
                     HOUGH, 
                     MERCURY_1960, 
                     MODIFIED_MERCURY_1968, 
                     SPHERE_RADIUS_6370997_M }
        
    enum UNITS     { RADIANS = 20, 
                     US_FEET, 
                     METERS, 
                     ARC_SECONDS, 
                     ARC_DEGREES, 
                     INTERNATIONAL_FEET,
                     STATE_ZONE_TBLE } 

    enum DATUMS    { DINDAN = 27, 
                     ARC1950, 
                     ARC1960, 
                     AUSTRALIAN_GEODETIC_1966,
                     AUSTRALIAN_GEODETIC_1984,
                     CAMP_AREA_ASTRO, 
                     CAPE, 
                     EUROPEAN_DATUM_1950, 
                     EUROPEAN_DATUM_1979, 
                     GEODETIC_DATUM_1949, 
                     HONG_KONG_1963, 
                     HU_TZU_SHAN, 
                     INDIAN, 
                     NAD27,
                     NAD83, 
                     OLD_HAWAIIAN_MEAN, 
                     OMAN, 
                     ORDNANCE_SURVEY_1936, 
                     PUERTO_RICO, 
                     PULKOVO_1942, 
                     PROVISIONAL_S_AMERICAN_1956, 
                     TOKYO, 
                     WGS_72,
                     WGS_84 }

} // namespace 

#endif 

