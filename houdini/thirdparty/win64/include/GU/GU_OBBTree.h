/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GU_OBBTree_h__
#define __GU_OBBTree_h__

#include "GU_API.h"
#include <BV/BV_OBBTree.h>
#include <UT/UT_VectorTypes.h>

class GU_Detail;

/// This is a simple extension to BV_OBBTree that includes debug output
/// capability.
class GU_API GU_OBBTree : public BV_OBBTree
{
public:
    typedef BV_OBBTree BaseClass;

		 GU_OBBTree();
		 GU_OBBTree(const GU_OBBTree &);

    GU_OBBTree	&operator=(const GU_OBBTree &);

    /// Create a GU_Detail showing the OBBs at each level of the
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
    bool	 outputTree(GU_Detail &gdp, const BV_OBB &node, 
			    const UT_Matrix3 &R, const UT_Vector3 &T,
			    int onlyDepth, int onlyLeaf, int depth) const;
};

#endif

