
#include "conic.h"
#include "projexception.h"

Conic::Conic(): Projection(), m_c(0.0), m_e(0.0), m_es(0.0), m_rh(0.0), m_ns0(0.0),
m_centerLon(0.0), m_centerLat(0.0), m_stdParallelLat1(0.0), m_stdParallelLat2(0.0)
{
}

Conic::Conic(double gctpParams[], int units, long datum, long spheroid) :
Projection(gctpParams, units, datum, spheroid), m_c(0.0), m_e(0.0), m_es(0.0), m_rh(0.0), m_ns0(0.0),
m_centerLon(0.0), m_centerLat(0.0), m_stdParallelLat1(0.0), m_stdParallelLat2(0.0)
{
}

void Conic::setCenterLat(double lat) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLat()"));

	m_centerLat = temp;
}

void Conic::setCenterLon(double lon) {
		long err = 0;
	double temp = 0;

	temp = Util::paksz(lon, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLon()"));

	m_centerLon = temp;
}

void Conic::setStdParallel1(double lat) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setStdParallel1()"));

	m_stdParallelLat1 = temp;
}

void Conic::setStdParallel2(double lat) {
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setStdParallel2()"));

	m_stdParallelLat2 = temp;
}
