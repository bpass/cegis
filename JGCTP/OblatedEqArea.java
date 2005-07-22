package jgctp;

public class OblatedEqArea extends Projection
{

protected 	double m_theta;

protected 	double m_m;

protected 	double m_n;

protected 	double m_sinLatO;

protected 	double m_cosLatO;



public 	OblatedEqArea()
{
    super();
    setName("Oblated Equal Area");
    setNumber(ProjCode.OBEQA);
    m_theta = 0;
    m_m = 0;
    m_n = 0;
    m_sinLatO = 0;
    m_cosLatO = 0;
   
}


public 	OblatedEqArea(double gctpParams[], ProjUnit units, ProjDatum dat)
{
    super(gctpParams, units, dat);
    setName("Oblated Equal Area");
    setNumber(ProjCode.OBEQA);
    setParamLoad(true);
    m_theta = 0;
    m_m = 0;
    m_n = 0;
    m_sinLatO = 0;
    m_cosLatO = 0;
   
}


public 	void setShapeM(double m) 
{
    m_m = m;
    setInit(true);
}

public 	void setShapeN(double n) 
{
    m_n = n;
    setInit(true);
}

public void setAngle(double theta) throws ProjException
{
    try
    {
        m_theta = Util.convertAngle(theta);
        setInit(true);
    }
    
    catch(ProjException e)
    {
        e.addFunc("OblatedEqArea.setAngle()");
        throw(e);
    }
}


public 	double shapeM() 
{
    return(m_m);
}

public 	double shapeN() 
{
    return(m_n);
}

public 	double angle() 
{
    return(m_theta);
}

protected 	void _loadFromParams() throws ProjException
{
   super._loadFromParams();
   setShapeM(m_gctpParams[2]);
   setShapeN(m_gctpParams[3]);
   setAngle(m_gctpParams[8]);
   
}


protected 	void _init() throws ProjException
{
    m_sinLatO = Math.sin(m_center.lat);
    m_cosLatO = Math.cos(m_center.lon); 
}


protected void _forward(GeoPoint p) throws ProjException
{
    double delta_lon;
    double sin_delta_lon;
    double cos_delta_lon;
    double sin_lat;
    double cos_lat;
    double z;
    double Az;
    double sin_Az;
    double cos_Az;
    double temp;            /* Re-used temporary variable */
    double x_prime;
    double y_prime;
    double M;
    double N;


    /* Forward equations
    -----------------*/
    delta_lon = p.lon - m_center.lon;
     sin_lat = Math.sin(p.lat);
     cos_lat = Math.cos(p.lat);

     sin_delta_lon = Math.sin(delta_lon);
     cos_delta_lon = Math.cos(delta_lon);

    z = Math.acos(m_sinLatO * sin_lat + m_cosLatO * cos_lat * cos_delta_lon);
    Az = Math.atan2(cos_lat * sin_delta_lon , m_cosLatO * sin_lat - m_sinLatO * 
        cos_lat * cos_delta_lon) + m_theta;
     sin_Az = Math.sin(Az);
     cos_Az = Math.cos(Az);

    temp = 2.0 * Math.sin(z / 2.0);
    x_prime = temp * sin_Az;
    y_prime = temp * cos_Az;
    M = Math.asin(x_prime / 2.0);
    temp = y_prime / 2.0 * Math.cos(M) / Math.cos(2.0 * M / m_m);
    N = Math.asin(temp);
    m_xy.y = m_n * m_sphere.radius * Math.sin(2.0 * N / m_n) + m_falseEasting;
    m_xy.x = m_m * m_sphere.radius * Math.sin(2.0 * M / m_m) * Math.cos(N) / Math.cos(2.0 * N / m_n) + m_falseNorthing;

}


protected void _inverse(CoordPoint p) throws ProjException
{
    double z;
    double sin_z;
    double cos_z;
    double Az;
    double temp;            /* Re-used temporary variable */
    double x_prime;
    double y_prime;
    double M;
    double N;
    double diff_angle;
    double sin_diff_angle;
    double cos_diff_angle;


    /* Inverse equations
    -----------------*/
    p.x -= m_falseEasting;
    p.y -= m_falseNorthing;
    N = (m_n / 2.0) * Math.asin(p.y / (m_n * m_sphere.radius));
    temp = p.x / (m_m * m_sphere.radius) * Math.cos(2.0 * N / m_n) / Math.cos(N);
    M = (m_m / 2.0) * Math.asin(temp);
    x_prime = 2.0 * Math.sin(M);
    y_prime = 2.0 * Math.sin(N) * Math.cos(2.0 * M / m_m) / Math.cos(M);
    temp = Math.sqrt(x_prime * x_prime + y_prime * y_prime) / 2.0;
    z = 2.0 * Math.asin(temp);
    Az = Math.atan2(x_prime, y_prime);
    diff_angle = Az - m_theta;
     sin_diff_angle = Math.sin(diff_angle);
     cos_diff_angle = Math.cos(diff_angle);

     sin_z = Math.sin(z);
     cos_z = Math.cos(z);

    m_lonLat.lat = Math.asin(m_sinLatO * cos_z + m_cosLatO * sin_z * cos_diff_angle);
    m_lonLat.lon = Util.adjust_lon(m_center.lon + Math.atan2((sin_z * sin_diff_angle), (m_cosLatO *
                                   cos_z - m_sinLatO * sin_z * cos_diff_angle)));
}



}