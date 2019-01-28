/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjSpatial_h__
#define __SIM_ConAnchorObjSpatial_h__

#include "SIM_API.h"
#include <UT/UT_Vector3.h>
#include "SIM_ConAnchorSpatial.h"

class SIM_API SIM_ConAnchorObjSpatial : public SIM_ConAnchorSpatial
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_OBJECT, ObjectName);
    /// Object-space offset.
    GETSET_DATA_FUNCS_V3(SIM_NAME_OFFSET, OffsetInput);

    /// This value represents the desired world-space position of the
    /// constraint. Every spatial constraint must be able to initialize
    /// itself using this world space position.
    GETSET_DATA_FUNCS_V3(SIM_NAME_WORLDSPACEPOS, WorldSpacePosition);
    /// This value must be true to force the constraint to initialize
    /// itself from the provided world space position. Otherwise the
    /// constraint-specific parameters are used directly.
    GETSET_DATA_FUNCS_B(SIM_NAME_USEWORLDSPACEPOS, UseWorldSpacePosition);

    /// Overridable get/set for Offset
    const UT_Vector3	 getOffset() const;
    void		 setOffset(const UT_Vector3 &vector);

protected:
    explicit		 SIM_ConAnchorObjSpatial(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjSpatial();

    virtual bool	 getNeedsInitSubclass() const;
    virtual void	 initAnchorSubclass(const SIM_Object *object);
    virtual bool	 getObjectSpecificationSubclass(UT_String &str) const;

    void		 initParmsFromWorldSpacePosition(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object);

    virtual void	 initParmsFromWorldSpacePositionSubclass(
					const UT_Vector3 &worldspacepos,
					const SIM_Object *object) = 0;

    virtual const UT_Vector3	getOffsetSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorObjSpatial, SIM_ConAnchorSpatial);
};

#endif
