/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointNumPos_h__
#define __SIM_ConAnchorObjPointNumPos_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjPointPos.h"

/// A concrete constraint anchor: tie the constraint to a point on a
/// simulated object.
class SIM_API SIM_ConAnchorObjPointNumPos : public SIM_ConAnchorObjPointPos
{
public:
    /// Store the point number if we are anchored to a geometry point.
    GETSET_DATA_FUNCS_I(SIM_NAME_POINTNUMBER, PointNumber);

protected:
    explicit		 SIM_ConAnchorObjPointNumPos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointNumPos();

    virtual int		 getPointSubclass() const;
    virtual void	 setPointSubclass(int ptnum);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointNumPos,
			SIM_ConAnchorObjPointPos,
			"Anchor: Object Point Number Position",
			getDopDescription());
};

#endif
