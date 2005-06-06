
#include "conic.h"
#include "projexception.h"

Conic::Conic(): Projection(),
m_centerLon(0.0), m_centerLat(0.0), m_stdParallelLat1(0.0), m_stdParallelLat2(0.0)
{
}

Conic::Conic(double gctpParams[], ProjUnit units, ProjDatum dat) :
Projection(gctpParams, units, dat)
{
	loadFromParams();
}

void Conic::loadFromParams() 
{
	Projection::loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
	setStdParallel1(m_gctpParams[2]);
	setStdParallel2(m_gctpParams[3]);
}

void Conic::setCenterLat(double lat) 
{
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLat()"));

	m_centerLat = temp;
	setInit();
}

void Conic::setCenterLon(double lon) 
{
		long err = 0;
	double temp = 0;

	temp = Util::paksz(lon, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setCenterLon()"));

	m_centerLon = temp;
	setInit();
}

void Conic::setStdParallel1(double lat) 
{
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setStdParallel1()"));

	m_stdParallelLat1 = temp;
	setInit();
}

void Conic::setStdParallel2(double lat) 
{
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Conic::setStdParallel2()"));

	m_stdParallelLat2 = temp;
	setInit();
}
