/**
 * @file Error.h
 * Purpose: Define the Error_Print macro in one place for all files
 *          that will use it.  The idea is that later this macro
 *          can be redefined to be nothing or to call another
 *          error reporting function.
 * @author Ryan Stelzleni
 * @date 7-9-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Error.h,v 1.3 2004/10/22 13:30:26 rstelzleni Exp $


#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdio.h>

#define Error_Print(x) fprintf(stderr, x)

#endif // ERROR_H_INCLUDED
