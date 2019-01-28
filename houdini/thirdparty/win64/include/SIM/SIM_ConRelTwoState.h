/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConRelTwoState_h__
#define __SIM_ConRelTwoState_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_ConRel.h"

/// The simplest type of constraint: forcing the geometry to exactly match
/// the anchor position and/or orientation.
class SIM_API SIM_ConRelTwoState : public SIM_ConRel,
				   public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_F(SIM_NAME_MINSTATETIME, MinStateTime);
    GETSET_DATA_FUNCS_F(SIM_NAME_LASTTRANSITIONTIME, LastTransitionTime);
    GETSET_DATA_FUNCS_I(SIM_NAME_CURRENTSTATE, CurrentState);

    GETSET_DATA_FUNCS_I(SIM_NAME_FORCECOMPARE1, ForceCompare1);
    GETSET_DATA_FUNCS_F(SIM_NAME_FORCEVALUE1, ForceValue1);
    GETSET_DATA_FUNCS_I(SIM_NAME_DISTANCECOMPARE1, DistanceCompare1);
    GETSET_DATA_FUNCS_F(SIM_NAME_DISTANCEVALUE1, DistanceValue1);
    GETSET_DATA_FUNCS_I(SIM_NAME_FORCECOMPARE0, ForceCompare0);
    GETSET_DATA_FUNCS_F(SIM_NAME_FORCEVALUE0, ForceValue0);
    GETSET_DATA_FUNCS_I(SIM_NAME_DISTANCECOMPARE0, DistanceCompare0);
    GETSET_DATA_FUNCS_F(SIM_NAME_DISTANCEVALUE0, DistanceValue0);

protected:
    explicit		 SIM_ConRelTwoState(const SIM_DataFactory *factory);
    virtual		~SIM_ConRelTwoState();

    virtual SIM_ConRel	*getSubConRelSubclass();
    virtual const SIM_ConRel
			*getConstSubConRelSubclass() const;
    virtual fpreal	 getStateForceSubclass() const;
    virtual fpreal	 getStateDistanceSubclass() const;
    virtual void	 makeStateTransitionSubclass(const SIM_Time &time);

private:
    static const SIM_DopDescription	*getConRelTwoStateDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConRelTwoState,
			SIM_ConRel,
			"Two State Constraint Relationship",
			getConRelTwoStateDopDescription());
};

#endif
