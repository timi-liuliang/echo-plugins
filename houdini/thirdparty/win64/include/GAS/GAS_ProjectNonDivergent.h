/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ProjectNonDivergent.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ProjectNonDivergent__
#define __GAS_ProjectNonDivergent__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_VectorField;

class GAS_API GAS_ProjectNonDivergent : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S(GAS_NAME_PRESSURE, PressureName);
    GET_DATA_FUNC_S(GAS_NAME_DIVERGENCE, DivergenceName);
    GET_DATA_FUNC_S("component", ComponentName);
    GET_DATA_FUNC_B("preservebubble", PreserveBubble);
    GET_DATA_FUNC_B("ghostfluid", GhostFluid);
    GET_DATA_FUNC_B("variational", Variational);
    GET_DATA_FUNC_I("preconditioner", Preconditioner);
    GET_DATA_FUNC_I("numiter", NumIterations);

protected:
    explicit		 GAS_ProjectNonDivergent(const SIM_DataFactory *factory);
    virtual		~GAS_ProjectNonDivergent();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Projects the field in the specific case of particles.
    void		 projectToNonDivergent(GU_Detail *gdp);
    
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ProjectNonDivergent,
			GAS_SubSolver,
			"Gas ProjectNonDivergent",
			getDopDescription());
};

#endif
