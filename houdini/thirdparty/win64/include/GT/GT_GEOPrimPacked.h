/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimPacked.h (GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimPacked__
#define __GT_GEOPrimPacked__

#include "GT_API.h"
#include "GT_Primitive.h"
#include <GU/GU_PrimPacked.h>

class UT_StringRef;

/// Container for a GU packed primitive
class GT_API GT_GEOPrimPacked : public GT_Primitive
{
public:
    /// Create a GT representation of the packed primitive @c prim.  If @c
    /// transformed is true, the transform on the packed primitive will be used
    /// when refining.
    /// The @c prim_gdh is the detail handle for the detail containing the
    /// packed @c prim.
    GT_GEOPrimPacked(const GU_ConstDetailHandle &prim_gdh,
		    const GU_PrimPacked *prim,
		     bool transformed=true,
		     bool include_packed_attribs = false)
	: myPrimDetail(prim_gdh)
	, myPrim(prim)
	, myAttribs()
	, myTransformed(transformed)
    {
	createPointPrimIDs(include_packed_attribs);
    }
    GT_GEOPrimPacked(const GT_GEOPrimPacked &src)
	: GT_Primitive(src)
	, myPrimDetail(src.myPrimDetail)
	, myPrim(src.myPrim)
	, myAttribs(src.myAttribs)
	, myTransformed(src.myTransformed)
    {
    }
    virtual ~GT_GEOPrimPacked();

    /// @{
    /// GT_Primitive interface
    virtual const char *className() const;
    virtual int		getPrimitiveType() const { return GT_GEO_PACKED; }
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nseg) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox boxes[], int nseg) const;
    virtual void	getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax,
				const UT_StringRef &attribute_name) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	save(UT_JSONWriter &w) const;
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimPacked(*this); }
    
    virtual const GT_AttributeListHandle	&getPointAttributes() const
						    { return myAttribs; }
    /// @}

    /// The default refine method will process primitives, providing a bounding
    /// box, point cloud, etc. based on the viewport LOD stored on the
    /// primitive.
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;

    /// @{
    /// Normally, the primitive will pick up the transform from the GU
    /// primitive implementation and use this in the GT representation.
    /// However, when doing instancing of shared geometry, the transforms will
    /// be picked up by the instance object, and so should be ignored on the
    /// base primitive.
    bool	transformed() const	{ return myTransformed; }
    void	setTransformed(bool f)	{ myTransformed = f; }
    /// @}

    /// @{
    /// Set or query the viewport LOD - full, points, bbox, centroid, hidden.
    virtual void setViewportLOD(GEO_ViewportLOD, int /*instance*/ )
				{}
    virtual GEO_ViewportLOD getViewportLOD(int /*instance*/) const
				{ return myPrim ? myPrim->viewportLOD()
					        : GEO_VIEWPORT_FULL; }
    /// @}

    /// Test whether a packed primitive should use the viewport LOD setting or
    /// not.  There are at least two factors in this determination:
    ///	- Whether the GT_RefineParms::getPackedViewportLOD() is true
    /// - Whether the LOD is being overriden using
    ///   GT_RefineParms::getOverridePackedViewportLOD()
    static bool	useViewportLOD(const GT_RefineParms *parms);

    /// Get the full, un-transformed geometry.
    /// @note Some packed primitives will always create "deformed" geometry.
    void	geometryAndTransform(
			const GT_RefineParms *parms,
			GT_PrimitiveHandle &result,
			GT_TransformHandle &xform) const;

    /// Test whether instancing of the underlying geometry can be done
    /// Returns @c false by default.  If you return @c true, you must implement
    ///   - getInstanceKey()
    ///   - getInstanceGeometry()
    ///   - getInstanceTransform()
    /// at a minimum.
    virtual bool	canInstance() const;

    /// If "instancing" is supported by the packed primitive, this method
    /// should return true.  The options that define the packed primitive
    /// should be stored in the UT_Options.  Primitives which have matching
    /// UT_Options can be considered to be shared.
    virtual bool	getInstanceKey(UT_Options &options) const;

    /// Return the geometry the geometry for instancing.  This may be different
    /// than getFullGeometry() in that
    ///   - The geometry should not be transformed
    ///   - The packed primitive attributes should not be included in the
    ///     geometry
    /// This allows multiple instances to share the geometry unmodified
    /// The default method returns a NULL ptr.
    virtual GT_PrimitiveHandle	getInstanceGeometry(
					const GT_RefineParms *parms,
					bool ignore_visibility =false) const;
    /// When instancing, this method extracts the attributes on the packed
    /// primitive and returns them as a GT_AttributeList.  The default method
    /// scoops up all attributes on the primitive (point, vertex, primitive &
    /// detail).
    virtual GT_AttributeListHandle	getInstanceAttributes() const;

    /// Return the transform handle for this particular instance.  This method
    /// should return the transform required to transform the instance geometry
    /// to the space of this instance.  The default method returns an identity
    /// transform.
    virtual GT_TransformHandle		getInstanceTransform() const;


    /// Returns either the primitive transform or the instance transform,
    /// depending on whether canInstance() returns true.
    GT_TransformHandle			getFullTransform() const;

    /// Accessor to the primitive
    const GU_PrimPacked	*getPrim() const	{ return myPrim; }
    /// Accessor to the packed implementation
    const GU_PackedImpl	*getImplementation() const
				{ return myPrim->implementation(); }

    /// The default implementation of getPackedDetail() is @code
    ///    return getPrim->getPackedDetail();
    /// @endcode
    /// It's called by @c getPointCloud() and @c getFullGeometry() to allow for
    /// sharing of geometry.
    virtual GU_ConstDetailHandle	getPackedDetail() const;

protected:
    /// @{
    /// Sub-classes may want to override these methods which currently just
    /// call getPackedDetail() and unpack() to access the geometry.
    /// These methods should return the "untransformed" primitives.
    ///
    /// @warning It is important to ensure that the primitive attributes refer
    /// to the GU_PrimPacked primitive, vertex, point.  For example, in
    /// GT_GEODetailList, you'd want to set the primitive id otherwise you'll
    /// get crashes in the viewport as it tries to dereference the
    /// points/vertices in the base detail (not the packed detail).
    ///
    /// If @c requires_transform is false if the point cloud is in the
    /// primitive's transformed space (the same as unpacking).  If it's true
    /// the packed transform will be applied to the resulting primitive
    /// (without modifying the returned geometry).  @c require_transform will
    /// be true by default, so it doesn't have to be written in most cases.
    virtual GT_PrimitiveHandle	getPointCloud(const GT_RefineParms *parms,
					bool &requires_transform) const;
    virtual GT_PrimitiveHandle	getFullGeometry(const GT_RefineParms *parms,
					bool &requires_transform) const;
    /// @}

    /// When extracting geometry for a single primitive, the base class may
    /// automatically copy some attributes from the packed primitive to the GT
    /// instance.  If you've written a custom handler which is able to instance
    /// geometry (i.e. share the geometry through a GT_PrimInstance), this will
    /// do the wrong thing, since the shader attributes will be copied to the
    /// geometry (rather than being able to change on a per-instance basis).
    /// So, you must override this method to indicate that you're handling the
    /// packed attributes yourself.
    ///
    /// The default is to copy the attributes, if you're handling them
    /// yourself, return @c false.
    virtual bool		copyPackedAttributesToGeometry() const;

    /// For fragments, the GU interface only provides a means of unpacking(),
    /// but not querying the fragment range.  This convenience method will
    /// create geometry for a range of a gdp without unpacking.
    /// The fragment_gdh is a detail representing the geometry of the fragment.
    GT_PrimitiveHandle	buildUntransformedFragment(
				const GU_ConstDetailHandle &fragment_gdh,
				const GA_Range &range) const;

    /// @{
    /// Sub-classes may want to override these methods which currently just
    /// call the bounding box method and create transformed geometry based on
    /// the bounds.
    /// The box & centroid should be @b untransformed.
    virtual GT_PrimitiveHandle	getBoxGeometry(const GT_RefineParms *parms) const;
    virtual GT_PrimitiveHandle	getCentroidGeometry(const GT_RefineParms *parms) const;
    /// @}

    void		 createPointPrimIDs(bool include_packed_attribs);
    void		 appendAttribute(const char *name,
					 const GT_DataArrayHandle &attrib);
    
    /// Hardening a packed primitive will extract the full geometry
    virtual GT_PrimitiveHandle	doHarden() const;

    void		setDetailPrim(const GU_ConstDetailHandle &dh,
				      const GU_PrimPacked *prim)
			{
			    myPrimDetail = dh;
			    myPrim = prim;
			}
private:
    GU_ConstDetailHandle	 myPrimDetail;
    const GU_PrimPacked		*myPrim;
    GT_AttributeListHandle	 myAttribs;
    bool			 myTransformed;
};

#endif
