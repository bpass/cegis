package JGCTP;

public class LambertAzimuthal extends Projection
{

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

public 	LambertAzimuthal()
{
    super();
    setNumber(ProjCode.LAMAZ);
    setName("Lambert Azimuthal");
    
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;

}


public 	LambertAzimuthal(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.LAMAZ);
    setName("Lambert Azimuthal");
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;

}


protected 	void _init() throws ProjException
{
     m_sinCenterLat = Math.sin(m_center.lat);
     m_cosCenterLat = Math.cos(m_center.lat);

}


protected void _forward(GeoPoint p) throws ProjException
{
    double delta_lon;   /* Delta longitude (Given longitude - center    */
    double sin_delta_lon;   /* Sine of the delta longitude          */
    double cos_delta_lon;   /* Cosine of the delta longitude        */
    double sin_lat;     /* Sine of the given latitude           */
    double cos_lat;     /* Cosine of the given latitude         */
    double g;       /* temporary varialbe               */
    double ksp;     /* heigth above elipsiod            */

    /* Forward equations
    -----------------*/
    delta_lon = Util.adjust_lon(p.lon - m_center.lon);
     sin_lat = Math.sin(p.lat);
     cos_lat = Math.cos(p.lat);

     sin_delta_lon = Math.sin(delta_lon);
     cos_delta_lon = Math.cos(delta_lon);

    g = m_sinCenterLat * sin_lat + m_cosCenterLat * cos_lat * cos_delta_lon;
    if (g == -1.0) 
    {
        throw(new ProjException(113, "LambertAzimuthal._forward()"));
      
    }

    ksp = m_sphere.radius * Math.sqrt(2.0 / (1.0 + g));
    m_xy.x = ksp * cos_lat * sin_delta_lon + m_falseEasting;
    m_xy.y = ksp * (m_cosCenterLat * sin_lat - m_sinCenterLat * cos_lat * m_cosCenterLat) + 
                m_falseNorthing;

}


protected void _inverse(CoordPoint p) throws ProjException
{
    double Rh;
    double z;       /* Great circle dist from proj center to given point */
    double sin_z;       /* Sine of z */
    double cos_z;       /* Cosine of z */
    double temp;        /* Re-used temporary variable */


    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    Rh = Math.sqrt(p.x * p.x + p.y * p.y);
    temp = Rh / (2.0 * m_sphere.radius);
    if (temp > 1) 
    {
        throw(new ProjException(115, "LambertAzimuthal._inverse()"));
        
    }

    z = 2.0 * Util.asinz(temp);
     sin_z = Math.sin(z);
     cos_z = Math.cos(z);

    m_lonLat.lon = m_center.lon;
    if (Math.abs(Rh) > Constants.EPSLN)
    {
        m_lonLat.lat = Util.asinz(m_sinCenterLat * cos_z + m_cosCenterLat * sin_z * p.y / Rh);
        temp = Math.abs(m_center.lat) - ((Math.PI)/2);
        if (Math.abs(temp) > Constants.EPSLN)
        {
            temp = cos_z - m_sinCenterLat * Math.sin(m_lonLat.lat);

            if(temp!=0.0)
                m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2(p.x*sin_z*m_cosCenterLat,temp*Rh));
        }
        else if (m_center.lat < 0.0) 
            m_lonLat.lon = Util.adjust_lon(m_center.lon - Math.atan2(-p.x, p.y));
        else 
            m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2(p.x, -p.y));
    }
    else 
        m_lonLat.lat = m_center.lat;
}



}