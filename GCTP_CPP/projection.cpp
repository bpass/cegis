#include "projection.h"

Projection::Projection(): m_number(-1), m_unitCode(-1), m_datum(-1), m_spheroid(-1),
m_longitude(0.0), m_latitude(0.0), m_x_coord(0.0), m_y_coord(0.0), m_falseEasting(0.0),
m_falseNorthing(0.0), m_rMajor(0.0), m_rMinor(0.0), m_radius(0.0)

{
	for(int i = 0; i < COEFCT; i++)
		m_gctpParams[i] = 0.0;
}

Projection::Projection ( double gctpParameters[], int units, int datum, int spheroid ): 
m_number(-1), m_unitCode(units), m_datum(datum), m_spheroid(spheroid),
m_longitude(0.0), m_latitude(0.0), m_x_coord(0.0), m_y_coord(0.0), m_falseEasting(0.0),
m_falseNorthing(0.0), m_rMajor(0.0), m_rMinor(0.0), m_radius(0.0)
{

  for( int index = 0; index < COEFCT; index++ )
     m_gctpParams[index] = gctpParameters[index];
  
  setFE(m_gctpParams[6]);
  setFN(m_gctpParams[7]);
  setRadii();
  return; 
}

//perform generic unit prep, 
//other projections can override this as needed.
void Projection::prepCoords(double& x, double& y) {
	double factor = 0.0;
	Util::untfz(m_unitCode, METER, &factor);
	
	x *= factor;
	y *= factor;
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





