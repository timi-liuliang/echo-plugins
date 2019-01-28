/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjPointPos_h__
#define __SIM_ConAnchorObjPointPos_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjSpatial.h"
#include "SIM_Geometry.h"

class SIM_API SIM_ConAnchorObjPointPos : public SIM_ConAnchorObjSpatial
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_GEOMETRY, GeometryName);
    
    /// Store the point number if we are anchored to a geometry point.
    GA_Index		 getPoint() const;
    void		 setPoint(GA_Index ptnum);

protected:
    explicit		 SIM_ConAnchorObjPointPos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointPos();

    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);
    virtual UT_Vector3	 getPositionSubclass(const SIM_Time &time) const;
    virtual UT_Vector3	 getVelocitySubclass(const SIM_Time &time) const;

    virtual int		 getPointSubclass() const = 0;
    virtual void	 setPointSubclass(int ptnum) = 0;

    const SIM_Geometry 	*getGeometry(const SIM_Object *object) const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorObjPointPos, SIM_ConAnchorObjSpatial);
};

#endif
