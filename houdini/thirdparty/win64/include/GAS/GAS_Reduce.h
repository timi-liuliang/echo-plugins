/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Reduce.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Reduce__
#define __GAS_Reduce__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>
#include <GA/GA_SplittableRange.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_PhysicalParms.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;

class GAS_API GAS_Reduce : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S(GAS_NAME_FIELDSOURCE, FieldSrcName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S("optionpath", OptionName);
    GET_DATA_FUNC_B("createattrib", CreateAttrib);

    GET_DATA_FUNC_I("reduction", Reduce);
    GET_DATA_FUNC_I("scaleby", ScaleBy);
    GET_DATA_FUNC_F("percentile", Percentile);
    GET_DATA_FUNC_B("maskissdf", MaskIsSDF);

    // Reduce methods
    enum REDUCTION_NAMES
    {
	REDUCE_MAX = 0,
	REDUCE_MIN,
	REDUCE_MAXABS,
	REDUCE_MINABS,
	REDUCE_AVERAGE,
	REDUCE_PERCENTILE,	// Yeah, expensive :>
	REDUCE_SUM,
	REDUCE_SUMABS,
	REDUCE_SUMSQUARE,
	REDUCE_RMS,
	NUM_REDUCE
    };

protected:
    explicit		 GAS_Reduce(const SIM_DataFactory *factory);
    virtual		~GAS_Reduce();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    fpreal		 reduceVal(const SIM_RawField *src,
				const SIM_RawField *maskfield);
    fpreal		 reduceValCL(const SIM_RawField *src,
				const SIM_RawField *maskfield);
    void		 doReduce(SIM_RawField *dest,
				const SIM_RawField *src,
				const SIM_RawField *maskfield);

    fpreal		 reduceVal(const GA_SplittableRange &range,
				   const GA_Attribute *src,
				   int comp,
				   REDUCTION_NAMES op);

    bool		reduceGeoToAttribute(SIM_Object *obj,
					     SIM_GeometryCopy *geo,
					     const UT_String &srcname,
					     const UT_String &dstname);

    bool		reduceGeoToOptions(SIM_Object *obj,
					   SIM_GeometryCopy *geo,
					   const UT_String &srcname,
					   const UT_String &optionpath);

private:
    static const SIM_DopDescription	*getDopDescription();


    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Reduce,
			GAS_SubSolver,
			"Gas Reduce",
			getDopDescription());

    // Cache our timestep for so we can calculate the timescale
    // effects.
    float		myTimeStep;

    bool getUseOpenCL();
};

#endif
