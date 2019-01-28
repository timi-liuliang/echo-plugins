/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_NetFetchData.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_NetFetchData__
#define __GAS_NetFetchData__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class UT_NetExchange;
class UT_NetMessage;

class GAS_API GAS_NetFetchData : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    
    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);

    GET_DATA_FUNC_I("peer", Peer);
    GET_DATA_FUNC_I("npeer", NPeer);
    GET_DATA_FUNC_S("srcobject", SourceObject);
    GET_DATA_FUNC_S("srcdataname", SourceDataName);
    GET_DATA_FUNC_I("srcpeer", SourcePeer);

    GET_DATA_FUNC_S("dstdataname", DestDataName);
protected:
    explicit		 GAS_NetFetchData(const SIM_DataFactory *factory);
    virtual		~GAS_NetFetchData();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 processRequest(UT_NetExchange &netxchg,
					UT_NetMessage *msg,
					SIM_Engine &engine);

    void		 postDataError(UT_NetExchange &netxchg,
					int dstpeer, const char *errmsg);

    void		 attachData(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				const char *dstdataname,
				const char *data,
				int len);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_NetFetchData,
			GAS_SubSolver,
			"Gas Net Fetch Data",
			getDopDescription());
};

#endif

