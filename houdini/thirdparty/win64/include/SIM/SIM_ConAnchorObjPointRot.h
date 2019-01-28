/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointRot_h__
#define __SIM_ConAnchorObjPointRot_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjRotational.h"
#include "SIM_Geometry.h"

class SIM_API SIM_ConAnchorObjPointRot : public SIM_ConAnchorObjRotational
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_GEOMETRY, GeometryName);
    GETSET_DATA_FUNCS_S(SIM_NAME_ORIENTATTRIBNAME, OrientAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_UPATTRIBNAME, UpAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_OUTATTRIBNAME, OutAttribName);

    int			 getPoint() const;
    void		 setPoint(int ptnum);

protected:
    explicit		 SIM_ConAnchorObjPointRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointRot();

    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);
    virtual UT_Vector3		 getGuidePositionSubclass(
					const SIM_Time &time) const;
    virtual UT_Quaternion	 getOrientationSubclass(
					const SIM_Time &time) const;
    virtual UT_Vector3		 getAngularVelocitySubclass(
					const SIM_Time &time) const;

    virtual int		 getPointSubclass() const = 0;
    virtual void	 setPointSubclass(int ptnum) = 0;

    const SIM_Geometry	*getGeometry(const SIM_Object *object) const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorObjPointRot, SIM_ConAnchorObjRotational);
};

#endif
