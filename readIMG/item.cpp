#include "item.h"

Item::Item():
m_type(UNDEFINED)
,m_pointer(false)
,m_pointerType(NOPOINTER)
,m_numEntries(0){
	m_enumData.clear();
}

void Item::setName(std::string name) {
	m_name = name;
}

void Item::setType(ItemTypes type) {
	m_type = type;
	switch(type) {
		case U1:
			m_typeName = "unsigned char";
			break;
		case U2:
			m_typeName = "unsigned char";
			break;
		case U4:
			m_typeName = "unsigned char";
			break;
		case UCHAR:
			m_typeName = "unsigned char";
			break;
		case CHAR:
			m_typeName = "char";
			break;
		case USHORT:
			m_typeName = "unsigned short";
			break;
		case SHORT:
			m_typeName = "short";
			break;
		case ULONG:
			m_typeName = "unsigned long";
			break;
		case LONG:
			m_typeName = "long";
			break;
		case TIME:
			m_typeName = "time";
			break;
		case FLOAT:
			m_typeName = "float";
			break;
		case DOUBLE:
			m_typeName = "double";
			break;
		case COMPLEX:
			m_typeName = "complex";
			break;
		case DCOMPLEX:
			m_typeName = "double complex";
			break;
		case BASEDATA: 
			m_typeName = "basedata";
			break;
		case ENUM: 
			m_typeName = "enum";
			break;
	}	
}

void Item::setType(const std::string& name) {
	m_type = PREV_DEFINED;
	m_typeName = name;
}

void Item::setPointer(bool pointer, PointerTypes type) {
	m_pointer = pointer;
	m_pointerType = type;
}

void Item::setNumEntries(unsigned int num) {
	m_numEntries = num;
}

bool Item::addEnumData(std::string data) {
	for(unsigned int i = 0; i < m_enumData.size(); i++) {
		if(m_enumData[i] == data)
			return false;
	}
	
	m_enumData.push_back(data);
	return true;
}

bool Item::isPointer() const {
	return(m_pointer);
}

PointerTypes Item::getPointerType() const {
	return(m_pointerType);
}

std::vector<std::string> Item::getEnumData() const {
	return(m_enumData);
}

std::string Item::getEnumVal(unsigned int index) const{
	if(index >= m_enumData.size())
		throw(GeneralException("Item::getEnumVal()->index out of bounds"));
	return(m_enumData[index]);
}
unsigned int Item::getEnumLength() const {
	return(m_enumData.size());
}

unsigned int Item::getNumEntries() const {
	return(m_numEntries);
}
std::string Item::getName() const {
	return(m_name);
}

std::string Item::getTypeName() const {
	return(m_typeName);
}
ItemTypes Item::getType() const {
	return(m_type);
}