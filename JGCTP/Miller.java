package jgctp;

public class Miller extends Projection
{

public 	Miller()
{
    super();
    setName("Miller Cylindrical");
    setNumber(ProjCode.MILLER);
}


public 	Miller(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Miller Cylindrical");
    setNumber(ProjCode.MILLER);
}


protected 	void _init() throws ProjException
{
    return;
}


protected void _forward(GeoPoint p) throws ProjException
{
    double dlon;

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon - m_center.lon);
    m_xy.x = m_falseEasting + m_sphere.radius * dlon;
    m_xy.y = m_falseNorthing + m_sphere.radius * Math.log(Math.tan((Constants.PI / 4.0) + (p.lat / 2.5))) * 1.25;

}


protected void _inverse(CoordPoint p) throws ProjException
{
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;

    m_lonLat.lon = Util.adjust_lon(m_center.lon + p.x / m_sphere.radius);
    m_lonLat.lat = 2.5 * (Math.atan(Math.exp(p.y / m_sphere.radius / 1.25)) - Constants.PI / 4.0);

}



}