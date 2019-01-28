/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimitive.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimitive__
#define __GT_GEOPrimitive__

#include "GT_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_BoundingBox.h>
#include <GA/GA_Types.h>
#include <GU/GU_DetailHandle.h>
#include "GT_Primitive.h"
#include "GT_Handles.h"

class GA_PrimitiveTypeId;
class GEO_Primitive;
class GEO_Primitive;
class GEO_Quadric;
class GEO_Curve;
class GEO_Face;
class GEO_Hull;
class GEO_TPSurf;
class GEO_PrimTube;
class GEO_PrimParticle;
class GT_GEOPrimCollect;

/// Base class for many of the GEO_Primitive objects
class GT_API GT_GEOPrimitive : public GT_Primitive
{
public:
    /// GT holds onto GU_DetailHandle objects.  However, it's possible for a
    /// user (SOP) to change the GU_Detail stored in the GU_DetailHandle
    /// without changing the GU_DetailHandle.  This can leave stale primitive
    /// pointers.  The GEOPrimitivePair object has a method to refresh the
    /// pointer to ensure it remains valid.
    class GEOPrimitivePair
    {
    public:
	GEOPrimitivePair()
	    { init(nullptr); }
	GEOPrimitivePair(const GEO_Primitive *prim)
	    { init(prim); }
	GEOPrimitivePair(const GU_ConstDetailHandle &gdh, GA_Index idx)
	    { init(gdh, idx); }

	void	init(const GEO_Primitive *prim);
	void	init(const GU_ConstDetailHandle &gdh, GA_Index idx);
	void	init(const GU_Detail *gdp, GA_Index idx);

	/// @{
	/// Reports true if the primitive pointer has changed
	bool	update(const GU_ConstDetailHandle &dtl);
	bool	update(const GU_Detail *gdp);
	/// @}

	const GEO_Primitive	*primitive() const	{ return myPrim; }
	template <typename T> const T *primitiveAs() const
				{ return UTverify_cast<const T *>(myPrim); }
	GA_Index		 mapIndex() const	{ return myMapIndex; }

    private:
	const GEO_Primitive	*myPrim;
	GA_Index		 myMapIndex;
    };

    GT_GEOPrimitive(const GU_ConstDetailHandle *dlist,
		    const GEO_Primitive *const*primlist,
		    int nsegments)
	: myDetails(NULL)
	, myPrimitives(NULL)
	, mySegmentCount(0)
    {
	init(dlist, primlist, nsegments);
    }
    GT_GEOPrimitive(const GT_GEOPrimitive &src)
	: myDetails(NULL)
	, myPrimitives(NULL)
	, mySegmentCount(0)
	, GT_Primitive(src)
    {
	init(src.myDetails, src.myPrimitives, src.mySegmentCount);
    }
    virtual ~GT_GEOPrimitive();

    virtual const char *className() const { return "GT_GEOPrimitive"; }
    
    /// Initialize the primitive lists
    void		init(const GU_ConstDetailHandle *dlist,
				const GEO_Primitive *const*primlist,
				int nsegments);
    void		init(const GU_ConstDetailHandle *dlist,
				const GEOPrimitivePair *primlist,
				int nsegments);

    /// @{
    /// Interface from GT_Primitive implemented using methods on GEO_Primitive
    virtual int		getPrimitiveType() const
			    { return GT_GEO_PRIMITIVE; }
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				int nsegments) const;
    virtual int		getMotionSegments() const
			    { return mySegmentCount; }
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimitive(*this); }
    /// @}

    /// @{
    /// Interface from GT_Primitive required by subclass
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;
    /// @}

    /// Update the primitive pointers
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

    /// @{
    /// Access to GEO primitives
    const GU_ConstDetailHandle	*detailHandles() const	{ return myDetails; }
    const GU_ConstDetailHandle	&detail(int seg) const
				{ return myDetails[seg];}
    const GEO_Primitive	*getPrimitive(int seg) const;
    template <typename T> const T *getPrimAs(int seg) const
			{
			    return UTverify_cast<const T *>(getPrimitive(seg));
			}
    /// @}

    /// Given a GEO_Quadric, create the GT transform
    static GT_TransformHandle		makeQuadricTransform(
						const GEO_Quadric *prim
					);

    /// Given a single-vertex primitive, create an attribute list for all
    /// point, vertex and primitive attributes.
    static GT_AttributeListHandle	makeQuadricAttributes(
						const GU_ConstDetailHandle &gdp,
						const GEO_Primitive *prims
					);
protected:
    void	clearLists();
    /// Get an attribute list for the primitive attributes, optionally
    /// including the detail attributes in the same list.
    /// These attributes will be "shared" with the GEO objects.
    GT_AttributeListHandle	getPrimitiveAttributes(
				    bool include_detail_attributes=true) const;

    GU_ConstDetailHandle	*myDetails;
    GEOPrimitivePair		*myPrimitives;
    int				 mySegmentCount;
};

#endif
