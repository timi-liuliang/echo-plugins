/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Align.h ( SYS Library, C++)
 *
 * COMMENTS:	System specific methods of creating aligned data.
 *
 */

#ifndef __SYS_Align__
#define __SYS_Align__

#include <stdlib.h>

/// These definitions are system specific methods of forcing data to be
/// aligned.  Some processors require data to be aligned on 16 byte boundaries
/// for certain operations.  These macros and inline functions provide a
/// consistent approach to doing this.
///
/// Example 1:
///    class Foo {
///	  SYS_ALIGN16 int	a[4];
///	  int			b;
///	  SYS_ALIGN16 char	c;
///	};
/// In the struct, both the A and C members will be aligned on a 16 byte
/// boundary.  This will force the size of the class to be 48 bytes.
///
/// Example 2:
///	fpreal32 *list = SYSamalloc(4*sizeof(fpreal32));
/// This will guarantee that the list pointer is aligned on a 16 byte boundary.
///
/// Example 3:
///	fpreal32 *list = SYSamalloc(4*sizeof(fpreal32), 128);
/// This will guarantee that the list is aligned to a 128 byte boundary.

#if defined(WIN32)
    #include <malloc.h>
    #include <string.h>

    #define SYS_ALIGN16		__declspec(align(16))
    #define SYS_ALIGN(b)	__declspec(align(b))

    inline void	*SYSamalloc(size_t b, size_t a=16)
		    { return _aligned_malloc(b, a); }

    inline void *SYSacalloc(size_t n, size_t b, size_t a=16)
		    {
			b *= n;
			void *p = _aligned_malloc(b, a);
			memset(p, 0, b);
			return p;
		    }
    inline void  SYSafree(void *p)
		    { _aligned_free(p); }

#elif defined(LINUX) || defined(MBSD)
    #include <malloc.h>
    #include <stdlib.h>
    #include <string.h>

    #define SYS_ALIGN16		__attribute__ ((aligned(16)))
    #define SYS_ALIGN(b)	__attribute__ ((aligned(b)))

// 64-bit libc guarantees 16-byte alignment for all calls to malloc.  32-/64-bit
// jemalloc and 32-bit libc guarantee only 8-byte alignment (though 64-bit
// jemalloc appears to always 16-byte-align sufficiently large allocations).
// Also, jemalloc aligns memory to twice its argument, whereas libc aligns it to
// the argument.
//
// There are no aligned versions of realloc and calloc in libc. There are
// "experimental" versions in jemalloc.

    inline void *SYSamalloc(size_t b, size_t a=16)
    {
	void	*p;
	if (posix_memalign(&p, a, b) != 0)
	    return nullptr;	// Invalid alignment or out of memory
#ifdef UT_DEBUG
	memset(p, 0xCD, b);
#endif
	return p;
    }
    inline void	*SYSacalloc(size_t n, size_t b, size_t a=16)
    {
	void	*p;
	if (posix_memalign(&p, a, b*n) != 0)
	    return nullptr;	// Invalid alignment or out of memory
	memset(p, 0, b);
	return p;
    }
    // posix_memalign() guarantees the memory can be released using free()
    inline void  SYSafree(void *p)
    {
	free(p);
    }
#else
    #define SYS_ALIGN16
    #define SYS_ALIGN(b)

    inline void *SYSamalloc(size_t b)		{ return malloc(b); }
    inline void	*SYSacalloc(size_t n, size_t b)	{ return calloc(n, b); }
    inline void  SYSafree(void *p)		{ free(p); }
    inline void *SYSamalloc(size_t b, size_t)	{ return malloc(b); }
#endif

#endif
