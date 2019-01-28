/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ReadPipe.h ( UT Library, C++)
 *
 * COMMENTS:	This is a read pipe which gets around the problem of having a
 *		broken pipe if you close before reading all of the input from
 *		the pipe command.
 */

#ifndef __UT_ReadPipe__
#define __UT_ReadPipe__

#include "UT_API.h"
#include <sys/types.h>
#include <stdio.h>
#include "UT_SysClone.h"

class UT_API UT_ReadPipe {
public:
     explicit UT_ReadPipe(const char *cmd = 0);

     // If the pipe is open and destructed, the process will be killed.
    ~UT_ReadPipe();

    FILE	*open(const char *cmd);

    int		 close();
    FILE	*getFilePtr()		{ return myFilePtr; }

    // Get the error for the pipe
    int		 getErrno() const	{ return myErrno; }

    // If close() succeeds, you can get the exit status of the child process by
    // calling the following method.
    int		getStatus() const	{ return myExitStatus; }

    // This method will return the child process id.
    pid_t	getPid() const		{ return myPid; }

private:
    FILE	*myFilePtr;
    int		 myErrno;
    int		 myExitStatus;
    pid_t	 myPid;
#ifdef WIN32
    void	*myPipe;
    void	*myHProcess;
#endif
};

#endif

