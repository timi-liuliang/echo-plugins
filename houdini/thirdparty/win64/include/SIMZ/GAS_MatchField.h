/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_MatchField.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_MatchField__
#define __GAS_MatchField__

#include "SIMZ_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include <GAS/GAS_SubSolver.h>
#include <GAS/GAS_Utils.h>

class GU_Detail;
class SIM_VectorField;
class SIM_IndexField;

class SIMZ_API GAS_MatchField : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S("reffield", RefFieldName);
    GET_DATA_FUNC_S("refsoppath", RefSopPath);

    GET_DATA_FUNC_I("rank", Rank);
    GET_DATA_FUNC_I("border", Border);
    GET_DATA_FUNC_B("matchsamples", MatchSamples);

protected:
    explicit		 GAS_MatchField(const SIM_DataFactory *factory);
    virtual		~GAS_MatchField();

    /// Builds a field that matches the reference field.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_MatchField,
			GAS_SubSolver,
			"Gas MatchField",
			getDopDescription());
};

#endif
