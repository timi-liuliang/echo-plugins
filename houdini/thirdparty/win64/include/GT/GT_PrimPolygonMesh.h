/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimPolygonMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPolygonMesh__
#define __GT_PrimPolygonMesh__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_CountArray.h"
#include "GT_FaceSetMap.h"
#include <GA/GA_Names.h>

class GT_DataArray;
class GT_PrimPolygon;
class GT_AttributeList;
class GT_AttributeMerge;
class UT_StringRef;

/// @brief A mesh of polygons
class GT_API GT_PrimPolygonMesh : public GT_Primitive
{
public:
    /// Default constructor
    GT_PrimPolygonMesh()
	: myVMerge(NULL)
	, myUMerge(NULL)
	, myConvexFlag(false)
    {     
    }
    /// Useful constructor
    GT_PrimPolygonMesh(const GT_DataArrayHandle &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			GT_IndexingMode indexing=GT_INDEXING_QUICK,
			GT_Size min_vertex_count=0,
			GT_Size max_vertex_count=0)
	: myVMerge(NULL)
	, myUMerge(NULL)
	, myConvexFlag(false)
    {
	init(vtx_counts, vtx_indices, shared, vertex, uniform, detail,
		    indexing, min_vertex_count, max_vertex_count);
    }

    GT_PrimPolygonMesh(const GT_CountArray &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail)
	: myVMerge(nullptr)
	, myUMerge(nullptr)
	, myConvexFlag(false)
    {
	init(vtx_counts, vtx_indices, shared, vertex, uniform, detail);
    }

    /// Copy the topology information from the source pmesh, but use different
    /// attribute lists.
    GT_PrimPolygonMesh(const GT_PrimPolygonMesh &pmesh,
		       const GT_AttributeListHandle &shared,
		       const GT_AttributeListHandle &vertex,
		       const GT_AttributeListHandle &uniform,
		       const GT_AttributeListHandle &detail);

    /// Create a new polygon mesh with re-mapped vertices (used by
    /// removeUnusedPoints).
    ///
    /// Since the polygon faces are remapped you'll have to pass in a new face
    /// set map.
    GT_PrimPolygonMesh(const GT_PrimPolygonMesh &pmesh,
		       const GT_DataArrayHandle &vtx_indices,
		       const GT_AttributeListHandle &shared);
    
    /// Copy c-tor
    GT_PrimPolygonMesh(const GT_PrimPolygonMesh &pmesh);

    /// Construct a polygon mesh from a GT_PrimPolygon
    GT_PrimPolygonMesh(const GT_PrimPolygon &poly);
    /// Destructor
    virtual ~GT_PrimPolygonMesh();

    virtual const char *className() const { return "GT_PrimPolygonMesh"; }
    virtual bool	save(UT_JSONWriter &w) const;
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual void		enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual int			getPrimitiveType() const;
    virtual bool		refine(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    virtual int			getMotionSegments() const;
    virtual int64		getMemoryUsage() const;
    /// @}

    /// @{
    /// Initialize the mesh
    /// - @c vtx_counts @n
    ///   An integer array, representing the number of vertices in each face.
    ///   The length of the array determines the number of faces in the mesh.
    /// - @c vtx_indices @n
    ///   An array of vertex references.  There is an entry for every vertex in
    ///   every face.  These indices refer to the shared attribute data.
    /// - @c shared @n
    ///   Shared attribute data.  This data is referenced by the vertex arrays.
    /// - @c vertex @n
    ///   Unique data per vertex.  The length of these attribute arrays should
    ///   be the same as the length of the vtx_indices array.
    /// - @c uniform @n
    ///   Attribute per face.  There should be one entry for each entry in the
    ///   @c vtx_counts array.
    /// - @c detail @n
    ///   Constant attribute for all faces.
    ///   @c vtx_counts array.
    /// - @c indexing @n
    ///   What type of indexing is required
    void		init(const GT_DataArrayHandle &vtx_counts,
			     const GT_DataArrayHandle &vtx_indices,
			     const GT_AttributeListHandle &shared,
			     const GT_AttributeListHandle &vertex,
			     const GT_AttributeListHandle &uniform,
			     const GT_AttributeListHandle &detail,
			     GT_IndexingMode indexing=GT_INDEXING_QUICK,
			     GT_Size min_vertex_count=0,
			     GT_Size max_vertex_count=0);
    void		init(const GT_CountArray &vtx_counts,
			     const GT_DataArrayHandle &vtx_indices,
			     const GT_AttributeListHandle &shared,
			     const GT_AttributeListHandle &vertex,
			     const GT_AttributeListHandle &uniform,
			     const GT_AttributeListHandle &detail);
    /// @}

    /// Return the number of faces in the mesh
    GT_Size			getFaceCount() const;
    /// Return a pointer to the individual face
    GT_PrimitiveHandle		getFace(GT_Offset i) const;

    /// @{
    /// Query the minimum/maximum number of vertices per face
    GT_Size	getMinVertexCount() const { return myFaceOffsets.getMinCount();}
    GT_Size	getMaxVertexCount() const { return myFaceOffsets.getMaxCount();}
    /// @}

    /// @{
    /// Accessor
    virtual const GT_DataArrayHandle &getVertexList() const
				    { return myVertexList; }
    const GT_CountArray		&getFaceCountArray() const
				    { return myFaceOffsets; }
#if 0
    const GT_DataArrayHandle	&getFaceOffsets() const
				    { return myFaceOffsets; }
#endif
    const GT_AttributeListHandle &getShared() const
				    { return myShared; }
    const GT_AttributeListHandle &getVertex() const
				    { return myVertex; }
    const GT_AttributeListHandle &getUniform() const
				    { return myUniform; }
    const GT_AttributeListHandle &getDetail() const
				    { return myDetail; }
    /// @}

    /// @{
    /// Access to the face sets
    const GT_FaceSetMapPtr	&faceSetMap() const	{ return myFaceSetMap; }
    void			 setFaceSetMap(const GT_FaceSetMapPtr &v)
					{ myFaceSetMap = v; }
    void			 addFaceSet(const UT_StringHolder &name,
					const GT_FaceSetPtr &set);
    /// @}

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimPolygonMesh(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getPointAttributes() const
						    { return myShared; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    /// Return the offset into the vertex list for the given face
    GT_Offset		getVertexOffset(GT_Offset face) const
			    { return myFaceOffsets.getOffset(face); }
    /// Return the length of the vertex list for the given face
    GT_Size		getVertexCount(GT_Offset face) const
			    { return myFaceOffsets.getCount(face); }
    /// For a given face, find the shared point numbers for the given vertex
    /// and the next vertex (i.e. the edge starting from the given vertex).
    void		getEdgePoints(GT_Offset face, GT_Offset vertex,
				GT_Offset &p0, GT_Offset &p1) const;

    /// Returns true if this mesh has been convexed
    bool		isConvexed() const { return myConvexFlag; }

    /// Set the convex flag on the mesh
    void		setConvexed(bool c) { myConvexFlag = c; }

    /// Return an array containing the face counts per-face
    GT_DataArrayHandle
		getFaceCounts(GT_IndexingMode indexing=GT_INDEXING_QUICK) const;

    /// The polygon mesh may have shared points which aren't used.  That is,
    /// there may be elements in the shared element list which aren't
    /// referenced by any vertex.  This method will build a list of indices for
    /// all the used points.
    ///
    /// The list returned will be monotonically ascending.
    const GT_DataArrayHandle
		getUsedPointList(GT_IndexingMode idx=GT_INDEXING_QUICK) const;

    /// Ensure all polygons in the mesh are convex
    GT_PrimitiveHandle		convex(int max_points_per_poly=3,
				       bool keep_degenerate = false,
				       bool allow_interrupt = true,
				       bool allow_indirect_flattening=0) const;

    /// Using the indirect arrays and vertex list from a convexed mesh,
    /// convex this mesh (assumes topology is the same).
    GT_PrimitiveHandle		adoptConvexing(
				    const GT_DataArrayHandle &verts,
				    const GT_DataArrayHandle &uniform_indexing,
				    const GT_DataArrayHandle &vertex_indexing,
				    const GT_DataArrayHandle &vert_info,
				    const GT_DataArrayHandle &prim_info)
				const;

    /// Fetches the convexed arrays from the mesh, to be used by adoptConvexing
    /// on a different mesh with the same topology.
    void			getConvexArrays(
				    GT_DataArrayHandle &verts,
				    GT_DataArrayHandle &uniform_indexing,
				    GT_DataArrayHandle &vertex_indexing,
				    GT_DataArrayHandle &vert_info,
				    GT_DataArrayHandle &prim_info) const;

    /// Remove unused points.  This will collapse the varying point arrays,
    /// leaving only the points referenced by the vertices.
    GT_PrimitiveHandle		removeUnusedPoints(const int32 *idx=NULL) const;

    /// @{
    /// Compute face normals.  This will use the shared attribute named P to
    /// compute the normals for each face.
    /// fpreal16 must have the proper vector length
    bool			faceNormals(UT_Vector3 *N,
					int segment=0,
					const UT_StringRef &P=GA_Names::P) const;
    bool			faceNormals(UT_Vector3D *N64,
					int segment=0,
					const UT_StringRef &P=GA_Names::P) const;
    bool			faceNormals(fpreal16 *N16,
					int segment=0,
					const UT_StringRef &P=GA_Names::P) const;
    GT_DataArrayHandle		faceNormals(int segment=0,
					const UT_StringRef &P=GA_Names::P,
					GT_Storage store=GT_STORE_REAL32) const;
    /// @}

    /// @{
    /// Compute shared normals.  This will use the shared attribute named P to
    /// compute the normals for each shared point.
    bool			pointNormals(UT_Vector3T<fpreal16> *N,
					GT_Size npts,
					int segment=0,
					const UT_StringRef &P=GA_Names::P,
					bool normalize=true,
					const fpreal32 *pntdata = NULL) const;
    bool			pointNormals(UT_Vector3 *N, GT_Size npts,
					int segment=0,
					const UT_StringRef &P=GA_Names::P,
					bool normalize=true,
					const fpreal32 *pntdata = NULL) const;
    bool			pointNormals(UT_Vector3D *N, GT_Size npts,
					int segment=0,
					const UT_StringRef &P=GA_Names::P,
					bool normalize=true,
					const fpreal32 *pntdata = NULL) const;
    virtual GT_DataArrayHandle	createPointNormals(int segment=0,
				       const UT_StringRef &P=GA_Names::P,
				       bool normalize=true,
				       const fpreal32 *pntdata = NULL,
				       GT_Storage store =GT_STORE_REAL32) const;
    /// @}


    /// @brief Create point normals on a new mesh if no normals are found.
    /// If no point or vertex normals are found, generate point normals from P
    /// and return a new mesh. If normals are found, return NULL.
    GT_PrimPolygonMesh *createPointNormalsIfMissing(int segment=0,
					const UT_StringRef &P=GA_Names::P,
					bool normalize=true) const;
    
    /// @brief Divide a mesh into smaller meshes if it exceeds the poly_limit.
    /// If this mesh has more than polygon_limit polygons, this will split the
    /// mesh into multiple sub-meshes, attempting to keep them all roughly the
    /// same number of polygons. If this mesh has fewer than polygon_limit
    /// polygons, it will return false and not add any meshes to split_meshes.
    /// If remove_unused_points is true, each submesh will have unused points
    // removed.
    bool	splitMesh(GT_Size polygon_limit,
			  UT_Array<GT_PrimitiveHandle> &split_meshes,
			  bool remove_unused_points = true) const;

    /// @{
    /// Methods for GEO/GU support.
    virtual fpreal	computePerimeter(int seg) const;
    virtual fpreal	computeSurfaceArea(int seg) const;
    virtual fpreal	computeVolume(const UT_Vector3 &ref_P, int seg) const;
    /// @}

    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

protected:
    void			 hardenAttributes();

    // We have "clone" methods so that sub-classes (i.e.
    // GT_PrimSubdivisionMesh) can create an appropriate clone.  These are used
    // by methods internal when duplicating the mesh (i.e. removeUnusedPoints)
    virtual GT_PrimPolygonMesh	*clone(const GT_DataArrayHandle &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			GT_IndexingMode indexing=GT_INDEXING_QUICK,
			GT_Size min_vertex_count=0,
			GT_Size max_vertex_count=0) const
    {
	return new GT_PrimPolygonMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail, indexing,
		min_vertex_count, max_vertex_count);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_CountArray &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail) const
    {
	return new GT_PrimPolygonMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_AttributeListHandle &shared,
		       const GT_AttributeListHandle &vertex,
		       const GT_AttributeListHandle &uniform,
		       const GT_AttributeListHandle &detail) const
    {
	return new GT_PrimPolygonMesh(*this, shared, vertex, uniform, detail);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_DataArrayHandle &vtx_indices,
		       const GT_AttributeListHandle &shared) const
    {
	return new GT_PrimPolygonMesh(*this, vtx_indices, shared);
    }

    GT_AttributeListHandle	 myShared;
    GT_AttributeListHandle	 myVertex;
    GT_AttributeListHandle	 myUniform;
    GT_AttributeListHandle	 myDetail;

private:
    template <typename VECTOR_T>
    bool			 computePointNormals(
					UT_Vector3T<VECTOR_T> *N,
					GT_Size Nsize,
					const UT_StringRef &P,
					int segment,
					bool normalize,
					const fpreal32 *point_data) const;

    void			 makeMerge();

    GT_DataArrayHandle		 myVertexList;
    GT_CountArray		 myFaceOffsets;
    GT_FaceSetMapPtr		 myFaceSetMap;
    GT_AttributeMerge		*myVMerge, *myUMerge;
    bool			 myConvexFlag;
};

#endif

