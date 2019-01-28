#ifndef __RE_OGLErrorChecker__
#define __RE_OGLErrorChecker__

#include "RE_API.h"
#include <UT/UT_Defines.h>

class RE_OGLRender;

//
//  functions for checking the OpenGL state for invalid operations
//
//  RE_CHECK_STATE	ERROR CHECKING (cumulative):
//
//	0		No checking performed.
//	1		Check for valid context, errors checked on swapbuffers
//	2		errors checked by RE_CheckNoErrors...()
//      3		errors checked every glEnd...() call.
//      4               OpenGL state verified for each RE call (debug RE).
//
// NOTE: If you have GL_KHR_DEBUG, you should use the environment variable
//       HOUDINI_OGL_DEBUG instead (also 0...4, though different meanings:
//
//      0		Off
//	1		Errors and high-priority messages 
//	2		Medium priority (Performance warnings)
//	3		Low priority (portability & deprecation)
//	4		Messages (Houdini annotations via debug group)
//
//	The priority is defined by the driver, and may differ between vendors.

//	HOUDINI_OGL_DEBUG has no noticeable performance hit (unlike
//	RE_CHECK_STATE) and you can also get reasonable stack traces from it.
//	You can	also exclude specific message types via
//	HOUDINI_OGL_DEBUG_EXCLUDE:
//	  portability, error, performance, undefined (behaviour), deprecated,
//	  annotation, other, opengl, compiler, window (glX,wGL)
//	  thirdparty, houdini (app-posted msgs), group (group annotations).
//	ie. export HOUDINI_OGL_DEBUG_EXCLUDE="portability window annotation"
//

// by default, we check for a valid context, and for errors on swapbuffers().
#ifndef RE_CHECK_STATE
#  ifdef MBSD_COCOA
#    ifdef UT_DEBUG
#       define RE_CHECK_STATE	4
#    else
#       define RE_CHECK_STATE	0
#    endif
#  else
#    define RE_CHECK_STATE	0
#  endif
#endif


#if (RE_CHECK_STATE >= 1)
    #define	RE_VerifyState()
    #define	RE_CheckForErrorsOnSwap()	REcheckForErrorsOnSwap()
    #define	RE_CheckValidContext()		REcheckValidContext()
    #define	RE_CheckNoErrors(string)	REcheckNoErrors( \
						    __FILE__, __LINE__, string)
    #define	RE_CheckContextErrors(r, string) REcheckContextErrors(r, \
						    __FILE__, __LINE__, string)
    #define	RE_SetLockedContext(render)	REsetLockedContext(render)

    #define     RE_EnableDebugMode( enable )	REenableDebugMode(enable)
    #define	RE_DebugModeEnabled()		REdebugModeEnabled()
    #define	RE_CHECK_ERRORS()		REcheckNoErrors( \
						    __FILE__, __LINE__, 0)
#else  /* RE_CHECK_STATE */
    #define     RE_VerifyState
    #define	RE_CheckForErrorsOnSwap()
    #define	RE_CheckValidContext()
    #define	RE_CheckNoErrors(string)
    #define	RE_CheckContextErrors(r, string)
    #define	RE_SetLockedContext(render)
    #define     RE_EnableDebugMode( enable )
    #define	RE_DebugModeEnabled() false
    #define	RE_CHECK_ERRORS()
#endif	/* RE_CHECK_STATE */

// Actual implemention
RE_API extern void	REcheckForErrorsOnSwap();
RE_API extern void	REcheckValidContext();
RE_API extern void	REcheckNoErrors(const char *file, int line,
					const char *message);
RE_API extern void	REcheckContextErrors(const RE_OGLRender *r,
					     const char *file, int line,
					     const char *message);
RE_API extern void 	REenableDebugMode(bool enable);
RE_API extern bool 	REdebugModeEnabled();
RE_API extern void	REsetLockedContext(const RE_OGLRender * render);

#endif /* __RE_OGLErrorChecker__ */
