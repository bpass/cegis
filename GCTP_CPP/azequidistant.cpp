
#include "azequidistant.h"

AzEquidistant::AzEquidistant() : Azimuthal(), m_sinCenterLat(0.0), m_cosCenterLat(0.0)
{
   setNumber(AZMEQD);
   setName("Azimuthal Equidistant");
}

AzEquidistant::AzEquidistant(double gctpParams[], ProjUnit units, ProjDatum dat) :
Azimuthal(gctpParams, units, dat), m_sinCenterLat(0.0), m_cosCenterLat(0.0)
{
   setNumber(AZMEQD);
   setName("Azimuthal Equidistant");
}

void AzEquidistant::init()
{
   clearError();
   Util::gctp_sincos(m_centerLat,&m_sinCenterLat,&m_cosCenterLat);
   m_initNeeded = false;
}

void AzEquidistant::forward(double lon, double lat, double* x, double* y)
{
   double sinphi, cosphi;	/* sin and cos value				*/
   double dlon;		/* delta longitude value			*/
   double coslon;		/* cos of longitude				*/
   double ksp;		/* scale factor					*/
   double g;		
   double z;		/* angle					*/

   clearError();
   if(m_initNeeded)
      init();

   Util::convertCoords(DEGREE, RADIAN, lon, lat);

   dlon = Util::adjust_lon(lon - m_centerLon);
   Util::gctp_sincos(lat,&sinphi,&cosphi);
   coslon = cos(dlon);
   g = m_sinCenterLat * sinphi + m_cosCenterLat * cosphi * coslon;
   if (fabs(fabs(g) - 1.0) < EPSLN)
   {
      ksp = 1.0;
      if (g < 0.0)
      {
         //con = 2.0 * HALF_PI * r_major;
         //sprintf(mess,"Point projects into a circle of radius = %12.2lf",con);
         //p_error(mess,"azim-for");  
         setError(123);
         return;
      }
   }
   else
   {
      z = acos(g);
      ksp = z/ sin(z);
   }

   m_x_coord = m_falseEasting + m_rMajor * ksp * cosphi * sin(dlon);
   m_y_coord = m_falseNorthing + m_rMajor * ksp * (m_cosCenterLat * sinphi - m_sinCenterLat * 
      cosphi * coslon);

   Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

   if(x)
      *x = m_x_coord;
   if(y)
      *y = m_y_coord;
}

void AzEquidistant::inverse(double x, double y, double* lon, double* lat)
{
   double rh;		/* height above ellipsoid			*/
   double z;		/* angle					*/
   double sinz,cosz;	/* sin of z and cos of z			*/
   double temp;
   double con;

   clearError();
   if(m_initNeeded)
      init();

   Util::convertCoords(m_unitCode, METER, x, y);

   /* Inverse equations
   -----------------*/
   x -= m_falseEasting;
   y -= m_falseNorthing;
   rh = sqrt(x * x + y * y);
   if (rh > (2.0 * HALF_PI * m_rMajor))
   {
      setError(125);
      return;
   }

   z = rh / m_rMajor;
   Util::gctp_sincos(z,&sinz,&cosz);
   m_longitude = m_centerLon;
   if (fabs(rh) <= EPSLN)
   {
      m_latitude = m_centerLat;
      Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);
      if(lon)
         *lon = m_longitude;
      if(lat)
         *lat = m_latitude;
      return;
   }

   m_latitude = Util::asinz(cosz * m_sinCenterLat + (y * sinz * m_cosCenterLat) / rh);
   con = fabs(m_centerLat) - HALF_PI;
   if (fabs(con) <= EPSLN)
   {
      if (m_centerLat >= 0.0)
      {
         m_longitude = Util::adjust_lon(m_centerLon + atan2(x , -y));
         Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);
         if(lon)
            *lon = m_longitude;
         if(lat)
            *lat = m_latitude;
         return;
      }
      else
      {
         m_longitude = Util::adjust_lon(m_centerLon - atan2(-x , y));
         Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);
         if(lon)
            *lon = m_longitude;
         if(lat)
            *lat = m_latitude;
         return;
      }
   }

   con = cosz - m_sinCenterLat * sin(m_latitude);
   if ((fabs(con) < EPSLN) && (fabs(x) < EPSLN))
   {
      Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);
      if(lon)
         *lon = m_longitude;
      if(lat)
         *lat = m_latitude;
      return;
   }

   temp = atan2((x * sinz * m_cosCenterLat), (con * rh));
   m_longitude = Util::adjust_lon(m_centerLon + atan2((x * sinz * m_cosCenterLat), (con * rh)));

   Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

   if(lon)
      *lon = m_longitude;
   if(lat)
      *lat = m_latitude;
}