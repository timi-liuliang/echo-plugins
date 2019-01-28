/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GuideStatic_h__
#define __SIM_GuideStatic_h__

#include "SIM_API.h"
#include <UT/UT_SymbolTable.h>
#include "SIM_RootDataId.h"
#include "SIM_Guide.h"

/// This SIM_Guide subclass has one piece of geometry for all data of
/// a given type. The geometry is built in the normal way the first time
/// we are asked for geometry for each data type. The transform is
/// recalculated for each request.
class SIM_API SIM_GuideStatic : public SIM_Guide
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_GuideStatic(const SIM_Data *owner,
						 bool displayonly);
    /// Destructor for this class.
    virtual			~SIM_GuideStatic();

protected:
    virtual GU_ConstDetailHandle getGuideGeometrySubclass(
						const SIM_RootData &root,
						UT_DMatrix4 &xform,
						const SIM_Time &t);
    virtual void		 clearSubclass();

private:
    SIM_RootDataId		 myCachedObjectId;
    bool			 myDisplayOnce;
    static UT_SymbolMap<GU_DetailHandle	*> theDetails;
};

#endif

