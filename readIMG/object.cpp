#include "object.h"

Object::Object() : m_name("") {
	m_items.clear();
	m_childObjects.clear();
}

Object::~Object() {
	for(unsigned int i = 0; i < m_childObjects.size(); i++) {
		if(m_childObjects[i])
			delete m_childObjects[i];
	}

	for(unsigned int i = 0; i < m_items.size(); i++) {
		if(m_items[i])
			delete m_items[i];
	}
}
void Object::addItem(Item* item) {
	if(!getItem(item->getName())) 
		m_items.push_back(item);
		
	else
		throw(GeneralException("Object::addChildITem->Error: item allready exists"));
}

void Object::addChildObject(Object* child) {
	if(child) {
		if(!getChildObject(child->getName())) {
			m_childObjects.push_back(child);
			child->m_parent = this;
		}
		else
			throw(GeneralException("Object::addChildObject->Error: object allready exists"));
	}
	else
		throw(GeneralException("Object::addChildObject->Error: adding NULL object"));
}


Object* Object::getChildObject(const std::string& name) const {
	for(unsigned int i = 0; i < m_childObjects.size(); i++) {
		if(m_childObjects[i]->getName() == name)
			return(m_childObjects[i]);
	}
	return(NULL);
}

Object* Object::getChildObject(unsigned int index) const {
	if(index < m_childObjects.size())
		return(m_childObjects[index]);
	else
		throw(GeneralException("Object::getChildObject()->index out of bounds"));
return(NULL);
}

unsigned int Object::getNumChildren() const {
	return(m_childObjects.size());
}

void Object::setName(std::string name) {
	m_name = name;
}

std::string Object::getName() const  {
	return(m_name);
}

Item* Object::getItem(const std::string& name) const {
	for(unsigned int i = 0; i < m_items.size(); i++) {
		if(m_items[i]) {
			if(m_items[i]->getName() == name)
				return(m_items[i]);
		}
		else 
			throw(GeneralException("Object::getItem()->null item in list"));
	}
	return(NULL);
}

Item* Object::getItem(unsigned int index) const {
	if(index < m_items.size()) {
		if(m_items[index])
			return(m_items[index]);
		else
			throw(GeneralException("Object::getItem()->null item in list"));
	}
	else
		throw(GeneralException("Object::getItem()->index out of bounds"));
}

unsigned int Object::numItems() const {
	return(m_items.size());
}

void Object::display() {
	listItems();
	listObjects();
}

void Object::listItems() {
	printf("typedef struct {\n");
	
	for(unsigned int i = 0; i < m_items.size(); i++) {
		
		if(m_items[i]) {
			
			int numEntries = m_items[i]->getNumEntries();

			printf("\t%s", m_items[i]->getTypeName().c_str());
			
			if(m_items[i]->isPointer()) 
				printf("*");
			printf(" %s", m_items[i]->getName().c_str());
			if(numEntries > 1)
				printf("[%d]", numEntries);
			
			if((m_items[i]->getType() == ENUM) && !m_items[i]->isPointer()) {
				printf(" {");
				for(unsigned int j = 0; j < m_items[i]->getEnumLength(); j++) {
					printf("%s", m_items[i]->getEnumVal(j).c_str());
					if((j+1) < m_items[i]->getEnumLength())
						printf(",");
				}
				printf("}");
			}
			
			printf(";\n");
		}
	
		else
			throw(GeneralException("Object::listItems()->null item found"));
	}

	printf("}%s;\n\n",m_name.c_str());
}

void Object::listObjects() {
	for(unsigned int i = 0; i < m_childObjects.size(); i++) {
		
		if(m_childObjects[i])
			m_childObjects[i]->listItems();
		else
			throw(GeneralException("Object::listObjects()->null child object found"));
	}
}



