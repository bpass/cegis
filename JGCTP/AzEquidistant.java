package jgctp;

public class AzEquidistant extends Projection
{

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

public 	AzEquidistant()
{
    super();
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
    setNumber(ProjCode.AZMEQD);
    setName("Azimuthal Equidistant");
}


public 	AzEquidistant(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
    setNumber(ProjCode.AZMEQD);
    setName("Azimuthal Equidistant");
}


protected 	void _init()
{
    m_sinCenterLat = Math.sin(m_center.lat);
    m_cosCenterLat = Math.cos(m_center.lat);
}


protected void _forward(GeoPoint p) throws ProjException
{
   
    double sinphi, cosphi;  /* sin and cos value                */
    double dlon;        /* delta longitude value            */
    double coslon;      /* cos of longitude             */
    double ksp;     /* scale factor                 */
    double g;       
    double z;       /* angle                    */

    dlon = Util.adjust_lon(p.lon - m_center.lon);
     sinphi = Math.sin(p.lat);
     cosphi = Math.cos(p.lat);

    coslon =Math.cos(dlon);
    g = m_sinCenterLat * sinphi + m_cosCenterLat * cosphi * coslon;
    if (Math.abs(Math.abs(g) - 1.0) < Constants.EPSLN)
    {
       ksp = 1.0;
       if (g < 0.0)
       {
         
          throw(new ProjException(123, "AzEquidistant._forward()"));
     
       }
    }
    else
    {
       z = Math.acos(g);
       ksp = z/Math.sin(z);
    }

    m_xy.x = m_falseEasting + m_sphere.rMajor * ksp * cosphi *Math.sin(dlon);
    m_xy.y = m_falseNorthing + m_sphere.rMajor * ksp * (m_cosCenterLat * sinphi - m_sinCenterLat * 
       cosphi * coslon);
}


protected void _inverse(CoordPoint p) throws ProjException
{
   
    double rh;      /* height above ellipsoid           */
    double z;       /* angle                    */
    double sinz,cosz;   /* sin of z and cos of z            */
   
    double con;


    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    rh =Math.sqrt(p.x * p.x + p.y * p.y);
    if (rh > (2.0 * ((Constants.PI)/2) * m_sphere.rMajor))
    {
       throw(new ProjException(125, "AzEquidistant._inverse()"));
     
    }

    z = rh / m_sphere.rMajor;
     sinz = Math.sin(z);
     cosz = Math.cos(z);

    m_lonLat.lon = m_center.lon;
    if(Math.abs(rh) <= Constants.EPSLN)
    {
       m_lonLat.lat = m_center.lat;
       return;
    }

    m_lonLat.lat = Util.asinz(cosz * m_sinCenterLat + (p.y * sinz * m_cosCenterLat) / rh);
    con =Math.abs(m_center.lat) - ((Constants.PI)/2);
    if (Math.abs(con) <= Constants.EPSLN)
    {
       if (m_center.lat >= 0.0)
       {
          m_lonLat.lon = Util.adjust_lon(m_center.lon +Math.atan2(p.x , -p.y));
          return;
       }
       else
       {
          m_lonLat.lon = Util.adjust_lon(m_center.lon -Math.atan2(-p.x , p.y));
          return;
       }
    }

    con = cosz - m_sinCenterLat *Math.sin(m_lonLat.lat);
    if ((Math.abs(con) < Constants.EPSLN) && (Math.abs(p.x) < Constants.EPSLN))
    {
       return;
    }

  
    m_lonLat.lon = Util.adjust_lon(m_center.lon +Math.atan2((p.x * sinz * m_cosCenterLat), (con * rh)));

}



}