/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_NoiseStandard_h__
#define __SIM_NoiseStandard_h__

#include "SIM_API.h"
#include <UT/UT_Noise.h>
#include "SIM_Noise.h"

class SIM_NoiseResolverStandard;

/// This class defines the interface for noise generation functions.
class SIM_API SIM_NoiseStandard : public SIM_Noise
{
public:
    GETSET_DATA_FUNCS_I(SIM_NAME_SEED, Seed);
    GETSET_DATA_FUNCS_I(SIM_NAME_FRACTALDEPTH, FractalDepth);
    GETSET_DATA_FUNCS_F(SIM_NAME_ROUGHNESS, Roughness);
    GETSET_DATA_FUNCS_F(SIM_NAME_ATTENUATION, Attenuation);
    GETSET_DATA_FUNCS_V3(SIM_NAME_FREQUENCY, Frequency);
    GETSET_DATA_FUNCS_V3(SIM_NAME_OFFSET, Offset);
    GETSET_DATA_FUNCS_E(SIM_NAME_NOISETYPE, NoiseType, UT_Noise::UT_NoiseType);

protected:
    explicit		 SIM_NoiseStandard(const SIM_DataFactory *factory);
    virtual		~SIM_NoiseStandard();

    /// Respond to changes in the noise type.
    virtual void	 optionChangedSubclass(const char *name);

    /// Override this function to return the correct noise extents.
    virtual void	 getNoiseLimitsSubclass(fpreal &min,
						fpreal &max) const;
    /// Override this function to implement a new 1D noise function.
    virtual fpreal	 getNoiseSubclass(const UT_Vector3 &pos) const;
    /// Override this function to implement a new 3D noise function.
    virtual void	 getNoiseSubclass(const UT_Vector3 &pos,
					  UT_Vector3 &noise) const;

    SIM_PropertyResolver *getNoiseResolverSubclass() const;

private:
    static const SIM_DopDescription	*getNoiseStandardDopDescription();

    UT_Noise				 myNoise;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_NoiseStandard,
			SIM_Noise,
			"Noise Field",
			getNoiseStandardDopDescription());

    friend class SIM_NoiseResolverStandard;
};

#endif
