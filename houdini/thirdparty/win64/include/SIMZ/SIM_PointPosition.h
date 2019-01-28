/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PointPosition_h__
#define __SIM_PointPosition_h__

#include <SIM/SIM_Motion.h>
#include "SIMZ_Utils.h"

class SIM_PointInfo;

/// This SIM_Motion implementation gets position information from a point
/// in a SOP. The velocity is calculated automatically.
class SIMZ_API SIM_PointPosition : public SIM_Motion
{
public:
    /// Access functions for our specific data.
    GETSET_DATA_FUNCS_F(SIM_NAME_TIME, Time);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOPPATH, SopPath);
    GETSET_DATA_FUNCS_B(SIM_NAME_USETRANSFORM, UseTransform);
    GETSET_DATA_FUNCS_S(SIM_NAME_POINTGROUP, PointGroup);
    GETSET_DATA_FUNCS_I(SIM_NAME_POINTINDEX, PointIndex);
    GETSET_DATA_FUNCS_S(SIM_NAME_EXTRAATTRIBUTES, ExtraAttributes);
    GETSET_DATA_FUNCS_B(SIM_NAME_ROTATETONORMAL, RotateToNormal);

    /// Force this data to update its position information from it's
    /// point information. This function is called automatically by
    /// setParametersSubclass(), but not when calling the set functions
    /// listed above.
    void		 calculateValuesFromPoint(const UT_Vector3 &prevpos,
						  const UT_Quaternion &prevrot,
						  fpreal prevtime);

protected:
    explicit		 SIM_PointPosition(const SIM_DataFactory *factory);
    virtual		~SIM_PointPosition();

    virtual void	 initializeSubclass();
    virtual void	 setParametersSubclass(const SIM_Options &parms);

private:
    // Keeps track of whether we have had our parms set yet or not.
    GETSET_DATA_FUNCS_B(SIM_NAME_HASHADPARMSSET, HasHadParmsSet);

    void		 getPointInformation(SIM_PointInfo &info);
    static const SIM_DopDescription	*getPointPositionDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PointPosition,
			SIM_Motion,
			"Point Position",
			getPointPositionDopDescription());
};

#endif

