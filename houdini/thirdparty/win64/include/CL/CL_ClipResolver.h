/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_ClipResolver.h (CL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CL_CLIPRESOLVER_H_INCLUDED__
#define __CL_CLIPRESOLVER_H_INCLUDED__

#include "CL_API.h"
#include "CL_Clip.h"

#include <UT/UT_Functor.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>

/// Attempt to resolve the given path to a clip represented by a CHOP, whose
/// unique id will be returned in op_id. If not represented by a node, then
/// op_id will be negative and NULL returned. Since this may cause CHOP
/// cooking, so you should run this from a UT_TaskArena::lockedExecute() using
/// the appropriate lock.  If cwd_op_id is non-negative, then it will be used
/// to resolve relative paths. Otherwise, OPgetDirector()->getCwd() gets used.
CL_API const CL_Clip*	    CLresolveClip(
				int& op_id,
				const UT_StringRef& path,
				const int cwd_op_id = -1);

/// Provides a helper for resolving CL_Clip objects given a path. This object
/// registers itself on construction and deregisters upon destruction in an
/// non-thread-safe fashion.
class CL_API CL_ClipResolverHelper
{
public:
			    CL_ClipResolverHelper();
    virtual		    ~CL_ClipResolverHelper();

    /// Given a path to a clip resolve it to a loaded clip. If lock is given,
    /// then it will be resolved via a UT_TaskArena::lockedExecute() using it.
    virtual const CL_Clip*  resolveClip(
				int& op_id,
				const UT_StringRef& path,
				const int source_op_id) const = 0;

private:
    int			    myId;
};

#endif // __CL_CLIPRESOLVER_H_INCLUDED__
