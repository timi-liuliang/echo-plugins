/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Limit.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Limit__
#define __GAS_Limit__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;

class GAS_API GAS_Limit : public GAS_SubSolver
{
public:
    bool		 shouldMultiThread(const UT_VoxelArrayF *field) const 
			 { return field->numTiles() > 1; }

    GET_DATA_FUNC_I("minchoice", MinChoice);
    GET_DATA_FUNC_I("maxchoice", MaxChoice);
    GET_DATA_FUNC_I("revertchoice", RevertChoice);
    GET_DATA_FUNC_V3("minvalue", MinValue);
    GET_DATA_FUNC_V3("maxvalue", MaxValue);
    GET_DATA_FUNC_V3("revertvalue", RevertValue);
    GET_DATA_FUNC_F("blendwidth", Blendwidth);

    struct 	gas_limitsrc
    {
	const SIM_RawField	*field;
	float			 val;
	bool			 usefield;
    };

protected:
    explicit		 GAS_Limit(const SIM_DataFactory *factory);
    virtual		~GAS_Limit();

    /// Limits scalar fields by moving along curvature or performing
    /// relaxation steps.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 doLimit(SIM_RawField *dstfield,
				const gas_limitsrc &minsrc,
				const gas_limitsrc &maxsrc,
				const gas_limitsrc &revertsrc,
				float blendwidth);

    THREADED_METHOD5_CONST(GAS_Limit, shouldMultiThread(dstfield->field()),
		    limit,
		    SIM_RawField *, dstfield,
		    const gas_limitsrc &, minsrc,
		    const gas_limitsrc &, maxsrc,
		    const gas_limitsrc &, revertsrc,
		    float, blendwidth);
    void	 limitPartial(SIM_RawField *dstfield,
			    const gas_limitsrc &minsrc,
			    const gas_limitsrc &maxsrc,
			    const gas_limitsrc &revertsrc,
			    float blendwidth,
			    const UT_JobInfo &jobinfo) const;

    gas_limitsrc buildLimitSrc(SIM_Object *obj, int i, int j,
			int choice,
			const char *parmname,
			UT_Vector3 val);
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Limit,
			GAS_SubSolver,
			"Gas Limit",
			getDopDescription());
};

#endif
