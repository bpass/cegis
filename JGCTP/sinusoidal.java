package JGCTP;

public class Sinusoidal extends Projection
{
	public Sinusoidal()
	{
		super();
		setNumber(ProjCode.SNSOID);
		setName("Sinusoidal");
	}
	
	public Sinusoidal(double gctpParams[], ProjUnit units, ProjDatum dat)
	{
		super(gctpParams, units, dat);
		setNumber(ProjCode.SNSOID);
		setName("Sinusoidal");
	}
	
	protected void _inverse(CoordPoint _p) throws ProjException
	{
	    double temp;		/* Re-used temporary variable */
	    CoordPoint p = new CoordPoint(_p);
	    
	    p.x -= m_falseEasting;
	    p.y -= m_falseNorthing;

	    m_lonLat.lat = p.y / m_sphere.radius;

	    if( Math.abs( m_lonLat.lat ) > Constants.HALF_PI) 
	    {
	  		throw(new ProjException(164, "Sinusoidal._inverse()"));
	    }

	    temp = Math.abs( m_lonLat.lat ) - Constants.HALF_PI;
	    if( Math.abs( temp ) > Constants.EPSLN )
	    {
	       temp = m_center.lon + p.x / (m_sphere.radius * Math.cos( m_lonLat.lat ));
	  	   m_lonLat.lon = Util.adjust_lon( temp );
	    }
	    else
	    {
	       m_lonLat.lon = m_center.lon;
	    }
	}
	
	protected void _forward(GeoPoint _p)
	{
	    double deltaLon;	/* Delta longitude (Given longitude - center */
	    
	    GeoPoint p = new GeoPoint(_p);
	    /* Forward equations */
	    deltaLon = Util.adjust_lon(p.lon - m_center.lon);

	    m_xy.x = m_sphere.radius * deltaLon * Math.cos(p.lat ) + m_falseEasting;
	    m_xy.y = m_sphere.radius * p.lat + m_falseNorthing;

	}
}