/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_Utils.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Utility functions for GR.
 */
#ifndef GR_Utils_h
#define GR_Utils_h

#include "GR_API.h"
#include "GR_Defines.h"
#include "GR_UpdateParms.h"

#include <UT/UT_VectorTypes.h>
#include <RE/RE_Types.h>
#include <RE/RE_CachedObject.h>
#include <GU/GU_SelectType.h>
#include <GA/GA_Types.h>
#include <GT/GT_Types.h>
#include <GT/GT_Primitive.h>

class RE_Render;
class RE_VertexArray;
class RE_VertexArrayRef;
class RE_Geometry;
class RE_Shader;
class GR_DisplayOption;

/// Utility GR methods common to all renderers
namespace GR_Utils
{
/// Returns the selection type in p.geometry
GR_API extern GR_AttribMask hasSelection(const GR_UpdateParms &p);
 
/// Returns true if prim_id is in the selection list for p.geometry
/// If geo is given it must be the unlocked p.geometry from a ReadLock
GR_API extern bool inPrimitiveSelection(const GR_UpdateParms &p,
					GA_Offset prim_id,
					const GU_Detail *geo = nullptr);
    
/// Returns true if point_id is in the selection list for p.geometry
GR_API extern bool inPointSelection(const GR_UpdateParms &p,
				    GA_Offset point_id,
				    const GU_Detail *geo = nullptr);

/// Returns true if vert_id is in the selection list for p.geometry
GR_API extern bool inVertexSelection(const GR_UpdateParms &p,
				     GA_Offset vert_id,
				     const GU_Detail *geo = nullptr);

/// @brief Checks if passed prim offsets are in p.geometry's selection.
/// Returns GR_SELECT_PRIM_PARTIAL/FULL if some or all of the prim offsets
/// are in the cook or temp selections, as indicated by the parms in p,
/// otherwise GR_SELECT_NONE is returned. The individual selection results 
/// are returned in 'selected'
GR_API extern GR_SelectMode inPrimitiveSelection(const GR_UpdateParms &p,
						 const UT_Array<GA_Offset>&pids,
						 UT_BitArray &selected);
    
/// @brief Checks if passed point offsets are in p.geometry's selection.
/// Returns GR_SELECT_POINT_PARTIAL/FULL if some or all of the point offsets
/// are in the cook or temp selections, as indicated by the parms in p,
/// otherwise GR_SELECT_NONE is returned. The individual selection results
/// are returned in 'selected'
GR_API extern GR_SelectMode inPointSelection(const GR_UpdateParms &p,
					     const UT_Array<GA_Offset> &pids,
					     UT_BitArray &selected);

/// @brief Checks if passed vertex offsets are in p.geometry's selection.
GR_API extern GR_SelectMode inVertexSelection(const GR_UpdateParms &p,
					      const UT_Array<GA_Offset> &vids,
					      UT_BitArray &selected);

/// Return the cache version for attribute 'ph'
GR_API extern RE_CacheVersion getAttribVersion(const GT_DataArrayHandle &ph,
					       RE_CacheVersion geo_version);
    
/// Return the cache version for the topology of primitive 'prim'
GR_API extern RE_CacheVersion getTopologyVersion(const GT_Primitive *prim,
						 RE_CacheVersion geo_version);

/// Return true if the geometry has a per-prim transform or per-prim visibility 
GR_API extern bool hasPrimTransform(const RE_Geometry *geo);

// Call setupGhosting() to set up wire-ghosting (or other sample-based
// ghosting effects). opaque_data[] is just used to clean up correctly.
// transparency (1.0 - opacity) is rounded up to the nearest multiple of
// 1/s where s is the number of samples (at least 4 for ghosting to work).
GR_API extern void	setupGhosting  (RE_Render *r, int opaque_data[3],
					fpreal transparency = 0.25f);
GR_API extern void	cleanupGhosting(RE_Render *r, int opaque_data[3]);

/// Convenience method to return an RE type from a GT storage type.
inline RE_GPUType getREType(GT_Storage s)
{
    switch(s)
    {
    case GT_STORE_UINT8:	return RE_GPU_UINT8;
    case GT_STORE_INT32:	return RE_GPU_INT32;
    case GT_STORE_INT64:	return RE_GPU_INT32; // no such 64b type
    case GT_STORE_REAL16:	return RE_GPU_FLOAT16;
    case GT_STORE_REAL32:	return RE_GPU_FLOAT32;
    case GT_STORE_REAL64:	return RE_GPU_FLOAT64;
    case GT_STORE_STRING:	return RE_GPU_INT32; // string indices
    default:
	UT_ASSERT(!"No available RE type");
	return RE_GPU_UINT1;
	break;
    }
}

GR_API extern void	assignInstanceGroupArray(RE_Geometry *to_geo,
						 const RE_VertexArray *array,
						 int instance_group =-1);
GR_API extern bool	removeInstanceGroupArray(RE_Geometry *from_geo,
						 int instance_group,
						 const char *attrib_name,
						 RE_GenericAttribID id
						    = RE_GENATTRIB_NONE);

/// Returns the GPU type and vector size of the attribute based on the
/// data array, display option settings and supported GL capabilities.
GR_API extern RE_GPUType      getAttributeType(RE_Render *r,
					       const char *attrib_name,
					       int &vsize,
					       const GT_DataArrayHandle &pa,
					       const GR_DisplayOption &opts,
					       RE_ArrayType array_type);


/// Builds an instance transform array  for p.instances on 'geo'. The
/// optional transform parameter will be set to the primitive transform
/// Returns 'true' if the primitive is instanced. 
GR_API extern bool	buildInstanceObjectMatrix(RE_Render *r,
					  const GT_PrimitiveHandle &h,
					  const GR_UpdateParms &p,
					  RE_Geometry *geo,
					  RE_CacheVersion version,
					  int instance_group = 0,
					  UT_Matrix4D *transform = NULL);
    
/// Builds an instance index mapping from GL instance to Houdini instance to
/// allow a subset of instances to be drawn.
GR_API extern void	buildInstanceIndex(RE_Render *r,
					   RE_Geometry *geo,
					   bool has_partial_visibility,
					   const UT_IntArray &inst_indices,
					   int instance_group = 0,
					   int max_capacity = -1);

/// Builds weight and index arrays for bone transforms.
/// 'bone_limit' specifies the maximal number of bone allowed
/// 'ret_bone_limit' returns the current number of bones being used by the current data set.
//  If more than bone_limit weights are present for a point, the top 
/// contributors are taken (to fit in a up to 3 vec4/ivec4 attribute pairs).
GR_API extern void buildBoneRegionsAndWeights(RE_Render *r,
					      RE_VertexArray **bone_idx,
					      RE_VertexArray **bone_weight,
					      const GT_DataArrayHandle &dh,
					      RE_CacheVersion version,
					      int bone_limit,
					      int &ret_bone_limit);

/// creates (or finds in the cache) attribute 'attrib_name' on RE_Geometry,
/// filling it with the data from the data array 'h'. 
GR_API extern RE_VertexArray     *fillAttribArray(RE_Render	 *r,
						  RE_Geometry	*geo,
						  const char	 *attrib_name,
						  const GT_DataArrayHandle &h,
						  const GR_DisplayOption &opts,
						  RE_ArrayType	  atype,
						  RE_CacheVersion  geo_version);
    
GR_API extern void *	      fillAttribData(RE_Render	*r,
					     const char	*attrib_name,
					     const GT_DataArrayHandle	&pa,
					     const GR_DisplayOption	&opts,
					     RE_ArrayType		 array,
					     bool build_string_tables = true);

/// returns the #chars in the longest string.
GR_API extern int	      fillStringArray(const GT_DataArrayHandle &pa,
					      void		       *data);
// simple GL32 shaders
GR_API extern RE_Shader	*getWireShader(RE_Render *r);
GR_API extern RE_Shader	*getConstPointShader(RE_Render *r);
GR_API extern RE_Shader	*getColorShader(RE_Render *r);
GR_API extern RE_Shader	*getMatteShader(RE_Render *r);
GR_API extern RE_Shader	*getMatteDisplaceShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthPrimTransformShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthCubeShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthCubeLineShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthCubePointShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthCubeDisplaceShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthCubePrimTransformShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthLinearShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthLinearDisplaceShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthLinearPrimTransformShader(RE_Render *r);
GR_API extern RE_Shader *getDepthWireShader(RE_Render *r);
GR_API extern RE_Shader	*getDepthWirePrimTransformShader(RE_Render *r);

GR_API extern void buildCenterPoint(RE_Render *r,
				    const GT_PrimitiveHandle &primh,
				    const GR_UpdateParms &p,
				    const UT_Matrix4DArray &prim_xforms,
				    const GT_DataArrayHandle &prim_ids,
				    const GT_DataArrayHandle &point_ids,
				    const GT_DataArrayHandle &vert_ids,
				    const UT_Vector3F &point,
				    RE_CacheVersion point_version,
				    RE_Geometry *geo,
				    RE_VertexArrayRef *p_ref,
				    RE_VertexArrayRef *inst_mat_ref,
				    GR_SelectMode *point_select_mode,
				    bool is_offsets);

GR_API extern void buildCenterPoints(RE_Render *r,
				     const GT_PrimitiveHandle &handle,
				     const GR_UpdateParms &p,
				     const UT_Matrix4DArray &primxforms,
				     const GT_DataArrayHandle &prim_ids,
				     const GT_DataArrayHandle &pnt_ids,
				     const GT_DataArrayHandle &vtx_ids,
				     RE_Geometry *geo,
				     GR_SelectMode *pnt_sel_mode = NULL,
				     bool is_offsets = true);
    
GR_API extern void buildCenterPointSelection(RE_Render *r,
					     const GT_PrimitiveHandle &primh,
					     const GR_UpdateParms &p,
					     const GT_DataArrayHandle &prim_ids,
					     const GT_DataArrayHandle &pnt_ids,
					     const GT_DataArrayHandle &vert_ids,
					     RE_Geometry *geo,
					     GR_SelectMode *point_select_mode,
					     bool is_offsets);
    
GR_API extern void createCenterPointAttribs(RE_Render *r,
					    const GT_PrimitiveHandle&ph,
					    const GR_UpdateParms &p,
					    RE_Geometry *geo);

GR_API extern void buildCenterPointIDs(RE_Render *r,
				       const GT_PrimitiveHandle &primh,
				       const GR_UpdateParms &p,
				       const GT_DataArrayHandle &prm_id,
				       const GT_DataArrayHandle &pnt_id,
				       const GT_DataArrayHandle &vtx_id,
				       RE_Geometry *geo,
				       bool is_offsets);
    
GR_API extern GR_SelectMode buildSelection(UT_Array<uint8> &selected,
					    const GR_UpdateParms &p,
					    GT_DataArrayHandle prim_id,
					    GT_DataArrayHandle pnt_id,
					    GT_DataArrayHandle vert_id,
					    bool is_offset,
					    int repeat_count);
    
GR_API extern GR_SelectMode buildInstSelectInfo(RE_Render *r,
						RE_Geometry *geo,
						const GR_UpdateParms &p,
						GT_DataArrayHandle primid,
						GT_DataArrayHandle vtxid,
						GT_DataArrayHandle vertid,
						bool is_offsets = true,
						int instances_per_prim = 1,
						UT_Array<uint8> *result = NULL);

GR_API extern bool inViewFrustum(const UT_BoundingBoxD &bbox,
				 const UT_Matrix4D &objviewproj,
				 const UT_Vector4D *frustum_area = NULL);

GR_API extern bool inViewFrustum(const UT_BoundingBoxF &bbox,
				 const UT_Matrix4F &objviewproj);

GR_API extern bool inViewFrustumWithLOD(const UT_BoundingBoxF &bbox,
					const UT_Matrix4F &objviewproj,
					bool        &has_bounds,
					UT_Vector2F &bounds_2d);

GR_API extern GU_Connectivity getConnectivityFromString(const char *str);

class gr_MatBucket
{
public:
    gr_MatBucket() : next(NULL) {}
    UT_IntArray   prims;
    gr_MatBucket *next;
};
GR_API extern UT_Array<UT_Pair<gr_MatBucket*,int> > *
bucketMaterials(const GT_DataArrayHandle &mat_id);
GR_API extern void cleanupBuckets(
			      UT_Array<UT_Pair<gr_MatBucket*,int> > *&buckets);

GR_API extern void printUpdateReason(GR_UpdateReason r,
				     std::ostream *os = NULL);
GR_API extern void updateReasonString(GR_UpdateReason r,
				      UT_WorkBuffer &buf);
GR_API extern void printDrawMode(GR_RenderMode mode,
				  std::ostream *os = NULL);
GR_API extern void printDrawFlags(GR_RenderFlags flags,
				  std::ostream *os = NULL);
GR_API extern void drawFlagsString(GR_RenderFlags flags,
				   UT_WorkBuffer &buf);
GR_API extern void drawModeString(GR_RenderMode mode,
				  UT_WorkBuffer &buf);

} // End namespace GR_Utils

GR_API size_t format(char *buf, size_t bufsize, const GR_UpdateReason &r);
GR_API size_t format(char *buf, size_t bufsize, const GR_RenderFlags &r);
GR_API size_t format(char *buf, size_t bufsize, const GR_RenderMode &r);

#endif

