package jgctp;

public class Stereo extends Projection
{

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

public 	Stereo()
{
    super();
    setName("Stereographic");
    setNumber(ProjCode.STEREO);
    
}


public 	Stereo(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Stereographic");
    setNumber(ProjCode.STEREO);
}


protected 	void _init() throws ProjException
{
    m_sinCenterLat = Math.sin(m_center.lat);
    m_cosCenterLat = Math.cos(m_center.lat);
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
    rh = Math.sqrt(p.x * p.x + p.y * p.y);
    z = 2.0 * Math.atan(rh / (2.0 * m_sphere.rMajor));
    
     sinz = Math.sin(z);
     cosz = Math.cos(z);

    
    m_lonLat.lon = m_center.lon;
    if (Math.abs(rh) <= Constants.EPSLN)
    {
        m_lonLat.lat = m_center.lat;
        return;
    }
    else
    {
        m_lonLat.lat = Math.asin(cosz * m_sinCenterLat + (p.y * sinz * m_cosCenterLat) / rh);
        con = Math.abs(m_center.lat) - ((Math.PI)/2);
        if (Math.abs(con) <= Constants.EPSLN)
        {
            if (m_center.lat >= 0.0)
            {
                m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2(p.x, -p.y));
                return;
            }
            else
            {
                m_lonLat.lon = Util.adjust_lon(m_center.lon - Math.atan2(-p.x, p.y));
                return;
            }
        }
        else
        {
            con = cosz - m_sinCenterLat * Math.sin(m_lonLat.lat);
            if ((Math.abs(con) < Constants.EPSLN) && (Math.abs(p.x) < Constants.EPSLN))
                return;
            else
                m_lonLat.lon = Util.adjust_lon(m_center.lon + 
                                               Math.atan2((p.x * sinz * m_cosCenterLat), (con * rh)));
        }
    }

}


protected void _forward(GeoPoint p) throws ProjException
{
    double sinphi, cosphi;  /* sin and cos value                */
    double dlon;        /* delta longitude value            */
    double coslon;      /* cos of longitude             */
    double ksp;     /* scale factor                 */
    double g;       

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon - m_center.lon);
     sinphi = Math.sin(p.lat);
     cosphi = Math.cos(p.lat);

    coslon = Math.cos(dlon);
    g = m_sinCenterLat * sinphi + m_cosCenterLat * cosphi * coslon;

    if (Math.abs(g + 1.0) <= Constants.EPSLN)
    {
        throw(new ProjException(103, "Stereo._forward()"));
        
    }

    ksp = 2.0 / (1.0 + g);
    m_xy.x = m_falseEasting + m_sphere.rMajor * ksp * cosphi * Math.sin(dlon);
    m_xy.y = m_falseNorthing + m_sphere.rMajor * ksp * (m_cosCenterLat * sinphi - m_sinCenterLat * 
                cosphi * coslon);
}



}