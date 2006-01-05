
#ifndef PROJTESTER_DATA_H
#define PROJTESTER_DATA_H

#include "ImageSpace.h"
#include "ProjInfo.h"

class ProjTesterData
{
public:
	ProjTesterData() {}
	ProjTesterData(const ImageSpace& imgSpace, const ProjInfo& fromProjInfo, const ProjInfo& toProjInfo):
	m_imgSpace(imgSpace), m_fromProjInfo(fromProjInfo), m_toProjInfo(toProjInfo) {}

	virtual ~ProjTesterData() {}

	void setImageSpace(const ImageSpace& imgSpace) { m_imgSpace = imgSpace;}
	void setFromProjInfo(const ProjInfo& projInfo) { m_fromProjInfo = projInfo;}
	void setToProjInfo(const ProjInfo& projInfo) { m_toProjInfo = projInfo;}

	ImageSpace getImageSpace() const {return m_imgSpace;}
	ProjInfo getFromProjInfo() const {return m_fromProjInfo;}
	ProjInfo getToProjInfo() const {return m_toProjInfo;}


private:

	ImageSpace m_imgSpace;
	ProjInfo m_fromProjInfo;
	ProjInfo m_toProjInfo;
};

#endif