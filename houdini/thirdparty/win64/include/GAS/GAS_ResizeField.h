/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ResizeField.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ResizeField__
#define __GAS_ResizeField__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_RawField;

class GAS_API GAS_ResizeField : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S("reffield", RefFieldName);
    GET_DATA_FUNC_V3("minpad", MinPad);
    GET_DATA_FUNC_V3("maxpad", MaxPad);
    GET_DATA_FUNC_V3("voxelpad", VoxelPad);
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_V3("size", Size);
    GET_DATA_FUNC_V3("t", Center);
    GET_DATA_FUNC_I("combinefixed", CombineWithFixed);
    GET_DATA_FUNC_I("combineorig", CombineWithOrig);
    GET_DATA_FUNC_B("keepdata", KeepData);
    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);

    // Reduce methods
    enum COMBINE_NAMES
    {
	COMBINE_REPLACE = 0,
	COMBINE_UNION,
	COMBINE_INTERSECT,
	NUM_COMBINE
    };
protected:
    explicit		 GAS_ResizeField(const SIM_DataFactory *factory);
    virtual		~GAS_ResizeField();

    // Moves & resizes the field keeping all data.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Synchronizes with any distributed nodes our desired
    /// box size, building the maximum of them all.
    void		 synchronizeBoxSizes(const char *dataname,
					     UT_Vector3 &size,
					     UT_Vector3 &center,
					     int slice,
					     int numslices);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ResizeField,
			GAS_SubSolver,
			"Gas Resize Field",
			getDopDescription());
};

#endif
