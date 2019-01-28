/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Action (C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_Spawn_h__
#define __UT_Spawn_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_SysClone.h"

#define INVALID_PID	(pid_t)-1

typedef void	(*UTsysWaitLoop)(int state);

// The wait loop class is used when starting up processes which may block
// the main thread.  This mechanism allows the main thread to do some
// processing while the foreground system process (which is really run in
// the background) is still active.
// The wait loop will be called with 0==entering loop, 1==polling, 2==exit
class UT_API UT_SystemWaitLoop {
public:
    static void	setWaitLoop(UTsysWaitLoop callback);

    static void	enterWaitLoop();
    static void	pollWaitLoop();
    static void	exitWaitLoop();
};

inline bool UTisValidPid( pid_t pid )
{
    return pid >= 0;
}

/// Set process group ID for a given pid.  With pid==0 and gpid==0 this will
/// set a new process group for the current process.
/// @return 0 if successful or -1 on error.
UT_API extern int       UTchangeProcessGroup(pid_t pid, pid_t gpid);

//
//  These first two basically fork and exec the command given as either
//  an array of arguments or as a single string. The second invokes the
//  first. The first uses fork and then execvp.  If detachConsole is set
//  then the child process will have it's stdout and stderr closed.
//
UT_API extern pid_t	UTspawnvp(const char *const* args,
				int detachConsole = 0,
				bool ignore_output = false,
				bool change_process_group = false);
UT_API extern pid_t	UTspawn(const char *string,
				int detachConsole = 0,
				bool ignore_output = false,
				bool change_process_group = false);

// Wait for a spawned process and return the exit code
UT_API extern int	UTwait(pid_t pid);

/// Wait for a spawned process to exit.  If the wait system call is
/// interrupted, the exit status will not be written and the function
/// will return false.
UT_API extern bool	UTwait(pid_t pid, int &exit_code);

// Return 1 if the specified *CHILD* process is still running
// NOTE: THIS ONLY WORKS FOR PROCESSES OF THE SAME USER ON WINDOWS!
UT_API extern int	UTisRunning(pid_t pid, int *exit_code);

// UTisRunning() does a wait, implying that the process in question is a child
// process.  However, on occasion, we want a general form to determine whether
// any process (child or not) is still running.
// NOTE: THIS ONLY WORKS FOR PROCESSES OF THE SAME USER ON WINDOWS!
UT_API extern int	UTisAnyProcessRunning(pid_t pid);

// UTcheckProcessId() returns true if the given pid corresponds to an existing
// process on the current system. This will work for any process regardless
// of whether it is owned by the current user.
UT_API extern bool	UTcheckProcessId(pid_t pid);

//
//  UTbackground will put the current process into the background and
//  disassociate it from the controlling terminal. This will protect
//  the process from signals generated from the tty. Setting the
//  full_daemon flag means that you really want this process to be
//  stand-alone. This causes the function to put error messages into
//  the SYSLOG file instead of to stderr and it will close all open
//  files, cd to the root of the file system and set the process
//  umask to zero. It also logs a successful startup into the SYSLOG.
//  The name argument is used for reporting purposes and must be
//  supplied.
//
UT_API extern pid_t	UTbackground(const char *name, int full_daemon = 0);

//
//  UTforeground will force the application to have a console where printf
//  messages will be output.  Killing this console will kill the application as
//  well.  The name argument will be the title of the console.
UT_API extern void	UTforeground(const char *name = "Console");

///  Kill a process (using SIGKILL on Irix/Linux, TerminateProcess on NT).
///  The NT version also closes the process handle.
UT_API extern void	UTkill(pid_t pid, bool kill_tree=false);

/// Kill a process (using SIGKILL on Irix/Linux, TerminateProcess on NT),
/// attempting to do a stack trace on the (parent) process to stderr.
/// The NT version also closes the process handle
UT_API extern void	UTkill(pid_t pid, bool kill_tree, bool dump_stack);

///  Kills a process (using SIGTERM on Irix/Linux, TerminateProcess on NT)
///  The NT version also closes the process handle.
UT_API extern void	UTterminate(pid_t pid, bool terminate_tree=false);

#endif
