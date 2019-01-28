/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConRelNone_h__
#define __SIM_ConRelNone_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_ConRel.h"

/// The simplest type of constraint: forcing the geometry to exactly match
/// the anchor position and/or orientation.
class SIM_API SIM_ConRelNone : public SIM_ConRel
{
protected:
    explicit		 SIM_ConRelNone(const SIM_DataFactory *factory);
    virtual		~SIM_ConRelNone();

    virtual void	 buildConRelGuide(
				const GU_DetailHandle &gdh,
				const SIM_Options &options,
				const UT_Vector3 &anchor_pos_1,
				const UT_Vector3 &anchor_pos_2,
				const UT_Quaternion &anchor_rot_1,
				const UT_Quaternion &anchor_rot_2,
                                const ConRelData &c_data) const;
    virtual void	 setStateDistanceSubclass(fpreal distance);
    virtual fpreal	 getStateDistanceSubclass() const;

private:
    static const SIM_DopDescription	*getConRelNoneDopDescription();

    fpreal		 myStateDistance;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConRelNone,
			SIM_ConRel,
			"No Constraint Relationship",
			getConRelNoneDopDescription());
};

#endif
