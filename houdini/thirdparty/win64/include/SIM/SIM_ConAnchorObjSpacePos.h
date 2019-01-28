/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjSpacePos_h__
#define __SIM_ConAnchorObjSpacePos_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjSpatial.h"

class SIM_API SIM_ConAnchorObjSpacePos : public SIM_ConAnchorObjSpatial
{
protected:
    explicit		 SIM_ConAnchorObjSpacePos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjSpacePos();

    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);
    virtual UT_Vector3	 getPositionSubclass(const SIM_Time &time) const;
    virtual UT_Vector3	 getVelocitySubclass(const SIM_Time &time) const;
    
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjSpacePos,
			SIM_ConAnchorObjSpatial,
			"Anchor: Object Space Position",
			getDopDescription());
};

#endif
