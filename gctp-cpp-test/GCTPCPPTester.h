#ifndef GCTPCPPTEST_H
#define GCTPCPPTEST_H

#include "gctpcpp.h"
#include "ProjTester.h"
#include "ProjTesterData.h"

class GCTPCPPTester : public ProjTester, private ProjTesterData
{
public:
	GCTPCPPTester() : ProjTesterData(), m_fromProj(NULL), m_toProj(NULL) {}
	GCTPCPPTester(const ImageSpace& imgSpace, const ProjInfo& fromProjInfo, const ProjInfo& toProjInfo) : 
	ProjTesterData(imgSpace, fromProjInfo, toProjInfo), m_fromProj(NULL), m_toProj(NULL){}

	virtual ~GCTPCPPTester() 
	{ 
		delete m_fromProj;
		delete m_toProj;
	}

	virtual Point doForward(const Point& geoP);
	virtual Point doInverse(const Point& projP);

	void setImageSpace(const ImageSpace& imgSpace) {ProjTesterData::setImageSpace(imgSpace);}
	void setFromProjInfo(const ProjInfo& projInfo);
	void setToProjInfo(const ProjInfo& projInfo);


	ImageSpace getImageSpace() const {return(ProjTesterData::getImageSpace());}
	ProjInfo getFromProjInfo() const {return(ProjTesterData::getFromProjInfo());}
	ProjInfo getToProjInfo() const {return(ProjTesterData::getToProjInfo());}
	Projection* createProj(const ProjCode projCode);

private:
	
	Projection* m_fromProj;
	Projection* m_toProj;
};

#endif