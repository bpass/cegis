#include "GCTPCPPTester.h"
#include "NullPointerException.h"
#include "ProjErrorException.h"

void GCTPCPPTester::setToProjInfo(const ProjInfo& projInfo)
{
	ProjTesterData::setToProjInfo(projInfo);

	ProjCode projCode = (ProjCode)(projInfo.projNumber);
	
	if(m_toProj)
	{
		delete m_toProj;
		m_toProj = NULL;
	}

	m_toProj = createProj(projCode);


	if(m_toProj)
	{
		m_toProj->setParams((double*)projInfo.projParams);
		m_toProj->setDatum((ProjDatum)projInfo.projDatum);
		m_toProj->setUnits((ProjUnit)projInfo.projUnits);
	}
}

void GCTPCPPTester::setFromProjInfo(const ProjInfo& projInfo)
{
	ProjTesterData::setToProjInfo(projInfo);

	ProjCode projCode = (ProjCode)(projInfo.projNumber);
	
	if(m_fromProj)
	{
		delete m_fromProj;
		m_fromProj = NULL;
	}

	m_fromProj = createProj(projCode);

	if(m_fromProj)
	{
		m_fromProj->setParams((double*)projInfo.projParams);
		m_fromProj->setDatum((ProjDatum)projInfo.projDatum);
		m_fromProj->setUnits((ProjUnit)projInfo.projUnits);
	}
}
Point GCTPCPPTester::doForward(const Point& geoP)
{
	if(!m_toProj)
		throw(NullPointerException(__LINE__, __FILE__));

	Point forwardP; 
	m_toProj->forward(geoP.y, geoP.x, &forwardP.x, &forwardP.y);

	if(m_toProj->errorOccured())
		throw(ProjErrorException(m_toProj->error(), m_toProj->number(), "forward"));

	return(forwardP);
}

Point GCTPCPPTester::doInverse(const Point& projP)
{
	if(!m_fromProj)
		throw(NullPointerException(__LINE__, __FILE__));

	Point inverseP; 
	m_fromProj->inverse(projP.x, projP.y, &inverseP.y, &inverseP.x);

	if(m_fromProj->errorOccured())
		throw(ProjErrorException(m_fromProj->error(), m_fromProj->number(), "inverse"));

	return(inverseP);
}

Projection* GCTPCPPTester::createProj(const ProjCode projCode)
{
	switch(projCode)
	{
		case GEO:
			return(NULL);
		case _UTM:
			return (new UTM());
			break;
		case SPCS:
			return(NULL);
		case ALBERS:
			return (new AlbersConEqArea());
			break;
		case LAMCC:
			return ( new LambertCC());
			break;
		case MERCAT:
			return (new Mercator());
			break;
		case PS:
			return (new PolarStereo());
			break;
		case POLYC:
			return (new Polyconic());
			break;
		case EQUIDC:
			return (new EquidistantC());
			break;
		case TM:
			return (new TransverseMercator());
			break;
		case STEREO:
			return (new Stereo());
			break;
		case LAMAZ:
			return (new LambertAzimuthal());
			break;
		case AZMEQD:
			return (new AzEquidistant());
			break;
		case GNOMON:
			return(new Gnomonic());
			break;
		case ORTHO:
			return(new Orthographic());
			break;
		case GVNSP:
			return(new GenVertNSP());
			break;
		case SNSOID:
			return(new Sinusoidal());
			break;
		case EQRECT:
			return(new Equirectangular());
			break;
		case MILLER:
			return(new Miller());
			break;
		case VGRINT:
			return(new VanDerGrinten());
			break;
		case HOM:
			return(new HotineObMerc());
			break;
		case ROBIN:
			return(new Robinson());
			break;
		case SOM:
			return(new SpaceObMerc());
			break;
		case ALASKA:
			return(new AlaskaConformal());
			break;
		case GOOD:
			return(new GoodeH());
			break;
		case MOLL:
			return(new Mollweide());
			break;
		case IMOLL:
			return(new IntMollweide());
			break;
		case HAMMER:
			return(new Hammer());
			break;
		case WAGIV:
			return(new WagnerIV());
			break;
		case WAGVII:
			return(new WagnerVII());
			break;
		case OBEQA:
			return(new OblatedEqArea());
			break;
		case USDEF: 
			return(NULL);
		default:
			return(NULL);
	}
}







