package jgctp;

public class Orthographic extends Projection
{

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

public 	Orthographic()
{
    super();
    setName("Orthographic");
    setNumber(ProjCode.ORTHO);
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
}


public 	Orthographic(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Orthographic");
    setNumber(ProjCode.ORTHO);
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
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
    if (rh > m_sphere.rMajor + .0000001)
    {
        throw(new ProjException(145, "Orthographic._inverse()"));
      
    }
    z = Util.asinz(rh / m_sphere.rMajor);
     sinz = Math.sin(z);
     cosz = Math.cos(z);

    m_lonLat.lon = m_center.lon;
    if (Math.abs(rh) <= Constants.EPSLN)
    {
        m_lonLat.lat = m_center.lat;
        return;
    }
    m_lonLat.lat = Util.asinz(cosz * m_sinCenterLat + (p.y * sinz * m_cosCenterLat)/rh);
    con = Math.abs(m_center.lat) - ((Constants.PI)/2);
    if (Math.abs(con) <= Constants.EPSLN)
    {
        if (m_center.lat >= 0)
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
    con = cosz - m_sinCenterLat * Math.sin(m_lonLat.lat);
    if ((Math.abs(con) >= Constants.EPSLN) || (Math.abs(p.x) >= Constants.EPSLN))
        m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2((p.x * sinz * m_cosCenterLat), (con * rh)));

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
    ksp = 1.0;
    if ((g > 0) || (Math.abs(g) <= Constants.EPSLN))
    {
        m_xy.x = m_falseEasting + m_sphere.rMajor * ksp * cosphi * Math.sin(dlon);
        m_xy.y = m_falseNorthing + m_sphere.rMajor * ksp * (m_cosCenterLat * sinphi -
                    m_sinCenterLat * cosphi * coslon);
    }
    else
    {
        throw(new ProjException(143, "Orthographic._forward()"));
        
    }
}



}