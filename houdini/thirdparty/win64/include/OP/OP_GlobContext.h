/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_GlobContext.h (OP  Library, C++)
 *
 * COMMENTS:
 *   An object passed to various globbing functions/methods that abstracts
 *   license checks.
 */

#ifndef __OP_GlobContext__
#define __OP_GlobContext__

#include "OP_API.h"

class OP_Node;
class OP_NetworkBox;

//
// An OP_GlobContext object can cache some data across multiple queries, which
// will only remain valid for a particular security context.  Consequently, it
// is not a good idea to store these objects.
//
class OP_API OP_GlobContext
{
public:
    // An OP_GlobContext() initialized by this constructor will look up the
    // current context from the script access manager.
    OP_GlobContext()
    {
	// Intentionally initialize the minimum set of members required for
	// our code paths.
	myOverrideContextFlag = false;
	myContextLookupPendingFlag = true;
	myCaseSensitiveFlag = true;
    }

    OP_GlobContext(const OP_Node *context_node)
    {
	// Intentionally initialize the minimum set of members required for
	// our code paths.
	myContextNode = context_node;
	myContextRoot = 0;
	myOverrideContextFlag = true;
	myCaseSensitiveFlag = true;
    }

    bool	 canSearchContents(const OP_Node *);
    bool	 canSearchContents(const OP_NetworkBox *);
    bool	 canSearchParms(const OP_Node *);

    bool	 isCaseSensitiveSearch() const;
    void	 setCaseSensitiveSearch(bool case_sensitive);

private:
    void	 resolveAndCacheContext(int thread);

    const OP_Node	*myContextNode;
    const OP_Node	*myContextRoot;
    void		*myOpaqueData;
    bool		 myOverrideContextFlag;
    bool		 myContextLookupPendingFlag;
    bool		 myContextInternalEvalFlag;
    bool		 myCaseSensitiveFlag;
};

#endif

