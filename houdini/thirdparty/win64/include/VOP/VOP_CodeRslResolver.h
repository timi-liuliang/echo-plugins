/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_CodeRslResolver.h ( VOP Library, C++)
 *
 * COMMENTS:	Rsl code resolver
 */

#ifndef __VOP_CodeRslResolver__
#define __VOP_CodeRslResolver__

#include "VOP_API.h"
#include <VEX/VEX_RslResolver.h>

// ============================================================================
class VOP_API VOP_CodeRslResolver : public VEX_RslResolver
{
public:
protected:
    virtual bool	canResolve(const char *shader);
    virtual bool	doResolve(UT_String &shader, int context_type);
};

// ============================================================================
class VOP_API VOP_CodeOslResolver : public VEX_OslResolver
{
public:
protected:
    virtual bool	canResolve(const char *shader);
    virtual bool	doResolve(UT_String &shader, int context_type);
};

#endif

