/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Chris Thompson
 *	Side Effects Software Inc.
 *
 * NAME:	UT_SysSpecific.h (Utility Library, C++)
 *
 * COMMENTS:	Functions related to operating-system specific
 *		preferences, defaults, helper applications, etc.
 *		The difference between this file and UT_SysClone
 *		is that that one simply replaces SGI system calls with
 *		custom calls of the same name, while SysSpecific is a true
 *		wrapper that hides even the system call name under a "UT"-
 *		prefixed name.	
 *
 */

#ifndef __UT_SysSpecific_h__
#define __UT_SysSpecific_h__

#include "UT_API.h"
#include "UT_Defines.h"
#include <SYS/SYS_Platform.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>

class UT_String;

// Returns the name of the user's text editor. 
// (This is determined by the EDITOR environment variable;
// a system-appropriate default editor is chosen if EDITOR
// is not set.)
// Do not free() or delete[] the returned pointer.
UT_API extern const char *UTgetTextEditor(bool &is_graphical);

UT_API extern void UTinvokeTextEditor(const char *fileToEdit, UT_Bool blocking, const char *title);

UT_API extern UT_Bool UTopenShell(UT_Bool blocking, const char *title);

UT_API extern UT_Bool UTrunTextProgram(const char *commandLine, UT_Bool blocking, const char *title);

UT_API extern UT_Bool UTrunGraphicalProgram(const char *commandLine, UT_Bool blocking, const char *title);

// 'in_shell' causes commandLine to be executed in csh on NT, instead of the
// brain-dead command prompt (which doesn't support I/O redirection & such).
// It has no effect on other UNIX based OSes.
UT_API extern int UTsystem(const char *commandLine, bool in_shell = false);

// Starts an external web browser and opens a URL.
UT_API extern void UTopenURLInWebBrowser( const char * url );

UT_API extern void UTgetProcessor(int &majortype, int &flags);

// Gets OS_PROC
static inline const char *UTgetOsProc()	{ return SYSgetOsProc(); }

// Sets name to a hardened version of the users name, ie: "jlait".
// If alphaonly is set, it will munge the user name to be something
// with no spaces by adding _, etc.
// Defaults to login name, but can be overridden.
UT_API extern void UTgetUserName(UT_String &name, int alphaonly = 1);

// you@machine
// The machine defaults to your host name, but can be overridden
UT_API extern void UTgetMachineName(UT_String &name, int alphaonly = 1);

// machine
// The machine defaults to your host name, but can be overridden
UT_API extern void UTgetUserMachineName(UT_String &name, int alphaonly = 1);

UT_API extern void UTdisplayMessageWindow(const char *message_text,
				  const char *message_title = "Houdini Error");
UT_API extern void UTdisplayMessageWindowWithHelpURL(const char *message_text,
				  const char *help_url,
				  const char *message_title = "Houdini Error");
UT_API extern bool UTdisplayConfirmationWindow(const char *message_text,
				  const char *message_title = "Houdini");

static inline int
UTvsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    // Windows prefixes vsnprintf with an underscore for some reason.
#ifdef WIN32
    return _vsnprintf(str, size, format, ap);
#else
    int result;
    va_list ap_copy;

    // We need to make a copy of the argument pointer in case we 
    // have to call UTvsnprintf multiple times.  It is generally not safe to 
    // re-use the same va_list in multiple calls.  Reusing the same va_list 
    // caused  a crash on a 64 bit Linux machine compiled with gcc 3.4.
    // va_copy only became standard with C99 and is currently not supported
    // on Windows.
    va_copy(ap_copy, ap);
    result = vsnprintf(str, size, format, ap_copy);
    va_end(ap_copy);
    return result;
#endif
}

UT_API extern std::string
UTunmangleClassNameFromTypeIdName(const std::string &name);

#define UTstatic_cast(Type, Data) static_cast<Type>(Data)

#endif // __UT_SysSpecific_h__
