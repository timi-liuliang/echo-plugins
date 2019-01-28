/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConRelHard_h__
#define __SIM_ConRelHard_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_ConRel.h"

/// The simplest type of constraint: forcing the geometry to exactly match
/// the anchor position and/or orientation.
class SIM_API SIM_ConRelHard : public SIM_ConRel,
                               public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_F(SIM_NAME_CFM, Cfm);
    GETSET_DATA_FUNCS_F(SIM_NAME_ERP, Erp);
    GETSET_DATA_FUNCS_F(SIM_NAME_RESTLENGTH, RestLength);
    GETSET_DATA_FUNCS_I(SIM_NAME_NUMITERATIONS, NumIterations);
    GETSET_DATA_FUNCS_I(SIM_NAME_DISABLECOLLISIONS, DisableCollisions);

    GETSET_DATA_FUNCS_V3(SIM_NAME_TARGETW, TargetW);
    GETSET_DATA_FUNCS_V3(SIM_NAME_MAXANGULARIMPULSE, MaxImpulse);
    GETSET_DATA_FUNCS_I(SIM_NAME_NUMANGULARMOTORS, NumAngularMotors);
    GETSET_DATA_FUNCS_V3(SIM_NAME_AXIS1, Axis1);
    GETSET_DATA_FUNCS_V3(SIM_NAME_AXIS2, Axis2);

protected:
    explicit		 SIM_ConRelHard(const SIM_DataFactory *factory);
    virtual		~SIM_ConRelHard();

    virtual void	 setStateForceSubclass(fpreal force);
    virtual fpreal	 getStateForceSubclass() const;

    virtual void	 buildConRelGuide(
				const GU_DetailHandle &gdh,
				const SIM_Options &options,
				const UT_Vector3 &anchor_pos_1,
				const UT_Vector3 &anchor_pos_2,
				const UT_Quaternion &anchor_rot_1,
				const UT_Quaternion &anchor_rot_2,
                                const ConRelData &c_options) const;

private:
    static void syncNodeVersion(OP_Node *node, const char *old_version,
                                const char *new_version, bool *node_deleted);
    static bool updateParmsFlags(OP_Node *node);
    static const SIM_DopDescription	*getConRelHardDopDescription();

    fpreal		 myStateForce;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConRelHard,
			SIM_ConRel,
			"Hard Constraint Relationship",
			getConRelHardDopDescription());
};

#endif
