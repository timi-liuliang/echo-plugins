/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_VexFunction.h ( OP Library, C++)
 *
 * COMMENTS:	VEX functions common to all OPs
 */

#ifndef __OP_VexFunction__
#define __OP_VexFunction__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Matrix4.h>

class VEX_Context;
class CVEX_RunData;
class OP_Context;
class OP_Node;

class OP_API OP_DopResolverXform
{
public:
    virtual ~OP_DopResolverXform() {}
    virtual UT_DMatrix4		resolve(OP_Node *node, const char *path, fpreal time) = 0;
    virtual void		addDopReference(int opid, int srcid) = 0;
};

OP_API extern void OPregisterDopResolverXform(OP_DopResolverXform *resolver);


#endif

