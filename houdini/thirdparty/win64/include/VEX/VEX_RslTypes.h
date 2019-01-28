/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_RslTypes.h (VEX  Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VEX_RslTypes__
#define __VEX_RslTypes__

#include "VEX_API.h"

/// TODO: While we have a library for VEX language
/// (ie, this one), currently we don't have any library for RSL stuff. We
/// already started putting RSL-related entities in this library (eg,
/// VEX_RslResolver), so this is continuation of this trend. Ideally, at some
/// point we should factor out any RSL related code out of this library into a
/// new RSL library.

// ============================================================================
/// RenderMan shader context types. 
typedef enum {
    RSL_INVALID_CONTEXT		= 0x00000000,
    RSL_SURFACE_CONTEXT 	= 0x00000001,
    RSL_DISPLACE_CONTEXT 	= 0x00000002,
    RSL_LIGHT_CONTEXT 		= 0x00000004,
    RSL_VOLUME_CONTEXT 		= 0x00000008,
    RSL_IMAGER_CONTEXT		= 0x00000010,
    RSL_CLASS_CONTEXT		= 0x00000020,
} RSL_ContextType;


// ============================================================================
/// Basic management of shader contexts in RSL.
class VEX_API RSL_ContextManager
{
public:
    /// Get the name token of a context.
    static const char		*getNameFromContextType( RSL_ContextType t );

    /// Returns a context label token for a given type.
    static const char		*getLabelFromContextType( RSL_ContextType t );

    /// Map the context name to a context type.
    static RSL_ContextType	 getContextTypeFromName( const char *name );
};

#endif


