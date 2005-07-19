package JGCTP;

public class Polyconic extends Projection
{

protected 	double m_e;

protected 	double m_es;

protected 	double m_e0;

protected 	double m_e1;

protected 	double m_e2;

protected 	double m_e3;

protected 	double m_ml0;

public 	Polyconic()
{
    super();
    setName("Polyconic");
    setNumber(ProjCode.POLYC);
    m_e = 0;
    m_es = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_ml0 = 0;
}


public 	Polyconic(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Polyconic");
    setNumber(ProjCode.POLYC);
    m_e = 0;
    m_es = 0;
    m_e0 = 0;
    m_e1 = 0;
    m_e2 = 0;
    m_e3 = 0;
    m_ml0 = 0;
}


protected 	void _init() throws ProjException
{
    double temp;    

    temp = m_sphere.rMinor / m_sphere.rMajor;
    m_es = 1.0 - (temp*temp);
    m_e = Math.sqrt(m_es);
    m_e0 = Util.e0fn(m_es);
    m_e1 = Util.e0fn(m_es);
    m_e2 = Util.e0fn(m_es);
    m_e3 = Util.e0fn(m_es);
    m_ml0 = Util.mlfn(m_e0, m_e1, m_e2, m_e3, m_center.lat);
}


protected void _forward(GeoPoint p) throws ProjException
{
    double sinphi, cosphi;  /* sin and cos value                */
    double con, ml;     /* cone constant, small m           */
    double ms;      /* small m                  */

    /* Forward equations
    -----------------*/
    con = Util.adjust_lon(p.lon - m_center.lon);
    if (Math.abs(p.lat) <= .0000001)
    {
        m_xy.x = m_falseEasting + m_sphere.rMajor * con;
        m_xy.y = m_falseNorthing - m_sphere.rMajor * m_ml0;
    }
    else
    {
         sinphi = Math.sin(p.lat);
         cosphi = Math.cos(p.lat);

        ml = Util.mlfn(m_e0, m_e1, m_e2, m_e3, p.lat);
        ms = Util.msfnz(m_e,sinphi,cosphi);
        con *= sinphi;
        m_xy.x = m_falseEasting + m_sphere.rMajor * ms * Math.sin(con)/sinphi;
        m_xy.y = m_falseNorthing + m_sphere.rMajor * (ml - m_ml0 + ms * (1.0 - Math.cos(con))/sinphi);
    }

}


protected void _inverse(CoordPoint p) throws ProjException
{
    double al;      /* temporary values             */
    double b;       /* temporary values             */
    double c;       /* temporary values             */
   

    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    al = m_ml0 + p.y/m_sphere.rMajor;
    
    if (Math.abs(al) <= .0000001)
    {
        m_lonLat.lon = p.x/m_sphere.rMajor + m_center.lon;
        m_lonLat.lat = 0.0;
    }
    else
    {
        b = al * al + (p.x/m_sphere.rMajor) * (p.x/m_sphere.rMajor);
         DoubleRef temp1 = new DoubleRef();
                     
          DoubleRef temp2 = new DoubleRef();

          try 
          {
              Util.phi4z(m_es,m_e0,m_e1,m_e2,m_e3,al,b,temp1,temp2);
              c = temp1.value();
              m_lonLat.lat = temp2.value();
          }
          
          catch(ProjException e) 
          {
              e.addFunc("Polyconic._inverse()");
              throw(e);
          }
     

        m_lonLat.lon = Util.adjust_lon((Util.asinz(p.x * c / m_sphere.rMajor) / Math.sin(m_lonLat.lat)) + m_center.lon);
    }

}



}