/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MemLeakDetector.h
 *
 * COMMENTS:    This file contains utilities to detect an isolate memory leaks.
 *		It currently only works on Windows debug builds.
 *		To enable the memory leak detector, make sure that 
 *		UT_MEMLEAKDETECTOR_ENABLE is defined and recompile the .C file.
 *		Check the .C file for options to control the memory leak 
 *		detection.
 */
#ifndef __UT_MemLeakDetector_h__
#define __UT_MemLeakDetector_h__

#include "UT_API.h"
#include <stdlib.h>

// Only enable leak detection in Windows debug builds.
#if !defined(WIN32) || !defined(_DEBUG)

#undef UT_MEMLEAKDETECTOR_ENABLE

#else

#define UT_MEMLEAKDETECTOR_ENABLE

#endif




#ifndef UT_MEMLEAKDETECTOR_ENABLE

inline void UTmemLeakDetectorReport() { }
inline void UTmemLeakDetectorReportAllocsSinceLastCheckpoint() { }
inline void UTmemLeakDetectorCheckPoint() { }

#else

#ifndef UT_MEMLEAKDETCTOR_DISABLE_MAP_ALLOC

// If .C files are compiled with this define, the reports generated here
// will be more useful.
#define  _CRTDBG_MAP_ALLOC

// The order of these includes is important.
#include <malloc.h>
#include <windows.h>
#include <crtdbg.h>

#else

#include <crtdbg.h>

#endif // UT_MEMLEAKDETCTOR_DISABLE_MAP_ALLOC


/// Generates a report of the memory leaks encountered so far.
UT_API void UTmemLeakDetectorReport();

/// Generates a report of all memory allocations since the last time
/// UTmemLeakDetectorCheckPoint() was called.
UT_API void UTmemLeakDetectorReportAllocsSinceLastCheckpoint();

/// Tags a spot in the execution of your program as a check point.
UT_API void UTmemLeakDetectorCheckpoint();

#endif // UT_MEMLEAK_DETECTOR_ENABLE


namespace UT_MemLeakDetectorImpl
{

class UT_API Block
{
public:
		    Block(
			const char *file,
			int line,
			const char *label = nullptr,
			bool dump_leaks = false);
		    ~Block();
    void	    begin(
			const char *file,
			int line,
			const char *label,
			bool dump_leaks);
    void	    end(bool dump_leaks = false);
private:
#ifdef UT_MEMLEAKDETECTOR_ENABLE
    _CrtMemState    myBeg;
#else
    size_t	    myBeg;
#endif
    bool	    myBegun:1;
    bool	    myDumpLeaks:1;
};

} // namespace UT_MemLeakDetectorDetail

// UT_MEMLEAK_VAR(name) creates a unique variable name<X> where <X> is
// different everytime the macro is used.
#define UT_MEMLEAK_CONCAT_IMPL(x, y)	x ## y
#define UT_MEMLEAK_CONCAT(x, y)		UT_MEMLEAK_CONCAT_IMPL(x, y)
#define UT_MEMLEAK_VAR(name)		UT_MEMLEAK_CONCAT(name, __COUNTER__)

/// A memory block to track memory growth:
///	UT_MEMBLOCK(const char *label = nullptr, bool dump_leaks = false)
///
/// On debug Windows builds, the dump_leaks option will also try to summarize
/// the outstanding memory allocations using the debug heap.
#define UT_MEMBLOCK(...) \
    UT_MemLeakDetectorImpl::Block \
	UT_MEMLEAK_VAR(memblock) (__FILE__, __LINE__, __VA_ARGS__) \
    /**/

/// Like UT_MEMBLOCK() but allows you to name the block variable
#define UT_MEMBLOCK_V(var, ...) \
    UT_MemLeakDetectorImpl::Block var(__FILE__, __LINE__, __VA_ARGS__) \
    /**/

/// Used with UT_MEMBLOCK_V to do intermediate check points.
#define UT_MEMBLOCK_CHECK(old_var, new_var, ...) \
    old_var.end(); \
    UT_MemLeakDetectorImpl::Block new_var(__FILE__, __LINE__, __VA_ARGS__) \
    /**/


#endif // __UT_MemLeakDetector_h__
