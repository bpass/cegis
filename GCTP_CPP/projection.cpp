#include "projection.h"
#include "projexception.h"

Projection::Projection(): 
m_longitude(0.0), m_latitude(0.0), m_x_coord(0.0), m_y_coord(0.0), m_falseEasting(0.0),
m_falseNorthing(0.0), m_rMajor(0.0), m_rMinor(0.0), m_radius(0.0),
m_forInitNeeded(true), m_invInitNeeded(true)


{
	for(int i = 0; i < COEFCT; i++)
		m_gctpParams[i] = 0.0;
}

Projection::Projection ( double gctpParameters[], ProjUnit units, Datum dat): 
m_unitCode(units), m_datum(dat),
m_longitude(0.0), m_latitude(0.0), m_x_coord(0.0), m_y_coord(0.0), m_falseEasting(0.0),
m_falseNorthing(0.0), m_rMajor(0.0), m_rMinor(0.0), m_radius(0.0),
m_forInitNeeded(true), m_invInitNeeded(true)
{

  for( int index = 0; index < COEFCT; index++ )
     m_gctpParams[index] = gctpParameters[index];
  
  return; 
}

void Projection::loadFromParams() {
	setFE(m_gctpParams[6]);
	setFN(m_gctpParams[7]);
	setRadii();
}

void Projection::xy ( double* x, double* y )
{
  if( x != NULL )
  {
     *x = m_x_coord;
  }
  
  if( y != NULL )
  {
     *y = m_y_coord;
  }
  
  
}

void Projection::latLon ( double* lat, double* lon )
{
  if( lat != NULL )
  {
     *lat = m_latitude;
  }
  
  if( lon != NULL )
  {
     *lon = m_longitude;
  }
  
 
}

void Projection::setParams(double gctpParams[]) {
	for(int i = 0; i < COEFCT; i++) {
		m_gctpParams[i] = gctpParams[i];
	}
	loadFromParams();
   setRadii();
}

void Projection::setParam(size_t index, double value) {
	if(index < COEFCT) {
		m_gctpParams[index] = value;
		loadFromParams();
      setRadii();
	}

	else
		throw(ProjException(0, "Projection::setParam()"));
}

double Projection::param(size_t index) {
	if(index < COEFCT)
		return m_gctpParams[index];
	else
		throw(ProjException(0, "Projection::setParam()"));

}





