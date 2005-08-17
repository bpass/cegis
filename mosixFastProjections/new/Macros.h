#ifndef __USGSMOSIX_MACROS_H_
#define __USGSMOSIX_MACROS_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file Macros.h
 * 
 * \brief Global constants, enumerations, and typedefs used by  
 * the MOSIX fast projections project.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

/* To enable debug messages for the program */
#define USE_DEBUG

/* For debug messages. */
#ifdef USE_DEBUG
   #define WRITE_DEBUG(x) std::cerr << __FILE__ << ":" << __LINE__ << ":" << x;
#else
   #define WRITE_DEBUG(x) ;
#endif 

#endif 
