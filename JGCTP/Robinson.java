package JGCTP;

public class Robinson extends Projection
{

    public double m_pr[];
    public double m_xlr[];
    
public 	Robinson()
{
    super();
    setName("Robinson");
    setNumber(ProjCode.ROBIN);
    
    m_pr = new double[21];
    m_xlr = new double[21];
    
    m_pr[1]= -0.062; 
    m_xlr[1]=0.9986; 
    m_pr[2]=0.0; 
    m_xlr[2]=1.0; 
    m_pr[3]=0.062; 
    m_xlr[3]=0.9986;
    m_pr[4]=0.124;   
    m_xlr[4]=0.9954;  
    m_pr[5]=0.186;
    m_xlr[5]=0.99; 
    m_pr[6]=0.248;  
    m_xlr[6]=0.9822; 
    m_pr[7]=0.31;     
    m_xlr[7]=0.973;    
    m_pr[8]=0.372;      
    m_xlr[8]=0.96;
    m_pr[9]=0.434; 
    m_xlr[9]=0.9427;
    m_pr[10]=0.4958;
    m_xlr[10]=0.9216;
    m_pr[11]=0.5571;  
    m_xlr[11]=0.8962;  
    m_pr[12]=0.6176;
    m_xlr[12]=0.8679;
    m_pr[13]=0.6769; 
    m_xlr[13]=0.835;  
    m_pr[14]=0.7346;
    m_xlr[14]=0.7986;
    m_pr[15]=0.7903;  
    m_xlr[15]=0.7597;  
    m_pr[16]=0.8435;
    m_xlr[16]=0.7186;
    m_pr[17]=0.8936; 
    m_xlr[17]=0.6732; 
    m_pr[18]=0.9394; 
    m_xlr[18]=0.6213;
    m_pr[19]=0.9761;  
    m_xlr[19]=0.5722;  
    m_pr[20]=1.0;  
    m_xlr[20]=0.5322;

    for (int i = 0; i < 21; i++)
        m_xlr[i] *= 0.9858;
}


public 	Robinson(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Robinson");
    setNumber(ProjCode.ROBIN);
    
    m_pr = new double[21];
    m_xlr = new double[21];
    
    m_pr[1]= -0.062; 
    m_xlr[1]=0.9986; 
    m_pr[2]=0.0; 
    m_xlr[2]=1.0; 
    m_pr[3]=0.062; 
    m_xlr[3]=0.9986;
    m_pr[4]=0.124;   
    m_xlr[4]=0.9954;  
    m_pr[5]=0.186;
    m_xlr[5]=0.99; 
    m_pr[6]=0.248;  
    m_xlr[6]=0.9822; 
    m_pr[7]=0.31;     
    m_xlr[7]=0.973;    
    m_pr[8]=0.372;      
    m_xlr[8]=0.96;
    m_pr[9]=0.434; 
    m_xlr[9]=0.9427;
    m_pr[10]=0.4958;
    m_xlr[10]=0.9216;
    m_pr[11]=0.5571;  
    m_xlr[11]=0.8962;  
    m_pr[12]=0.6176;
    m_xlr[12]=0.8679;
    m_pr[13]=0.6769; 
    m_xlr[13]=0.835;  
    m_pr[14]=0.7346;
    m_xlr[14]=0.7986;
    m_pr[15]=0.7903;  
    m_xlr[15]=0.7597;  
    m_pr[16]=0.8435;
    m_xlr[16]=0.7186;
    m_pr[17]=0.8936; 
    m_xlr[17]=0.6732; 
    m_pr[18]=0.9394; 
    m_xlr[18]=0.6213;
    m_pr[19]=0.9761;  
    m_xlr[19]=0.5722;  
    m_pr[20]=1.0;  
    m_xlr[20]=0.5322;

    for (int i = 0; i < 21; i++)
        m_xlr[i] *= 0.9858;
}


protected 	void _init() throws ProjException
{
    return;
}


protected void _inverse(CoordPoint p) throws ProjException
{
    double yy;
    double p2;
    double u,v,t,c;
    double phid;
    double y1;
    int ip1;
    int i;


    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;

    yy = 2.0 * p.y / Math.PI / m_sphere.radius;
    phid = yy * 90.0;
    p2 = Math.abs(phid / 5.0);
    ip1 = (int)(p2 - Constants.EPSLN);
    if (ip1 == 0)
    ip1 = 1;

    /* Stirling's interpolation formula as used in forward transformation is 
    reversed for first estimation of LAT. from rectangular coordinates. LAT.
    is then adjusted by iteration until use of forward series provides correct 
    value of Y within tolerance.
    ---------------------------------------------------------------------------*/
    for (i = 0;;)
    {
        u = m_pr[ip1 + 3] - m_pr[ip1 + 1];
        v = m_pr[ip1 + 3] - 2.0 * m_pr[ip1 + 2] + m_pr[ip1 + 1];
        t = 2.0 * (Math.abs(yy) - m_pr[ip1 + 2]) / u;
        c = v / u;
        p2 = t * (1.0 - c * t * (1.0 - 2.0 * c * t));

        if ((p2 >= 0.0) || (ip1 == 1))
            {
            if (p.y >= 0)
                phid = (p2 + (double) ip1 ) * 5.0;
            else
                phid = -(p2 + (double) ip1 ) * 5.0;
              
            do
            {
                p2 = Math.abs(phid / 5.0);
                ip1 = (int) (p2 - Constants.EPSLN);
                p2 -= (double) ip1;
         
                if (p.y >= 0)
                y1 = m_sphere.radius * (m_pr[ip1 +2] + p2 *(m_pr[ip1 + 3] - m_pr[ip1 +1]) / 2.0 + p2 
                        * p2 * (m_pr[ip1 + 3] - 2.0 * m_pr[ip1 + 2] + m_pr[ip1 + 1])/2.0) 
                        * Math.PI / 2.0; 
                else
                y1 = -m_sphere.radius * (m_pr[ip1 +2] + p2 *(m_pr[ip1 + 3] - m_pr[ip1 +1]) / 2.0 + p2 
                        * p2 * (m_pr[ip1 + 3] - 2.0 * m_pr[ip1 + 2] + m_pr[ip1 + 1])/2.0) 
                        * Math.PI / 2.0; 
                phid += -180.0 * (y1 - p.y) / Math.PI / m_sphere.radius;
                i++;
                if (i > 75)
                {
                    throw(new ProjException(234, "Robinson._inverse()"));
                    
                }
            }
        while (Math.abs(y1 - p.y) > .00001);
        break;
        }
    else
    {
        ip1 -= 1;
        if (ip1 < 0)
        {
            throw(new ProjException(234, "Robinson._inverse()"));
           
        }
    }
  }
    m_lonLat.lat  = phid * .01745329252;

    /* calculate  LONG. using final LAT. with transposed forward Stirling's 
    interpolation formula.
    ---------------------------------------------------------------------*/
    m_lonLat.lon = m_center.lon + p.x / m_sphere.radius / (m_xlr[ip1 + 2] + p2 * (m_xlr[ip1 + 3] - m_xlr[ip1 + 1])
                        / 2.0 + p2 * p2 * (m_xlr[ip1 + 3] - 2.0 * m_xlr[ip1 + 2] + 
                        m_xlr[ip1 + 1]) / 2.0);
    m_lonLat.lon = Util.adjust_lon(m_lonLat.lon);
}


protected void _forward(GeoPoint p) throws ProjException
{
    double dlon;
    double p2;
    int ip1;

    /* Forward equations
    -----------------*/
    dlon = Util.adjust_lon(p.lon - m_center.lon);
    p2 = Math.abs(p.lat / 5.0 / .01745329252);
    ip1 = (int) (p2 - Constants.EPSLN);

    /* Stirling's interpolation formula (using 2nd Diff.)
    ---------------------------------------------------*/
    p2 -= (double) ip1;
    m_xy.x = m_sphere.radius * (m_xlr[ip1 + 2] + p2 * (m_xlr[ip1 + 3] - m_xlr[ip1 + 1]) / 2.0 +
            p2 * p2 * (m_xlr[ip1 + 3] - 2.0 * m_xlr[ip1 + 2] + m_xlr[ip1 + 1])/2.0) * 
            dlon + m_falseEasting;

    if (p.lat >= 0)
        m_xy.y = m_sphere.radius * (m_pr[ip1 + 2] + p2 * (m_pr[ip1 + 3] - m_pr[ip1 +1]) / 2.0 + p2 * p2 *
                    (m_pr[ip1 + 3] - 2.0 * m_pr[ip1 + 2] + m_pr[ip1 + 1]) / 2.0) * Math.PI / 2.0 +
                m_falseNorthing;
    else
         m_xy.y = -m_sphere.radius * (m_pr[ip1 + 2] + p2 * (m_pr[ip1 + 3] - m_pr[ip1 +1]) / 2.0 + p2 * p2 *
             (m_pr[ip1 + 3] - 2.0 * m_pr[ip1 + 2] + m_pr[ip1 + 1]) / 2.0) * Math.PI / 2.0 + m_falseNorthing;
}



}