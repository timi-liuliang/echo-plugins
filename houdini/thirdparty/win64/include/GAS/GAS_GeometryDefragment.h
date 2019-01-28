/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_GeometryDefragment.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_GeometryDefragment
#define __GAS_GeometryDefragment

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;

class GU_Detail;

class GAS_API GAS_GeometryDefragment : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_GEOMETRY, Geometry);

protected:
    explicit		 GAS_GeometryDefragment(const SIM_DataFactory *factory);
    virtual		~GAS_GeometryDefragment();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_GeometryDefragment,
			GAS_SubSolver,
			"Gas Geometry Defragment",
			getDopDescription());
};

#endif
