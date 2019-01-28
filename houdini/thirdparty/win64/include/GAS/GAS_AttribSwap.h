/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_AttribSwap.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_AttribSwap
#define __GAS_AttribSwap

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;

class GU_Detail;

class GAS_API GAS_AttribSwap : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_GEOMETRY, Geometry);

protected:
    explicit		 GAS_AttribSwap(const SIM_DataFactory *factory);
    virtual		~GAS_AttribSwap();

    /// AttribSwap scalar fields by moving along curvature or performing
    /// relaxation steps.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    bool	swapAttributes(SIM_Object *obj, GU_Detail *gdp, int which);

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_AttribSwap,
			GAS_SubSolver,
			"Gas Attribute Swap",
			getDopDescription());
};

#endif
