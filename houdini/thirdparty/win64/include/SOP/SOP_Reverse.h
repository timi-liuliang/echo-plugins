/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:	The Primitive SOP
 *
 */

#ifndef __SOP_Reverse_h__
#define __SOP_Reverse_h__

#include "SOP_API.h"
#include "SOP_Reverse.proto.h"
#include <GEO/GEO_PrimTypeCompat.h>
class GA_Primitive;
class GD_Primitive;

#include "SOP_Node.h"

class SOP_API SOP_Reverse : public SOP_Node
{
public:
    virtual OP_ERROR	cookInputGroups(OP_Context &context,
				int alone = 0) override;

    static PRM_Template *buildTemplates();
    const SOP_NodeVerb	*cookVerb() const override;
    static OP_Node	*myConstructor(OP_Network *, const char *,
				OP_Operator *);

    void	SET_VERTEX(int i)       { setInt("vtxsort", 0, 0, i); }

protected:
    		 SOP_Reverse(OP_Network *, const char *, OP_Operator *);
    virtual	~SOP_Reverse();

    virtual OP_ERROR	 cookMySop(OP_Context &context) override;
    virtual const char	*inputLabel(unsigned idx) const override;
};

#endif
