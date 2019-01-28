/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GBIterators.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_GBIterators__
#define __GA_GBIterators__

#include "GA_API.h"
#include "GA_Iterator.h"
#include "GA_Range.h"

class GA_Detail;
class GA_IndexMap;
class GA_Primitive;
class GA_PrimitiveGroup;


namespace GA_PrimCompat { class TypeMask; }


/// @file GA_GBIterators.h
/// @brief Contains iterators to help backward compatibility with GB library.
/// The constructors will initialize the iterator for the first iteration (i.e.
/// getPrimitive(), getNextPrimitive() will return the first and second
/// primitive in the iteration.  advance() will advance the iterator.
/// @see GA_GBMacros.h

class GA_API GA_GBPrimitiveIterator
{
public:
    /// Iterate over all primitives
    explicit GA_GBPrimitiveIterator(const GA_Detail &gdp);
             GA_GBPrimitiveIterator(const GA_Detail &gdp,
				    GA_Range::safedeletions);

    /// Iterate over all points from the given prim till the last in the list
    GA_GBPrimitiveIterator(
	    const GA_Detail &gdp, const GA_Primitive *first,
	    const GA_Primitive *last);

    /// Iterate over all primitives in group (group is optional)
    GA_GBPrimitiveIterator(
	    const GA_Detail &gdp, const GA_PrimitiveGroup *group);
    GA_GBPrimitiveIterator(
	    const GA_Detail &gdp, const GA_PrimitiveGroup *group,
	    GA_Range::safedeletions);

    /// Iterate over all primitives whose obsolete primitive ids match the
    /// given mask
    GA_GBPrimitiveIterator(
	    const GA_Detail &gdp, const GA_PrimCompat::TypeMask &mask);

#if 0
    /// Iterate over all primitives included by the family mask
    /// This also allows the GB macros to be mis-used.  For example: @code
    ///	   GA_FOR_MASK_PRIMITIVES(gdp, prim, GEO_PRIMPOLY)
    /// @endcode
    /// which is obviously invalid code.
    GA_GBPrimitiveIterator(const GA_Detail &gdp, uint family_mask);
#endif

    /// Iterate over pairs of primitives - groups are optional
    GA_GBPrimitiveIterator(
	    const GA_Detail &gdp1, const GA_PrimitiveGroup *group1,
	    const GA_Detail &gdp2, const GA_PrimitiveGroup *group2);

    /// Destructor
    ~GA_GBPrimitiveIterator();

    /// Return the current primitive (NULL if complete)
    GA_Primitive	*getPrimitive() const		{ return myP1; }
    /// Return the next primitive (NULL if no valid primitive)
    GA_Primitive	*getNextPrimitive() const	{ return myN1; }

    /// Return the current primitive from the 2nd detail (NULL if complete)
    GA_Primitive	*getPrimitive2() const		{ return myP2; }
    /// Return the next primitive from the 2nd detail
    GA_Primitive	*getNextPrimitive2() const	{ return myN2; }

    /// Advance to next iteration
    void		advance();

    /// Increment operator calls advance() to move to the next element
    GA_GBPrimitiveIterator	&operator++()		{ advance(); return *this; }

    // No post inc as it is harmful.

private:
    void		 setNext();
    bool		 atEnd() const
			 {
			    return myI1.atEnd() ||
				    (myI2.isValid() && myI2.atEnd());
			 }
    const GA_IndexMap	*myIndex1, *myIndex2;
    GA_Iterator		 myI1, myI2;	// Iterators
    GA_Primitive	*myP1, *myP2;	// Current primitive
    GA_Primitive	*myN1, *myN2;	// Next primitives
};

#endif
