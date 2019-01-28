/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Primitive.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimCollect__
#define __GT_PrimCollect__

#include "GT_API.h"
#include "GT_Primitive.h"
#include <UT/UT_ConcurrentVector.h>

class UT_StringRef;


/// A class which contains a collection of primitives.  The primitives may be
/// heterogeneous (i.e. different types).
class GT_API GT_PrimCollect : public GT_Primitive
{
public:
    typedef UT_ConcurrentVector<GT_PrimitiveHandle>	ListType;
	     GT_PrimCollect();
	     GT_PrimCollect(const GT_PrimCollect &src);
    virtual ~GT_PrimCollect();

    virtual const char *className() const { return "GT_PrimCollect"; }
    virtual bool	save(UT_JSONWriter &w) const;

    /// Clear the list
    void		clear();
    
    /// @{
    /// Interface from GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nsegs) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox b[], int n) const;
    virtual void	getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax,
				const UT_StringRef &attribute_name) const;
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual int		getPrimitiveType() const;
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimCollect(*this); }
    /// @}

    /// Reserve space for a total of N primitives
    void		reserve(exint size)	{ myList.reserve(size); }

    /// Add a primitive to the list.  This method is thread-safe
    void		appendPrimitive(const GT_PrimitiveHandle &pr)
				{ myList.push_back(pr); }

    /// Return the number of primitives in the collection
    exint			entries() const	{ return myList.size(); }
    /// Return the given primitive
    GT_PrimitiveHandle		getPrim(exint i) const;

    /// Sort the list
    template <typename Compare>
    void		sortList(Compare &cmp)
    {
	std::sort(myList.begin(), myList.end(), cmp);
    }

protected:
    void		hardenContents();

    ListType	myList;
};

#endif
