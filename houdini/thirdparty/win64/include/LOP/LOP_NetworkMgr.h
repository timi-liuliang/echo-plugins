/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP Library (C++)
 *
 * COMMENTS:	LOP manager node specifically for the root level (/stage).
 *
 */

#ifndef __LOP_NetworkMgr_h__
#define __LOP_NetworkMgr_h__

#include "LOP_API.h"
#include "LOP_Network.h"

class LOP_API LOP_NetworkMgr : public LOP_Network
{
public:
    // Make our constructor public so it can be called directly by MOT,
    // like we do for construcing MGR_Nodes.
	     LOP_NetworkMgr(OP_Network *parent, const char *name,
		       OP_Operator *entry);

    virtual const char	*getOpType() const;
    virtual OP_OpTypeId  getOpTypeID() const;

protected:
    virtual ~LOP_NetworkMgr();
};

#endif
