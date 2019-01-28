/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ReinitializeSDF.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ReinitializeSDF__
#define __GAS_ReinitializeSDF__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_ReinitializeSDF : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);

    GET_DATA_FUNC_B("rebuildsdf", RebuildSDF);

    GET_DATA_FUNC_B("rebuildsdffastiterative", RebuildSDFWithFIM);
    GET_DATA_FUNC_F("fimtolerance", FIMTolerance);
    GET_DATA_FUNC_I("fimiterations", FIMIterations);

    GET_DATA_FUNC_B("fixsigns", FixSigns);
    GET_DATA_FUNC_B("reinitializesdf", ReinitializeSDF);
    GET_DATA_FUNC_I("reinitializeiter", ReinitializeIterations);
    GET_DATA_FUNC_I("integratetype", IntegrateType);
    GET_DATA_FUNC_B("usebandwidth", UseBandwidth);
    GET_DATA_FUNC_F("bandwidth", Bandwidth);


protected:
    explicit		 GAS_ReinitializeSDF(const SIM_DataFactory *factory);
    virtual		~GAS_ReinitializeSDF();

    /// Adds to the velocity field the effect of all forces at that point.
    /// Unit density is currently assumed.
    /// Only incorpreates the external simulation forces.  Internal
    /// gas forces are handled elsewher.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Reinitizalizes a Signed Distance Field from the zero contour.
    void		 rebuildSDF(SIM_ScalarField *surface);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ReinitializeSDF,
			GAS_SubSolver,
			"Gas Reinitialize SDF",
			getDopDescription());
};

#endif
