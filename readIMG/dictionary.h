
#ifndef DICT_H
#define DICT_H
#include <string>
#include <map>
#include <vector>
#include "object.h"
#include "item.h"

//! The Dictionary Class.

/*! This class is used to parse the specially formatted ASCII string
	stored at the end of an Imagine HFA file (the dictionary) that 
	describes all objects contained in the image file.
*/
class Dictionary {
public:

	//! Default constructor.
	Dictionary();
	
	//! Constuctor.

	/*! This constructor allows you to pass in a dictionary string,
		and have it parsed automatically.
		\param dictString The dictionary string.
	*/
	Dictionary(const std::string& dictString); 
	
	//!Desctructor.
	~Dictionary();

	//! Get an object by name.

	/*! This function allows you to retrieve a pointer to an object
		by referring to its name.  If the object you wish to obtain 
		is declared inside of another object, you must use the following 
		naming convention: "ParentObject::ChildObject".
		\param name The name of the object.
	*/
	Object* getObject(const std::string& name); 

	//! Add an object to the dictionary.

	/*! This function allows you to add an object to the 
		dictionary.  If the object is declared inside of
		another object, you must pass a pointer to its
		parent object.  If you are not adding a child
		object, use NULL for the parent.
		\param parent The parent of the object being added.
		\param child The new object being added.
	*/
	void addObject(Object* parent, Object* child); 

	//! Load a dictionary from a dictionary string.

	/*! This function allows you to load a new dictionary 
		from a dictionary string.
		\param dictString The dictionary string to be loaded.
	*/
	void loadDict(const std::string& dictString);

	//! Print the dictionary to stdout.
	void display();
private:

	//! Parse the dictionary string.

	/*! This function begins the parsing process 
		for the currently loaded dictionary string.
		\throw GeneralException.
		\throw ParseException.
	*/
	void parse();

	//! Parse the next object in the dictionary.

	/*! This function parses out the next object in the
		dictionary, or if we have objects withing objects
		parses the next child object of the current object.
		\return bool True if another object exists, false otherwise.
		\throw GeneralException.
		\throw ParseException.
	*/
	bool parseNextObject();

	//! Parse the next item in the current object.
	
	/*! This function parses the next item definition within
		the current object.
		\return bool True if there is another item to parse, false otherwise.
		\throw GeneralException.
		\throw ParseException.
	*/
	bool parseNextItem();
	
	//! Match the current input token with the token passed in.

	/*! The purpose of this function is to check the current input 
		token for a certain expected character value. For instance,
		if we were at a point in the string where we should get a ':',
		we would type match(':') and it would check the current 
		input token against ':', and if these did not match, a
		ParseException would be thrown.  One other important thing to 
		note is that, if the characters match, the current index into
		the dictionary string is incremented by one. For example, if you
		called match(':'), and the character at the current index was 
		':', then the function would increment the current index by 1
		and return.
		\param c The character to match.
		\throw ParseException
	*/
	void match(char c);
	
	//! Demolish all of the object trees and reclaim all memory.
	void cleanup(); 

	//! Current index into the dictionary string.
	int m_curIndex;

	//! The dictionary string.
	std::string m_dictString;

	//! Current object being parsed.
	Object* m_currentObject;

	//! New object being built.
	Object* m_newObject;

	//! New item being built.
	Item* m_newItem;

	//! Map of names to object pointers.
	std::map<std::string, Object*> m_objectMap;
	
	//! Vector of all objects at the root level.
	std::vector<Object*> m_rootObjectVec;
};


#endif