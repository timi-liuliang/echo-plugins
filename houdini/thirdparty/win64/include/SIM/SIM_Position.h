/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Position_h__
#define __SIM_Position_h__

#include "SIM_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_VectorTypes.h>
#include "SIM_DataUtils.h"


/// This is the abstract base class for standard positional data.
/// Positional data is not exactly the same as a transform in that it
/// does not permit scales, and the rotation is always applied before
/// the translation.
class SIM_API SIM_Position : public SIM_Data
{
public:
    /// Gets a transform matrix from our position and rotation data.
    /// This function just calls getTransformSubclass().
    void		 getTransform(UT_DMatrix4 &xform) const;

    /// Gets the inverse transform matrix from our position and rotation.
    /// This function just calls getInverseTransformSubclass().
    void		 getInverseTransform(UT_DMatrix4 &xform) const;

    /// Gets a matrix representing the rotation component of the position.
    /// This function just calls getRotationMatrixSubclass().
    void		 getOrientation(UT_Quaternion &q) const;

    /// Gets a vector representing the translation component of the position.
    /// This function just calls getPositionSubclass().
    void		 getPosition(UT_Vector3 &t) const;

    /// Gets a vector representing the pivot component of the position.
    /// This function just calls getPivotSubclass().
    void		 getPivot(UT_Vector3 &p) const;

    /// Transforms a point into worldspace from object space.
    /// This function just calls selfToWorldSubclass().
    UT_Vector3		 selfToWorld(const UT_Vector3 &p) const;

    /// Transforms a point into object space from world space.
    /// This function just calls worldToSelfSubclass().
    UT_Vector3		 worldToSelf(const UT_Vector3 &p) const;

    /// Transforms a vector into worldspace from object space.
    /// This function just calls selfToWorldVectorSubclass().
    UT_Vector3		 selfToWorldVector(const UT_Vector3 &p) const;

    /// Transforms a vector into object space from world space.
    /// This function just calls worldToSelfVectorSubclass().
    UT_Vector3		 worldToSelfVector(const UT_Vector3 &p) const;

protected:
    explicit		 SIM_Position(const SIM_DataFactory *factory);
    virtual		~SIM_Position();

    /// This implementation returns the identity matrix.
    virtual void	 getTransformSubclass(UT_DMatrix4 &xform) const;

    /// This implementation calls getTransform and inverts it.
    virtual void	 getInverseTransformSubclass(UT_DMatrix4 &xform) const;

    /// This implementation calls getTransform and extracts the rotations.
    virtual void	 getOrientationSubclass(UT_Quaternion &q) const;

    /// This implementation calls getTransform and extracts the translates.
    virtual void	 getPositionSubclass(UT_Vector3 &t) const;

    /// This implementation returns (0, 0, 0) as the pivot.
    virtual void	 getPivotSubclass(UT_Vector3 &p) const;

    /// This implementation calls getTransform and does the multiply.
    virtual UT_Vector3	 selfToWorldSubclass(const UT_Vector3 &p) const;

    /// This implementation calls getInverseTransform and does the multiply.
    virtual UT_Vector3	 worldToSelfSubclass(const UT_Vector3 &p) const;

    /// This implementation calls getTransform and does the multiply.
    virtual UT_Vector3	 selfToWorldVectorSubclass(const UT_Vector3 &p) const;

    /// This implementation calls getInverseTransform and does the multiply.
    virtual UT_Vector3	 worldToSelfVectorSubclass(const UT_Vector3 &p) const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Position, SIM_Data);
};

#endif

