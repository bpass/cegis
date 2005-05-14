#ifndef PROJECTION_H
#define PROJECTION_H

#include <math.h>
#include <string>
#include "util.h"
#include "constants.h"

class Projection
{
  // Associations
  // Attributes
  protected:
	std::string m_name;
    int m_number;
    int m_unitCode;
    long m_datum;
    long m_spheroid;
    double m_longitude;
    double m_latitude;
    double m_x_coord;
    double m_y_coord;
	double m_falseEasting;
	double m_falseNorthing;
	double m_rMajor;
	double m_rMinor;
	double m_radius;
    double m_gctpParams[COEFCT];
  // Operations
  public:
    Projection();
	Projection (double gctpParameters[], int units = 0, int datum = 0, int spheroid = 0);
    virtual void forward ( double lon, double lat, double* x = NULL, double* y = NULL ) = 0;
    virtual void inverse ( double x, double y, double* lon = NULL, double* lat = NULL ) = 0;

	//get functions
	double x () {return m_x_coord;}
	double y () {return m_y_coord;}
	double lat () {return m_latitude;}
	double lon () {return m_longitude;}
    void xy ( double* x, double* y );
    void latLon ( double* lat, double* lon );
	std::string name() {return m_name;}
	int number() { return m_number;}
	int units() {return m_unitCode;}
	long datum() {return m_datum;}
	long spheroid() {return m_spheroid;}

/*	//set functions
	void setLat(double lat) {m_latitude = lat;}
	void setLon(double lon) {m_longitude = lon;}
	void setX(double x) {m_x_coord = x;}
	void setY(double y) {m_y_coord = y;}
*/	
	
	void setUnits(int units) {m_unitCode = units;
							  m_forInitNeeded = true; 
							  m_invInitNeeded = true;}
	
	void setDatum(long datum) {m_datum = datum;
							   m_forInitNeeded = true; 
							   m_invInitNeeded = true;}
	
	void setSpheroid(long spheroid) {m_spheroid = spheroid;
									 m_forInitNeeded = true; 
									 m_invInitNeeded = true;}
	
	void setName(std::string name) {m_name = name;}
	
	void setName(const char* name) {m_name = name;}
	
	void setFE(double fe) {m_falseEasting = fe;
						   m_forInitNeeded = true; 
					       m_invInitNeeded = true;}
	
	void setFN(double fn) {m_falseNorthing = fn;
						   m_forInitNeeded = true; 
						   m_invInitNeeded = true;}
	
	//set sphere radii according to spheroid and parameter array
	void setRadii() {Util::sphdz(m_spheroid, m_gctpParams, &m_rMajor, &m_rMinor, &m_radius);
					 m_forInitNeeded = true; 
					 m_invInitNeeded = true;}
	
	void setRMajor(double rMajor) {m_rMajor = rMajor;
								   m_forInitNeeded = true; 
								   m_invInitNeeded = true;}
	
	void setRMinor(double rMinor) {m_rMinor = rMinor;
								   m_forInitNeeded = true; 
								   m_invInitNeeded = true;}

	void setRadius(double radius) {m_radius = radius;
								   m_forInitNeeded = true; 
								   m_invInitNeeded = true;}

protected:
	
	bool m_forInitNeeded;
	bool m_invInitNeeded;
	virtual void forward_init () = 0;
    virtual void inverse_init () = 0;
	void setNumber(int number) {m_number = number;}
    
};

#endif
