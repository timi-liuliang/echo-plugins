/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SPHForces.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SPHForces__
#define __GAS_SPHForces__

#include "GAS_API.h"

#include <GU/GU_Detail.h>

#include <UT/UT_ThreadedAlgorithm.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class GAS_SPH;

class GAS_API GAS_SPHForces : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_F("gasconstant", GasConstant);
    GET_DATA_FUNC_B("dopressure", DoPressure);
    GET_DATA_FUNC_B("doviscosity", DoViscosity);
    GET_DATA_FUNC_B("dotension", DoTension);
    GET_DATA_FUNC_B("computev", ComputeVelocity);

protected:
    explicit		 GAS_SPHForces(const SIM_DataFactory *factory);
    virtual		~GAS_SPHForces();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 shouldMultiThread() { return true; }

protected:
    void		 calculatePressure(GU_Detail *gdp, fpreal gasconstant,
				GA_Range r);
    void		 accumulatePressureForce(GU_Detail *gdp, const GAS_SPH *sph,
				GA_Range r);
    void		 accumulateViscousForce(GU_Detail *gdp, const GAS_SPH *sph,
				GA_Range r);
    void		 accumulateSurfaceForce(GU_Detail *gdp, const GAS_SPH *sph,
				GA_Range r);

    THREADED_METHOD2(GAS_SPHForces, shouldMultiThread(), calcPressure,
				GU_Detail *, gdp, 
				fpreal, gasconstant)
    void		 calcPressurePartial(GU_Detail *gdp, 
				fpreal gasconstant,
				const UT_JobInfo &info);

    THREADED_METHOD5(GAS_SPHForces, shouldMultiThread(), calcForces,
				GU_Detail *, gdp, 
				const GAS_SPH &, sph,
				bool, dopressure,
				bool, doviscosity, bool, dotension)
    void		 calcForcesPartial(GU_Detail *gdp,
				const GAS_SPH &sph,
				bool dopressure, bool diviscosity,
				bool dotension, const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SPHForces,
			GAS_SubSolver,
			"Gas SPH Forces",
			getDopDescription());
};

#endif
