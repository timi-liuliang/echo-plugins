/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointIdRot_h__
#define __SIM_ConAnchorObjPointIdRot_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjPointRot.h"

class SIM_API SIM_ConAnchorObjPointIdRot : public SIM_ConAnchorObjPointRot
{
public:
    /// Store the point number if we are anchored to a geometry point.
    GETSET_DATA_FUNCS_I(SIM_NAME_POINTID, PointId);
    /// The name of the attribute used to identify the point we want.
    GETSET_DATA_FUNCS_S(SIM_NAME_IDATTRIBNAME, IdAttribName);

protected:
    explicit		 SIM_ConAnchorObjPointIdRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointIdRot();

    virtual int		 getPointSubclass() const;
    virtual void	 setPointSubclass(int ptnum);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointIdRot,
			SIM_ConAnchorObjPointRot,
			"Anchor: Object Point Id Rotation",
			getDopDescription());
};

#endif
