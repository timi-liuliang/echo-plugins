/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_IO.h (SYS Library, C++)
 *
 * COMMENTS:	Platform-independent I/O routines that we require wrappers for.
 */

#ifndef __SYS_IO_H_INCLUDED__
#define __SYS_IO_H_INCLUDED__

#include "SYS_API.h"

// We only include these headers here because it is common for people to use
// fileno() with SYSisatty() which requires different platform-specific
// headers.
#ifdef WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif


/// Platform-independent version of isatty()
SYS_API extern bool SYSisatty(int fd);

/// Synonym for SYSisatty(fileno(stdin))
SYS_API extern bool SYSisStdinTTY();

/// Synonym for SYSisatty(fileno(stdout))
SYS_API extern bool SYSisStdoutTTY();

/// Synonym for SYSisatty(fileno(stderr))
SYS_API extern bool SYSisStderrTTY();

#endif // __SYS_IO_H_INCLUDED__
