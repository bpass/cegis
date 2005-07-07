package JGCTP;
	
public abstract class Projection 
{
    //!Center longitude and latitude / longitude and latitude of central meridian of the projection.
	protected GeoPoint m_center;
    
	//!Latitude of first standard parallel.
	protected double m_stdParallelLat1;
	
	//!latitude of second standard parallel.
	protected double m_stdParallelLat2;
	
	//!False easting value.
	protected double m_falseEasting;
	
	//!False northing value.
	protected double m_falseNorthing;
	
    //! Output x and y coordinates from a forward transformation.
	protected CoordPoint m_xy;
    
    //! Output longitude and latitude coordinates from an inverse transformation.
    protected GeoPoint m_lonLat;

    //! Projection spheroid.
	protected Sphere m_sphere;
	
	//! The numeric identifier for the units of this projection.	
	protected ProjUnit m_unitCode;
	
    //! The numeric identifier for the spheroid of this projection.
	protected ProjDatum m_datum;
	
	//! The longitude value produced from an inverse transformation.
	protected ProjCode m_number;
	
	//! The name of the projection.
	protected String m_name;

	//!Do we need to do an initialization?
	protected boolean m_initNeeded;
	
	//!Do we need to reload the members from the parameter array?
	protected boolean m_paramLoadNeeded;
	
	//! The GCTP parameter array. 
	protected double m_gctpParams[];
		
	public Projection() 
	{	
        m_center = new GeoPoint(0.0,0.0);
		
		m_stdParallelLat1 = 0.0;
		m_stdParallelLat2 = 0.0;
        
        m_xy = new CoordPoint(0.0, 0.0);
		m_lonLat = new GeoPoint(0.0, 0.0);
		m_sphere = new Sphere();
        m_name = new String();
		m_initNeeded = true;
		m_paramLoadNeeded = true;
		
		m_gctpParams = new double[15];
		
		for(int i = 0; i < 15; i++)
			m_gctpParams[i] = 0.0;
	}
	
	
	public Projection(double gctpParams[], ProjUnit units, ProjDatum datum)
	{
        m_center = new GeoPoint(0.0,0.0);
        
        m_stdParallelLat1 = 0.0;
        m_stdParallelLat2 = 0.0;
        m_name = new String();
        m_xy = new CoordPoint(0.0, 0.0);
        m_lonLat = new GeoPoint(0.0, 0.0);
        m_sphere = new Sphere();
        m_initNeeded = true;
        m_paramLoadNeeded = true;
		m_unitCode = units;
		m_datum = datum;
		
		m_gctpParams = new double[15];
		
		for(int i = 0; i < 15; i++)
			m_gctpParams[i] = gctpParams[i];
		
	}
	
	
	public final void inverse(CoordPoint xy, GeoPoint lonLat) throws TransformationException, InitException
	{
		CoordPoint temp = new CoordPoint(xy);
		
		try {
			if(paramLoadNeeded())
				loadFromParams();
	
			if(initNeeded())
				init();
	
			Util.convertCoords(m_unitCode, ProjUnit.METER, temp);
	
			_inverse(temp);
	
			Util.convertCoords(ProjUnit.RADIAN, ProjUnit.DEGREE, lonLat);
		}
		
		catch(ProjException e)
		{
			throw(new TransformationException(e));
		}
		
		catch(InitException e)
		{
		    throw(e);
		}
	}
	
	
	public final void forward(GeoPoint lonLat, CoordPoint xy) throws TransformationException, InitException
	{
		GeoPoint temp = new GeoPoint(lonLat);
		try {
			if(paramLoadNeeded())
				loadFromParams();
	
			if(initNeeded())
				init();
	
			Util.convertCoords(ProjUnit.DEGREE, ProjUnit.RADIAN, temp);
	
			_forward(temp);
	
			Util.convertCoords(ProjUnit.METER, m_unitCode, xy);
		}
		
		catch(ProjException e)
		{
			throw(new TransformationException(e));
		}
		
		catch(InitException e)
		{
		    throw(e);
		}
	}
	
	
	public void setFE(double val) 
	{
		m_falseEasting = val; 
		setInit(true);
	}
	
	
	public void setFN(double val) 
	{
		m_falseNorthing = val; 
		setInit(true);
	}
	
	
	public void setCenterLon(double lon) throws ProjException
	{
		try 
		{
			m_center.lon = Util.convertAngle(lon);
			setInit(true);
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	
	public void setCenterLat(double lat) throws ProjException
	{
		try 
		{
			m_center.lat = Util.convertAngle(lat);
			setInit(true);
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	
	public void setStdParallelLat1(double lat) throws ProjException
	{
		try 
		{
			m_stdParallelLat1 = Util.convertAngle(lat);
			setInit(true);
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	
	public void setStdParallelLat2(double lat) throws ProjException
	{
		try 
		{
			m_stdParallelLat2 = Util.convertAngle(lat);
			setInit(true);
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	//! Set sphere radii according to spheroid and parameter array

	public void setRadii()
	{
		Util.sphdz(m_datum.val(), m_gctpParams, m_sphere);
		setInit(true);	
		
	}
	
	public void setUnits(ProjUnit units)
	{
		m_unitCode = units;
	}
	
	public void setDatum(ProjDatum datum)
	{
		m_datum = datum;
	}

	public void setParams(double params[])
	{
		for(int i = 0; i < 15; i++)
			m_gctpParams[i] = params[i];
	}
	
	//! Get the x coordinate from the most recent forward() call.

	public double x() 
	{
		return m_xy.x;
	}
	
	//! Get the y coordinate from the most recent forward() call.

	public double y()
	{
		return m_xy.y;
	}
	
	//! Get the longitude from the most recent inverse() call.

	public double lon() 
	{
		return m_lonLat.lon;
	}
	
	//! Get the latitude from the most recent inverse() call.

	public double lat()
	{
		return m_lonLat.lat;
	}
	
	//! Get the center longitude / longitude of central meridian for projection.
	public double centerLon() 
	{
		return m_center.lon;
	}
	
	//! Get the center latitude / latitude of central meridian for projection.
	public double centerLat()
	{
		return m_center.lat;
	}
	
	//! Get the latitude of the first standard parallel for projection.
	public double stdParallelLat1()
	{
		return m_stdParallelLat1;
	}
	
	//! Get the latitude of the second standard parallel for projection.
	public double stdParallelLat2()
	{
		return m_stdParallelLat2;
	}
	
	//! Get the GCTP parameter array.
	public double[] params()
	{
		return m_gctpParams;
	}
	
	
	//!Get the name of the projection
	public String name() 
	{
		return m_name;
	}
	
	//!Get the numeric identifier for the projection.
	public ProjCode number()
	{
		return m_number;
	}
	
	//! Get the numeric identifier for the units of the projection.
	public ProjUnit units()
	{
		return m_unitCode;
	}
	
	//!Get the numeric identifier for the projection datum.
	public ProjDatum datum()
	{
		return m_datum;
	}
	
	//!Perform all intializations needed for forward and inverse transformations.
	/*! This function calls the virtual function _init() which is overriden
		by each derived class and performs other setup as needed by every
		projection.
	*/
	
	public final void init() throws InitException
	{
	    try 
	    {
	        _init();
	        m_initNeeded = false;
	    }
	    
	    catch(ProjException e)
	    {
	        throw(new InitException(e));
	    }
	}
	
	//! Do we need to reinitialize?
	
	protected boolean initNeeded() 
	{
		return m_initNeeded;
	}
	
	//!Do we need to reload the members from the parameter array?
	
	protected boolean paramLoadNeeded()
	{
		return m_paramLoadNeeded;
	}
	
	//!Do we need to reload the members from the parameter array?
	
	protected void setName(String name)
	{
		m_name = new String(name);
	}
	
	//!Set the numeric identifier for the projection.
	protected void setNumber(ProjCode number)
	{
		m_number = number;
	}
	
	//!Call _loadFromParams() to load appropriate member variables with their corresponding values in the parameter array.
	protected final void loadFromParams() throws ProjException
	{
		try {
			_loadFromParams();
			setParamLoad(false);
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	//!Load member variables with their corresponding values in the parameter array.
	/*! If any derived class needs to load parameters from the gctp parameter array
		aside from false easting, false northing, and sphere radii this function 
		must be overloaded.
	*/
	protected void _loadFromParams() throws ProjException
	{
		setFE(m_gctpParams[6]);
		setFN(m_gctpParams[7]);
		
		try {
			setCenterLon(m_gctpParams[4]);
			setCenterLat(m_gctpParams[5]);
			setStdParallelLat1(m_gctpParams[2]);
			setStdParallelLat2(m_gctpParams[3]);
			setRadii();
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
	}
	
	//! Toggle forward and inverse initialization flags.

	protected void setInit(boolean set) {m_initNeeded = set;}
	
	//!Toggle reload from parameter array.

	protected void setParamLoad(boolean set) {m_paramLoadNeeded = set; setInit(true);}
	
	//! Performs the forward transformation.
	/*! This function is responsible for performing
		the projection's forward transformation.
		\param lon Input longitude in decimal degrees
		\param lat Input latitude in decimal degrees
	*/

	protected abstract void _forward(GeoPoint _p) throws ProjException;
	
	//! Performs the inverse transformation.
	/*! This function is responsible for performing
		the projection's inverse transformation.
		Note that the units of the input coordinates
		must be set either in the constructor or by
		using the setUnits() function. For a list of 
		unit codes please 
		\param x Input x coordinate
		\param y Input y coordinate
	*/

	protected abstract void _inverse(CoordPoint _p) throws ProjException;
	
	//!This function performs the actual initialization for each projection.
	
	protected void _init() throws ProjException
    {
	    return;
    }
    
}