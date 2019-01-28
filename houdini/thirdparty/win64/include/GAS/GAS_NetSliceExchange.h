/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_NetSliceExchange.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_NetSliceExchange__
#define __GAS_NetSliceExchange__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <SIM/SIM_Slice.h>

class SIM_GeometryCopy;
class GU_Detail;

class GAS_API GAS_NetSliceExchange : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    
    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);
    GET_DATA_FUNC_I("slice", Slice);

    GET_DATA_FUNC_B("assignslice", AssignSlice);
    GET_DATA_FUNC_B("transferpoints", TransferPoints);
    GET_DATA_FUNC_B("exchangeattribs", ExchangeAttribs);
    GET_DATA_FUNC_B("deleteexternal", DeleteExternal);
    GET_DATA_FUNC_B("computeslicedist", ComputeSliceDistance);
    GET_DATA_FUNC_B("normalizeslicedist", NormalizeSliceDistance);

    GET_DATA_FUNC_I("blend", Blend);
    GET_DATA_FUNC_S("id", Id);
    GET_DATA_FUNC_S("sliceattr", SliceAttr);
    GET_DATA_FUNC_S("attrib", Attrib);
    GET_DATA_FUNC_S("sliceroot", SliceRoot);

    GET_DATA_FUNC_F("overlap", Overlap);
    GET_DATA_FUNC_F("overlappscale", OverlapPscale);

protected:
    explicit		 GAS_NetSliceExchange(const SIM_DataFactory *factory);
    virtual		~GAS_NetSliceExchange();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    void		 doNetExchange(SIM_GeometryCopy *geo,
				    const SIM_SliceSearchTable &table,
				    const char *address,
				    int port,
				    const char *jobname,
				    int blendmode);

    void		 buildAttribPacket(std::ostream &os,
				    GU_Detail *gdp,
				    UT_Array<GA_Offset> &ptlist,
				    GA_ROHandleI &idh,
				    const char *attribstring);


    void		 applyAttribPacket(UT_IStream &is,
			   GU_Detail *gdp, const UT_Map<int, GA_Offset> &idlut);

    THREADED_METHOD3(GAS_NetSliceExchange, gdp->getNumPoints() > 2048, assignSlices,
		    GU_Detail *, gdp,
		    const char *, slicename,
		    const SIM_SliceSearchTable &, slicetable)
    void		 assignSlicesPartial(GU_Detail *gdp,
				    const char *slicename,
				    const SIM_SliceSearchTable &slicetable,
				    const UT_JobInfo &info);

    THREADED_METHOD5(GAS_NetSliceExchange, gdp->getNumPoints() > 2048, computeSliceDist,
		    GU_Detail *, gdp,
		    const SIM_SliceSearchList &, slicelist,
		    float, overlap,
		    float, overlapscale,
		    bool, normalize)
    void		 computeSliceDistPartial(GU_Detail *gdp,
					const SIM_SliceSearchList &slicelist,
					float overlap,
					float overlapscale,
					bool normalize,
					const UT_JobInfo &info);

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_NetSliceExchange,
			GAS_SubSolver,
			"Gas Net Slice Exchange",
			getDopDescription());
};

#endif

