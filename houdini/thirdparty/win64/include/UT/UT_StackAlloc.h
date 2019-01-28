/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StackAlloc.h ( UT Library, C++)
 *
 * COMMENTS:	Wrapper for alloca.
 */

#ifndef __UT_StackAlloc__
#define __UT_StackAlloc__

#include "UT_Assert.h"

#ifdef WIN32
    #include <malloc.h>
#else
    #if defined(LINUX) || defined(MBSD)
        #include <stdlib.h>
    #else
        #include <alloca.h>
    #endif
#endif

// The maximum size of an object that we would allocate on the stack,
// in bytes
#define UT_STACK_LIMIT	131072

// Sentinel values to be stored in the first 4 bytes of the stack data
#define UT_STACK_ALLOCA	100
#define UT_STACK_MALLOC	200

#define UTstackAlloc(size)	 (void *)(&((size < UT_STACK_LIMIT) ?    \
    (*((int32 *)::alloca(size+2*sizeof(int32))) = UT_STACK_ALLOCA) : \
    (*((int32 *)::malloc(size+2*sizeof(int32))) = UT_STACK_MALLOC)) + 2)

#define UTstackFree(ptr)						\
        {								\
            UT_ASSERT_P(*((int32 *)ptr - 2) == UT_STACK_MALLOC ||	\
                        *((int32 *)ptr - 2) == UT_STACK_ALLOCA);	\
            if (*((int32 *)ptr - 2) == UT_STACK_MALLOC)		\
                ::free(((int32 *)ptr - 2));				\
        }

#endif
