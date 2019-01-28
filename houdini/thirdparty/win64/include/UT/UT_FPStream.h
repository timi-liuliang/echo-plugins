/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FPStream.h ( UT Library, C++)
 *
 * COMMENTS:	Stream using FILE * objects instead of file descriptors
 *
 *	In general, use this class instead of UT_FDStream if possible.  This is
 *	because this class is buffered while the FDStreams are not (and thus
 *	have very poor performance).
 */

#ifndef __UT_FPStream__
#define __UT_FPStream__

#include "UT_API.h"
#include "UT_Assert.h"
#include <SYS/SYS_Stdio.h>

#include <istream>
#include <ostream>
#include <stdio.h>
#include <streambuf>
#include <string.h>


/************************************************************
 * UT_OFPStream
 * - a stream that writes on a file descriptor
 ************************************************************/

class UT_API UT_FPOutBuf : public std::streambuf
{
public:
    UT_FPOutBuf (FILE *_fp)
	   : myFP(_fp)
    {
    }
    void close()
    {
	UT_ASSERT(myFP);
	::fclose(myFP);
	myFP = 0;
    }

protected:
    // write one character
    virtual int_type	overflow(int_type c) 
			{
			    UT_ASSERT(myFP);
			    return putc(c, myFP);
			}

    // In order to allow tellg to work, we need to provide seekoff.
    virtual pos_type	seekoff(off_type _Off,
			    std::ios_base::seekdir _Way,
			    std::ios_base::openmode /*_Mode*/ = std::ios::out)
			{
			    int		whence = SEEK_CUR;
			    UT_ASSERT(myFP);
			    switch (_Way)
			    {
				case std::ios_base::beg:
				    whence = SEEK_SET;
				    break;
				case std::ios_base::end:
				    whence = SEEK_END;
				    break;
				case std::ios_base::cur:
				default:
				    whence = SEEK_CUR;
				    break;
			    }
			    if (SYSfseek(myFP, _Off, whence) < 0)
				return -1;
			    return (pos_type)SYSftell(myFP);
			}
    virtual pos_type	seekpos(pos_type _Off,
			    std::ios_base::openmode /*_Mode*/ = std::ios::out)
			{
			    if (SYSfseek(myFP, _Off, SEEK_SET) < 0)
				return -1;
			    return (pos_type)SYSftell(myFP);
			}

    // write multiple characters
    virtual std::streamsize	xsputn (const char* s, std::streamsize num) 
			{
			    UT_ASSERT(myFP);
			    return fwrite(s, 1, num, myFP);
			}

protected:
    FILE	*myFP;		// My file pointer
};

class UT_API UT_OFPStream : public std::ostream
{
  protected:
    UT_FPOutBuf buf;
  public:
    UT_OFPStream (FILE *fp) : std::ostream(0), buf(fp) 
    {
        rdbuf(&buf);
    }

    // Streams cannot be moved
    UT_OFPStream(UT_OFPStream &&) = delete;

    void close()		{ buf.close(); }
};

#endif

