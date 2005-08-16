package jgctp;

public class PolarStereo extends Projection
{

protected 	double m_es;

protected 	double m_e;

protected 	double m_e4;

protected 	double m_fac;

protected 	double m_ind;

protected 	double m_mcs;

protected 	double m_tcs;

public 	PolarStereo()
{
    super();
    setName("Polar Stereographic");
    setNumber(ProjCode.PS);
    m_es = 0;
    m_e = 0;
    m_e4 = 0;
    m_fac = 0;
    m_ind = 0;
    m_mcs = 0;
    m_tcs = 0;
}


public 	PolarStereo(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    m_es = 0;
    m_e = 0;
    m_e4 = 0;
    m_fac = 0;
    m_ind = 0;
    m_mcs = 0;
    m_tcs = 0;
}


protected 	void _init() throws ProjException
{
    double temp;                /* temporary variable       */
    double con1;                /* temporary angle      */
    double sinphi;              /* sin value            */
    double cosphi;              /* cos value            */

    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);
    m_e4 = Util.e0fn(m_e);

    if (m_center.lat < 0)
        m_fac = -1.0;
    else
        m_fac = 1.0;
    
    m_ind = 0;
    if (Math.abs(Math.abs(m_center.lat) - ((Constants.PI)/2)) > Constants.EPSLN)
    {
        m_ind = 1;
        con1 = m_fac * m_center.lat; 
         sinphi = Math.sin(con1);
         cosphi = Math.cos(con1);

        m_mcs = Util.msfnz(m_e,sinphi,cosphi);
        m_tcs = Util.tsfnz(m_e,con1,sinphi);
    }
}


protected void _forward(GeoPoint p) throws ProjException
{
    double con1;            /* adjusted longitude       */
    double con2;            /* adjusted latitude        */
    double rh;          /* height above ellipsoid   */
    double sinphi;          /* sin value            */
    double ts;          /* value of small t     */

    con1 = m_fac * Util.adjust_lon(p.lon - m_center.lon);
    con2 = m_fac * p.lat;
    sinphi = Math.sin(con2);
    ts = Util.tsfnz(m_e,con2,sinphi);
    if (m_ind != 0)
        rh = m_sphere.rMajor * m_mcs * ts / m_tcs;
    else
        rh = 2.0 * m_sphere.rMajor * ts / m_e4;

    m_xy.x = m_fac * rh * Math.sin(con1) + m_falseEasting;
    m_xy.y = -m_fac * rh * Math.cos(con1) + m_falseEasting;
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double rh;          /* height above ellipsiod   */
    double ts;          /* small value t        */
    double temp;            /* temporary variable       */
   
    p.x = (p.x - m_falseEasting) * m_fac;
    p.y = (p.y - m_falseNorthing) * m_fac;
    rh = Math.sqrt(p.x * p.x + p.y * p.y);
    if (m_ind != 0)
        ts = rh * m_tcs/(m_sphere.rMajor * m_mcs);
    else
        ts = rh * m_e4 / (m_sphere.rMajor * 2.0);

    try
    {
        m_lonLat.lat = m_fac * Util.phi2z(m_e,ts);
    }
   catch(ProjException e) 
   {
       e.addFunc("PolarStereo._inverse()");
       throw(e);
     
    }
    
    if (rh == 0)
        m_lonLat.lon = m_fac * m_center.lon;
    else
    {
        temp = Math.atan2(p.x, -p.y);
        m_lonLat.lon = Util.adjust_lon(m_fac *temp + m_center.lon);
    }
}



}