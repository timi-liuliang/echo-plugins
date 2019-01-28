/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_UndoBundle.h ( Operator Library, C++)
 *
 * COMMENTS:
 *		This is the Undo class that is responsible for undoing all
 *		changes to OP bundles. It even handles creation and deletion.
 */

#ifndef __OP_UndoBundle__
#define __OP_UndoBundle__

#include "OP_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_String.h>
#include <UT/UT_Undo.h>

class OP_Bundle;
class OP_BundleFilter;

enum OP_UndoBundleOperation
{
    OP_UNDO_BUNDLE_CREATE,		// Create a new bundle
    OP_UNDO_BUNDLE_DELETE,		// Delete a bundle
    OP_UNDO_BUNDLE_MODIFY,		// Add/Remove ops to a bundle
    OP_UNDO_BUNDLE_FILTER,		// Change bundle filter
    OP_UNDO_BUNDLE_LOCK_FLAG,		// Change flag
    OP_UNDO_BUNDLE_NAME,		// Name change
    OP_UNDO_BUNDLE_PATTERN		// Smart bundle pattern change
};

class OP_API OP_UndoBundle : public UT_Undo
{
public:
			 OP_UndoBundle(OP_Bundle *, OP_UndoBundleOperation);
			 OP_UndoBundle(OP_Bundle *, const char *oldname);
    virtual		~OP_UndoBundle();

    virtual void	 undo();
    virtual void	 redo();

private:
    void		 resetSize();
    void		 swap(OP_Bundle *bundle);
    void		 swapFilter(OP_Bundle *bundle);
    void		 swapPattern(OP_Bundle *bundle);
    void		 swapName();

    UT_String		 myBundleName;
    UT_String		 myOldBundleName;
    UT_String *		 myPattern;
    const OP_BundleFilter	*myFilter;
    int			 myLockFlag:1;
    int			 myPatternNode;
    UT_IntArray		 myNodeIds;
    OP_UndoBundleOperation myOperation;
};
#endif

