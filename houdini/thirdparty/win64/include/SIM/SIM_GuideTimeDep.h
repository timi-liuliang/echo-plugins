/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GuideTimeDep_h__
#define __SIM_GuideTimeDep_h__

#include "SIM_API.h"
#include "SIM_Guide.h"

/// This SIM_Guide subclass is a container for another type of SIM_Guide
/// which is provided as a template parameter. Each object causes a new
/// SIM_Guide to be generated, and requests are passed on to that contained
/// SIM_Guide.
template<class GuideBase>
class SIM_GuideTimeDep : public GuideBase
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_GuideTimeDep(const SIM_Data *owner)
				    : GuideBase(owner)
				 {
				 }
    /// Constructor which takes an owner, and a flag to pass on to our base.
    /// This flag is used by SIM_GuideShared.
    explicit			 SIM_GuideTimeDep(const SIM_Data *owner,
						  bool displayonce)
				    : GuideBase(owner, displayonce)
				 {
				 }
    /// Destructor for this class.
    virtual			~SIM_GuideTimeDep()
				 {
				 }

protected:
    virtual GU_ConstDetailHandle getGuideGeometrySubclass(
						const SIM_RootData &root,
						UT_DMatrix4 &xform,
						const SIM_Time &t)
				 {
				     if( t != myTime )
				     {
					 this->clear();
					 myTime = t;
				     }

				     return GuideBase::
					    getGuideGeometrySubclass(root,
								     xform,
								     t);
				 }

private:
    SIM_Time			 myTime;
};

#endif

