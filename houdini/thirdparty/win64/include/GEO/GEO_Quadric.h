/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This is the base class for Quadratics (Sphere, Tube, Cone, Blob, etc.)
 *	Since these quadrics may have any rotation, there are axes kept
 *	for the orientation.  These axes are maintained by the base class.
 *	This class doesn't really mean quadric/quadratic functions. In practice,
 *	it has come to mean "this primitive has a transform."
 *
 */

#ifndef __GEO_Quadric_H__
#define __GEO_Quadric_H__

#include "GEO_API.h"
#include <UT/UT_Matrix3.h>
#include "GEO_Primitive.h"

#include <UT/UT_VectorTypes.h>

class GEO_Detail;

// For the for loop, we can use getFastVertex since we know the index is valid

class GEO_API GEO_Quadric : public GEO_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_Quadric(GEO_Detail *d, GA_Offset offset = GA_INVALID_OFFSET);

#if !GA_PRIMITIVE_VERTEXLIST
    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_Quadric();
#endif

public:
//
//  Methods common to all primitives.
    virtual int		 getBBox(UT_BoundingBox *bbox) const = 0;
    /// @{
    /// Enlarge a bounding box by the bounding box of the primitive.  A
    /// return value of false indicates an error in the operation, most
    /// likely an invalid P.  For any attribute other than the position
    /// these methods simply enlarge the bounding box based on the vertex.
    virtual bool	 enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool	 enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    virtual void	 enlargePointBounds(UT_BoundingBox &e) const;
    /// @}
    /// Enlarge a bounding sphere to encompass the primitive.  A return value
    /// of false indicates an error in the operation, most likely an invalid
    /// P.  For any attribute other than the position this method simply
    /// enlarges the sphere based on the vertex.
    virtual bool	 enlargeBoundingSphere(UT_BoundingSphere &b,
					const GA_Attribute *P) const;
    virtual UT_Vector3	 baryCenter() const;
    virtual UT_Vector3	 computeNormal() const;
    virtual bool	 saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	 loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    // Transforms the matrix associated with this primitive.  The
    // translate component is ignored: Translate the vertices of
    // the primitive to translate the primitive.
    // This only works with quadrics (sphere, tube, metaballs) and volumes.
    virtual void		 transform(const UT_Matrix4 &mat);

    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;
    virtual void	copyPrimitive(const GEO_Primitive *src);
    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	addPointRefToGroup(GA_PointGroup &grp) const;

    virtual bool	vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;
#endif
    virtual void reverse() {}

#if GA_PRIMITIVE_VERTEXLIST
    using GEO_Primitive::getVertexOffset;
    using GEO_Primitive::getPointOffset;
    using GEO_Primitive::getPointIndex;
#else
    virtual GA_Size	getVertexCount() const;
    virtual GA_Offset	getVertexOffset(GA_Size index) const;
#endif

    /// NOTE: Getting the vertex count of a quadric is rather unnecessary.
    SYS_FORCE_INLINE
    GA_Size             getFastVertexCount() const
    { return 1; }
    /// The fast vertex offset accessor
    SYS_FORCE_INLINE
    GA_Offset           getVertexOffset() const
#if GA_PRIMITIVE_VERTEXLIST
    { return getVertexOffset(0); }
#else
    { return myVertex; }
#endif

    /// The fast vertex index accessor
    SYS_FORCE_INLINE
    GA_Index            getVertexIndex() const
#if GA_PRIMITIVE_VERTEXLIST
    { return GEO_Primitive::getVertexIndex(0); }
#else
    { return getDetail().vertexIndex(getVertexOffset()); }
#endif
    /// The fast point offset accessor
    SYS_FORCE_INLINE
    GA_Offset           getPointOffset() const
#if GA_PRIMITIVE_VERTEXLIST
    { return GEO_Primitive::getPointOffset(0); }
#else
    { return getDetail().vertexPoint(myVertex); }
#endif
#if !GA_PRIMITIVE_VERTEXLIST
    /// NOTE: This version is just here to avoid hiding
    /// GA_Primitive::getPointOffset without a replacement.
    GA_Offset           getPointOffset(GA_Size i) const
    {
        UT_ASSERT_P(i == 0);
        return getDetail().vertexPoint(myVertex);
    }
#endif
    /// The fast point index accessor
    SYS_FORCE_INLINE
    GA_Index            getPointIndex() const
#if GA_PRIMITIVE_VERTEXLIST
    { return GEO_Primitive::getPointIndex(0); }
#else
    { return getDetail().pointIndex(getPointOffset()); }
#endif
#if !GA_PRIMITIVE_VERTEXLIST
    /// NOTE: This version is just here to avoid hiding
    /// GA_Primitive::getPointIndex without a replacement.
    GA_Index            getPointIndex(GA_Size i) const
    {
        UT_ASSERT_P(i == 0);
        return getDetail().pointIndex(getPointOffset());
    }
#endif
    /// The fast point position accessor
#if GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE
    UT_Vector3 getPos3() const
    { return GEO_Primitive::getPos3(0); }
    SYS_FORCE_INLINE
    void setPos3(const UT_Vector3 &pos) const
    { GEO_Primitive::setPos3(0, pos); }
    SYS_FORCE_INLINE
    UT_Vector4 getPos4() const
    { return GEO_Primitive::getPos4(0); }
    SYS_FORCE_INLINE
    void setPos4(const UT_Vector4 &pos) const
    { GEO_Primitive::setPos4(0, pos); }
    SYS_FORCE_INLINE
    void setPointOffset(GA_Offset ptoff)
    { GEO_Primitive::setPointOffset(0, ptoff); }
#else
    UT_Vector3          getPos3() const
    { return getDetail().getPos3(getPointOffset()); }
    void		setPos3(const UT_Vector3 &pos) const
    { getDetail().setPos3(getPointOffset(), pos); }
    UT_Vector4		getPos4() const
    { return getDetail().getPos4(getPointOffset()); }
    void		setPos4(const UT_Vector4 &pos) const
    { getDetail().setPos4(getPointOffset(), pos); }
    void                setPointOffset(GA_Offset ptoff) const
    { getDetail().getTopology().wireVertexPoint(getVertexOffset(), ptoff); }
#endif

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints (GA_PointGroup &grp);

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

    virtual bool	 isDegenerate() const;

//
//  Methods to set the space of a primitive
    void 	 	getAxes(UT_Vector3 &x, UT_Vector3 &y,
				UT_Vector3 &z) const;
    void		getXAxis(UT_Vector3 &x) const;
    void		getYAxis(UT_Vector3 &y) const;
    void		getZAxis(UT_Vector3 &z) const;

    void 	 	setAxes(const UT_Vector3 &x, const UT_Vector3 &y,
				const UT_Vector3 &z);
    void		setXAxis(const UT_Vector3 &x);
    void		setYAxis(const UT_Vector3 &y);
    void		setZAxis(const UT_Vector3 &z);

    const UT_Matrix3	&getTransform(void) const { return myXform; }
    void		 setTransform(const UT_Matrix3 &m) { myXform = m; }

    void		 getTransform4(      UT_Matrix4 &matx) const;
    void		 setTransform4(const UT_Matrix4 &matx);

    virtual void	 getLocalTransform(UT_Matrix3D &x) const;
    virtual void	 setLocalTransform(const UT_Matrix3D &x);

    SYS_FORCE_INLINE
    void setVertexPoint(GA_Offset pt)
    {
        setPointOffset(pt);
    }

    /// This method assigns a preallocated vertex to the quadric, optionally
    /// creating the topological link between the primitive and new vertex.
    void		 assignVertex(GA_Offset new_vtx, bool update_topology);

    /// Builds a quadric primitive of the specified type, with either the
    /// specified point offset, or a new point if the provided offset
    /// is invalid.
    /// NOTE: type must be GA_PRIMSPHERE, GA_PRIMCIRCLE, GA_PRIMTUBE,
    ///       GA_PRIMMETABALL, or GA_PRIMMETASQUAD.
    static GEO_Quadric *build(GA_Detail &detail,
                            GA_PrimitiveTypeId type,
                            GA_Offset existing_ptoff=GA_INVALID_OFFSET);

    /// Builds a contiguous block of nprims quadric primitives of the specified
    /// type, appending a block of new points for them if append_points is true,
    /// else leaving the vertices unwired.
    /// NOTE: type must be GA_PRIMSPHERE, GA_PRIMCIRCLE, GA_PRIMTUBE,
    ///       GA_PRIMMETABALL, or GA_PRIMMETASQUAD.
    static GA_Offset buildBlock(GA_Detail &detail,
                            GA_PrimitiveTypeId type,
                            GA_Size nprims,
                            bool append_points=true);

    // Have we been deactivated and stashed?
    virtual void	 stashed(bool beingstashed,
					GA_Offset offset = GA_INVALID_OFFSET);

    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm)const;
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm, float tolerance)const;

    virtual void	 unitToUnitLengthPair(float  uparm,   float  vparm,
					      float &ulength, float &vlength)
									const;

    // Is this primitive a GEO_Quadric?
    virtual bool	 isQuadric() const { return true; }

protected:
    /// Return the family mask for all quadric sub-classes
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GEO_FAMILY_QUADRIC; }

    // Declare intrinsic attribute methods
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE);

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	clearForDeletion();
#endif

    virtual bool 	savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool 	loadPrivateH9(UT_IStream &is);

    SYS_FORCE_INLINE
    GA_Offset vertexPoint() const
    { return getPointOffset(); }

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

    virtual bool	 evaluateBaryCenterRefMap(GA_Offset result_vertex,
					GA_AttributeRefMap &hlist) const;

private:
#if !GA_PRIMITIVE_VERTEXLIST
    GA_Offset		myVertex;	// My vertex
#endif
    UT_Matrix3		myXform;	// My transform

    friend std::ostream	&operator<<(std::ostream &os, const GEO_Quadric &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
