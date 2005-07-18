package JGCTP;

public class Mercator extends Projection
{

protected 	double m_e;

protected 	double m_es;

protected 	double m_m1;

public 	Mercator()
{
    super();
    setName("Mercator");
    setNumber(ProjCode.MERCAT);
    m_e = 0;
    m_es = 0;
    m_m1 = 0;
    
    
}


public 	Mercator(double gctpParams[],  ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Mercator");
    setNumber(ProjCode.MERCAT);
    m_e = 0;
    m_es = 0;
    m_m1 = 0;
}


protected 	void _init() throws ProjException
{
    double temp;            /* temporary variable       */

    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);
    m_m1 = Math.cos(m_center.lat)/(Math.sqrt(1.0 - m_es * Math.sin(m_center.lat) * Math.sin(m_center.lat)));

}


protected void _forward(GeoPoint p) throws ProjException
{
    double ts;      /* small t value                */
    double sinphi;      /* sin value                    */


    /* Forward equations
     -----------------*/
    if (Math.abs(Math.abs(p.lat) - ((Math.PI)/2))  <= Constants.EPSLN) {
        throw(new ProjException(52, "Mercator._forward()"));
       
    }

    else
     {
        sinphi = Math.sin(p.lat);
        ts = Util.tsfnz(m_e,p.lat,sinphi);
        m_xy.x = m_falseEasting + m_sphere.rMajor * m_m1 * Util.adjust_lon(p.lon - m_center.lon);
        m_xy.y = m_falseNorthing - m_sphere.rMajor * m_m1 * Math.log(ts);
     }

}


protected void _inverse(CoordPoint p) throws ProjException
{
    double ts;      /* small t value                */
   
    /* Inverse equations
    -----------------*/

    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    ts = Math.exp(-p.y/(m_sphere.rMajor * m_m1));
    
    try
    {
        m_lonLat.lat = Util.phi2z(m_e,ts);
    }
    
    catch(ProjException e)
    {
        e.addFunc("Mercator._inverse()");
        throw(e);
      
    }

    m_lonLat.lon = Util.adjust_lon(m_center.lon + p.x/(m_sphere.rMajor * m_m1));
}



}