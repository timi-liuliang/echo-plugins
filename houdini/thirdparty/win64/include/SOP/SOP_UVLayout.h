/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP library (C++)
 *
 * COMMENTS:	UVLayout SOP
 */


#ifndef __SOP_UVLayout_h__
#define __SOP_UVLayout_h__

#include "SOP_API.h"
#include <GU/GU_UVPack.h>

#include "SOP_Node.h"

namespace sop_uvlayout_private {
enum SOP_PackingMethod
{
    SOP_SINGLE_FRAME = 0,
    SOP_MULTI_FRAMES,
    SOP_FROM_ATTRIBUTE,
    SOP_CURRENT_FRAME
};

enum
{
    SOP_LAYOUT_GROUP_INDEX = 0,
    SOP_LAYOUT_PACKING_INDEX,
    SOP_LAYOUT_SCALE_INDEX,
    SOP_LAYOUT_PADDING_INDEX,
    SOP_LAYOUT_PADDINGBOUNDARY_INDEX,
    SOP_LAYOUT_CORRECT_AREAS,
    SOP_LAYOUT_AXISALIGNISLANDS_INDEX,
    SOP_LAYOUT_OPTIMIZE_WASTED_SPACE
};
}

using namespace sop_uvlayout_private;

class SOP_API SOP_UVLayout : public SOP_Node
{
public:
			 SOP_UVLayout(OP_Network *net, const char *name,
				      OP_Operator *op);

    virtual int		 isRefInput(unsigned int i) const;

    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);

    virtual bool	 updateParmsFlags();

    virtual void syncNodeVersion(const char *old_version,
        const char *cur_version, bool *node_deleted);

    static OP_Node	*myConstructor(OP_Network*, const char *,
				OP_Operator *);

    static PRM_Template	 myTemplateList[];

protected:
    virtual const char  *inputLabel(unsigned idx) const;

    virtual OP_ERROR	 cookMySop(OP_Context &context);

private:
    const
    GA_PrimitiveGroup    *myGroup;
    int 		 PACKMETHOD();
    float		 SCALE();
    int			 PADDING();
    bool    		 PADDINGBOUNDARY();
    bool    		 CORRECTAREAS();
    bool         AXISALIGNISLANDS();
    bool         OPTIMIZEWASTEDSPACE();
};

#endif
