
#include "pseudocylindrical.h"
#include "projexception.h"

Pseudocylindrical::Pseudocylindrical() : Projection(), m_centerLat(0.0), m_centerLon(0.0)
{
}

Pseudocylindrical::Pseudocylindrical(double gctpParams[], int units, long datum, long spheroid) 
: Projection(gctpParams, units, datum, spheroid)
{
	loadFromParams();
}

void Pseudocylindrical::setCenterLat(double lat) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLat()"));

	m_centerLat = temp;
	setInit();
}

void Pseudocylindrical::setCenterLon(double lon) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lon, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLon()"));

	m_centerLon = temp;
	setInit();
}

void Pseudocylindrical::loadFromParams() {
	Projection::loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
}