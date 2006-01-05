
#ifndef PROJINFO_H
#define PROJINFO_H

struct ProjInfo
{
	ProjInfo() : projNumber(-1), projZone(-1), projUnits(-1), projDatum(-1)
	{
		for(int i = 0; i < 15; i++)
			projParams[i] = 0.0;
	}

	int projNumber;
	int projZone;
	int projUnits;
	int projDatum;
	double projParams[15];
};

#endif