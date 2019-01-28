/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Output.h ( DOP Library, C++)
 *
 * COMMENTS:	Dynamics OPs.  These are nodes which are used for setting
 * 		up and controlling dynamic simulations.
 */

#ifndef __DOP_Output__
#define __DOP_Output__

#include "DOP_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_Array.h>
#include <SIM/SIM_ObjectArray.h>

class DOP_Node;

class DOP_API DOP_Output
{
public:
				 DOP_Output(DOP_Node *node, int outputidx)
				    : myNode(node),
				      myOutputIdx(outputidx)
				 { }
				 DOP_Output()
				    : myNode(0),
				      myOutputIdx(-1)
				 { }
    virtual			~DOP_Output()
				 { }

    bool			 operator==(const DOP_Output &cmp) const
				 {
				    return (myNode == cmp.myNode &&
					    myOutputIdx == cmp.myOutputIdx);
				 }

    DOP_Node			*getNode() const
				 { return myNode; }
    int				 getOutputIdx() const
				 { return myOutputIdx; }

private:
    DOP_Node			*myNode;
    int				 myOutputIdx;

    friend size_t hash_value(const DOP_Output &output);
};

inline size_t hash_value(const DOP_Output &output)
{
    return SYSpointerHash(output.myNode) ^ SYSwang_inthash(output.myOutputIdx);
}

#endif
