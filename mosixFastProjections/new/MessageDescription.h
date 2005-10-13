
/*!
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/13 22:27:40 $
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
#include "ClientSocket.h"
#include "SerializableInterface.h"

namespace USGSMosix
{
    
/// A type of header for all socket communication
struct MessageDescription : public SerializableInterface
{
    unsigned long int m_sender;
 
    enum Type { MSG_JOB = 1001,
                MSG_RESULT,
                MSG_REQUEST,
                MSG_NOWORK,
                MSG_ABORTING,
                MSG_OK,
                MSG_ERROR = 0 } m_type;
   
    MessageDescription() : m_sender(0), m_type(MSG_ERROR) {}
    MessageDescription(unsigned long pid) : m_sender(pid) {}
    MessageDescription(unsigned long pid, Type ty) :
        m_sender(pid), m_type(ty) {}

    virtual void exportToSocket( ClientSocket & socket )const
    {
        int eType = static_cast<int>(m_type);
        socket.send(&m_sender, sizeof(m_sender));
        socket.send(&eType,sizeof(eType));
        return;

    }
    
    static MessageDescription createFromSocket( ClientSocket & socket )
    {
        int eType(0);
        unsigned int sender(0);
        socket.receive(&sender, sizeof(sender));
        socket.receive(&eType, sizeof(eType));
        return MessageDescription( sender, static_cast<Type>(eType));
    }

};

} // namespace

#endif // __USGSMOSIX_MESSAGEDESCRIPTION_H_

