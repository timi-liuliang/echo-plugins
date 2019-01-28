/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SocketListener.h (UT Library, C++)
 *
 * COMMENTS:
 * 	This class monitors a socket asynchronously and notifies
 * 	subclasses (via onSocketEvent()) when socket events occur.
 * 	This class is useful for reading from a socket asynchronously.
 */

#ifndef __UT_SocketListener__
#define __UT_SocketListener__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

typedef enum
{
    UT_SOCKET_ERROR		= 1,
    UT_SOCKET_READ_READY	= 1 << 1,
    UT_SOCKET_WRITE_READY	= 1 << 2,
} UT_SocketListenerEvent;

class UT_NetSocket;
class UT_Thread;

class UT_API UT_SocketListener {
public:
    /// Calls stop().
    virtual			~UT_SocketListener();

    /// Returns a bitmask of UT_SocketListenerEvents indicating which
    /// events trigger a call to onSocketEvent().
    int				 getEventTypes() const { return myEventTypes; }

    /// Returns the socket that is being monitored.
    UT_NetSocket		*getSocket() const { return mySocket; }

    /// Returns true if the socket is being monitored for events.
    bool			 isListening() const { return myListening; }

    /// Sets the events that trigger a call to onSocketEvent().
    /// event_types is a bitmask of UT_SocketListenerEvents.
    void			 setEventTypes(int event_types);

    /// Start/stop listening on the socket provided to create().
    /// Successive calls to either method have no effect.
    void			 start();
    void			 stop();

    /// Returns the amount of memory owned by this UT_SocketListener
    virtual int64		 getMemoryUsage(bool inclusive) const;

protected:
    /// Constructs an instance of UT_SocketListener that monitors
    /// the specified socket for the specified event types.
    /// onSocketEvent() is called whenever the specified events
    /// occur.
				 UT_SocketListener(
					UT_NetSocket *socket,
					int event_types = UT_SOCKET_READ_READY);

    /// Subclasses must implement this method in order to receive
    /// notification of socket events (such as data being available
    /// for reading).  event_types is a bitmask of UT_SocketListenerEvents
    /// and indicates which events were fired.
    virtual void		 onSocketEvent(int event_types) = 0;

private:
    static void			*doListenThread(void *data);

    UT_NetSocket		*mySocket;
    UT_Thread			*myThread;
    int				 myEventTypes;
    bool			 myListening;
};

#endif

