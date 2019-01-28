/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_RslResolver.h ( VEX Library, C++)
 *
 * COMMENTS:	Resolver for Rsl (RenderMan Shader Language).
 *
 *	Since shader code can't be embedded in a RIB stream, the way this
 *	resolver works is that:
 *	  - The resolver will save the object code to a pre-defined directory.
 *	  - The shader name will be changed to point to the object code in
 *	    question.
 */

#ifndef __VEX_RslResolver__
#define __VEX_RslResolver__

#include "VEX_API.h"
class UT_String;

// ============================================================================
class VEX_API VEX_RibResolver
{
public:
    /// @{ Specifies where the temporary source code file is created
    ///    and where the compiled object file is saved.
    static void		setCachePath(const char *path);
    static const char  *getCachePath();
    /// @}

    // Helper that simply replaces the shader name with the appropriate path.
    static void		patchShader(UT_String &shader, const char *path);

protected:
    virtual	       ~VEX_RibResolver();
    virtual bool	canResolve(const char *shader) = 0;
    virtual bool	doResolve(UT_String &shader, int context_type) = 0;
};

// ============================================================================
class VEX_API VEX_RslResolver : public VEX_RibResolver
{
public:
    static bool		canResolveShader(UT_String &shader);
    static bool		resolveShader(UT_String &shader, int context_type);

protected:
				 VEX_RslResolver();
    virtual			~VEX_RslResolver();
    static VEX_RslResolver	*getRslResolver(const char *shader);
};

// ============================================================================
// TODO: factor out VEX_RslResolver and VEX_OslResolver into one class without
//	 static list of resolvers
class VEX_API VEX_OslResolver : public VEX_RibResolver
{
public:
    static bool		canResolveShader(UT_String &shader);
    static bool		resolveShader(UT_String &shader, int context_type);

protected:
				 VEX_OslResolver();
    virtual			~VEX_OslResolver();
    static VEX_OslResolver	*getOslResolver(const char *shader);
};
#endif

