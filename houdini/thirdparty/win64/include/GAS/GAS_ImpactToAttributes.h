/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ImpactToAttributes.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ImpactToAttributes
#define __GAS_ImpactToAttributes

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;

class GAS_API GAS_ImpactToAttributes : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_S(SIM_NAME_GEOMETRY, Geometry);
    GETSET_DATA_FUNCS_S("impactdata", ImpactData);
    GETSET_DATA_FUNCS_S("attribute", Attribute);

    GET_DATA_FUNC_B("resethitnum", ResetHitNum);

    GET_DATA_FUNC_B("addhitnum", AddHitNum);
    GET_DATA_FUNC_B("addhitpos", AddHitPos);
    GET_DATA_FUNC_B("addhitnml", AddHitNml);
    GET_DATA_FUNC_B("addhittime", AddHitTime);
    GET_DATA_FUNC_B("addhitpath", AddHitPath);
    GET_DATA_FUNC_B("addhitprim", AddHitPrim);
    GET_DATA_FUNC_B("addhituv", AddHitUV);
    GET_DATA_FUNC_B("addhitimpulse", AddHitImpulse);

protected:
    explicit		 GAS_ImpactToAttributes(const SIM_DataFactory *factory);
    virtual		~GAS_ImpactToAttributes();

    /// Limits scalar fields by moving along curvature or performing
    /// relaxation steps.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 impactToAttributes(SIM_Object *obj, GU_Detail *gdp);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ImpactToAttributes,
			GAS_SubSolver,
			"Gas Impact To Attributes",
			getDopDescription());
};

#endif
