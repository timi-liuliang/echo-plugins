/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetSocket.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Simple Network socket definition. Non-Blocking by default.
 *
 */

#ifndef __UT_NetSocket__
#define __UT_NetSocket__

#include "UT_API.h"
#include <SYS/SYS_Time.h>

// For fd_set, you will need to #include <SYS/SYS_Socket.h>
#ifdef WIN32
struct fd_set;
#endif

class UT_String;
class UT_WorkBuffer;


class UT_API UT_NetSocket
{
public:
    enum
    {
	UT_CONNECT_SUCCESS	=  0,
	
	UT_WOULD_BLOCK		= -1,
	UT_BAD_ADDRESS		= -2,
	UT_CONNECT_FAILED	= -3,
	UT_ERROR_OCCURED	= -4,
	UT_WRONG_SOCKET		= -5,
	UT_NO_CONNECTION	= -6
    };

    enum
    {
	UT_SHUTDOWN_RECEIVE	= 0,
	UT_SHUTDOWN_SEND	= 1,
	UT_SHUTDOWN_BOTH	= 2
    };

    /// Convertes the enum of UT_CONNECT_SUCCESS, etc, into an english
    /// error message roughly matching the enum's name.  Be careful
    /// as this uses different errors different locations.
    static const char *getErrorName(int code);

    static int	getPortByService(const char *service, const char *proto="tcp",
				    int default_port = 0);

    static void getHostName(char *name, int max);
    
    // Get the host address of the form xxx.xxx.xxx.xxx.  Returns 1 on success
    static int	getHostAddress(unsigned char address[4],
				const char *hostname = 0);

    // Obtains the actual machine name of the host specified by 
    // alias name. Return true on success. If alias = 0, obtains local machine.
    static bool	getHostNameByAlias(UT_String &host, const char *alias = NULL);

    // Try to map a port number into the range of unprivileged port numbers.
    static int			 mapToUnprivilegedPort(int port);

    // This convenience method can be called from external programs to send
    // a command on a socket port and wait for a response.  If the host name
    // is null, the local host will be used.  If remap_privileged_ports is
    // true, the given port might be remapped to increase the likelihood
    // of it being opened.  False will be returned if the socket could not be
    // opened.
    static bool			 sendCommandAndGetResult(int port,
					const char *command,
					UT_WorkBuffer &response,
					const char *host_name = 0,
					bool remap_privileged_ports = true);

    static bool			 nonBlockingSendCommandAndGetResult(int port,
					const char *command,
					UT_WorkBuffer &response,
					const char *host_name = 0,
					bool remap_privileged_ports = true);

    /// Creates a new listen socket on the specified port.  A port of 0
    /// will auto-choose a free port.  Be careful overusing that, however,
    /// as Windows in particular has very few ports available by default.
    /// portisonlyhint will attempt to bind to the given port but if
    /// it fails (ie, already in use) will revert to using 0 to select
    /// a free port.  The caller should thus double check the actual
    /// bound port if they care.
    static UT_NetSocket 	*newSocket(int port, bool blocking = false,
					    bool portisonlyhint = false);
    static UT_NetSocket		*newSocketFromAddr(
					const char *address,
					int port,
				        bool blocking = false,
				        int localport = -1,
				        int forcesocket=0);
    static void			 fdZero(fd_set *set);
    static void			 fdSet(int fd, fd_set *set);
    static void			 fdClr(int fd, fd_set *set);
    static int			 fdIsSet(int fd, fd_set *set);

    /// Performs a select() call on the socket.
    ///
    ///
    /// Note, it turns out there is a discrepancy between how select() works on
    /// Linux vs. Windows.  
    /// 
    /// On Windows, the select() call selects for reading the sockets even if  
    /// they were closed by the remote counterparts. The rationale is that  
    /// trying to read from such a socket returns zero, which is a signal that  
    /// the connection was closed.
    ///
    /// On Linux/OSX, select() call does not select such sockets for reading.  
    /// The rationale is that there cannot be any more data for reading on  
    /// closed sockets.
    ///
    ///
    /// WARNING: the following select will not work on UT_PipeSocket instance
    ///     for write and error sets because there is no system call that probes
    ///     these states. UT_PipeSocket derives from UT_NetSocket, so be careful
    ///
    ///     Use it only on sockets!
    static int			 select(int fd, fd_set *r_set, fd_set *w_set,
					fd_set *e_set, SYS_TimeVal *tv,
					bool forcesocket=false);
    // Simplified select which takes a timeout in milliseconds
    static int			 selectInMS(int maxfd, 
					fd_set *r_set,
					fd_set *w_set,
					fd_set *e_set,
					int timeoutms = 0,
					bool forcesocket = false);

    bool			 isSocketSelected(fd_set &set);
    // Adds this socket to the given fd set.
    void			 addToFDSet(fd_set *set, int &maxfd);

    // Destructor. Closes connection if open.
    virtual ~UT_NetSocket();

    virtual int64		 getMemoryUsage(bool inclusive) const;

    // *******************************************************
    // Connection routines
    
    // Determines if any connection requests are pending, and creates a
    // new socket for that connection (SERVER SOCKET ONLY).
    virtual UT_NetSocket *accept(int blocking, int &condition);

    // Connects to the server named in the constructor (CLIENT SOCKET ONLY)
    // If timeout is greater than zero, the connection time is going to be
    // no longer than the amount specified. If connection succeeds within
    // the time specified, UT_CONNECT_SUCCESS is returned. If timeout is
    // zero, a blocking connectoin is carried out, and if fails UT_WOULD_BLOCK
    // or UT_CONNECT_FAILED is returned.
    virtual int		connect(int timeout_ms = 0);

    // closes the socket (client or server).
    virtual int		close();

    virtual int		shutdown(int type);

    // *******************************************************
    // I/O Routines (client or server)
    
    // writes len bytes to the socket.
    virtual int		write(const void *data,int len,int *numWritten = 0);

    // peeks to see if any data is available.  Timeout is specified in ms.
    // Returns 1 if there is data to be read, 0 if not, and less
    // than 0 if error.
    virtual int		dataAvailable(int timeout=0);

    // Attempts to read len bytes. Actual number of read bytes is 
    // returned in numRead. 
    // If read ever succeeds, UT_CONNECT_SUCCESS is returned.
    // If any error ever occurs during a read, UT_ERROR_OCCURED is returned.
    // If specified timeout_ms is non-negative then read will block 
    //   for at most that many miliseconds, and then it will return
    //   UT_WOULD_BLOCK if still no data has been read.
    //   If UT_WOULD_BLOCK is returned, numRead may be non-zero to
    //   show that some data was read prior to the block.
    // If specified timeout_ms is less than zero, a blocking socket will
    //   block (wait) till data is available, and a non blocking socket
    //   will return right away with UT_WOULD_BLOCK code.
    virtual int		read(void *data, int len, int *numRead = 0, 
			     int timeout_ms = -1);

    // Read all available data into a work buffer.
    virtual int		read(UT_WorkBuffer &data, int timeout_ms = -1,
			     bool wait_for_null_terminator = false);
    
    // Reads len bytes & doesn't remove them from the queue
    virtual int		peek(void *data, int len, int timeout_ms = -1);

    // Flushes the read or write queue.
    virtual int		flushRead();
    virtual int		flushWrite();


    void		terminateOnConnectionLost(bool term = true);
    
    // ********************************************************
    // Miscellaneous routines.

    // returns the address name and port of the host, or the connected
    // host (in the case of a UT_NetSocket returned by accept().)
    
    const char *getAddress() const
		{ return myAddressName ? myAddressName:""; }

    int		getPort() const
    		{ return myAddressPort; }

    // Returns information about the other end of the socket. "host" buffer
    // must be at least 20 characters longs.
    virtual int		getRemoteSocket(char *host, int &port) const;

    // Sets the socket to blocking or non blocking
    virtual int 	setBlocking(bool blocking);
    bool		isBlocking() const	{ return myIsBlocking; }

    void		setNoDelay(int usenodelay);
    
    // true if the socket is valid.
    virtual int		isValid();

    // true if the socket is connected.
    int			isConnected();

    // returns the file descriptor for the socket... beware!
    int			getSocket() const;

    uint32		getRemoteIP4() const	{ return myRemoteIP4; }
    bool		getRemoteIP4(int ip[4]);
    uint32		getLocalIP4() const;
    bool		getLocalIP4(int ip[4]);
    
protected:
    // Because virtuals shouldn't be called in destructors, we have a
    // non-virtual close method.
    int			closeInetSocket();

    // The actual net-socket constructors
    UT_NetSocket(UT_NetSocket *netsocket, int socket, bool blocking);
    UT_NetSocket(int port, bool blocking = false, bool portisonlyhint=false);
    UT_NetSocket(const char *address, int port, bool blocking = false,
		 int localport = -1);

    // Default constructor used by shmsocket stuff
    UT_NetSocket();		// Default

    // Helper method for waiting for data on the socket or a timeout.
    // It is really a wrapper for data available with extra error checks
    // Returns UT_NO_CONNECTION, UT_ERROR_OCCURED if error, 
    // UT_WOULD_BLOCK if timeout expired, or UT_CONNECTION_SUCCESS if
    // specified timeout <= 0 or if data is available before timeout expired.
    int			waitForDataOrTimeout( int timeout_ms );

private:
    // helper method that implements a timed-out connection for sockets
    // (especially, NOT FOR PIPES ON NT!)
    // Returns the errno of the socket (eg, 0 if OK, ETIMEDOUT if timeout, ...)
    int			connectOrTimeout(struct sockaddr_in *address, 
					 int address_length, int timeout_ms);

    // platform dependent code for API
    // Returns 0 if OK, or errno if failed
    int			doConnect(struct sockaddr_in *address,
				  int address_length, bool check_err);

    // probes the activity of this socket for read, write, error readines
    // (whichever is not NULL) and sets the given argument appropriately.
    // Returns 0 if timeout, 1 if OK, negative if error.
    // NB: this call is used by UT_NetPacket::connect() during the connection
    //     process. It is applicable only to UT_NetSocket, and is not
    //     to UT_PipeSocket which derives from UT_NetSocket. The reason is
    //     that pipe cannot test fd_write or fd_error, which may be essential
    //     during connection process implemented by this class. UT_PipeSocket
    //     has own connect() method that does not use checkConnectStatus()
    int			checkDataStatus(bool *fd_read, bool *fd_write, 
					bool *fd_error, int timeout_ms);

    // abstracted host by alias part that is common to Linux and NT
    // On Linux this method must be surrounded by locks.
    static bool		commonGetHostNameByAlias( UT_String &host, 
						  const char *alias );

protected:

    char	*myAddressName;
    int		 myAddressPort;
    int		 mySocket;
    uint32	 myRemoteIP4;		// Remote IP address

    //flags
    unsigned char 	myIsServer	:1,
			myConnected	:1,
			myShmFlag	:1,	// Am I a shared memory socket?
			myTermOnLost    :1,	// exit if connection broken.
			myIsBlocking	:1;
};

class UT_API UT_AutoSocketDeleter
{
public:
    UT_AutoSocketDeleter(UT_NetSocket *socket) : mySocket(socket) {}
    ~UT_AutoSocketDeleter() { delete mySocket; }
private:
    UT_NetSocket *mySocket;
};

#endif
