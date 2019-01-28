/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_HDKVersion.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_HDKVERSION_H_INCLUDED__
#define __UT_HDKVERSION_H_INCLUDED__

#include "UT_API.h"

/// @file UT_HDKVersion.h

/// Macro that specifies the HDK API version.
///
/// This monotonically increasing value is updated to the last Houdini version
/// with known ABI incompatibilities. Typically, this will be set on every
/// major release and only occasionally on releases within a X.Y version.
///
/// Note this does not include stub versions, so one should assume ABI
/// is incompatible between stub versions (16.0.459.3 -> 16.0.459.4).
///
/// This number is only updated after release.  If you are using alpha
/// or beta versions, you should assume ABI is always incompatible.
///
/// The format is a packed 8 digit integer consisting of version of Houdini
/// when the API was last changed:
///     ("%02d%02d%04d", major, minor, build)
/// See the corresponding SYS_VERSION_* defines in SYS_Version.h.
///
/// Usage:
/// @code
/// // If using the HDK since the API change from Houdini 15.5.499
/// #if HDK_API_VERSION >= 15050499
/// #endif
/// @endcode
#define HDK_API_VERSION 17000000

/// Returns the runtime HDK API version. This can be used by plugins to warn or
/// error if running in an unsupported Houdini version.
UT_API unsigned UTgetHDKAPIVersion();

#endif // __UT_HDKVERSION_H_INCLUDED__
