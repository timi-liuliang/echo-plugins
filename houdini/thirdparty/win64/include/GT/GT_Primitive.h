/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Primitive.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Primitive__
#define __GT_Primitive__

#include "GT_API.h"
#include "GT_AttributeList.h"
#include "GT_DataArray.h"
#include "GT_Handles.h"
#include "GT_PrimitiveTypes.h"
#include "GT_Transform.h"
#include "GT_Types.h"
#include "GT_ViewportRefineOptions.h"
#include <GA/GA_Names.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_VectorTypes.h>

class GT_Refine;
class GT_RefineParms;
class GU_ConstDetailHandle;
class UT_JSONWriter;
class UT_StringRef;

/// @brief The base class for all GT primitive types
///
/// Primitives represent geometric entities like spheres and polygons.
/// There is a list of @em well-known primitives defined in
/// GT_PrimitiveTypes.h.  These are returned by the @c getPrimitiveType() method.
///
/// Each primitive must be able to compute a bounding box.  Since a primitive
/// may be part of a sequence of primitives, the primitive must be able to
/// compute the bounding boxes for each segment.
///
class GT_API GT_Primitive
    : public UT_IntrusiveRefCounter<GT_Primitive>
{
public:
	     GT_Primitive();
	     GT_Primitive(const GT_Primitive &src);
    virtual ~GT_Primitive();

    virtual const char *className() const = 0;

    /// The GT library has pre-defined primitive types (see GT_PrimitiveType)
    /// Custom primitives may ask for an identifier which is guaranteed
    /// to be unique.  These integers are created at run time and may have
    /// different values from run to run.  They will always be greater or equal
    /// to GT_MAX_PRIM_TYPES.
    static int		createPrimitiveTypeId();

    /// Return the primitive type.
    /// By default this returns GT_PRIM_UNDEFINED,
    virtual int		getPrimitiveType() const;

    /// Return a unique primitive ID and true, if an ID can be generated (false
    /// otherwise).
    virtual bool	getUniqueID(int64 &id) const;
    
    /// An array of bounding boxes is passed in.  There is a bounding box for
    /// each segement.  Each bounding box should be enlarged appropriately.
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const = 0;

    /// This is used when computing bounds for rendering (not just geometric
    /// bounds).  This normally just calls @c enlargeBounds(), but for some
    /// primitives, their bounds can be influenced by attribute values (i.e.
    /// the "width" in point mesh primitives).  This allows proper bounds to be
    /// passed to mantra.
    /// Users may want to call @c enlargeWidth()
    virtual void	enlargeRenderBounds(UT_BoundingBox boxes[],
					int nsegments) const;

    /// This method is used to find the minimum/maximum velocity bounds.  By
    /// default, the shared/vertex attributes are scanned for the attribute and
    /// its range is used.  Only the first segment is queried.
    ///
    /// If unable to compute the velocity range, the @c min and @c max should
    /// be set to 0.
    virtual void	getVelocityRange(
			    UT_Vector3 &min, UT_Vector3 &max,
			    const UT_StringRef &attribute_name = GA_Names::v) const;

    /// Refine the primitive.
    /// - @c refiner @n
    ///	   The refiner object is responsible for processing primitives, and
    ///	   also giving the primitive hints on how to refine itself.
    /// The method should returns true if new primitives were added to the
    /// refiner.
    virtual bool	refine(GT_Refine &refiner,
					const GT_RefineParms *parms=NULL) const;

    /// Return the number of motion segments defined on the geometry
    /// By default, this simply returns the number of segments on P.
    virtual int		getMotionSegments() const = 0;

    /// Return an approximate memory usage.  Since data may be shared, this
    /// will always be an over-estimation.
    virtual int64	getMemoryUsage() const = 0;

    /// Return a transform applied to the entire primitive.  By default, this
    /// returns an identity transform.
    const GT_TransformHandle &getPrimitiveTransform() const
			{ return myTransform; }
    /// Set the transform for a the primitive.
    void		setPrimitiveTransform(const GT_TransformHandle &x)
			{ myTransform = x; }

    /// Create a clone of this primitive, the clone may share references to the
    /// same data.
    GT_PrimitiveHandle	clone() const
			{ return doSoftCopy(); }

    /// @{
    /// Virtual methods to access the attribute data for primitives
    /// Some primitives may not implement these methods, in which case, the
    /// returned value will be an empty list.
    virtual const GT_AttributeListHandle	&getPointAttributes() const;
    virtual const GT_AttributeListHandle	&getVertexAttributes() const;
    virtual const GT_AttributeListHandle	&getUniformAttributes() const;
    virtual const GT_AttributeListHandle	&getDetailAttributes() const;
    const GT_AttributeListHandle	&getAttributeList(GT_Owner owner) const;
    /// @}

    /// Returns true if a data array with 'name' is found in 'owner_scope',
    /// and optionally its storage and tuple_size.
    virtual bool	hasDataArray(const UT_StringRef &name,
				     GT_Owner owner_scope[],
				     int num_owners,
				     GT_Storage *storage = NULL,
				     GT_Size	*tuple_size = NULL) const;
	
    /// update any cached data for geometry and its attributes
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &parms);

    /// Return true if the primitive represents geometry at frame 'fr'.
    void		setStaticGeometry(bool static_geo)
			    { myStaticGeoState=static_geo ? 1 : 0; }
    bool		isStaticGeometry() const { return myStaticGeoState==1; }
    bool		isFrameInfoAvailable() const
					 { return myStaticGeoState>=0; }
    
    /// Return the data ID hash derived from attribute lists. If cache_data_id
    /// is true, cache the result for future calls. This should only be done
    /// if the primitive and its attributes will not change (such as packed
    /// primitive contents).
    bool		getDataIdHash(int64 &hash, int segment=0,
				      bool cache_data_id = false) const;

    /// Returns the topology version for the primitive.
    virtual bool	getTopologyVersion(int64 &version) const;
    
    /// Find an attribute of a given name.  The search order is:
    ///	- getVertexAttributes()
    ///	- getPointAttributes()
    ///	- getUniformAttributes()
    ///	- getDetailAttributes()
    GT_DataArrayHandle	findAttribute(const UT_StringRef &name,
				      GT_Owner &owner,
				      int segment) const;

    /// print out all attribute lists
    void	dumpAttributeLists(const char *label, bool data_too) const;

    /// Dump the primitive.  If the primitive's save() method isn't implemented
    /// (i.e. returns false), the attributes will be dumped.
    void	dumpPrimitive() const;

    /// Save all attribute lists to a JSON stream.  The attribute lists are
    /// saved as a dictionary/map, keyed on the class name.
    bool	saveAttributeLists(UT_JSONWriter &w) const;

    /// Save the primitive to a JSON stream.  The default method does nothing
    /// and returns false.
    virtual bool	save(UT_JSONWriter &w) const;

    /// This method is used to pass information about refinement to the Houdini
    /// viewport.  This returns a default GT_ViewportRefineOptions.
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    /// The jsonWriter class can be used by sub-classes to simplify saving
    /// primitives to a JSON object.  For example: @code
    ///	   jsonWriter	w("PolygonMesh");
    ///	   myVertexList.save(*w);
    ///	   myFaceOffsets.save(*w);
    ///	   saveAttributeLists(*w);
    /// @endcode
    /// The constructor automatically starts an array.  The destructor closes
    /// the array.
    class GT_API jsonWriter
    {
    public:
	 jsonWriter(UT_JSONWriter &w, const char *primitive_name);
	~jsonWriter();
	UT_JSONWriter	&operator*() { return myWriter; }
    private:
	UT_JSONWriter	&myWriter;
    };

    /// @{
    /// Methods defined on GT_Primitive which are a short-cut to avoid a
    /// static/dynamic cast of the primitive when we know that the primitive is
    /// a polygon mesh.  These are not part of the interface of GT_Primitive
    /// per se.
    virtual const GT_DataArrayHandle	&getVertexList() const;
    virtual GT_DataArrayHandle	createPointNormals(int segment=0,
					const UT_StringRef &P = GA_Names::P,
					bool normalize=true,
					const fpreal32 *pntdata = NULL,
					GT_Storage store =GT_STORE_REAL32) const;
    /// @}

    /// Refine a detail or primitive. The refine parms may be NULL. If the
    /// refinement fails, the returned handle will be NULL.
    /// @{
    static GT_PrimitiveHandle refineDetail(const GU_ConstDetailHandle &detail,
					   const GT_RefineParms *parms);
    static GT_PrimitiveHandle refinePrimitive(const GT_PrimitiveHandle &primh,
					      const GT_RefineParms *parms);
    /// @}

    /// The harden method will return a NULL pointer if hardening isn't
    /// implemented for the primitive in question.  Otherwise, any references
    /// will be hardened into local data for the primitive.
    GT_PrimitiveHandle	harden() const
			    { return doHarden(); }

    /// Create a transformed copy.  This will transform a @c clone() of the
    /// primitive by the given transform.  It's possible @c this primitive
    /// might be returned if the clone doesn't need to be transformed.  The @c
    /// force parameter will force a clone to be created.
    GT_PrimitiveHandle	copyTransformed(const GT_TransformHandle &x,
				bool force = false) const;

    /// Copy the transform from the source primitive for transforming.  This is
    /// only required when you're not using a copy c-tor on the primitive
    /// (which automatically copies the transform).
    void		refineCopyTransformFrom(const GT_Primitive &src);

    /// The attributeMerge method will return a new primitive, but with the
    /// given attributes merged in from the source primitive.  If the method
    /// isn't implementd on the sub-class, or if the primitive types don't
    /// match, or if there are other errors, the method will return the
    /// original primitive (i.e. @c this).
    GT_PrimitiveHandle	attributeMerge(const GT_Primitive &src,
				const UT_StringMMPattern *vertex_pattern,
				const UT_StringMMPattern *point_pattern,
				const UT_StringMMPattern *uniform_pattern,
				const UT_StringMMPattern *detail_pattern) const
    {
	return doAttributeMerge(src, vertex_pattern, point_pattern,
			uniform_pattern, detail_pattern);
    }

public:
    /// @{
    /// For memory tracking, we override the new/delete operators
    static void	*operator	new(size_t size);
    static void	*operator	new(size_t size, void *p);
    static void	 operator	delete(void *p, size_t size);
    /// @}

    /// Methods for GEO/GU support.
    /// @{
    /// The default methods for these simply refine until a primitive can
    /// handle the method.
    virtual fpreal	computePerimeter(int seg=0) const;
    virtual fpreal	computeSurfaceArea(int seg=0) const;
    virtual fpreal	computeVolume(const UT_Vector3 &ref_P, int seg=0) const;
    /// @}

    /// Hardening the primitive will return a copy of the primitive, but
    /// without any dangling references.  The default implementation returns a
    /// NULL ptr.
    virtual GT_PrimitiveHandle	doHarden() const;

    /// Create a copy of the primitive, referencing all the source data
    /// This can return a NULL pointer, but it would be better to implement it
    /// properly.
    virtual GT_PrimitiveHandle	doSoftCopy() const = 0;

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

    /// This method will "merge" two attribute lists given 
    /// The @c changed variable will be set to true if there are modifications,
    /// otherwise, it will remain unchanged.
    static GT_AttributeListHandle
			mergeAttributeLists(bool &changed,
				    const GT_AttributeListHandle &src,
				    const GT_AttributeListHandle &merge,
				    const UT_StringMMPattern *pattern,
				    const UT_StringMMPattern *alternate = NULL);


    /// Convenience method to compute the velocity bounds given a data array
    /// Returns @c false if the data array handle wasn't valid.  If invalid, @c
    /// vmin and @c vmax will be set to 0.
    static bool	computeVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax,
			const GT_DataArrayHandle &v);

    /// Search attributes for a "width" or "pscale" attribute.  If one is
    /// found, enlarge the bounding boxes by the maximum value of the
    /// attribute.  If no attributes are found, use the @c defwidth.
    /// Returns @c true if an attribute was found
    /// @note If the @c defwidth is less than 0, the "mantra" default width
    /// will be used.  Pass in 0 to leave the boxes unchanged.
    bool	enlargeWidth(UT_BoundingBox boxes[],
				int nsegments,
				fpreal defwidth=-1) const;

protected:
    /// Convenience function to compute the point bounding box given an
    /// attribute list.  This tests whether "P" is in homogeneous coordinates
    /// or not.
    void	enlargeP(UT_BoundingBox &box,
			const GT_AttributeListHandle &list,
			int segment) const;

    /// Convenience method to enlarge a bounding box given a position attribute
    void	enlargeP(UT_BoundingBox &B, const GT_DataArrayHandle &P) const;

    /// Convenience method to enlarge a bounding box given P in homogeneous
    /// coordinates.
    void	enlargePw(UT_BoundingBox &B, const GT_DataArrayHandle &P) const;

private:
    // Keep the 32 bit int first in the structure.  Since we inherit from
    // GA_IntrusiveRefCounter, this alignment makes the object smaller.
    mutable int		myCachedDataIdHashSegment : 30,
			myStaticGeoState : 2;
    GT_TransformHandle	myTransform;
    mutable int64	myCachedDataIdHash;
};

#endif
