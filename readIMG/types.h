#ifndef TYPES_H
#define TYPES_H

//! Specifies all the standard datatypes that can occur in an image.

/*! The data type specifiers are as follows:
	U1: Unsigned 1-bit
	U2: Unsigned 2-bit
	U4: Unsigned 4-bit
	UCHAR: Unsigned Char
	CHAR: Signed Char
	USHORT: Unsigned Short
	SHORT: Signed Short
	ENUM: 
	ULONG: Unsigned Long
	LONG: Signed Long
	TIME: 
	FLOAT: 32-bit floating point
	DOUBLE: 64-bit floating point
	COMPLEX: 64-bit complex
	DCOMPLEX: 128-bit complex
	BASEDATA:
	PREV_DEFINED: Previously defined object
	OBJ_DEF: Object definition within another object
	UNDEFINED: No type set.
*/
typedef enum {U1='1', U2='2', U4='4', UCHAR='c', CHAR='C', USHORT='s', SHORT='S', ENUM='e', ULONG='l', LONG='L', 
		      TIME='t', FLOAT='f', DOUBLE='d', COMPLEX='m', DCOMPLEX='M', BASEDATA='b',
	          PREV_DEFINED='o', OBJ_DEF='x', UNDEFINED='%'} ItemTypes; 

//! Specifies the different types of pointers.

/*! The types are as follows:
	FIXED: Points to a fixed-size data type (i.e. int, long, etc...)
	VARIABLE: Points to a dynamically sized data type (i.e. Object)
*/
typedef enum {FIXED='*', VARIABLE='p', NOPOINTER='%'} PointerTypes;

#endif