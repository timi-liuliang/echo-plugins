/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_FieldVex.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_FieldVex__
#define __GAS_FieldVex__

#include "SIMZ_API.h"

#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_RawIndexField.h>
#include <SIM/SIM_PhysicalParms.h>

#include <CVEX/CVEX_Context.h>
#include <CVEX/CVEX_Value.h>

#include <GAS/GAS_SubSolver.h>

class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;
class SIM_IndexField;

class gas_BlockData;
class GU_VexGeoInputs;

class SIMZ_API gas_volumeMap
{
public:
    UT_StringHolder	name;
    const SIM_RawField	*vlist[3][3];
    SIM_RawField	*outlist[3][3];

    // Since you can't have these in matrices, there's only one.
    const SIM_RawIndexField *idxvlist;
    SIM_RawIndexField	*idxoutlist;

    UT_String		dataname[3][3];
    int			dataidx_x[3][3];
    int			dataidx_y[3][3];
    CVEX_Type		type;
    bool		processed[3][3];
};

struct SIMZ_API gas_volumeOutput
{
    const SIM_RawField	*srcvol;
    const SIM_RawIndexField *srcidxvol;
    SIM_RawField	**outfield;
    SIM_RawIndexField	**idxoutfield;
    int			map;
    int			i, j;
};

class OP_Caller;

struct SIMZ_API gas_volumeParms
{
    const SIM_RawField	*vol;
    const SIM_RawIndexField	*idxvol;
    int			argc;
    const char *const	*argv;
    UT_Array<gas_volumeMap>	*volmap;
    UT_Array<gas_volumeOutput>	outputs;
    OP_Caller		*opcaller;
    UT_Lock		myErrorLock;
    UT_StringArray	myErrors;
    UT_StringArray	myWarnings;
    bool		myHasOutputMask;
    UT_String		myOutputMask;
    const GU_VexGeoInputs	*myGeoInputs;
    const UT_Array<UT_Matrix4D>	*myInputXform;
    const UT_IntArray	*myInputXformId;
};


class SIMZ_API GAS_FieldVex : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_B("vex_multithread", MultiThread);
    GETSET_DATA_FUNCS_S("vexscript", VexScript);
    GETSET_DATA_FUNCS_S("bindings", Bindings);
    GETSET_DATA_FUNCS_B("autobind", Autobind);
    GETSET_DATA_FUNCS_S("inputbindings", InputBindings);
    GETSET_DATA_FUNCS_S("vex_cwdpath", VexCWD);
    GETSET_DATA_FUNCS_S("vex_outputmask", VexOutputMask);
    GETSET_DATA_FUNCS_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GETSET_DATA_FUNCS_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_FieldVex(const SIM_DataFactory *factory);
    virtual		~GAS_FieldVex();

    bool		 shouldMultiThread(SIM_RawField *field) const 
			 { return field->field()->numTiles() > 1; }

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 processVexScript(
				SIM_Object *obj, 
				UT_String &script, 
				UT_Array<gas_volumeMap> &volmap,
				const GU_VexGeoInputs &geoinputs,
				const UT_Array<UT_Matrix4D> &inputxform,
				const UT_IntArray &inputxformid) const;
    void 		 executeVex(SIM_Object *obj, int argc,
				const char *const*argv, 
				UT_Array<gas_volumeMap> &volmap,
				OP_Caller &caller,
				const GU_VexGeoInputs &geoinputs,
				const UT_Array<UT_Matrix4D> &inputxform,
				const UT_IntArray &inputxformid) const;

    THREADED_METHOD1_CONST(GAS_FieldVex, true, processVexVolume,
		    gas_volumeParms &, parms)
    void		 processVexVolumePartial(gas_volumeParms &parms,
			    const UT_JobInfo &info) const;

    bool		 processVexBlock(gas_BlockData &data, 
				CVEX_Context &context,
				int argc, const char *const*argv,
				const UT_Array<gas_volumeOutput> &outlist,
				const SIM_RawField *vol,
				const SIM_RawIndexField *idxvol,
				const UT_Array<gas_volumeMap> &volmap,
				int num, int *ix, int *iy, int *iz,
				const UT_IntArray &completetilelist,
				const UT_JobInfo &info) const;
private:
    static const SIM_DopDescription	*getDopDescription();

    fpreal		 myTime, mySopTime, myTimeInc, myFrame;
    fpreal		 mySimTime, mySimFrame;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_FieldVex,
			GAS_SubSolver,
			"Gas Field Vex",
			getDopDescription());
};

#endif
