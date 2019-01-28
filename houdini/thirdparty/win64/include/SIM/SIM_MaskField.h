/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_MaskField_h__
#define __SIM_MaskField_h__

#include "SIM_API.h"
#include <UT/UT_Noise.h>
#include "SIM_Noise.h"

/// This class defines the interface for field mask functions.
class SIM_API SIM_MaskField : public SIM_Noise
{
public:
    GETSET_DATA_FUNCS_B(SIM_NAME_TREATASSDF, TreatAsSDF);
    GETSET_DATA_FUNCS_B(SIM_NAME_INVERT, InvertSign);
    GETSET_DATA_FUNCS_F(SIM_NAME_OFFSET, Offset);
    GETSET_DATA_FUNCS_F(SIM_NAME_SCALE, Scale);
    /// The maximum distance we are allowed to search for a point.
    GETSET_DATA_FUNCS_F(SIM_NAME_MAXDISTANCE, MaxDistance);

protected:
    explicit		 SIM_MaskField(const SIM_DataFactory *factory);
    virtual		~SIM_MaskField();

    /// Override this function to return the actual bounds of the noise.
    virtual void	 getNoiseLimitsSubclass(fpreal &min,
						fpreal &max) const;
    /// Override this function to implement a new 1D noise function.
    virtual fpreal	 getNoiseSubclass(const UT_Vector3 &pos) const;
    /// Override this function to implement a new 3D noise function.
    virtual void	 getNoiseSubclass(const UT_Vector3 &pos,
					  UT_Vector3 &noise) const;

private:
    static const SIM_DopDescription	*getMaskFieldDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_MaskField,
			SIM_Noise,
			"Mask Field",
			getMaskFieldDopDescription());
};

#endif
