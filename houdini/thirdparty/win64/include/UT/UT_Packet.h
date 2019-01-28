/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Packet.h (Utility Libray, C++)
 *
 * COMMENTS:
 *		This class is used to save and load binary packets of
 *		byte streams. It is assumed that the stream is seekable.
 *
 */

#ifndef __UT_Packet__
#define __UT_Packet__

#include "UT_API.h"
#include "UT_NTStreamUtil.h"
#include "UT_OStream.h"

// streampos is 64 bits on alpha, and 32 bits on the other machines,
// so let's use a 32 bit value
typedef int UT_StreamPos;
class UT_IStream;
class UT_PacketTaint;
class UT_OStringStream;

UT_API short		UTgetClassPacketID(const char *classname);

class UT_API UT_Packet
{
public:
		 UT_Packet();
		~UT_Packet();

    int		 beginWrite(std::ostream &str, short class_id, short signature);
    int		 beginWrite(UT_OStream &str, short class_id, short signature);
    int		 endWrite();

    bool	 beginRead(UT_IStream &stream, short &class_id,
			    short &signature);
    int		 cancelRead();
    int		 endRead();

    // The following methods can be used if the stream is a pipe (or
    // stdin/stdout).
    std::ostream*beginPipeWrite(std::ostream &str, short class_id, short signature);
    int		 endPipeWrite();

    UT_IStream	*beginPipeRead(UT_IStream &stream, short &class_id, short &sig);
    int		 endPipeRead();

    void	 setHeaderTaint(UT_PacketTaint *taint);

    // compressData will do zlib compression on the source data.  The data
    // should be "smallish" in that a buffer is allocated off the stack which
    // is roughly 1.1 times the size passed in.  There's an internal limit of
    // 8Mb on data to be stored.
    // A malloc'ed buffer is returned, and size is set to the compressed size
    // The packet has a packet header which contains additional information
    // used in decompression.
    static char	*compressPacket(const char *src, int &size);

    // decompressData will do zlib uncompression on the source data.  The
    // compressed data must have been created with UTcompressPacket.
    // A malloc'ed buffer is returned, and size is set to the un-compressed size
    static char	*uncompressPacket(const char *src, int &size);

    // getHeaderSize() returns the number of bytes used for the packet header.
    static unsigned int	 getHeaderSize();


private:
    char		 myState;
    UT_StreamPos	 myPacketSize;
    UT_StreamPos	 myPacketStart;
    UT_OStringStream	*myOutBuffer;
    UT_IStream		*mySBuffer;	// UT IStreams
    UT_IStream		*mySStreamPtr;
    UT_OStream		 myOutStreamRef;
    char		*myData;
    UT_PacketTaint	*myHeaderTaint;
};

// use this function to get the class packet-id for your class
// just include the line: SETUPCLASSPACKETID("LIB_CLASS") or
// whatever in your .C file and also add "LIB_CLASS" to the table
// at the top of UT_Packet.C

#define SETUPCLASSPACKETID(classname) \
static int \
getClassPacketID() \
{ \
    static int classid = UTgetClassPacketID(classname); \
    return classid; \
}

#endif
