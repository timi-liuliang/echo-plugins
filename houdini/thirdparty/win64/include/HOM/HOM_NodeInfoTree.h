/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeInfoTree_h__
#define __HOM_NodeInfoTree_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <UT/UT_InfoTree.h>
#include <UT/UT_SharedPtr.h>

SWIGOUT(%rename(NodeInfoTree) HOM_NodeInfoTree;)

class HOM_API HOM_NodeInfoTree
{
public:
    HOM_NodeInfoTree(UT_SharedPtr<UT_InfoTree> tree_root, UT_InfoTree *tree)
	: mySharedRoot(tree_root),
	  myInfoTree(tree)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_NodeInfoTree()
    { HOM_DESTRUCT_OBJECT(this) }

    std::string __repr__() const;

    std::string name() const;
    std::string infoType() const;
    std::vector<std::string> branchOrder() const;
    std::map<std::string, HOM_NodeInfoTree> branches() const;
    std::vector<std::string> headings() const;
    std::vector<std::vector<std::string> > rows() const;

private:
    UT_SharedPtr<UT_InfoTree>	 mySharedRoot;
    UT_InfoTree			*myInfoTree;
};

#endif
