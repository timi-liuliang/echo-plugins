/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFMerge.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for merging
 */

#ifndef __GA_AIFMerge__
#define __GA_AIFMerge__

#include "GA_API.h"


class GA_Attribute;
class GA_MergeMap;


/// @brief Attribute Interface for merging attribute data between details
/// 
/// This class provides an interface used to copy attribute data from one
/// detail to another detail.  The @b source detail is const and contains the
/// geometry which will be merged into the @b destination detail.
/// 
/// Merging is done in stages:
/// @li Initially, the attributes on the source detail (the one being merged
///   into the destination) are iterated over and if there's a matching
///   "destination" attribute, the @c destroyDestination() method is called.<br>
///   This gives the merging interface a chance to destroy any attributes on the
///   destination (depending on the options in the GA_MergeMap).  The method
///   will be called once for each attribute on the source detail.<br>
///   This provides the opportunity to delete attributes on the detail prior to
///   the re-allocation of attribute arrays.
///
/// @li If @c map.getMergeStrategy() is not @c
///   GA_MergeOptions::MERGE_INTERLEAVE, then, the @c growArray() method is
///   called.  By default, this will simply call @c
///   GA_Attribute::setArraySize() with the new size.  However, this method
///   provides the opportunity to perform optimizations in copying data from
///   the source.
///
/// @li After the destination arrays have been resized to accommodate new
///   elements, (or modify) attributes on the destination attribute, the
///   @c addDestination() method will be called to create new attributes
///   on the desintion detail.  The method will be called for each attribute on
///   the source detail.
///
/// @li The last stage of a merge is the @c copyArray() method.  At this point,
///   the attribute should have all the storage allocated and it should be as
///   simple as using the source and destination iterators to copy the data.
///   If optimizations are made in the @c growArray() method, you will most
///   likely have to alter behaviour based on the @c map.getMergeStrategy().
///
class GA_API GA_AIFMerge 
{
public:
	     GA_AIFMerge();
    virtual ~GA_AIFMerge();

    /// The @c destroyDestination() method is used to delete attributes on the
    /// destination (dattrib).  These are attributes which will not
    /// survive the merge.
    ///
    /// @param map	The merge options
    /// @param dattrib  NULL or the existing destination attribute
    /// @param sattrib	The source attribute
    virtual void		 destroyDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib,
					const GA_Attribute &sattrib) const = 0;

    /// The @c addDestination() method is used to either @b create or @b modify
    /// attributes on the destination.  For example, this method can be used to
    /// change storage, tuple size, etc. on an attribute, or to create an
    /// entirely new attribute.
    /// The dattrib parameter will be set to point to an existing
    /// attribute on the destination detail.
    /// @param map	The merge options
    /// @param dattrib  NULL or the existing destination attribute
    /// @param sattrib	The source attribute
    virtual GA_Attribute	*addDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib,
					const GA_Attribute &sattrib) const = 0;

    /// The @c growArray() method is responsible for growing the array.  The
    /// default behaviour is to call @c GA_Attribute::setArraySize().  This
    /// method is only called if the merge strategy is not set to
    /// @c GA_MergeOptions::MERGE_INTERLEAVE.  It may be possible to perform
    /// optimizations depending on how data is stored.
    /// @param map	The merge options
    /// @param dattrib  NULL or the existing destination attribute
    /// @param sattrib	The source attribute
    ///
    /// The default behaviour is to call: @code
    ///   dattrib.setArraySize( map.getDestCapacity(dattrib.getOwner()) );
    /// @endcode
    virtual void		growArray(const GA_MergeMap &map,
					GA_Attribute &dattrib,
					const GA_Attribute *sattrib) const;

    /// The @c copyArray() method is responsible for copying the data from the
    /// source to the destination attribute.  The @c GA_MergeMap provides
    /// all the information necessary for mapping the source data to the
    /// destination.
    ///
    /// Note:  The destination attribute's AIFMerge is used, and it's possible
    /// that the source attribute is NULL (indicating that there's no data to
    /// copy).
    /// @code
    ///  bool
    ///  Example::mergeAppend(GA_MergeMap &map, const GA_Attribute *src)
    ///  {
    ///      if (src) {
    ///         // Copy data from the src array to my array
    ///         const GA_Range     &di = map.getDestIterator(getOwner());
    ///         const GA_Range     &si = map.getSourceIterator(getOwner());
    ///         copyArray(*this, di, src, si);
    ///      }
    ///  }
    ///
    ///  bool
    ///  ExampleMergeAIF::copyArray(GA_MergeMap &map, GA_Attribute &d, const GA_Attribute *s) const {
    ///    Example *da = dynamic_cast<Example *>(&d);
    ///    if (da)
    ///        da->mergeAppend(map, s);
    ///  }
    /// @endcode
    /// @param map	The merge options
    /// @param dattrib  NULL or the existing destination attribute
    /// @param sattrib	The source attribute
    virtual bool		 copyArray(const GA_MergeMap &map,
					GA_Attribute &dattrib,
					const GA_Attribute *sattrib) const = 0;

    /// The @c destroyMismatchedType() function is a convenience function to
    /// use the options in GA_MergeMap to handle attributes of different types.
    ///
    /// The method should only be called if the dest and src
    /// attributes are different.
    ///
    /// This should only be called from within @c destroyDestination()
    ///
    /// @code
    ///  void
    ///  ExampleMergeAIF::destroyDestination(GA_MergeMap &map, GA_Attribute *dest, const GA_Attribute &s) const {
    ///    Example *da = dynamic_cast<Example *>(dest);
    ///    if (!da)
    ///	       GA_AIFMerge::destroyMismatchedType(map, dest, s);
    ///  }
    /// @endcode
    ///
    /// @b Note: It's possible for dest to be NULL.
    /// @param map	The merge options
    /// @param dest	NULL or the existing destination attribute
    /// @see GA_MergeOptions
    static bool		 destroyMismatchedType(const GA_MergeMap &map,
				    GA_Attribute *dest);

    /// The @c addMismatchedType() function is a convenience function to use
    /// the options in GA_MergeMap to handle attributes of different types.
    /// There are three possible types of return values:
    /// @li The original mis-matched attribute.  This will happen if the dest
    ///     attribute passed in is a different type than the source attribute,
    ///     but the merge options specify to keep the original attribute.
    /// @li A new attribute of the same type as the source.  This should only
    ///     really happen if the dest parameter was NULL.
    /// @li A NULL pointer.  This will happen if it wasn't possible to call @c
    ///     clone() on the source attribute.
    ///
    /// The method should only be called if the dest and src attributes are
    /// different.
    ///
    /// This should only be called from within @c addDestination()
    ///
    /// @code
    ///  GA_Attribute *
    ///  ExampleMergeAIF::addDestination(GA_MergeMap &map, GA_Attribute *dest, const GA_Attribute &s) const {
    ///    Example *da = dynamic_cast<Example *>(dest);
    ///    if (!da)
    ///    {
    ///	       dest = GA_AIFMerge::addMismatchedType(map, dest, s);
    ///        da = dynamic_cast<Example *>(dest);
    ///        return da;
    ///    }
    ///    return da;
    ///  }
    /// @endcode
    ///
    /// @b Note: It's possible for dest to be NULL.
    /// @param map	The merge options
    /// @param dest	NULL or the existing destination attribute
    /// @param src	The source attribute
    /// @see GA_MergeOptions
    static GA_Attribute	*addMismatchedType(const GA_MergeMap &map,
				    GA_Attribute *dest,
				    const GA_Attribute &src);

    /// Convenience function to deal with changing tuple sizes (based on
    /// merging strategy)
    /// The function returns true if the tuple storage match, false if there's
    /// an irreconcilable difference.
    /// This should be called from within @c addDestination().  If the function
    /// returns false, then no attribute should be returned from @c
    /// addDestination() (indicating no merging).
    /// @param map	The merge options
    /// @param dest	The existing destination attribute
    /// @param src	The source attribute
    /// @see GA_MergeOptions
    static bool		checkTupleStorage(const GA_MergeMap &map,
				    GA_Attribute &dest,
				    const GA_Attribute &src);

    /// Convenience function to deal with changing tuple storage (based on
    /// merging strategy).
    /// The function returns true if the tuple sizes match, false if there's an
    /// irreconcilable difference.
    /// This should be called from within @c addDestination().  If the function
    /// returns false, then no attribute should be returned from @c
    /// addDestination() (indicating no merging).
    /// @param map	The merge options
    /// @param dest	The existing destination attribute
    /// @param src	The source attribute
    /// @see GA_MergeOptions
    static bool		checkTupleSize(const GA_MergeMap &map,
				    GA_Attribute &dest,
				    const GA_Attribute &src);
};

#endif
