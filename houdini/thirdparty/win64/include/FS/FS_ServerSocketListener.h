/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_ServerSocketListener.h ( FS Library, C++ )
 *
 * COMMENTS:
 *	This event generator opens a socket and waits for connections.
 *	When a client makes a connection and sends data, a callback
 *	is called.  Note that multiple active connections can exist at once.
 */

#ifndef __FS_ServerSocketListener_h__
#define __FS_ServerSocketListener_h__

#include "FS_API.h"
#include <UT/UT_String.h>
#include <UT/UT_SysClone.h>
#include "FS_EventGenerator.h"
#include "FS_ConnectedSocketListener.h"
class UT_NetSocket;

class FS_API FS_ServerSocketListener : public FS_EventGenerator
{
public:
    // Use this static method to create a instance of this class.
    // Note that this method might return null if the socket could not be
    // created (eg. if the port is already in use).
    //     callback: this function will be called when data has been read from
    //         the socket.  callback_data will be passed to this function.
    //     ip_mask: restricts who is allowed to connect to this socket.  If
    //         it is null, it defaults to +.+.+.+.
    //     remap_privileged_ports: if set, port numbers will be mapped into
    //         the range of unprivileged port numbers.
    static FS_ServerSocketListener *createSocketListener(int port,
				FS_ConnectedSocketListener::Callback callback,
				void *callback_data = 0,
				const char *ip_mask = 0,
				bool remap_privileged_ports = false);
    virtual ~FS_ServerSocketListener();

    virtual const char	*getClassName() const
				{ return "FS_ServerSocketListener"; }

    // If the original port number was privileged and was remapped, getPort()
    // will return the actual port number that was used and getRequestedPort()
    // will return the port number that was requested.
    int			 getPort() const;
    int			 getRequestedPort() const { return myRequestedPort; }

    // The IP mask determines which machines are allowed to connect to the
    // socket.
    const UT_String	&getIPMask() const { return myIPMask; }

    // The listener can be told to close the server socket when a child
    // process exits.  To turn off this behaviour, use INVALID_PID for
    // the pid.  If delete_on_child_exit is set, this listener will also
    // uninstall and delete itself when the child exits.  If a callback is
    // given, it will be called when we delete ourselves because the child
    // process is no longer running.
    //
    // Note that if the listener is waiting on a child process that has not
    // exited and someone else deletes the listener, the callback will be
    // called with listener_forced_to_close set to true.
    typedef void (*ChildExitedCallback)(FS_ServerSocketListener &,
					void *callback_data,
					bool listener_forced_to_close);
    void		 closeSocketOnChildExit(
				pid_t child_pid,
				bool delete_on_child_exit = false,
				ChildExitedCallback child_exited_callback = 0,
				void *callback_data = 0);

    // This method is called from the wrapper class to see if events are
    // waiting.
    virtual bool	 areEventsWaiting();

    // When there is data on the socket (ie. some is trying to connect)
    // to it, this method is called.
    virtual int		 processEvents();

    // Override FS_EventGenerator's getFileDescriptor() method to return the
    // file descriptor of the server socket.
    virtual int		 getFileDescriptor();

    // Override FS_EventGenerator's getPollTime() method.  If the listener
    // is waiting for a child to exit, it will return a positive poll time
    // so it can check if the child is still running.  Otherwise, it will
    // return a negative poll time, so it only processes events if a socket
    // connection is attempted.
    virtual int		 getPollTime();

protected:
    // This protected constructor is called by the createSocketListener()
    // static method.
    FS_ServerSocketListener(UT_NetSocket *server_socket,
			     int requested_port,
			     FS_ConnectedSocketListener::Callback callback,
			     void *callback_data,
			     const char *ip_mask);

    // Subclasses may need access to the server socket.
    UT_NetSocket	*getServerSocket() { return myServerSocket; }

    // Subclasses should override this method to create the correct type of
    // FS_ConnectedSocketListener subclass.  Since the connected socket
    // listener will delete itself when the connection is closed, this
    // method does not return anything.
    virtual void	 createConnectedSocketListener();

private:
    void		 closeServerSocket();
    void		 acceptConnectionFromClient();
    bool		 shouldStopBecauseChildHasExited();

    // Data:
    UT_NetSocket	*myServerSocket;
    int			 myRequestedPort;
    FS_ConnectedSocketListener::Callback myDataReadyCallback;
    void		*myDataReadyCallbackData;
    UT_String		 myIPMask;

    pid_t		 myChildPid;
    bool		 myDeleteOnChildExit;
    ChildExitedCallback	 myChildExitedCallback;
    void		*myChildExitedCallbackData;
};

#endif
