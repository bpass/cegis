/******************************************************************************
 * File:     types.h                                                          *
 * Author:   Jason Maestri                                                    *
 * Date:     22-Jun-1999                                                      *
 *                                                                            *
 *****************************************************************************/

/******************************************************************************
 ******************************************************************************
 ** Description:                                                             **
 **        This file defines the various data types and constants used in    **
 **    the shape file classes.                                               **
 **                                                                          **
 ******************************************************************************
 *****************************************************************************/
#ifndef TYPES_H
#define TYPES_H



// Shape Type Codes:
#define NULLSHAPE   0
#define API_POINT       1
#define POLYLINE    3
#define POLYGON     5
#define MULTIPOINT  8
#define POINTZ      11
#define POLYLINEZ   13
#define POLYGONZ    15
#define MULTIPOINTZ 18
#define POINTM      21
#define POLYLINEM   23
#define POLYGONM    25
#define MULTIPOINTM 28
#define MULTIPATCH  31

//File Header Length (in 16 bit words)
#define FILE_HEADER_LEN 50
#define HEADER_BYTES 100

//File Code  and Version # (Used to properly identify a shape file)
#define FILE_CODE      9994
#define VERSION        1000
const int UnusedSize = 5;
#define UNUSEDVAL		0     	    

#define RECORD_HEADER_LEN 4
#define	REC_HEADER_BYTES 8

//Array Size
#define MAXBUFFER 255

//Constant for Interpolation
#define MAXTOUCHING 4
#define VARIANCE 1.5



#define MAXSTREAMS 3
#define PIE 3.14159

//FIRED EVENT TYPES 
enum DIRECTION { NONE, RIGHT, UPRIGHT, UP, UPLEFT, LEFT, DOWNLEFT, DOWN, DOWNRIGHT };
enum EVENTTYPE { ANYERROR = -999, READ = 1, CALCULATE = 2, WRITE = 3 };
enum FLAG { BOUNDARY = 1, JOINT = 2, TROUBLE = 4, HIT = 8 };

#endif



