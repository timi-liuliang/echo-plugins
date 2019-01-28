/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetUtil.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_NETUTIL_H_INCLUDED__
#define __UT_NETUTIL_H_INCLUDED__

#include "UT_API.h"

#ifdef _WIN32
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
#endif

/// @file
/// Common network utilities

class UT_String;

/// Given host, resolves it to the canonical name if possible.
/// If false is returned, then host is untouched.
UT_API extern bool UTresolveCanonicalHostname(UT_String& host);

#endif // __UT_NETUTIL_H_INCLUDED__
