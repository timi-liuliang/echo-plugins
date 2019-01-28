/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimCollect.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimCollect__
#define __GT_GEOPrimCollect__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_GEODetailList.h"
#include "GT_GEOSupport.h"
#include <UT/UT_SharedPtr.h>
#include <UT/UT_Assert.h>

class GT_RefineParms;

/// Collection data container.
class GT_API GT_GEOPrimCollectData
{
public:
    GT_GEOPrimCollectData();
    virtual ~GT_GEOPrimCollectData();

    template <typename T> T	*asPointer()
				 {
				    return UTverify_cast<T *>(this);
				 }
};

/// Collection data container to store primitive offsets
class GT_API GT_GEOPrimCollectOffsets : public GT_GEOPrimCollectData
{
public:
    GT_GEOPrimCollectOffsets();
    virtual ~GT_GEOPrimCollectOffsets();

    virtual void		 append(const GA_Primitive *prim);
    const GT_GEOOffsetList	&getPrimitives() const
				    { return myPrimitives; }
private:
    GT_GEOOffsetList	myPrimitives;
};

class GT_GEOPrimCollect;
typedef UT_SharedPtr<GT_GEOPrimCollect>	GT_GEOPrimCollectPtr;
/// Refine primitives for a detail.
///
/// When a detail refines itself, it asks the "collectors" to refine
/// primitives.  The refiners can "collect" primitives on the fly, or they can
/// collect primitives and generate a single primitive at the end of the
/// collection.
class GT_API GT_GEOPrimCollect
{
public:
    /// Constructor
    GT_GEOPrimCollect();
    /// Destructor
    virtual ~GT_GEOPrimCollect();

    /// This method is called when the detail begins collecting.  The method
    /// may or may not be called if there are now collectable primitives in the
    /// detail.
    ///
    /// The data container is passed to the @c collect() and @c endCollect()
    /// methods.  It is automatically deleted on completion.  If no data is
    /// needed for collection, this method should return a NULL pointer (which
    /// is the default behaviour).
    virtual GT_GEOPrimCollectData *
		beginCollecting(const GT_GEODetailListHandle &geometry,
				const GT_RefineParms *parms) const;

    /// For each matching primitive in the detail, this method will be called.
    /// If no GT primitive should be created, return an empty handle.
    virtual GT_PrimitiveHandle
		collect(const GT_GEODetailListHandle &geometry,
				const GEO_Primitive *const* prim_list,
				int nsegments,
				GT_GEOPrimCollectData *data) const = 0;

    /// Complete any processing fter all primitives have been collected
    ///
    /// This method should @b not delete the collection data
    virtual GT_PrimitiveHandle
		endCollecting(const GT_GEODetailListHandle &geometry,
				GT_GEOPrimCollectData *data) const;

    /// Register to process primitives of the given primitive id.
    /// You can bind to multiple primitive types, in which case,
    /// @c beginCollecting() and @c endCollecting() will be invoked only once
    /// for each detail traversal.  @c collect() will be called for each
    /// primitive that matches any of the bound primitive types.
    /// For example: @code
    ///	   gt_GEOPrimQuadricCollect  *q = new gt_GEOPrimQuadricCollect();
    ///    q->bind(GA_PrimitiveType(GA_PRIMCIRCLE));
    ///    q->bind(GA_PrimitiveType(GA_PRIMSPHERE));
    ///    q->bind(GA_PrimitiveType(GA_PRIMTUBE));
    /// @endcode
    void	bind(const GA_PrimitiveTypeId &id) const;

    /// Get the current collector bound for a primitive
    static GT_GEOPrimCollectPtr	getCollector(const GA_PrimitiveTypeId &id);
};

#endif
