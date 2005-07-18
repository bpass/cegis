package JGCTP;

public class GoodeH extends Projection
{

protected double m_centerLons[];
    
protected double m_falseEastings[];
    
public 	GoodeH()
{
    super();
    m_centerLons = new double[12];
    m_falseEastings = new double[12];
    
    setNumber(ProjCode.GOOD);
    setName("Goode's Homolosine");

    m_centerLons[0] = -1.74532925199;       /* -100.0 degrees */
    m_centerLons[1] = -1.74532925199;       /* -100.0 degrees */
    m_centerLons[2] =  0.523598775598;  /*   30.0 degrees */
    m_centerLons[3] =  0.523598775598;  /*   30.0 degrees */
    m_centerLons[4] = -2.79252680319;       /* -160.0 degrees */
    m_centerLons[5] = -1.0471975512;        /*  -60.0 degrees */
    m_centerLons[6] = -2.79252680319;       /* -160.0 degrees */
    m_centerLons[7] = -1.0471975512;        /*  -60.0 degrees */
    m_centerLons[8] =  0.349065850399;  /*   20.0 degrees */
    m_centerLons[9] =  2.44346095279;       /*  140.0 degrees */
    m_centerLons[10] = 0.349065850399;  /*   20.0 degrees */
    m_centerLons[11] = 2.44346095279;       /*  140.0 degrees */

}


public 	GoodeH(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    m_centerLons = new double[12];
    m_falseEastings = new double[12];
    
    setNumber(ProjCode.GOOD);
    setName("Goode's Homolosine");

    m_centerLons[0] = -1.74532925199;       /* -100.0 degrees */
    m_centerLons[1] = -1.74532925199;       /* -100.0 degrees */
    m_centerLons[2] =  0.523598775598;  /*   30.0 degrees */
    m_centerLons[3] =  0.523598775598;  /*   30.0 degrees */
    m_centerLons[4] = -2.79252680319;       /* -160.0 degrees */
    m_centerLons[5] = -1.0471975512;        /*  -60.0 degrees */
    m_centerLons[6] = -2.79252680319;       /* -160.0 degrees */
    m_centerLons[7] = -1.0471975512;        /*  -60.0 degrees */
    m_centerLons[8] =  0.349065850399;  /*   20.0 degrees */
    m_centerLons[9] =  2.44346095279;       /*  140.0 degrees */
    m_centerLons[10] = 0.349065850399;  /*   20.0 degrees */
    m_centerLons[11] = 2.44346095279;       /*  140.0 degrees */
    
}


public 	double[] centerLons() 
{
    return(m_centerLons);
}

public 	double[] falseEastings() 
{
    return(m_falseEastings);
}

public 	void _init() throws ProjException
{
    m_falseEastings[0] = m_sphere.radius * -1.74532925199;
    m_falseEastings[1] = m_sphere.radius * -1.74532925199;
    m_falseEastings[2] = m_sphere.radius * 0.523598775598;
    m_falseEastings[3] = m_sphere.radius * 0.523598775598;
    m_falseEastings[4] = m_sphere.radius * -2.79252680319;
    m_falseEastings[5] = m_sphere.radius * -1.0471975512;
    m_falseEastings[6] = m_sphere.radius * -2.79252680319;
    m_falseEastings[7] = m_sphere.radius * -1.0471975512;
    m_falseEastings[8] = m_sphere.radius * 0.349065850399;
    m_falseEastings[9] = m_sphere.radius * 2.44346095279;
    m_falseEastings[10] = m_sphere.radius * 0.349065850399;
    m_falseEastings[11] = m_sphere.radius * 2.44346095279;

}


public 	void _forward(GeoPoint p) throws ProjException
{
    

    double delta_lon;   /* Delta longitude (Given longitude - center */
    double theta;
    double delta_theta;
    double constant;
    int i;
    int region;

    /* Forward equations
    -----------------*/
    if (p.lat >= 0.710987989993)                 /* if on or above 40 44' 11.8" */
    {
        if (p.lon <= -0.698131700798) 
            region = 0;   /* If to the left of -40 */
        else 
            region = 2;
    }
    else if (p.lat >= 0.0)               /* Between 0.0 and 40 44' 11.8" */
    {
        if (p.lon <= -0.698131700798) 
            region = 1;   /* If to the left of -40 */
        else 
            region = 3;
    }
    else if (p.lat >= -0.710987989993)           /* Between 0.0 & -40 44' 11.8" */
    {
        if (p.lon <= -1.74532925199) 
            region = 4;     /* If between -180 and -100 */
        else if (p.lon <= -0.349065850399) 
            region = 5; /* If between -100 and -20 */
        else if (p.lon <= 1.3962634016) 
            region = 8; /* If between -20 and 80 */
        else 
            region = 9;             /* If between 80 and 180 */
    }
    else                        /* Below -40 44' */
    {
        if (p.lon <= -1.74532925199)
            region = 6;       /* If between -180 and -100 */
        else if (p.lon <= -0.349065850399) 
            region = 7;     /* If between -100 and -20 */
        else if (p.lon <= 1.3962634016) 
            region = 10;   /* If between -20 and 80 */
        else 
            region = 11;                            /* If between 80 and 180 */
    }

    if (region==1||region==3||region==4||region==5||region==8||region==9)
    {
        delta_lon = Util.adjust_lon(p.lon - m_centerLons[region]);
        m_xy.x = m_falseEastings[region] + m_sphere.radius * delta_lon * Math.cos(p.lat);
        m_xy.y = m_sphere.radius * p.lat;
    }
    else
    {
        delta_lon = Util.adjust_lon(p.lon - m_centerLons[region]);
        theta = p.lat;
        constant = Math.PI * Math.sin(p.lat);

        /* Iterate using the Newton-Raphson method to find theta
        -----------------------------------------------------*/
        for (i=0;;i++)
        {
            delta_theta = -(theta + Math.sin(theta) - constant) / (1.0 + Math.cos(theta));
            theta += delta_theta;
            if (Math.abs(delta_theta) < Constants.EPSLN) 
                break;

            if (i >= 50) 
            {
                throw(new ProjException(251, "GoodeH._forward()"));
               
            }
        }
        theta /= 2.0;

        /* If the latitude is 90 deg, force the p.x coordinate to be
            "0 + false easting" this is done here because of precision problems
            with "Math.cos(theta)"
            ------------------------------------------------------------------*/
        if (Math.PI / 2 - Math.abs(p.lat) < Constants.EPSLN)
            delta_lon = 0;
        m_xy.x = m_falseEastings[region] + 0.900316316158 * m_sphere.radius * delta_lon * Math.cos(theta);
        m_xy.y = m_sphere.radius * (1.4142135623731 * Math.sin(theta) - 0.0528035274542 * Util.sign(p.lat));
    }
}


public 	void _inverse(CoordPoint _p) throws ProjException
{
    CoordPoint p = new CoordPoint(_p);
    
    double arg;
    double theta;
    double temp;
    int region;

    /* Inverse equations
    -----------------*/
    if (p.y >= m_sphere.radius * 0.710987989993)                 /* if on or above 40 44' 11.8" */
    {
        if (p.x <= m_sphere.radius * -0.698131700798) 
            region = 0; /* If to the left of -40 */
        else 
            region = 2;
    }
    else if (p.y >= 0.0)                           /* Between 0.0 and 40 44' 11.8" */
    {
        if (p.x <= m_sphere.radius * -0.698131700798) 
            region = 1; /* If to the left of -40 */
        else 
            region = 3;
    }
    else if (p.y >= m_sphere.radius * -0.710987989993)           /* Between 0.0 & -40 44' 11.8" */
    {
        if (p.x <= m_sphere.radius * -1.74532925199) 
            region = 4;     /* If between -180 and -100 */
        else if (p.x <= m_sphere.radius * -0.349065850399) 
            region = 5; /* If between -100 and -20 */
        else if (p.x <= m_sphere.radius * 1.3962634016) 
            region = 8;  /* If between -20 and 80 */
        else 
            region = 9;                             /* If between 80 and 180 */
    }
    else                                            /* Below -40 44' 11.8" */
    {
        if (p.x <= m_sphere.radius * -1.74532925199) 
            region = 6;     /* If between -180 and -100 */
        else if (p.x <= m_sphere.radius * -0.349065850399) 
            region = 7; /* If between -100 and -20 */
        else if (p.x <= m_sphere.radius * 1.3962634016) 
            region = 10; /* If between -20 and 80 */
        else 
            region = 11;                            /* If between 80 and 180 */
    }
    p.x = p.x -  m_falseEastings[region];

    if (region==1||region==3||region==4||region==5||region==8||region==9)
    {
        m_lonLat.lat = p.y / m_sphere.radius;
        if (Math.abs(m_lonLat.lat) > ((Math.PI)/2)) 
        {
            throw(new ProjException(252, "GoodeH._inverse()"));
           
        }
        temp = Math.abs(m_lonLat.lat) - ((Math.PI)/2);
        if (Math.abs(temp) > Constants.EPSLN)
        {
            temp = m_centerLons[region] + p.x / (m_sphere.radius * Math.cos(m_lonLat.lat));
            m_lonLat.lon = Util.adjust_lon(temp);
        }
        else 
            m_lonLat.lon = m_centerLons[region];
    }
    else
    {
        arg = (p.y + 0.0528035274542 * m_sphere.radius * Util.sign(p.y)) /  (1.4142135623731 * m_sphere.radius);
        if (Math.abs(arg) > 1.0) 
        {
            throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
          
        }
        theta = Math.asin(arg);
        m_lonLat.lon = Util.adjust_lon(m_centerLons[region]+(p.x/(0.900316316158 * m_sphere.radius * Math.cos(theta))));
        if(m_lonLat.lon < -(Math.PI + Constants.EPSLN)) 
        {
            throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
            
        }
        arg = (2.0 * theta + Math.sin(2.0 * theta)) / Math.PI;
        if (Math.abs(arg) > 1.0) 
        {
            throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
           
        }
        m_lonLat.lat = Math.asin(arg);
    }
    /* because of precision problems, long values of 180 deg and -180 deg
    may be mixed.
    ----------------------------------------------------------------*/
    if (((p.x < 0) && (Math.PI - m_lonLat.lon < Constants.EPSLN)) || ((p.x > 0) && (Math.PI + m_lonLat.lon < Constants.EPSLN)))
        m_lonLat.lon = -(m_lonLat.lon);

    /* Are we in a interrupted area?  If so, return status code of Constants.IN_BREAK.
    ---------------------------------------------------------------------*/
    if (region == 0 && (m_lonLat.lon < -(Math.PI + Constants.EPSLN) || m_lonLat.lon > -0.698131700798))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 1 && (m_lonLat.lon < -(Math.PI + Constants.EPSLN) || m_lonLat.lon > -0.698131700798))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 2 && (m_lonLat.lon < -0.698131700798 || m_lonLat.lon > Math.PI + Constants.EPSLN))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 3 && (m_lonLat.lon < -0.698131700798 || m_lonLat.lon > Math.PI + Constants.EPSLN))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 4 && (m_lonLat.lon < -(Math.PI + Constants.EPSLN) || m_lonLat.lon > -1.74532925199))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 5 && (m_lonLat.lon < -1.74532925199 || m_lonLat.lon > -0.349065850399))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 6 && (m_lonLat.lon < -(Math.PI + Constants.EPSLN) || m_lonLat.lon > -1.74532925199))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 7 && (m_lonLat.lon < -1.74532925199 || m_lonLat.lon > -0.349065850399))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 8 && (m_lonLat.lon < -0.349065850399 || m_lonLat.lon > 1.3962634016))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region == 9 && (m_lonLat.lon < 1.3962634016|| m_lonLat.lon > Math.PI + Constants.EPSLN))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region ==10 && (m_lonLat.lon < -0.349065850399 || m_lonLat.lon > 1.3962634016))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));
    if (region ==11 && (m_lonLat.lon < 1.3962634016 || m_lonLat.lon > Math.PI + Constants.EPSLN))
        throw(new ProjException(Constants.IN_BREAK, "GoodeH._inverse()"));

}



}