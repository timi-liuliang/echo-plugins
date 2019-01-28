/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SurfaceCollisionParms_h__
#define __SIM_SurfaceCollisionParms_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

/// This class contains physical attributes that apply to all objects.
class SIM_API SIM_SurfaceCollisionParms : public SIM_Data,
				          public SIM_OptionsUser
{
public:
    /// Collision padding around around the polygons
    GETSET_DATA_FUNCS_F(SIM_NAME_COLLISION_RADIUS, Thickness)

    // TODO: option for one-sided or two-sided collision?
    // TODO: option for indicating that object is closed?

protected:
    explicit		 SIM_SurfaceCollisionParms(const SIM_DataFactory *factory);
    virtual		~SIM_SurfaceCollisionParms();

private:
    static const SIM_DopDescription	*getSurfaceCollisionParmsDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SurfaceCollisionParms,
			SIM_Data,
			"Surface Collision Parameters",
			getSurfaceCollisionParmsDopDescription());
};

#endif
