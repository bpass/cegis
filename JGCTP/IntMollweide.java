package jgctp;

public class IntMollweide extends Projection
{

    double m_centerLons[];
    double m_falseEastings[];
    
public 	IntMollweide()
{
    super();
    setNumber(ProjCode.IMOLL);
    setName("Interrupted Mollweide");
    m_centerLons = new double[6];
    m_falseEastings = new double[6];
    m_centerLons[0] = 1.0471975512;
    m_centerLons[1] = -2.96705972839;
    m_centerLons[2] = -0.523598776; 
    m_centerLons[3] = 1.57079632679;
    m_centerLons[4] = -2.44346095279; 
    m_centerLons[5] = -0.34906585;

}


public 	IntMollweide(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.IMOLL);
    setName("Interrupted Mollweide");
    m_centerLons = new double[6];
    m_falseEastings = new double[6];
    m_centerLons[0] = 1.0471975512;
    m_centerLons[1] = -2.96705972839;
    m_centerLons[2] = -0.523598776; 
    m_centerLons[3] = 1.57079632679;
    m_centerLons[4] = -2.44346095279; 
    m_centerLons[5] = -0.34906585;
}


protected 	void _init() throws ProjException
{
    /* Initialize false eastings for each of the 6 regions
       ---------------------------------------------------*/
    m_falseEastings[0] = m_sphere.radius * -2.19988776387;
    m_falseEastings[1] = m_sphere.radius * -0.15713484;
    m_falseEastings[2] = m_sphere.radius * 2.04275292359;
    m_falseEastings[3] = m_sphere.radius * -1.72848324304;
    m_falseEastings[4] = m_sphere.radius * 0.31426968;
    m_falseEastings[5] = m_sphere.radius * 2.19988776387;

}


protected void _forward(GeoPoint p) throws ProjException
{
  
    double delta_lon;   /* Delta longitude (Given longitude - center */
    double theta;
    double delta_theta;
    double con;
    int i;
    int region;

    /* Forward equations
    -----------------*/
    /* Note:  Constants.PI has been adjusted so that the correct region will be assigned
    when p.lon = 180 deg.
    ----------------------------------------------------------------------*/
    if (p.lat >= 0.0)
    {
        if (p.lon >= 0.34906585 && p.lon < 1.91986217719) 
            region = 0; 
        else if 
            ((p.lon >= 1.919862177 && p.lon <= (Constants.PI + 1.0E-14)) ||
                        (p.lon >= (-Constants.PI - 1.0E-14) && p.lon < -1.745329252))
            region=1; 
        else 
            region = 2;
    }
    else
    {
        if (p.lon >= 0.34906585 && p.lon < 2.44346095279) 
            region = 3; 
        else if 
            ((p.lon >= 2.44346095279 && p.lon <= (Constants.PI +1.0E-14)) ||
                        (p.lon >= (-Constants.PI - 1.0E-14) && p.lon<-1.2217304764))
                region=4; 
        else 
            region = 5;
    }

    delta_lon = Util.adjust_lon(p.lon - m_centerLons[region]);
    theta = p.lat;
    con = Constants.PI * Math.sin(p.lat);

    /* Iterate using the Newton-Raphson method to find theta
    -----------------------------------------------------*/
    for (i=0;;i++)
    {
        delta_theta = -(theta + Math.sin(theta) - con) / (1.0 + Math.cos(theta));
        theta += delta_theta;
        if (Math.abs(delta_theta) < Constants.EPSLN) 
            break;
        
        if (i >= 50) 
        {
            throw(new ProjException(2, "IntMollweide._forward()"));  
        }
    }
    
    theta /= 2.0;

    /* If the latitude is 90 deg, force the p.x coordinate to be "0 + false easting"
    this is done here because of percision problems with "Math.cos(theta)"
    --------------------------------------------------------------------------*/
    if (Constants.PI / 2 - Math.abs(p.lat) < Constants.EPSLN)
        delta_lon = 0;

    m_xy.x = m_falseEastings[region] + 0.900316316158 * m_sphere.radius * delta_lon * Math.cos(theta);
    m_xy.y = m_sphere.radius * 1.4142135623731 * Math.sin(theta);
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double theta;
    int region;

    /* Inverse equations
    -----------------*/
    if (p.y >= 0.0)
    {
        if (p.x <= m_sphere.radius * -1.41421356248) 
            region = 0;
        else if (p.x <= m_sphere.radius * 0.942809042) 
            region = 1;
        else 
            region = 2;
    }
    else
    {
        if (p.x <= m_sphere.radius * -0.942809042) 
            region = 3;
        else if (p.x <= m_sphere.radius * 1.41421356248) 
            region = 4;
        else 
            region = 5;
    }

    p.x = p.x - m_falseEastings[region];

    theta = Math.asin(p.y / (1.4142135623731 * m_sphere.radius));
    m_lonLat.lon = Util.adjust_lon(m_centerLons[region] + (p.x / (0.900316316158*m_sphere.radius * Math.cos(theta))));
    m_lonLat.lat = Math.asin((2.0 * theta + Math.sin(2.0 * theta)) / Constants.PI);

    /* Are we in a interrupted area?  If so, return status code of Constants.IN_BREAK.
    ---------------------------------------------------------------------*/
    if (region == 0 && (m_lonLat.lon < 0.34906585 || m_lonLat.lon > 1.91986217719))
        throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));
    
    if (region == 1 && ((m_lonLat.lon < 1.91986217719 && m_lonLat.lon > 0.34906585) || 
                (m_lonLat.lon > -1.74532925199 && m_lonLat.lon < 0.34906585))) 
                throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));
    
    if (region == 2 && (m_lonLat.lon < -1.745329252 || m_lonLat.lon > 0.34906585)) 
        throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));
    
    if (region == 3 && (m_lonLat.lon < 0.34906585 || m_lonLat.lon > 2.44346095279))
        throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));
    
    if (region == 4 && ((m_lonLat.lon < 2.44346095279 && m_lonLat.lon > 0.34906585) || 
                (m_lonLat.lon > -1.2217304764 && m_lonLat.lon < 0.34906585))) 
                throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));

    if (region == 5 && (m_lonLat.lon < -1.2217304764 || m_lonLat.lon> 0.34906585))
        throw(new ProjException(Constants.IN_BREAK, "IntMollweide._inverse()"));
}



}