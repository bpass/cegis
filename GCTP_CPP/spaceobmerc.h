
#ifndef SPACE_OB_MERC_H
#define SPACE_OB_MERC_H

#include "projection.h"

class SpaceObMerc : public Projection
{
public:

	SpaceObMerc();
	SpaceObMerc(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setSatNum(long num) {m_satNum = num; setInit();}
	void setPath(long path) {m_path = path; setInit();}
	void setMode(long mode) {m_mode = mode; setInit();}
	void setAlf(double val);
	void setCenterLon(double lon);
	void setTime(double time) {m_time = time; setInit();}
	void setStart(double start) {m_start = start; setInit();}

	long satNum() {return m_satNum;}
	long path() {return m_path;}
	long mode() {return m_mode;}
	double alf() {return m_alf;}
	double centerLon() {return m_centerLon;}
	double time() {return m_time;}
	double start() {return m_start;}

protected:

	long m_satNum;
	long m_path;
	long m_mode;

	double m_alf;
	double m_time;
	double m_a;
	double m_b;
	double m_a2;
	double m_a4;
	double m_c1;
	double m_c3;
	double m_q;
	double m_t;
	double m_u;
	double m_w;
	double m_xj;
	double m_p21;
	double m_sa;
	double m_ca;
	double m_es;
	double m_s;
	double m_start;
	double m_centerLon;

	void som_series(double* fb, double* fa2, double* fa4, double* fc1, double* fc3, double* dlam);

	void init();

	void _loadFromParams();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);
};

#endif