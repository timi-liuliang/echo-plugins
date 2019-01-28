/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_CatPolygonMesh.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_CatPolygonMesh__
#define __GT_CatPolygonMesh__

#include "GT_API.h"
#include "GT_Handles.h"
#include <UT/UT_Array.h>

class GT_PrimPolygonMesh;

/// Concatenate GT_PrimPolygonMesh objects into a single GT_PrimPolygonMesh
class GT_API GT_CatPolygonMesh
{
public:
    GT_CatPolygonMesh(bool merge_detail_attribs = true,
		      int64 total_num_faces = SYS_INT64_MAX,
		      int64 max_meshes = SYS_INT64_MAX,
		      int64 max_mesh_face_count = SYS_INT64_MAX);

    GT_CatPolygonMesh(const UT_Array<GT_PrimitiveHandle> &meshes,
		      bool merge_detail_attribs = true);

    /// If 'merge_detail_attribs' is true, the detail lists will be promoted
    /// to uniform attributes. If false, only the first meshes detail attribs
    /// are used (useful for merging meshes built from the same GU_Detail).
    /// default is to merge.
    void	mergeDetailLists(bool merge_detail_lists)
		    { myMergeDetailLists = merge_detail_lists; }

    /// Limit the # of faces in the mesh. append will return false once full.
    void	setTotalFaceCount(int64 max_num_faces)
		    { myTotalFaceCount = max_num_faces; }
    /// Limit the # of consituent meshes in the mesh. append will return false
    /// once full.
    void	setMaxMeshCount(int64 max_meshes)
		    { myMaxMeshCount = max_meshes; }
    /// Don't allow meshes larger than this size to be added.
    void	setMaxMeshFaceCount(int64 max_faces)
		    { myMaxMeshFaceCount = max_faces; }

    enum gt_CatAppendReason
    {
	APPEND_OK,
	APPEND_UNSUPPORTED_TYPE,// not a poly mesh
	APPEND_TOO_MANY_MESHES,	// collected mesh exceeds the mesh count
	APPEND_TOO_MANY_FACES,	// collected mesh exceeds total #faces
	APPEND_NO_FACES,	// mesh has no faces
	APPEND_MESH_TOO_LARGE,	// mesh exceeds max mesh face count
	APPEND_ATTRIB_MISMATCH	// mesh attributes don't match collected mesh's
    };

    /// Test if the primitive can be added, without adding it.
    bool	canAppend(const GT_PrimitiveHandle &prim,
			  gt_CatAppendReason *reason = NULL) const;
    
    /// Append a polygon mesh.  Returns false if the primitive isn't valid
    bool	append(const GT_PrimitiveHandle &prim);

    /// Append a polygon mesh.  Returns false if the primitive isn't valid
    bool	append(const GT_PrimitiveHandle &prim,
		       const GT_TransformHandle &prim_transform);

    exint	getNumSourceMeshes() const { return myMeshes.entries(); }

    /// Return the concatenation of all the polygon meshes
    GT_PrimitiveHandle	result() const;

private:
    bool	matchAttributes(const GT_PrimPolygonMesh &pmesh) const;
    const GT_PrimPolygonMesh	*getMesh(exint i) const
	{ return UTverify_cast<const GT_PrimPolygonMesh *>(myMeshes(i).get()); }

    UT_Array<GT_PrimitiveHandle>	myMeshes;
    bool				myMergeDetailLists;
    int64				myFaceCount;
    int64				myTotalFaceCount;
    int64				myMaxMeshCount;
    int64				myMaxMeshFaceCount;
};

#endif

