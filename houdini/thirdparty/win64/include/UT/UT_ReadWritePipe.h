/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ReadWritePipe.h (UT Library, C++)
 *
 * COMMENTS:	This class spawns a program and allows bidirectional
 *		communication between this process and the program by using
 *		two pipes.  The program only has to use stdin and stdout.
 *		This process must use the pipe files.
 */

#ifndef __UT_ReadWritePipe_h__
#define __UT_ReadWritePipe_h__

#include "UT_API.h"
#include "UT_Spawn.h"
#include <stdio.h>

class UT_API UT_ReadWritePipe {
public:
    // Note that the constructor does not open the pipe.  Instead, you must
    // call open().
    UT_ReadWritePipe();

     // If the pipe is still open when this object is destructed, the
     // child process will be killed.
    ~UT_ReadWritePipe();

    // If the command could not be run for any reason, open() will return
    // false.  The reason for the failure can be determined with getErrno().
    bool	 open(const char *cmd);
    int		 getErrno() const { return myErrno; }

    // If the pipe was already closed, close() will return false.  If the child
    // is still running, close() will kill the child with a SIGINT.
    bool	 close();

    // If close() succeeds, you can get the exit status of the child process
    // with this method.
    int		getChildExitStatus() const { return myChildExitStatus; }

    // Use these files to read from and write to the pipe.  Note that if you
    // try to write to the pipe when the child has exited, you will get a
    // SIGPIPE.  When you write, you should call fflush() to force the
    // pipe to be flushed.
    FILE	*getReadFile() { return myReadFile; }
    FILE	*getWriteFile() { return myWriteFile; }

    // This method will return the process id of the command that was executed.
    pid_t	getChildPid() const { return myChildPid; }

private:
    FILE	*myReadFile;
    FILE	*myWriteFile;

    pid_t	 myChildPid;
    int		 myChildExitStatus;
    int		 myErrno;

#ifdef WIN32
    void	*myReadPipe;
    void	*myWritePipe;
    void	*myHProcess;
#endif
};

#endif
