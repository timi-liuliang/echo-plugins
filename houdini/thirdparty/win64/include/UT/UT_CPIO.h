/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	CPIO packetting for I/O
 *
 * USAGE:
 *	To read from a CPIO archive:
 *
 *      Memory intensive but safe methods
 *
 *	myclass::readObject(istream &filestream, int binary)
 *	{
 *	    UT_CPIO		 packet;
 *	    UT_WorkBuffer	 type;
 *	    istream		*is;
 *	    
 *	    if (packet.open(filestream, type) == UT_CPIO_OK)
 *	    {
 *		obj = myclass->addObject(type.buffer());
 *		is = packet.read();		// Read in the contents
 *		obj->readData(*is, binary);
 *		packet.close(filestream);	// Correct position
 *	    }
 *	    else flagError();
 *	}
 *
 *	myclass::writeObject(object *obj, ostream &filestream, int binary)
 *	{
 *	    UT_CPIO	 packet;
 *	    ostream	*os;
 *
 *	    if (os = packet.open())	// Open a CPIO packet for writing
 *	    {
 *		obj->saveData(*os, binary);
 *		// Now, flush the contents
 *		if (packet.close(filestream, obj->getType()) != UT_CPIO_OK)
 *		    flagError();
 *		// If we're at the end, flag the EOF
 *		if (lastObject)
 *		    packet.writeTrailer(filestream);
 *	    }
 *	}
 *
 *      Raw methods (not using the memory intensive mechanisms)
 *	
 *	myclass::readCommands(istream &filestream)
 *	{
 *	    UT_CPIO		packet;
 *	    UT_WorkBuffer	pathname;
 *	    char		buffer[SIZE];
 *
 *	    if (packet.open(filestream, pathname) == UT_CPIO_OK)
 *	    {
 *		if (pathname == "commands")	// Read commands
 *		{
 *		    long	fileSize = packet.getFileSize();
 *
 *		    while (fileSize > 0)
 *		    {
 *			filestream.getline(buffer, SIZE);
 *			fileSize -= strlen(buffer);
 *			execute(buffer);
 *		    }
 *		    // Now, correct my position in case of overrun
 *		    packet.close(filestream);
 *		}
 *	    }
 *	}
 *
 *	myclass::writeStuff(ostream &filestream)
 *	{
 *	    UT_CPIO	packet;
 *
 *	    if (packet.open(filestream, "commands") == UT_CPIO_OK)
 *	    {
 *		for (i = 0; i < ncommands; i++)
 *		    os << command[i] << endl;
 *		packet.close(filestream);
 *	    }
 *	    if (last) packet.writeTrailer(filestream);
 *	}
 *
 *
 * NOTE: Because some UT_CPIO methods seek back when writing the stream,
 *	 this class should be used with UT_OStringRefStream class, which
 *	 may discard previoulsy written data when auto-terminating the string.
 *	 Please, use UT_OStringStream instead, use its string length,
 *	 and watch out for null characters in the middle of the CPIO stream.
 */

#ifndef __UT_CPIO_h__
#define __UT_CPIO_h__

#include "UT_API.h"
#include "UT_String.h"
#include "UT_UniquePtr.h"
#include "UT_WorkBuffer.h"

class UT_IStream;
class UT_OStringStream;

enum {
    UT_CPIO_OK,
    UT_CPIO_EOF_OK,		// End of file reached
    UT_CPIO_BINARY,		// Binary CPIO format not supported yet
    UT_CPIO_BAD_EOF,		// Unexpected EOF
    UT_CPIO_BAD_HEADER,		// Bad header
    UT_CPIO_BAD_CONTENTS,	// Bad contents in CPIO packet
    UT_CPIO_BAD_STREAM,		// Bad stream (i.e. bad calling sequence)
    UT_CPIO_USER_ABORT		// User cancelled reading the stream
};

class UT_API UT_CPIO {
public:
	     UT_CPIO();
    virtual ~UT_CPIO();

    //  This open will read a CPIO header from the stream.
    virtual int	open(UT_IStream &is, UT_WorkBuffer &pathname);
    int		open(UT_IStream &is, UT_String &pathname)
    {
	UT_WorkBuffer	pathname_buffer;
	pathname_buffer.append(pathname);
	int result = open(is, pathname_buffer);
	pathname_buffer.copyIntoString(pathname);
	return result;
    }

    // This opens after we have verified it is a true CPIO file.
    int	 openPostCheckNormal(UT_IStream &is, UT_WorkBuffer &pathname);
    int	 openPostCheckExtended(UT_IStream &is, UT_WorkBuffer &pathname);

    //  A safe (but memory intensive) mechanism for reading from the file
    virtual UT_IStream	*read(UT_IStream &is);

    //  Method to skip over the packet that's currently open
    virtual int		 skip(UT_IStream &is);

    //  After reading or skipping, closing will move the stream pointer
    //	to the correct position.  This is not a manditory method.  In fact,
    //	read packets should not be closed if the stream is cin.
    virtual int		 close(UT_IStream &is);

    //  Memory intensive method to write a packet, but it works if you're
    //	sending data to cout or cerr streams
    virtual std::ostream *open();
    virtual int		 close(std::ostream &os, const char *path, int bin=0, exint modtime = -1);

    //  Non-memory intensive method to write a packet, it does not work when
    //	writing to cout or cerr streams
    virtual int		 open(std::ostream &os, const char *path, int bin=0, exint modtime = -1);
    virtual int		 close(std::ostream &os);

    //  When all packets have been written, please write a trailer
    virtual int		 writeTrailer(std::ostream &os, exint modtime = -1);

    exint		 getFileSize() const	{ return myFileSize; }
    exint		 getModTime() const	{ return myModTime; }

protected:
    UT_WorkBuffer	 myCurrentFile;
    UT_UniquePtr<UT_IStream> myIs;
    UT_UniquePtr<UT_OStringStream> myOs;
    exint		 myModTime;
    exint		 myFileSize;	  // For non-memory intensive read
    exint		 myHeaderStart;	  // For non-memory intensive write
    exint		 myHeaderEnd;	  // End of header
    bool		 myPacketTainted; // Is this packet being read tainted?
};

#endif
