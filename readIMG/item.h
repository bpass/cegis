#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include "types.h"
#include "exception.h"

//! The Item class.

/*! This class is used to store information about the items that make up
	objects inside of an Imagine data dictionary. For instance, this could
	be used to describe a character array or unsinged long members inside 
	of an object.
*/
class Item {
public:

	//! Default constructor.
	Item();

	//! Set the name item name.

	/*! This function allows you to set the name of the item.
		\param name The new name for the item.
	*/
	void setName(std::string name);

	//! Set the data type of the item.

	/*! This function allows you to set the data type of 
		the item. The parameter ItemTypes is an enum of 
		allowable data types.
		\param type The data type to set this item to.
	*/
	void setType(ItemTypes type);

	//! Set the data type of the item.

	/*! This function allows you to set the data type of the
		item by actual name, rather than integer type specifier.
		The purpose of this function is to allow the user to set
		the data type to a type that doesn't exist in the list
		of standard types, such as objects.
		\param name The name of the data type.
	*/
	void setType(const std::string& name);

	//! Set the number of occurences of this item in the object.

	/*! This function allows you to set how many instances of this 
		item exist in the object (used for arrays of items).
		\param num Number of items in the object.
	*/
	void setNumEntries(unsigned int num);
	
	//! Set wether or not this item is a pointer.

	/*! This function allows you to set whether or not
		this item is a pointer, and what kind of pointer it is.
		If the pointer type is set to STATIC, it points 
		to a statically sized data type (such as an int).
		If the pointer type is set to VARIABLE, it is a 
		pointer to a variably sized object.
		\param pointer True -> is a pointer False -> Not a pointer.
		\param type STATIC if pointing to an item of fixed size,
					DYNAMIC if pointing to a variable sized item.
	*/
	void setPointer(bool pointer, PointerTypes type);

	//! Add to the enum list of this item is an enum.

	/*! This function allows you to add a new identifier
		to the item's enum list if it is an enum.
		\param data The new identifier to add to the list.
	*/
	bool addEnumData(std::string data);

	//! Get the number of items in the enum list.
	unsigned int getEnumLength() const;

	//! Get the number of this item in the current object.
	unsigned int getNumEntries() const;

	//! Get the vector of enum data.
	std::vector<std::string> getEnumData() const;

	//! Get a particular enum identifier.

	/*! This function allow you to get a particular identifer
		from the enum list as specified by "index".
		\param index Identifier index to return.
		\throw GeneralExcpetion
		\return std::string The enum identifier
	*/
	std::string getEnumVal(unsigned int index) const;

	//! Get the name of the item.
	std::string getName() const;

	//! Get the string identifier for the item type.
	std::string getTypeName() const;

	//! Get the integer identifier for the item type.
	ItemTypes getType() const;

	//! Check if the item is a pointer.
	bool isPointer() const;

	//! Get the type of pointer if the item is a pointer.
	PointerTypes getPointerType() const;
private:

	//! The name of the item.
	std::string m_name;

	//! Vector of enum data if it is enum.
	std::vector<std::string> m_enumData;

	//! Item type.
	ItemTypes m_type;

	//! String name for item type.
	std::string m_typeName;

	//! Is this item a pointer?
	bool m_pointer;

	//! Pointer type if it's a pointer.
	PointerTypes m_pointerType;

	//! Number of entries in the object.
	unsigned int m_numEntries; 						  
};

#endif