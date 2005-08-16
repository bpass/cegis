package jgctp;

public class Mollweide extends Projection
{

public 	Mollweide()
{
    super();
    setName("Mollweide");
    setNumber(ProjCode.MOLL);
    
}


public 	Mollweide(double gctpParams[],  ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Mollweide");
    setNumber(ProjCode.MOLL);
}


protected 	void _init() throws ProjException
{
    return;
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
    con = Constants.PI * Math.sin(p.lat);

   /* Iterate using the Newton-Raphson method to find theta
    -----------------------------------------------------*/
    for (i=0;;i++)
    {
    delta_theta = -(theta + Math.sin(theta) - con)/ (1.0 + Math.cos(theta));
    theta += delta_theta;
    if (Math.abs(delta_theta) < Constants.EPSLN) break;
    if (i >= 50) 
    {
        throw(new ProjException(241, "Mollweide._forward()"));
  
    }
      
    }
    theta /= 2.0;

    /*  If the latitude is 90 deg, force the p.x coordinate to be "0 + false easting"
    this is done here because of precision problems with "Math.cos(theta)"
     --------------------------------------------------------------------------*/
    if (Constants.PI/2 - Math.abs(p.lat) < Constants.EPSLN)
        delta_lon =0;
    
    m_xy.x = 0.900316316158 * m_sphere.radius * delta_lon * Math.cos(theta) + m_falseEasting;
    m_xy.y = 1.4142135623731 * m_sphere.radius * Math.sin(theta) + m_falseNorthing;
}


protected void _inverse(CoordPoint p)
{
    double theta;
    double arg;

    /* Inverse equations
     -----------------*/
    p.x -= m_falseEasting; 
    p.y -= m_falseNorthing;
    arg = p.y /  (1.4142135623731 * m_sphere.radius);

    /* Because of division by zero problems, 'arg' can not be 1.0.  Therefore
       a number very close to one is used instead.
       -------------------------------------------------------------------*/
    if(Math.abs(arg) > 0.999999999999) 
      arg=0.999999999999;
    theta = Math.asin(arg);
    m_lonLat.lon = Util.adjust_lon(m_center.lon + (p.x / (0.900316316158 * m_sphere.radius * Math.cos(theta))));
    if(m_lonLat.lon < (-Constants.PI)) 
      m_lonLat.lon = -Constants.PI;
    if(m_lonLat.lon > Constants.PI) 
      m_lonLat.lon = Constants.PI;
    arg = (2.0 * theta + Math.sin(2.0 * theta)) / Constants.PI;
    if(Math.abs(arg) > 1.0)
      arg=1.0;
    m_lonLat.lat = Math.asin(arg);
}



}