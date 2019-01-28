/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PositionSimple_h__
#define __SIM_PositionSimple_h__

#include "SIM_API.h"
#include "SIM_Position.h"
#include "SIM_OptionsUser.h"

/// This is the simplest implementation for standard positional data.
/// Positional data is not exactly the same as a transform in that it
/// does not permit scales, and the rotation is always applied before
/// the translation.
class SIM_API SIM_PositionSimple : public SIM_Position,
				   public SIM_OptionsUser
{
public:
    /// TODO: These accessors are shadowing the SIM_Position versions.
    /// This is a bad thing as getPivot() will not invoke getPivotSubclass
    /// and anyone trying to chase the call graph will see an apparent
    /// infinite loop.  We need to rename these to RawPosition, I think.

    /// Control the translation attribute of the position.
    GETSET_DATA_FUNCS_V3(SIM_NAME_POSITION, Position);
    /// Control the pivot point which the orientation is applied to.
    GETSET_DATA_FUNCS_V3(SIM_NAME_PIVOT, Pivot);
    /// Control the orientation attribute of the position.
    GETSET_DATA_FUNCS_Q(SIM_NAME_ORIENTATION, Orientation);

protected:
    explicit		 SIM_PositionSimple(const SIM_DataFactory *factory);
    virtual		~SIM_PositionSimple();

    /// Override this function so that we can understand an option
    /// called rotation. Although this attribute isn't supported directly,
    /// it can be reinterpreted into the orientation option.
    virtual void	 optionChangedSubclass(const char *name);

    /// Gets a transform matrix from our position and rotation data.
    virtual void	 getTransformSubclass(UT_DMatrix4 &xform) const;

    /// Gets the inverse transform matrix from our position and rotation.
    /// Because we build our transform matrices just in time, this is
    /// faster than getTransform() & invert().
    virtual void	 getInverseTransformSubclass(UT_DMatrix4 &xform) const;

    /// Gets a rotation matrix from our position data.
    virtual void	 getOrientationSubclass(UT_Quaternion &q) const;

    /// Gets a translation vector from our position data.
    virtual void	 getPositionSubclass(UT_Vector3 &t) const;

    /// Gets pivot info from our position data.
    virtual void	 getPivotSubclass(UT_Vector3 &p) const;

    /// Transforms a point into worldspace from object space.
    /// This is the same as multiplying by getTransform()
    virtual UT_Vector3	 selfToWorldSubclass(const UT_Vector3 &p) const;

    /// Transforms a point into object space from world space.
    /// This is the same as multiplying by getInverseTransform()	
    virtual UT_Vector3	 worldToSelfSubclass(const UT_Vector3 &p) const;

    /// Transforms a vector into worldspace from object space.
    /// This is the same as multiplying by getTransform()
    virtual UT_Vector3	 selfToWorldVectorSubclass(const UT_Vector3 &p) const;

    /// Transforms a vector into object space from world space.
    /// This is the same as multiplying by getInverseTransform()	
    virtual UT_Vector3	 worldToSelfVectorSubclass(const UT_Vector3 &p) const;

    /// Interpolation of positions can't be done in a componentwise
    /// fashion.  If pivot is changing over time it is important
    /// that we adjust the rotation/translation to account for it.
    virtual void	 interpolateSubclass(const SIM_Data *source1,
					     const SIM_Data *source2,
					     fpreal interp);

private:
    static const SIM_DopDescription	*getPositionSimpleDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PositionSimple,
			SIM_Position,
			"Position",
			getPositionSimpleDopDescription());
};

#endif

