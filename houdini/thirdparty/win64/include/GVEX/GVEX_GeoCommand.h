/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GVEX_GeoCommand.h ( GVEX Library, C++)
 *
 * COMMENTS:	Applies VEX_GeoCommands to GDPs
 */

#ifndef __GVEX_GeoCommand__
#define __GVEX_GeoCommand__

#include "GVEX_API.h"

#include <GU/GU_Detail.h>
#include <VEX/VEX_GeoCommand.h>
#include <UT/UT_ThreadedAlgorithm.h>

class GU_Agent;
class GU_PrimPacked;

template <VEX_Precision PREC>
class GVEX_API GVEX_GeoCommand
{
public:
    GVEX_GeoCommand();
    ~GVEX_GeoCommand();

    // WARNING: Because of shared string tables, the provided geocmd
    // must remain live until after this is applied!
    void		appendQueue(VEX_GeoCommandQueue<PREC> &geocmd);
    void		appendQueues(UT_Array<VEX_GeoCommandQueue<PREC>> &geocmds);

    // Used by apply() to resolve relative op: paths
    void		setCwdOpId(int op_id)
			    { myCwdOpId = op_id; }

    void		apply(GU_Detail *gdp);

public:
    GA_Offset		pointOffset(const GU_Detail *gdp, exint elem, exint pointidx) const;
    GA_Offset		primitiveOffset(const GU_Detail *gdp, exint elem, exint primitiveidx) const;
    GA_Offset		vertexOffset(const GU_Detail *gdp, exint elem, exint vertexidx) const;
    GA_Offset		vertexOffset(const GU_Detail *gdp, exint elem, exint primidx, exint vertexidx) const;

    GA_Offset		dictOffset(const GU_Detail *gdp, GA_AttributeOwner owner, exint elem, exint objidx, exint secidx) const;

protected:
    void		applyAttribCreate(GU_Detail *gdp, GA_AttributeOwner owner, VEX_GeoAttribEdit<PREC> *edit) const;
    void		applyAttribEdit(GU_Detail *gdp, GA_AttributeOwner owner, VEX_GeoAttribEdit<PREC> *edit, bool alreadybumpingid) const;
    void		applyGroupCreate(GU_Detail *gdp, GA_AttributeOwner owner, VEX_GeoAttribEdit<PREC> *edit) const;
    void		applyGroupEdit(GU_Detail *gdp, GA_AttributeOwner owner, VEX_GeoAttribEdit<PREC> *edit, bool alreadybumpingid) const;
    void		applyIntrinsicEdit(GU_Detail *gdp, VEX_GeoAttribEdit<PREC> *edit) const;

    THREADED_METHOD2(GVEX_GeoCommand, myGeoCmd.myPointAdd.entries() > 8*1024,
			setNewPointPos,
			GU_Detail *, gdp,
			bool *, hasclone);
    void		setNewPointPosPartial(GU_Detail *gdp,
				    bool *hasclone,
				    const UT_JobInfo &info);

private:
    template <typename GeoCmdT>
    GU_Agent *		getAgent(GU_Detail *gdp, const GeoCmdT &cmd,
				 bool all_agents, GU_PrimPacked *&packed_prim);

private:
    bool			mySorted;
    VEX_GeoCommandQueue<PREC>		myGeoCmd;

    UT_IntArray			myNetPrimIndex;
    UT_IntArray			myNetPointIndex;
    UT_IntArray			myNetVertexIndex;

    int				myCwdOpId;
};

GVEX_EXTERN_TEMPLATE(GVEX_GeoCommand<VEX_32>);

#endif
