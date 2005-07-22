package jgctp;

public class WagnerIV extends Projection
{

public 	WagnerIV()
{
    super();
    setNumber(ProjCode.WAGIV);
    setName("Wagner IV");
}


public 	WagnerIV(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.WAGIV);
    setName("Wagner IV");
}


protected 	void _init() throws ProjException
{
    return;
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double theta;

    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    theta = Math.asin(p.y /  (1.56548 * m_sphere.radius));
    m_lonLat.lon = Util.adjust_lon(m_center.lon + (p.x / (0.86310 * m_sphere.radius * Math.cos(theta))));
    m_lonLat.lat = Math.asin((2.0 * theta + Math.sin(2.0 * theta)) / 2.9604205062);

}


protected void _forward(GeoPoint p) throws ProjException
{
    double delta_lon;   /* Delta longitude (Given longitude - center */
    double theta;
    double delta_theta;
    double con;
    long i;

    /* Forward equations
    -----------------*/
    delta_lon = Util.adjust_lon(p.lon - m_center.lon);
    theta = p.lat;
    con = 2.9604205062 * Math.sin(p.lat);

    /* Iterate using the Newton-Raphson method to find theta
    -----------------------------------------------------*/
    for (i=0;;i++)
    {
        delta_theta = -(theta + Math.sin(theta) - con) / (1.0 + Math.cos(theta));
        theta += delta_theta;
        if (Math.abs(delta_theta) < Constants.EPSLN) 
            break;
        if (i >= 30) {
            throw(new ProjException(2, "WagnerIV._forward()"));
          
        }
    }
    theta /= 2.0;
    m_xy.x = 0.86310 * m_sphere.radius * delta_lon * Math.cos(theta) + m_falseEasting;
    m_xy.y = 1.56548 * m_sphere.radius * Math.sin(theta) + m_falseNorthing;
}



}