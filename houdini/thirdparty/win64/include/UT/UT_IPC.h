#ifndef __UT_IPC__
#define __UT_IPC__

#include "UT_API.h"
// This file contains the following class declarations:
//	UT_InetAddress
//	UT_UnixAddress
//	UT_Transport
//	UT_Socket
//	UT_UnixSocket
//	UT_InetSocket
//	UT_UnixStream
//	UT_InetStream
//	UT_InetDatagram

#include <sys/types.h>

#ifdef WIN32
    #include <windows.h>
    #include <winsock.h>
    struct sockaddr_un {
        short   sun_family;
        char    sun_path[108];
    };
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

#include <iosfwd>

class UT_API UT_IPCPacket {
public:
      UT_IPCPacket(char *msg, int len = 0);
     ~UT_IPCPacket();

     int	 len;		// Size of the packet
     char	*buf;		// Message data
};

typedef struct sockaddr		*SOCKADDR_PTR;
typedef struct iovec		*IOVEC_PTR;
typedef const struct sockaddr	*CONST_SOCKADDR_PTR;

//===========================================================================
class UT_API UT_InetAddress
{
    friend class	 UT_InetSocket;
    friend std::ostream	&operator<<(std::ostream &os, const UT_InetAddress &ia);

public:
    static const char	*UT_Any;		// any address
    static const char	*UT_Broadcast;	// broadcast address
    enum UT_Status {
	UT_NoHostGiven,
	UT_UnknownHost,
	UT_UnKnownService,
	UT_HostFound
    };

    UT_InetAddress();
    explicit UT_InetAddress(int port);
    UT_InetAddress(const char *service,  const char *protocol);
    UT_InetAddress(const char *hostname, int port);
    UT_InetAddress(const char *hostname, const char *service, const char *proto);

    const char *        hostname() const;
    const char *numeric_hostname() const;

    int		port()		const;
    int		ok()		const { return state==UT_HostFound; }
    UT_Status	status()	const { return state; }
    int		size()		const { return sizeof(address); }
    int		isany()		const { return any_flag; }
    int		isbroadcast()	const { return broadcast_flag; }

    operator       SOCKADDR_PTR()	{ return (SOCKADDR_PTR)&address; }
    operator CONST_SOCKADDR_PTR() const { return (CONST_SOCKADDR_PTR)&address; }

protected:

    int		lookup(const char *hostname, int port);
    int		lookup(const char *hostname,
		       const char *service,
		       const char *proto);

    int		lookup(int aport)    { return lookup(UT_Any,aport); }
    int		lookup(const char *service, const char *protocol)
				    { return lookup(UT_Any, service, protocol); }

    struct sockaddr_in	address;
    int			addrlen;
    UT_Status		state;
    int			any_flag;
    int			broadcast_flag;
};

//===========================================================================
class UT_API UT_UnixAddress
{
    friend class	 UT_UnixSocket;
    friend std::ostream	&operator<<(std::ostream &os, const UT_UnixAddress &ia);
public:
    static char		*Temp;	// any address

    UT_UnixAddress();		// Not sure what this one does.
    UT_UnixAddress(const char *pathname);

    const char *pathname() const;

    operator SOCKADDR_PTR()		{ return (SOCKADDR_PTR) &address; }
    operator CONST_SOCKADDR_PTR() const { return (CONST_SOCKADDR_PTR)&address;}

protected:
    void	set(const char *pathname);
    int		size() const { return sizeof(address); }

    struct sockaddr_un	address;
    int			addrlen;
};


//===========================================================================
class UT_API UT_Transport
{
public:
	     UT_Transport();
    virtual ~UT_Transport();

    virtual void	  abstract_class() = 0;
    virtual int		  close();

    virtual int		  read(      char *buffer, int len);
    virtual int		  write(const char *buffer, int len);
    virtual UT_IPCPacket *getPacket();	// Used for non-binary data

    virtual int		  readv(const IOVEC_PTR, int);
    virtual int		  writev(const IOVEC_PTR, int);

    virtual int		  ok();
    virtual int		  inuse();

    const char		*getError() const { return errorMsg; }
    int			 getDescriptor() const { return td; }

    int	    		 return_on_error;

    static void		 daemon_start(int ignore_sigcld);

protected:
    void		 setError(const char *module, const char *func,
                                  const char *mess);
    int		 	 readPartial(char *buffer, int len);
    int		 	 writePartial(char *buffer, int len);

    enum UT_ShutDownType { 
	UT_ShutDownReceive = 0, 
	UT_ShutDownSend = 1, 
	UT_ShutDownBoth = 2 
    };
    virtual void	 shutdown(UT_ShutDownType) = 0;

    int			 status;	// connection status
    int			 td;		// the transport file descriptor
    char		 errorMsg[256];
};

//===========================================================================
class UT_API UT_Socket : public UT_Transport
{
    void abstract_class();

public:
    enum UT_SocketSex {
	UT_Unknown,
	UT_Client,
	UT_Server
    };
    enum UT_AddressFamily {	// just work with these three for now
	UT_AfUnix    =  AF_UNIX,
	UT_AfInet    =  AF_INET
    };
    enum UT_SocketType {
	UT_Stream    =  SOCK_STREAM,
	UT_Datagram  =  SOCK_DGRAM,
	UT_Raw       =  SOCK_RAW,
	UT_RDM       =  SOCK_RDM,
	UT_SeqPacket =  SOCK_SEQPACKET
    };

    enum UT_MsgSendFlags {	
	UT_MsgSendNoFlags	=	0,
	UT_MsgSendDontRoute	=	MSG_DONTROUTE,
	UT_MsgSendOob		=	MSG_OOB,
	UT_MsgSendDontRouteOob	=	MSG_OOB|MSG_DONTROUTE
    };
    enum UT_MsgReceiveFlags {	
	UT_MsgReceiveNoFlags	=	0,
	UT_MsgReceivePeek	=	MSG_PEEK,
	UT_MsgReceiveOob	=	MSG_OOB,
	UT_MsgReceivePeekOob	=	MSG_OOB|MSG_PEEK
    };

    enum UT_FlagOption {
	UT_Off = 0,
	UT_On  = 1
    };
    enum UT_SocketLevelOption {
	UT_Broadcast	=	SO_BROADCAST,
	UT_Debug	=	SO_DEBUG,
	UT_DontRoute	=	SO_DONTROUTE,
	UT_KeepAlive	=	SO_KEEPALIVE,
	UT_OOBInline	=	SO_OOBINLINE,
	UT_ReuseAddr	=	SO_REUSEADDR,
#if !defined(LINUX) && !defined(MBSD)
	UT_UseLoopBack	=	SO_USELOOPBACK
#endif
    };
     UT_Socket(UT_AddressFamily af, UT_SocketType st);
    virtual ~UT_Socket();

    UT_Socket(UT_AddressFamily af);
    UT_Socket(UT_SocketType st);

    virtual int close();
    
    int		listen(int);
    int		recv    (char *, int, UT_MsgReceiveFlags = UT_MsgReceiveNoFlags);
    int		recvfrom(char *, int, struct sockaddr *, 
			 int &, UT_MsgReceiveFlags = UT_MsgReceiveNoFlags);

    int		send  (const char *, int, UT_MsgSendFlags = UT_MsgSendNoFlags);
    int		sendto(const char *, int, const struct sockaddr *, 
			int, UT_MsgSendFlags = UT_MsgSendNoFlags); 

    int		recvmsg(struct msghdr *, UT_MsgReceiveFlags = UT_MsgReceiveNoFlags);
    int		sendmsg(struct msghdr *, UT_MsgSendFlags = UT_MsgSendNoFlags);

    void	linger();
    int		setOption(UT_SocketLevelOption slo, UT_FlagOption fo);
    int		getOption(UT_SocketLevelOption slo, UT_FlagOption &fo);

    UT_AddressFamily address_family() 	{ return family;	}
    UT_SocketType    socket_type() 	{ return type;		}
    UT_SocketSex     socket_sex()	{ return sex;		}
    int		  socket_protocol()	{ return protocol;	}
    int		  isclient()		{ return sex==UT_Client;	}
    int		  isserver()		{ return sex==UT_Server;	}
    virtual int		socket();

protected:
    virtual void	shutdown(UT_ShutDownType);

    int		     accept(struct sockaddr *sa, int &len);
    int		       bind(const struct sockaddr *,int);
    int		    connect(const struct sockaddr *,int);
    int		getpeername(struct sockaddr *, int &len);
    int		getsockname(struct sockaddr *, int &len);
    int		getsockopt(int, int, char *, int *);
    int		setsockopt(int, int, const char *, int);	//

    int		check_syscall(int state, const char *mess);
    int		syscall(int state, const char *mess);

    UT_SocketType	type;		// Socket Type
    UT_AddressFamily	family;		// Address Family
    int			protocol;	// Protocol (currently 0)
    UT_SocketSex	sex;		// Client or Server?
};



//===========================================================================
class UT_API UT_UnixSocket : public UT_Socket
{
public:
     UT_UnixSocket(UT_SocketType st);
    ~UT_UnixSocket();

    int		open(const UT_UnixAddress &ua, UT_SocketSex sex);
    int		accept(UT_UnixSocket &newsocket);
    int		close();

    const UT_UnixAddress  &local_address() { return local; }
    const UT_UnixAddress &remote_address() { return remote; }

private:
    UT_UnixAddress	local;
    UT_UnixAddress	remote;
    int			unlink_socket;
};

//===========================================================================
class UT_API UT_InetSocket : public UT_Socket
{
public:		
    UT_InetSocket();
    UT_InetSocket(UT_SocketType st);
    ~UT_InetSocket();

    int		accept(UT_InetSocket &newsocket);
    int		bind(const UT_InetAddress &ia);
    int		connect(const UT_InetAddress &ia);
    int		getpeername(UT_InetAddress &ia);
    int		getsockname(UT_InetAddress &ia);

    int		recvfrom(char *buffer, int max,       UT_InetAddress &is);
    int		  sendto(char *buffer, int len, const UT_InetAddress &is);
};

//===========================================================================
class UT_API UT_UnixStream : public UT_UnixSocket
{
public:
    UT_UnixStream() : UT_UnixSocket(UT_Stream) {};
    ~UT_UnixStream();

    int		open(const char *path, UT_SocketSex asex)
			{ return UT_UnixSocket::open(path,asex); }
    int		accept(UT_UnixStream &newstream)
			{ return UT_UnixSocket::accept(newstream); }
    int		close(); 
};

//===========================================================================
class UT_API UT_InetStream :public UT_InetSocket
{
public:						
    UT_InetStream();
    ~UT_InetStream();

    int		accept(UT_InetStream &newstream)
			{ return UT_InetSocket::accept(newstream); }

    int		socketServer(const UT_InetAddress &ia, int minPort = -1,
						       int maxPort = -1 );
    int		socketClient(const UT_InetAddress &ia, int minPort = -1,
						       int maxPort = -1	);
    void	setNoDelay(int on_or_off);
};


//===========================================================================
class UT_API UT_InetDatagram :public UT_InetSocket
{
public:						
    UT_InetDatagram();
    ~UT_InetDatagram();

    int		accept(UT_InetDatagram &newstream);

    int		socketClient();
    int		socketClient(const UT_InetAddress &ia);
    int		socketServer(const UT_InetAddress &ia);

    virtual int	socket(); 
};

#endif /* __UT_IPC__ */
