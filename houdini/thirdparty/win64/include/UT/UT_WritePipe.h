/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_WritePipe.h ( UT Library, C++)
 *
 * COMMENTS:	This is a write pipe which allows you to poll the pipe to see
 *		if the child process has completed.  It works almost exactly
 *		like popen/pclose except for a few things:
 *			1:  You can easily get the pid of the child
 *			2:  The close() method has an option to not block
 *			    and return even if the child is running.
 *
 * CAVEATS:	This class does not work for read pipes, but then again, read
 *		pipes don't have the problem of blocking on the child
 *		process...
 */

#ifndef __UT_WritePipe__
#define __UT_WritePipe__

#include "UT_API.h"
#include <sys/types.h>
#include <stdio.h>
#include "UT_Lock.h"
#include "UT_SysClone.h"

class UT_API UT_WritePipe {
public:
    // change_process_group will put the child process in a new process group 
    // to isolate the parent from any crashes coming from the child.
     explicit UT_WritePipe(const char *cmd = 0,
			   bool change_process_group = false);

     // If the pipe is open and destructed, the process will be blocked until
     // the child is terminated.
    ~UT_WritePipe();

    /// Open a command as a write pipe.  The file descriptor returned will be
    /// the command's stdin.
    FILE	*open(const char *cmd)
		    { return open(cmd, NULL, NULL); }

    /// Open a command as a write pipe.  The file descriptor returned will be
    /// the command's stdin.  If the @c kid_stdout file descriptor is given,
    /// this will become stdout for the child.  If @c kid_stderr is specified,
    /// it will be come stderr for the child.
    FILE	*open(const char *cmd,
			FILE *kid_stdout,
			FILE *kid_stderr);

    // Close:  Close will return
    //		-1 - Error closing the pipe
    //		 0 - Child is still running (only returned if the arg is 0)
    //		>0 - Child is terminated
    // After close is called, it is not safe to write to the pipe anymore
    // (since the file descriptor is closed).  Please use isComplete() to see
    // if the child process has terminated.
    int		 close(bool wait_for_child_to_terminate = false);

    // Call this to find out whether it's safe to close (i.e. the child process
    // has terminated).  This can be called before, or after the pipe has been
    // closed.
    bool	 isComplete(bool block = false);

    void	 suspend();
    void	 restart();
    void	 terminate();
    bool	 isSuspended() const;

    FILE	*getFilePtr()		{ return myFilePtr; }

    // If close() fails, you can get the errno of the fail call by calling
    //	this method.  If the child is still running, the error will be
    //	set to EWOULDBLOCK
    int		 getErrno() const	{ return myErrno; }

    // If close() succeeds, you can get the exit status of the child process by
    // calling the following method.
    int		getStatus() const	{ return myExitStatus; }

    // This method will return the child process id.
    pid_t	getPid() const		{ return myPid; }

#ifdef WIN32
private:
    FILE	*doPOpen( char *cmd, FILE *kstdout, FILE *kstderr );
    int		 isCompleteImpl(bool block);
public:
#endif

private:
    FILE	*myFilePtr;
    int		 myErrno;
    int		 myExitStatus;
    unsigned	 myFlag;
    pid_t	 myPid;
    bool	 myChangeProcessGroup;
#ifdef WIN32
    void	*myHThread;
    void	*myHProcess;
    void	*myPipe;
#else
    UT_Lock	 myCompleteLock;
#endif
};

#endif

