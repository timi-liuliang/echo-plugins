/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_RefineList.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_RefineList__
#define __GT_RefineList__

#include "GT_API.h"
#include "GT_Refine.h"
#include "GT_Primitive.h"
#include <UT/UT_Array.h>

/// Refine to a list of primtives.
///
/// When refining large numbers of primitives, you may want to refine each
/// primitive in turn rather than generating a full list of refined primitives.
class GT_API GT_RefineList {
public:
     GT_RefineList();
    ~GT_RefineList();

    /// Return the number of primitives in the list
    int		entries() const		{ return myList.entries(); }

    /// Return a given primitive from the list
    const GT_PrimitiveHandle	&getPrimitive(int i) const { return myList(i); }

    /// Refine a primitive, adding primitives to the list
    bool	refine(GT_Primitive *prim, const GT_RefineParms *parms)
		    { return prim->refine(myRefiner, parms); }

    /// Clear all primitives from the list
    void	clear();

    class GT_API iterator
    {
	public:
	    iterator()
		: myOwner(NULL), myCurr(0) {}
	    iterator(const iterator &src)
		{ *this = src; }
	    ~iterator() {}

	    const GT_PrimitiveHandle	getPrimitive() const
					{
					   return myOwner->getPrimitive(myCurr);
					}
	    int				getCurr() const
					{
					    return myCurr;
					}

	    void	rewind()	{ myCurr = 0; }
	    void	advance()	{ myCurr++; }
	    bool	atEnd() const	{ return myCurr >= myOwner->entries(); }

	    iterator	&operator++()	{ advance(); return *this; }
	    // No post increment as it is dangerous.
	    const iterator	&operator=(const iterator &src)
				{
				    myOwner = src.myOwner;
				    myCurr = src.myCurr;
				    return *this;
				}

	private:
	    iterator(const GT_RefineList *list)
		: myOwner(list)
		, myCurr(0) {}

	    const GT_RefineList	*myOwner;
	    int			 myCurr;
	    friend class GT_RefineList;
    };
    iterator	begin() const		{ return iterator(this); }

private:
    class refiner : public GT_Refine {
    public:
	refiner(GT_RefineList &l)
	    : myOwner(l) {}
	~refiner() {}
	virtual void	addPrimitive(const GT_PrimitiveHandle &p)
			    { myOwner.myList.append(p); }
	GT_RefineList	&myOwner;
    };

    refiner				myRefiner;
    UT_Array<GT_PrimitiveHandle>	myList;
};

#endif
