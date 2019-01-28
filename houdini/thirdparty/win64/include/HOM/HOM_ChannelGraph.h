/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ChannelGraph_h__
#define __HOM_ChannelGraph_h__

#include "HOM_API.h"
#include "HOM_BaseKeyframe.h"
#include "HOM_ElemPtr.h"
#include "HOM_PaneTab.h"
#include "HOM_Parm.h"
#include "HOM_PtrOrNull.h"

#include <map>
#include <vector>


SWIGOUT(%rename(ChannelGraph) HOM_ChannelGraph;)

class HOM_API HOM_ChannelGraph
{
public:
    HOM_ChannelGraph()
    {
	HOM_CONSTRUCT_OBJECT(this)
    }
    HOM_ChannelGraph(const HOM_ChannelGraph &pane)
    { 
	// The only reason we need to explicitly define the copy constructor is
	// so we can call HOM_CONSTRUCT_OBJECT.  Otherwise, the compiler's
	// built-in version would be fine.
	HOM_CONSTRUCT_OBJECT(this)
    }
    virtual ~HOM_ChannelGraph()
    {
	HOM_DESTRUCT_OBJECT(this)
    }

    virtual bool operator==(HOM_PtrOrNull<HOM_ChannelGraph> graph) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ChannelGraph> graph) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual std::map<
		HOM_ElemPtr<HOM_Parm>,
		std::vector< HOM_ElemPtr<HOM_BaseKeyframe> >
	    > selectedKeyframes() = 0;
};

#endif // __HOM_ChannelGraph_h__
