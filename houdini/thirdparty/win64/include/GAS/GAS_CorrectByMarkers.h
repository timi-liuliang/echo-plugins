/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_CorrectByMarkers.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_CorrectByMarkers__
#define __GAS_CorrectByMarkers__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_VectorField;

class GAS_API GAS_CorrectByMarkers : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, SurfelName);

    GETSET_DATA_FUNCS_F("threshold", Threshold);

protected:
    explicit		 GAS_CorrectByMarkers(const SIM_DataFactory *factory);
    virtual		~GAS_CorrectByMarkers();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Corrects the iso surface according to marker values.
    void		 correctSurface(SIM_ScalarField *surface, GU_Detail *gdp);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_CorrectByMarkers,
			GAS_SubSolver,
			"Gas Correct By Markers",
			getDopDescription());
};

#endif
