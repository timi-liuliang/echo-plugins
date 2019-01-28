/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_OslTypes.h (VEX  Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VEX_OslTypes__
#define __VEX_OslTypes__

#include "VEX_API.h"

/// TODO: While we have a library for VEX language (ie, this one), currently 
/// we don't have any library for RSL or OSL stuff. 
/// We already started putting RSL-related entities in this library (eg,
/// VEX_RslResolver), so this is continuation of this trend. Ideally, at some
/// point we should factor out any RSL and OSL code out of this library into a
/// new RSL and OSL library (or some base language library).

// ============================================================================
/// RenderMan shader context types. 
typedef enum {
    OSL_INVALID_CONTEXT		= 0x00000000,
    OSL_SURFACE_CONTEXT 	= 0x00000001,
    OSL_DISPLACE_CONTEXT 	= 0x00000002,
    OSL_GENERIC_CONTEXT 	= 0x00000004,
    OSL_VOLUME_CONTEXT		= 0x00000008,
} OSL_ContextType;


// ============================================================================
/// Basic management of shader contexts in OSL.
class VEX_API OSL_ContextManager
{
public:
    /// Get the name token of a context.
    static const char		*getNameFromContextType( OSL_ContextType t );

    /// Returns a context label token for a given type.
    static const char		*getLabelFromContextType( OSL_ContextType t );

    /// Map the context name to a context type.
    static OSL_ContextType	 getContextTypeFromName( const char *name );
};

#endif


