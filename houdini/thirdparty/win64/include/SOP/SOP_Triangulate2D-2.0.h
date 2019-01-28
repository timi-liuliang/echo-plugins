/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Triangulate2D-2.0.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_Triangulate2D2__
#define __SOP_Triangulate2D2__

#include "SOP_API.h"
#include "SOP_Node.h"

#define TRIANGULATE_2D2_DEBUG_PARMS  0

class SOP_API SOP_Triangulate2D2 : public SOP_Node
{
public:

			 SOP_Triangulate2D2(OP_Network *net, const char *,
				 OP_Operator *entry);

    virtual 		~SOP_Triangulate2D2() {}

    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);

    static OP_Node	*myConstructor(OP_Network  *net, const char *name,
				OP_Operator *entry);

    static void		 buildAttribNameMenu(void *data, PRM_Name *entries,
				int size, const PRM_SpareData *,
				const PRM_Parm *);

    static PRM_Template		*buildTemplates();
    const SOP_NodeVerb		*cookVerb() const;

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
};

#endif

