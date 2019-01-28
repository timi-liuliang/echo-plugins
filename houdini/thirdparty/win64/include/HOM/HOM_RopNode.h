/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_RopNode_h__
#define __HOM_RopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"

SWIGOUT(%rename(RopNode) HOM_RopNode;)

class HOM_API HOM_RopNode : virtual public HOM_Node
{
public:
    HOM_RopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RopNode(const HOM_RopNode &rop_node)
    : HOM_Node(rop_node),
      HOM_NetworkMovableItem(rop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_RopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isLocked() = 0;
    virtual void setLocked(bool on) = 0;

    virtual std::vector<std::pair<HOM_ElemPtr<HOM_Node>, std::vector<double> > >
	    inputDependencies() = 0;

    SWIGOUT(%kwargs render;)
    virtual void render(
	    const std::vector<double> &frame_range = std::vector<double>(),
	    const std::vector<double> &res = std::vector<double>(),
	    const char *output_file = NULL,
	    const char *output_format = NULL,
	    bool to_flipbook = false,
	    int quality = 2,
	    bool ignore_inputs = false,
	    HOM_EnumValue &method = HOM_renderMethod::RopByRop,
	    bool ignore_bypass_flags = false,
	    bool ignore_lock_flags = false,
	    bool verbose=false,
	    bool output_progress=false) = 0;
};

#endif
