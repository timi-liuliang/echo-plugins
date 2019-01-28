/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GuideShared_h__
#define __SIM_GuideShared_h__

#include "SIM_API.h"
#include "SIM_Time.h"
#include "SIM_RootDataId.h"
#include "SIM_Guide.h"

/// This SIM_Guide subclass caches the geometry and transform from the first
/// object it is asked to build for. Subsequent requests for other objects
/// return null geometry. This way this guide geometry will only be rendered
/// once, not once for each object it is attached to.
class SIM_API SIM_GuideShared : public SIM_Guide
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_GuideShared(const SIM_Data *owner,
						 bool displayonce);
    /// Destructor for this class.
    virtual			~SIM_GuideShared();

protected:
    virtual GU_ConstDetailHandle getGuideGeometrySubclass(
						const SIM_RootData &root,
						UT_DMatrix4 &xform,
						const SIM_Time &t);
    virtual int64		 getMemorySizeSubclass() const;
    virtual void		 clearSubclass();

private:
    GU_DetailHandle		 myDetailHandle;
    SIM_RootDataId		 myCachedObjectId;
    const bool			 myDisplayOnce;
};

#endif

