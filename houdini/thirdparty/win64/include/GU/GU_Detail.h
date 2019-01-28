/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Detail.h (GU Library, C++)
 *
 * COMMENTS:
 *	Header file for GU_Detail class...
 *
 */

#pragma once

#ifndef __GU_Detail_h__
#define __GU_Detail_h__

#include "GU_API.h"
#include "GU_Error.h"
#include "GU_PrimitiveFactory.h"
#include "GU_SelectType.h"
#include "GU_Types.h"

#include <GEO/GEO_Detail.h>
#include <GEO/GEO_PrimType.h>
#include <GEO/GEO_Primitive.h>
#include <GEO/GEO_SurfaceType.h>
#include <GEO/GEO_HedgeInterface.h>

#include <GA/GA_GroupTypeTraits.h>

#include <UT/UT_Array.h>
#include <UT/UT_ArraySet.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_ErrorManager.h>	// for UTgetErrorManager()
#include <UT/UT_Matrix4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_OBBox.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

#include <stddef.h>


class IMG_Raster;
class UT_Interrupt;
template<typename T> class UT_VectorT;
using UT_Vector = UT_VectorT<fpreal64>;
class UT_MemoryCounter;

class GA_BreakpointGroup;
class GA_EdgeGroup;
class GA_VertexGroup;
class GA_NUBBasis;
class GA_ElementWranglerCache;
class GA_OffsetMatrix;
class GEO_ConvertParms;
class GEO_Curve;
class GEO_Face;
class GEO_MetaConvertParms;
class GEO_PolySoupParms;
class GEO_PrimMesh;
class GEO_PrimPoly;
class GEO_PrimPolySoup;
class GEO_SoftTransformCache;
class GEO_TPSurf;
class GEO_Hull;
class GEO_IOTranslator;

class GU_EdgeDiv;
class GU_SplitLoc;
class GU_Selection;

class GU_PrimPoly;
class GU_PrimMesh;
class GU_PrimPart;
class GU_PrimSphere;
class GU_PrimTube;
class GU_PrimBlob;
class GU_PrimNURBCurve;
class GU_PrimNURBSurf;
class GU_PrimPolySoup;
class GU_PrimRBezSurf;

// Parameter classes for methods
class GU_AlignParms;
class GU_CameraParms;
class GU_CapParms;
class GU_CapOptions;
class GU_CreepParms;
class GU_CurveClayParms;
class GU_CurveNetworkParms;
class GU_EdgeCreaseParms;
class GU_ExtrudeParms;
class GU_FilletParms;
class GU_GridParms;
class GU_JoinParms;
class GU_LoftParms;
class GU_LSystemParms;
class GU_MagnetParms;
class GU_ClothMatchSeamsParms;
class GU_OffsetParms;
class GU_PolyExtrudeParms;
class GU_PolypatchParms;
class GU_PolyReduceParms;
class GU_PolysplineParms;
class GU_RailParms;
class GU_RevolveParms;
class GU_RoundFilletParms;
class GU_RuledParms;
class GU_SkinParms;
class GU_SkinCache;
class GU_StitchParms;
class GU_SuperQuadParms;
class GU_SweepParms;
class GU_TorusParms;
class GU_TraceParms;
class GU_TrimFilletParms;
class GU_TwistParms;
class GU_WarpParms;



typedef UT_Array<GU_SplitLoc *> GU_SplitLocArray;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newGeometryIO(void *);
    SYS_VISIBILITY_EXPORT extern void	newGeometryPrim(GA_PrimitiveFactory *factory);
}

class gu_ValueLookupCache;

class GU_API GU_Detail : public GEO_Detail
{
public:
    /// NOTE: Need an explicit default constructor to work around internal
    /// compiler error in Visual Studio 2015 Update 3.
    /// See: https://connect.microsoft.com/VisualStudio/feedback/details/2869531
                GU_Detail() : GU_Detail(true) {}
    explicit	GU_Detail(bool full_topology)
		    : GEO_Detail(GUgetFactory(), full_topology)
		{
		    construct();
		}
    /// See GEO_Detail::merge() for documentation of the data_id_strategy
    /// argument.
    explicit	GU_Detail(const GU_Detail *src, GA_PrimitiveGroup *primGroup,
			  GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP)
		    : GEO_Detail(GUgetFactory())
		{
		    construct();
		    merge(*src, primGroup,
			  /*mergePrimGroup*/1, /*insertPrimsAtHead*/0,
			  /*dest_to_src_ptarray*/0,
			  /*keep_internal_groups*/true,
			  data_id_strategy);
		}
    /// See GEO_Detail::copy() for documentation of the data_id_strategy
    /// argument.
    explicit	GU_Detail(const GU_Detail *src,
			  GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP)
		    : GEO_Detail(GUgetFactory())
		{
		    construct();
		    copy(*src, GEO_COPY_ONCE,
			 /*collapse_on_end*/false,
			 /*keep_internal_groups*/true,
			 data_id_strategy);
		}

    virtual	~GU_Detail();

    /// Compute memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// This clears any caches that subclasses of GA_Detail may have, so that
    /// replacing the content of the detail doesn't cause crashes.
    virtual void clearCaches();

    void		 duplicate(const GU_Detail& gdp, int = 0,
                                   GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP);

    /// Create a new detail that has all the same attributes, groups, etc. but
    /// has no elements.
    /// Subclasses should look at: cloneCopyGroupsAndAttributes()
    virtual GA_Detail	*cloneEmptyDetail(bool clone_attributes) const;

    class RingRef
    {
    public:
	RingRef(const UT_Array<GA_OffsetArray> &ringzero,
		const UT_IntArray &ringvalence)
	    : myRingZero(ringzero)
	    , myRingValence(ringvalence)
	{
	}

	const UT_Array<GA_OffsetArray> &    ringZero() const
					    { return myRingZero; }
	const UT_IntArray &		    ringValence() const
					    { return myRingValence; }

    public:
	const UT_Array<GA_OffsetArray> &    myRingZero;
	const UT_IntArray &		    myRingValence;
    };

    /// This initializes the passed in int array so it is 0 where
    /// points are a boundary and 1 where they are not a boundary.
    /// This handles meshes, etc, properly.  It doesn't handle non-manifold
    /// properly.  The checkuv flag determines if it should check uv
    /// boundaries, if set, it will mark as 1 any points which have
    /// differing UV texture coordinates in their vertices.
    /// For efficiency, you may call buildRingZero() once and pass the built
    /// data structures to it.
    void		 findBoundaryPoints(
			    UT_BitArray &isboundary,
			    bool checkuv,
			    const RingRef *ringref = nullptr,
                            const UT_StringHolder &uvattribname = "uv"_UTsh) const;

    /// This takes an array which should be the same size as the number
    /// of points, and has 1 wherever a point is to be considered
    /// selected.  It then initializes & sets 1 in the isboundary array
    /// any point which is on the 8-way boundary of the selection.
    void		 findSelectionBoundaryPoints(
				const UT_BitArray &pointselection,
				UT_BitArray &isboundary) const;

    /// Fills the ringzero array, (which is indexed by point GA_Index,
    /// not GA_Offset), with the GA_Offsets of each point's neighbours,
    /// (i.e. they share an edge in some primitive).  It avoids
    /// duplicates within each point's list of neighbours.
    /// If ringvalence is non-NULL, it is filled with the number of
    /// half-edges each point is part of, i.e. including duplicates
    /// and both directions.
    void		 buildRingZeroPoints(
			    UT_Array<GA_OffsetArray> &ringzero,
			    UT_IntArray *ringvalence = 0) const;

    /// Fills the ringzero array, (which is indexed by point GA_Index,
    /// not GA_Offset), with the GA_Offsets of each point's neighbours,
    /// (i.e. they share an edge in some primitive).  It avoids
    /// duplicates within each point's list of neighbours.
    /// If ringvalence is non-NULL, it is filled with the number of
    /// half-edges each point is part of, i.e. including duplicates
    /// and both directions.
    ///
    /// In this version, an edge is only counted if it is in a primitive
    /// in primgroup (if non-NULL).  Although only entries for
    /// points in ptgroup (if non-NULL) are written-to in ringzero
    /// and ringvalence (if non-NULL), neighbours outside of ptgroup
    /// will be listed.
    void		 buildRingZeroPoints(
			    const GA_PointGroup *ptgroup,
			    UT_Array<GA_OffsetArray> &ringzero,
			    UT_IntArray *ringvalence = 0,
                            const GA_PrimitiveGroup *primgroup = 0) const;

    /// NOTE: Unlike buildRingZeroPoints, the array in buildRingZeroVertices
    ///       is indexed by GA_Offset, not GA_Index, for simplicity.
    void                 buildRingZeroVertices(
                            UT_Array<GA_OffsetArray> &ringzero,
                            const GA_PrimitiveGroup *prims=0) const;

    void		 buildRingZeroPrimitives(
			    UT_Array<GA_OffsetArray> &ringzero) const;

    //
    // Build various geometries. Returns the first primitive of the new
    // geometry.
    //

    /// Creates a polygon cube in this detail.
    /// NOTE: Set doConsolidatePoints to true in order to have
    ///       correctly connected polygons.  The default behaviour
    ///       produces disconnected sides!
    GU_PrimPoly		*cube(float xmin = -1, float xmax = 1,
			      float ymin = -1, float ymax = 1,
			      float zmin = -1, float zmax = 1,
			      int xdiv = 0, int ydiv = 0, int zdiv = 0,
			      int enforcementBars = 0, 
			      int doConsolidatePoints = 0);

    GU_PrimNURBSurf    *nurbCube(int xdiv, int ydiv, int zdiv,
				  int orderx = 4, int ordery = 4, int orderz=4,
				  float xmin = -0.5F, float xmax = 0.5F,
				  float ymin = -0.5F, float ymax = 0.5F,
				  float zmin = -0.5F, float zmax = 0.5F,
				  GEO_SurfaceType type = GEO_PATCH_QUADS,
				  bool consolidate = false);

    GU_PrimRBezSurf    *bezCube(int xdiv, int ydiv, int zdiv,
				int orderx = 4, int ordery = 4, int orderz=4,
				float xmin = -0.5F, float xmax = 0.5F,
				float ymin = -0.5F, float ymax = 0.5F,
				float zmin = -0.5F, float zmax = 0.5F,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				bool consolidate = false);
    
    GU_PrimMesh        *meshCube(int xdiv, int ydiv, int zdiv,
			 	 float xmin = -0.5F, float xmax = 0.5F,
				 float ymin = -0.5F, float ymax = 0.5F,
				 float zmin = -0.5F, float zmax = 0.5F,
				 GEO_SurfaceType type = GEO_PATCH_QUADS,
				 bool consolidate = false);
    
    GU_PrimPoly        *polymeshCube(int xdiv, int ydiv, int zdiv,
			 	 float xmin = -0.5F, float xmax = 0.5F,
				 float ymin = -0.5F, float ymax = 0.5F,
				 float zmin = -0.5F, float zmax = 0.5F,
				 GEO_SurfaceType type = GEO_PATCH_QUADS,
				 bool consolidate = false);

    /// Creates a grid in this detail based on parms.
    /// Returns the offset of the first polygon primitive.
    GA_Offset            buildGrid(GU_GridParms &parms,
				   GU_GridType type = GU_GRID_MESH);

    /// Creates a grid of points in this detail.
    /// NOTE: If startpoint is valid, it must refer to a *contiguous*
    ///       block of rows*cols point offsets, in which case, this
    ///       just sets point positions, so can be used on polygon
    ///       or polysoup grids with a consistent point order too.
    /// NOTE: When plane is GU_PLANE_XY, cols corresponds with x, and
    ///       rows corresponds with y.
    GA_Offset		 pointGrid(int rows, int cols,
				float xsize=1, float ysize=1,
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
                                GA_Offset startpoint=GA_INVALID_OFFSET);
    /// Creates a grid of polygons in this detail.
    /// Returns the offset of the first polygon primitive.
    GA_Offset            polyGrid(int rows, int cols,
				float xsize=1, float ysize=1,
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
				GEO_SurfaceType type = GEO_PATCH_QUADS);
    /// Creates a grid that is a single polygon soup in this detail.
    GU_PrimPolySoup	*polySoupGrid(int rows, int cols,
				float xsize=1, float ysize=1,
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
				GEO_SurfaceType type = GEO_PATCH_QUADS);
    GU_PrimMesh		*meshGrid(int rows, int cols,
				float xsize=1, float ysize=1,
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				int wrapu = 0, int wrapv = 0);
    GU_PrimNURBSurf	*nurbGrid(int rows, int cols, 
				int orderu = 4, int orderv = 4,
				int interpEndsU = 1, int interpEndsV = 1,
				float xsize=1, float ysize=1, 
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				int wrapu = 0, int wrapv = 0);
    GU_PrimRBezSurf	*bezGrid(int rows, int cols, 
				int orderu = 4, int orderv = 4,
				float xsize=1, float ysize=1, 
				float xc = 0, float yc = 0, float zc = 0,
				GU_OrientationType plane = GU_PLANE_XY,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				int wrapu = 0, int wrapv = 0);

    /// Creates a torus in this detail, based on parms.
    /// type is one of the values in the enum in GA_PrimitiveTypes.h .
    /// GA_PRIMPOLY, GA_PRIMPOLYSOUP, GA_PRIMMESH, GA_PRIMBEZSURF,
    /// and GA_PRIMNURBSURF are supported.
    void		 torus(unsigned type, GU_TorusParms &parms);

    /// poly-iso surfaces
    /// To find the primitives created, place
    /// GA_IndexMap::Marker marker(gdp->getPrimitiveMap());
    /// before the call, and call marker.getRange() afterward.
    void		 polyIsoSurface(
				float (*ptOutside)(const UT_Vector3 &, void *),
				void *data,
				const UT_BoundingBox &bbox,
				int xdiv, int ydiv, int zdiv,
				bool makepolysoup = false);

    /// When building a meta-surface, the new surface is built in this
    /// gdp.  If desired, src can be "this" as well...
    void		 buildMetaSurface(const GU_Detail *src, float lod,
					  const GA_PrimitiveGroup *primGroup = 0,
                                          bool makepolysoup = false);

    void		 buildMetaSurface(const GU_Detail *src,
					  int divx, int divy, int divz,
					  const GA_PrimitiveGroup *primGroup = 0,
                                          bool makepolysoup = false);

    /// A faster way of conversion is to build cross sections.  However, this
    /// doesn't build real surfaces
    void		buildMetaXsection(const GU_Detail *src, int nsections,
				    int axis = 2, int maxdepth = 4,
				    const GA_PrimitiveGroup *primGroup = NULL);

    //
    // Geometry filters
    //


    /// orient all the polygonal faces to have the same winding direction
    void		orient(const GA_PrimitiveGroup *group = 0);

    /// inset and extrude a face
    void		extrude(GU_ExtrudeParms &parms);

    /// create a weighted sum of two source inputs
    /// This bumps data IDs of any attributes that are modified, and of
    /// the primitive list, if any primitives are modified.
    void		blend(const GU_Detail *source, float weight,
				bool doPos, bool doClr, bool doNml, bool doTxt,
				bool doVoxels, bool doSlerp,
				const char *ptidattr, const char *primidattr);


    /// create a weighted sum of size source inputs
    /// Return 1 if at least one pasted surface was involved in the blend,
    /// else 0.
    /// This bumps data IDs of any attributes that are modified, and of
    /// the primitive list, if any primitives are modified.
    int			blend(const GU_Detail *gdps[], const float weights[],
				int size, bool doPos, bool doClr,
				bool doNml, bool doTxt,
				bool doVoxels, bool doSlerp,
				const char *ptidattr, const char *primidattr,
				const GA_PointGroup *ptGroup);

    /// Transform points to follow surface of path input
    void		creep(GU_CreepParms &parms);

    /// Consolidate points within a specified distance (returns num done)
    /// If a point group is specified, then, only points in that
    /// group are consolidated.  If the forceConsAll flag is set then
    /// all points will be consolidated in a greedy fashion using a
    /// branch and bound algorithm.
    GA_Size 		 consolidatePoints(fpreal distance, 
					   const GA_PointGroup *ptGrp = 0,
					   bool forceConsAll = false,
					   bool mark = false,
					   bool accurate = false);
    
    /// Performs fast consolidation by calling onlyConsolidatePoints.
    /// By default, calls removeUnused when done to remove any unused points
    /// When deleteConsOnly is set, then only consolidated points are removed.
    GA_Size              fastConsolidatePoints(fpreal distance,
                                               const GA_PointGroup *ptGrp=0,
                                               bool mark = false,
                                               bool accurate = false);

    /// Specifies methods by which points after consolidation will
    /// get their group names. ONLYCONS_GRP_PROP_LEAST means that the
    /// points will belong to the same group as the original point
    /// with least point number. ONLYCONS_GRP_PROP_UNION means that the
    /// points will belong to the full set of groups represented 
    /// by the original points (so if orig. point 1 is in group A, B and
    /// orig. point 2 is in group B, C, the final point will be in A B C).
    /// INTERSECT means the point will only belong in groups that are common
    /// to all original points. With the example above, it will belong in
    /// B.
    enum OnlyConsGroupPropagateType
    {
	ONLYCONS_GRP_PROP_LEAST = 0,
	ONLYCONS_GRP_PROP_UNION,
	ONLYCONS_GRP_PROP_INTERSECT
    };
    /// This does fast consolidation but does *not* call removeUnused when
    /// done.  This means that degenerate primitives will be left along
    /// with orphaned points. If deleteconsolidated is set, the consolidated
    /// points only will be deleted. grouppropagate controls which groups
    /// the new points will belong to. 
    GA_Size		 onlyConsolidatePoints(fpreal distance,
						const GA_PointGroup *ptgrp = 0,
						bool mark = false,
						bool deleteconsolidated = false,
						OnlyConsGroupPropagateType
						grouppropagate = 
						ONLYCONS_GRP_PROP_LEAST,
						bool accurate = false);

    GA_Size		 consolidateNormals(fpreal distance,
					    const GA_PointGroup *ptGrp = 0,
					    bool forceConsAll = false,
					    bool accurate = false);
    GA_Size		 fastConsolidateNormals(fpreal distance,
					        const GA_PointGroup *ptGrp = 0,
					        bool accurate = false);
    /// Consolidate UV attributes within a specified distance.
    ///   This distance can be in UV space or XYZ space.
    ///   There are various methods of placing the consolidated UVs.
    ///
    ///   	metric:		0 => UV space
    ///   			1 => XYZ space
    ///
    ///   	method:		0 => Average
    ///   			1 => First in Group
    ///   			2 => Specify uvw coordinate
    int			 fastConsolidatePointUVs(float distance,
						 const GA_PointGroup &ptGrp,
						 const GU_MetricType metric,
						 int method,
						 UT_Vector3 &uvw);
    int			 fastConsolidatePointUVs(const GA_RWHandleV3 &uvattrib,
                                                float distance,
                                                const GA_PointGroup &ptGrp,
                                                const GU_MetricType metric,
                                                int method,
                                                UT_Vector3 &uvw);
    int			 fastConsolidateVertexUVs(float distance,
						  const GA_VertexGroup &vtxGrp,
						  const GU_MetricType metric,
						  int method,
						  UT_Vector3 &uvw);
    int			 fastConsolidateVertexUVs(const GA_RWHandleV3 &uvattrib,
                                                  float distance,
                                                  const GA_VertexGroup &vtxGrp,
                                                  const GU_MetricType metric,
                                                  int method,
                                                  UT_Vector3 &uvw);

    /// Snap points within a specified distance (returns num done)
    /// If a point group is specified, then, only points in that
    /// group are consolidated.  
    /// Snapping doesn't fuse the points together, just makes their
    /// positions match.
    /// The type is 0 for average, 1 for round to lowest point number
    /// and 2 for highest point number.
    /// snappointpos controls whether the point positions will be
    /// snapped.
    /// snapptattribs can be set to indicate that point attributes
    /// are to be snapped.
    /// ptattribhandles is a list of attribute handles for the
    /// attributes that will be snapped.
    GA_Size              snapPoints(int type, fpreal distance,
                                    const GA_PointGroup *ptgrp=0,
                                    bool snapptpos = true,
                                    UT_Array<GA_Attribute *> *ptattribs = 0,
                                    bool accurate = true);

    /// Methods for snapping attributes (for use with snapAttributes).
    /// SNAP_ATTRIBUTE_AVERAGE averages the attributes together.
    /// SNAP_ATTRIBUTE_INDEX tells snapAttributes to use an index
    /// into the list of snapped points to set the attributes for
    /// other points.
    enum SnapAttributesType 
    {
	SNAP_ATTRIBUTES_AVERAGE = 0,
	SNAP_ATTRIBUTES_INDEX
    };
    
    /// This version of snapping snaps onto grid coordinates.  You specify
    /// the number of lines per HUnit, so "2" for example will snap to 0.5
    /// boundaries.
    /// type is 0 for round nearest, 1 for round down, 2 for round up.
    /// 0.5 rounds up in nearest.
    void		 snapGrid(int type, 
				  float xlines, float ylines, float zlines,
				  float xoff, float yoff, float zoff,
				  float tol,
				  const GA_PointGroup *ptGrp=0);
    /// Same as above, but with UVs...
    void		 snapGridPointUVs(int type,
				  float xlines, float ylines, float zlines,
				  float xoff, float yoff, float zoff,
				  float tol,
				  const GA_PointGroup *ptGrp=0);
    void                 snapGridPointUVs(
                                const GA_RWHandleV3 &uvattrib, int type,
                                float xlines, float ylines, float zlines,
                                float xoff, float yoff, float zoff,
                                float tol,
                                const GA_PointGroup *ptGrp=0);
    void		 snapGridVertexUVs(int type,
				  float xlines, float ylines, float zlines,
				  float xoff, float yoff, float zoff,
				  float tol,
				  const GA_VertexGroup *vtxGrp=0);
    void                 snapGridVertexUVs(
                                const GA_RWHandleV3 &uvattrib, int type,
                                float xlines, float ylines, float zlines,
                                float xoff, float yoff, float zoff,
                                float tol,
                                const GA_VertexGroup *vtxGrp=0);

    /// Build holes in the geometry by bridging the holes to their outlines
    /// The angle should be specified as the angle (in degrees) between
    /// the normals.
    int			 buildHoles(float dist = 0.001F, float angle = 0.2F,
				int snapFace=0,
				const GA_PrimitiveGroup *primGroup=0);

    /// Remove bridged holes from other than polygons
    void		 unHole(const GA_PrimitiveGroup *primGroup=0);

    /// Unique all points in the detail.  If a point group is specified,
    /// then only points in that group are uniqued
    /// If a primitive group is specified, only those primitives will
    /// have their points uniqued.
    void		 uniquePoints(const GA_PointGroup *group=0);

    /// Unique all the points that are in this primitive, even if referenced
    /// by other primitives.
    void		 uniquePrimitive(GEO_Primitive *prim);

    /// Remove repeated references of vertices in the faces, then remove all
    /// degenerate primitives regardless of their type.
    /// Set removeRepPoints flag to remove the repeat points as well and
    /// the deletePoints flag to delete the points that were part of the 
    /// degenerate primitive.
    GA_Size		 cleanData     (const GA_PrimitiveGroup *primGrp=0,
					bool removeRepPoints = false,
					float tol = 0.001F,
					bool deleteDegenPrimPoints = false,
					bool deleteOrphanedPoints = false);

    /// Identify dirty data, which is the degenerate primitives that would
    /// be deleted by cleanData.  Return them in the returnGrp, and return
    /// the number of dirty primitives.  If returnGrp is NULL then only
    /// return the count.
    GA_Size		 getDirtyData  (GA_PrimitiveGroup *returnGrp,
					const GA_PrimitiveGroup *primGrp=0,
					bool checkRepPoints = false,
					float tol = 0.001F);

    /// If the applyToVertex is less than 0, the "natural" place will be use,
    /// otherwise 0 = point attrib, 1 = vertex attrib
    /// Returns false if the attribute failed to be created, else true.
    bool		 applyTexture(GU_TextureType type, GU_AxisType axis,
				const GA_PrimitiveGroup *primGroup=0,
				int applyToVertex = -1, int fixPolySeams = 0,
				const GU_CameraParms *userData = 0);
    /// If the applyToVertex is less than 0, the "natural" place will be use,
    /// otherwise 0 = point attrib, 1 = vertex attrib
    /// Returns false if the attribute failed to be created, else true.
    bool                 applyTexture(const UT_StringRef &uvattribname,
                                GU_TextureType type, GU_AxisType axis,
                                const GA_PrimitiveGroup *primGroup=0,
                                int applyToVertex = -1, int fixPolySeams = 0,
                                const GU_CameraParms *userData = 0);
    /// The scaleTexture() and rotateTexture() methods are depreciated.  Please
    /// use the GU_MODIFY_TEX projection and simply create the approprate
    /// post-transform.
    /// Returns false if the attribute failed to be created, else true.
    bool		 scaleTexture(float umult = 1, float uoff = 0,
				      float vmult = 1, float voff = 0,
				      float wmult = 1, float woff = 0,
				      const GA_PrimitiveGroup *primGroup=0);
    bool                 scaleTexture(const UT_StringRef &uvattribname,
                                      float umult = 1, float uoff = 0,
                                      float vmult = 1, float voff = 0,
                                      float wmult = 1, float woff = 0,
                                      const GA_PrimitiveGroup *primGroup=0);
    bool		 rotateTexture(float angle,
				      const GA_PrimitiveGroup *primGroup=0);
    bool                 rotateTexture(const UT_StringRef &uvattribname,
                                       float angle,
                                       const GA_PrimitiveGroup *primGroup=0);

    /// Methods for transforming point and vertex texture attributes:
    void		transformPointTexture(const UT_Matrix4& mat,
				const GA_PointGroup *ptGroup=nullptr,
				GEO_Delta *geodelta=0);

    void                transformPointTexture(
                                const GA_RWHandleV3 &pth,
                                const UT_Matrix4& mat,
                                const GA_PointGroup *ptGroup=nullptr,
                                GEO_Delta *geodelta=0);

    /// Precompute a list of points to soft transform, along with
    /// the distance (squared) to the closest "hard" point.
    /// The metric specifies how distance is measured in space.
    void		computeSoftTransformPointTextureCache(
				GEO_SoftTransformCache &cache,
				const GA_PointGroup *ptgroup,
				const GEO_Rolloff &rolloff,
				const GU_MetricType metric) const;

    /// Precompute a list of points to soft transform, along with
    /// the distance (squared) to the closest "hard" point.
    /// The metric specifies how distance is measured in space.
    void                computeSoftTransformPointTextureCache(
                                const GA_ROHandleV3 &pttxth,
                                GEO_SoftTransformCache &cache,
                                const GA_PointGroup *ptgroup,
                                const GEO_Rolloff &rolloff,
                                const GU_MetricType metric) const;

    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///			    any non-zero falloffs.  If not set, you must call
    ///			    computeSoftPointFalloff() to obtain the falloffs.
    void		softTransformPointTexture(
				const UT_XformOrder &order,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				float s_xy, float s_xz, float s_yz,
				float px, float py, float pz,
				const GEO_SoftTransformCache &cache,
				const GEO_Rolloff &rolloff,
				GEO_Delta *geodelta = 0,
				const GA_RWHandleF *falloff_output = NULL,
				bool *falloff_written = NULL);

    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///                     any non-zero falloffs.  If not set, you must call
    ///                     computeSoftPointFalloff() to obtain the falloffs.
    void                softTransformPointTexture(
                                const GA_RWHandleV3 &ptattr,
                                const UT_XformOrder &order,
                                float tx, float ty, float tz,
                                float rx, float ry, float rz,
                                float sx, float sy, float sz,
                                float s_xy, float s_xz, float s_yz,
                                float px, float py, float pz,
                                const GEO_SoftTransformCache &cache,
                                const GEO_Rolloff &rolloff,
                                GEO_Delta *geodelta = 0,
                                const GA_RWHandleF *falloff_output = NULL,
                                bool *falloff_written = NULL);

    void		transformVertexTexture(const UT_Matrix4& mat,
				const GA_VertexGroup *vertexGroup=nullptr,
				GEO_Delta *geodelta=0);

    void                transformVertexTexture(
                                const GA_RWHandleV3 &vtxh,
                                const UT_Matrix4& mat,
                                const GA_VertexGroup *vertexGroup=nullptr,
                                GEO_Delta *geodelta=0);

    /// Precompute a list of vertices to soft transform, along with
    /// the distance (squared) to the closest "hard" point.
    /// The metric specifies how distance is measured in space.
    /// ignore_uv_connectivity controls whether we affect vertices
    /// which are not in the same uvw-wise connected component.
    void		computeSoftTransformVertexTextureCache(
				GEO_SoftTransformCache &cache,
				const GA_VertexGroup *vtxgroup,
				const GEO_Rolloff &rolloff,
				const GU_MetricType metric,
				bool ignore_uv_connectivity) const;

    /// Precompute a list of vertices to soft transform, along with
    /// the distance (squared) to the closest "hard" point.
    /// The metric specifies how distance is measured in space.
    /// ignore_uv_connectivity controls whether we affect vertices
    /// which are not in the same uvw-wise connected component.
    void                computeSoftTransformVertexTextureCache(
                                const GA_ROHandleV3 &vtxh,
                                GEO_SoftTransformCache &cache,
                                const GA_VertexGroup *vtxgroup,
                                const GEO_Rolloff &rolloff,
                                const GU_MetricType metric,
                                bool ignore_uv_connectivity) const;

    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///			    any non-zero falloffs.  If not set, you must call
    ///			    computeSoftPointFalloff() to obtain the falloffs.
    void		softTransformVertexTexture(
				const UT_XformOrder &order,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				float s_xy, float s_xz, float s_yz,
				float px, float py, float pz,
				const GEO_SoftTransformCache &cache,
				const GEO_Rolloff &rolloff,
				GEO_Delta *geodelta = 0,
				const GA_RWHandleF *falloff_output = NULL,
				bool *falloff_written = NULL);

    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///                     any non-zero falloffs.  If not set, you must call
    ///                     computeSoftPointFalloff() to obtain the falloffs.
    void                softTransformVertexTexture(
                                const GA_RWHandleV3 &vtxh,
                                const UT_XformOrder &order,
                                float tx, float ty, float tz,
                                float rx, float ry, float rz,
                                float sx, float sy, float sz,
                                float s_xy, float s_xz, float s_yz,
                                float px, float py, float pz,
                                const GEO_SoftTransformCache &cache,
                                const GEO_Rolloff &rolloff,
                                GEO_Delta *geodelta = 0,
                                const GA_RWHandleF *falloff_output = NULL,
                                bool *falloff_written = NULL);

    /// This routine will compute the average normal of a group of points.
    /// Returns true on success and false on failure.
    /// NOTE: The version that doesn't take a point normal attribute handle
    ///       will temporarily create a detached point normal attribute if a point N
    ///       doesn't already exist.  The caller may or may not want to cache
    ///       a detached attribute and pass it in, instead.
    /// @{
    bool                computeAvgNormal(
                            const GA_PointGroup *group,
                            const GA_ROHandleV3 &normals,
                            UT_Vector3 &avg_normal) const;
    bool                computeAvgNormal(
                            const GA_PointGroup *group,
                            UT_Vector3 &avg_normal) const;
    /// @}

    /// Reverse polygons 
    void		 reversePolys(const GA_PrimitiveGroup *prmGrp=0);

    /// Conversion Routines - to convert from one primitive to another
    /// @{
    void		 convert(GEO_ConvertParms &parms);
    void		 convertNew(GEO_ConvertParms &parms);
    /// @}

    /// This routine only converts metaballs using a more comprehensive set
    /// of parameters.
    void		 convertMetaballs(GEO_MetaConvertParms& parms, const GA_PrimitiveGroup* prim_grp = NULL);

    //
    // SORTING
    //

    /// Sort by the specified dominant axis
    /// @{
    void		 sortPoints(GA_IndexMap &array,
			  		GU_AxisType axis = GU_XAXIS);
    void		 sortPoints(GA_OffsetArray &array,
			  		GU_AxisType axis = GU_XAXIS);
    void		 sortPrims(GA_IndexMap &array,
			  		GU_AxisType axis = GU_XAXIS);
    void		 sortPointList(GU_AxisType axis)
			     { sortPoints(getIndexMap(GA_ATTRIB_POINT), axis); }
    void		 sortPrimitiveList(GU_AxisType axis)
			     { sortPrims(getIndexMap(GA_ATTRIB_PRIMITIVE), axis); }
    /// @}

    /// Sorting by spatial locality.  The points and primitives will be
    /// sorted in a way that tries to assign nearby primitives closer
    /// primitive ids.
    /// @{
    void		 sortPointsSpatial();
    void		 sortPrimsSpatial();
    /// @}

    /// Sort along an arbitrary vector
    /// @{
    void		 sortPoints(GA_IndexMap &array,
				    const UT_Vector3 &o, const UT_Vector3 &d);
    void		 sortPrims(GA_IndexMap &array,
				    const UT_Vector3 &o, const UT_Vector3 &d);
    void		 sortPointList(const UT_Vector3 &o, const UT_Vector3 &d)
			     { sortPoints(getIndexMap(GA_ATTRIB_POINT), o, d); }
    void		 sortPrimitiveList(const UT_Vector3 &o,
	    				   const UT_Vector3 &d)
			     { sortPrims(getIndexMap(GA_ATTRIB_PRIMITIVE), o, d); }
    /// @}

    /// Sort in random order
    /// @{
    void		 sortPoints(GA_IndexMap &array, int seed);
    void		 sortPrims(GA_IndexMap &array, int seed);
    void		 sortPointList(int seed)
			     { sortPoints(getIndexMap(GA_ATTRIB_POINT), seed); }
    void		 sortPrimitiveList(int seed)
			     { sortPrims(getIndexMap(GA_ATTRIB_PRIMITIVE), seed); }
    /// @}

    /// Sorting according to a provided value list
    /// @{
    void		 sortElements(GA_IndexMap &array, fpreal *order);
    void		 sortPointList(fpreal *order)
			     { sortElements(getIndexMap(GA_ATTRIB_POINT), order); }
    void		 sortPrimitiveList(fpreal *order)
			     { sortElements(getIndexMap(GA_ATTRIB_PRIMITIVE), order); }
    /// @}

    /// "Rotate" (cycle) the order of vertices for all primitives in the group,
    /// by the specified number of places.
    void		 shift(int uoffset, int voffset,
				const GA_PrimitiveGroup *group=NULL);

    /// "Rotate" (cycle) the order of points or primitives in the detail, by the
    /// specified number of places.
    /// @{
    void		 shiftPoints(GA_IndexMap &, GA_Size offset);
    void		 shiftPrims(GA_IndexMap &, GA_Size offset);
    void		 shiftPointList(GA_Size aoffset)
			     { shiftPoints(getIndexMap(GA_ATTRIB_POINT), aoffset); } 
    void		 shiftPrimitiveList(GA_Size aoffset)
			     { shiftPrims(getIndexMap(GA_ATTRIB_PRIMITIVE), aoffset); }
    /// @}

    /// Reverse vertices for all primitives in the group
    void		 reverse(const GA_PrimitiveGroup *group=NULL);

    /// Reverse order of points or primitives in the detail.
    /// @{
    void		 reversePoints(GA_IndexMap &map);
    void		 reversePrims(GA_IndexMap &map);
    void		 reversePointList()
			     { reversePoints(getIndexMap(GA_ATTRIB_POINT)); } 
    void		 reversePrimitiveList()
			     { reversePrims(getIndexMap(GA_ATTRIB_PRIMITIVE)); }
    /// @}

    /// Sort points or primitives by proximity to a position
    /// @{
    void		 proximityPoints(GA_IndexMap &points,
					const UT_Vector3 &point);
    void		 proximityPrims(GA_IndexMap &primitives,
					const UT_Vector3 &point);
    void		 proximityToPointList(const UT_Vector3 &point)
			     { proximityPoints(getIndexMap(GA_ATTRIB_POINT), point); } 
    void		 proximityToPrimitiveList(const UT_Vector3 &point)
			     { proximityPrims(getIndexMap(GA_ATTRIB_PRIMITIVE), point); }
    /// @}

    /// Sort points by the order in which they are first referred-to by
    /// vertices.
    /// @{
    void                 vertexOrder(GA_IndexMap &points);
    void                 sortByVertexOrder()
			     { vertexOrder(getIndexMap(GA_ATTRIB_POINT)); }
    /// @}

    /// Create a mesh primitive or polygon primitives representing a
    /// super-quadric surface specified by parms.
    void	superEllipsoid(const GU_SuperQuadParms &parms);

    /// Split the specified polygon into convex polygons with at most
    /// maxpts vertices each.  By default, this triangulates the polygon.
    /// If flipedges is true, it will flip edges to get the Delaunay
    /// triangulation, avoiding small angles where possible.
    /// If avoiddegeneracy is true, degenerate triangles will not be generated;
    /// note that this means that the mesh may not be watertight or even
    /// connected.
    void        convexPoly(GEO_PrimPoly *pp, GA_ElementWranglerCache &wranglers,
                           GA_Size maxpts = 3, const GA_Detail *restgdp=0,
                           bool flipedges = false, bool avoiddegeneracy = false);
    /// Split polygons in the specified polygon soup into convex polygons
    /// with at most maxpts vertices each.  By default, this triangulates the
    /// polygon. If flipedges is true, it will flip edges to get the Delaunay
    /// triangulation, avoiding small angles where possible.
    /// If avoiddegeneracy is true, degenerate triangles will not be generated;
    /// note that this means that the mesh may not be watertight or even
    /// connected.
    void        convexPolySoup(GEO_PrimPolySoup *polysoup,
                               GA_Size maxpts = 3, const GA_Detail*restgdp=0,
                               bool flipedges = false,
                               bool avoiddegeneracy = false);
    /// Split polygons (including in polygon soups) in the detail into convex
    /// polygons with at most maxpts vertices each.  By default, this
    /// triangulates the polygon. If flipedges is true, it will flip edges to
    /// get the Delaunay triangulation, avoiding small angles where possible.
    /// If avoiddegeneracy is true, degenerate triangles will not be generated;
    /// note that this means that the mesh may not be watertight or even
    /// connected.
    void        convex(GA_Size maxpts=3, GA_PrimitiveGroup *primGroup=0,
                       const GA_Detail *restgdp=0, bool flipedges = false,
                       bool avoiddegeneracy = false);

    /// Add non-planar polygon primitives to nonplanargroup.
    GA_Size	findNonPlanar(GA_PrimitiveGroup *nonplanargroup, float tol = 0.0001F,
			      const GA_PrimitiveGroup *searchprimgroup = NULL);

    /// Clip primitives, keeping everything where dot(normal, P) >= d.
    /// If clippts is true, disconnected points are also clipped.
    void	clip(UT_Vector3 &normal, float d = 0, int normlize = 0,
		     const GA_PrimitiveGroup *primGroup = 0,
		     bool clippts = false);

//
//  Polygon creasing
    void	crease(UT_Vector3 &normal, float d = 0, 
		       int normlize = 0, int outputGroups = 0,
		       GA_PrimitiveGroup *above = 0,
		       GA_PrimitiveGroup *below = 0,
		       const GA_PrimitiveGroup *creaseGroup = 0);
//
// fractals
    void 	fractalize(int seed = 1, float roughness = 0.6F, 
			   float scaleby=1, int divs=1, 
			   int fixedBoundry = 1, int useVtxNorms = 0,
			   float nx = 0, float ny = 0, float nz = 1,
			   const GA_PrimitiveGroup *fractalGroup = 0);

    //
    // Shrink Wrap and tools

    /// This routine is originally developed for the Bullet RBD solver to adjust
    /// the geometry of a convex 3D polygon to remove the space around the geometry
    /// caused by the collision margin required for the Bullet solver.
    void	shrink( fpreal distance,
		        fpreal clip_tolerance = 0.0001,
		        fpreal consilidate_tolerance = 0.001);

    /// preliminary routine to setup the gdp to be shrunk by the shrink routine
    bool	tetrahedralizeForShrink( const GA_PointGroup *ptGroup = 0,
					 GU_Detail	     *gdp = 0 );


    /// Twist operations. The method returns 0 if everything goes OK, else -1.
    /// "pasted" returns 1 if at least one pasted surface was involved in
    /// the deformation, else 0.
    int 	 nonLinearDeform(GU_DeformType type,const GU_TwistParms &parms,
				 int &pasted);

    /// NOTE: This is something specific to NURBS surfaces and it uses
    ///       pasted surface primitives, so it's probably not what you want!!!
    /// Creates an offset surface from the specified surface & radius.
    GEO_Hull	*offset(const GU_OffsetParms &parms);

    /// Create a Gordon surface out of the given bi-linear network of faces.
    /// The faces don't have to be the same type or order or anything.
    /// We return the surface if OK and 0 otherwise. If the u or v faces 
    /// contain only one face, we automatically generate a final curve. If
    /// both u faces and vfaces have 1 curve or one has 2 and the other 1,
    /// we generate a Coons surface. "accurate" is used when the curves do not
    /// intersect (i.e. when we must compute their min distances).
    /// @{
    GEO_Hull	*gordon(GEO_SurfaceType surftype, int accurate,
		        const GA_PrimitiveGroup &ufaces,
		        const GA_PrimitiveGroup &vfaces,
			int reparameterize = 0);
    GEO_Hull	*gordon(GEO_SurfaceType surftype, int accurate,
		      const UT_Array<GEO_Primitive*> &uprims_in,
		      const UT_Array<GEO_Primitive*> &vprims_in,
		      GU_SkinCache& skin_cache, 
		      int reparameterize = 0);
    /// @}

    /// Generate a Coons surface out of up to 4 boundary curves. Return a 
    /// pointer to the surface if successful and 0 otherwise. The faces do
    /// not have to have the same type or order.
    GEO_Hull	*coons(GEO_SurfaceType surftype,
		       const GEO_Face &uface1, const GEO_Face &vface1,
		       const GEO_Face *uface2 = 0, const GEO_Face *vface2 = 0);

    /// Generate a skinned surface out of a set of cross-sections. The faces
    /// don't have to be the same type or order. We return the surface is OK
    /// and 0 otherwise. Specifying a vorder of 0 tells the method to
    /// come up with a legal default value for the surface. If "doskin" is
    /// true, the cross-sections will be right on the surface; otherwise, we
    /// build a ruled surface, which simply steals the curves' CVs and assigns
    /// them to the surface.
    GEO_Hull	*skin(GU_SkinParms &parms, GU_SkinCache& skin_cache);

    /// This restrictive skinning method assumes the faces are of the same type
    /// and have the same number of CVs. order 0 means pick most suitable one.
    /// If doskin is false, we build a ruled surface. nprims is the number
    /// of faces in the group; we compute it if -1. We return the resulting
    /// surface and do not delete the input faces. We also assume that if
    /// nprims > 2 and vorder != 2 and doskin, all the faces are nonrational.
    /// Finally, the provided vParmValues if given specifies what v coordinate
    /// each of the provided faces should interpolate.
    /// @{
    GEO_Hull	*skin(const GA_PrimitiveGroup &ucfaces,
		      GEO_SurfaceType surftype = GEO_PATCH_QUADS,
		      unsigned vorder = 0, int vwrap = 0,
		      int doskin = 1, int nprims = -1,
		      const UT_Vector *vParmValues = 0);
    GEO_Hull	*skin(const UT_Array<GEO_Primitive*> & prims_in,
                      GU_SkinCache& skin_cache,
		      GEO_SurfaceType surftype = GEO_PATCH_QUADS,
		      unsigned vorder = 0, int vwrap = 0,
		      int doskin = 1, int nprims = -1,
		      const UT_Vector *vParmValues = 0);
    /// @}

    /// See GU_Ruled.h for the various closure and interpolation types
    /// which are valid. Specifying an orderv of 0 makes the routine
    /// come up with a legal default value for the surface.
    int		ruled( GEO_Hull *&surface, const GU_RuledParms &p,
			const GU_CapOptions &caps);

    /// This is what the Skin SOP and Sweep SOP use for skinning
    /// cross-sections.
    /// See GU_CurveNetwork.h for parameter types. This method generates
    /// a skinned surface, a Coons surface, or a Gordon surface depending on
    /// the primitive groups it is given. Specifying an orderv of 0 makes the
    /// routine come up with a legal default value for the surface.
    int 	curveNetwork(GEO_Hull *&hull, GU_CurveNetworkParms &p, 
			    int &count, GU_SkinCache& skin_cache);

    GU_ERROR 	sweep( GU_SweepParms parms );
    void	rails( GU_RailParms parms );

    /// Join more faces or surfaces together.
    GEO_Primitive	*join( GU_JoinParms &p, int &count);

    /// Generate a fillet between two curves on surfaces:
    GEO_Hull		*filletTrims(GU_TrimFilletParms &parms, int &count);

    /// Generate a rounded fillet:
    GEO_Hull		*filletRound(GU_RoundFilletParms &parms);
    /// NOTE: The first 5 columns of the matrix must be GEO_Curve*,
    ///       and the other 2 columns of the matrix must be GEO_TPSurf*.
    void		 filletRoundNetwork(UT_RefMatrix<GEO_Primitive *>,
					    GU_RoundFilletParms &parms);

    /// Fillet a set of faces/hulls
    /// Return 0 if OK, -1 otherwise (non face/hull types)
    int			 fillet(GU_FilletParms &parms, int &count);

    /// Loft (stitch) a number of polygons together without changing the number
    /// of points in the detail.
    /// @{
    void		 loft(GU_LoftParms &p, const GA_PointGroup &g1,
					       const GA_PointGroup &g2);
    void		 loft(GU_LoftParms &p,
			      const GA_PrimitiveGroup *group = 0);
    /// @}

    /// Revolves all curves in the given gdp (or only curves in the 
    /// group if the group is given) around a line given by the center 
    /// and axis. Only polygonal curves use the divisions. NURBS and
    /// Bezier curves are revolved a special way to create a perfectly
    /// round surface with a minimum number of revolution copies.
    int		revolve( const GU_RevolveParms &parms,
			 const GU_CapOptions &caps, int &count);

    /// Warp curves or surfaces. Return 0 if successful and -1 otherwise.
    int		warp(GU_WarpParms &parms);

    /// Curve Clay it!
    GU_ERROR	curveClay(const GU_CurveClayParms &ccparm);

    /// This puts endcaps on the specified hull
    int		endCap(GEO_Hull &hull, const GU_CapParms &parms);
    int		endCap(GEO_Face &face, const GU_CapParms &parms);

    /// This places all caps on the hull
    int		setCaps(GEO_Hull &hull, const GU_CapOptions &parms, int &count);
    int		setCaps(GEO_Face &face, const GU_CapOptions &parms, int &count);

    /// Creates a deformed copy of a source detail given a pair of 
    /// rest and deform details. Return 1 if a pasted surface was deformed,
    /// else 0. If 'deform_history' is passed in, every time we deform a point
    /// from our source, we mark this by setting the bit in 'deform_history'
    /// whose index corresponds to the point's number to 1. Useful if you want
    /// to know which points we deformed.
    int  	lattice(const GU_Detail *source, const GU_Detail *rest,
			const GU_Detail *deform, int xdiv, int ydiv, int zdiv,
			const GA_PointGroup *ptgroup = 0,
			UT_BitArray *deform_history = NULL,
			GU_LatticeType type = GU_LATTICE_BEZIER);

    /// All points of the gdp are deformed according to the meta source
    /// field that surrounds it.  The deformation is controlled by a
    /// transformation matrix.
    int		magnet( const GU_MagnetParms &parms );

    /// trace a raster image to form closed polygons
    float	trace(GU_TraceParms *parms);

    /// add point texture attributes to traced geometry
    void	applyTraceTexture(float xres, float yres, float resolution);


    /// fit a sequence of discrete points to a series of bezier curves
    void	fit2DCurve(const GU_Detail *source, float error_squared,
			   GA_PrimitiveGroup *primGroup = 0);

    /// Fit a curve through its breakpoints
    /// @{
    GEO_Curve	*interpCurveBreakpoints(const GEO_Face &face,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD);
    GEO_Curve	*interpCurveBreakpoints(const GA_OffsetList &point_offsets,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD);
    GEO_Curve	*interpCurveBreakpoints(const UT_Vector4Array &v4Data,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD);
    /// @}

    /// interpolation data points
    /// @{
    GEO_Curve	*interpCurveGlobal(const GEO_Face &face,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);
    GEO_Curve	*interpCurveGlobal(const GA_OffsetList &point_offsets,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);
    GEO_Curve	*interpCurveGlobal(const GA_Range &point_range,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);
    GEO_Curve	*interpCurveGlobal(const UT_Vector4Array &v4Data,
			    const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);
    GEO_Curve	*interpCurveLocal(const GEO_Face &face,
			    const GA_PrimitiveTypeId &type=GEO_PRIMBEZCURVE,
			    int order = 4, int corner=0);
    GEO_Curve	*interpCurveLocal(const UT_Array<GA_Offset> &goData,
			    const GA_PrimitiveTypeId &type=GEO_PRIMBEZCURVE,
			    int order = 4, int wrapped=0, int corner=0);
    GEO_Curve	*interpCurveLocal(const UT_Vector4Array &v4Data,
			    const GA_PrimitiveTypeId &type=GEO_PRIMBEZCURVE,
			    int order = 4, int wrapped=0, int corner=0);

    GEO_TPSurf	*interpSurfGlobal(const GEO_Hull &mesh,
			  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
			  int uOrder=4, int vOrder=4,
			  GEO_SurfaceType sType=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);

    GEO_TPSurf	*interpSurfGlobal(const GA_OffsetMatrix &goMesh,
			  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
			  int uOrder=4, int vOrder=4,
			  bool uWrapped=false, bool vWrapped=false,
			  GEO_SurfaceType sType=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);

    GEO_TPSurf	*interpSurfBreakpoints(const GEO_Hull &mesh,
			  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
			  int uOrder=4, int vOrder=4,
			  GEO_SurfaceType sType=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);

    GEO_TPSurf	*interpSurfBreakpoints(const GA_OffsetMatrix &goMesh,
			  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
			  int uOrder=4, int vOrder=4,
			  bool uWrapped=false, bool vWrapped=false,
			  GEO_SurfaceType sType=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);
    /// @}

    /// Approximate data points given a tolerance.
    /// Only for open endinterpolated surface.
    /// @{
    GEO_Curve	*approxCurveGlobal(const GEO_Face &face,
				   const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE, 
				   int order=4, 
				   float tol=1e-1F, float smooth=0.0F,
				   int noMultipleKnots=1);
    GEO_Curve	*approxCurveGlobal(const UT_Vector4Array &v4Data,
				   const GA_PrimitiveTypeId &type=GEO_PRIMNURBCURVE, 
				   int order=4, int wrapped=0,
				   float tol=1e-1F, float smooth=0.0F,
				   int noMultipleKnots=1);

    GEO_TPSurf	*approxSurfGlobal(const GEO_Hull &mesh,
				  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
				  int uOrder=4, int vOrder=4,
				  GEO_SurfaceType sType=GEO_PATCH_QUADS,
				  float tol=1e-1F, float smooth=0.0F,
				  int uNoMultipleKnots=1,
				  int vNoMultipleKnots=1);
    GEO_TPSurf	*approxSurfGlobal(const GA_OffsetMatrix &gpMesh,
				  const GA_PrimitiveTypeId &type=GEO_PRIMNURBSURF,
				  int uOrder=4, int vOrder=4,
				  int uWrapped=0, int vWrapped=0,
				  GEO_SurfaceType sType=GEO_PATCH_QUADS,
				  float tol=1e-1F, float smooth=0.0F,
				  int uNoMultipleKnots=1,
				  int vNoMultipleKnots=1);
    /// @}

    /// methods to refine face and hull types
    /// @{
    void	refine(float *uunit, int ulen,
		       float *vunit, int vlen,
		       int countu=1, int countv=1, 
		       int arcspacing = 0,
		       const GA_PrimitiveGroup *primGroup = 0);

    void	unrefine(float umin, float umax,
		         float vmin, float vmax,
			 int countu=1, int countv=1,
		         float utol=0.001F, float vtol=0.001F, 
		         const GA_PrimitiveGroup *primGroup = 0);

    void	subdivide(float *uunit, int ulen,
			  float *vunit, int vlen,
			  int arcspacing = 0,
			  const GA_PrimitiveGroup *primGroup = 0);

    /// op = {0 = isoparms, 1 = points, 2 = profiles}
    int 	extract(float *uunit, int ulen,
			float *vunit, int vlen,
		        const GA_PrimitiveGroup *primGroup = 0,
			GA_PointGroup *newPoints = 0,
			int op = 0, int keepOriginal = 0,
			int atbreakpoints = 0, int use_arc_length = 0);

    void	extractIsoParms(float *uunit, int ulen,
			        float *vunit, int vlen,
			        GA_PrimitiveGroup *newPrims = 0,
			        const GA_PrimitiveGroup *primGroup = 0);

    void	extractPoints(float *uunit, int ulen,
			      float *vunit, int vlen,
			      GA_PointGroup *newPoints = 0,
			      const GA_PrimitiveGroup *primGroup = 0);

    void	extractProfiles(float *uunit, int ulen,
				float *vunit, int vlen, 
			        const GA_PrimitiveGroup *primGroup = 0);

    void	extractIsoParmsAtBreak(float *uunit, int ulen,
				       float *vunit, int vlen,
				       GA_PrimitiveGroup *newPrims = 0,
			               const GA_PrimitiveGroup *primGroup = 0);

    void	extractPointsAtBreak(float *uunit, int ulen,
				     float *vunit, int vlen,
				     GA_PointGroup *newPoints = 0,
				     const GA_PrimitiveGroup *primGroup = 0);

    void	extractProfilesAtBreak(float *uunit, int ulen,
				       float *vunit, int vlen, 
			               const GA_PrimitiveGroup *primGroup = 0);


    int 	cut(float *uunit, int ulen,
		    float *vunit, int vlen,
		    const GA_PrimitiveGroup *primGroup = 0,
		    int keepin=1, int keepout=0, int atbreakpoints = 0,
		    int allU = 0, int allV = 0, int use_arc_length = 0);
    /// @}


    /// approximate a spline by using polygonal hull inputs
    /// deletes existing polygons if deleteAll is true
    void	polySpline(GU_PolysplineParms &parms,
			   const GA_PrimitiveGroup *primGroup = 0,
			   bool deleteAll = true);

    /// approximate a patch by using polygonal or mesh hull inputs
    /// Returns true on success, else false if an error occurred.
    bool	polyPatch(GU_PolypatchParms &parms,
			  const GA_PrimitiveGroup *primGroup = 0);

    /// reduce the number of polygons in a detail:
    int		polyReduce(GU_PolyReduceParms &parms,
			    const GA_PrimitiveGroup *primGroup = 0,
			    int *pointIndexTable = 0);

    /// Create polygon soup(s), i.e. GU_PrimPolySoup, where possible in this detail
    void	polySoup(const GEO_PolySoupParms &parms , const GU_Detail *srcdetail);

    /// Create geometry through premise and rule substitution
    void 	lsystem(GU_LSystemParms &lp);

    /// Get information about a GU_Detail
    /// If maxlines < 0, there will be no limit to the number of groups
    /// If overridememusage is positive, it will be used rather than the
    /// getMemoryUsage().
    int		info(std::ostream &os,
			int max_group_lines=15,
			bool pretty=false,
                        const UT::ArraySet<const void *> *dontcounttheseblocks = 0,
                        bool instanced=false) const;
    /// Get information text for the geometry.  This will @b append to the work
    /// buffer.
    void	info(UT_WorkBuffer &wbuf,
			int max_group_lines=15,
			bool pretty=false,
                        const UT::ArraySet<const void *> *dontcounttheseblocks = 0,
                        bool instanced=false) const;

    /// Fills in a stat structure with the volume information.
    virtual void statVolumes(GA_Stat &stat, uint level/*=0xffff*/) const;

    /// @c intersectRay will find the closest intersection with a primitive
    ///	in the detail.  If the dist pointer is nil, then the first intersection
    ///	found will be returned (instead of the closest).  The nml will contain
    ///	the normal for the primitive at the point of intersection.
    /// Accuracy of 0 is inaccurate, 1 is normal, 2 engages Algebraic pruning
    /// where available.
    ///
    /// @param o The ray origin
    /// @param d The ray directin
    /// @param tmax This can be used to limit the ray to intersections within
    ///      the given radius.
    /// @param tol Intersection tolerance
    /// @param dist Return the distance from the origin to the hit
    /// @param pos Return the hit position (i.e. <tt>o + dist*d</tt>)
    /// @param nml Return the normal of the primitive at the hit position
    /// @param accurate Determine primitive intersection algorithms for NURBS
    /// @param u The u parametric coordinate of the hit surface
    /// @param v The v parametric coordinate of the hit surface
    /// @param ignoretrim Ignore trim inside/out tests for trimmed surfaces.
    ///
    /// @warning If the @c dist parameter is 0 the first hit found will be
    ///     returned.  This may @b not be the closest hit.  This can be used to
    ///     perform a quick check to see if @b any primitives intersect the ray
    ///     (i.e. for shadow casting).
    ///
    /// @warning This function iterates over all primitives without any
    /// acceleration structure.  If you need to send multiple rays, please see
    /// GU_RayIntersect.
    SYS_DEPRECATED_HDK(16.0)
    GEO_Primitive	*intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *dist = 0, UT_Vector3 *pos = 0, 
				UT_Vector3 *nml = 0, int accurate = 0, 
				float *u = 0, float *v = 0, 
				int ignoretrim = 1) const;

    /// Figure out the parameters of the face/surface that all the given
    /// faces/surfaces should have in order to be fully compatible with one
    /// another. Return 0 if all the primitive types are faces/surfaces,
    /// and -1 otherwise.
    /// @{
    int 		commonFaceConfig(
				const GA_PrimitiveGroup &faceGroup,
				GA_PrimCompat::TypeMask &type, int &order,
				bool &open, bool &ends) const;
    int 		commonFaceConfig(
				const UT_Array<GEO_Primitive*> &faces_in,
				GA_PrimCompat::TypeMask &type, int &order,
				bool &open, bool &ends) const;
    int			commonSurfaceConfig(const GA_PrimitiveGroup &surfs,
				GA_PrimCompat::TypeMask &type,
				int &orderu, int &orderv,
				bool &openu, bool &openv,
				bool &endsu, bool &endsv) const;
    /// @}

    /// Incompatible faces in infaces are made compatible and set in outfaces.
    /// Return 0 of OK, -1 otherwise. If extratype is not nil and the highest
    /// face type of the infaces is less than *extratype, replace the common
    /// type by *extratype. In the method that takes 2 input groups, we make
    /// sure that the primitives in the 2 groups have the same type. Set
    /// equalcvs to true make them all have the same number of CVs too.
    /// @{
    int			makeFacesCompatible(
				const GA_PrimitiveGroup &infaces,
				GA_PrimitiveGroup &outfaces,
				bool mustopen = false,
				bool mustends = false,
				bool nonrational = false,
				GA_PrimCompat::TypeMask *extratype = 0,
				bool equalcvs = true);
    int			makeFacesCompatible(
				const UT_Array<GEO_Primitive*> &prims_in,
				UT_Array<GEO_Primitive*>& prims_out,
				bool mustopen = false,
				bool mustends = false,
				bool nonrational = false,
				GA_PrimCompat::TypeMask *extratype = 0,
				bool equalcvs = true);
    int			makeFacesCompatible(
				const GA_PrimitiveGroup &infacesU,
				const GA_PrimitiveGroup &infacesV,
				GA_PrimitiveGroup &outfacesU,
				GA_PrimitiveGroup &outfacesV,
				bool mustopen = false,
				bool mustends = false,
				bool nonrational = false,
				GA_PrimCompat::TypeMask *extratype = 0,
				bool equalcvs = true);
    int			makeFacesCompatible(
				const UT_Array<GEO_Primitive*> &ufaces_array,
				const UT_Array<GEO_Primitive*> &vfaces_array,
				UT_Array<GEO_Primitive*> &ucfaces_array,
				UT_Array<GEO_Primitive*> &vcfaces_array,
				bool mustopen = false,
				bool mustends = false,
				bool nonrational = false,
				GA_PrimCompat::TypeMask *extratype = 0,
				bool equalcvs = true);
    /// @}

    /// Loft a subsection of each hull and update the vertex indices where
    /// the subsection occurs. Does partial nurb merging if the bases is given
    /// Assumes the two hulls have been properly reconfigured.
    static int		loftHulls(GEO_Hull *left, GEO_Hull *right,
				   float lwidth1, float lwidth2, 
				   float rwidth1, float rwidth2,
				   int sharp, int &lstart, int &rstart, 
				   int &lmax, int &rmax,
				   int loftU, GA_NUBBasis *nubbasis);

    /// Refine the hull at the unit domain value and return the
    /// row/col index where it occurs
    /// @{
    static int		getSubdividedCV(GEO_Hull *hull, int udir, float unit);
    static int		getSubdividedCV(GEO_Face *face, float unit);
    /// @}

    /// Methods for registering and unregistering geometry io converters.
    /// @{
    static void		 registerIOTranslator(GEO_IOTranslator *trans);
    static void		 unregisterIOTranslator(GEO_IOTranslator *trans);
    static exint	 getIOTranslatorCount();
    static const GEO_IOTranslator	*getIOTranslator(exint i);
    /// @}

    GA_Detail::IOStatus	        saveH9File(const char *filename,
				const GA_SaveOptions *options) const;
    static GA_Detail::IOStatus	statH9File(const char *filename,
					GA_Stat &sbuf, uint level);

    /// Returns true if filename is a recognized extension.
    static bool	 	 isFormatSupported(const char *filename);

    /// Allocate an IO handler for a filename, or return NULL if the handler is
    /// the classic format.  The returned translator must be deleted.  The
    /// easiest way to do this is with a UT_UniquePtr: @code
    ///   UT_UniquePtr<GEO_IOTranslator> xlate(getSupportedFormat(filename));
    ///   if (xlate)
    ///       xlate->fileSave(gdp, ...);
    /// @endcode
    static GEO_IOTranslator	*getSupportedFormat(const char *filename);

    /// Align a set of primitives by rotation and translation
    void		 align(GU_AlignParms &parms);	

    /// Align a set of faces/hulls at optional levels of continuity.
    /// face/hull pre/posttranslate + row stitching + row tangent
    /// Faces are aligned with faces, hulls with hulls. 
    /// Return 0 if OK, -1 otherwise (non face/hull types)
    int			 stitch(GU_StitchParms &parms);

    /// Incompatible surfaces in insurfs are made compatible and set in outsurfs.
    /// Return 0 of OK, -1 otherwise. If extratype is not nil and the highest
    /// surface type of the insurfs is less than *extratype, replace the common
    /// type by *extratype. Set equalcvs to true make them all have the same number
    /// of CVs too.
    int			 makeSurfacesCompatible(
				const GA_PrimitiveGroup &insurfs,
				GA_PrimitiveGroup &outsurfs,
				bool mustopenu=false, bool mustopenv=false,
				bool mustendsu=false, bool mustendsv=false,
				bool equalcvsu=true, bool equalcvsv=true,
				GA_PrimCompat::TypeMask *extratype = 0);

    /// NOTE: These functions only apply to pasted surface primitives,
    ///       so they're probably not what you want!
    /// Convenience wrappers for several common pasting operations. The last
    /// method destroys both groups:
    /// @{
    GA_PrimitiveGroup	*getPastedSurfaces(const char *n = "__gu_all_pasted__");
    GA_PrimitiveGroup	*getPastedSurfaces(GA_PrimitiveGroup *&used,
					   const char *n = "__gu_all_pasted__",
					   const char *u = "__gu_used_pasted__");
    void		 updatePastedDisplacement(GA_Offset ptoff,
						const GA_PrimitiveGroup *all,
						GA_PrimitiveGroup *used);
    int 		 updatePastedDisplacements(void);
    int 		 updatePastedDependents(GA_PrimitiveGroup *all,
						GA_PrimitiveGroup *used);
    /// @}

    /// Error Routines
    /// @{
    GU_ERROR             error() const
			 {
			     return UTgetErrorManager()->getSeverity();
		         }

    void                 addMessage(GU_ErrorCodes code, 
				    const char *msg = 0) const
			 {
			     UTgetErrorManager()->addMessage("GU", code, msg);
			 }
    void                 addWarning(GU_ErrorCodes code, 
				    const char *msg = 0) const
			 {
			     UTgetErrorManager()->addWarning("GU", code, msg);
			 }
    void                 addError(GU_ErrorCodes code, const char *msg = 0) const
			 {
			     UTgetErrorManager()->addError("GU", code, msg);
			 }
    /// @}

    /// Returns a cook selection to fill in. If an already existing cook
    /// selection exists, of the wrong type, it is removed, and a new one
    /// created.  Otherwise the existing cook selection is optionally cleared
    /// and returned.
    ///
    /// The cook selection is the selection being manipulated and displayed
    /// by a node.
    GU_SelectionHandle	 getOrCreateCookSelection(GA_GroupType group_type,
						  bool ordered = false,
						  bool clear_matching = false);

    /// Returns the group type of the current cook selection, or 
    /// GA_GROUP_INVALID if there isn't one.
    GA_GroupType	 getCookSelectionGroupType() const;

    /// Creates a cook selection that directly references the supplied group.
    /// Any changes to the cook selection later will copy this group and use
    /// that copy.
    ///
    /// You must call removeCookSelection() if this group is later destroyed
    /// and the cook selection is still referencing it, which can be checked
    /// by calling hasShallowCookSelectionReference(group).
    ///
    /// NB: Only use this method if you know what you're doing.  Any gains in
    ///     performance are likely not to be worth the management pain.
    GU_SelectionHandle	 createShallowCookSelection(GA_Group *group);

    /// Returns whether this detail has a shallow cook selection referencing
    /// the specified group.
    bool		 hasShallowCookSelectionReference(
						const GA_Group *group) const;
    
    /// Returns the current cook selection;
    GU_SelectionHandle	 cookSelection() const;

    /// Removes the current cook selection from this detail. Since the selection
    /// is a shared object, there's no guarantee that it will be immediately
    /// destroyed, but only that this detail is no longer associated with it.
    void		 removeCookSelection();

    /// Do not use this method unless you know EXACTLY what you are doing.  It
    /// forces the use of the supplied cook selection.
    void 		 forceCookSelection(GU_SelectionHandle sel);

    /// Do not use this method unless you know EXACTLY what you are doing.  It
    /// removes the cook selection from the detail without orphaning it so the
    /// only safe thing to do is to return it later using forceCookSelection().
    GU_SelectionHandle	 stealCookSelection();
    
    /// Remove points from polygons if they lie on (within a tolerance)
    /// the line connecting the previous and next points.
    void		removeInlinePoints(float tol,
					   const GA_PrimitiveGroup *prims);

    /// Remove zero area polygons
    /// Returns number of polygons removed
    /// @{
    GA_Size		removeZeroAreaFaces(GA_PrimitiveGroup *grp,
					    bool ignoreOpenFaces = false,
					    float tolerance = 0.001F,
					    bool onlyPolys = true);
    GA_Size		removeZeroAreaFaces(const UT_IntArray &primlist,
					    bool ignoreOpenFaces = false,
					    float tolerance = 0.001F,
					    bool onlyPolys = true);
    /// @}

    /// Deletes the geometry in the given group, unlike deleteEdges,
    /// it doesn't repair the holes left behind.
    void		deleteGroupGeometry(const GA_Group &group);

    /// Deletes edge geometry
    SYS_DEPRECATED_HDK(16.0)
    void		deleteEdges(GA_EdgeGroup &edgegroup,
				    bool del_inline_points,
				    float inlinetol,
				    bool del_unused_points = true,
				    bool del_bridges = true);

    /// There are three different ways of handling situations where all edges
    /// that connect polygon boundaries are dissolved.
    /// - GU_BRIDGEMODE_BRIDGE @n
    ///   Insert bridge edges to connect the boundary loops.
    /// - GU_BRIDGEMODE_DISJOINT @n
    ///   Create a separate polygon for each boundary loop.
    /// - GU_BRIDGEMODE_DELETE @n
    ///   Delete the polygons.
    enum BridgeMode
    {
	GU_BRIDGEMODE_BRIDGE,
	GU_BRIDGEMODE_DISJOINT,
	GU_BRIDGEMODE_DELETE
    };
    void		dissolveEdges(const GA_EdgeGroup &edgegroup,
				    bool del_inline_points,
				    fpreal inlinetol,
				    bool del_unused_points,
				    BridgeMode bridge_mode,
				    bool del_degenerate_bridges,
				    bool boundary_curves);

    /// Flips edges
    void		flipEdges(const GA_EdgeGroup &edgegroup,
                                  int rotations = 1,
				  bool rotattribs = false,
				  GA_EdgeGroup *outedges = 0);
    /// Splits edges
    /// This is the main method for the PolySplit SOP.  The splitlocs are
    /// the positions along which you wish to split polygons.  When quadcut
    /// is false, the Shortest Path algorithm is used, otherwise the Quad Cut
    /// algorithm is used.  The forcecut flag is used with the Shortest Path
    /// algorithm to force it to perform cuts where the initial cut has failed.
    /// To form a complete path of edges, supply the first location again as
    /// the last location in the splitlocs array. If quadcomplete is true and
    /// quadcut is false, then in certain cases extra edges will be inserted so
    /// that faces which began as quads get split into faces which are quads. If
    /// outPositions is set, it will be filled with a list of points in the path
    /// in RE_PRIM_LINES fashion. If fixEnds is set, either end of the path that
    /// is on a face will be given an extra segment to connect it to the nearest
    /// point to ensure that the resulting polygons don't self-intersect.
    void		edgeSplit(const GU_SplitLocArray &splitlocs,
				  bool quadcut = false,
				  bool forcecut = true,
				  GA_EdgeGroup *outedges = 0,
				  float tolerance = SYS_FTOLERANCE,
				  bool quadcomplete = false,
				  bool modifyGdp = true,
				  UT_Fpreal32Array *outPositions = NULL,
				  bool fixEnds = false);

    /// Inserts points on edges
    /// @{
    void		divideEdges(const GA_EdgeGroup &edgegroup, int numdivs = 1,
				    bool applytoall = true,
				    bool use_shared_points = false,
				    GA_EdgeGroup *outedges = 0,
				    UT_Array<GA_Offset> *new_points = 0,
				    bool preserve_edge_dir = false);
    void		divideEdges(const GA_EdgeGroup &edgegroup,
				    int numdivs,
				    bool applytoall,
				    bool use_shared_points,
				    GA_EdgeGroup *outedges,
				    UT_Array<GA_Offset> *new_points,
				    bool preserve_edge_dir,
				    float fraction);
    /// @}

    /// Inserts points on edges, with a different fraction and number of
    /// divisions per edge.
    void                divideEdges(const UT_Array<GU_EdgeDiv *> &divlocs,
                                    GA_PointGroup &outgrp);

    /// Transform breakpoints
    void		transformBreakpoints(const UT_Matrix4 &mat,
					     const GA_BreakpointGroup *grp = 0,
					     int quickxform = 0);


    /// Extrude faces, uses local face spaces and tries to mirror extrusions.
    void		polyExtrude(const GU_PolyExtrudeParms &parms);

    /// Collapse Edges
    void		edgeCollapse(const GA_EdgeGroup &edgegroup,
				     bool removedegen = true,
				     bool updatenmls  = true,
				     GA_PointGroup *outpoints = 0);

    /// Adds crease weights to edges
    void 		edgeCrease(const GU_EdgeCreaseParms &parms); 

    /// Insets points
    void		pointInset(const GA_PointGroup *ptgroup,
				   float insetdist, float nmldist);

    /// Find the oriented bounding box that contains the given primitives.
    /// The box is returned as a rotate+translate matrix to the box's
    /// orientation and centre, and a vector containing the boxes extents
    /// along its primary axes.  The extents are radii, ie, one-half
    /// of what a bounding box size() would be.
    ///
    /// Returns zero radii if the group contains no primitives.
    void		getOBBox(const GA_PrimitiveGroup *grp,
				 UT_Matrix4 &transform,
				 UT_Vector3 &radii) const;
    /// Return the oriented bounding box that contains the given points,
    /// returning a UT_OBBox, which contains translation and rotation information
    void		getOBBoxForPoints(const GA_PointGroup *grp,
				    UT_OBBoxD &obb) const;

    /// Get cached bounding box.  This uses the meta cache count and the data
    /// id on P to determine whether the bounds need to be recomputed.
    ///
    /// It's possible that the cached bounds might be invalid in some cases
    /// (where the geometry is modified without updates to the meta cache count
    /// or the data id on P.
    bool	getCachedBounds(UT_BoundingBox &box) const;
    /// Forcibly clear the cached bounds without having to update the meta
    /// cache count or data ID on P.
    void	clearCachedBounds();

    /// Make each primitive planar, returns the number of primitives xformed
    GA_Size		makePrimsPlanar(const GA_PrimitiveGroup *grp = 0);

    static void		 loadIODSOs();

    /// Create a list of all internal file extensions
    static void		 getFileExtensions(UT_StringArray &result);

    /// Check to see whether the file extension matches a known binary file
    /// extension.  The argument is the full filename.
    static bool		matchBinaryFileExtension(const UT_StringRef &filename);

    /// Check to see whether the file extension matches a known ASCII file
    /// extension.  The argument is the full filename.
    static bool		matchASCIIFileExtension(const UT_StringRef &filename);


    /// These methods assume no changes have been made to the geometry since the
    /// last call to incrementMetaCacheCount(). "attrib" should refer to a string
    /// or integer attribute.
    int			 getUniqueValueCount(const GA_Attribute *attrib) const;
    int			 getUniqueIntegerValue(const GA_Attribute *attrib, int idx) const;
    const char		*getUniqueStringValue(const GA_Attribute *attrib, int idx) const;
    GA_Range		 getRangeByValue(const GA_Attribute *attrib, int v) const;
    GA_Range		 getRangeByValue(const GA_Attribute *attrib, const char *v) const;


    class AttribValueLookupTable;
    class AttribSingleValueLookupTable;

    const AttribValueLookupTable *getLookupTable(const GA_Attribute *attrib) const;
    const AttribSingleValueLookupTable *getSingleLookupTable(const GA_Attribute *attrib) const;

protected:
    /// Register intrinsic attributes
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveFactory &factory);

private:
    int 		 twist		(const GU_TwistParms &parms, int &p);
    int 		 bend		(const GU_TwistParms &parms, int &p);
    int 		 squashStretch	(const GU_TwistParms &parms, int &p);
    int 		 shear		(const GU_TwistParms &parms, int &p);
    int 		 taper		(const GU_TwistParms &parms, int &p);
    int 		 linearTaper	(const GU_TwistParms &parms, int &p);

    template <typename ArrayType>
    GEO_Curve *		 privateInterpCurveGlobal(
			    const ArrayType &gpData,
			    const GA_PrimitiveTypeId &type,
			    int order, int wrapped,
			    GA_ParameterizationType parmType,
			    GA_KnotSpaceType knotSpaceType);

    void		 fillGrid(GEO_Hull &hull, int rows, int cols,
				  float x_size, float y_size,
				  float x_center, float y_center,
				  float z_center,
				  GU_OrientationType plane);

    void		 convertPolysToHull(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertToSoups(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertVolumesToVDBs(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertVDBsToVolumes(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertVolumesToPolys(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertMetasToPolys(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 convertTetsToPolys(
				GEO_ConvertParms &parms,
				bool keep_original);
    void		 doConversion(
				GEO_ConvertParms &parms,
				bool keep_original);

    void		 orientPoly(GEO_PrimPoly *poly,
				    UT_BitArray &process,
				    UT_BitArray &reverse);

    GEO_Primitive	*surfCube(int xdiv, int ydiv, int zdiv,
				  int orderx, int ordery, int orderz,
				  float xmin, float xmax,
				  float ymin, float ymax,
				  float zmin, float zmax,
				  GEO_SurfaceType type, int kind,
				  bool consolidate);

    // Checks if a single primitive is dirty (see getDirtyData).
    bool		 isDirtySinglePrimitive(GA_Primitive *prim,
    						bool checkRepPoints,
						float tol);

    void		 lodConvert(GA_PrimitiveGroup *grp, 
				UT_BoundingBox &box, int cluster, float lod,
                                bool makepolysoup = false);
    void		 divisionConvert(GA_PrimitiveGroup *grp, int adjust,
				UT_BoundingBox &box, int divx,int divy,int divz,
				const UT_Array<GA_PrimitiveGroup *> &groups,
                                bool makepolysoup = false);
    int			 adjustStepBox(float &min, float &max, float step);
    void		 stepSizeConvert(GA_PrimitiveGroup *grp, int adjust, 
				UT_BoundingBox &box, float sx,float sy,float sz,
				const UT_Array<GA_PrimitiveGroup *> &groups,
                                bool makepolysoup = false);

    /// makes a single primitive planar (see makePrimsPlanar)
    bool		 primToPlane(GEO_Primitive *pprim,
        GA_PointGroup &fixedPoints);

    /// Prohibited to avoid accidentally passing by value.
    /// Use constructor that takes a const GU_Detail * if you
    /// really need to construct a copied detail.
    GU_Detail(const GU_Detail &src);

    friend class GU_PrimitiveFactory;	// For intrinsic registration

    void		 construct()
			 {
			     myBoundingCache = NULL;
			     myValueLookupCache = NULL;
			 }

    /// The sole cook selection associated with this detail.
    GU_SelectionHandle	 myCookSelection;

    class BoundingCache;	// Forward declare
    BoundingCache	*myBoundingCache;

    mutable gu_ValueLookupCache	*myValueLookupCache;
};

#endif
