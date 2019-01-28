/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Version.h ( UT Library, C/C++)
 */

/*
 * NOTE: This is automatically generated from SYS_Version.tmpl.h
 */
#ifndef __SYS_Version__
#define __SYS_Version__

/// @file SYS_Version.h
/// Defines compile time version constants for use in code.
/// @note Please see UT_String::compareVersionString()

// <sys/types.h> defines macros with the names 'major' and 'minor'.
// glibc 2.25+ emits deprecation warnings for this, so eventually the include
// of <sys/sysmacros.h> will be removed and the undef'ing won't be necessary.
#undef major
#undef minor


// The major.minor.build version as a string
#define SYS_VERSION_FULL "17.0.352"

// The major.minor version as a string
#define SYS_VERSION_RELEASE "17.0"

// The major version.  .9 counts as next higher.
#define SYS_VERSION_MAJOR "17"
#define SYS_VERSION_MINOR "0"
#define SYS_VERSION_BUILD "352"
#define SYS_VERSION_PATCH "0"

// Version numbers as integer values
#define SYS_VERSION_MAJOR_INT 17
#define SYS_VERSION_MINOR_INT 0
#define SYS_VERSION_BUILD_INT 352
#define SYS_VERSION_PATCH_INT 0

// Version as a packed integer: ("%02x%02x%04x", major, minor, build)
#define SYS_VERSION_FULL_INT 0x11000160

#if defined(__cplusplus)
struct SYS_Version
{
     /// Return the "major.minor.build" version
     static inline const char *full() 	 { return SYS_VERSION_FULL; }
     
     /// Return the "major.minor" version
     static inline const char *release() { return SYS_VERSION_RELEASE; }
     
     /// Return the "major" version
     static inline const char *major()	 { return SYS_VERSION_MAJOR; }
     
     /// Return the "minor" version
     static inline const char *minor()	 { return SYS_VERSION_MINOR; }
     
     /// Return the "build" version
     static inline const char *build()	 { return SYS_VERSION_BUILD; }
     
     /// Return the "patch" version
     static inline const char *patch()	 { return SYS_VERSION_PATCH; }
     
     /// Return the full version as a numerical set of values.
     static inline void getFull(int &major, int &minor, int &build,
                                int *patch = 0)
     {
	 major = SYS_VERSION_MAJOR_INT;
	 minor = SYS_VERSION_MINOR_INT;
	 build = SYS_VERSION_BUILD_INT;
         if (patch)
             *patch = SYS_VERSION_PATCH_INT;
     }
     
     /// Return the release version as a numerical set of values.
     static inline void getRelease(int &major, int &minor)
     {
	 major = SYS_VERSION_MAJOR_INT;
	 minor = SYS_VERSION_MINOR_INT;
     }

     static inline int compare(int major1, int minor1, int build1,
             int major2, int minor2, int build2)
     {
         if(major1 != major2) return major1 - major2;
         if(minor1 != minor2) return minor1 - minor2;
         if(build1 != build2) return build1 - build2;
         return 0;
     }

     static inline int compare(int major1, int minor1, int build1, int patch1,
             int major2, int minor2, int build2, int patch2)
     {
         if(major1 != major2) return major1 - major2;
         if(minor1 != minor2) return minor1 - minor2;
         if(build1 != build2) return build1 - build2;
         if(patch1 != patch2) return patch1 - patch2;
         return 0;
     }
};
#endif

#endif

