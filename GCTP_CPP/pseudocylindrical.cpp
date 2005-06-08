
#include "pseudocylindrical.h"
#include "projexception.h"

Pseudocylindrical::Pseudocylindrical() : Projection(), m_centerLat(0.0), m_centerLon(0.0)
{
}

Pseudocylindrical::Pseudocylindrical(double gctpParams[], ProjUnit units, ProjDatum dat) 
: Projection(gctpParams, units, dat)
{
	loadFromParams();
}

void Pseudocylindrical::setCenterLat(double lat) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0) {
		setError(err);
		return;
	}

	m_centerLat = temp;
	setInit();
}

void Pseudocylindrical::setCenterLon(double lon) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lon, &err) * 3600 * S2R;
	if(err != 0) {
		setError(err);
		return;
	}

	m_centerLon = temp;
	setInit();
}

void Pseudocylindrical::loadFromParams() {
	Projection::loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
}