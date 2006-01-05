
#include "GCTPTester.h"
#include <stdlib.h>
Point GCTPTester::doForward(const Point& geoP)
{
	Point retP; // x / y coord
	double inCoord[2] = {0};
	double outCoord[2] = {0};

	inCoord[0] = geoP.y;
	inCoord[1] = geoP.x;
	
	long inSys = getFromProjInfo().projNumber; //Equirectangular
	long inZone = getFromProjInfo().projZone; //doesn't matter
	long inUnit = getFromProjInfo().projUnits; //meters
	long inDatum = getFromProjInfo().projDatum; //no datum shifting, in and out datum the same.
	long outDatum = inDatum;

	long outSys = getToProjInfo().projNumber;
	long outZone = getToProjInfo().projZone;
	long outUnit = getToProjInfo().projUnits;

	gctp(inCoord, &inSys, &inZone, getFromProjInfo().projParams, &inUnit, &inDatum, 
		 NULL, NULL, NULL, NULL, outCoord, &outSys, &outZone, getToProjInfo().projParams, 
		 &outUnit, &outDatum, NULL, NULL, NULL);

	retP.x = outCoord[0];
	retP.y = outCoord[1];

	return(retP);
}

Point GCTPTester::doInverse(const Point& projP)
{
	Point retP; //lon / lat coord
	double inCoord[2] = {0};
	double outCoord[2] = {0};

	inCoord[0] = projP.x;
	inCoord[1] = projP.y;
	
	long outSys = 0; //GEO
	long outZone = 0; //doesn't matter
	long outUnit = 4; //degrees of arc
	long outDatum = getToProjInfo().projDatum; //no datum shifting, in and out datum the same.
	long inDatum = outDatum;

	long inSys = getToProjInfo().projNumber;
	long inZone = getToProjInfo().projZone;
	long inUnit = getToProjInfo().projUnits;

	gctp(inCoord, &inSys, &inZone, getFromProjInfo().projParams, &inUnit, &inDatum, 
		 NULL, NULL, NULL, NULL, outCoord, &outSys, &outZone, getToProjInfo().projParams, 
		 &outUnit, &outDatum, NULL, NULL, NULL);

	retP.x = outCoord[1];
	retP.y = outCoord[0];

	return(retP);
}

