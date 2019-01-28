/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointIdPos_h__
#define __SIM_ConAnchorObjPointIdPos_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjPointPos.h"

class SIM_API SIM_ConAnchorObjPointIdPos : public SIM_ConAnchorObjPointPos
{
public:
    /// Store the point number if we are anchored to a geometry point.
    GETSET_DATA_FUNCS_I(SIM_NAME_POINTID, PointId);
    /// The name of the attribute used to identify the point we want.
    GETSET_DATA_FUNCS_S(SIM_NAME_IDATTRIBNAME, IdAttribName);

protected:
    explicit		 SIM_ConAnchorObjPointIdPos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointIdPos();

    virtual int		 getPointSubclass() const;
    virtual void	 setPointSubclass(int ptnum);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointIdPos,
			SIM_ConAnchorObjPointPos,
			"Anchor: Object Point Id Position",
			getDopDescription());
};

#endif
