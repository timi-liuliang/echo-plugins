/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GuidePerObject_h__
#define __SIM_GuidePerObject_h__

#include "SIM_API.h"

#include "SIM_Guide.h"
#include "SIM_RootDataId.h"

#include <UT/UT_Map.h>

/// This SIM_Guide subclass creates a separate guide geometry for each object.
class SIM_API SIM_GuidePerObject : public SIM_Guide
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_GuidePerObject(const SIM_Data *owner);
    /// Destructor for this class.
    virtual			~SIM_GuidePerObject();

protected:
    virtual GU_ConstDetailHandle getGuideGeometrySubclass(
						const SIM_RootData &root,
						UT_DMatrix4 &xform,
						const SIM_Time &t);
    virtual int64		 getMemorySizeSubclass() const;
    virtual void		 clearSubclass();

private:
    UT_Map<SIM_RootDataId, GU_DetailHandle *> myGeometries;
};

#endif

