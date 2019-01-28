/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	Paste surface class.
 *
 */

#ifndef __GEO_PrimPasteDAG_h__
#define __GEO_PrimPasteDAG_h__

#include "GEO_API.h"
#include "GEO_PasteSurfDAG.h"
#include "GEO_Primitive.h"

class GA_Detail;

class GEO_API GEO_PrimPasteDAG : public GEO_Primitive, public GEO_PasteSurfDAG
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimPasteDAG(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimPasteDAG() {}

public:
    virtual bool	 isDegenerate() const;

    virtual bool	 saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	 loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    bool		 saveFeatures(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		 loadFeatures(UT_JSONParser &p,
				const GA_LoadMap &map);

    virtual int		 getBBox(UT_BoundingBox *bbox) const;
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;
    /// @{
    /// Enlarge a bounding box by the bounding box of the primitive.  A
    /// return value of false indicates an error in the operation, most
    /// likely an invalid P.
    virtual bool	 enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool	 enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    /// @}
    /// Enlarge a bounding sphere to encompass the primitive.  A return value
    /// of false indicates an error in the operation, most likely an invalid
    /// P.
    virtual bool	 enlargeBoundingSphere(UT_BoundingSphere &b,
					const GA_Attribute *P) const;
    virtual UT_Vector3	 baryCenter() const;
    virtual UT_Vector3	 computeNormal() const;
    virtual void	 reverse();

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	 addPointRefToGroup(GA_PointGroup &grp) const;
#endif
    void                 addSurfPointRefsToGroup(GA_PointGroup &grp) const;

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints(GA_PointGroup &grp);

    /// WARNING: This is NOT equivalent to GEO_Primitive::isPointUsed!
    ///          This checks if the GEO_PrimPasteDAG's surfaces use the
    ///          point, whereas GEO_Primitive::isPointUsed does not.
    bool		 isPointUsed(GA_Offset ptoff) const;

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

#if !GA_PRIMITIVE_VERTEXLIST
    virtual GA_Size	 getVertexCount() const;
    virtual GA_Offset	 getVertexOffset(GA_Size index) const;

    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;
#endif

    virtual int		 isPrimary(void) const;

    virtual void	 copyPrimitive(const GEO_Primitive *src);
    virtual GEO_Primitive*copy(int preserve_shared_pts = 0) const;

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

    virtual void	 clearForDeletion();

    // Convert the real domain values of the primitive to unit values to be
    // used in the evaluation methods below:
    virtual void	 realToUnitPair(float  ureal, float  vreal,
					float &uunit, float &vunit) const;
    // and vice versa
    virtual void	 unitToRealPair(float  uunit, float  vunit,
					float &ureal, float &vreal) const;

    virtual int		 evaluateNormalVector(UT_Vector3 &nml, float u,
					float v = 0, float w = 0) const;

    virtual int		 parametricBBox(float u, float v, 
					float *u0, float *u1,
					float *v0, float *v1);

    // Returns distance between two points in parameter space, aware
    // of any possible wrapping.
    virtual float	 uvDist(float u1, float v1, float u2, float v2) const;

    // Tell the dag it's obsolete. This will happen when the dag has been
    // pasted onto some other dag, and should be deleted. This method clears
    // the nodes and deletes "this", so don't use the dag thereafter.
    virtual void	 obsolete(void);

    // Remove all the references to GEO_TPSurf and clear.
    virtual void	 isolate(void);

    // Add all the TPSurfs to the given group. We DON'T CLEAR the group first.
    void		 toGroup(GA_PrimitiveGroup &surfgroup) const;

    // Resolve all the load links: parenting of domains and their frames,
    // base surfaces, CV displacements, pointers to TPSurf, etc. Return 0
    // if OK and -1 otherwise. Call this method only after loading the gdp.
    int			 resolveLinks(void);

    // Resolve the remaining copy links, ie. the pointers to the spline
    // surfaces. What we carry now is the indices of the spline primitives
    // in the source DAG. Return 0 if all resolved, -1 otherwise.
    int			 resolveSplines(const GEO_Detail &srcgdp, 
				const UT_Array<GA_Primitive *> &prim_map);

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Stash (deactivate) or unstash (reactivate) the primitive.
    virtual void	stashed(bool beingstashed,
				GA_Offset offset = GA_INVALID_OFFSET);

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_NONE; }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Primitive::registerIntrinsics(defn); }

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

    // Evaluate the position, the derivative or the normal at domain point
    // (u,v), where u and v MUST be in [0,1]. "v" and "dv" will be ignored
    // when dealing with one-dimensional types such as circles and polygons.
    // Return 0 if OK and -1 otherwise. The normal is not normalized.
    virtual bool	 evaluatePointRefMap(GA_Offset result,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v,
					unsigned du, unsigned dv) const;

    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;
    virtual bool	 evaluateBaryCenterRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist) const;

private:
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
