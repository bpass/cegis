
#ifndef PROJINFO_DATA_H
#define PROJINFO_DATA_H

class ProjInfoData
{
public:
	ProjInfoData();
	ProjInfoData(const ImageSpace& imgSpace, const ProjInfo& projInfo);
	virtual ~ProjInfoData() {}

	void setImageSpace(const ImageSpace& imgSpace) { m_imgSpace = imgSpace;}
	void setProjInfo(const ProjInfo& projInfo) { m_projInfo = projInfo;}