package JGCTP;

public class VanDerGrinten extends Projection
{

public 	VanDerGrinten()
{
    super();
    setNumber(ProjCode.VGRINT);
    setName("Van Der Grinten");
}


public 	VanDerGrinten(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.VGRINT);
    setName("Van Der Grinten");
    setParamLoad(true);
}


protected 	void _init() throws ProjException
{
    return;
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double xx,yy,xys,c1,c2,c3;
    double a1;
    double m1;
    double con;
    double th1;
    double d;

    /* inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    con = Math.PI * m_sphere.radius;
    xx = p.x / con;
    yy = p.y / con;
    xys = xx * xx + yy * yy;
    c1 = -Math.abs(yy) * (1.0 + xys);
    c2 = c1 - 2.0 * yy * yy + xx * xx;
    c3 = -2.0 * c1 + 1.0 + 2.0 * yy * yy + xys * xys;
    d = yy * yy / c3 + (2.0 * c2 * c2 * c2 / c3 / c3 / c3 - 9.0 * c1 * c2 / c3 /c3)
        / 27.0;
    a1 = (c1 - c2 * c2 / 3.0 / c3) / c3;
    m1 = 2.0 * Math.sqrt( -a1 / 3.0);
    con = ((3.0 * d) / a1) / m1;
    if (Math.abs(con) > 1.0)
    {
    if (con >= 0.0)
        con = 1.0;
    else
        con = -1.0;
    }
    th1 = Math.acos(con) / 3.0;
    if (p.y >= 0)
    m_lonLat.lat = (-m1 * Math.cos(th1 + Math.PI / 3.0) - c2 / 3.0 / c3) * Math.PI;
    else
    m_lonLat.lat = -(-m1 * Math.cos(th1 + Math.PI / 3.0) - c2 / 3.0 / c3) * Math.PI;

    if (Math.abs(xx) < Constants.EPSLN)
    {
        m_lonLat.lon = m_center.lon;
        return;
    }
    
    m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.PI * (xys - 1.0 + Math.sqrt(1.0 + 2.0 * 
                                   (xx * xx - yy * yy) + xys * xys)) / 2.0 / xx);
}


protected void _forward(GeoPoint p) throws ProjException
{
    double dlon;
    double theta;
    double al,asq;
    double g,gsq;
    double m,msq;
    double con;
    double costh,sinth;

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon  - m_center.lon);

    if (Math.abs(p.lat) <= Constants.EPSLN)
    {
        m_xy.x = m_falseEasting  + m_sphere.radius * dlon;
        m_xy.y = m_falseNorthing;
        return;
    }

    theta = Util.asinz(2.0 * Math.abs(p.lat / Math.PI));

    if ((Math.abs(dlon) <= Constants.EPSLN) || (Math.abs(Math.abs(p.lat) - ((Math.PI)/2)) <= Constants.EPSLN))
    {
        m_xy.x = m_falseEasting;
        
        if (p.lat >= 0)
            m_xy.y = m_falseNorthing + Math.PI * m_sphere.radius * Math.tan(.5 * theta);
        else
            m_xy.x = m_falseNorthing + Math.PI * m_sphere.radius * -Math.tan(.5 * theta);

        return;
    }
    
    al = .5 * Math.abs((Math.PI / dlon) - (dlon / Math.PI));
    asq = al * al;
     sinth = Math.sin(theta);
     costh = Math.cos(theta);

    g = costh / (sinth + costh - 1.0);
    gsq = g * g;
    m = g * (2.0 / sinth - 1.0);
    msq = m * m;
    con = Math.PI * m_sphere.radius * (al * (g - msq) + Math.sqrt(asq * (g - msq) * (g - msq) - (msq + asq)
        * (gsq - msq))) / (msq + asq);
    
    if (dlon < 0)
        con = -con;
    
    m_xy.x = m_falseEasting + con;
    con = Math.abs(con / (Math.PI * m_sphere.radius));

    if (p.lat >= 0)
        m_xy.y = m_falseNorthing + Math.PI * m_sphere.radius * Math.sqrt(1.0 - con * con - 2.0 * al * con);
    else
        m_xy.y = m_falseNorthing - Math.PI * m_sphere.radius * Math.sqrt(1.0 - con * con - 2.0 * al * con);
}



}