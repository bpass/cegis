package jgctp;

public class EquidistantC extends Projection
{

protected 	double m_e;

protected 	double m_es;

protected 	double m_e0;

protected 	double m_e1;

protected 	double m_e2;

protected 	double m_e3;

protected 	double m_ml0;

protected 	double m_ns;

protected 	double m_g;

protected 	double m_rh;

protected 	int m_mode;

public 	EquidistantC()
{
    super();
    setNumber(ProjCode.EQUIDC);
    setName("Eqidistant Conic");
    m_e = 0;
    m_es = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_ml0 = 0;
    m_ns = 0;
    m_g = 0;
    m_rh = 0;
    m_mode = 0;
}


public 	EquidistantC(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.EQUIDC);
    setName("Eqidistant Conic");
    setParamLoad(true);
    m_e = 0;
    m_es = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_ml0 = 0;
    m_ns = 0;
    m_g = 0;
    m_rh = 0;
    m_mode = 0;
}


public 	void setMode(int mode) 
{
     m_mode = mode;
     setInit(true);
}

public 	int mode() 
{
    return(m_mode);
}

protected 	void _init() throws ProjException
{
    double temp;            /* temporary variable       */
    double sinphi,cosphi;       /* sin and cos values       */
    double ms1,ms2;
    double ml1,ml2;

    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);
    m_e0 = Util.e0fn(m_es);
    m_e1 = Util.e0fn(m_es);
    m_e2 = Util.e0fn(m_es);
    m_e3 = Util.e0fn(m_es);

     sinphi = Math.sin(m_stdParallelLat1);
     cosphi = Math.cos(m_stdParallelLat1);

    ms1 = Util.msfnz(m_e,sinphi,cosphi);
    ml1 = Util.mlfn(m_e0, m_e1, m_e2, m_e3, m_stdParallelLat1);
    
    if (m_mode != 0)
    {
        if (Math.abs(m_stdParallelLat1 + m_stdParallelLat2) < Constants.EPSLN) 
        {
            throw(new ProjException(81, "EquidistantC._init()"));
            
        }
        
         sinphi = Math.sin(m_stdParallelLat2);
         cosphi = Math.cos(m_stdParallelLat2);

        ms2 = Util.msfnz(m_e,sinphi,cosphi);
        ml2 = Util.mlfn(m_e0, m_e1, m_e2, m_e3, m_stdParallelLat2);
        if (Math.abs(m_stdParallelLat1 - m_stdParallelLat2) >= Constants.EPSLN)
            m_ns = (ms1 - ms2) / (ml2 - ml1);
        else
            m_ns = sinphi;
    }
    else
        m_ns = sinphi;  

    m_g = ml1 + ms1/m_ns;
    m_ml0 = Util.mlfn(m_e0, m_e1, m_e2, m_e3, m_center.lat);
    m_rh = m_sphere.rMajor * (m_g - m_ml0);

}


protected 	void _loadFromParams() throws ProjException
{
    try 
    {
        super._loadFromParams();
        setMode((int)m_gctpParams[8]);
    }
    
    catch(ProjException e)
    {
        e.addFunc("EquidistantC._loadFromParams()");
        throw(e);
    }

}


protected void _forward(GeoPoint p)
{

    double ml;
    double theta;
    double rh1;
    
    /* Forward equations
    -----------------*/
    ml = Util.mlfn(m_e0, m_e1, m_e2, m_e3, p.lat);
    rh1 = m_sphere.rMajor * (m_g - ml);
    theta = m_ns * Util.adjust_lon(p.lon - m_center.lon);
    m_xy.x = m_falseEasting  + rh1 * Math.sin(theta);
    m_xy.y = m_falseNorthing + m_rh - rh1 * Math.cos(theta);
}


protected void _inverse(CoordPoint p) throws ProjException
{
   
    double rh1;
    double ml;
    double con;
    double theta;
   
    p.x -= m_falseEasting;
    p.y  = m_rh - p.y + m_falseNorthing;
    if (m_ns >= 0)
    {
        rh1 = Math.sqrt(p.x * p.x + p.y * p.y); 
        con = 1.0;
    }
    else
    {
        rh1 = -Math.sqrt(p.x * p.x + p.y * p.y); 
        con = -1.0;
    }

    theta = 0.0;
    if (rh1  != 0.0)
        theta = Math.atan2(con * p.x, con * p.y);
    
    ml = m_g - rh1 / m_sphere.rMajor;
    try 
    {
        m_lonLat.lat = Util.phi3z(ml,m_e0,m_e1,m_e2,m_e3);
    }
    
    catch(ProjException e)
    {
        e.addFunc("EquidistantC._inverse()");
        throw(e);   
    }

    m_lonLat.lon = Util.adjust_lon(m_center.lon + theta / m_ns);
}



}