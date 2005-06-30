
class Util 
{
	
	public static double asinz(double con)
	{
		 if (Math.abs(con) > 1.0)
		   {
		   if (con > 1.0)
		     con = 1.0;
		   else
		     con = -1.0;
		   }
		 return(Math.asin(con));
	}
	
	public static double msfnz(double eccent, double sinphi, double cosphi)
	{
		double con;

	    con = eccent * sinphi;
	    return((cosphi / (Math.sqrt (1.0 - con * con))));

	}
	
	public static double qsfnz(double eccent, double sinphi) 
	{
		double con;

		if (eccent > 1.0e-7)
		{
			con = eccent * sinphi;
			return (( 1.0- eccent * eccent) * (sinphi /(1.0 - con * con) - (.5/eccent)*
				Math.log((1.0 - con)/(1.0 + con))));
		}
		else
			return(2.0 * sinphi);
	}
	
	public static double phi1z(double eccent, double qs) throws ProjException
	{
		double eccnts;
		double dphi;
		double con;
		double com;
		double sinphi;
		double cosphi;
		double phi;
		long i;

		phi = asinz(.5 * qs);

		if (eccent < Constants.EPSLN) 
			return(phi);
		
		eccnts = eccent * eccent;

		for (i = 1; i <= 25; i++) 
		{
			sinphi = Math.sin(phi);
			cosphi = Math.cos(phi);
			con = eccent * sinphi; 
			com = 1.0 - con * con;
			dphi = .5 * com * com / cosphi * (qs / (1.0 - eccnts) - sinphi / com + 
				   .5 / eccent * Math.log ((1.0 - con) / (1.0 + con)));
			phi = phi + dphi;
			
			if (Math.abs(dphi) <= 1e-7)
				return(phi);
		}
		
		throw(new ProjException(001, "Util.phi1z"));
	}
	
	public static double phi2z(double eccent, double ts) throws ProjException
	{
		double eccnth;
		double phi;
		double con;
		double dphi;
		double sinpi;
		long i;

		eccnth = .5 * eccent;
		phi = Constants.HALF_PI - 2 * Math.atan(ts);
		for (i = 0; i <= 15; i++)
		{
			sinpi = Math.sin(phi);
			con = eccent * sinpi;
			dphi = Constants.HALF_PI - 2 * Math.atan(ts *(Math.pow(((1.0 - con)/(1.0 + con)),eccnth))) - 
				phi;
			phi += dphi; 
			if (Math.abs(dphi) <= .0000000001)
				return(phi);
		}
		
		throw(new ProjException(002, "Util.phi2z"));

	}
	
	public static double phi3z(double ml, double e0, double e1, double e2, double e3) throws ProjException
	{
		double phi;
		double dphi;
		long i;

		phi = ml;
		for (i = 0; i < 15; i++)
		{
			dphi = (ml + e1 * Math.sin(2.0 * phi) - e2 * Math.sin(4.0 * phi) + e3 * Math.sin(6.0 * phi))
				/ e0 - phi;
			phi += dphi;
			if (Math.abs(dphi) <= .0000000001)
				return(phi);
			
		}
		
		throw(new ProjException(3, "Util.phi3z"));

	}
	
	public static void phi4z(double eccent, double e0, double e1, double e2, 
			                 double e3, double a, double b, DoubleRef c, DoubleRef phi) throws ProjException
	{
		double sinphi;
		double sin2ph;
		double tanphi;
		double ml;
		double mlp;
		double con1;
		double con2;
		double con3;
		double dphi;
		long i;

		phi.setValue(a);
		for (i = 1; i <= 15; i++)
		{
			sinphi = Math.sin(phi.value());
			tanphi = Math.tan(phi.value()); 
			c.setValue(tanphi * Math.sqrt (1.0 - eccent * sinphi * sinphi));
			sin2ph = Math.sin (2.0 * phi.value());
			ml = e0 * phi.value() - e1 * sin2ph + e2 * Math.sin (4.0 *  phi.value()) - e3 * 
				Math.sin (6.0 *  phi.value());
			mlp = e0 - 2.0 * e1 * Math.cos (2.0 *  phi.value()) + 4.0 * e2 *
				Math.cos (4.0 *  phi.value()) - 6.0 * e3 * Math.cos (6.0 *  phi.value());
			con1 = 2.0 * ml + c.value() * (ml * ml + b) - 2.0 * a *  (c.value() * ml + 1.0);
			con2 = eccent * sin2ph * (ml * ml + b - 2.0 * a * ml) / (2.0 * c.value());
			con3 = 2.0 * (a - ml) * (c.value() * mlp - 2.0 / sin2ph) - 2.0 * mlp;
			dphi = con1 / (con2 + con3);
			phi.add(dphi);
			if (Math.abs(dphi) <= .0000000001 )
				return;
		}
		throw(new ProjException(004, "Util.phi4z"));
	}
	
	public static double pakcz(double pak)
	{
		double con;
		double secs;
		long degs,mins;

		con = Math.abs (pak);
		degs = (long) ((con / 10000.0) + .001);
		con =  con  - degs * 10000;
		mins = (long) ((con / 100.0) + .001);
		secs = con  - mins * 100;
		con = (double) (degs) * 1000000.0 + (double) (mins) * 1000.0 + secs;

		if (pak < 0) 
			con *= -1;

		return(con); 
	}
	
	public static double paksz(double ang) throws ProjException
	{
		double fac;		/* sign flag			*/
		double deg;		/* degree variable		*/
		double min;		/* minute variable		*/
		double sec;		/* seconds variable		*/
		double tmp;		/* temporary variable		*/
		long i;			/* temporary variable		*/

		if (ang < 0.0)
			fac = -1;
		else
			fac = 1;

		/* find degrees
		-------------*/
		sec = Math.abs(ang);
		tmp = 1000000.0;
		i = (long) (sec/tmp);
		
		if (i > 360)
			throw(new ProjException(1116, "Util.paksz"));
		
		else
			deg = i;

		/* find minutes
		-------------*/
		sec = sec - deg * tmp;
		tmp = 1000;
		i = (long) (sec/tmp);
		
		if (i > 60)
			throw(new ProjException(1116, "Util.paksz"));
		
		else
			min = i;

		/* find seconds
		-------------*/
		sec = sec - min * tmp;
		if (sec > 60)
			throw(new ProjException(1116, "Util.paksz"));
		
		else
			sec = fac * (deg * 3600.0 + min * 60.0 + sec);
		
		deg = sec / 3600.0;

		return(deg);
	}
	
	public static double pakr2dm(double pak)
	{
		  double con;
		  double secs;
		  long degs,mins;
		
		  pak *= Constants.R2D;

		  con = Math.abs (pak);
		  degs = (long) (con);
		  con =  (con  - degs) * 60;
		  mins = (long) con;
		  secs = (con  - mins) * 60;
		  con = (double) (degs) * 1000000.0 + (double) (mins) * 1000.0 + secs;

		  if (pak < 0) 
			  con *= -1;

		  return(con); 
	}
	
	public static double tsfnz(double eccent, double phi, double sinphi)
	{
		  double con;
		  double com;
		  
		  con = eccent * sinphi;
		  com = .5 * eccent; 
		  con = Math.pow(((1.0 - con) / (1.0 + con)),com);
		  return (Math.tan(.5 * (Constants.HALF_PI - phi))/con);
	}
	
	public static int sign(double x)
	{
		if (x < 0.0)
		    return(-1);
		else
		    return(1);
	}
	
	public static double adjust_lon(double x)
	{
		long count = 0;
		for(;;)
		{
			
			if (Math.abs(x)<=Math.PI)
				break;
			
			else if (((long) Math.abs(x / Math.PI)) < 2)
				x = x-(sign(x) *(2*Math.PI));
			
			else if (((long) Math.abs(x / (2*Math.PI))) < Constants.MAXLONG)
				x = x-(((long)(x / (2*Math.PI)))*(2*Math.PI));
			
			
			else if (((long) Math.abs(x / (Constants.MAXLONG * (2*Math.PI)))) < Constants.MAXLONG)
				x = x-(((long)(x / (Constants.MAXLONG * (2*Math.PI)))) * ((2*Math.PI) * Constants.MAXLONG));
			
			
			else if (((long) Math.abs(x / (Constants.DBLLONG * (2*Math.PI)))) < Constants.MAXLONG)
				x = x-(((long)(x / (Constants.DBLLONG * (2*Math.PI)))) * ((2*Math.PI) * Constants.DBLLONG));
			
			else
				x = x-(sign(x) *(2*Math.PI));
			
			count++;
			
			if (count > Constants.MAX_VAL)
				break;
		}

		return(x);
	}

	public static double e0fn(double x) 
	{
		return(1.0-0.25*x*(1.0+x/16.0*(3.0+1.25*x)));
	}
	
	public static double e1fn(double x) 
	{
		return(0.375*x*(1.0+0.25*x*(1.0+0.46875*x)));
	}
	
	public static double e2fn(double x) 
	{
		return(0.05859375*x*x*(1.0+0.75*x));
	}
	
	public static double e3fn(double x) 
	{
		return(x*x*x*(35.0/3072.0));
	}
	
	public static double e4fn(double x) 
	{ 
		double con;
  	    double com;
		con = 1.0 + x;
		com = 1.0 - x;
		return (Math.sqrt((Math.pow(con,con))*(Math.pow(com,com))));
	}
	
	public static double mlfn(double e0, double e1, double e2, double e3, double phi) 
	{
		return(e0*phi-e1*Math.sin(2.0*phi)+e2*Math.sin(4.0*phi)-e3*Math.sin(6.0*phi));
	}

	public static long calc_utm_zone(double lon) 
	{
		return((long)(((lon + 180.0) / 6.0) + 1.0));
	}
	
	public static double untfz(int inunit, int outunit) throws ProjException
	{
		double factor = 0.0;
		
		if ((outunit >= 0) && (outunit <= Constants.MAXUNIT) && (inunit >= 0)
				&& (inunit <= Constants.MAXUNIT))
			{
				factor = Tables.factors[inunit][outunit];

				/* Angle units can not be converted to length units
				------------------------------------------------*/
				if (factor == 0.0)
					throw(new ProjException(1101, "Util.untfz"));
				
			}
			else
				throw(new ProjException(5, "Util.untfz"));
			
			return(factor);
	}

	public static void sphdz(int isph, double parm[], Sphere outSphere)
	{
		double t_major;		/* temporary major axis				*/
		double t_minor;		/* temporary minor axis				*/
		int jsph;		/* spheroid code number				*/

		/* if the spheroid code is a negative number, get the semi-major and semi-minor
		axis from the projection array
		--------------------------------------------------------------------------*/
		if (isph < 0)
		{
			t_major = Math.abs(parm[0]);
			t_minor = Math.abs(parm[1]);

			if (t_major  > 0.0) 
			{
				/* The semimajor axis and the semiminor axis are in the array, assign
				them directly
				--------------------------------------------------------------------*/
				if (t_minor > 1.0)
				{
					outSphere.setRMajor(t_major);
					outSphere.setRMinor(t_minor);
					outSphere.setRadius(t_major);
				} 
				/* The semimajor axis and the eccentricity squared values are in the array,
				therefore, the semiminor axis is computed from the eccentricity
				squared value parm[1]
				----------------------------------------------------------------------*/
				else
					if (t_minor > 0.0)
					{
						outSphere.setRMajor(t_major);
						outSphere.setRadius(t_major);
						outSphere.setRMinor((Math.sqrt(1.0 - t_minor)) * t_major); 
					}
					/* The semiminor axis is zero or less, assign the semimajor axis to
					the semiminor axis.
					-----------------------------------------------------------------*/
					else
					{
						outSphere.setRMajor(t_major);
						outSphere.setRMinor(t_major);
						outSphere.setRadius(t_major);
					}
			}
			/* The sphroid code is to be used to assign the axis
			-------------------------------------------------*/
			else
				if (t_minor > 0.0)	/* t_major = 0 */

					/* Assign Clarke 1866 semi-major and semi-minor axis
					---------------------------------------------------*/
				{
					outSphere.setRMajor(Tables.major[0]);
					outSphere.setRadius(Tables.major[0]);
					outSphere.setRMinor(Tables.minor[0]);
				}
				else
					/* Assign Spheroid radius to semi-major and semi-minor axis
					---------------------------------------------------------*/
				{
					outSphere.setRMajor(Tables.major[Constants.RADVAL]);
					outSphere.setRadius(Tables.major[Constants.RADVAL]);
					outSphere.setRMinor(Tables.major[Constants.RADVAL]);
				}
		}
		/* Use the spheroid code to assign the semi-major and semi-minor axis
		-----------------------------------------------------------------*/
		else		/* isph >= 0 */
		{
			jsph = isph;

			/* The spheroid code is out of range, assign Clarke 1866
			------------------------------------------------------*/
			if (jsph > (Constants.SPHDCT - 1))	
				jsph = 0;
			
			/* Assign the radius argument to the standard radius value
			-------------------------------------------------------*/
			outSphere.setRMajor(Tables.major[jsph]);
			outSphere.setRMinor(Tables.minor[jsph]);
			outSphere.setRadius(Tables.major[Constants.RADVAL]);
		}
		return;
	}

	public static void convertCoords(ProjUnit fromUnit, ProjUnit toUnit, CoordPoint p) throws ProjException
	{
		double factor = 0.0;
		try 
		{
			factor = untfz(fromUnit.val(), toUnit.val());
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
		
		p.x *= factor;
		p.y *= factor;
	}
	
	public static void convertCoords(ProjUnit fromUnit, ProjUnit toUnit, GeoPoint p) throws ProjException
	{
		double factor = 0.0;
		try 
		{
			factor = untfz(fromUnit.val(), toUnit.val());
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
		
		p.lon *= factor;
		p.lat *= factor;
	}

	//convert a packed DMS angle to radians
	public static double DMSToRad(double angle) throws ProjException 
	{
		try 
		{
			return (paksz(angle) * 3600 * Constants.S2R);
		}
		catch(ProjException e)
		{
			throw(e);
		}
		
	}
	
	public static double convertAngle(double ang) throws ProjException
	{
		try {
			return(Util.DMSToRad(ang));
		}
		
		catch(ProjException e)
		{
			throw(e);
		}
		
	}
	
}