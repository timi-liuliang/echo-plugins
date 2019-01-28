/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointNumRot_h__
#define __SIM_ConAnchorObjPointNumRot_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjPointRot.h"

/// A concrete constraint anchor: tie the constraint to a point on a
/// simulated object.
class SIM_API SIM_ConAnchorObjPointNumRot : public SIM_ConAnchorObjPointRot
{
public:
    /// Store the point number if we are anchored to a geometry point.
    GETSET_DATA_FUNCS_I(SIM_NAME_POINTNUMBER, PointNumber);

protected:
    explicit		 SIM_ConAnchorObjPointNumRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointNumRot();

    virtual int		 getPointSubclass() const;
    virtual void	 setPointSubclass(int ptnum);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointNumRot,
			SIM_ConAnchorObjPointRot,
			"Anchor: Object Point Number Rotation",
			getDopDescription());
};

#endif
