/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetMessage.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Very similar to UT_NetStream, but rather than using threads
 *	to interleave packets successfully, it is designed
 *	around using ::select for the same purpose.
 *
 *	This allows the user to only have to worry about dealing with complete
 *	messages.
 *
 *	The key tools for end users is the UT_NetMessage and the 
 *	UT_NetExchange.
 */

#ifndef __UT_NetMessage__
#define __UT_NetMessage__

#include "UT_API.h"

#include "UT_NetSocket.h"
#include "UT_ValArray.h"
#include "UT_Interrupt.h"

class UT_StopWatch;

///
/// UT_NetMessage bears similarity to a UT_NetPacket in that
/// it tries to abstract the necessitiy of retrying TCP/IP streams
/// until the message is completely received.  However, it is built
/// around the idea of having a datapump, pumpData, to handle this,
/// allowing one to interleave many messages in one thread using
/// select()
///
/// Write Protocol:
///	STATE_WRITE:
///		send 32bit length in network byte order
///		if length greater than 1<<31, send 32 bit -1
///			followed by 64 bit length in network-byte order.
///		send myLength from myData
///	STATE_WAITACK:
///		receive 'j'
///		shutdown the connection
///
/// Read Protocol:
///	STATE_READ:
///		receive 32bit length in network byte order
///		if length is -1, receive 64bit length in network byte order.
///		set myLength and allocate myData
///		receive myLength into myData
///	STATE_SENDACK:
///		send 'j'
///	STATE_READFIN:
///		receive '' (ie, wait till safe to close)
///
class UT_API UT_NetMessage
{
public:
    UT_NetMessage();

    /// Frees the allocated data buffer.
    ~UT_NetMessage();

    /// What state we are in.
    /// Thanks to the vagaries of TCP/IP we need an application
    /// level ACK to verify the data made it across the wire
    /// before we do our shutdown.
    enum TransmitState
    {
	STATE_INVALID,

	STATE_READ,	/// Read message
	STATE_SENDACK,	/// Send ack
	STATE_READFIN,	/// Connection closed after ack sent.

	STATE_WRITE,	/// Send message
	STATE_WAITACK,	/// Wait for connection ack.

	STATE_READPIPE,	/// Blind read of a pipe for our size
	STATE_READPIPE_BLOSC,	/// Blind read blosc pipe.
	STATE_READPIPE_DONE,
	STATE_WRITEPIPE,/// Blind write of a pipe for our size
	STATE_WRITEPIPE_DONE,
    };

    /// Sends or receives data from the socket until the message
    /// is complete.  When it is complete or errored, returns true.  Returns
    /// false if you should put it back in the select loop.
    bool	pumpData();

    /// Trys to connect the socket, if already connected a no-op
    /// Returns false if still not connected, true if connected or error.
    /// failed is set to true if the connectionf ailed due to a 
    /// UT_CONNECT_FAILED as opposed to being delayed by a UT_WOULD_BLOCK
    bool	tryConnection(bool *failed=0);

    /// The socket we are attached to.
    UT_NetSocket *getSocket() const { return mySocket; }
    /// File descriptor of socket.
    int		 getSocketFD() const { return mySocket->getSocket(); }

    /// Adds our file descriptor to the select set and update
    /// maxfd.
    void	 addToFDSet(fd_set *set, int &maxfd) const;

    TransmitState	state() const { return myState; }
    bool	 isErrored() const { return myErrorState; }
    void	 flagErrored() { myErrorState = true; }

    const char	*data() const { return myData; }
    char	*data() { return myData; }
    exint	 length() const { return myLength; }

    /// Resizes, useful for pipe messages.
    void	 resetLength(TransmitState state, exint newlen);

    /// Extracts data from the message, applying proper byteswaps.
    /// Offset is in bytes.  Does not need to be aligned.
    int64	 extractInt64(exint offset);
    int32	 extractInt32(exint offset);
    fpreal32	 extractFloat32(exint offset);
    fpreal64	 extractFloat64(exint offset);
    int16	 extractInt16(exint offset);
    int8	 extractInt8(exint offset);

    /// Inserts data into the message, applying proper byte swaps.
    /// Offset is in bytes.  Does not need to be aligned.
    void	 overwriteInt64(exint offset, int64 val);
    void	 overwriteInt32(exint offset, int32 val);
    void	 overwriteFloat32(exint offset, fpreal32 val);
    void	 overwriteFloat64(exint offset, fpreal64 val);
    void	 overwriteInt16(exint offset, int16 val);
    void	 overwriteInt8(exint offset, int8 val);

    /// Determine source/sink of this message.
    /// Because we shutdown the socket on completion of the
    /// protocol, we always gain ownership of the socket and
    /// will handle deleting it ourselves.
    void	 setReadSocket(UT_NetSocket *socket);
    void	 setWriteSocket(UT_NetSocket *socket);
    void	 setWriteSocket(const char *addr, int port);

    /// The headersize, in bytes, is prepended as nulls to the message.
    /// The headersize *is* included in the myLength, the receiver
    /// doesn't know about the distinction between header and data!
    void	 setWriteData(const char *data, exint len) 
		 { setWriteDataWithHeader(0, data, len); }
    void	 setWriteData(const char *data) 
		 { setWriteData(data, strlen(data)); }
    void	 setWriteDataWithHeader(exint headersize, const char *data, exint len);
    void	 setWriteDataWithHeader(exint headersize, const char *data)
		 { setWriteDataWithHeader(headersize, data, strlen(data)); }

    /// Allocates a blank write buffer of the given size.
    /// It is assumed the caller will fill it out by writing to data()
    void	 setWriteDataLength(exint bufsize)
		 { setWriteDataWithHeader(bufsize, 0, 0); }

    static int64	 totalBytesSent();
    static int64	 totalBytesReceived();
    static void		 clearByteCounters();

    /// Tracks total network time.  This does not include time
    /// spent in background threads, only the time spent to sync
    /// up to such threads.  This includes transfer time and sync
    /// time.  (It is hard to determine what time spent waiting on
    /// a pipe is the other system still computing, and what is just
    /// waiting on the network to transfer data...)
    /// There is no way to clear this because some load balancing
    /// systems may want it to properly accumulate between frames
    static fpreal64	 totalNetworkTime();

    /// Adds the given number of seconds to total network timecounter.
    static void		addToTotalNetworkTime(fpreal64 netnewtime);

    void	 setState(TransmitState state) { myState = state; }

    /// Compresses our data into swap buffer, then swap the pointers
    /// so we will send the compressed buffer.  The header measures the
    /// number of bytes that will be copied over directly.
    bool		compress(exint header);

    /// Decompress our block and swap so we point at the decompressed
    /// buffer.  If it fails, it was a corrupt buffer of some sort.
    /// The header is copied directly.
    bool		decompress(exint header);

private:

    /// Tracking variables.
    static int64	 ourTotalBytesSent;
    static int64	 ourTotalBytesReceived;
    static fpreal64	 ourTotalNetworkTime;

    UT_NetSocket	*mySocket;
    exint		 myCapacity;
    exint		 myLength;
    char		*myData;

    // When we compress/decompress we swap buffers with this
    // buffer.  We can thus avoid re-creating every frame in cases where
    // we cache our messages.
    char		*mySwapData;
    exint		 mySwapLength;
    exint		 mySwapCapacity;

    TransmitState	 myState;
    bool		 myErrorState;

    // Internal state of where we are in terms of pumping...
    // -4 to -1 are for sending the length.
    exint		 myDataPos;

    // Flag if we are pumping the second length.
    bool		 myProcessing64BitLength;

    int32		 myNetLength;	// htonl version of length, -1 if 64bit
    int64		 myNetLength64;	// htonl version of length64
};


///
/// UT_NetMessagePump
/// 
/// Interleaves processing from possibly many different message
/// sources.
///
/// postMessage() can be used to queue messages to send after
/// their setWriteSocket and setWriteData has been invoked.
///
/// listenSocket() will invoke accept() and add to the completed()
/// list messages as they are done.
///
///
class UT_API UT_NetMessagePump
{
public:
		 UT_NetMessagePump();
    /// On destruction all messages on our internal lists will be destroyed.
    virtual	~UT_NetMessagePump();

    /// Adds the message to our queue to process.
    /// We take ownership of the UT_NetMessage until it is removed
    /// from one of our lists.
    void	 postMessage(UT_NetMessage *msg);

    /// Resets, but does *not* free any messages it has.
    void	 reset()
    {
	myWorkList.clear();
	myErrorList.clear();
	myCompleteList.clear();
    }

    /// List of all completed incoming messages. 
    ///
    /// To process a message from the completed list,
    /// UT_NetMessage *msg = pump.completed()(i);
    /// pump.completed()(i) = 0;
    /// ... use msg ...
    /// delete msg;
    UT_ValArray<UT_NetMessage *> &completed() { return myCompleteList; }

    /// List of messages that have entered the error status.
    /// This includes failed outgoing messages as well as incomplete
    /// incoming messages.
    UT_ValArray<UT_NetMessage *> &errored() { return myErrorList; }
    
    /// Starts listening to the given socket for messages
    /// We do not own this socket, caller should delete it (but not
    /// before stopping listening or deleting the pump!)
    /// Connections accepted from this socket will be read
    /// via UT_NetMessage's protocol.
    void	 listenSocket(UT_NetSocket *socket);
    /// Stops listening from a given socket.
    void	 stopListening(UT_NetSocket *socket);

    /// Processes active messages and listens for incoming messages.
    /// Returns true if there is the *potential* of more work - if
    /// listening to a socket this is always true!
    bool	 pumpData(int timeoutms);

    /// The number of messages still pending.
    int		 pendingMessages() const { return myWorkList.entries(); }

    void	 deleteSentMessages(bool shoulddelete) { myDeleteSent = shoulddelete; }
    void	 receiveAnnounce(bool receiveannounce) { myReceiveAnnounce = receiveannounce; }
protected:
    bool			    myDeleteSent;
    bool			    myReceiveAnnounce;
    UT_ValArray<UT_NetMessage *>    myWorkList;
    UT_ValArray<UT_NetMessage *>    myErrorList;
    UT_ValArray<UT_NetMessage *>    myCompleteList;
    UT_ValArray<UT_NetSocket *>     myServerList;
};

class ut_NetPeerData
{
public:
    ut_NetPeerData() {}
    virtual ~ut_NetPeerData() {}
    ut_NetPeerData(const ut_NetPeerData &src)
    {
	*this = src;
    }

    const ut_NetPeerData &operator=(const ut_NetPeerData &src)
    {
	myAddress.harden(src.myAddress);
	myPort = src.myPort;
	myPeer = src.myPeer;
	return *this;
    }

    UT_String		myAddress;
    int			myPort;
    int			myPeer;
};

///
/// UT_NetExchange
///
/// Uses a tracker to exchange data between peers.
/// The tracker handles peer discovery and synchronization.  A unique
/// jobname is used to allow multiple net exchanges to use the same
/// tracker.
///
/// EXTERNAL PROTOCOL:
///
/// UT_NetMessages accepted or returned from this have an 8 byte
/// header.  The fields are filled out for you on a sendData()
/// and can be queried to determine the message source when
/// pulling completed messages from the result list.
///
/// The first byte is the message type.  If it is 'p', it is
/// peer data.   Users of UT_NetExchange should only see these
/// messages as the other are internal tracker messages.
/// 'P' is used for blosc compressed peer data, it will be uncompressed
/// and revealed to the user as a 'p' data.
///
/// extractInt16(4): Destination peer #
/// extractInt16(6): Source peer #
///
/// Anything 8 and after is user data.
///
/// INTERNAL PROTOCOL:
///
/// 	From the tracker:
///
/// Message of type 'b' is a barrier value.  The 32 bit network
/// byte order barrier value starts at offset 1.
///
/// Message type of 'c' is a peer list.  A space delimited list
/// of address port peer triples is present, one for each peer.
///
/// Message type of 'd' is a done flag.  It informs that all
/// the peers have reported done to the tracker.
///
/// Message type of 'e' is an error.  One of the peers reported
/// an error to the tracker that got broadcast back.
///
/// 	To the tracker:
///
/// Messages to the tracker all have the format
/// command port peer npeer jobname
///
/// port is the listen port used by this peer.
/// peer is the peer number
/// npeer is the number of peers
/// jobname is the name of the current job
///
/// command is one of:
/// acquire: A peer is logging on to this jobname.
/// done:    A peer has all the data it cares about from the netexchange.
/// error:   A peer has encountered a fatal network error and wants a clean
///          shutdown.
/// barrierset: Establish a barrier, the current value is peer.
/// barrierwait: Will recevie a done message when the peer value is reached or
/// 	     exceeded by the named barrier.
///
class UT_API UT_NetExchange
{
public:
		 UT_NetExchange(const UT_StringHolder &trackeraddr, int trackerport,
			    int peer, int npeer, const UT_StringHolder &jobname);
    virtual	~UT_NetExchange();

    class 	AutoNetworkTimer
    {
    public:
	AutoNetworkTimer(UT_NetExchange *xchg) { myXchg = xchg; myXchg->startNetworkTimer(); }
	~AutoNetworkTimer() { myXchg->stopNetworkTimer(); }
	UT_NetExchange		*myXchg;
    };


    /// Posts the data to be sent to the specific destination
    /// machine.
    /// The data will be copied into an internal buffer.
    /// Note that destpeer could be this, in which case we'll just
    /// have effected a very slow copy.
    void	 sendData(int destpeer, const char *data, exint len);

    /// Gains ownership of the message.  The message must have
    /// an 8 byte header reserved for the net exchange protocol.
    /// The rest of the data in the net message is unaffected.
    void	 sendData(int destpeer, UT_NetMessage *msg);

    /// Flags this peer as done.  You should still process the pump until
    /// interrupted or completed, however, as the other peers
    /// may not yet be done.   Likewise, you may still continue
    /// to get new requests from other peers.
    void	 sendDone();

    /// Returns true until the tracker notifies us that all of the
    /// peers have invoked sendDone() or an error occurs.
    bool	 pumpData(int timeoutms);

    /// Pumps the message pump until we have received the expected
    /// number of messages and the tracker reports us as done.  The
    /// messages in the completed array must be deleted by the caller.
    /// UT_Interrupt is used to interrupt this.
    /// true if everything finished properly, false if there was
    /// an interrupt or error.
    bool	 receiveDataLoop(UT_Array<UT_NetMessage *> &completed,
				int expectedmessages, int timeoutms = 100);


    /// Uses a background thread to run the message loop.  You should
    /// not inspect/alter UT_NetExchange or its owned UT_NetMessages
    /// until finished.
    bool	 asyncReceiveDataLoop(UT_Array<UT_NetMessage *> &completed,
				int expectedmessages, int timeoutms = 100);
    /// Blocks until the asyncReceiveDataLoop completes/errors.
    bool	 finishAsyncReceiveDataLoop();

    /// Pumps the message pump until we have received the expected
    /// number of data messages and the tracker reports us as done.  The
    /// messages in the completed array must be deleted by the caller.
    /// UT_Interrupt is used to interrupt this.
    /// true if everything finished properly, false if there was
    /// an interrupt or error.
    ///
    /// The callback processes any message with a 'r' as the 8th byte.
    /// Final messages (which do not get further processed) should have 
    /// 'd' as the 8th byte - they will
    /// be added to the completed list and count to the expectedmessages.
    /// It should be a function object taking two parameters,
    /// UT_NetExchange * and UT_NetMessage *.
    ///
    /// UT_Interrupt is used to interrupt this.
    /// true if everything finished properly, false if there was
    /// an interrupt or error.
    template <class CallbackType>
    bool	processDataLoop(UT_Array<UT_NetMessage *> &complist,
				int expectedmessages,
				CallbackType callback,
				int timeoutms = 100)
    {
	// Because we append to complist, allow for the case where
	// we are accumulating...
	int goal = complist.entries() + expectedmessages;
	bool done = false;
	UT_Interrupt *boss = UTgetInterrupt();
	AutoNetworkTimer	timer(this);

	while (1)
	{
	    if (boss->opInterrupt())
		return false;

	    if (!pumpData(timeoutms))
		break;

	    for (exint i = 0; i < completed().entries(); i++)
	    {
		UT_NetMessage *msg = completed()(i);
		completed()(i) = 0;

		if (msg->extractInt8(8) == 'r')
		{
		    callback(this, msg);
		    delete msg;
		}
		else
		    complist.append(msg);
	    }

	    // See if we got our done package.
	    if (!done && gotPeerList() && (complist.entries() >= goal))
	    {
		sendDone();
		done = true;
	    }
	}

	return !isErrored();
    }

    /// The net exchange goes into an error state when the connection
    /// the tracker sends an error message. 
    /// peer-to-peer errors are dealt with by sending the tracker
    /// an error message which is supposed to broadcast the error
    /// back to all the peers.  Error recovery can thus be effected
    /// provided it is the tracker that stays up.
    bool	 isErrored() const { return myErrorFromTracker; }

    /// Same semantics as UT_NetMessagePump.  Stores all the messages
    /// that have successfully arrived.
    /// This first 8 bytes of theses messages is the header with
    /// which you can extract the source peer of the message.
    UT_ValArray<UT_NetMessage *> &completed() { return myCompleted; }

    /// Returns if we have received a peer list from the tracker.
    bool	 gotPeerList() const { return myGotPeerList; }

    void	 setCompressionThreshold(exint threshold_bytes) { myCompressionThreshold = threshold_bytes; }
    exint	 compressionThreshold() const { return myCompressionThreshold; }

    ut_NetPeerData		*findPeer(int peer);
protected:
    void			processTrackerMessage(UT_NetMessage *msg);
    /// Assembles a message to the tracker.
    void	 		sendTrackerMessage(const char *msg);

    void			startNetworkTimer();
    void			stopNetworkTimer();

    UT_NetMessagePump		myPump;
    int				myNPeer, myPeer;
    UT_NetSocket		*myToSelf;

    UT_StringHolder		myTrackerAddr;
    int				myTrackerPort;

    bool			myGotPeerList;
    bool			myError;
    bool			myErrorFromTracker;
    UT_Array<ut_NetPeerData>	myPeerList;

    UT_ValArray<UT_NetMessage *> myWaitingForPeers;
    UT_ValArray<UT_NetMessage *> myCompleted;

    UT_StringHolder			myJobName;

    /// We track our elapsed time for printing status
    UT_StopWatch		*myTimer;
    int				 myTimerDelay, myHeartbeat;
    fpreal64			 myLastTimerVal;

    /// Number of bytes to start compressing at.
    exint			 myCompressionThreshold;

    friend class AutoNetworkTimer;
};

class UT_API UT_NetMessagePipe
{
public:
		 UT_NetMessagePipe(const UT_StringHolder &trackeraddr, int trackerport,
			    int peer, int npeer, const UT_StringHolder &jobname);
    virtual	~UT_NetMessagePipe();

    class 	AutoNetworkTimer
    {
    public:
	AutoNetworkTimer(UT_NetMessagePipe *pipe) { myPipe = pipe; myPipe->startNetworkTimer(); }
	~AutoNetworkTimer() { myPipe->stopNetworkTimer(); }
	UT_NetMessagePipe		*myPipe;
    };


    /// Prepares the pipes, returns true if successful.
    bool	 openPipes(int timeoutms = 100);

    /// Shuts down the pipes, returns true if successful.
    bool	 closePipes(int timeoutms = 100);

    /// Run resetLength on the read pipes to setup the desired amount
    /// to read.
    /// Likewise, resetLength the write pipes & fill in the data to write.
    UT_NetMessage	*readPipe(int peer) { return myReadPipes(peer); }
    UT_NetMessage	*writePipe(int peer) { return myWritePipes(peer); }

    /// Flushes all write pipes, fills all read pipes.
    /// Afterwards the readPipes will have their data filled in. 
    /// Returns true if not errored or aborted.
    bool	 transferData(int timeoutms = 100);

    void	 setCompressionThreshold(exint threshold_bytes) { myCompressionThreshold = threshold_bytes; }
    exint	 compressionThreshold() const { return myCompressionThreshold; }

protected:
    /// Returns true until all the messages have been flushed.
    bool	 pumpData(int timeoutms);

    void			startNetworkTimer();
    void			stopNetworkTimer();

    UT_StringHolder			myTrackerAddress;
    UT_StringHolder			myJobName;
    int					myTrackerPort;
    int					myNPeer, myPeer;
    UT_NetSocket			*myToSelf;
    UT_NetMessagePump		 myPump;
    UT_ValArray<UT_NetMessage *>		myAnnounce;
    UT_ValArray<UT_NetMessage *>		myReadPipes;
    UT_ValArray<UT_NetMessage *>		myWritePipes;

    /// Number of bytes to start compressing at.
    exint			 myCompressionThreshold;

    friend class AutoNetworkTimer;
};

///
/// UT_NetBarrier
///
/// Very similar to UT_NetExchange, using the same tracker.  However,
/// it is meant for creating producer/consumer queues where the data
/// is transmitted OOB (ie, on shared disk resource)  The producer
/// can call setValue() to indicate how far it has completed.  The consumer
/// can invoke waitValue() to idle until the setValue is equal or
/// greater than the waitValue.
///
/// These calls will all block until success, error, or interrupt.
///
class UT_API UT_NetBarrier
{
public:
	         UT_NetBarrier(const UT_StringHolder &trackeraddr, int trackerport,
			  const UT_StringHolder &jobname);
    virtual	~UT_NetBarrier();

    /// Tells the tracker to update the barrier value to the given
    /// value.  Does not listen for a tracker response.
    void	 setValue(int val);

    /// Requests the tracker to alert us when the given value is
    /// reached on the barrier.
    void	 waitValue(int val);

    /// Returns current value of the barrier and then increments
    /// the barrier's value.
    /// Blocks until the tracker responds.
    int		 incrementValue(int defaultval);

protected:
    /// Creates and posts a message for the tracker.
    void	 sendTrackerMessage(const char *msg, int value);

    /// A simplified method for running the message pump until
    /// the system errors or is interrupted.
    void	 blockUntilComplete();

    /// Returns true until the barrier has been resolved or error
    /// occurs
    bool	 pumpData(int timeoutms);

    UT_NetMessagePump		 myPump;
    UT_NetSocket		*myToSelf;
    bool			 myError;

    int				 myBarrierValue;

    UT_StringHolder		 myTrackerAddr;
    int				 myTrackerPort;
    UT_StringHolder		 myJobName;
};


#endif

