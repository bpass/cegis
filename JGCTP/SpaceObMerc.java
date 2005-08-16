package jgctp;

public class SpaceObMerc extends Projection
{

protected 	long m_satNum;

protected 	long m_path;

protected 	long m_mode;

protected 	double m_alf;

protected 	double m_time;

protected 	double m_a;

protected 	double m_b;

protected 	double m_a2;

protected 	double m_a4;

protected 	double m_c1;

protected 	double m_c3;

protected 	double m_q;

protected 	double m_t;

protected 	double m_u;

protected 	double m_w;

protected 	double m_xj;

protected 	double m_p21;

protected 	double m_sa;

protected 	double m_ca;

protected 	double m_es;

protected 	double m_s;

protected 	double m_start;

protected 	double m_centerLon;

public 	SpaceObMerc()
{
    super();
    m_satNum = 0; 
    m_path = 0; 
    m_mode = 0;
    m_alf = 0.0; 
    m_time = 0.0;
    m_a = 0.0; 
    m_b = 0.0;
    m_a2 = 0.0; 
    m_a4 = 0.0; 
    m_c1 = 0.0;
    m_c3 = 0.0; 
    m_q = 0.0; 
    m_t = 0.0;
    m_u = 0.0; 
    m_w = 0.0; 
    m_xj = 0.0;
    m_p21 = 0.0; 
    m_sa = 0.0; 
    m_ca = 0.0;
    m_es = 0.0; 
    m_s = 0.0; 
    m_start = 0.0;
    setNumber(ProjCode.SOM);
    setName("Space Oblique Mercator");
}


public 	SpaceObMerc(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    m_satNum = 0; 
    m_path = 0; 
    m_mode = 0;
    m_alf = 0.0; 
    m_time = 0.0;
    m_a = 0.0; 
    m_b = 0.0;
    m_a2 = 0.0; 
    m_a4 = 0.0; 
    m_c1 = 0.0;
    m_c3 = 0.0; 
    m_q = 0.0; 
    m_t = 0.0;
    m_u = 0.0; 
    m_w = 0.0; 
    m_xj = 0.0;
    m_p21 = 0.0; 
    m_sa = 0.0; 
    m_ca = 0.0;
    m_es = 0.0; 
    m_s = 0.0; 
    m_start = 0.0;
    setNumber(ProjCode.SOM);
    setName("Space Oblique Mercator");
    setParamLoad(true);
}


public 	void setSatNum(long num) 
{
    m_satNum = num;
    setInit(true);
}

public 	void setPath(long path) 
{
    m_path = path;
    setInit(true);
}

public 	void setMode(long mode) 
{
    m_mode = mode;
    setInit(true);
}

public 	void setAlf(double val) throws ProjException
{
    
    Util.e0fn(val);
    m_alf = val;
    setInit(true);
   
}


public 	void setTime(double time) 
{
    m_time = time;
    setInit(true);
}

public 	void setStart(double start) 
{
    m_start = start;
    setInit(true);
}

public 	long satNum() 
{
    return(m_satNum);
}

public 	long path() 
{
    return(m_path);
}

public 	long mode() 
{
    return(m_mode);
}

public 	double alf() 
{
   return(m_alf);
}

public 	double time() 
{
    return(m_time);
}

public 	double start() 
{
    return(m_start);
}

protected 	void som_series(DoubleRef fb, DoubleRef fa2, DoubleRef fa4, DoubleRef fc1, DoubleRef fc3, DoubleRef dlam)
{
    double sd,sdsq,h,sq,fc;

    dlam.setValue(dlam.value()*0.0174532925);               /* Convert dlam to radians */
    sd=Math.sin(dlam.value());
    sdsq=sd*sd;
    m_s=m_p21*m_sa*Math.cos(dlam.value())*Math.sqrt((1.0+m_t*sdsq)/((1.0+m_w*sdsq)*(1.0+m_q*sdsq)));
    
    h = Math.sqrt((1.0+m_q*sdsq)/(1.0+m_w*sdsq))*(((1.0+m_w*sdsq)/((1.0+m_q*sdsq)*(1.0+
        m_q*sdsq)))-m_p21*m_ca);
    
    sq=Math.sqrt(m_xj*m_xj+m_s*m_s);
    
    fb.setValue(h*m_xj-m_s*m_s/sq);
    
    fa2.setValue(fb.value()*Math.cos(2.0*dlam.value()));
    
    fa4.setValue(fb.value()*Math.cos(4.0*dlam.value()));
    
    fc=m_s*(h+m_xj)/sq;
    
    fc1.setValue(fc*Math.cos(dlam.value()));
    
    fc3.setValue(fc*Math.cos(3.0* dlam.value()));
}


protected 	void _init()
{
    long i;
    double alf,e2c,e2s,one_es;
    double suma2,suma4,sumc1,sumc3,sumb;
    DoubleRef dlam,fb,fa2,fa4,fc1,fc3;
    
    dlam = new DoubleRef();
    fb = new DoubleRef();
    fa2 = new DoubleRef();
    fa4 = new DoubleRef();
    fc1 = new DoubleRef();
    fc3 = new DoubleRef();
    
    m_a = m_sphere.rMajor;
    m_b = m_sphere.rMinor;
    m_es = 1.0 - (m_sphere.rMinor/m_sphere.rMajor)*(m_sphere.rMinor/m_sphere.rMajor);

    if (m_mode != 0)
    {
        alf = m_alf;
        m_p21 = m_time/1440.0;
    }
    else
    {
        if (m_satNum < 4)
        {
            alf = 99.092 * Constants.D2R;
            m_p21=103.2669323/1440.0;
            m_center.lon = (128.87 - (360.0/251.0 * m_path)) * Constants.D2R;
        }
        else
        {
            alf = 98.2 * Constants.D2R;
            m_p21 = 98.8841202/1440.0;
            m_center.lon = (129.30 - (360.0/233.0 * m_path)) * Constants.D2R;
        }
        m_start=0.0;
    }


    m_ca=Math.cos(alf);

    if (Math.abs(m_ca)<1.e-9) 
        m_ca=1.e-9;

    m_sa=Math.sin(alf);
    e2c = m_es*m_ca*m_ca;
    e2s = m_es*m_sa*m_sa;
    m_w = (1.0-e2c)/(1.0-m_es);
    m_w = m_w*m_w-1.0;
    one_es = 1.0-m_es;
    m_q = e2s / one_es;
    m_t = (e2s*(2.0-m_es)) / (one_es*one_es);
    m_u = e2c / one_es;
    m_xj = one_es*one_es*one_es;
    dlam.setValue(0.0);
    som_series(fb,fa2,fa4,fc1,fc3,dlam);
    suma2 = fa2.value();
    suma4 = fa4.value();
    sumb = fb.value();
    sumc1 = fc1.value();
    sumc3 = fc3.value();
    for(i=9;i<=81;i+=18)
    {
        dlam.setValue(i);
        som_series(fb,fa2,fa4,fc1,fc3,dlam);
        suma2 = suma2+4.0*fa2.value();
        suma4 = suma4+4.0*fa4.value();
        sumb = sumb+4.0*fb.value();
        sumc1 = sumc1+4.0*fc1.value();
        sumc3 = sumc3+4.0*fc3.value();
    }
    for(i=18; i<=72; i+=18)
    {
        dlam.setValue(i);
        som_series(fb,fa2,fa4,fc1,fc3,dlam);
        suma2 = suma2+2.0*fa2.value();
        suma4 = suma4+2.0*fa4.value();
        sumb = sumb+2.0*fb.value();
        sumc1 = sumc1+2.0*fc1.value();
        sumc3 = sumc3+2.0*fc3.value();
    }

    dlam.setValue(90.0);
    som_series(fb,fa2,fa4,fc1,fc3,dlam);
    suma2 = suma2+fa2.value();
    suma4 = suma4+fa4.value();
    sumb = sumb+fb.value();
    sumc1 = sumc1+fc1.value();
    sumc3 = sumc3+fc3.value();
    m_a2 = suma2/30.0;
    m_a4 = suma4/60.0;
    m_b = sumb/30.0;
    m_c1 = sumc1/15.0;
    m_c3 = sumc3/45.0;
}


protected void _loadFromParams() throws ProjException
{
    try
    {
        super._loadFromParams();
        setPath((long)m_gctpParams[3]);
        setSatNum((long)m_gctpParams[2]);
        if(m_gctpParams[12] == 0) 
        {
            setMode(1);
            setAlf(m_gctpParams[3]);
            setTime(m_gctpParams[8]);
            setStart(m_gctpParams[10]);
        }
    
        else
            setMode(0);
    }
    
    catch(ProjException e)
    {
        throw(e);
    }
}


protected void _inverse(CoordPoint _p) throws ProjException
{
    double tlon,conv,sav,sd,sdsq,blon,dif,st,defac,actan,tlat,dd,bigk,bigk2,xlamt = 0;
    double sl = 0;
    double scl = 0;
    double dlat = 0;
    double dlon = 0;
    long inumb;
    
    CoordPoint p = new CoordPoint(_p);
    /* Inverse equations. Begin inverse computation with approximation for tlon. 
    Solve for transformed long.
    ---------------------------*/
    p.x -= m_falseEasting; 
    p.y -= m_falseNorthing;

    tlon= p.x/(m_a*m_b);
    conv=1.e-9;
    for(inumb=0;inumb<50;inumb++)
    {
        sav=tlon;
        sd=Math.sin(tlon);
        sdsq=sd*sd;
        m_s=m_p21*m_sa*Math.cos(tlon)*Math.sqrt((1.0+m_t*sdsq)/((1.0+m_w*sdsq)*(1.0+m_q*sdsq)));
        blon=(p.x/m_a)+(p.y/m_a)*m_s/m_xj-m_a2*Math.sin(2.0*tlon)-m_a4*Math.sin(4.0*tlon)-(m_s/m_xj)*
            (m_c1*Math.sin(tlon)+m_c3*Math.sin(3.0*tlon)); 
        tlon=blon/m_b;
        dif=tlon-sav;
        if(Math.abs(dif)<conv)
            break; 
    }
    if(inumb>=50)  
    {
        throw(new ProjException(214, "SpaceObMerc._inverse()"));
       
    }

    /* Compute transformed lat.
    ------------------------*/
    st=Math.sin(tlon);
    defac=Math.exp(Math.sqrt(1.0+m_s*m_s/m_xj/m_xj)*(p.y/m_a-m_c1*st-m_c3*Math.sin(3.0*tlon)));
    actan=Math.atan(defac);
    tlat=2.0*(actan-(Constants.PI/4.0));

    /* Compute geodetic longitude
    --------------------------*/
    dd=st*st;
    if(Math.abs(Math.cos(tlon))<1.e-7) tlon=tlon-1.e-7;
    bigk=Math.sin(tlat); 
    bigk2=bigk*bigk;
    xlamt=Math.atan(((1.0-bigk2/(1.0-m_es))*Math.tan(tlon)*m_ca-bigk*m_sa*Math.sqrt((1.0+m_q*dd)
        *(1.0-bigk2)-bigk2*m_u)/Math.cos(tlon))/(1.0-bigk2*(1.0+m_u)));

    /* Correct inverse quadrant
    ------------------------*/
    if(xlamt>=0.0) 
        sl=1.0;
    
    if(xlamt<0.0) 
        sl= -1.0;
    
    if(Math.cos(tlon)>=0.0) 
        scl=1.0;
    
    if(Math.cos(tlon)<0.0) 
        scl= -1.0;
    
    xlamt=xlamt-((Constants.PI/2.0)*(1.0-scl)*sl);
    dlon=xlamt-m_p21*tlon;

    /* Compute geodetic latitude
    -------------------------*/
    if(Math.abs(m_sa)<1.e-7)dlat=Math.asin(bigk/Math.sqrt((1.0-m_es)*(1.0-m_es)+m_es*bigk2));
    if(Math.abs(m_sa)>=1.e-7)dlat=Math.atan((Math.tan(tlon)*Math.cos(xlamt)-m_ca*Math.sin(xlamt))/((1.0-m_es)*m_sa));
    m_lonLat.lon = Util.adjust_lon(dlon+m_center.lon);
    m_lonLat.lat = dlat;

}


protected void _forward(GeoPoint _p) throws ProjException
{
    long n=0,l=0;
    double delta_lon=0;
    double rlm=0,tabs=0,tlam=0,xlam=0,c=0,xlamt=0,ab2=0,ab1=0,xlamp=0,sav=0;
    double d=0,sdsq=0,sd=0,tanlg=0,xtan=0,tphi=0,dp=0,rlm2=0;
    double scl=0,tlamp=0,conv=0,delta_lat=0,radlt=0,radln=0;
    boolean end = false;
    
    GeoPoint p = new GeoPoint(_p);
    /* Forward equations
    -----------------*/
    conv=1.e-7;
    delta_lat = p.lat;
    delta_lon= p.lon-m_center.lon;

    /* Test for latitude and longitude approaching 90 degrees
    ----------------------------------------------------*/
    if (delta_lat>1.570796) 
        delta_lat=1.570796;
    if (delta_lat<-1.570796) 
        delta_lat= -1.570796;

    radlt=delta_lat;
    radln=delta_lon;

    if(delta_lat>=0.0)
        tlamp=Constants.PI/2.0; 
    if(m_start != 0.0)
        tlamp=2.5*Constants.PI;
    if(delta_lat<0.0) 
        tlamp=1.5*Constants.PI;
    n=0;

    while(true)
    {
        sav=tlamp;
        l=0;
        end = false;
        xlamp=radln+m_p21*tlamp;
        ab1=Math.cos(xlamp);
        
        if(Math.abs(ab1)<conv) 
            xlamp=xlamp-1.e-7;
        
        if(ab1>=0.0) 
            scl=1.0;
        
        if(ab1<0.0) 
            scl= -1.0;

        ab2=tlamp-(scl)*Math.sin(tlamp)*(Constants.PI/2);

        while(true)
        {
            xlamt=radln+m_p21*sav;
            c=Math.cos(xlamt);

            if (Math.abs(c)<1.e-7) 
                xlamt=xlamt-1.e-7;

            xlam=(((1.0-m_es)*Math.tan(radlt)*m_sa)+Math.sin(xlamt)*m_ca)/c;
            tlam=Math.atan(xlam);
            tlam=tlam+ab2;
            tabs=Math.abs(sav)-Math.abs(tlam);

            if(Math.abs(tabs)<conv) 
            {
                rlm=Constants.PI*Constants.LANDSAT_RATIO;
                rlm2=rlm+2.0*Constants.PI;
                n++;

                if(n >= 3)
                    end = true;

                if((tlam>rlm) && (tlam<rlm2)) 
                    end = true;

                if(tlam<rlm)
                    tlamp=2.50*Constants.PI;
                if(tlam>=rlm2) 
                    tlamp= Constants.PI/2;

                break;
            }

            l++;

            if (l > 50) 
            {
                throw(new ProjException(216, ""));
                
            }

            sav=tlam;
        }

        if(end)
            break;
    }

    
    
   
    dp=Math.sin(radlt);
    tphi=Math.asin(((1.0-m_es)*m_ca*dp-m_sa*Math.cos(radlt)*Math.sin(xlamt))/Math.sqrt(1.0-m_es*dp*dp));

    /* compute x and y
    ---------------*/
    xtan = (Constants.PI/4.0) + (tphi/2.0);
    tanlg = Math.log(Math.tan(xtan));
    sd=Math.sin(tlam);
    sdsq=sd*sd;
    m_s=m_p21*m_sa*Math.cos(tlam)*Math.sqrt((1.0+m_t*sdsq)/((1.0+m_w*sdsq)*(1.0+m_q*sdsq)));
    d=Math.sqrt(m_xj*m_xj+m_s*m_s);
    m_xy.x = m_b * tlam + m_a2 * Math.sin(2.0*tlam) + m_a4*Math.sin(4.0*tlam)-tanlg*m_s/d;
    m_xy.x *= m_a;
    m_xy.y = m_c1*sd+m_c3*Math.sin(3.0*tlam)+tanlg*m_xj/d;
    m_xy.y *= m_a;

    m_xy.x += m_falseEasting;
    m_xy.y += m_falseNorthing;
}



}