/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NamedPipe.h ( UT Library, C++)
 *
 * COMMENTS:	This is a read pipe which gets around the problem of having a
 *		broken pipe if you close before reading all of the input from
 *		the pipe command.
 */

#ifndef __UT_NamedPipe__
#define __UT_NamedPipe__

#include "UT_API.h"
#include "UT_String.h"

#include <SYS/SYS_Handle.h>

#if defined(LINUX) || defined(MBSD)
    #include <stdio.h>
#endif

class UT_API UT_NamedPipe {
public:
     explicit UT_NamedPipe(SYS_Handle *filehandle);

     // If the pipe is open and destructed, the process will be killed.
    ~UT_NamedPipe();

    // Returns 1 if the pipe is open, 0 otherwise.
    int		 isOpen();
    
    // Create a named pipe of the given name. Returns 0 on error, 1 on success.
    // Created pipes are all write-only.
    int		 create(const char *filename);

    // Open a named pipe ofh the given name. Returns 0 on error, 1 on success.
    // Opened pipes are all read only.
    int		 open(const char *filename);

    // Close the pipe.
    void	 close();

    // Read up to the specified number of bytes. Returns the number of bytes
    // actually read. This call never blocks.
    int		 read(char *data, int len);

    // Prepare named pipe for writing. Call this before doing a block
    // of writes. Under NT it makes sure the pipe is connected. Does
    // nothing under SGI.
    void	 prepWrite();
    
    // Write the specified number of bytes. Returns the number of bytes
    // actually read. This call blocks until all bytes have been written.
    int		 write(const char *data, int len);

    // Flush input or output.
    void	 flush();
    
    // Returns 1 if there is data to be read, 0 if not, and less
    // than 0 if error.
    int		 dataAvailable();

private:

    SYS_Handle	*myFileHandle;

    int		 myCreated;
    UT_String	 myFileName;
    int		 myBytePending;
    char	 myNextByte;
#ifndef WIN32
    FILE	*myPipe;
#endif
};

#endif
