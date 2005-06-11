#ifndef PROJECTION_H
#define PROJECTION_H

#include <math.h>
#include <string>
#include "util.h"
#include "constants.h"

//! Base projection class.
/*! This class provides an interface that all inheriting projection objects
	must adhere to.
	*/
class Projection
{
   
  public:
    Projection();

	//! Constructor 
	/*! This constructor allows the user to construct a fully initialized projection 
		object that is ready to be used.
		\param gctpParameters The 15 required projection parameters
		\param units The units that this projection will use
		\param datum The datum that this projection uses
		\param spheroid The spheroid that this projection uses
		*/
	Projection (double gctpParameters[], ProjUnit units, ProjDatum dat);
    
	Projection (const Projection& proj);
	//! Perform a forward transformation.

	/*! This function transforms input lat/lon coordinates to the desired coordinate system.
		If desired the user can use non-NULL pointers for x and y to obtain the output
		coordinates, but member variables are also set with these values that can be
		obtained using the x() and y() member functions.
		\param lon Input Longitude
		\param lat Input Latitude
		\param x Optional storage for output x coordinate
		\param y Optional storage for output y coordinate
	*/
	virtual void forward ( double lon, double lat, double* x = NULL, double* y = NULL ) = 0;
    
	//! Perform an inverse transformation.
	/*! This function transforms input x/y coordinates to the geographic coordinate system.
		If desired the user can use non-NULL pointers for lat and lon to obtain the output
		coordinates, but member variables are also set with these values that can be
		obtained using the lat() and lon() member functions.
		\param x Input x cooridnate
		\param y Input y coordinate
		\param lat Optional storage for output latitude
		\param lon Optional storage for output longitude
	*/
	virtual void inverse ( double x, double y, double* lon = NULL, double* lat = NULL ) = 0;

	//! Get the x coordinate from the most recent forward() call.
	double x () {return m_x_coord;}

	//! Get the y coordinate from the most recent forward() call.
	double y () {return m_y_coord;}

	//! Get the latitude from the most recent inverse() call.
	double lat () {return m_latitude;}

	//! Get the longitude from the most recent inverse() call.
	double lon () {return m_longitude;}

	//! Get both the x and y coordinates from the most recent forward() call.
    void xy ( double* x, double* y );

	//! Get both the lat and lon from the most recent inverse() call.
    void latLon ( double* lat, double* lon );

	double* params() {return m_gctpParams;}

	double param(size_t index);

	//! Get the name of the projection.
	std::string name() {return m_name;}

	//! Get the projection number.
	ProjCode number() { return m_number;}

	//! Get the units being used.
	ProjUnit units() {return m_unitCode;}

	//! Get the spheroid being used.
	ProjDatum datum() {return m_datum;}

	//! Set the units being used.
	void setUnits(ProjUnit units) {m_unitCode = units; setInit();}
	
	//! Set the spheroid being used.
	void setDatum(ProjDatum dat) {m_datum = dat; setInit(); setRadii();}
		
	//! Set the false easting.
	void setFE(double fe) {m_falseEasting = fe; setInit();}
	
	//! Set the false northing.
	void setFN(double fn) {m_falseNorthing = fn; setInit();}
	
	//! Set sphere radii according to spheroid and parameter array
	void setRadii() {Util::sphdz(m_datum, m_gctpParams, &m_rMajor, &m_rMinor, &m_radius);
					setInit();}
	
	//! Set the major sphere radius
	void setRMajor(double rMajor) {m_rMajor = rMajor; setInit();}
	
	//! Set the minor sphere radius
	void setRMinor(double rMinor) {m_rMinor = rMinor; setInit();}

	//! Set the radius of the sphere.
	void setRadius(double radius) {m_radius = radius; setInit();}

	void setParams(double gctpParams[]);

	void setParam(size_t index, double value);

	void setError(long errorCode) {m_errorCode = errorCode;}

	void clearError() {m_errorCode = 0;}

	long error() {return m_errorCode;}

	bool errorOccured() {return(m_errorCode != 0);}


protected:
	
	
	//! Numeric error code of latest error
	long m_errorCode;

	//! The name of the projection.
	std::string m_name;

	//! The numeric identifier of the projection.
    ProjCode m_number;
    
	//! The numeric identifier for the units of this projection.
	ProjUnit m_unitCode;

	//! The numeric identifier for the spheroid of this projection.
    ProjDatum m_datum;

	//! The longitdue value produced from an inverse transformation.
    double m_longitude;

	//! The latitude value produced from an inverse transformation.
	double m_latitude;

	//! The x coordinate produced from a forward transformation.
    double m_x_coord;

	//! The y coordinate produced from a forward transformation.
    double m_y_coord;

	//! False easting value for this projection.
	double m_falseEasting;

	//! False northing value for this projection.
	double m_falseNorthing;

	//! Major radius of sphere.
	double m_rMajor;

	//! Minor radius of sphere.
	double m_rMinor;

	//! Radius of sphere.
	double m_radius;

	//! Array of 15 projection parameters (as used in the original GCTP).
    double m_gctpParams[COEFCT];

	//! Flag to indicate if a forward init needs to be done.
	bool m_forInitNeeded;

	//! Flag to indicate if an inverse init needs to be done.
	bool m_invInitNeeded;

	//! Perform all neccessary initializations to prepare for a forward transformation.
	virtual void forward_init () = 0;

	//! Perform all neccessary initializations to prepare for an inverse transformation.	
	virtual void inverse_init () = 0;

	//! Set the number of the projection.
	void setNumber(ProjCode number) {m_number = number;}

	//! Set the name of the projection.
	void setName(std::string name) {m_name = name;}

	//! Toggle forward and inverse initialization flags.
	void setInit() {m_forInitNeeded = true; m_invInitNeeded = true;}

	
	virtual void loadFromParams();


};

#endif
