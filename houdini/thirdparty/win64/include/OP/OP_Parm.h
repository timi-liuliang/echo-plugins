/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Parm.h ( OP Library, C++)
 *
 * COMMENTS:
 *	This class is an abstraction of a PRM_Parm-index pair for an OP's
 *	parameter.  This class is mostly useful for UI operations that don't
 *	rely on the speed of a lookup operation.   The advantage of this class
 *	is that it does not rely on the persistance of a Parm, nor its node.
 *	In some cases holding on to a PRM_Parm pointer is dangerous because
 *	PRM_Parm pointers can be deleted for a variety of reasons (PRM_Multi,
 *	OTL changes, etc...)
 */

#ifndef __OP_Parm_h__
#define __OP_Parm_h__

#include "OP_API.h"
#include <UT/UT_String.h>

class OP_Node;
class PRM_Parm;

class OP_API OP_Parm
{
public:
     OP_Parm(PRM_Parm &parm, int subindex);
    ~OP_Parm();


    // Returns the PRM_Parm corresponding to our OP_Parm.  Always check for
    // a NULL value as it is possible.
    PRM_Parm	*getParm() const;

    // Use this version of getParm if you would also like to know the subindex
    PRM_Parm	*getParm(int &subindex) const;

    // Returns the node, could be NULL.
    OP_Node	*getNode() const;


protected:
    int		 myNodeId;
    UT_String	 myParmName;
};

#endif

