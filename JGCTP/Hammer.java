package JGCTP;

public class Hammer extends Projection
{

public 	Hammer()
{
    super();
    setNumber(ProjCode.HAMMER);
    setName("Hammer");
}


public 	Hammer(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.HAMMER);
    setName("Hammer");
}


protected 	void _init() throws ProjException
{
    return;
}


protected void _inverse(CoordPoint p) throws ProjException
{
   
    double fac;

    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    fac = Math.sqrt(4.0 * m_sphere.radius * m_sphere.radius - (p.x * p.x)/ 4.0 - p.y * p.y) / 2.0;
    m_lonLat.lon = Util.adjust_lon(m_center.lon+ 2.0 * 
                    Math.atan2((p.x * fac), (2.0 * m_sphere.radius * m_sphere.radius - p.x * p.x/4 - p.y * p.y)));
    m_lonLat.lat = Util.asinz(p.y * fac / m_sphere.radius / m_sphere.radius);

}


protected void _forward(GeoPoint p) throws ProjException
{
    double dlon;
    double fac;

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon - m_center.lon);

    fac  = m_sphere.radius * 1.414213562 / Math.sqrt(1.0 + Math.cos(p.lat) * Math.cos(dlon / 2.0));
    m_xy.x = m_falseEasting + fac * 2.0 * Math.cos(p.lat) * Math.sin(dlon / 2.0);
    m_xy.y = m_falseNorthing + fac * Math.sin(p.lat);

}



}