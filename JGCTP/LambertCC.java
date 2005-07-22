package jgctp;

public class LambertCC extends Projection
{

protected 	double m_es;

protected 	double m_e;

protected 	double m_ns;

protected 	double m_f0;

protected 	double m_rh;

public 	LambertCC()
{
    super();
    setNumber(ProjCode.LAMCC);
    setName("Lambert Conformal Conic");
    m_es = 0;
    m_e = 0;
    m_ns = 0;
    m_f0 = 0;
    m_rh = 0;
}


public 	LambertCC(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setNumber(ProjCode.LAMCC);
    setName("Lambert Conformal Conic");
    m_es = 0;
    m_e = 0;
    m_ns = 0;
    m_f0 = 0;
    m_rh = 0;
}


protected 	void _init() throws ProjException
{
    double sin_po;                  /* sin value                            */
    double cos_po;                  /* cos value                            */
    double con;                     /* temporary variable                   */
    double ms1;                     /* small m 1                            */
    double ms2;                     /* small m 2                            */
    double temp;                    /* temporary variable                   */
    double ts0;                     /* small t 0                            */
    double ts1;                     /* small t 1                            */
    double ts2;                     /* small t 2                            */

    /* Standard Parallels cannot be equal and on opposite sides of the equator
    ------------------------------------------------------------------------*/
    if (Math.abs(m_stdParallelLat1+m_stdParallelLat2) < Constants.EPSLN)
    {
        throw(new ProjException(41, "LambertCC._init()"));
       
    }
       
    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);

     sin_po = Math.sin(m_stdParallelLat1);
     cos_po = Math.cos(m_stdParallelLat1);

    con = sin_po;
    ms1 = Util.msfnz(m_e,sin_po,cos_po);
    ts1 = Util.tsfnz(m_e,m_stdParallelLat1,sin_po);
     sin_po = Math.sin(m_stdParallelLat2);
     cos_po = Math.cos(m_stdParallelLat2);

    ms2 = Util.msfnz(m_e,sin_po,cos_po);
    ts2 = Util.tsfnz(m_e,m_stdParallelLat2,sin_po);
    sin_po = Math.sin(m_center.lat);
    ts0 = Util.tsfnz(m_e,m_center.lat,sin_po);

    if (Math.abs(m_stdParallelLat1 - m_stdParallelLat2) > Constants.EPSLN)
        m_ns = Math.log (ms1/ms2)/ Math.log (ts1/ts2);
    else
        m_ns = con;
    m_f0 = ms1 / (m_ns * Math.pow(ts1,m_ns));
    m_rh = m_sphere.rMajor * m_f0 * Math.pow(ts0,m_ns);
}


protected void _forward(GeoPoint p) throws ProjException
{
    double con;                     /* temporary angle variable             */
    double rh1;                     /* height above ellipsoid               */
    double sinphi;                  /* sin value                            */
    double theta;                   /* angle                                */
    double ts;                      /* small value t                        */
    
    con  = Math.abs( Math.abs(p.lat) - ((Math.PI)/2));
    if (con > Constants.EPSLN)
    {
        sinphi = Math.sin(p.lat);
        ts = Util.tsfnz(m_e,p.lat,sinphi);
        rh1 = m_sphere.rMajor * m_f0 * Math.pow(ts,m_ns);
    }
    else
    {
        con = p.lat * m_ns;
        if (con <= 0)
        {
            throw(new ProjException(44, "LambertCC._forward()"));
           
        }
        rh1 = 0;
    }

    theta = m_ns * Util.adjust_lon(p.lon - m_center.lon);
    m_xy.x = rh1 * Math.sin(theta) + m_falseEasting;
    m_xy.y = m_rh - rh1 * Math.cos(theta) + m_falseNorthing;    
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double rh1;         /* height above ellipsoid   */
    double con;         /* sign variable        */
    double ts;          /* small t          */
    double theta;           /* angle            */
   
    p.x -= m_falseEasting;
    p.y = m_rh - p.y + m_falseNorthing;
    if (m_ns > 0)
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
    if (rh1 != 0)
        theta = Math.atan2((con * p.x),(con * p.y));
    if ((rh1 != 0) || (m_ns > 0.0))
    {
        con = 1.0/m_ns;
        ts = Math.pow((rh1/(m_sphere.rMajor * m_f0)),con);
        try
        {
            m_lonLat.lat = Util.phi2z(m_e,ts);
        }
        catch(ProjException e)
        {
            e.addFunc("LambertCC._inverse()");
            throw(e);  
        }
    }
    else
        m_lonLat.lat = -((Math.PI)/2);
    
    m_lonLat.lon = Util.adjust_lon(theta/m_ns + m_center.lon);
}



}