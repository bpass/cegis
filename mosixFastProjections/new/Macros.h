#ifndef __USGSMOSIX_MACROS_H_
#define __USGSMOSIX_MACROS_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/13 22:27:40 $
 *
 * \version 0.1
 * 
 * \file Macros.h
 * 
 * \brief Global macros. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

/// To enable debug messages for the program 
// #define USE_DEBUG

/// For debug messages. 
#ifdef USE_DEBUG
   #define WRITE_DEBUG(x) std::cerr << __FILE__ << ":" << __LINE__ << ":" << x;
#else
   #define WRITE_DEBUG(x) ;
#endif 

#endif 
