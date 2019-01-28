/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Wavelets.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Wavelets__
#define __GAS_Wavelets__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_Wavelet.h>

class SIM_RawField;

class GAS_API GAS_Wavelets : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDSOURCE, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, OutputName);
    GET_DATA_FUNC_I("mode", Mode);
    GET_DATA_FUNC_I("wavelet", WaveletRaw);
    GET_DATA_FUNC_I("level", Level);
    GET_DATA_FUNC_I("component", Component);
    GET_DATA_FUNC_B("limitpasses", LimitPasses);
    GET_DATA_FUNC_I("maxpasses", MaxPasses);

    enum MODE_NAMES {
	MODE_TRANSFORM,
	MODE_INVERT,
	MODE_PSD,
	MODE_COMPONENT,
	NUM_MODES
    };

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_Wavelets(const SIM_DataFactory *factory);
    virtual		~GAS_Wavelets();

    /// Applies a force to the velocity field proportional
    /// to the vorticle values.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);
    void		 applyWavelets(SIM_Object *obj,
				SIM_RawField *field,
				SIM_RawField *coeff);

    UT_Wavelet::WAVELET_NAMES	getWaveletType();

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Wavelets,
			GAS_SubSolver,
			"Gas Wavelets",
			getDopDescription());
};

#endif

