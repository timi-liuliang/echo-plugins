/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GU_KDOPTree_h__
#define __GU_KDOPTree_h__

#include "GU_API.h"
#include <BV/BV_KDOPTree.h>

class GU_Detail;

/// This is a simple extension to BV_KDOPTree that includes debug output
/// capability.
template <int K>
class GU_API GU_KDOPTree : public BV_KDOPTree<K>
{
public:
    typedef BV_KDOPTree<K> BaseClass;

		 GU_KDOPTree();
		 GU_KDOPTree(const GU_KDOPTree &);

    GU_KDOPTree	&operator=(const GU_KDOPTree &);

    /// Create a GU_Detail showing the KDOPs at each level of the
    /// hierarchy.
    ///
    /// If the depth parameter is non-negative, then only leaves
    /// at the given depth level are included.
    /// If the onlyLeaf parameter is non-negative, then only boxes containing
    /// that primitive are included.
    void	 createDebugTree(GU_Detail &gdp, 
				 int depth = -1,
				 int prim = -1) const;
private:
    static bool	 outputTree(GU_Detail &gdp, const BV_KDOPNode<K> &node, 
			    int onlyDepth, int onlyLeaf, int depth);
    static void  outputNode(GU_Detail &gdp, const BV_KDOPNode<K> &node);
};

typedef GU_KDOPTree<6>   GU_AABBTree;
typedef GU_KDOPTree<14>  GU_14DOPTree;
typedef GU_KDOPTree<18>  GU_18DOPTree;
typedef GU_KDOPTree<26>  GU_26DOPTree;

#endif
