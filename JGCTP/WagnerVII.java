package JGCTP;

public class WagnerVII extends Projection
{

public 	WagnerVII()
{
    super();
    setNumber(ProjCode.WAGVII);
    setName("Wagner VII");
}


public 	WagnerVII(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.WAGVII);
    setName("Wagner VII");
}


protected void _init()
{
    return;
}


protected void _inverse(CoordPoint _p)
{
    CoordPoint p = new CoordPoint(_p);
    
    double t1, t2, p1, c;

    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    t1 = p.x / 2.66723;
    t2 = p.y / 1.24104;
    t1 *= t1;
    t2 *= t2;
    p1 = Math.sqrt(t1 + t2);
    c = 2.0 * Util.asinz(p1 / (2.0 * m_sphere.radius));
    m_lonLat.lat = Util.asinz(p.y * Math.sin(c) / (1.24104 * 0.90631 * p1));
    m_lonLat.lon = Util.adjust_lon(m_center.lon + 3.0 * Math.atan2(p.x * Math.tan(c), 2.66723 * p1));
}


protected void _forward(GeoPoint _p)
{
    
    GeoPoint p = new GeoPoint(_p);
    double delta_lon;   /* Delta longitude (Given longitude - center */
    double sin_lon, cos_lon;
    double s, c0, c1;

    /* Forward equations
    -----------------*/
    delta_lon = Util.adjust_lon(p.lon - m_center.lon);
    sin_lon = Math.sin(delta_lon/3.0);
    cos_lon = Math.cos(delta_lon/3.0);
    
    s = 0.90631 * Math.sin(p.lat);
    c0 = Math.sqrt(1-s*s);
    c1 = Math.sqrt(2.0 / (1.0 + c0 * cos_lon));
    m_xy.x = 2.66723 * m_sphere.radius * c0 * c1 * sin_lon + m_falseEasting;
    m_xy.y = 1.24104 * m_sphere.radius * s * c1 + m_falseNorthing;
  

}


}