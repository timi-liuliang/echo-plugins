/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Util.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Util__
#define __GT_Util__

#include "GT_API.h"

#include <UT/UT_Array.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_PackedTypes.h>
#include "GT_DANumeric.h"
#include "GT_Types.h"
#include "GT_Handles.h"

class GA_Attribute;
class GA_Range;
class GU_Detail;
class GU_DetailHandle;
class GD_Detail;
class GT_RefineParms;
class GT_TrimNuCurves;
class GT_PrimPolygonMesh;

class GT_API GT_Util
{
public:
    /// Create a list of geometry details.  Each refined primitive is stored in
    /// a separate detail.
    /// Refine parameters include:
    ///	 - bool "gconvert:include_ids" (default: false)@n
    ///    Keep the __vertex_id and __primitive_id attributes in conversion
    ///  - bool "gconvert:particleprim" (default: false)@n
    ///    Convert point mesh primitives to a particle primitive.  If off, the
    ///    detail will just contain points (no primitives).
    ///  - bool "gconvert:pack" (default: false) @n
    ///    Each primitive will be packed into a packed primitive, meaning the
    ///    list of returned details will each have a single packed primitive.
    static void	makeGEO(UT_Array<GU_Detail *> &result,
			const GT_PrimitiveHandle &prim,
			const GT_RefineParms *rparms = NULL,
			int segment = 0);
    /// Create a list of geometry details for a single GT primitive.  Each
    /// refined primitive is stored in a separate detail. These details must be
    /// deleted by the caller.
    static void makeGEO(UT_Array<GU_Detail *> &result,
			const GT_Primitive &prim,
			const GT_RefineParms *rparms = NULL,
			int segment = 0);

    /// Create a list of geometry details, but return the result in an array of
    /// GU_DetailHandle's.  Each primitive's refined geometry is stored in a
    /// separate detail.
    ///
    /// Refine parameters include:
    ///	 - bool "gconvert:include_ids" (default: false)@n
    ///    Keep the __vertex_id and __primitive_id attributes in conversion
    ///  - bool "gconvert:particleprim" (default: false)@n
    ///    Convert point mesh primitives to a particle primitive.  If off, the
    ///    detail will just contain points (no primitives).
    static void	makeGEO(UT_Array<GU_DetailHandle> &result,
			const GT_PrimitiveHandle &prim,
			const GT_RefineParms *parms = NULL,
			int segment = 0);
    /// Create a list of geometry details for a single GT primitive.  Each
    /// refined primitive is stored in a separate detail.
    static void makeGEO(UT_Array<GU_DetailHandle> &result,
			const GT_Primitive &prim,
			const GT_RefineParms *rparms = NULL,
			int segment = 0);


    /// Test whether a primitive is atomic (i.e. converts to a single gdp)
    static bool	isSimpleGEO(const GT_Primitive &prim,
			const GT_RefineParms *rparms = NULL);

    /// Convert a GT_TrimNuCurves primitive to a GD_Detail for trim curves
    static void	makeGD(GD_Detail &gdp, const GT_TrimNuCurves &curves);

    /// Map from GA_Type info to a GT_Type info
    static GT_Type	getType(GA_TypeInfo type);

    /// Map from GT_Type to GA_TypeInfo
    static GA_TypeInfo	getGAType(GT_Type type);

    /// Map from GA_Storage to GT_Storage
    static GT_Storage	getStorage(GA_Storage storage);

    /// Map from GT_Storage to GA_Storage
    static GA_Storage	getGAStorage(GT_Storage storage);

    /// Extract a data array from a GA attribute
    static GT_DataArrayHandle	extractAttribute(const GA_Attribute &attrib,
					const GA_Range &range);

    /// Some software represents crease weights as a list of edges and
    /// sharpnesses, or corners and sharpnesses.  This code will look for the
    /// Houdini "crease" attributes and map them to edge or corner crease
    /// weights.  Edge creases are specified by two indices and a single
    /// sharpneess (i.e. the length of @c edge_indices will be twice the length
    /// of @c edge_sharpness). @n
    /// Each index (edge or corner) refers to a vertex in the vertex list. @n
    /// @note: This function relies on being able to find the __primitive_id
    /// and __vertex_id attributes on the polygon mesh to evaluate the @c
    /// creaseweight attribute.
    /// @note: You only need a GT_PrimPolygonMesh to compute creases...
    /// GT_PrimSubdivisionMesh is a sub-class of GT_PrimPolygonMesh.
    static bool		computeSubdivisionCreases(const GU_Detail &gdp,
			    const GT_PrimPolygonMesh &polymesh,
			    GT_DataArrayHandle &edge_indices,
			    GT_DataArrayHandle &edge_sharpnesses,
			    GT_DataArrayHandle &corner_indices,
			    GT_DataArrayHandle &corner_sharpnesses,
			    GT_DataArrayHandle &hole_indices);
    /// This version of @c computeSubdivisionCreases() looks for uniform or
    /// vertex attributes named "creaseweight" on the GT_PrimPolygonMesh.
    static bool		computeSubdivisionCreases(
			    const GT_PrimPolygonMesh &polymesh,
			    GT_DataArrayHandle &edge_indices,
			    GT_DataArrayHandle &edge_sharpnesses,
			    GT_DataArrayHandle &corner_indices,
			    GT_DataArrayHandle &corner_sharpnesses,
			    GT_DataArrayHandle &hole_indices);

    /// Return an array of the point IDs of the primitives (in GA_Index's).
    static GT_DataArrayHandle	getPointIndex(const GT_Primitive &prim,
					      const GU_Detail &parent_detail,
					      int npoints);
    static GT_DataArrayHandle	getPointIndex(const GT_Primitive &prim,
					      const GU_ConstDetailHandle &geo,
					      int npoints);

    static GT_DataArrayHandle	getPointOffset(const GT_Primitive &prim,
					      const GU_Detail &parent_detail,
					      int npoints);
    static GT_DataArrayHandle	getPointOffset(const GT_Primitive &prim,
					      const GU_ConstDetailHandle &geo,
					      int npoints);

    /// Return an array of the primitive IDs (in GA_Index's).
    /// If 'use_nested' is true, attempt to find __nested_prim_id first, then
    /// fall back to __primitive_id.
    static GT_DataArrayHandle	getPrimitiveIndex(const GT_Primitive &prim,
						  const GU_Detail &parent_dtl,
						  bool use_nested = false);
    static GT_DataArrayHandle	getPrimitiveIndex(const GT_Primitive &prim,
					       const GU_ConstDetailHandle &pdtl,
					       bool use_nested = false);

    static GT_DataArrayHandle	getPrimitiveOffset(const GT_Primitive &prim,
						  const GU_Detail &parent_dtl,
						  bool use_nested = false);
    static GT_DataArrayHandle	getPrimitiveOffset(const GT_Primitive &prim,
					       const GU_ConstDetailHandle &pdtl,
					       bool use_nested = false);

    template <typename T>
	static GT_DataArrayHandle buildPointIndirect(GT_Size *points,
					      GT_Size npoints,
					      GT_Size newpoints)
    {
	GT_DANumeric<T> *array;
	T		*data;
	array = new GT_DANumeric<T>(newpoints, 1);
	data = array->data();
	exint dpt = 0;
	for (exint i = 0; i < npoints; ++i)
	{
	    if (points[i] > 0)
	    {
		points[i] = dpt;	// Old point to new point index
		data[dpt] = i;	// New point to old point index
		dpt++;
	    }
	}
	return GT_DataArrayHandle(array);
    }

    template <typename T>
	static GT_DataArrayHandle buildVertexIndirect(const GT_Size *points,
					       const GT_DataArrayHandle &vtx)
    {
	GT_DANumeric<T> *array;
	T		*data;
	GT_Size		 nvtx = vtx->entries();

	array = new GT_DANumeric<T>(nvtx, 1);
	data = array->data();
	for (exint i = 0; i < nvtx; ++i)
	{
	    GT_Size	voff = vtx->getI64(i);
	    UT_ASSERT_P(points[voff] >= 0);
	    data[i] = points[voff];
	}
	return GT_DataArrayHandle(array);
    }


    static GT_PrimitiveHandle optimizePolyMeshForGL(
					const GT_PrimitiveHandle &mesh,
					GU_ConstDetailHandle &parent_dtl);

    static void addViewportLODAttribs(const UT_Array<GEO_ViewportLOD> &lods,
				      GT_AttributeListHandle &uniform,
				      GT_AttributeListHandle &detail);

    static void addBBoxAttrib(const UT_BoundingBox &bbox,
			      GT_AttributeListHandle &detail);
private:
};

#endif
