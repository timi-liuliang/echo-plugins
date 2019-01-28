/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Rest.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Rest__
#define __GAS_Rest__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;

class GAS_API GAS_Rest : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);

    GET_DATA_FUNC_B("extrapolate", EnableRestExtrapolation);

    bool		 shouldMultiThread(SIM_RawField *field) const 
			 { return field->field()->numTiles() > 1; }

protected:
    explicit		 GAS_Rest(const SIM_DataFactory *factory);
    virtual		~GAS_Rest();

    /// Builds a rest field consisting of the locations at
    /// each voxel.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Builds the actual rest field.
    THREADED_METHOD3_CONST(GAS_Rest, shouldMultiThread(dst), 
		    buildRest,
		    SIM_RawField *, dst,
		    const UT_DMatrix4 &, xform,
		    int, axis);
    void		 buildRestPartial(SIM_RawField *field,
				const UT_DMatrix4 &xform,
				int axis,
				const UT_JobInfo &info) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Rest,
			GAS_SubSolver,
			"Gas Rest",
			getDopDescription());
};

#endif
