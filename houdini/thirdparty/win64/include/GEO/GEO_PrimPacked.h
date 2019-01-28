/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimPacked.h (GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PrimPacked__
#define __GEO_PrimPacked__

#include "GEO_API.h"
#include "GEO_PackedTypes.h"
#include "GEO_Primitive.h"
#include <UT/UT_IntrusivePtr.h>

class GEO_PackedNameMap;
using GEO_PackedNameMapPtr = UT_IntrusivePtr<GEO_PackedNameMap>;

/// This is the key associated with the name map hash.  It should not be used
/// by sub-classes for loadSharedLoadData()
#define GEO_SHARED_DATA_NAMEMAP	(('A'<<24)|('m'<<16)|('a'<<8)|('p'))

class GEO_API GEO_PrimPacked : public GEO_Primitive
{
public:
    /// The primitive takes ownership if the @c procedural passed in
    GEO_PrimPacked(GEO_Detail *d,
		    GA_Offset offset = GA_INVALID_OFFSET);

    /// This needs to be outlined in order to avoid including GEO_PackedNameMap.h
    virtual ~GEO_PrimPacked();

    /// @{
    /// Required interface methods for GEO primitive
    virtual bool  	isDegenerate() const = 0;
    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	enlargePointBounds(UT_BoundingBox &box) const;
    virtual void	reverse() {}
    virtual UT_Vector3  computeNormal() const;
    virtual void	copyPrimitive(const GEO_Primitive *src);
    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);
    virtual void	copyImplementation(const GEO_PrimPacked &src) = 0;

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints(GA_PointGroup &grp);
    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);
    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Evalaute a point given a u,v coordinate (with derivatives)
    virtual bool	evaluatePointRefMap(GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				fpreal u, fpreal v, uint du, uint dv) const;
    /// Evalaute position given a u,v coordinate (with derivatives)
    virtual int		evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const
			 {
			    return GEO_Primitive::evaluatePointV4(pos, u, v,
					du, dv);
			 }
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;
    virtual void	stashed(bool beingstashed, GA_Offset offset=GA_INVALID_OFFSET);
    virtual void	transform(const UT_Matrix4 &xform) = 0;
    virtual UT_Vector3	baryCenter() const;
    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const = 0;
    virtual fpreal	calcArea() const = 0;
    virtual fpreal	calcPerimeter() const = 0;
    virtual void	getLocalTransform(UT_Matrix3D &matrix) const;
    virtual void	setLocalTransform(const UT_Matrix3D &matrix);
    virtual void	getPivot(UT_Vector3 &pos) const;
    virtual void	setPivot(const UT_Vector3 &pos);
    /// @}

    /// Return the full transform matrix for the primitive.  This includes the
    /// local transform combined with any additional transforms which might be
    /// implicit to the primitive.
    /// The default behaviour is to simply call @c getLocalTransform4();
    virtual void	getFullTransform4(UT_Matrix4D &matrix) const = 0;

    /// @{
    /// Get the un-transformed bounding box
    virtual bool getUntransformedBounds(UT_BoundingBox &box) const = 0;
    virtual bool getUntransformedRenderingBounds(UT_BoundingBox &box) const = 0;
    virtual void getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax) const = 0;
    /// @}

    /// Method to assist with rendering.  To get accurate bounding boxes for
    /// rendering, the "width" of points and curve objects needs to be taken
    /// into account.  These bounds will include the transform on the primitive.
    ///
    /// Returns false if the bounds are invalid.
    bool	getRenderingBounds(UT_BoundingBox &box) const;

    /// Method to assist with rendering.  When rendering velocity based motion
    /// blur, the renderer needs to know the bounds on the velocity to
    /// accurately compute a bounding box.
    ///
    /// These velocities will *not* include any scales applied by the transform

    /// Set vertex to reference the given point.  This will ensure the vertex
    /// is allocated.
    SYS_FORCE_INLINE
    void setVertexPoint(GA_Offset point)
    {
        if (getVertexCount() == 0)
            myVertexList.append(allocateVertex());

        setPointOffset(0, point);
    }
    /// Called when loading to set the vertex
    void	assignVertex(GA_Offset vtx, bool update_topology);

    /// @{
    /// Accessors for viewport LOD
    GEO_ViewportLOD	 viewportLOD() const	{ return myViewportLOD; }
    void		 setViewportLOD(GEO_ViewportLOD vlod);
    /// @}

    /// @{
    /// Attribute name mappings
    void			setAttributeNameMap(const GEO_PackedNameMapPtr &m);
    const GEO_PackedNameMapPtr &attributeNameMap() const
    {
	if (mySharedNameMapData)
	{
            setupNameMap();
	}
	return myAttributeNameMap;
    }
    /// @}

    /// @{
    /// Faceset name attribute
    void setFacesetAttribute(const char *s)
        { myFacesetAttribute.harden(s); }
    const UT_String &facesetAttribute() const
        { return myFacesetAttribute; }
    /// @}

    /// Fast access to local transform
    const UT_Matrix3D	&localTransform() const	{ return myLocalTransform; }

    /// Fast access to pivot transform
    const UT_Vector3	&pivot() const	{ return myPivot; }
    UT_Vector3		&pivot() { return myPivot; }

    /// @{
    /// Sub-classes should make sure to call the base class dirty methods
    virtual void	transformDirty();
    virtual void	attributeDirty();
    virtual void	topologyDirty();
    /// @}

    virtual bool	saveOptions(UT_Options &options,
				const GA_SaveMap &map) const = 0;
    virtual bool	loadOptions(const UT_Options &options,
				const GA_LoadMap &map) = 0;
    virtual bool	supportsJSONLoad() const = 0;
    virtual bool	loadOptionsJSON(UT_JSONParser &p,
				const GA_LoadMap &map) = 0;
    virtual bool	loadOptionsJSONMap(UT_JSONValueMap &options,
				const GA_LoadMap &map) = 0;
    virtual bool	loadUnknownToken(const char *token,
				UT_JSONParser &p,
				const GA_LoadMap &map) = 0;

    /// @{
    /// Any sub-classes *must* call the GEO method for shared load data since
    /// name maps are saved as shared data.
    ///
    /// The sub-classes cannot use GEO_SHARED_DATA_NAMEMAP as a key.
    virtual bool	saveSharedLoadData(UT_JSONWriter &w, GA_SaveMap &map, GA_GeometryIndex* geometryIndex) const;
    virtual bool	registerSharedLoadData(int load_data_type, GA_SharedDataHandlePtr item);
    /// @}

    /// Copies the member data of the source packed primitive into this.
    /// This does not modify the vertex list or any attributes.
    void copyMemberDataFrom(const GEO_PrimPacked &src);

private:
    void setupNameMap() const;
protected:
    virtual int		 evaluateInteriorPointV4(UT_Vector4 &pos,
				       fpreal u, fpreal v, fpreal w = 0) const;

    UT_Vector3                  myPivot;
    UT_Matrix3D			myLocalTransform;
    GEO_PackedNameMapPtr	myAttributeNameMap;
    UT_String                   myFacesetAttribute;
    GEO_ViewportLOD		myViewportLOD;
    GA_SharedDataHandlePtr	mySharedNameMapData;
};

#endif
