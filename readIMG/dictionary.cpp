

#include <ctype.h>
#include "dictionary.h"
#include "ParseException.h"
#include "exception.h"
#include "item.h"
#include "object.h"

Dictionary::Dictionary():
m_curIndex(0)
,m_currentObject(NULL)
,m_newObject(NULL)
,m_newItem(NULL) 
{
}

Dictionary::Dictionary(const std::string& dictString) 
: 
m_curIndex(0)
,m_currentObject(NULL)
,m_newObject(NULL)
,m_newItem(NULL)
,m_dictString(dictString)
{
  m_objectMap.clear();
  m_rootObjectVec.clear();
  try {
	parse();
  }

  catch(ParseException e) {
		e.printMessage();
		cleanup();
  }

  catch(GeneralException e) {
	e.printMessage();
	cleanup();
  }
}

Dictionary::~Dictionary() {
	for(unsigned int i = 0; i < m_rootObjectVec.size(); i++){
		if(m_rootObjectVec[i])
			delete m_rootObjectVec[i];
	}
}

void Dictionary::cleanup() {
	bool currentExists = false;
	bool newObjExists = false;
	bool newItemExists = false;
	std::map<std::string, Object*>::iterator iter;

	//if m_currentObject or m_newObject point to something
	if(m_currentObject || m_newObject) {

		//check all entries in the object map to see if they point somewhere;
		for(iter = m_objectMap.begin(); iter != m_objectMap.end(); iter++) {
			
			if(m_currentObject == iter->second)
				currentExists = true;
			if(m_newObject == iter->second)
				newObjExists = true;
		}

		//if they don't point to the same thing
		if(m_currentObject != m_newObject) {
			//if m_currentObject or m_newObject point to something
			//but have not been added any object tree,
			//delete them to avoid memory leaks.
			if(!currentExists) 
				delete m_currentObject;
				
			if(!newObjExists) 
				delete m_newObject;
			
		}
		
		//else if they point to the same area
		else {
			//if one of them is in an object tree,
			//delete one and set both to null.
			if(!currentExists || !newObjExists) 
				delete m_currentObject;
		}
		m_currentObject = NULL;
		m_newObject = NULL;
	}

	//check if m_newItem is pointing to something.
	if(m_newItem) {
		for(unsigned int i = 0; i < m_rootObjectVec.size(); i++) {
			for(unsigned int j = 0; j < m_rootObjectVec[j]->numItems(); j++) {
				if(m_newItem == (m_rootObjectVec[j]->getItem(j))) 
					newItemExists = true;
			}
		}

		if(!newItemExists) 
			delete m_newItem;

		m_newItem = NULL;
		
	}

	//demolish the object trees
	for(unsigned int i = 0; i < m_rootObjectVec.size(); i++){
		if(m_rootObjectVec[i])
			delete m_rootObjectVec[i];
	}
	m_rootObjectVec.clear();
	m_objectMap.clear();
}

	

void Dictionary::loadDict(const std::string& dictString) {
	//demolish object trees.
	cleanup();
	m_dictString = dictString;
	try {
		parse();
	}

	catch(ParseException e) {
		e.printMessage();
		cleanup();
	}
	catch(GeneralException e) {
		e.printMessage();
		cleanup();
	}
}
void Dictionary::match(char c) {
	if(m_dictString[m_curIndex] != c) {
		std::string error("Parse Error: Mismatched token");
		error = error + " expected " + c + " got " + m_dictString[m_curIndex] + "\n";
		throw(ParseException(error.c_str()));
	}
	m_curIndex++;
}

Object* Dictionary::getObject(const std::string& name) {
	std::map<std::string, Object*>::const_iterator iter;
	iter = m_objectMap.find(name);
	if(iter != m_objectMap.end())
		return(iter->second);
	else
		return(NULL);
}
void Dictionary::addObject(Object* parent, Object* child) {
	std::string name;
	if(child) {
		//if this object is a sub-object.
		//construct appropriate name.
		if(parent) 
			name = parent->getName() + "::" + child->getName();

		else 
			name = child->getName();
			
		

		//if this object hasn't allready been defined.
		if(!getObject(name)) {

			//add it to the global object map.
			std::pair<std::string, Object*> newMapPair(name, child);

			//if no parent and it doesn't allready exist, add it to the vector
			//of root objects
			if(!parent)
				m_rootObjectVec.push_back(child);
			
		}
		else
			throw(GeneralException("Dictionary::addObject()->object allready exists"));
	}
	else
		throw(GeneralException("Dictionary::addObject()->adding null object"));
}

void Dictionary::parse() {
	try {
		while(parseNextObject()) 
			addObject(NULL, m_newObject);	
	}

	catch(ParseException e) {
		e.printMessage();
	}

	catch(GeneralException e) {
		e.printMessage();
	}
}

//***************rework for sub objects!!**************************
bool Dictionary::parseNextObject() {
	
	if(m_dictString[m_curIndex] == '.')
		return false;

    m_newObject = new Object;

	match('{');

	while(parseNextItem()) {
		if(m_newItem) 
			m_newObject->addItem(m_newItem);
				
	}

	match('}');

	//get object name
	std::string temp("");
	while(m_dictString[m_curIndex] != ',') {
		temp += m_dictString[m_curIndex];
		m_curIndex++;
	}
	m_newObject->setName(temp);
	match(',');
	return true;
}

bool Dictionary::parseNextItem() {
	//Item format: N:*|P['Type']<enum length: ["name"], <"name2">,...,<"name_n">,>["item name"]
	//
	//  {} = item definition
	//  [] = required field
	//  <> = optional field
	//  N = How many of each item (do we have an array?)

	//No more items in this object
	if(m_dictString[m_curIndex] == '}') {

		m_newItem = NULL;
		return false;
	}
	
	m_newItem = new Item;

	char curType = '\0';
	std::string temp("");
	
	//strip out N
	char c = m_dictString[m_curIndex];
	while(c != ':') {
		//Check digits
		if(!isdigit(c))
			throw(ParseException("Parse Error: Expected number\n", m_curIndex));
		else 
			temp += c;
		m_curIndex++;
		c = m_dictString[m_curIndex];
	}
	
	m_newItem->setNumEntries(atoi(temp.c_str()));

	match(':');

	//check for *|P 
	if((m_dictString[m_curIndex] == '*')) {
		m_newItem->setPointer(true, FIXED);
		m_curIndex++;
	}
		
	else if(m_dictString[m_curIndex] == 'p') {
			m_newItem->setPointer(true, VARIABLE);
			m_curIndex++;
	}
	
	//get data type of item
	curType = m_dictString[m_curIndex];
	m_curIndex++;
	switch(curType) {
		case U1:
		case U2:
		case U4:
		case UCHAR:
		case CHAR:
		case USHORT:
		case SHORT:
		case ULONG:
		case LONG:
		case TIME:
		case FLOAT:
		case DOUBLE:
		case COMPLEX:
		case DCOMPLEX:
		case BASEDATA: {
			temp.clear();
			ItemTypes curTypeT = (ItemTypes)curType;
			m_newItem->setType(curTypeT);
			char c = m_dictString[m_curIndex];
			while(c != ',') {
				temp += c;
				m_curIndex++;
				c = m_dictString[m_curIndex];
			}
			
			m_newItem->setName(temp);
			match(',');
		 }
			break;
		case ENUM: {
			char c = '\0';
			ItemTypes curTypeT = (ItemTypes)curType;
			m_newItem->setType(curTypeT);
			int enumCount = -1;
			temp.clear();
			std::string curEnumData("");

			//strip out N
			c = m_dictString[m_curIndex];
			while(c != ':') {
				//Check digits
				if(!isdigit(c))
					throw(ParseException("Parse Error: Expected number\n", m_curIndex));
				else 
					temp += c;
				m_curIndex++;
				c = m_dictString[m_curIndex];
			}
			
			enumCount = atoi(temp.c_str());

			match(':');
			
			//strip out each enum constant
			for(int i = 0; i < enumCount; i++) {
				char c = m_dictString[m_curIndex];
				while(c != ',') {
					curEnumData += c;
					m_curIndex++;
					c = m_dictString[m_curIndex];
				}
				m_newItem->addEnumData(curEnumData);
				curEnumData.clear();
				match(',');
			}

//			m_curIndex++;
			temp.clear();

			c = m_dictString[m_curIndex];
			while(c != ',') {
				temp += c;
				m_curIndex++;
				c = m_dictString[m_curIndex];
			}
			
			m_newItem->setName(temp);
			match(',');
		}
			break;
		case PREV_DEFINED: {
			std::string objType("");
			std::string itemName("");
			char c = '\0';

			c = m_dictString[m_curIndex];
			while(c != ',') {
				objType += c;
				m_curIndex++;
				c = m_dictString[m_curIndex];
			}
			
			match(',');

			c = m_dictString[m_curIndex];
			while(c != ',') {
				itemName += c;
				m_curIndex++;
				c = m_dictString[m_curIndex];
			}
				
			
			match(',');

	//		if(!getObject(objType)) {
	//			std::string error("Parse Error: Use of undefined object type: ");
	//			error += objType;
	//			throw(ParseException(error.c_str(), m_curIndex));
	//		}

			if(m_newObject->getItem(itemName)) {
				std::string error("Parse Error: ");
				error += itemName + " allready in use";
				throw(ParseException(error.c_str(), m_curIndex));
			}

			else {
				m_newItem->setType(objType);
				m_newItem->setName(itemName);
//				m_curIndex++;
		    }
		  }
			break;

		//object definition inside of another object.
		case OBJ_DEF: {
			if(m_newItem) {
				delete m_newItem;
				m_newItem = NULL;
			}

			Object* temp = m_currentObject;
			m_currentObject = m_newObject;
			parseNextObject();
			m_currentObject->addChildObject(m_newObject);
			addObject(m_currentObject, m_newObject);
			m_newObject = m_currentObject;
			m_currentObject = temp;
			}
		break;
		default:
			throw(ParseException("Error: Unknown Data Type", m_curIndex));
		}
	return(true);
}

void Dictionary::display() {
	for(unsigned int i = 0; i < m_rootObjectVec.size(); i++) {
		if(m_rootObjectVec[i])
			m_rootObjectVec[i]->display();
		else
			throw(GeneralException("Dictionary::display()->null object found"));
	}
}
	

	


			
	




