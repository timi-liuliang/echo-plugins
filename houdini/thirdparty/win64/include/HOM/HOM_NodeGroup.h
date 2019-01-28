/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeGroup_h__
#define __HOM_NodeGroup_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include <string>

class HOM_Node;

SWIGOUT(%rename(NodeGroup) HOM_NodeGroup;)

class HOM_API HOM_NodeGroup
{
public:
    HOM_NodeGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NodeGroup(const HOM_NodeGroup &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NodeGroup()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_NodeGroup> node_group) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NodeGroup> node_group) = 0;

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes() = 0;

    SWIGOUT(%newobject parent;)    
    virtual HOM_Node *parent() = 0;

    virtual void addNode(HOM_Node &node) = 0;

    virtual void removeNode(HOM_Node &node) = 0;

    virtual void clear() = 0;

    virtual void destroy() = 0;

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(
	    bool save_creation_commands=true, 
	    bool save_node_memberships=true,
	    const char *function_name=NULL) = 0;
};

#endif
