/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BitStream.h ( UT Library, C++)
 *
 * COMMENTS: This allows you to read and write from binary
 *	buffers on a per bit basis.  It can handle up to 32bit
 *	reads or writes.
 */

#ifndef __UT_BitStream__
#define __UT_BitStream__

#include "UT_API.h"

#include "UT_IntArray.h"

class UT_String;
class UT_WorkBuffer;

class UT_API UT_BitStream
{
public:
    UT_BitStream();
    UT_BitStream(int nbits);
    virtual ~UT_BitStream();

    // Delete & reset the buffer.
    void		 clear();

    int			 getLen() const;
    unsigned int	*getBuf() const { return myBuf; }
    void		 setBuf(unsigned int *buf, int len);

    int			 getBitLength() const	{ return myBitLength; }

    // Note that the write/read head is shared, so it is not proper
    // to try and write and read to the same buffer.
    // You can write to the buffer, do a reset, and then read, however.
    // Just be careful you don't try to read past the end or you'll assert.
    void		 write(int val, int bits);
    int			 read(int bits);	// Read, sign extended
    unsigned		 uread(int bits);	// Read, zero extended

    // Seek nbits into the stream.   When:
    //	whence[0] -- Seek from beginning of stream
    //	whence[1] -- Seek from current location
    //	whence[2] -- Seek from end of stream
    int			 seek(int nbits, int whence = 0);
    int			 tell() const;
    int			 rewind() { return seek(0, 0); }

    //
    // All the save methods take a number of "words" to process.  The number of
    // bits per word is defined by the method.  For example, saveToHex() has a
    // word-size of 4 bits.  This may be counter intuitive, but that's how it
    // works.
    // The offset parameter determines how many BITS into the stream to start
    // reading from when generating the string.
    //

    // Load from a hexidecimal string or generate a hex string
    // (word size = 4 bits)
    void		 loadFromHex(const char *hex);
    void		 saveToHex(UT_String &result, int nwords=-1,
						      int offset=0);
    void		 saveToHex(UT_WorkBuffer &result, int nwords=-1,
						      int offset=0);

    // Load or save a 6-bit encoded string
    // (word size = 6 bits)
    void		 loadFrom6Bit(const char *data);
    void		 saveTo6Bit(UT_String &result, int nwords=-1,
							int offset=0);
    void		 saveTo6Bit(UT_WorkBuffer &result, int nwords=-1,
							int offset=0);

    // Load or save a 8-bit encoded string.  Note that the binary data
    // may not be handled correctly since nulls can be inserted into the
    // UT_String (and are considered terminators).
    // (word size = 8 bits)
    void		 loadFromByte(const char *data);
    void		 saveToByte(UT_String &result, int nwords=-1,
							int offset=0);
    // Load or save a 8-bit encoded buffer. These are used for the binary
    // data so that it handles the nulls within the data.
    void		 loadFromByte(const char *data, int nwords);
    int			 saveToByte(unsigned char *data, int nwords, 
							int offset=0);

    // Initialize from a penta string.  If failed, returns non-zero
    // Also initializes the errors array to the start and end character
    // of each penta which failed.
    // A penta is 5 character sequense delimitted by non-alnum chars.
    // Each one stores 20 bits of data and has a 5 bits of checksum data.
    int 		 loadFromPenta(const char *penta, UT_IntArray &errors);
    // Write out the penta, only using the preferred characters.
    void		 saveToPenta(UT_String &result, int nwords=-1,
							int offset=0);

protected:
    // Load a single penta word.
    int			 loadPenta(const char *penta);
    int			 padTo(int nbits, int max, int off, int &savelen);
    void		 popPad(int savelen)
			 {
			     myBitLength = savelen;
			     seek(0, 2);	// Seek to end of file again
			 }
    
    // Local data.
    unsigned int	*myBuf;			// The buffer to read bits from
    int			 myHard;		// Whether we own the buffer.
    int			 myBitOff, myWordOff;	// The seek position
    int			 myBitLength;		// Total written length
    int			 mySize;		// Allocated size
};

#endif

