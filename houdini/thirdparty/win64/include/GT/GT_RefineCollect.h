/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_RefineCollect.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_RefineCollect__
#define __GT_RefineCollect__

#include "GT_API.h"
#include "GT_Refine.h"
#include "GT_Handles.h"
#include "GT_PrimCollect.h"
#include <UT/UT_Array.h>

/// A simple refiner which just collects the refined primitives in a list
class GT_API GT_RefineCollect : public GT_Refine
{
public:
    /// Default constructor
    GT_RefineCollect();
    /// The primitive handle @b must be a handle to a primitive collection
    /// If the primitive is not a collection, the refiner will allocate its own
    /// collection (as the default behaviour)
    GT_RefineCollect(const GT_PrimitiveHandle &collection);
    /// D-tor
    virtual ~GT_RefineCollect();

    /// Clear the collection
    void			 clear();

    /// @{
    /// Accessor
    const GT_PrimitiveHandle	&getPrim() const
				    { return myList; }
    const GT_PrimCollect	*getPrimCollect() const
				    { return getList(); }
    exint			 entries() const
				    {
					const GT_PrimCollect *cprim = getList();
					return cprim ? cprim->entries() : 0;
				    }
    GT_PrimitiveHandle		 getPrim(int i) const
				    { return getList()->getPrim(i); }
    /// @}

    /// @{
    /// Implemented from GT_Refine
    virtual bool	allowThreading() const	{ return true; }
    virtual void	addPrimitive(const GT_PrimitiveHandle &prim);
    /// @}

private:
    const GT_PrimCollect	*getList() const
    {
	return UTverify_cast<const GT_PrimCollect *>(myList.get());
    }

    GT_PrimitiveHandle		myList;
};

#endif
