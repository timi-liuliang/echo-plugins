/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This is the base class for all SOPs that use HOM to cook.
 */

#ifndef __SOP_HOM_h__
#define __SOP_HOM_h__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_HOM : public SOP_Node
{
public:
    SOP_HOM(
	OP_Network *network, const char *name, OP_Operator *script_op_type);

    // Returns whether or not this sop is currently cooking.  It's used
    // by HOM to decide if this node's geometry may be modified.
    bool		 isCooking() const { return myIsCooking; }

    // When this sop cooks, it first duplicates its input geometry and then
    // runs a Python script.  This method is used by HOM to get the detail
    // handle for this sop when it is cooking.
    GU_DetailHandle	&getDetailHandle() { return myGdpHandle; }

    virtual OP_ERROR	 cookMySop(OP_Context &context);

    // Call these methods from cookWithHOM() to set or clear the current point/
    // primitive/vertex.  This point/primitive/vertex determines the value of
    // expressions inside parameters like $PT, $PR, hou.SopNode.curPoint, etc.
    void setCurPoint(GA_Offset point)
	{ myCurPtOff[0] = point; }
    void setCurPrim(const GEO_Primitive *prim)
	{ myCurPrimOff[0] = (prim ? prim->getMapOffset() : GA_INVALID_OFFSET); }
    void setCurVertex(GA_Offset vertex, int vertex_num)
	{
	    myCurVtxOff[0] = vertex;
	    myCurVtxNum[0] = vertex_num;
	}

protected:
    virtual void	 cookWithHOM() = 0;

private:
    bool myIsCooking;
};

#endif
