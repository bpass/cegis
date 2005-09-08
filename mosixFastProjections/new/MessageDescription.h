
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file MessageDescription.h 
 * 
 * \brief Header file which contains struct that identifies the purpose of 
 * a socket transmission, i.e., a message "header" or "envelope."
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#ifndef __USGSMOSIX_MESSAGEDESCRIPTION_H_ 
#define __USGSMOSIX_MESSAGEDESCRIPTION_H_

#include <sys/types.h>
#include "SlaveInfo.h"

namespace USGSMosix 
{
    /// A type of header for all socket communication
    struct MessageDescription
    {
      unsigned long int m_sender; 
      
      enum Type { MSG_JOB = 1001, 
                  MSG_RESULT, 
                  MSG_NOWORK, 
                  MSG_ABORTING, 
                  MSG_OK,
                  MSG_ERROR = 0 } m_type;
    };

} // namespace 

#endif // __USGSMOSIX_MESSAGEDESCRIPTION_H_

