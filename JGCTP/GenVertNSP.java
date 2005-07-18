package JGCTP;

public class GenVertNSP extends Projection
{

protected 	double m_p;

protected 	double m_height;

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

public 	GenVertNSP()
{
    super();
    setNumber(ProjCode.GVNSP);
    setName("General Vertical Near-Side Perspective");
    m_p = 0;
    m_height = 0;
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
}


public 	GenVertNSP(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.GVNSP);
    setName("General Vertical Near-Side Perspective");
    setParamLoad(true);
    m_p = 0;
    m_height = 0;
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
}


public 	void setHeight(double height) 
{
    m_height = height;
    setInit(true);
}

public 	double height() 
{
 return(m_height);
}

protected void _forward(GeoPoint p) throws ProjException
{
    
    double dlon;
    double sinphi,cosphi;
    double coslon;
    double g;
    double ksp;

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon - m_center.lon);
     sinphi = Math.sin(p.lat);
     cosphi = Math.cos(p.lat);

    coslon = Math.cos(dlon);
    g = m_sinCenterLat * sinphi + m_cosCenterLat * cosphi * coslon;
    if (g < (1.0/ m_p))
    {
        throw(new ProjException(153, "GenVertNSP._forward()"));
       
    }

    ksp = (m_p - 1.0)/(m_p - g);
    m_xy.x = m_falseEasting + m_sphere.radius * ksp * cosphi * Math.sin(dlon);
    m_xy.y = m_falseNorthing + m_sphere.radius * ksp * (m_cosCenterLat * sinphi - m_sinCenterLat * cosphi * coslon);

}


protected void _inverse(CoordPoint p) throws ProjException
{
    
    double rh;
    double r;
    double con;
    double com;
    double z,sinz,cosz;


    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    rh = Math.sqrt(p.x * p.x + p.y * p.y);
    r  = rh / m_sphere.radius;
    con = m_p - 1.0;
    com = m_p + 1.0;
    if (r > Math.sqrt(con/com))
    {
        throw(new ProjException(155, "GenVertNSP._inverse()"));
       
    }
    sinz = (m_p - Math.sqrt(1.0 - (r * r * com) / con)) / (con / r + r/con);
    z = Util.asinz(sinz);
     sinz = Math.sin(z);
     cosz = Math.cos(z);

    m_lonLat.lon = m_center.lon;
    if (Math.abs(rh) <= Constants.EPSLN)
    {
        m_lonLat.lat = m_center.lat;
        return;
    }
    m_lonLat.lat = Util.asinz(cosz * m_sinCenterLat + ( p.y * sinz * m_cosCenterLat)/rh);
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
    con = cosz - m_sinCenterLat * Math.sin(m_lonLat.lat);
    if ((Math.abs(con) < Constants.EPSLN) && (Math.abs(p.x) < Constants.EPSLN))
        return;
    
    m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2((p.x * sinz * m_cosCenterLat), (con * rh)));

}


protected 	void _init() throws ProjException
{
    /* Place parameters in static storage for common use
    -------------------------------------------------*/
    m_p = 1.0 + m_height / m_sphere.radius;
     m_sinCenterLat = Math.sin(m_center.lat);
     m_cosCenterLat = Math.cos(m_center.lat);

}


protected 	void _loadFromParams()
{

}



}