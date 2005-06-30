
class CoordPoint
{
	public double x;
	public double y;
	public CoordPoint(double _x, double _y)
	{
		x = _x;
		y = _y;
	}
	
	public CoordPoint(CoordPoint p)
	{
		x = p.x;
		y = p.y;
	}
	
}

class GeoPoint
{
	public double lon;
	public double lat;
	public GeoPoint(double _lon, double _lat)
	{
		lon = _lon;
		lat = _lat;
	}
	
	public GeoPoint(GeoPoint p)
	{
		lon = p.lon;
		lat = p.lat;
	}
}

class Sphere
{
	private double m_rMajor;
	private double m_rMinor;
	private double m_radius;
	
	
	public Sphere()
	{
		m_rMajor = 0.0;
		m_rMinor = 0.0;
		m_radius = 0.0;
	}
	
	public Sphere(double rMajor, double rMinor, double radius)
	{
		m_rMajor = rMajor;
		m_rMinor = rMinor;
		m_radius = radius;
	}
	
	public Sphere(Sphere s)
	{
		m_rMajor = s.m_rMajor;
		m_rMinor = s.m_rMinor;
		m_radius = s.m_radius;
	}
	
	public double rMajor()
	{
		return m_rMajor;
	}
	
	public double rMinor()
	{
		return m_rMinor;
	}
	
	public double radius()
	{
		return m_radius;
	}
	
	public void setRMinor(double rMinor)
	{
		m_rMinor = rMinor;
	}
	
	public void setRMajor(double rMajor)
	{
		m_rMajor = rMajor;
	}
	
	public void setRadius(double radius)
	{
		m_radius = radius;
	}
}
	
abstract class Projection 
{
	//!Center longitude / longitude of central meridian of the projection.
	protected double m_centerLon;
	
	//!Center latitude / latitude of central merridian of the projection.
	protected double m_centerLat;
	
	//!Latitude of first standard parallel.
	protected double m_stdParallelLat1;
	
	//!latitude of second standard parallel.
	protected double m_stdParallelLat2;
	
	//!False easting value.
	protected double m_falseEasting;
	
	//!False northing value.
	protected double m_falseNorthing;
	
	//! The x coordinate produced from a forward transformation.
	protected double m_x_coord;

	//! The y coordinate produced from a forward transformation.
	protected double m_y_coord;
	
	//! The longitude value produced from an inverse transformation.
	protected double m_longitude;
	
	//! The latitude value produced from an inverse transformation.	
	protected double m_latitude;

	//! Major radius of sphere.
	protected double m_rMajor;
	
	//! Minor radius of sphere.	
	protected double m_rMinor;
	
	//! Radius of sphere.	
	protected double m_radius;
	
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
		m_centerLon = 0.0;
		m_centerLat = 0.0;
		m_stdParallelLat1 = 0.0;
		m_stdParallelLat2 = 0.0;
		m_x_coord = 0.0;
		m_y_coord = 0.0;
		m_longitude = 0.0;
		m_latitude = 0.0;
		m_initNeeded = true;
		m_paramLoadNeeded = true;
		
		m_gctpParams = new double[15];
		
		for(int i = 0; i < 15; i++)
			m_gctpParams[i] = 0.0;
	}
	
	
	public Projection(double gctpParams[], ProjUnit units, ProjDatum datum)
	{
		m_centerLon = 0.0;
		m_centerLat = 0.0;
		m_stdParallelLat1 = 0.0;
		m_stdParallelLat2 = 0.0;
		m_x_coord = 0.0;
		m_y_coord = 0.0;
		m_longitude = 0.0;
		m_latitude = 0.0;
		m_initNeeded = true;
		m_paramLoadNeeded = true;
		m_unitCode = units;
		m_datum = datum;
		
		m_gctpParams = new double[15];
		
		for(int i = 0; i < 15; i++)
			m_gctpParams[i] = gctpParams[i];
		
	}
	
	
	public final void inverse(CoordPoint xy, GeoPoint lonLat) throws ProjException
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
			throw(e);
		}
	}
	
	
	public final void forward(GeoPoint lonLat, CoordPoint xy) throws ProjException
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
			m_centerLon = Util.convertAngle(lon);
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
			m_centerLat = Util.convertAngle(lat);
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
		Sphere s = new Sphere();
		Util.sphdz(m_datum.val(), m_gctpParams, s);
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
		return m_x_coord;
	}
	
	//! Get the y coordinate from the most recent forward() call.

	public double y()
	{
		return m_y_coord;
	}
	
	//! Get the longitude from the most recent inverse() call.

	public double lon() 
	{
		return m_longitude;
	}
	
	//! Get the latitude from the most recent inverse() call.

	public double lat()
	{
		return m_latitude;
	}
	
	//! Get the center longitude / longitude of central meridian for projection.
	public double centerLon() 
	{
		return m_centerLon;
	}
	
	//! Get the center latitude / latitude of central meridian for projection.
	public double centerLat()
	{
		return m_centerLat;
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
	
	public final void init() throws ProjException
	{
	    try 
	    {
	        _init();
	        m_initNeeded = false;
	    }
	    
	    catch(ProjException e)
	    {
	        throw(e);
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
		m_name = name;
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
	
	protected abstract void _init() throws ProjException;
}