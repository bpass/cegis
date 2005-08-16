package jgctp;

public class TransverseMercator extends Projection
{

protected 	double m_scaleFactor;

protected 	double m_e0;

protected 	double m_e1;

protected 	double m_e2;

protected 	double m_e3;

protected 	double m_e;

protected 	double m_es;

protected 	double m_esp;

protected 	double m_ml0;

protected 	double m_ind;

public 	TransverseMercator()
{
    super();
    setName("Transverse Mercator");
    setNumber(ProjCode.TM);
    m_scaleFactor = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_e = 0;
    m_es = 0;
    m_esp = 0;
    m_ml0 = 0;
    m_ind = 0;
    
}


public 	TransverseMercator(double gctpParams[], ProjUnit units, ProjDatum datum)
{
    super(gctpParams, units, datum);
    setName("Transverse Mercator");
    setNumber(ProjCode.TM);
    setParamLoad(true);
    m_scaleFactor = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_e = 0;
    m_es = 0;
    m_esp = 0;
    m_ml0 = 0;
    m_ind = 0;
}


public 	void setScaleFactor(double scale) 
{
    m_scaleFactor = scale;
    setInit(true);
}

public 	double scaleFactor() 
{
    return m_scaleFactor;
}

protected 	void _init()
{
    double temp;            /* temporary variable       */


    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);
    m_e0 = Util.e0fn(m_es);
    m_e1 = Util.e0fn(m_es);
    m_e2 = Util.e0fn(m_es);
    m_e3 = Util.e0fn(m_es);
    m_ml0 = m_sphere.rMajor * Util.mlfn(m_e0, m_e1, m_e2, m_e3, m_center.lat);
    m_esp = m_es / (1.0 - m_es);

    if (m_es < .00001)
        m_ind = 1;
    else 
        m_ind = 0;
}


protected 	void _loadFromParams() throws ProjException
{
    try
    {
        super._loadFromParams();
        setScaleFactor(m_gctpParams[2]);
    }
    
    catch(ProjException e)
    {
        e.addFunc("ProjException._loadFromParams()");
        throw(e);
    }
}


protected void _forward(GeoPoint p) throws ProjException
{
    double delta_lon;   /* Delta longitude (Given longitude - center    */
    double sin_phi, cos_phi;/* sin and cos value                */
    double al, als;     /* temporary values             */
    double b;       /* temporary values             */
    double c, t, tq;    /* temporary values             */
    double con, n, ml;  /* cone constant, small m           */

    /* Forward equations
    -----------------*/
    delta_lon = Util.adjust_lon(p.lon - m_center.lon);
     sin_phi = Math.sin(p.lat);
     cos_phi = Math.cos(p.lat);


    /* This part was in the fortran code and is for the spherical form 
    ----------------------------------------------------------------*/
    if (m_ind != 0)
    {
    b = cos_phi * Math.sin(delta_lon);
    if ((Math.abs(Math.abs(b) - 1.0)) < .0000000001)
        {
        throw(new ProjException(93, "TransverseMercator._forward()"));
      
        }
    else
        {
        m_xy.x = .5 * m_sphere.rMajor * m_scaleFactor * Math.log((1.0 + b)/(1.0 - b));
        con = Math.acos(cos_phi * Math.cos(delta_lon)/Math.sqrt(1.0 - b*b));
        if (p.lat < 0)
            con = - con;
        m_xy.y = m_sphere.rMajor * m_scaleFactor * (con - m_center.lat); 
        return;
        }
    }

    al  = cos_phi * delta_lon;
    als = (al*al);
    c   = m_esp * (cos_phi*cos_phi);
    tq  = Math.tan(p.lat);
    t   = (tq*tq);
    con = 1.0 - m_es * (sin_phi*sin_phi);
    n   = m_sphere.rMajor / Math.sqrt(con);
    ml  = m_sphere.rMajor * Util.mlfn(m_e0, m_e1, m_e2, m_e3, p.lat);

    m_xy.x  = m_scaleFactor * n * al * (1.0 + als / 6.0 * (1.0 - t + c + als / 20.0 *
        (5.0 - 18.0 * t + (t*t) + 72.0 * c - 58.0 * m_esp))) + m_falseEasting;

    m_xy.y  = m_scaleFactor * (ml - m_ml0 + n * tq * (als * (0.5 + als / 24.0 *
        (5.0 - t + 9.0 * c + 4.0 * (c*c) + als / 30.0 * (61.0 - 58.0 * t
        + (t*t) + 600.0 * c - 330.0 * m_esp))))) + m_falseNorthing;
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double con,phi;     /* temporary angles             */
    double delta_phi;   /* difference between longitudes        */
    long i;         /* counter variable             */
    double sin_phi, cos_phi, tan_phi;   /* sin cos and tangent values   */
    double c, cs, t, ts, n, r, d, ds;   /* temporary variables      */
    double f, h, g, temp;           /* temporary variables      */
    long max_iter = 6;          /* maximun number of iterations */

    /* fortran code for spherical form 
    --------------------------------*/
    if (m_ind != 0)
    {
        f = Math.exp(p.x/(m_sphere.rMajor * m_scaleFactor));
        g = .5 * (f - 1/f);
        temp = m_center.lat + p.y/(m_sphere.rMajor * m_scaleFactor);
        h = Math.cos(temp);
        con = Math.sqrt((1.0 - h * h)/(1.0 + g * g));
        m_lonLat.lat = Util.asinz(con);
        
        if (temp < 0)
            m_lonLat.lat = -m_lonLat.lat;

        if ((g == 0) && (h == 0))
        {
            m_lonLat.lon = m_center.lon;
            return;
        }
        
        else
        {
            m_lonLat.lon = Util.adjust_lon(Math.atan2(g,h) + m_center.lon);
            return;
        }
    }

    /* Inverse equations
    -----------------*/
    p.x = p.x - m_falseEasting;
    p.y = p.y - m_falseNorthing;

    con = (m_ml0 + p.y / m_scaleFactor) / m_sphere.rMajor;
    phi = con;
    for (i=0;;i++)
    {
        delta_phi = ((con + m_e1 * Math.sin(2.0*phi) - m_e2 * Math.sin(4.0*phi) + m_e3 * Math.sin(6.0*phi))
                    / m_e0) - phi;

        phi += delta_phi;

        if (Math.abs(delta_phi) <= Constants.EPSLN) 
            break;

        if (i >= max_iter) 
        { 
            throw(new ProjException(95, "TransverseMercator._inverse()"));
          
        }
    }
    if (Math.abs(phi) < ((Constants.PI)/2))
    {
         sin_phi = Math.sin(phi);
         cos_phi = Math.cos(phi);

        tan_phi = Math.tan(phi);
        c    = m_esp * (cos_phi*cos_phi);
        cs   = (c*c);
        t    = (tan_phi*tan_phi);
        ts   = (t*t);
        con  = 1.0 - m_es * (sin_phi*sin_phi); 
        n    = m_sphere.rMajor / Math.sqrt(con);
        r    = n * (1.0 - m_es) / con;
        d    = p.x / (n * m_scaleFactor);
        ds   = (d*d);
        m_lonLat.lat = phi - (n * tan_phi * ds / r) * (0.5 - ds / 24.0 * (5.0 + 3.0 * t + 
                    10.0 * c - 4.0 * cs - 9.0 * m_esp - ds / 30.0 * (61.0 + 90.0 * t +
                    298.0 * c + 45.0 * ts - 252.0 * m_esp - 3.0 * cs)));
        m_lonLat.lon = Util.adjust_lon(m_center.lon + (d * (1.0 - ds / 6.0 * (1.0 + 2.0 * t +
                    c - ds / 20.0 * (5.0 - 2.0 * c + 28.0 * t - 3.0 * cs + 8.0 * m_esp +
                    24.0 * ts))) / cos_phi));
    }
    else
    {
        m_lonLat.lat = ((Constants.PI)/2) * Util.sign(p.y);
        m_lonLat.lon = m_center.lon;
    }
}



}