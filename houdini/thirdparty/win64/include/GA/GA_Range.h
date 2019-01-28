/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Range.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_Range__
#define __GA_Range__

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"

#include <stddef.h>


class GA_Detail;
class GA_ElementGroup;
class GA_IndexMap;
class GA_Iterator;
class GA_IteratorState;
class GA_RangeMemberQuery;
class GA_RangeTypeInterface;


/// @brief A range of elements in an index-map
///
/// A range specifies a selection/collection of elements in an index map
/// for iteration.  The membership state of individual elements can also 
/// be queried (@see allocMemberQuery()).
///
/// Iteration over a range, performed by instantiating a GA_Iterator, does
/// not affect the range object itself.  The begin() method can be used to
/// initialize a basic iterator.
///
/// Custom range types are implemented by subclassing GA_RangeTypeInterface.
class GA_API GA_Range 
{
public:
    typedef GA_Iterator const_iterator;
    typedef GA_Iterator iterator;

    /// @{
    /// Constructor/destructors
    GA_Range();
    GA_Range(const GA_RangeTypeInterface &range);
    GA_Range(const GA_Range &src);
    ~GA_Range();
    /// @}

    /// @{
    /// Default iterator
    GA_Iterator	begin() const;
    GA_Iterator	end() const;
    /// @}

    /// Determine whether a range is empty
    bool	empty() const
		    { return myRange->isEmpty(); }

    /// @{
    /// Classes used to differentiate between different constructors
    class pointref {};
    class ordered {};
    class primitiveref {};
    class emptyrange {};
    class includetemporaries {};
    class safedeletions {};
    class enditerator {};
    class ignore_order {};
    /// @}

    /// @{
    /// Interface for making another range into an ordered range.
    /// @param	descending  If true, the index order is going to be in
    ///		    a descending order. If false, it's ascending.
    GA_Range(const GA_Range &src, ordered, bool descending = false);
    /// @}
    //
    /// @{
    /// Interface for making another range into one where deletions
    /// are safe to perform during the traversal.
    GA_Range(const GA_Range &src, safedeletions);
    /// @}

    /// @{
    /// An empty range.
    GA_Range(const GA_IndexMap &map, emptyrange);
    /// @}

    /// @{
    /// Convenience constructor to construct an offset range
    /// The range includes @c start, but doesn't include @c end.
    /// @see GA_RTIOffset
    explicit GA_Range(const GA_IndexMap &map);
    GA_Range(const GA_IndexMap &map, GA_Offset start, GA_Offset end);
    /// @}

    /// @{
    /// Convenience constructor to construct an index range
    /// @see GA_RTIIndex
    GA_Range(const GA_IndexMap &map, GA_Index start, GA_Index end, ordered);
    GA_Range(const GA_IndexMap &map, GA_Index start, GA_Index end,
			GA_Size step, ordered);
    /// @}


    /// Constructor to create a range containing the offsets in the given list.
    /// If end < 0, it will be set to the @c offsetlist.entries()
    /// @see GA_RTIOffsetList
    GA_Range(const GA_IndexMap &map, const GA_OffsetList &offsetlist,
			GA_Size start=0, GA_Size end=-1);

    /// Construct a range of the elements in a given group
    /// @see GA_RTIElementGroup
    explicit GA_Range(const GA_ElementGroup &group,
			bool complement=false);

    /// Construct a range of the elements in a given group
    /// @see GA_RTIElementGroup
    GA_Range(const GA_IndexMap &map, const GA_ElementGroup *group,
			bool complement=false);

    /// Construct a range of the elements in a given group, ignoring its order
    /// even if it is ordered.
    /// @see GA_RTIElementGroup
    /// @{
    GA_Range(const GA_ElementGroup &group, ignore_order, bool complement=false);
    GA_Range(const GA_IndexMap &map, const GA_ElementGroup *group,
	     ignore_order, bool complement=false);
    /// @}

    /// Construct a range of all the elements referencing a given point.  The
    /// harden argument determines whether it is safe to use this range after
    /// the point has been deleted.  If the point will exist for the lifetime
    /// of the range, harden can be false and thus more efficient.
    ///
    /// For example, to find all the vertices which reference point number 3:
    /// @code
    ///	   GA_Range(geo, 3, GA_ATTRIB_VERTEX, GA_Range::pointref, false);
    /// @endcode
    /// @see GA_RTIPointRef
    GA_Range(const GA_Detail &geo, GA_Offset point, GA_AttributeOwner owner,
		    pointref, bool harden);

    /// Construct a range of all primitives/vertices referenced by the points
    /// in the given range.  The harden argument determines whether it is safe
    /// to use this range after the point has been deleted.  If the point will
    /// exist for the lifetime of the range, harden can be false and thus more
    /// efficient.
    /// @code
    ///	   GA_Range(geo, geo.getPointRange(), GA_ATTRIB_PRIMITIVE, false);
    ///	   GA_Range(geo, geo.getPointRange(), GA_ATTRIB_VERTEX, false);
    /// @endcode
    /// @see GA_RTIPointComprehension
    GA_Range(const GA_Detail &geo, const GA_Range &point_range,
		    GA_AttributeOwner type, pointref, bool harden);


    /// Construct a range of all the elements used by a given primitive.  The
    /// harden argument determines whether it is safe to use this range after
    /// the primitive has been deleted.  If the primitive will exist for the
    /// lifetime of the range, harden can be false, and thus more efficient.
    /// If vertices are added or removed during traversal and harden is true,
    /// only the original vertices will be traversed, but behaviour is 
    /// undefined if harden is false.
    ///
    /// For example, to find all the points which referenced by primitive 3:
    /// @code
    ///	   GA_Range(geo, 3, GA_ATTRIB_VERTEX, GA_Range::primitiveref);
    /// @endcode
    /// @see GA_RTIPrimitiveRef
    GA_Range(const GA_Detail &geo, GA_Offset prim, GA_AttributeOwner owner,
		    primitiveref, bool harden);

    /// Construct a range of all points/vertices referenced by the primitives
    /// in the given range.  The harden argument determines whether it is safe
    /// to use this range after the primitive has been deleted.  If the
    /// primitive will exist for the lifetime of the range, harden can be false
    /// and thus more efficient.
    /// @code
    ///	   GA_Range(geo, geo.getPrimitiveRange(), GA_ATTRIB_POINT);
    ///	   GA_Range(geo, geo.getPrimitiveRange(), GA_ATTRIB_VERTEX);
    /// @endcode
    /// @see GA_RTIPrimitiveComprehension
    GA_Range(const GA_Detail &geo, const GA_Range &primitive_range,
		    GA_AttributeOwner type, primitiveref, bool harden);

    /// Construct a range of all points/vertices referenced by the primitives
    /// in the given range.  The harden argument determines whether it is safe
    /// to use this range after the primitive has been deleted.  If the
    /// primitive will exist for the lifetime of the range, harden can be false
    /// and thus more efficient.
    ///
    /// This version delivers the points/vertices in the order imposed by the
    /// @c primitive_range.  This is equivalent to traversing the vertices of
    /// each primitive with a GA_Primitive::const_iterator as it is visited.
    /// @code
    ///	   GA_Range(geo, geo.getPrimitiveRange(), GA_ATTRIB_POINT);
    ///	   GA_Range(geo, geo.getPrimitiveRange(), GA_ATTRIB_VERTEX);
    /// @endcode
    /// @see GA_RTIPrimitiveComprehension
    GA_Range(const GA_Detail &geo, const GA_Range &primitive_range,
		    GA_AttributeOwner type, primitiveref, ordered, bool harden);

    /// Iterate over all elements in the index map, including any temporary
    /// elements.  This should be used within ATI's when changing data for all
    /// elements (including temporaries).
    GA_Range(const GA_IndexMap &map, includetemporaries);

    /// @{
    /// Operators
    const GA_Range	&operator=(const GA_Range &src)
			 {
			     setRange(src.myRange);
			     return *this;
			 }
    bool		 operator==(const GA_Range &src) const;
    /// @}

    /// Take ownership of the given GA_RangeTypeInterface.  This is used to
    /// avoid unnecessary calls to GA_RangeTypeInterface::copy().
    void		 adopt(GA_RangeTypeInterface *r) { setRange(r); }

    /// Check whether range is valid (i.e. has an implementation)
    bool	isValid() const		{ return myRange != NULL; }

    /// Query type of element
    GA_AttributeOwner	getOwner() const { return myRange->getOwner(); }

    /// Check whether the range is empty
    bool	isEmpty() const		{ return myRange->isEmpty(); }

    /// Get an upper bound on the range
    GA_Size	getMaxEntries() const	{ return myRange->getMaxEntries(); }
    /// Get an accurate count of the entries in the range
    GA_Size	getEntries() const	{ return myRange->getEntries(); }

    /// Query if it is safe to delete an element during traversal.  If not,
    /// you can build an equivalent range by calling:
    ///     GA_Range(initial_range, GA_Range::safedeletions)
    bool		areTraversalDeletionsSafe() const
			{
			    return !getRTI() ||
				    getRTI()->areTraversalDeletionsSafe();
			}

    /// Query if the range contains duplicates. If it cannot, then
    /// optimizations can be made assuming that there are no duplicates in the
    /// range. This defaults to true.
    bool		canContainDuplicates() const
			    { return myRange->canContainDuplicates(); }


    /// Allocate an object for testing the membership of individual elements
    /// to this range.  The caller is responsible for deleting this object,
    /// which should not outlive the allocating range.
    ///
    /// Behaviour of this object is undefined for any elements that have been
    /// deleted or moved since its creation.
    const GA_RangeMemberQuery	*allocMemberQuery() const;

    /// @cond INTERNAL
    /// Interface used by iterators to perform iteration
    void	iterateCreate(GA_IteratorState &state) const
		    { myRange->iterateCreate(state); }
    void	iterateDestroy(GA_IteratorState &state) const
		    { myRange->iterateDestroy(state); }
    void	iterateRewind(GA_IteratorState &state,
			GA_Offset &start, GA_Offset &end) const
		    { myRange->iterateRewind(state, start, end); }
    void	iterateNext(GA_IteratorState &state,
			GA_Offset &start, GA_Offset &end) const
		    { myRange->iterateNext(state, start, end); }
    void	iterateCopy(GA_IteratorState &dest,
			    const GA_IteratorState &src) const
		    { myRange->iterateCopy(dest, src); }
    /// @endcond

    /// Accessor for RTI
    const GA_RangeTypeInterface	*getRTI() const	{ return myRange; }

private:
    void	setRange(GA_RangeTypeInterface *r)
		{
		    if (r != myRange)
		    {
			if (r)
			    r->incref();
			if (myRange)
			    myRange->decref();
			myRange = r;
		    }
		}
    GA_RangeTypeInterface	*myRange;

    friend class	GA_RangeTypeInterface;	// For splitting
    friend class	GA_SplittableRange;	// For splitting
};

#endif
