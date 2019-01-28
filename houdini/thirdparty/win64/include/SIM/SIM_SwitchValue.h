/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SwitchValue_h__
#define __SIM_SwitchValue_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

/// This data type stres a single float value which is used by the Blend
/// Solver to control the interpolation between the sources.
class SIM_API SIM_SwitchValue : public SIM_Data,
				public SIM_OptionsUser
{
public:
    /// Control the translation attribute of the blended data.
    GETSET_DATA_FUNCS_I(SIM_NAME_SWITCHVALUE, SwitchValue);

protected:
    explicit		 SIM_SwitchValue(const SIM_DataFactory *factory);
    virtual		~SIM_SwitchValue();

private:
    static const SIM_DopDescription	*getSwitchValueDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SwitchValue,
			SIM_Data,
			"Switch Value",
			getSwitchValueDopDescription());
};

#endif

