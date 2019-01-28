/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_RenderList.h ( ROP Library, C++)
 *
 * COMMENTS:
 *    Defines a list of render objects, and ensures that no ROP/frame pair
 *    is rendered twice within the list.
 */
#ifndef ROP_RenderList_h
#define ROP_RenderList_h

#include "ROP_API.h"
#include "ROP_RenderItem.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>

class ROP_RenderDepParms;

class ROP_API ROP_RenderList
{
public:
    ROP_RenderList() {}
    ~ROP_RenderList();

    // this steals the pointer, and will delete it when this list is
    // destroyed. It is added to the list unless it's a duplicate of an item
    // already in the list (if so, it will return false)
    bool	    addItem(const ROP_RenderItemHandle &item,
			    const ROP_RenderDepParms &parms);

    // This adds a new copy of 'item' to the list, if 'item' is not already in
    // the list.
    ROP_RenderItemHandle    addItemDependency(const ROP_RenderItemHandle &item);

    void	    replaceDependencies(const ROP_RenderItemHandle &item,
					const ROP_RenderItemHandle &newitem);
    void	    replaceOldDependencies(const ROP_RenderItemHandle &item) const;
    ROP_RenderItemHandle    removeItem(unsigned int i);
    
    int			    entries() const { return myItems.entries(); }

    ROP_RenderItemHandle   &operator()(unsigned int i)  { return myItems(i); }

    const ROP_RenderItemHandle &operator()(unsigned int i) const
						 { return myItems(i); }

    // Merges 'mergelist' into this list, along with removed dependencies.
    // mergelist is emptied after this operation.
    void		    mergeAndClear (ROP_RenderList &merge_list,
					   bool append);

    // makes this list depend on the items in merge_list, and prepends
    // those items into this list.
    void		    dependMerge(ROP_RenderList &merge_list);

    // Ensures that all dependents are rendered after the ROPs they are
    // dependent on. This should be the last step for the final dependency
    // list.
    void		    resortDependencies();

    /// Obtain a list of items that nobody depends on in the list.
    void		    getDependencyRoots( 
				UT_Array<ROP_RenderItemHandle> &roots) const;
	
    /// Obtain a list of items that don't depend on any other item in the list.
    void		    getDependencyLeaves( 
				UT_Array<ROP_RenderItemHandle> &leaves)const;

    // assign rendered frame information to each node's popup info.
    void		    assignRenderInfo(const char *job_tag);

    // dump the render information to os.
    void	 print(std::ostream &os, bool full_path, 
        	       bool condense_range) const;

private:
    UT_Array<ROP_RenderItemHandle> myItems;
    UT_Array<ROP_RenderItemHandle> myUnusedItems;
    UT_IntArray			   myRemovedDependencies;
    UT_Array<ROP_RenderItemHandle> myReplacementDependencies;
};

#endif
