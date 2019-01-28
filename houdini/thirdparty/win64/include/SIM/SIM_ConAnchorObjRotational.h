/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjRotational_h__
#define __SIM_ConAnchorObjRotational_h__

#include "SIM_API.h"
#include "SIM_ConAnchorRotational.h"

/// Base class for constraint anchors that specify a rotation in the space
/// of a particular object.
class SIM_API SIM_ConAnchorObjRotational : public SIM_ConAnchorRotational
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_OBJECT, ObjectName);

    /// This value must be true to force the constraint to initialize
    /// itself from the provided world space position. Otherwise the
    /// constraint-specific parameters are used directly.
    GETSET_DATA_FUNCS_B(SIM_NAME_USEWORLDSPACEPOS, UseWorldSpacePosition);

    /// Retrieve the world-space position for the guide geometry
    GETSET_DATA_FUNCS_V3(SIM_NAME_WORLDSPACEPOS, WorldSpacePosition);


protected:
    explicit		 SIM_ConAnchorObjRotational(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjRotational();

    virtual bool	 getNeedsInitSubclass() const;
    virtual void	 initAnchorSubclass(const SIM_Object *object);
    virtual bool	 getObjectSpecificationSubclass(UT_String &str) const;

    void		 initParmsFromWorldSpacePosition(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);
    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object) = 0;

    virtual UT_Quaternion getOrientationSubclass(const SIM_Time &time) const;
    virtual UT_Vector3	 getAngularVelocitySubclass(const SIM_Time &time) const;
    
private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorObjRotational, SIM_ConAnchorRotational);
};

#endif

