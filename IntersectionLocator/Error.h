/*
 * File: Error.h
 * Purpose: Define the Error_Print macro in one place for all files
 *          that will use it.  The idea is that later this macro
 *          can be redefined to be nothing or to call another
 *          error reporting function.
 * Programmer: Ryan Stelzleni
 * Date: 7-9-04
 */


// Majic numbers for CVS
// $Id: Error.h,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $


#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdio.h>

#define Error_Print(x) fprintf(stderr, x)

#endif // ERROR_H_INCLUDED
