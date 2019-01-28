/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Side Effects Software Inc.
 *
 * NAME:	SYS_Platform.h (SYS Library, C++)
 *
 * COMMENTS:	Functions related to OS/Machine specific
 *		information.
 */

#ifndef __SYS_Platform_h__
#define __SYS_Platform_h__

#include "SYS_API.h"
#include "SYS_Types.h"
#include <string>

/// Get processor information about the CPU that Houdini is running on.
/// Please see SYS_ProcessorDefines.h for CPU information
SYS_API extern void		 SYSgetProcessor(int &majortype, int &flags);

/// Determine if we satisfy minimum processor requirements
SYS_API extern bool		 SYShasMinimumProcessor();

/// Returns the number of processors available for the process, taking into
/// account affinity masks.
///
/// NOTE: Unless you have a good reason to call this directly, you probably
///       want to call UT_Thread::getNumProcessors(), instead, because that
///       takes into account users opting to limit the thread count,
///       and caches the number for fast querying.
SYS_API extern int		 SYSgetProcessorCount();

#if defined(WIN32)
/// These are intended for private use by UT_Thread
/// @{
struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX;
SYS_API extern const struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *SYSgetProcessorGroupInfo();
struct _GROUP_AFFINITY;
typedef int (__stdcall *SetThreadGroupAffinityFunc)(void*, const struct _GROUP_AFFINITY *, struct _GROUP_AFFINITY*);
SYS_API extern SetThreadGroupAffinityFunc SYSgetSetThreadGroupAffinity();
/// @}
#endif

/// Find the name of the operating system that Houdini was compiled for
SYS_API extern const char	*SYSgetOsProc();

/// Return a "nice" name of the current operating system
SYS_API extern std::string	 SYSgetRuntimeOSLabel();

/// Find the exact OS, 32-bit or 64-bit build, compiler version
SYS_API extern const char	*SYSgetPlatformSpec();

/// Returns the amount of installed memory (not VM).
SYS_API extern exint		 SYSgetMemorySize();

/// Returns a char* representing the manufacturerer's name
SYS_API extern std::string	SYSgetCPUBrandString();

/// Just a handy, quick, and fast function to test the platform for which
/// the code is being compiled.
SYS_API extern bool		 SYSisMBSD();
SYS_API extern bool		 SYSisLinux();
SYS_API extern bool		 SYSisWindows();

/// Return the MAC addresses for all the deviced on this system.  The caller is
/// responsible to delete [] the addresses if num_addresses > 0.
SYS_API extern void		 SYSgetMACAddresses(
				    unsigned char *&addresses,
				    int &num_addresses);

/// Return a single MAC address that we treat as the main MAC address for this
/// machine.  Returns false if no MAC address could be determined.
SYS_API extern bool		 SYSgetMACAddress(unsigned char mac_address[6]);

// Return the MAC address giving the same value returned by Python's
// uuid.getnode().
SYS_API extern bool		 SYSgetMACAddressAsInteger(int64 &mac_address);

#endif // __UT_SysSpecific_h__
