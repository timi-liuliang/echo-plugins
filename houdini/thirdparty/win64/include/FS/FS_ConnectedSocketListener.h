/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_ConnectedSocketListener.h ( FS Library, C++ )
 *
 * COMMENTS:
 *	This event generator listens for data that's sent to a connected
 *	socket.  When data is sent to this socket, a callback is called.
 *	If the socket is closed, this event generator removes itself and
 *	schedules the UI_Queue to delete it.  You must create this event
 *	generator with new and you should not keep a pointer to it.
 *
 *	You probably don't want to use this class directly.  Instead, you
 *	probably want to use FS_ServerSocketListener.
 */

#ifndef __FS_ConnectedSocketListener_h__
#define __FS_ConnectedSocketListener_h__

#include "FS_API.h"
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_String.h>
#include "FS_EventGenerator.h"
class UT_NetSocket;

class FS_API FS_ConnectedSocketListener : public FS_EventGenerator
{
public:
    typedef void (*Callback)(void *callback_data, const char *data, int length,
			     UT_NetSocket &connection_socket);

    // The callback is called when data has been read from the socket.  The
    // socket is passed into the callback so it can write data out to the
    // socket.  The ip mask restricts who is allowed to connect to this
    // socket.  If the ip mask is null it defaults to +.+.+.+.
    FS_ConnectedSocketListener(UT_NetSocket &server_socket, Callback callback,
				void *callback_data = 0,
				const char *ip_mask = 0);
    virtual ~FS_ConnectedSocketListener();

    virtual const char		*getClassName() const
				 { return "FS_ConnectedSocketListener"; }
    bool			 getIsInstalled() const
				 { return myConnectionSocket != NULL; }

protected:
    // This method returns true when there is data on the socket.
    virtual bool	 areEventsWaiting();

    // When there is data on the socket, this method is called.
    virtual int		 processEvents();

    // Return the file descriptor of the server socket.
    virtual int		 getFileDescriptor();

private:
    void		 closeConnectionSocket();

    // Data:
    void		*myCallbackData;
    Callback		 myCallback;
    UT_NetSocket	*myConnectionSocket;
    UT_WorkBuffer	 myDataBuffer;
    bool		 myProcessingCallback;
};

#endif
