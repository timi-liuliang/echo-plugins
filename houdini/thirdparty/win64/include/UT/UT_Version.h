/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Version.h ( UT Library, C++)
 */

/*
 * NOTE: This is a compatibility header. Use SYS_Version.h instead.
 */

#ifndef __UT_Version__
#define __UT_Version__

#include <SYS/SYS_Version.h>

/// @file UT_Version.h
/// Defines compile time version constants for use in code.
/// @note Please see UT_String::compareVersionString()

// The major.minor.build full version as a string
#define UT_FULL_VERSION SYS_VERSION_FULL

// The major.minor release version as a string
#define UT_RELEASE_VERSION SYS_VERSION_RELEASE

// The major version.  .9 counts as next higher.
#define UT_MAJOR_VERSION SYS_VERSION_MAJOR
#define UT_MINOR_VERSION SYS_VERSION_MINOR

// Version numbers as integer values
#define UT_MAJOR_VERSION_INT SYS_VERSION_MAJOR_INT
#define UT_MINOR_VERSION_INT SYS_VERSION_MINOR_INT
#define UT_BUILD_VERSION_INT SYS_VERSION_BUILD_INT
#define UT_PATCH_VERSION_INT SYS_VERSION_PATCH_INT

// Version as a packed integer: ("%02x%02x%04x", major, minor, build)
#define UT_VERSION_INT SYS_VERSION_FULL_INT


/// The version string to insert in paths for save directories.
#define UT_SAVE_VERSION SYS_VERSION_RELEASE


/// Return the full major.minor.build version.
inline const char *UTgetFullVersion() { return SYS_Version::full(); }
/// Return the "major.minor" release version
inline const char *UTgetReleaseVersion() { return SYS_Version::release(); }
/// Return the "major" version
inline const char *UTgetMajorVersion()	{ return SYS_Version::major(); }
/// Return the version used for save directories
inline const char *UTgetSaveVersion()	{ return SYS_Version::release(); }

#endif
