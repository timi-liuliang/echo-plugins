/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_PBDSolve.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_PBDSolve__
#define __GAS_PBDSolve__

#include "GAS_API.h"

#include <GU/GU_Detail.h>

#include <UT/UT_ThreadedAlgorithm.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class GAS_SPH;

class GAS_API GAS_PBDSolve : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_F("repulsionstiffness", RepulsionStiffness);
    GET_DATA_FUNC_F("repulsionweight", RepulsionWeight);
    GET_DATA_FUNC_F("attractionstiffness", AttractionStiffness);
    GET_DATA_FUNC_F("attractionweight", AttractionWeight);
    GET_DATA_FUNC_I("iterations", Iterations);

protected:
    explicit		 GAS_PBDSolve(const SIM_DataFactory *factory);
    virtual		~GAS_PBDSolve();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 shouldMultiThread() { return true; }

protected:
    THREADED_METHOD1(GAS_PBDSolve, shouldMultiThread(), solvePBD,
				GU_Detail *, gdp)
    void		 solvePBDPartial(GU_Detail *gdp, 
				const UT_JobInfo &info);

    void		 doSolvePBD(GU_Detail *gdp, GA_Range r);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_PBDSolve,
			GAS_SubSolver,
			"Gas PBD Solve",
			getDopDescription());
};

#endif
