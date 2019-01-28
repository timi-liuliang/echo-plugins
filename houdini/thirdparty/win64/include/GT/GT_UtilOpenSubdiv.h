/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_UtilOpenSubdiv.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_UtilOpenSubdiv__
#define __GT_UtilOpenSubdiv__

#include "GT_API.h"
#include <UT/UT_UniquePtr.h>
#include <UT/UT_StringArray.h>
#include <GU/GU_SubDivPacking.h>
#include "GT_Types.h"
#include "GT_Handles.h"
#include "GT_OSD3.h"

class GU_Detail;
class GA_Range;

#if 0
class GT_API GT_OSDDrawContext
{
public:
    GT_OSDDrawContext(const Far::PatchTable &patchTables,
		      const GT_OSDTopology<OsdVertex> &topology,
		      const GT_OSDSubdivider<OsdVertex> &subdivider);
    ~GT_OSDDrawContext();

    class PatchArrayDescriptor
    {
    public:
	bool operator<(const PatchArrayDescriptor &other) const
	{
	    if (myMaxValence == other.myMaxValence)
	    {
		if (myType == other.myType)
		{
		    if (myPattern == other.myPattern)
		    {
			if (myRotation == other.myRotation)
			{
			    if (mySubPatch == other.mySubPatch)
			    {
				return false;
			    }

			    return mySubPatch < other.mySubPatch;
			}

			return myRotation < other.myRotation;
		    }

		    return myPattern < other.myPattern;
		}

		return myType < other.myType;
	    }

	    return myMaxValence < other.myMaxValence;
	}

	int			 myMaxValence;
	int			 myType;
	int			 myPattern;
	int			 myRotation;
	int			 mySubPatch;
    };

    class PatchArray
    {
    public:
	PatchArrayDescriptor	 myDescriptor;
	int			 myPatchSize;
	int			 myPatchCount;
	exint			 myVertexOffset;
	exint			 myQuadOffsetBase;
	exint			 myPrimitiveIdBase;
    };

    GT_AttributeListHandle	 myPointAttribs;
    GT_AttributeListHandle	 myVertexAttribs;
    const uint32		*myPatchTable;
    exint			 myPatchCount;
    const int32			*myValenceTable;
    exint			 myValenceCount;
    const uint32		*myQuadOffsetTable;
    exint			 myQuadOffsetCount;
    const FarPatchParam		*myPatchParamTable;
    exint			 myPatchParamCount;
    UT_Array<PatchArray>	 myPatchArrays;
    int				 myMaxValence;
    int				 myPrimitiveMultiplier;
};
#endif

/// Class to perform subdivision refinement using OpenSubdivision
class GT_API GT_UtilOpenSubdiv
{
public:
     GT_UtilOpenSubdiv();
    ~GT_UtilOpenSubdiv();

    using SdcOptions = GT_OSDOptions::SdcOptions;
    using VtxBoundaryInterpolation = SdcOptions::VtxBoundaryInterpolation;
    using FVarLinearInterpolation = SdcOptions::FVarLinearInterpolation;
    using AttribId = GT_OSDAttributes::AttribId;

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

    /// @{
    /// Option accessors
    const GT_OSDOptions	&options() const	{ return myOptions; }
    void		 setOptions(const GT_OSDOptions &options)
			 {
			     clear();
			     myOptions = options;
			 }

    GT_Scheme	scheme() const { return myOptions.scheme(); }
    void	setScheme(GT_Scheme s)
		{
		    clear();
		    myOptions.setScheme(s);
		}

    int		levels() const { return myOptions.level(); }
    void	setLevels(int l)
		{
		    clear();
		    myOptions.setLevel(l);
		}
    bool	adaptive() const { return myOptions.adaptive(); }
    void	setAdaptive(bool v)
		{
		    clear();
		    myOptions.setAdaptive(v);
		}
    bool	removeHoles() const	{ return myOptions.removeHoles(); }
    void	setRemoveHoles(bool v)
		{
		    clear();
		    myOptions.setRemoveHoles(v);
		}

    bool	doCreaseOverride() const
		    { return myOptions.enableCreaseOverride(); }
    fpreal	creaseOverride() const
		    { return myOptions.creaseOverride(); }
    void	setCreaseOverride(bool yes, fpreal w)
		    { setCreaseOverride(yes ? w : -1); }
    void	setCreaseOverride(fpreal w)
		{
		    clear();
		    myOptions.setCreaseOverride(w);
		}

    bool	polySoup() const { return myPolySoup; }
    void	setPolySoup(bool v) { myPolySoup = v; }

    VtxBoundaryInterpolation	vtxBoundaryInterpolation() const
		{ return myOptions.vtxBoundaryInterpolation(); }
    FVarLinearInterpolation	fvarLinearInterpolation() const
		{ return myOptions.fvarLinearInterpolation(); }
    void	setVtxBoundaryInterpolation(VtxBoundaryInterpolation v)
		{
		    clear();
		    myOptions.setVtxBoundaryInterpolation(v);
		}
    void	setFVarLinearInterpolation(FVarLinearInterpolation v)
		{
		    clear();
		    myOptions.setFVarLinearInterpolation(v);
		}
    void	setCreasingMethod(SdcOptions::CreasingMethod v)
		{
		    clear();
		    myOptions.setCreasingMethod(v);
		}
    void	setTriangleSubdivision(SdcOptions::TriangleSubdivision v)
		{
		    clear();
		    myOptions.setTriangleSubdivision(v);
		}
    /// @}

    /// @{
    /// Accessors for the limit surface evaluation options
    GT_OSDPatchTableOptions		&limitOptions()
					    { return myLimitOptions; }
    const GT_OSDPatchTableOptions	&limitOptions() const
					    { return myLimitOptions; }
    /// @}

    /// Clear any topology, attributes or limit surface info
    void	clear();

    const UT_StringArray	&errors() const { return myErrors; }
    /// @}

    /// Refine the mesh to the given number of levels using a given subdivision
    /// scheme.  If the primitive is a subdivision mesh, the crease tags will
    /// be used in refinement. If the supplied prim is not a subdivision
    /// mesh, any required refinements will be performed automatically.
    ///
    /// If the method fails, a null pointer will be returned.
    GT_PrimitiveHandle	subdivide(const GT_PrimitiveHandle &mesh);

    /// Refine Houdini geometry
    bool		subdivide(GU_Detail &gdp, GA_Range *range=NULL);

    /// Set up mesh for limit surface evaluation.  If successful, a subdivision
    /// surface will be set up for evaluation.  You can then do something like
    /// @code
    /// for (int i = 0; i < osd.limitCoarseFaceCount(); ++i)
    /// {
    ///     int     first = osd.limitFirstPatch(i);
    ///     int     size = osd.limitPatchSize(i);
    ///     UT_ASSERT(first + size <= limit.patchCount());
    ///     for (int j = 0; j < size; ++j)
    ///     {
    ///         int         patch_index = first + j;
    ///         float       coord = .5;
    ///         // It's considerably more efficient to evaluate these in batch
    ///         osd.limitSurface("P", false, 1, &patch_index, &coord, &coord);
    ///     }
    /// }
    /// @endcode
    bool	setupLimitEval(const GT_PrimitiveHandle &mesh);

    /// Return the number of input faces.  A cube would have 6 faces, a
    /// tetrahedron would have 4 faces.
    GT_Size	limitCoarseFaceCount() const;

    /// Return the number of individual patches (ptex face ids).  So, a cube
    /// would have 6 patches, while a tetrahedron would have 12 patches (each
    /// triangle generates 3 patches).
    GT_Size	limitPatchCount() const;

    /// Return the ptexture id for a given input face.  For example, if the
    /// input mesh is a tetrahedron, @c limitFirstPatch(2) would return 6.
    /// This is also equivalent to the ptexture index of the first patch for a
    /// given face.
    GT_Size	limitFirstPatch(GT_Size face_index) const;

    /// Return the number of subdivision patches for a given input face.  For
    /// example, if the input mesh is a tetrahedron, @c limitPTextureId(2)
    /// would return 3.
    GT_Size	limitPatchSize(GT_Size face_index) const;

    /// Definition of an attribute id on the topolgy
    AttribId	limitFindAttribute(const char *name) const;

    /// Given a face on the coarse mesh and a u,v coordinate from an attribute,
    /// look up the corresponding patch and interpolant.  If the attribute is
    /// invalid, then the uv's will be assumed to be the patch interpolants,
    /// with the patch offset added to the u coordinate.
    bool		limitLookupPatch(GT_Size hou_face,
				    fpreal hou_u, fpreal hou_v,
				    GT_Size &osd_face,
				    fpreal &osd_u, fpreal &osd_v,
				    const AttribId &attrib,
				    int seg = 0) const;
    /// Given a patch on the subdivision mesh and a uv interpolant on the
    /// patch, return the face on the coarse mesh and possibly the uv
    /// attributes.  If the topology attribute id is invalid, then the u/v
    /// returned will be the uv interpolants on the patch, with the patch
    /// offset added to the uv.
    bool		limitLookupFace(GT_Size osd_face,
				    fpreal osd_u, fpreal osd_v,
				    GT_Size &hou_prim,
				    fpreal &hou_u, fpreal &hou_v,
				    const AttribId &attrib,
				    int seg = 0) const;

    /// Evaluate the limit surface for a given point/vertex attribute at the
    /// coordinates specified.
    GT_DataArrayHandle	limitSurface(const char *attrib, bool vertex_attrib,
					const GT_DataArrayHandle &faces,
					const GT_DataArrayHandle &uvs) const;
    GT_DataArrayHandle	limitSurface(const char *attrib, bool vertex_attrib,
					const GT_DataArrayHandle &faces,
					const GT_DataArrayHandle &u,
					const GT_DataArrayHandle &v) const;
    /// Evaluate the limit surface for a given point/vertex attribute at the
    /// coordinates specified.  The @c face_indices array should have @c npts
    /// entries.  The @c uvs array should have @c uv_stride*npts entries.  Only
    /// the first 2 coordinates will be used (and represent the s/t coordinates
    /// of the face).
    GT_DataArrayHandle	limitSurface(const char *attrib, bool vertex_attrib,
					exint npts,
					const int32 *face_indices,
					const fpreal32 *u,
					const fpreal32 *v,
					int uv_stride = 1) const;

    /// @{
    /// Evaluate the limit surface for a given point/vertex attribute at the
    /// coordinates specified.  The @c face_indices array should have @c npts
    /// entries.  The @c uvs array should have @c uv_stride*npts entries.  Only
    /// the first 2 coordinates will be used (and represent the s/t coordinates
    /// of the face).
    /// The data handles determine which properties are returned:
    ///     @c result    - Attribute
    ///     @c result_du - Derivative of attribute in 'u'
    ///     @c result_dv - Derivative of attribute in 'v'
    /// Passing 'NULL' for a handle disables evaluation of that property.
    /// The size of the evaluated attribute is returned.
    int         	limitSurface(GT_DataArrayHandle *result,
                                     GT_DataArrayHandle *result_du,
                                     GT_DataArrayHandle *result_dv,
                                     const AttribId &attrib,
                                     exint npts,
                                     const int32 *face_indices,
                                     const fpreal32 *u,
                                     const fpreal32 *v,
                                     int uv_stride = 1) const;
    int         	limitSurface(GT_DataArrayHandle *result,
                                     GT_DataArrayHandle *result_du,
                                     GT_DataArrayHandle *result_dv,
                                     const char *attrib, bool vertex_attrib,
                                     exint npts,
                                     const int32 *face_indices,
                                     const fpreal32 *u,
                                     const fpreal32 *v,
                                     int uv_stride = 1) const;
    /// @}

    /// Create the OSD data structures to represent a particular subdivision
    /// mesh. The suppolied handle must refer to a GT_PrimSubdivisionMesh or
    /// the functions will fail. This function can be called once followed
    /// by ay number of calls to refine and/or extractSubdivisionMesh.
    bool		createTopology(const GT_PrimitiveHandle &mesh);

    /// Copies attribute data from the supplied mesh into the OSD data
    /// structure, and performs the refinement. The topology of the supplied
    /// mesh must match that of the mesh passed to the createTopology
    /// function, which must be called before this one.
    bool		refine(const GT_PrimitiveHandle &mesh,
			       bool force_update = false);

    /// Returns a GT_PrimPolygonMesh that is the result of subdividing the
    /// mesh passed to the refine function above.
    GT_PrimitiveHandle	extractSubdividedMesh(int level=-1);

#if 0
    /// Returns a GT_OSDDrawContext that can be used for adaptive rendering
    /// of the subdivision surface in the viewport.
    const GT_OSDDrawContext	*getDrawContext();

    /// Build patches for render-controlled subdivision.  The raw float
    /// arrays provided by GT_OSDVertex will be ordered according to the
    /// ptattribs and vtxattribs order.
    void		buildPatches(UT_Array<GT_OSDPatchHandle> &patches,
				     Scheme scheme,
				     const GU_Detail &gdp,
				     const GU_SubDivPacking &ptattribs,
				     const GU_SubDivPacking &vtxattribs,
				     GA_Range *range=NULL);
#endif

    /// @{
    /// Error handling (used internally)
    void	addError(const char *msg) { myErrors.append(msg); }
    void	clearErrors()		  { myErrors.clear(); }
    /// @}

private:
    bool	updateGeo(GU_Detail &gdp, GA_Range *range,
			const GT_PrimitiveHandle &prim) const;

    GT_PrimitiveHandle	getHull(const GT_PrimitiveHandle &prim,
			    bool triangulate_loop = true) const;

    UT_StringArray			myErrors;
    GT_OSDOptions			myOptions;
    GT_OSDPatchTableOptions		myLimitOptions;
    GT_OSDPatchTable			myPatches;
    UT_UniquePtr<GT_OSDTopology>	myTopology;
    UT_UniquePtr<GT_OSDAttributes>	myAttributes;
    UT_UniquePtr<GT_OSDLimitSurface>	myLimitEval;
    GT_TransformHandle			myPrimitiveTransform;
    bool				myPolySoup;
};

#endif
