/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SurfaceTension.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SurfaceTension__
#define __GAS_SurfaceTension__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <UT/UT_ThreadedAlgorithm.h>

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_SurfaceTension : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S("mask", MaskName);
    
    GETSET_DATA_FUNCS_F("surfacetension", SurfaceTension);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_SurfaceTension(const SIM_DataFactory *factory);
    virtual		~GAS_SurfaceTension();

    /// Applies a force to the velocity field proportional
    /// to the vorticle values.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 addSurfaceTension(SIM_VectorField *velocity,
				    const SIM_ScalarField *surface,
				    const SIM_ScalarField *mask,
				    fpreal timestep);

    bool		 shouldMultiThread(const UT_VoxelArrayF *field) const 
			 { return field->numTiles() > 1; }

    THREADED_METHOD5_CONST(GAS_SurfaceTension, 
			    shouldMultiThread(dstfield->field()),
			    doSurfaceTension,
			    SIM_RawField *, dstfield,
			    int, axis,
			    const SIM_RawField *, surface,
			    const SIM_RawField *, mask,
			    float, scale);
    void	doSurfaceTensionPartial(SIM_RawField *dstfield,
			    int axis,
			    const SIM_RawField *surface,
			    const SIM_RawField *mask,
			    float scale,
			    const UT_JobInfo &info) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SurfaceTension,
			GAS_SubSolver,
			"Gas Surface Tension",
			getDopDescription());
};

#endif

