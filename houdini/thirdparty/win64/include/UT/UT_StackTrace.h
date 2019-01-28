/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StackTrace.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Debugging utility to print stack tracebacks from anywhere in
 *	the code. Example usage:
 *
 *	#include <UT/UT_StackTrace.h>
 *	UT_StackTrace	tracer(stderr);
 *	tracer.doTraceback("in load method");
 *
 *	From inside a signal handler use as follows:
 *
 *	void sig_handler( UTsignalHandlerArg sig_arg)
 *	{
 *	    UT_StackTrace	tracer;
 *
 *	    tracer.doTraceback(sig_arg, "inside sig_handler");
 *	    ...
 *	}
 *
 */

#ifndef __UT_StackTrace__
#define __UT_StackTrace__

#include "UT_API.h"
#include "UT_Signal.h"
#include "UT_Debug.h"

#include <SYS/SYS_Types.h>
#include <stdio.h>

class UT_WorkBuffer;

#define TRACE_STACK()         { UT_StackTrace tr; tr.doTraceback(); }
#define TRACE_STACK_LIMIT(lm) { UT_StackTrace tr; tr.setMaxStackLevel(lm); \
				tr.doTraceback(); }
#define TRACE_STACK_FILE(fp)  { UT_StackTrace tr(fp); tr.doTraceback(); }
#define TRACE_STACK_COND(cond){ UT_StackTrace tr; tr.doCondTraceback(cond); }

/// Helper class to output stack traces from the current execution point.
class UT_API UT_StackTrace
{
public:
		 explicit UT_StackTrace(FILE *output = stderr);

    void	 doTraceback(UTsignalHandlerArg sig_arg,
			     const char *label = 0,
			     bool add_markers = false);

    void	 doTraceback(const char *label = 0,
        	             bool add_markers = false);

    void	 doCondTraceback(const char *symbol,
				 const char *label = 0,
				 bool add_markers = false);

    void	 doTracebackIntoBuffer(UT_WorkBuffer &result,
				       UTsignalHandlerArg sig_arg,
				       const char *label = 0,
				       bool add_markers = false);
    void	 doTracebackIntoBuffer(UT_WorkBuffer &result,
				       const char *label = 0,
				       bool add_markers = false);

    bool	 getSymbolNameAtAddress(const void *address,
					UT_WorkBuffer &result);

    
    void	 setShowPC(bool state)
		 {
		     myShowPC = state;
		 }
    void	 setVerbose(bool state)
		 {
		     myVerbose = state;
		 }
    void	 setTraceAllThreads(bool state) // defaults to OFF
		 {
		     myTraceAllThreads = state;
		 }
    // setting the Max stack level  is useful if you only want to 
    // print out a set number of levels of the stack 
    // (i.e. you are trying to find out who is directly calling a 
    // certain method)
    void	 setMaxStackLevel(int level)
		 {
		     myMaxStackLevel = level;
		 }
    //
    // Allow global control of the stack tracebacks so that we can
    // selectively enable/disable them in chosen code blocks.
    //
    static void  enableTraceBacks(bool state)
		 {
		    ourEnableFlag = state;
		 }

    static void	 enableCondTrace(const char *symbol, bool enable);


private:
    void	 doTracebackIntoBuffer(UT_WorkBuffer &result,
				       UTsignalHandlerArg *sig_arg,
				       const char *label,
				       bool add_markers);

    FILE	    *myFile;
    int		     myMaxStackLevel;
    bool	     myVerbose:1,
		     myShowPC:1,
		     myTraceAllThreads:1;

    static bool	     ourEnableFlag;
};


// Simply returns the usage of the current stack of the current thread.
UT_API extern exint UTgetCurrentStackUsage();

// Simply returns the max size of the current stack of the current thread.
UT_API extern exint UTgetCurrentStackLimit();

#ifdef _WIN32
/// WINDOWS ONLY: Get the stack trace of an (opened) process handle with
/// sufficient permissions.
UT_API extern bool  UTgetProcessStackTrace(
			UT_WorkBuffer &result,
			void *process,
			bool add_markers = false);
#endif

#endif
