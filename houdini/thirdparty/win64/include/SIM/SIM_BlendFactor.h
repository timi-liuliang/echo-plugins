/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_BlendFactor_h__
#define __SIM_BlendFactor_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

/// This data type stres a single float value which is used by the Blend
/// Solver to control the interpolation between the sources.
class SIM_API SIM_BlendFactor : public SIM_Data,
				public SIM_OptionsUser
{
public:
    /// Control the translation attribute of the blended data.
    GETSET_DATA_FUNCS_F(SIM_NAME_BLENDFACTOR, BlendFactor);
    /// Specifies whether the solver should use the last blended value as
    /// its starting point each frame. The alternative is to use the value
    /// calculated just by this solver on the last timestep.
    GETSET_DATA_FUNCS_B(SIM_NAME_USEBLENDEDDATA, UseBlendedData);

protected:
    explicit		 SIM_BlendFactor(const SIM_DataFactory *factory);
    virtual		~SIM_BlendFactor();

private:
    static const SIM_DopDescription	*getBlendFactorDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_BlendFactor,
			SIM_Data,
			"Blend Factor",
			getBlendFactorDopDescription());
};

#endif

