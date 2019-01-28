/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_DSD.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_DSD__
#define __GAS_DSD__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <SIM/SIM_RawField.h>
#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_DSD : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S("d", DName);
    GET_DATA_FUNC_S("ddot", DDotName);
    GET_DATA_FUNC_S("curvature", KappaName);
    GET_DATA_FUNC_S("curvatureadvected", KappaStarName);
    
    GETSET_DATA_FUNCS_F("c1", C1);
    GETSET_DATA_FUNCS_F("c2", C2);
    GETSET_DATA_FUNCS_F("c3", C3);
    GETSET_DATA_FUNCS_F("c4", C4);
    GETSET_DATA_FUNCS_F("c5", C5);
    GETSET_DATA_FUNCS_F("mutheta", MuTheta);
    GETSET_DATA_FUNCS_F("dcj", DCJ);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_DSD(const SIM_DataFactory *factory);
    virtual		~GAS_DSD();

    /// Applies a force to the velocity field proportional
    /// to the vorticle values.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 shouldMultiThread(SIM_RawField *field) const 
			 { return field->field()->numTiles() > 1; }

    THREADED_METHOD5_CONST(GAS_DSD, shouldMultiThread(d),
			applyDSD,
			SIM_RawField *, d,
			SIM_RawField *, ddot,
			const SIM_ScalarField *, kappa,
			const SIM_ScalarField *, kappastar,
			fpreal, timestep);
    void		 applyDSDPartial(SIM_RawField *d,
				SIM_RawField *ddot,
				const SIM_ScalarField *kappa,
				const SIM_ScalarField *kappastar,
				fpreal timestep,
				const UT_JobInfo &info) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_DSD,
			GAS_SubSolver,
			"Gas DSD",
			getDopDescription());
};

#endif

