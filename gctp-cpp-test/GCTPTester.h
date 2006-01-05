#ifndef GCTP_TEST_H
#define GCTP_TEST_H

#include <gctp/proj.h>
#include "ProjTester.h"
#include "ProjTesterData.h"

class GCTPTester : public ProjTester, private ProjTesterData
{
public:
	GCTPTester() : ProjTesterData() {}
	GCTPTester(const ImageSpace& imgSpace, const ProjInfo& fromProjInfo, const ProjInfo& toProjInfo) : 
			   ProjTesterData(imgSpace, fromProjInfo, toProjInfo) {}

	virtual ~GCTPTester() {}

	virtual Point doForward(const Point& geoP);
	virtual Point doInverse(const Point& projP);

	void setImageSpace(const ImageSpace& imgSpace) {ProjTesterData::setImageSpace(imgSpace);}
	void setFromProjInfo(const ProjInfo& projInfo) {ProjTesterData::setFromProjInfo(projInfo);}
	void setToProjInfo(const ProjInfo& projInfo) {ProjTesterData::setToProjInfo(projInfo);}
	
	ImageSpace getImageSpace() const {return(ProjTesterData::getImageSpace());}
	ProjInfo getFromProjInfo() const {return(ProjTesterData::getFromProjInfo());}
	ProjInfo getToProjInfo() const {return(ProjTesterData::getToProjInfo());}
};

#endif



