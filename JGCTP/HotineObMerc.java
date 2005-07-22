package jgctp;

public class HotineObMerc extends Projection
{

protected 	double m_azimuth;

protected 	double m_scaleFactor;

protected 	double m_e;

protected 	double m_es;

protected 	double m_sinCenterLat;

protected 	double m_cosCenterLat;

protected 	double m_bl;

protected 	double m_al;

protected 	double m_ts;

protected 	double m_d;

protected 	double m_el;

protected 	double m_u;

protected 	double m_singam;

protected 	double m_cosgam;

protected 	double m_sinaz;

protected 	double m_cosaz;

protected 	double m_lat1;

protected 	double m_lat2;

protected 	double m_lon1;

protected 	double m_lon2;

protected 	int m_mode;

public 	HotineObMerc()
{
   super();
   setNumber(ProjCode.HOM);
   setName("Hotine Oblique Mercator");
   m_azimuth = 0;
   m_scaleFactor = 0;
   m_e = 0;
   m_es = 0;
   m_sinCenterLat = 0;
   m_cosCenterLat = 0;
   m_bl = 0;
   m_al = 0;
   m_ts = 0;
   m_d = 0;
   m_el = 0;
   m_u = 0;
   m_singam = 0;
   m_cosgam = 0;
   m_sinaz = 0;
   m_cosaz = 0;
   m_lat1 = 0;
   m_lat2 = 0;
   m_lon1 = 0;
   m_lon2 = 0;
   m_mode = 0;
}


public 	HotineObMerc(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.HOM);
    setName("Hotine Oblique Mercator");
    setParamLoad(true);
    m_azimuth = 0;
    m_scaleFactor = 0;
    m_e = 0;
    m_es = 0;
    m_sinCenterLat = 0;
    m_cosCenterLat = 0;
    m_bl = 0;
    m_al = 0;
    m_ts = 0;
    m_d = 0;
    m_el = 0;
    m_u = 0;
    m_singam = 0;
    m_cosgam = 0;
    m_sinaz = 0;
    m_cosaz = 0;
    m_lat1 = 0;
    m_lat2 = 0;
    m_lon1 = 0;
    m_lon2 = 0;
    m_mode = 0;
}


public 	void setScaleFactor(double fac) 
{
    m_scaleFactor = fac;
    setInit(true);
}

public 	void setAzimuth(double angle) throws ProjException
{
    try
    {
        m_azimuth = Util.convertAngle(angle);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc.setAzimuth()");
        throw(e);
    }
}


public 	void setLon1(double lon) throws ProjException
{
    try
    {
        m_lon1 = Util.convertAngle(lon);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc.setLon1()");
        throw(e);
    }
}


public 	void setLon2(double lon) throws ProjException
{
    try
    {
        m_lon2 = Util.convertAngle(lon);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc.setLon2()");
        throw(e);
    }
}


public 	void setLat1(double lat) throws ProjException
{
    try
    {
        m_lat1 = Util.convertAngle(lat);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc.setLat1()");
        throw(e);
    }
}


public 	void setLat2(double lat) throws ProjException
{
    try
    {
        m_lat2 = Util.convertAngle(lat);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc.setLat2()");
        throw(e);
    }
}


public 	void setMode(int mode) 
{
    m_mode = mode;
    setInit(true);
}

public 	double scaleFactor() 
{
    return(m_scaleFactor);
}

public 	double azimuth() 
{
    return (m_azimuth);
}

public 	double lon1() 
{
    return(m_lon1);
}

public 	double lon2() 
{
    return(m_lon2);
}

public 	double lat1() 
{
    return(m_lat1);
}

public 	double lat2() 
{
    return(m_lat2);
}

public 	int mode() 
{
    return(m_mode);
}

protected 	void _init() throws ProjException
{
    double temp = 0;            /* temporary variable       */
    double con = 0,com = 0;
    double h = 0,l = 0,ts1 = 0,ts2 = 0;
    double j = 0,p = 0,dlon = 0;
    double f = 0,g = 0,gama = 0;
    double sinphi;


    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);

     m_cosCenterLat = Math.sin(m_center.lat);
     m_cosCenterLat = Math.cos(m_center.lat);

    con = 1.0 - m_es * m_cosCenterLat * m_cosCenterLat;
    com = Math.sqrt(1.0 - m_es);
    m_bl = Math.sqrt(1.0 + m_es * Math.pow(m_cosCenterLat,4.0)/(1.0 - m_es));
    m_al = m_sphere.rMajor * m_bl * m_scaleFactor * com / con;
    if (Math.abs(m_center.lat) < Constants.EPSLN)
    {
        m_ts = 1.0;
        m_d = 1.0;
        m_el = 1.0;
    }
    else
    {
        m_ts = Util.tsfnz(m_e,m_center.lat,m_cosCenterLat);
        con = Math.sqrt(con);
        m_d = m_bl * com / (m_cosCenterLat * con);
        if ((m_d * m_d - 1.0) > 0.0)
        {
            if (m_center.lat>= 0.0)
                f = m_d + Math.sqrt(m_d * m_d - 1.0);
            else
                f = m_d - Math.sqrt(m_d * m_d - 1.0);
        }
        else
            f = m_d;
        m_el = f * Math.pow(m_ts,m_bl);
    }


    if (m_mode != 0)
    {
        g = .5 * (f - 1.0/f);
        gama = Util.asinz(Math.sin(m_azimuth) / m_d);
        m_center.lon= m_center.lon- Util.asinz(g * Math.tan(gama))/m_bl;

        con = Math.abs(m_center.lat);
        if ((con > Constants.EPSLN) && (Math.abs(con - ((Math.PI)/2)) > Constants.EPSLN))
        {
             m_singam = Math.sin(gama);
             m_cosgam = Math.cos(gama);

             m_sinaz = Math.sin(m_azimuth);
             m_cosaz = Math.cos(m_azimuth);

            if (m_center.lat>= 0)
                m_u =  (m_al / m_bl) * Math.atan(Math.sqrt(m_d*m_d - 1.0)/m_cosaz);
            else
                m_u =  -(m_al / m_bl) * Math.atan(Math.sqrt(m_d*m_d - 1.0)/m_cosaz);
        }
        else
        {
            throw(new ProjException(201, "HotineObMerc._init()"));
            
        }
    }
    else
    {
        sinphi = Math.sin(m_lat1);
        ts1 = Util.tsfnz(m_e,m_lat1,sinphi);
        sinphi = Math.sin(m_lat2);
        ts2 = Util.tsfnz(m_e,m_lat2,sinphi);
        h = Math.pow(ts1,m_bl);
        l = Math.pow(ts2,m_bl);
        f = m_el/h;
        g = .5 * (f - 1.0/f);
        j = (m_el * m_el - l * h)/(m_el * m_el + l * h);
        p = (l - h) / (l + h);
        dlon = m_lon1 - m_lon2;
        if (dlon < -Math.PI)
            m_lon2 = m_lon2 - 2.0 * Math.PI;
        if (dlon > Math.PI)
            m_lon2 = m_lon2 + 2.0 * Math.PI;
        dlon = m_lon1 - m_lon2;
        m_center.lon= .5 * (m_lon1 + m_lon2) - Math.atan(j * Math.tan(.5 * m_bl * dlon)/p)/m_bl;
        dlon  = Util.adjust_lon(m_lon1 - m_center.lon);
        gama = Math.atan(Math.sin(m_bl * dlon)/g);
        m_azimuth = Util.asinz(m_d * Math.sin(gama));

        if (Math.abs(m_lat1 - m_lat2) <= Constants.EPSLN)
        {
            throw(new ProjException(202, "HotineObMerc._init()"));
           
        }
        else
            con = Math.abs(m_lat1);
        if ((con <= Constants.EPSLN) || (Math.abs(con - ((Math.PI)/2)) <= Constants.EPSLN))
        {
            throw(new ProjException(202, "HotineObMerc._init()"));
           
        }
        else 
            if (Math.abs(Math.abs(m_center.lat) - ((Math.PI)/2)) <= Constants.EPSLN)
            {
                throw(new ProjException(202, "HotineObMerc._init()"));
               
            }

             m_singam = Math.sin(gama);
             m_cosgam = Math.cos(gama);

             m_sinaz = Math.sin(m_azimuth);
             m_cosaz = Math.cos(m_azimuth);

            if (m_center.lat>= 0)
                m_u =  (m_al/m_bl) * Math.atan(Math.sqrt(m_d * m_d - 1.0)/m_cosaz);
            else
                m_u = -(m_al/m_bl) * Math.atan(Math.sqrt(m_d * m_d - 1.0)/m_cosaz);
    }
}


protected void _loadFromParams() throws ProjException
{
    try
    {
        super._loadFromParams();
        setScaleFactor(m_gctpParams[2]);
        setMode((int)m_gctpParams[12]);
        
    
        if(mode() == 1) 
            setAzimuth(m_gctpParams[3]);
        
        else
        {
            setLon1(m_gctpParams[8]);
            setLat1(m_gctpParams[9]);
            setLon2(m_gctpParams[10]);
            setLat2(m_gctpParams[11]);
        }
    }
    
    catch(ProjException e)
    {
        e.addFunc("HotineObMerc._loadFromParams()");
        throw(e);
    }
}


protected void _inverse(CoordPoint p) throws ProjException
{
    
    double theta;       /* angle                    */
    double t;   /* temporary values             */
    double con; /* cone constant, small m           */
    double vs,us,q,s,ts1;
    double vl,ul;
    

    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;

    vs = p.x * m_cosaz - p.y * m_sinaz;
    us = p.y * m_cosaz + p.x * m_sinaz;
    us = us + m_u;
    q = Math.exp(-m_bl * vs / m_al);
    s = .5 * (q - 1.0/q);
    t = .5 * (q + 1.0/q);
    vl = Math.sin(m_bl * us / m_al);
    ul = (vl * m_cosgam + s * m_singam)/t;
    if (Math.abs(Math.abs(ul) - 1.0) <= Constants.EPSLN)
    {
        m_lonLat.lon = m_center.lon;
        if (ul >= 0.0)
            m_lonLat.lat = ((Math.PI)/2);
        else
            m_lonLat.lat = -((Math.PI)/2);
    }
    else
    {
        con = 1.0 / m_bl;
        ts1 = Math.pow((m_el / Math.sqrt((1.0 + ul) / (1.0 - ul))),con);
        try
        {
            m_lonLat.lat = Util.phi2z(m_e,ts1);
        }
        
        catch(ProjException e)
        {
            e.addFunc("HotineObMerc._inverse()");
            throw(e);
            
        }

        con = Math.cos(m_bl * us /m_al);
        theta = m_center.lon - Math.atan2((s * m_cosgam - vl * m_singam) , con)/m_bl;
        m_lonLat.lon = Util.adjust_lon(theta);
    }
}


protected void _forward(GeoPoint p) throws ProjException
{
   
    double sin_phi;/* sin and cos value             */
    double t;   /* temporary values             */
    double con; /* cone constant, small m           */
    double q,us,vl;
    double ul,vs;
    double s;
    double dlon;
    double ts1;

    /* Forward equations
    -----------------*/
    sin_phi = Math.sin(p.lat);
    dlon = Util.adjust_lon(p.lon - m_center.lon);
    vl = Math.sin(m_bl * dlon);
    if (Math.abs(Math.abs(p.lat) - ((Math.PI)/2)) > Constants.EPSLN)
    {
        ts1 = Util.tsfnz(m_e,p.lat,sin_phi);
        q = m_el / (Math.pow(ts1,m_bl));
        s = .5 * (q - 1.0 / q);
        t = .5 * (q + 1.0/ q);
        ul = (s * m_singam - vl * m_cosgam) / t;
        con = Math.cos(m_bl * dlon);
        if (Math.abs(con) < .0000001)
        {
            us = m_al * m_bl * dlon;
        }
        else
        {
            us = m_al * Math.atan((s * m_cosgam + vl * m_singam) / con)/m_bl;
            if (con < 0)
                us = us + Math.PI * m_al / m_bl;
        }
    }
    else
    {
        if (p.lat >= 0)
            ul = m_singam;
        else
            ul = -m_singam;

        us = m_al * p.lat / m_bl;
    }
    if (Math.abs(Math.abs(ul) - 1.0) <= Constants.EPSLN)
    {
        throw(new ProjException(205, "HotineObMerc._forward()"));
      
    }

    vs = .5 * m_al * Math.log((1.0 - ul)/(1.0 + ul)) / m_bl;
    us = us - m_u;
    m_xy.x = m_falseEasting + vs * m_cosaz + us * m_sinaz;
    m_xy.y = m_falseNorthing + us * m_cosaz - vs * m_sinaz;
}



}