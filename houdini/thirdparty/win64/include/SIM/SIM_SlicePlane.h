/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SlicePlane_h__
#define __SIM_SlicePlane_h__

#include "SIM_API.h"
#include "SIM_Slice.h"
#include "SIM_OptionsUser.h"

class SIM_API SIM_SlicePlane : public SIM_Slice,
			       public SIM_OptionsUser
{
public:
    /// Control the force attribute.
    GETSET_DATA_FUNCS_V3("dir", SliceDirection);
    /// Control the position attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_POSITION, SlicePosition);

protected:
    explicit		 SIM_SlicePlane(const SIM_DataFactory *factory);
    virtual		~SIM_SlicePlane();

    /// Returns the distance to the slice's boundary.  If the point
    /// is inside the slice this is a negative distance, else positive.
    /// This is what must be overridden to make the slice do something
    /// interesting
    virtual fpreal	 getSliceBoundaryDistSubclass(const UT_Vector3 &pos) const;

    /// Which side of our parent we are on
    virtual UT_Vector3	 getCenterSubclass() const { return myPos; }

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    /// We override this method to create the fan geometry.
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

    /// Update our cached position and size so we can store pre-normalized
    /// values to speed up detection
    virtual void	 optionChangedSubclass(const char *name);

private:
    static const SIM_DopDescription	*getSlicePlaneDopDescription();

    UT_Vector3		myDir, myPos;
    fpreal		myOffset;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SlicePlane,
			SIM_Slice,
			"Slice by Plane",
			getSlicePlaneDopDescription());
};

#endif

