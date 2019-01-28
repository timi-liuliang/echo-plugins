/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Stdio.h (SYS Library, C++)
 *
 * COMMENTS:	Platform-independent wrappers for functions in <stdio.h>
 */

#ifndef __SYS_STDIO_H_INCLUDED__
#define __SYS_STDIO_H_INCLUDED__

#include "SYS_API.h"
#include "SYS_Types.h"

#include <stdio.h>

static inline int
SYSfseek(FILE *stream, int64 offset, int whence)
{
#ifdef _WIN32
    return _fseeki64(stream, offset, whence);
#else
    // fseeko uses 64-bit offsets even on 32-bit platforms when
    // _FILE_OFFSET_BITS is defined as 64.
    return fseeko(stream, offset, whence);
#endif
}

static inline int64
SYSftell(FILE *stream)
{
#ifdef _WIN32
    return _ftelli64(stream);
#else
    // ftello uses 64-bit offsets even on 32-bit platforms when
    // _FILE_OFFSET_BITS is defined as 64.
    return ftello(stream);
#endif
}

#endif // __SYS_STDIO_H_INCLUDED__
