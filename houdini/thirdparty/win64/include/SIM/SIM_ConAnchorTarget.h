/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ConAnchorTarget.h ( SIM Library, C++)
 *
 * COMMENTS:
 *     If SIM_ConAnchorTarget is used as a goal anchor,
 *     then the constrained object is made to follow the point attributes
 *     targetP and targetv.
 */

#ifndef __SIM_ConAnchorTarget__
#define __SIM_ConAnchorTarget__

#include "SIM_API.h"
#include "SIM_ConAnchor.h"

class SIM_API SIM_ConAnchorTarget : public SIM_ConAnchor
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_OBJECT, ObjectName);

protected:
    explicit SIM_ConAnchorTarget(const SIM_DataFactory *f);
    virtual ~SIM_ConAnchorTarget();

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorTarget,
			SIM_ConAnchor,
			"Anchor: Target",
			getDopDescription());
};

#endif

