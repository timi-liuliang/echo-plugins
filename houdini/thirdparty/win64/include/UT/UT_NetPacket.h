/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetPacket.h ( UT Library, C++)
 *
 * COMMENTS:	This class can be used to read/write packetized data over a
 *		UT_NetSocket.  It's really just a convenience class.  The
 *		read/write methods are a little "safer" than using vanilla
 *		sockets.  There are methods for setting the sockets up, data
 *		compression is supported, etc.
 *
 * CAVEATS:	- It is not possible to write a packet which has no data
 *		- The user is responsible for putting the packet data into
 *		  network byte order.
 */

#ifndef __UT_NetPacket__
#define __UT_NetPacket__

#include "UT_API.h"
#include <string.h>
#include <SYS/SYS_Types.h>

class UT_NetSocket;
class UT_WorkBuffer;

class UT_API UT_NetPacket {
public:
    // By default, UT_NetPacket uses buffered writes and unbuffered reads.
    // To change this behavior, adjust the bufsize parameters passed to
    // the constructor.  If you use a non-zero read buffer size, make sure
    // that your algorithm only uses a single UT_NetPacket for the socket
    // being read.
    explicit UT_NetPacket(unsigned write_bufsize=4096,
                          unsigned read_bufsize=0);
    ~UT_NetPacket();

    int64 getMemoryUsage(bool inclusive) const;

    // The acceptClient convenience function can be used by a server to look
    // for a client.  If there's no client, the server will wait for a few ms.
    // before returning.
    //	- The server should be created using:   UT_NetSocket(server_port)
    //	- If no client connects, the server socket should not be re-created
    // For example:
    //	    while (1) {
    //		UT_NetSocket	server(port, 0);
    //		if (!server.isValid()) error("Invalid server port");
    //		do {
    //		     extraProcessing();
    //		     client = UT_NetPacket::acceptClient(server, 1);
    //		} while (!client);
    // Timeout is specified in ms.
    //
    static UT_NetSocket		*acceptClient(UT_NetSocket &server,
					      int blocking = 1,
					      int timeout=10);

    // Convenience method to get "host" string.
    static const char		*getHost();

    // Convenience method to get "user@host" string.
    static const char		*getUserHost();

    // This will return 1 if the IP address matches anything in the mask.
    static int			 matchIPMask(const char *mask,
					     const char *address,
					     UT_NetSocket *socket);

    // Returns true if server ip matches the client's under given mask pattern.
    static bool			 matchServerClientIPs(
					const unsigned short server[4], 
					const unsigned short client[4],
					const char *mask_pattern);

    // The connectToServer convenience function can be used to connect to a
    // server on a remote host.  If no connection is made within the specified
    // timeout (in seconds), then a null will be returned.
    // If no remote host is specified, then we will connect to the localhost.
    // This will construct a PipeSocket if NOSOCKETS is set, unless the
    // force socket is chosen.
    static UT_NetSocket		*connectToServer(const char *host = 0,
						 int port = -1,
						 int blocking=1,
						 int timeout=5,
						 int forcesocket=0);

    static int			 getDefaultServerPort();

    // Disconnect and delete the socket.
    static void			 disconnectFromServer(UT_NetSocket *socket);

    enum {
	PacketCompress	= 0x01,
	PacketCrypt	= 0x02		// Currently not enabled
    };

    // Caution:  If compression is enabled, writing data may grow the internal
    //		 packet buffer
    // This returns the length of the buffer written (compressed length if
    //	turned on).
    int		 writeData(UT_NetSocket &sock, const void *data, unsigned len);
    int		 writeData(UT_NetSocket &sock, const char *data)
		 {
		     return writeData(sock, data, strlen(data)+1);
		 }

    // If the flush fails, then 0 is returned
    int		 flush(UT_NetSocket &sock);

    // Read the uncompressed size of the next packet (0 indicates error or 
    // timeout). 
    // If timeout_ms is NULL or *timout_ms < 0, a blocking socket
    //   will block until data is available; a nonblocking will either read
    //   available data or not and will return immediatelly. 
    // If *timeout_ms >= 0, both blocking and nonblocking sockets will wait for
    //   the data until timeout in miliseconds elapses or data shows up on the
    //   socket. The timeout_ms is updated to reflect the remaining time
    //   the call would block untill the timeout would expire.
    int		 readPacketLength(UT_NetSocket &sock, int *timeout_ms = NULL);
    int		 readPacketLength(UT_NetSocket &sock, int *timeout_ms,
				  int &status);

    // Read the next packets data. See above for timeout description.
    int		 readPacketData(UT_NetSocket &sock, void *buf, unsigned len,
				int *timeout_ms = NULL);
    int		 readPacketData(UT_NetSocket &sock, UT_WorkBuffer &buf,
				unsigned int len, int *timeout_ms = NULL);
    int		 readPacketData(UT_NetSocket &sock, void *buf, unsigned len,
				int *timeout_ms, int &status);
    int		 readPacketData(UT_NetSocket &sock, UT_WorkBuffer &buf,
				unsigned int len, int *timeout_ms,
				int &status);

    // A convenience method to use my internal buffer size. See above for
    // timeout description.
    void	*readPacket(UT_NetSocket &sock, unsigned &len, 
			    int timeout_ms, int &status)
		 {
		     void	*rcode = 0;
		     len = readPacketLength(sock, &timeout_ms, status);
		     if (len > 0)
		     {
			 growBuffer(len);
			 if (readPacketData(sock, myData, len, &timeout_ms,
					    status))
			     rcode = myData;
		     }
		     return rcode;
		 }
    void	*readPacket(UT_NetSocket &sock, unsigned &len, 
			    int timeout_ms = -1)
		 {
		     int status;
		     return readPacket(sock, len, timeout_ms, status);
		 }

    // Read a packet into the buffer pointed to by data.  The size of the
    // buffer is specified.  If the packet is too big for the buffer or if
    // there is an error reading the packet, 0 is returned. See above
    // for timeout description.
    unsigned	 readPacket(UT_NetSocket &sock, void *data, unsigned size,
			    int timeout_ms, int &status)
		 {
		     unsigned		len;
		     len = readPacketLength(sock, &timeout_ms, status);
		     if (len > 0 && len <= size)
		     {
			 if (!readPacketData(sock, data, len, &timeout_ms,
					     status))
			     len = 0;
		     }
		     else len = 0;
		     return len;
		 }
    unsigned	 readPacket(UT_NetSocket &sock, void *data, unsigned size,
			    int timeout_ms = -1)
		 {
		     int status;
		     return readPacket(sock, data, size, timeout_ms, status);
		 }

		 // Reading a packet into a workbuffer assumes that the packet
		 // data is a string.  That is, no null characters are
		 // contained within the packet data.
    unsigned	 readPacket(UT_NetSocket &sock, UT_WorkBuffer &wbuf,
			    int timeout_ms = -1);
    unsigned	 readPacket(UT_NetSocket &sock, UT_WorkBuffer &wbuf,
			    int timeout_ms, int &status);

    void	 setFlag(unsigned flag)		{ myFlag |= flag; }
    void	 clearFlag(unsigned flag)	{ myFlag &= ~flag; }
    int		 getFlag(unsigned flag)	const	{ return myFlag & flag; }

    unsigned	 getDataSize() const		{ return myDataSize; }
    void	 limitDataSize(unsigned maxsize=4096)
		 {
		     if (maxsize > myDataSize)
			 growBuffer(maxsize, true);
		 }
    void	 *steal()
		  {
		      void	*rcode = myData;
		      myData = 0;
		      myDataSize = 0;
		      return rcode;
		  }

private:
    // Ensure the buffer is big enough
    void	 growBuffer(unsigned len, bool doshrink=false);

    int		 bufferWrite(UT_NetSocket &sock, const void *data, unsigned l);
    int		 bufferRead(UT_NetSocket &sock, void *data, unsigned l,
			    int *timeout_ms, int &status);

    void	*myData;
    char	*myWriteBuf, *myWriteCurr, *myWriteEnd;
    char	*myReadBuf, *myReadCurr, *myReadLast, *myReadEnd;
    unsigned	 myDataSize;
    unsigned	 myFlag;
};

#endif

