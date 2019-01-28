/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPrimPos_h__
#define __SIM_ConAnchorObjPrimPos_h__

#include "SIM_API.h"
#include <UT/UT_Vector2.h>
#include "SIM_ConAnchorObjSpatial.h"

class SIM_API SIM_ConAnchorObjPrimPos : public SIM_ConAnchorObjSpatial
{
public:
    /// Store the primitive number if we are anchored to a geometry primitive.
    GETSET_DATA_FUNCS_I(SIM_NAME_PRIMITIVE, Primitive);
    /// (u,v) position within the primitive
    GETSET_DATA_FUNCS_UV(SIM_NAME_PRIMUV, PrimUV);

protected:
    explicit		 SIM_ConAnchorObjPrimPos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPrimPos();

    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);
    virtual UT_Vector3	 getPositionSubclass(const SIM_Time &time) const;
    virtual UT_Vector3	 getVelocitySubclass(const SIM_Time &time) const;
    
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPrimPos,
			SIM_ConAnchorObjSpatial,
			"Anchor: Object Primitive Position",
			getDopDescription());
};

#endif
