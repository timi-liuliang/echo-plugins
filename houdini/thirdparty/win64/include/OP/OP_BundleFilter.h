/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_BundleFilter.h ( OP Library, C++)
 *
 * COMMENTS:	Filter class for bundles
 */

#ifndef __OP_BundleFilter__
#define __OP_BundleFilter__

#include "OP_API.h"
#include "OP_DataTypes.h"
#include <UT/UT_IntArray.h>

class OP_Node;
class OP_Bundle;

class OP_API OP_BundleFilter {
public:
    //
    // The constructor of the bundle will automatically register the filter in
    // the filter database.
    //
	     OP_BundleFilter();
    virtual ~OP_BundleFilter();

    static const OP_BundleFilter	*findFilter(const char *name);
    static int				 getFilterCount();
    static const OP_BundleFilter	*getFilter(int index);

    static const char			*getNullFilterName();
    // Guesses most restrictive filter that allows all the listed nodes.
    // Returns the best fitting filter, or NULL if could not find best guess.
    static const OP_BundleFilter	*findTightFilter(
						const OP_Bundle *bundle, 
						const OP_NodeList &list);

    virtual const char	*getName() const = 0;
    virtual const char	*getLabel() const = 0;
    virtual bool	 isNodeValid(const OP_Bundle *bundle,
				     const OP_Node *node) const = 0;

    virtual int		 findAllNodes(const OP_Bundle *bundle,
				    UT_IntArray &list,
				    const OP_Node *net,
				    const OP_Node *relativeto) const;

    /// Check whether the @c needle node is in the bundle.
    /// The base class implementation checks that @c isNodeValid() is true an
    /// that the @c needle is a child of the patternNode.
    virtual bool	 matchNode(const OP_Bundle *bundle,
				    const OP_Node *needle,
				    const OP_Node *patternNode,
				    const OP_Node *relativeTo) const;

private:
};

#endif

