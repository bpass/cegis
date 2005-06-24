
#include "cylindrical.h"

Cylindrical::Cylindrical() : Projection(), m_centerLon(0.0), m_centerLat(0.0) 
{
}

Cylindrical::Cylindrical(double gctpParams[], ProjUnit units, ProjDatum dat):
Projection(gctpParams, units, dat), m_centerLon(0.0), m_centerLat(0.0) 
{
	setParamLoad();
}

void Cylindrical::setCenterLat(double lat) 
{
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

void Cylindrical::setCenterLon(double lon) 
{
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

void Cylindrical::_loadFromParams() {
	Projection::_loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
}


