/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_RefineParms.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_RefineParms__
#define __GT_RefineParms__

#include "GT_API.h"
#include <GEO/GEO_Normal.h>
#include <GEO/GEO_PackedTypes.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_String.h>
#include <UT/UT_Options.h>
#include "GT_Handles.h"
#include "GT_DataArray.h"

class GT_API GT_RefineParms {
public:
	     GT_RefineParms();
	     GT_RefineParms(const GT_RefineParms &ref);
    virtual ~GT_RefineParms();

    /// Hash function
    virtual unsigned	hash() const;

    /// Comparison test
    virtual bool	isEqual(const GT_RefineParms &src) const;

    /// @{
    /// Equivalence operators
    bool		operator==(const GT_RefineParms &src) const
			    { return isEqual(src); }
    bool		operator!=(const GT_RefineParms &src) const
			    { return !isEqual(src); }
    /// @}

    /// @{
    /// Import parameters/options.  It might be useful for these to be virtual
    void	set(const char *name, bool v)
		    { myOptions.setOptionB(name, v); }
    void	set(const char *name, exint v)
		    { myOptions.setOptionI(name, v); }
    void	set(const char *name, fpreal v)
		    { myOptions.setOptionF(name, v); }
    void	set(const char *name, const char *v)
		    { myOptions.setOptionS(name, v); }
    void	set(const char *name, UT_Int64Array &v)
		    { myOptions.setOptionIArray(name, v); }
    void	set(const char *name, UT_Fpreal64Array &v)
		    { myOptions.setOptionFArray(name, v); }
    /// @}


    /// @{
    /// Import parameters/options.  It might be useful for these to be virtual
    bool	import(const char *name, bool &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, int32 &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, int64 &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, fpreal32 &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, fpreal64 &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, UT_String &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, UT_Int64Array &v) const
		    { return importOption(name, v); }
    bool	import(const char *name, UT_Fpreal64Array &v) const
		    { return importOption(name, v); }
    /// @}

    /// @{
    /// Get a defaulted value
    bool	get(const char *name, bool def) const
		{
		    bool	val;
		    return import(name, val) ? val : def;
		}
    exint	get(const char *name, exint def) const
		{
		    exint	val;
		    return import(name, val) ? val : def;
		}
    fpreal	get(const char *name, fpreal def) const
		{
		    fpreal	val;
		    return import(name, val) ? val : def;
		}
    const char	*get(const char *name, const char *def,
			UT_String &storage) const
		{
		    return import(name, storage) ? (const char *)storage : def;
		}
    /// @}

    /// @{
    /// Static methods to get a defaulted parameter, with possibly a NULL
    /// parameter pointer.
    static bool		getBool(const GT_RefineParms *parms,
				const char *name, bool def)
			{
			    return parms ? parms->get(name, def) : def;
			}
    static exint	getInt(const GT_RefineParms *parms,
				const char *name, exint def)
			{
			    return parms ? parms->get(name, def) : def;
			}
    static fpreal	getFloat(const GT_RefineParms *parms,
				const char *name, fpreal def)
			{
			    return parms ? parms->get(name, def) : def;
			}
    static const char	*getString(const GT_RefineParms *parms,
				const char *name, const char *def,
				UT_String &storage)
			{
			    return parms
				    ? parms->get(name, def, storage)
				    : def;
			}
    /// @}

    /// @{
    /// Common parameters

    /// @def Define get/set method for bool, int, float types
#define GT_GETSET(METHOD, NAME, TYPE, DEF)	\
	    void set##METHOD(TYPE v)	{ set(NAME, v); } \
	    TYPE get##METHOD() const	{ return get(NAME, (TYPE)DEF); } \
	    static TYPE get##METHOD(const GT_RefineParms *parms) \
		    { return parms ? parms->get(NAME,(TYPE)DEF) : (TYPE)DEF; }

    /// @def Define get/set method for string types
#define GT_GETSET_STR(METHOD, NAME, DEF)	\
	    void set##METHOD(const char *v)	{ set(NAME, v); } \
	    const char *get##METHOD(UT_String &storage) const \
		    { return get(NAME, DEF, storage); } \
	    static const char *get##METHOD(const GT_RefineParms *parms, \
			    UT_String &storage) \
		    { return parms ? parms->get(NAME,DEF,storage) : DEF; }

#define GT_GETSET_BIT(METHOD, MEMBER, DEF)	\
    void set##METHOD(bool v) { MEMBER = v; }	\
    bool get##METHOD() const { return MEMBER; } \
    static bool get##METHOD(const GT_RefineParms *parms) \
	    { return parms ? parms->get##METHOD() : DEF; }

#define GT_GETSET_INT(METHOD, MEMBER, DEF)	\
    void  set##METHOD(exint v) { MEMBER = v; }	\
    exint get##METHOD() const { return MEMBER; } \
    static exint get##METHOD(const GT_RefineParms *parms) \
	    { return parms ? parms->get##METHOD() : DEF; }

    /// Render a geometry as a point mesh
    GT_GETSET   (GeoPointMesh,		"geo:pointmesh", bool, false)

    /// Render dangling (unreferenced) points as a point mesh
    GT_GETSET	(ShowUnusedPoints,	"geo:unusedpoints", bool, true)
    /// Render all points, even if not dangled.  Requires unusedpoints
    /// already be set!
    GT_GETSET	(ShowAllPoints,	"geo:allpoints", bool, false)

    /// Use polygon mesh primitives
    GT_GETSET   (GeoPolygonMesh,	"geo:polygonmesh", bool, true)

    /// Refine only a specific group of primitives
    GT_GETSET_STR(GeoRefineGroup,	"geo:refinegroup", NULL)

    /// Global level of detail
    GT_GETSET   (LOD,			"lod", fpreal, 1.0)

    /// Parametric u level of detail
    GT_GETSET   (ULOD,			"ulod", fpreal, 1.0)
    GT_GETSET   (VLOD,			"vlod", fpreal, 1.0)

    /// Quadric parameters
    GT_GETSET	(QuadricDivs,		"quadricdivs", fpreal, 20.0)

    /// Metaball parameters
    GT_GETSET   (MetaMaxDivs,		"metamaxdivs", exint, 4096)
    GT_GETSET	(MetaDensity,		"metadensity", fpreal, 30.0)

    /// Parametric u/v range
    GT_GETSET   (URange0,		"urange_min", fpreal, 0.0)
    GT_GETSET   (URange1,		"urange_max", fpreal, 1.0)
    GT_GETSET   (VRange0,		"vrange_min", fpreal, 0.0)
    GT_GETSET   (VRange1,		"vrange_max", fpreal, 1.0)

    /// Whether to respect the packed primitive viewport LOD.  This is set for
    /// viewport rendering, but not for mantra.
    GT_GETSET_BIT(PackedViewportLOD, myPackedViewLOD, false)

    /// Override the viewport LOD for packed primitives.  The value should be
    /// set to one of the enum values in $GEO/GEO_PackedTypes.h
    GT_GETSET_INT(OverridePackedViewportLOD, myOverridePackedLOD, -1);

    /// The maximum number of LOD levels for agents.
    GT_GETSET	(ViewportAgentMaxLOD, "agent:maxlod", exint, 4)

    /// The minimum number of polygons for an LOD level; will halt the LOD.
    GT_GETSET	(ViewportAgentPointLimit, "agent:pointlimit", exint, 1000)

    /// Whether to enable the viewport specific code when refining agents.
    GT_GETSET   (RefineAgentsForViewport, "agent:refineviewport", bool, false)

    /// Whether to refine the collision layer (if available) for agents instead
    /// of their current layer.
    GT_GETSET   (UseAgentCollisionLayer, "agent:collisionlayer", bool, false)

    /// @{
    /// When loading Alembic for the viewport, (i.e. when PackedViewportLOD is
    /// true), we check the following settings to determine whether to load
    /// face sets and arbitrary attributes.
    GT_GETSET_BIT(ViewportAlembicFaceSets, myAlembicFaceSets, false)
    GT_GETSET_BIT(ViewportAlembicArbGeometry, myAlembicArbGeo, true)
    /// @}
    GT_GETSET_BIT(AlembicHoudiniAttribs, myAlembicHoudiniAttribs, true)
    GT_GETSET_BIT(AlembicInstancing, myAlembicInstancing, false)
    GT_GETSET_BIT(AlembicGLOptimize, myAlembicGLOptimize, false)
    GT_GETSET_BIT(PointInstanceEnable, myPointInstanceEnable, false)


    /// When unpacking Alembic, prune geometry that isn't visible.  If this is
    /// false, empty geometry will be created.
    GT_GETSET_BIT(AlembicSkipInvisible, myAlembicSkipInvisible, true)

    /// Whether to include primtive/vertex id's when converting to geo
    GT_GETSET   (ConvertIncludeIds,	"gconvert:include_ids", bool, false)
    /// Convert point clouds into a particle primitive (instead of unconnected
    /// points).
    GT_GETSET   (ConvertParticlePrim,	"gconvert:particleprim", bool, false)
    /// When converting from GT to GU, create packed geometry containing the
    /// converted GU details.
    GT_GETSET	(ConvertPack,		"gconvert:pack", bool, false)
    /// Convert to Polygon Soup primitives if possible
    GT_GETSET	(AllowPolySoup,		"gconvert:allow_polysoup", bool, true)
    /// When converting from GU_Detail to GT primitives, create a subdivivision
    /// surface for polygons (instead of a GT_PrimPolygonMesh).
    GT_GETSET_BIT(PolysAsSubdivision, mySubdivPolys, false)

    /// Whether to coalesce fragments into a single mesh.  In most cases, the
    /// fragment geometry is constant over time, while only the transform
    /// changes.  Some applications prefer getting a large chunk of geometry
    /// rather than lots of little pieces.  Others prefer to use the fact that
    /// only the transform is animated.
    GT_GETSET	(CoalesceFragments,	"coalescefragments", bool, true);

    /// Whether to coalesce volumes into a single GT_Primitive. Mantra and
    /// probably most uses of GT require this to be true but at least HAPI
    /// likes to have volumes be kept as separate GT_Primitives.
    GT_GETSET	(CoalesceVolumes,	"coalescevolumes", bool, true);

    enum
    {
	FACESET_NONE = 0,	// Don't create facesets
	FACESET_NON_EMPTY,	// Create face sets for non-empty groups
	FACESET_ALL_GROUPS,	// Create face sets for all groups
    };
    /// When building polygon/curve meshes from geometry, optionally build a
    /// "face set" to store group information in the meshes.
    GT_GETSET	(FaceSetMode,	"facesetmode", exint, FACESET_NONE)

    /// When collecting polygons, there are some simple/fast heuristics to
    /// check whether point attributes need to be compacted.  However, this
    /// doesn't always catch cases if there are stray points (unconnected
    /// points) in the geometry.  If you want precise polygonal meshes, you can
    /// disable fast detection.
    GT_GETSET	(FastPolyCompacting, "fastpolycompacting", bool, true);

    /// When collecting polygons, if there are no point or vertex normals
    /// on the incoming geometry, this indicates whether to compute and add
    /// vertex normals to the GT_PrimPolygonMesh.
    GT_GETSET   (AddVertexNormals, "addvertexnormals", bool, false);

    /// When adding vertex normals, this is the cusp angle to use.
    /// If the dihedral angle of an edge is larger than this, it will be cusped.
    GT_GETSET   (CuspAngle, "cuspangle", float,GEO_DEFAULT_ADJUSTED_CUSP_ANGLE);

    /// Maximum number of points in a point mesh.  This is used when refining a
    /// GEO_Detail.
    GT_GETSET	(MaxPointMeshSize,	"maxpointmeshsize", exint, -1)

    /// Maximum number of points in a point mesh.  This is used when refining a
    /// GEO_Detail.
    GT_GETSET	(MaxPolyMeshSize,	"maxpolymeshsize", exint, -1)

    /// Number of sub-frame timesteps used when unpacking sequence packed
    /// primitives.  Each sub-step requires a blended detail.  A value of -1
    /// indicated no quantization, 0 will use only keyframe values, 1 will have
    /// one sub-frame (i.e. 0.5), etc.
    GT_GETSET	(SequenceSubsteps,	"sequencesubsteps", exint, -1)

    /// When creating instances during refining, rather than creating an
    /// instance with multiple transforms for the geometry, create separate
    /// instances, but still with a single shared GT_Primitive.  This helps
    /// create aligned instances when trying to match segments for motion blur.
    GT_GETSET	(SeparateInstances,	"separateinstance", bool, false)

    /// When refining Houdini curves (NURBs/Bezier), generate Bezier curve
    /// meshes along with NURBS curve meshes where appropriate.  Otherwise,
    /// only NURBS curves will be generated.
    GT_GETSET	(FullBezierSupport,	"fullBezierSupport", bool, true);
    /// If full Beziers aren't supported, still refine cubic Bezier curves to a
    /// pure Bezier curve mesh.
    GT_GETSET	(CubicBezierSupport,	"cubicBezierSupport", bool, true);

    /// Whether to refine capture regions to curves (e.g. as required by the
    /// viewport) or to polygons.
    GT_GETSET   (RefineCRegionsToCurves, "refinecregionstocurves", bool, false)

    /// Prune geometry that is hidden.
    GT_GETSET_BIT(SkipHidden, mySkipHidden, true)

#undef GT_IMPLEMENT_GETSET
#undef GT_IMPLEMENT_GETSET_STR

    GT_DataArrayHandle	getViewportMaterialAttrib() const
			{ return myViewportMaterialAttrib; }
    GT_DataArrayHandle	getViewportMaterialRemapAttrib() const
			{ return myViewportMaterialRemapAttrib; }
    void		setViewportMaterialAttrib(const GT_DataArrayHandle &mh,
						  const GT_DataArrayHandle &rh)
			{ myViewportMaterialAttrib = mh;
			  myViewportMaterialRemapAttrib = rh; }

    /// Load from UT_Options
    bool	load(const UT_Options &opts);

    /// @{
    /// Debugging methods to dump the non-default parameters.  Dump to either
    /// stdout or a string buffer.
    void	dump() const;
    bool	dump(UT_WorkBuffer &buf) const;
    /// @}

public:
    /// @{
    /// For memory tracking, we override the new/delete operators
    static void	*operator	new(size_t size);
    static void	*operator	new(size_t size, void *p);
    static void	 operator	delete(void *p, size_t size);
    /// @}

protected:
    template <typename T> bool
    importOption(const char *name, T &value) const
    {
	return myOptions.importOption(name, value);
    }
private:
    UT_Options		myOptions;
    int			myAlembicArbGeo		: 1,
			myAlembicFaceSets	: 1,
			myAlembicHoudiniAttribs	: 1,
			myAlembicInstancing	: 1,
			myAlembicGLOptimize	: 1,
			myAlembicSkipInvisible	: 1,
			myPackedViewLOD		: 1,
			mySubdivPolys		: 1,
			myPointInstanceEnable	: 1,
			myOverridePackedLOD	: 5,
			mySkipHidden		: 1;
    GT_DataArrayHandle	myViewportMaterialAttrib;
    GT_DataArrayHandle	myViewportMaterialRemapAttrib;
};

#endif
