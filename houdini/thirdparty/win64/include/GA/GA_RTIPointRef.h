/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTIPointRef.h ( GA Library, C++)
 *
 * COMMENTS:	Iterate over elements which connect to a point.
 *
 * The selection of all vertices which reference point number 32
 * 	GA_RTIPointRef	i(geo, 32, GA_ATTRIB_VERTEX);
 *
 * The selection of all primitives which reference point number 14
 * 	GA_RTIPointRef	i(geo, 14, GA_ATTRIB_PRIMITIVE);
 *
 * If a primitive refers to a point more than once, the primitive will be
 * visited multiple times.
 */

#ifndef __GA_RangePoints__
#define __GA_RangePoints__

#include "GA_API.h"
#include "GA_RTISingle.h"
#include "GA_Types.h"


class GA_Detail;
class GA_IndexMap;
class GA_IteratorState;
class GA_Range;
class GA_RangeTypeInterface;


/// @brief Specify range of elements (vertex, primitive) that reference a point
///
/// This specifies a range of elements which reference a given point.  For
/// example, the range of all vertices which reference a point or the range of
/// all primitives which reference a point.
class GA_API GA_RTIPointRef : public GA_RTISingle 
{
public:
    /// Define the range.  The @c owner parameter defines what type elements
    /// are in the range.  This should be either GA_ATTRIB_VERTEX or
    /// GA_ATTRIB_PRIMITIVE.  The @c point parameter specifies the point of
    /// interest.  The @c geo parameter specifies the source ot the geometry.
    /// For example, to iterate over all the vertices which reference point 32
    /// @code
    ///	   GA_RTIPointRef(geo, 32, GA_ATTRIB_VERTEX);
    /// @endcode
    GA_RTIPointRef(const GA_Detail &geo, GA_Offset point, GA_AttributeOwner owner);
    /// Copy constructor
    GA_RTIPointRef( const GA_RTIPointRef &i );
    /// Destructor
    virtual ~GA_RTIPointRef();

    /// Point iterators may not work unless the topological attributes exist.
    bool	isValid() const;

    /// @{
    /// RangeTypeInterface implementation
    virtual GA_RangeTypeInterface	*copy() const;
    virtual bool	isSplittable() const	{ return false; }
    virtual bool	isEmpty() const;
    virtual GA_Size	getMaxEntries() const;
    virtual GA_Size	getEntries() const;
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual bool	isEqual(const GA_RangeTypeInterface &src) const;
    virtual void	iterateCopy(GA_IteratorState &dest,
				    const GA_IteratorState &src) const;
    virtual GA_Size	getPageCount() const	{ return -1; }
    virtual bool	getPageElementRange(GA_Range &, GA_Size, GA_Size) const
			    { return getPageElementRangeNonSplittable(); }
    virtual GA_Offset	getFirstOffsetInPage(GA_Size) const
			    { return getFirstOffsetInPageNonSplittable(); }
    /// @}

private:
    /// @{
    /// RangeTypeInterface implementation
    virtual void	singleRewind(GA_IteratorState &state) const;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const;
    virtual void	singleNext(GA_IteratorState &state) const;
    /// @}
    const GA_IndexMap	&mySourceMap;
    GA_Offset		 myPoint;
    GA_AttributeOwner	 myTargetType;
    GA_Size		 myComputedSize;
};

#endif
