
#include "azimuthal.h"

Azimuthal::Azimuthal(): Projection(), m_centerLon(0.0), m_centerLat(0.0)
{
	
}

Azimuthal::Azimuthal(double gctpParams[], ProjUnit units, ProjDatum dat):
Projection(gctpParams, units, dat), m_centerLon(0.0), m_centerLat(0.0)
{
	loadFromParams();
}

void Azimuthal::setCenterLat(double lat)
{
	convertAndSetAngle(m_centerLat, lat);
	setInit();
}

void Azimuthal::setCenterLon(double lon)
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

void Azimuthal::loadFromParams()
{
	Projection::loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
}
