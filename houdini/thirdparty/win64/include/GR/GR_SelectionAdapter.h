/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_SelectionAdapter.h ( GR Library, C++)
 *
 * COMMENTS:
 *      A utility class to allow working with GU_Selection and GA_Group
 *      objects more transparently.  Much of the drawing code will draw
 *      the user selection (GU_Selection) and cook selection (GA_Group)
 *      the same way and this adapter class provides a common interface
 *      for both so we can share code more easily.
 */
#ifndef GR_SelectionAdapter_h
#define GR_SelectionAdapter_h

#include "GR_API.h"

#include <GU/GU_Selection.h>

class OP_Node;
class UT_StringArray;
class UT_BitArray;

class GR_API GR_SelectionAdapter
{
public:
    GR_SelectionAdapter() :
	myCookGroup(NULL) {}

    void	 setUserSelection(GU_SelectionHandle selection)
				    { myUserSelection = selection; }
    void	 setCookSelection(const GU_Detail &gdp)
			    {
				myUserSelection = gdp.cookSelection();
			    }
    void	 setCookGroup(const GA_Group *group)
				    { myCookGroup = group; }

    bool	 isSet() const { return myUserSelection || myCookGroup; }

    // Returns whether what we consider to be the selection is potentially
    // non-empty.  The use of the word "potentially" highlights that some
    // simplifying assumptions are made here and we may return true when
    // the selection does end up being empty, but if we return false, the
    // selection will always end up being empty.
    bool	 isPotentiallyNonEmpty() const;

    GA_GroupType	 classType() const
    {
	if (myUserSelection)
	    return myUserSelection->classType();
	if (myCookGroup)
	    return myCookGroup->classType();
	return GA_GROUP_INVALID;
    }

    const GA_PointGroup		*points(const GU_Detail &gd) const;
    const GA_PrimitiveGroup	*primitives(const GU_Detail &gd) const;
    const GA_VertexGroup	*vertices(const GU_Detail &gd) const;
    const GA_EdgeGroup		*edges(const GU_Detail &gd) const;
    const GA_BreakpointGroup 	*breakpoints(const GU_Detail &gd) const;

private:
    GU_SelectionHandle	 myUserSelection;
    const GA_Group	*myCookGroup;
};


#endif
