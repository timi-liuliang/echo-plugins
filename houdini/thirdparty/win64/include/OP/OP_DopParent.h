/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_DopParent.h ( SIM Library, C++)
 *
 * COMMENTS:
 *	This provides a bridge up to DOP_Parent for methods interested
 *	in the simulation status but live below SIM.
 */

#ifndef __OP_DopParent__
#define __OP_DopParent__

#include "OP_API.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_VectorTypes.h>

class OP_Node;

class OP_API OP_DopParentMostRecentResolver
{
public:
    virtual ~OP_DopParentMostRecentResolver() {}
    virtual OP_Node *resolve() const = 0;
};

OP_API extern void OPregisterDopParentMostRecentResolver(OP_DopParentMostRecentResolver *resolver);

class OP_API OP_DopParent
{
public:
    OP_DopParent() {}
    virtual ~OP_DopParent() {}

    virtual bool opGetNeedsResimulation() const = 0;
    virtual bool opGetDisableSimulation() const = 0;
    static OP_Node *opGetMostRecentDopOwner();

    /// Checks to see if the dop is in play mode
    virtual bool opIsPlayer() const = 0;

    virtual void setNeedsToCompletelyReset() = 0;

    // Returns global frames, not simulation frames, rounded outwards.
    virtual void opGetDiskCache(UT_IntArray &frames) const = 0;
    virtual void opGetMemoryCache(UT_IntArray &frames) const = 0;
};


#endif // __OP_DopParent__
