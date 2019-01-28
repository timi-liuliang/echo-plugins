/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ActiveValue_h__
#define __SIM_ActiveValue_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

/// This data type stres a single float value which is used by the Blend
/// Solver to control the interpolation between the sources.
class SIM_API SIM_ActiveValue : public SIM_Data,
				public SIM_OptionsUser
{
public:
    /// Control the translation attribute of the blended data.
    GETSET_DATA_FUNCS_F(SIM_NAME_ACTIVE, Active);

protected:
    explicit		 SIM_ActiveValue(const SIM_DataFactory *factory);
    virtual		~SIM_ActiveValue();

private:
    static const SIM_DopDescription	*getActiveValueDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ActiveValue,
			SIM_Data,
			"Active Value",
			getActiveValueDopDescription());
};

#endif

