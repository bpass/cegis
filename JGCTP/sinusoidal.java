
class Sinusoidal extends Projection
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
	
	protected void _init() throws ProjException
	{
	   return;
	}
	
	protected void _inverse(CoordPoint _p) throws ProjException
	{
	    double temp;		/* Re-used temporary variable */
	    CoordPoint p = new CoordPoint(_p);
	    
	    p.x -= m_falseEasting;
	    p.y -= m_falseNorthing;

	    m_latitude = p.y / m_radius;

	    if( Math.abs( m_latitude ) > Constants.HALF_PI) 
	    {
	  		throw(new ProjException(164, "Sinusoidal._inverse()"));
	    }

	    temp = Math.abs( m_latitude ) - Constants.HALF_PI;
	    if( Math.abs( temp ) > Constants.EPSLN )
	    {
	       temp = m_centerLon + p.x / (m_radius * Math.cos( m_latitude ));
	  	   m_longitude = Util.adjust_lon( temp );
	    }
	    else
	    {
	       m_longitude = m_centerLon;
	    }
	}
	
	protected void _forward(GeoPoint _p)
	{
	    double deltaLon;	/* Delta longitude (Given longitude - center */
	    
	    GeoPoint p = new GeoPoint(_p);
	    /* Forward equations */
	    deltaLon = Util.adjust_lon(p.lon - m_centerLon);

	    m_x_coord = m_radius * deltaLon * Math.cos(p.lat ) + m_falseEasting;
	    m_y_coord = m_radius * p.lat + m_falseNorthing;

	}
}