/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetPacket.h ( UT Library, C++)
 *
 * COMMENTS:
 *	This class is a special network stream that asynchronously reads data
 *	into houdini. It is also responsible for listening for connections or
 *	attempting to connect to sockets. Endian issues are handled. 
 *	The I/O is handled in a separate thread.
 *
 * CAVEATS:	- This is built partially upon UT_NetPacket.
 *		- This is an abstract base class - you must define the
 *		  processData method.
 */

#ifndef __UT_NetStream__
#define __UT_NetStream__

#include "UT_API.h"
#include "UT_Lock.h"
#include "UT_String.h"

#include <sstream>
#include <iosfwd>

class UT_String;
class UT_Thread;
class UT_NetSocket;
class UT_NetPacket;

#define NET_STREAM_IN		0
#define NET_STREAM_OUT		1

#define NET_STREAM_SOCKET	0
#define NET_STREAM_PLUG		1

class UT_API UT_NetStream
{
public:
		 UT_NetStream();
    virtual	~UT_NetStream();

    // call first before attempting to open a connection. Returns 0 if already
    // open (or for stop, if it's not open). 
    int		startIOThread();
    int		stopIOThread();

    // call to open and close client/server connections.
    void	openConnection(const char *address, int port,
			       int mode = NET_STREAM_IN,
			       int connect_type = NET_STREAM_PLUG);
    void	closeConnection();

    // returns 1 if startIOThread successfully started a thread.
    int		isThreadStarted() const { return myIOThread ? 1 : 0; }

    // returns 1 if the socket is valid.
    int		isValidStream() const { return myValidAddress; } 

    // returns 1 if a connection has been established.
    int		isConnected() const { return myIsConnected; }

    // when writing, this is used to tell the thread that data is ready to
    // be sent. The thread will then call transmitData(). 
    void	setDataReady();

    // when ready, check this flag to tell you if data is ready (if so, it
    // will clear the flag). 
    int		isDataReady();
    
protected:
    // Fill these methods in to send data, or to receive a packet of data. Both
    // methods operate inside the thread.
    virtual void  receiveData(std::istream &is) = 0;
    virtual void transmitData(std::ostream &os) = 0;

    // called when a connection is made.
    virtual void justConnected() { ; }
    virtual void dataReceived() { ; }

    static void *ioLoopEntry(void *data);
    void	 ioLoop();

private:

    void	breakConnection();
    
    UT_NetSocket	*myServer;
    UT_NetSocket	*myConnection;
    UT_NetPacket	*myPacket;
    UT_Thread		*myIOThread;

    // address to create socket at or connect to. 
    UT_String		myAddress;
    int			myPort;
    int			myMode;
    int			myConnectType;

    // query flags.
    int			myIsConnected;
    int			myValidAddress;
    UT_Lock		myThreadLock;

    // flags to control the thread.
    int			myDataReadyFlag;
    int			mySocketInfoSetFlag;
    int			myCloseConnectionFlag;
    int			myThreadStopFlag;

    UT_NetStream	*myThis;
};

#endif
