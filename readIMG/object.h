
#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include <algorithm>
#include "item.h"
#include "exception.h"

//! The Object class.

/*! This class is used to represent the object definitions in an Imagine 
	image file dictionary.
*/
class Object {
public:
	//! Default constructor.
	Object();

	//! Destructor.
	~Object(); 

	//! Add an item to the object.

	/*! This function allows the user to add an item to 
		the object. Items are what make up an object. They
		can be standard data types such as int or long, or
		other objects that have allready been defined.
		\param item Pointer to the new item to add.
		\throw GeneralException.
	*/
	void addItem(Item* item);

	//! Add a child object to this object.

	/*! This function allows the user to add a child 
		a child object to the object. Child objects 
		are objects that are defined inside other
		objects.
		\param child The child object to add.
		\throw GeneralException.
	*/
	void addChildObject(Object* child);

	//! Set the name of the object.

	/*! This function allows you to set the name of 
		the object.
		\param name The new name for the object.
	*/
	void setName(std::string name);

	//! Get the name of the object.
	std::string getName() const ;

	//! Get an item specified by "name".

	/*! This function allows you to get an item by specifiying
		its name.
		\param name the name of the item.
		\return Item* NULL if the item doesn't exist.
		\throw GeneralException
	*/
	Item* getItem(const std::string& name) const;

	//! Get an item by its index.

	/*! This function allows you to retrieve an item 
		by specifying its 0-based index.
		\param index the index of the item.
		\throw GeneralException
	*/
	Item* getItem(unsigned int index) const;

	//! Get the number of items in the object.
	unsigned int numItems() const;

	//! Get a child object by its name.
	
	/*! This function allows the user to retrieve a child
		object by specifying its name.
		\param name the name of the object.
		\return Object* NULL if the child object doesn't exist.
		\throw GeneralException.
	*/
	Object* getChildObject(const std::string& name) const;

	//! Get a child object by its index.

	/*! This function allows you to get a child object by
		specifying its 0-based index.
		\param index the index of the child object.
		\throw GeneralException
	*/
	Object* getChildObject(unsigned int index) const;

	//! Get the number of child objects.
	unsigned int getNumChildren() const;

	//! Print the object description to stdout
	void display();
	
private:	
	
	//! Print all child objects to stdout.
	void listObjects();

	//! Print all items to stdout
	void listItems();

	//! The name of the object.
	std::string m_name;

	//! The parent of the object.
	Object* m_parent;

	//! The items vector.
	std::vector<Item*> m_items;

	//! The child objects vector.
	std::vector<Object*> m_childObjects;
};

#endif