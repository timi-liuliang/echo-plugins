/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Wrapper on signal handlers.
 *		By instantiating an object of this type, the signal
 *		handler is set to the desired value.  When this
 *		object is destructed, the signals are automatically
 *		returned to their previous values.
 *
 * NOTES:	Since the constructor/destructor handle installing
 *		and removal of the signals, this can even be used
 *		within a block of code to allow signals to be caught
 *		temporarily.
 *		When installing signals permanently, it is suggested
 *		that signal(2) still be used.  However, it is possible
 *		to use the static "override" method below.
 */

#ifndef __UT_Signal_h__
#define __UT_Signal_h__

#include "UT_API.h"
#include <signal.h>		// Get a list of signals for us please

#if defined(GAMEOS)
    #include "UT_GameOsUtil.h"

    #define	SIGINT	2
    #define	SIGILL	4
    #define	SIGFPE	8
    #define	SIGSEGV	11
    #define	SIGTERM	15

    #define	SIG_DFL	((__sighandler_t)0)
    #define	SIG_IGN	((__sighandler_t)1)
    #define	SIG_ERR	((__sighandler_t)1)
#endif

#if defined(WIN32) || defined(GAMEOS)
    // define all the signals not already defined by NT.
    // Removed two signals that conflicted with defined NT signals.
	// Reserved signal #'s on NT so far are: 2,4,8,11,15,21,22
    // Please note that addSignal will silently ignore any non-handled
    // windows handlers as the runtime library does parameter validation
    // and rejects them.
    #define	SIGHUP	1
    // WIN32:	SIGINT  2
    #define	SIGQUIT	3
    // WIN32:	SIGILL	4
    #define	SIGTRAP	5	// Unhandled
    #define	SIGEMT	7	// Unhandled
    // WIN32:	SIGFPE	8
    #define	SIGKILL	9	// Unhandled
    #define	SIGBUS	10	// Unhandled
    // WIN32:	SIGSEGV	11
    #define	SIGSYS	12	// Unhandled
    #define	SIGPIPE	13
    #define	SIGALRM	14	// Unhandled
    // WIN32:	SIGTERM	15
    #define	SIGUSR1	16	// Windows: IOINT?
    #define	SIGUSR2	17	// Windows: STOP?
		// do not use SIGCLD anymore! provided here for backwards
		// compatibility only!
    #define	SIGCLD	18	// Unhandled
		// SIGCCHLD is the new POSIX name for SIGCLD, use this instead
    #define	SIGCHLD 18	// Unhandled
    #define	SIGPWR	19	// Unhandled
    #define	SIGWINCH 20	// Unhandled
    // WIN32:	SIGBREAK 21
    // It's probably ok that SIGURG (urgent socket condition) is handled the
    // same as SIGBREAK since there's no such thing on NT
    #define	SIGURG 	21
    // WIN32:	SIGABRT	22
    #define	SIGSTOP	23	// Unhandled
    #define	SIGTSTP	24	// Unhandled
    #define	SIGCONT	25	// Unhandled
    #define	SIGTTIN	26	// Unhandled
    #define	SIGTTOU	27	// Unhandled
    #define 	SIGVTALRM 28	// Unhandled
    #define 	SIGPROF	29	// Unhandled
    #define 	SIGXCPU	30	// Unhandled
    #define 	SIGXFSZ	31	// Unhandled
    #define	SIG32	32	// Unhandled
    // According to Irix it supports all signal numbers between 0 and 64
    // So all other signals should probably start at 65
    #define	SIGPOLL	65	// Unhandled

    #define sigset(a,b) signal(a,b)
#else

#if !defined(SIGEMT)
#if defined(SIGUNUSED)
#define SIGEMT SIGUNUSED
#else
#define SIGEMT SIGBUS
#endif
#endif

#if !defined(SIGSYS)
#if defined(SIGUNUSED)
#define SIGSYS SIGUNUSED
#else
#define SIGSYS SIGBUS
#endif
#endif

#endif

#define UT_MAX_SIGNALS	64	// Allow for POSIX standard

#if defined(LINUX) || defined(MBSD)
#define SUPPORTS_SIGINFO
#endif

class UT_API UTsignalHandlerArg
{
public:
    UTsignalHandlerArg(int signal_num)
	: mySignalNum(signal_num),
	  myOpaqueCPUContext(0),
	  myDepthFromSignalHandler(0)
    {}

    // Increment the depth from the signal handler in the copy constructor.
    // That way, as the signal handler argument is passed from the signal
    // handler function to other functions, we'll know how far back up
    // in the stack the original signal handler function is.
    UTsignalHandlerArg(const UTsignalHandlerArg &sig_arg)
	: mySignalNum(sig_arg.mySignalNum),
#ifdef SUPPORTS_SIGINFO
	  mySignalInfo(sig_arg.mySignalInfo),
#endif
	  myOpaqueCPUContext(sig_arg.myOpaqueCPUContext),
	  myDepthFromSignalHandler(sig_arg.myDepthFromSignalHandler + 1)
    {}

    int getSignalNum() const { return mySignalNum; }

#ifdef SUPPORTS_SIGINFO
    void	 setSignalInfo(siginfo_t *signal_info)
    {
	mySignalInfo = signal_info;
    }
    const siginfo_t *getSignalInfo() const { return mySignalInfo; }
#endif
    void	 setSignalContext(void *opaque_cpu_context)
    {
	myOpaqueCPUContext = opaque_cpu_context;
    }
    const void	    *getOpaqueCPUContext() const { return myOpaqueCPUContext; }

    int		     getDepthFromSignalHandler() const
					{ return myDepthFromSignalHandler; }

private:
    int		 mySignalNum;
    void	*myOpaqueCPUContext;
    int		 myDepthFromSignalHandler;
#ifdef SUPPORTS_SIGINFO
    siginfo_t	*mySignalInfo;
#endif
};


typedef void (*UTsigHandler)(UTsignalHandlerArg);


#if !defined(LINUX) && !defined(MBSD)
extern "C"
#endif
typedef void (*UToldStyleSigHandler)(int);

class UT_API UT_Signal
{
public:
    // Catch a specified signal.  If signum <= 0, then all signals
    // (except ABRT) are caught.  There are two styles of constructors.
    // One takes an old-style signal handler that is needed so we can easily
    // specify SIG_DFL, SIG_IGN, and SIG_ERR as the signal behaviours.  The
    // other constructor takes our own data structure as an argument and is
    // designed to support the SA_SIGINFO 3-argument style callback added in
    // POSIX.1b.
    // If ignore_prev is false, all other signal handlers for the raised
    // signal will be called in the reverse order they were installed.
    // Otherwise, only the last handler for this signal will be called.
    explicit UT_Signal(int signum=0, UToldStyleSigHandler handler = SIG_DFL,
		       bool ignore_prev=false);
    explicit UT_Signal(int signum, UTsigHandler handler,
		       bool ignore_prev=false);

    // Catch a specified set of signals.  The first argument is an
    //	  array of signal numbers to catch.  The array should be
    //	  null terminated (since there is no signal 0)
    UT_Signal(const int signums[], UToldStyleSigHandler handler = SIG_DFL,
	      bool ignore_prev=false);
    UT_Signal(const int signums[], UTsigHandler handler,
	      bool ignore_prev=false);

    // Destructing will restore the signals to their previous values.
    ~UT_Signal();

    // The following method is made available to override the default
    // handler without destructing the object.  For example, in the
    // signal handler, you may want to ignore further signals of a certain
    // type.  This method allows you to set the signals.  In fact, it
    // simply calls sigaction() with the new handler.
    void	override(int signum, UToldStyleSigHandler,
			 bool ignore_prev=false);
    void	override(int signum, UTsigHandler,
			 bool ignore_prev=false);

private:
    // This class keeps track of a signal handler callback that is either
    // an old-style callback (one that accepts an int, like SIG_DFL or
    // SIG_IGN) or a new-style callback (one that accepts a UTsignalHandlerArg).
    class UT_API UT_ComboSignalHandler
    {
    public:
	UT_ComboSignalHandler();
	UT_ComboSignalHandler(UTsigHandler handler);
	UT_ComboSignalHandler(UToldStyleSigHandler handler);

	UT_ComboSignalHandler &operator=(UTsigHandler handler);
	UT_ComboSignalHandler &operator=(UToldStyleSigHandler handler);
	bool operator!=(UToldStyleSigHandler handler) const;

	bool isSet() const;
	void operator()(int signal_num,
#ifdef SUPPORTS_SIGINFO
			siginfo_t *signal_info,
#endif
			void *opaque_cpu_context) const;

    private:
	union
	{
	    UTsigHandler	 myNewStyle;
	    UToldStyleSigHandler myOldStyle;
	} mySigHandler;
	bool myIsOldStyleSigHandler;
    };

    void		 constructPriv(int signum,
				       UT_ComboSignalHandler handler,
				       bool ignore_prev);
    void		 constructPriv(const int signums[],
				       UT_ComboSignalHandler handler,
				       bool ignore_prev);
    void		 addSignal(int signum, UT_ComboSignalHandler handler,
				   bool ignore_prev);
    void		 removeSignal(int signum);
    void		 updateActuals();
    void		 overridePriv(int signum,
				      UT_ComboSignalHandler handler,
				      bool ignore_prev);

    static void		 processSignal(int signal_num
#ifdef SUPPORTS_SIGINFO
				       , siginfo_t *, void *
#endif
				      );

    UT_ComboSignalHandler mySignals[UT_MAX_SIGNALS];
    bool		 myIgnorePrev[UT_MAX_SIGNALS];

    UT_Signal		*myNext, *myPrev;
};

#endif
