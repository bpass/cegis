package JGCTP;

public class AlbersConEqArea extends Projection
{

protected   double m_c;

protected   double m_e;

protected   double m_es;

protected   double m_rh;

protected   double m_ns0;

public 	AlbersConEqArea()
{
    super();
    m_c = 0;
    m_e = 0;
    m_es = 0;
    m_rh = 0;
    m_ns0 = 0;
    setNumber(ProjCode.ALBERS);
    setName("Albers Conical Equal-Area");
    
}


public 	AlbersConEqArea(double gctpParameters[], ProjUnit units, ProjDatum dat)
{
    super(gctpParameters, units, dat);
    m_c = 0;
    m_e = 0;
    m_es = 0;
    m_rh = 0;
    m_ns0 = 0;
    setNumber(ProjCode.ALBERS);
    setName("Albers Conical Equal-Area");
}


protected   void _init() throws ProjException
{
    double sin_po,cos_po;       /* sine and cos values          */
    double con;                 /* temporary variable           */
    double temp;                /* temporary variable           */
    double ms1;                 /* small m 1                            */
    double ms2;                 /* small m 2                            */
    double qs0;                 /* small q 0                            */
    double qs1;                 /* small q 1                            */
    double qs2;                 /* small q 2                            */
    

    if (Math.abs(m_stdParallelLat1 + m_stdParallelLat2) < Constants.EPSLN) {
        throw(new ProjException(31, "AlbersConEqArea._init()"));
       
    }

    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e =Math.sqrt(m_es);

     sin_po = Math.sin(m_stdParallelLat1);
     cos_po = Math.cos(sin_po);

    con = sin_po;

    ms1 = Util.msfnz(m_e,sin_po, cos_po);
    qs1 = Util.qsfnz(m_e,sin_po);

     sin_po = Math.sin(m_stdParallelLat2);
     cos_po = Math.cos(sin_po);


    ms2 = Util.msfnz(m_e,sin_po, cos_po);
    qs2 = Util.qsfnz(m_e,sin_po);

     sin_po = Math.sin(m_center.lat);
     cos_po = Math.cos(sin_po);


    qs0 = Util.qsfnz(m_e,sin_po);

    if( Math.abs(m_stdParallelLat1 - m_stdParallelLat2) > Constants.EPSLN)
        m_ns0 = (ms1 * ms1 - ms2 *ms2)/ (qs2 - qs1);

    else
        m_ns0 = con;

    m_c = ms1 * ms1 + m_ns0 * qs1;
    m_rh = m_sphere.rMajor *Math.sqrt(m_c - m_ns0 * qs0)/m_ns0;

}


protected void _forward(GeoPoint p) throws ProjException
{
    double sin_phi;     /* sine and cos values      */
    double qs;          /* small q          */
    double theta;           /* angle            */ 
    double rh1;         /* height above ellipsoid   */

    sin_phi = Math.sin(p.lat);
    
    qs = Util.qsfnz(m_e,sin_phi);
    rh1 = m_sphere.rMajor *Math.sqrt(m_c - m_ns0 * qs)/m_ns0;
    theta = m_ns0 * Util.adjust_lon(p.lon - m_center.lon); 
    m_xy.x = rh1 *Math.sin(theta) + m_falseEasting;
    m_xy.y = m_rh - rh1 *Math.cos(theta) + m_falseNorthing; 

}


protected void _inverse(CoordPoint p) throws ProjException
{
    double rh1;         /* height above ellipsoid   */
    double qs;          /* function q           */
    double con;         /* temporary sign value     */
    double theta;           /* angle            */
    long   flag;            /* error flag;          */
    
    flag = 0;
    p.x -= m_falseEasting;
    p.y = m_rh - p.y + m_falseNorthing;;
    
    if (m_ns0 >= 0)
    {
        rh1 =Math.sqrt(p.x * p.x + p.y * p.y);
        con = 1.0;
    }
    
    else
    {
        rh1 = Math.sqrt(p.x * p.x + p.y * p.y);
        con = -1.0;
    }

    theta = 0.0;
    if (rh1 != 0.0)
        theta =Math.atan2(con * p.x, con * p.y);
    
    con = rh1 * m_ns0 / m_sphere.rMajor;
    qs = (m_c - con * con) / m_ns0;
    
    if (m_e >= 1e-10)
    {
        con = 1 - .5 * (1.0 - m_es) * Math.log((1.0 - m_e) / (1.0 + m_e))/m_e;
        if (Math.abs(Math.abs(con) -Math.abs(qs)) > .0000000001 )
        {
            try 
            {
                m_lonLat.lat = Util.phi1z(m_e,qs);
            }
            
            catch(ProjException e)
            {
                e.addFunc("AlbersConEqArea._inverse()");
                throw(e);
            }
               
        }

        else
    
        {
         if (qs >= 0)
            m_lonLat.lat = .5 * Math.PI;
         else
            m_lonLat.lat = -.5 * Math.PI;
        }
    }
    
    else
    {
        try
        {
            m_lonLat.lat = Util.phi1z(m_e,qs);
        }
        
        catch(ProjException e)
        {
            e.addFunc("AlbersConEqArea._inverse()");
            throw(e);
        }
    }

    m_lonLat.lon = Util.adjust_lon(theta/m_ns0 + m_center.lon);
}



}