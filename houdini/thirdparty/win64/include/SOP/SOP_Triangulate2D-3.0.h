/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Triangulate2D-3.0.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_Triangulate2D3__
#define __SOP_Triangulate2D3__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_Triangulate2D3 : public SOP_Node
{
public:

			 SOP_Triangulate2D3(OP_Network *net, const char *,
				 OP_Operator *entry);

    virtual 		~SOP_Triangulate2D3() {}

    OP_ERROR	 	 cookInputGroups(OP_Context &context,
				int alone = 0) override;

    static OP_Node	*myConstructor(OP_Network  *net, const char *name,
				OP_Operator *entry);

    static PRM_Template	*buildTemplates();

    const SOP_NodeVerb	*cookVerb() const override final;

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context) override;

    UT_Vector3		 myBoundingSquareCorners[4];
};

#endif

