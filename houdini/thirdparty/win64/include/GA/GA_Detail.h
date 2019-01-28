/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Detail.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_Detail__
#define __GA_Detail__

#include "GA_API.h"
#include "GA_ATITopology.h"
#include "GA_AttributeRef.h"
#include "GA_AttributeSet.h"
#include "GA_Defaults.h"
#include "GA_EdgeGroup.h"
#include "GA_EdgeGroupTable.h"
#include "GA_ElementGroup.h"
#include "GA_ElementGroupOrder.h"
#include "GA_ElementGroupTable.h"
#include "GA_Handle.h"
#include "GA_IndexMap.h"
#include "GA_IntrinsicManager.h"
#include "GA_OffsetList.h"
#include "GA_PrimitiveFactory.h"
#include "GA_PrimitiveList.h"
#include "GA_PrimitiveTypeMask.h"
#include "GA_Range.h"
#include "GA_Topology.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>

class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_Options;
class UT_String;
class UT_StringArray;
class UT_StringHolder;
class UT_StringRef;
class UT_WorkBuffer;
template <typename T> class UT_Array;

class GA_ATINumeric;
class GA_AttribLoadDataH9;
class GA_AttribSaveDataH9;
class GA_Attribute;
class GA_AttributeDict;
class GA_AttributeFilter;
class GA_AttributeInstanceMatrix;
class GA_AttributeOptions;
class GA_AttributeType;
class GA_Group;
class GA_GroupTable;
class GA_IntrinsicEval;
class GA_IO;
class GA_LoadMap;
class GA_LoadOptions;
class GA_MergeMap;
class GA_MergeOptions;
class GA_PolyCounts;
class GA_Primitive;
class GA_PrimitiveTypeId;
class GA_RangeMemberQuery;
class GA_SaveOptions;
class GA_Stat;
class ga_TailInitializeTable;

namespace GA_FileFormatH9 { class PrimTypeXlate; }

/// @brief Container class for all geometry.
///
/// All Houdini geometry is stored in a GA_Detail class.  The detail stores
/// lists of attributes (GA_Attribute) which are maintained in GA_AttributeSet
/// collections.  The attribute set contains 4 separate classes of attribute
/// based on the GA_AttributeOwner enum.
class GA_API GA_Detail
{
public:
    /// @param Pstorage determines the precision of the @b P attribute.
    /// @param isprimary determines the default value of the @b P attribute.
    /// true yields (0,0,0) as the default, and false yields (0,0,1),
    /// for use by GD_Detail, where the z component is actually the
    /// point weight for NURBS/Bezier trim/profile curves.
    /// P will have typeinfo "hpoint" if secondary, and "point" if primary.
    /// @param full_topology indicates whether the detail should maintain
    /// additional attributes which make it easy to lookup
    ///  - Primitives using a shared point
    ///  - The primitive containing a unique point (vertex)
    ///  - The list of vertices referencing a shared point
    /// @see typedef::GA_Storage.
    GA_Detail(GA_PrimitiveFactory &factory, GA_Storage Pstorage=GA_STORE_REAL32,
	       bool isprimary=true,
	       bool full_topology=true);
     /// Destructor
    virtual ~GA_Detail();

    exint		 getUniqueId() const	{ return myUniqueId; }

    /// Re-initialize:
    /// - Clears all attributes
    /// - Clears all geometry elements (points, primitives, vertices, etc.)
    /// - Clears all groups
    /// NOTE: Does *NOT* clear caches like the primitive stash or selections,
    ///       though it does clear the attribute instance matrix.  It has
    ///       to keep the stash, because it's used by stashAll().
    ///       Use clearAndDestroy() to clear everything.
    void		clear()		{ clearData(false); }

    /// Returns true if the detail is empty.  This is not just
    /// having zero points/primitives, but also only having trivial
    /// attributes.
    bool		isEmpty() const;

    /// These two functions should be used instead of the more
    /// destructive clearAndDestroy() when it it likely the
    /// geometry it relatively stable between operations
    /// @{
    void stashAll()
    {
        clearCaches();

        getPrimitiveList().stashAll();

        // We have to clear the primitive index map before calling the virtual
        // clearAndDestroy() to prevent primitive iterators from ending up in
        // a bad state with expected entries missing from the primitive list.
        getIndexMap(GA_ATTRIB_PRIMITIVE).clear(/*clear_capacity*/ true);

        // Increment the meta-cache count, since it did so before, in case
        // anything is relying on it being incremented here.
        incrementMetaCacheCount();

        // remove the rest
        clear();
    }
    void destroyStashed()
    {
        getPrimitiveList().destroyStashed();
    }
    ///@}

    // -------------- P attribute ----------------------------
    /// Convenience method to access the @b P attribute
    GA_Attribute	*getP()		{ return myP; }
    const GA_Attribute	*getP()	const	{ return myP; }

    /// The ptoff passed is the point offset. @see vertexPoint()
    UT_Vector2		 getPos2(GA_Offset ptoff) const
    {
        UT_Vector3 v3 = myHandlePV3.get(ptoff);
        return UT_Vector2(v3.x(), v3.y());
    }
    /// The ptoff passed is the point offset. @see vertexPoint()
    SYS_FORCE_INLINE UT_Vector3
    getPos3(GA_Offset ptoff) const
    {
	return myHandlePV3.get(ptoff);
    }
    /// Get an array of all positions from the given point range.
    /// The template parameter T is must be one of: fpreal, fpreal32, or
    /// fpreal64.
    template <typename T>
    bool		 getPos3AsArray(
			    const GA_Range &ptrange,
			    UT_Array< UT_Vector3T<T> > &positions) const
    {
	return getAttributeAsArray(getP(), ptrange, positions);
    }
    /// Get an array of all positions from the given point range.
    /// The template parameter T is must be one of: fpreal, fpreal32, or
    /// fpreal64.
    template <typename T>
    bool		 setPos3FromArray(
			    const GA_Range &ptrange,
			    const UT_Array< UT_Vector3T<T> > &positions)
    {
	return setAttributeFromArray(getP(), ptrange, positions);
    }

    /// Set @b P from a UT_Vector2
    void		 setPos2(GA_Offset ptoff, const UT_Vector2 &P)
    {
        UT_Vector3 v3 = UT_Vector3(P.x(), P.y(), myHandlePV3.get(ptoff).z());
        myHandlePV3.set(ptoff, v3);
    }
    /// Set @b P from a UT_Vector3
    SYS_FORCE_INLINE
    void		 setPos3(GA_Offset ptoff, const UT_Vector3 &P)
    {
	myHandlePV3.set(ptoff, P);
    }
    /// Set @b P given the x, y, z components.
    SYS_FORCE_INLINE
    void		setPos3(GA_Offset ptoff, fpreal x, fpreal y, fpreal z)
    {
	myHandlePV3.set(ptoff, UT_Vector3(x,y,z));
    }

    /// Apply a translation to the specified point index.  @see vertexPoint()
    /// Effectively setPos3(offset, getPos3(offset) + delta)
    SYS_FORCE_INLINE
    void translatePoint(GA_Offset ptoff, const UT_Vector3 &delta)
    {
        myHandlePV3.add(ptoff, delta);
    }

    //
    // Functions for handling Pw: specific to rational NURBS and Bezier
    // curves and surfaces!  DO NOT USE unless you actually need to adjust the
    // point weights!
    //

    /// Get the homogeneous coordinate for a point
    fpreal		getPw(GA_Offset ptoff) const;
    /// Set the homogeneous coordinate for a point
    void		setPw(GA_Offset ptoff, fpreal w);
    /// NOTE: This will usually return nullptr, because very
    ///       few things actually set the homogenous coordinate
    ///       to anything other than 1.0, in which case, the
    ///       attribute won't be created.
    /// @{
    const GA_Attribute *getPwAttribute() const
    {
        return myPw;
    }
    GA_Attribute *getPwAttribute()
    {
        return myPw;
    }
    /// @}

    /// The ptoff passed is the point offset. @see vertexPoint()
    UT_Vector4		 getPos4(GA_Offset ptoff) const
    {
	return UT_Vector4(getPos3(ptoff), getPw(ptoff));
    }

    /// Set @b P from a UT_Vector4
    void		 setPos4(GA_Offset ptoff, const UT_Vector4 &P)
    {
        setPos3(ptoff, UT_Vector3(P));
        setPw(ptoff, P.w());
    }
    /// Set @b P given the x, y, z, w components.
    void		 setPos4(GA_Offset ptoff, fpreal x, fpreal y,
					fpreal z, fpreal w)
    {
        setPos3(ptoff, UT_Vector3(x, y, z));
        setPw(ptoff, w);
    }

    //
    // -------------- Element Interface ----------------------
    //

    /// Append a new point, returning its new data offset
    SYS_FORCE_INLINE
    GA_Offset		 appendPoint()
			    { return appendPointBlock(1); }
    /// Append new points, returning the first offset of the contiguous block
    SYS_FORCE_INLINE
    GA_Offset		 appendPointBlock(GA_Size npoints)
			    { return myPointMap.addElementBlock(npoints); }
    /// Return the number of points
    SYS_FORCE_INLINE
    GA_Size		 getNumPoints() const
			    { return myPointMap.indexSize(); }
    /// This will be an exclusive, possibly loose, upper bound when iterating
    /// over point offsets.
    /// Every point offset in this detail will be strictly less than this.
    /// @note (getNumPoints() <= getNumPointOffsets()) is always true.
    SYS_FORCE_INLINE
    GA_Offset		 getNumPointOffsets() const
			    { return myPointMap.offsetSize(); }
    /// Given a point's index (in append order), return its data offset
    SYS_FORCE_INLINE
    GA_Offset		 pointOffset(GA_Index index) const
			    { return myPointMap.offsetFromIndex(index); }
    /// Given a point's data offset, return its index
    SYS_FORCE_INLINE
    GA_Index		 pointIndex(GA_Offset offset) const
			    { return myPointMap.indexFromOffset(offset); }
    /// Reorder a point. The index of the point at the given data offset
    /// will be changed to the new order (provided that the ordered position is
    /// in a valid range).
    /// @return The new index of the point, or -1 if there was an error
    GA_Index		 reorderPoint(GA_Offset ptoff, GA_Index new_order);

    /// Append a primitive by GA_PrimitiveTypeId
    GA_Primitive	*appendPrimitive(const GA_PrimitiveTypeId &type);
    /// Append a primitive by type name string
    GA_Primitive	*appendPrimitive(const UT_StringRef &type);
    /// Append a contiguous block of primitives by GA_PrimitiveTypeId
    GA_Offset		 appendPrimitiveBlock(const GA_PrimitiveTypeId &type,
					GA_Size nprimitives);

    /// Append a contiguous block of primitives and a contiguous
    /// block of vertices, with each primitive having nvertices_each vertices,
    /// though the vertices will be unwired, so the caller is responsible
    /// for wiring the vertices to points.
    /// Make absolutely sure you know what you're doing if you call this!
    /// It's used by GEO_PrimPoly::buildBlock() and GEO_PrimTetrahedron::
    /// buildBlock().
    /// The return value is the start offset of the primitive block,
    /// and vertex_block_start is filled with the start offset of the
    /// vertex block.
    GA_Offset            appendPrimitivesAndVertices(
                                const GA_PrimitiveTypeId &type,
                                GA_Size nprimitives,
                                GA_Size nvertices_each,
                                GA_Offset &vertex_block_start,
                                bool closed_flag = false);

    /// Append a contiguous block of primitives and a contiguous
    /// block of vertices, with vertex_counts being a run-length encoded
    /// array indicating the number of vertices in each primitive,
    /// though the vertices will be unwired, so the caller is responsible
    /// for wiring the vertices to points.
    /// Make absolutely sure you know what you're doing if you call this!
    /// It's used by GEO_PrimPoly::buildBlock() and GEO_PrimTetrahedron::
    /// buildBlock().
    /// The return value is the start offset of the primitive block,
    /// and vertex_block_start is filled with the start offset of the
    /// vertex block.
    GA_Offset            appendPrimitivesAndVertices(
                                const GA_PrimitiveTypeId &type,
                                const GA_PolyCounts &vertex_counts,
                                GA_Offset &vertex_block_start,
                                bool closed_flag = false);

    /// Return the number of primitives
    SYS_FORCE_INLINE
    GA_Size		 getNumPrimitives() const
			    { return myPrimitiveMap.indexSize(); }
    /// This will be an exclusive, possibly loose, upper bound when iterating
    /// over primitive offsets.
    /// Every primitive offset in this detail will be strictly less than this.
    /// @note (getNumPrimitives() <= getNumPrimitiveOffsets()) is always true.
    SYS_FORCE_INLINE
    GA_Offset		 getNumPrimitiveOffsets() const
			    { return myPrimitiveMap.offsetSize(); }
    /// Given a primitive's index (in append order), return its data offset
    SYS_FORCE_INLINE
    GA_Offset		 primitiveOffset(GA_Index index) const
			    { return myPrimitiveMap.offsetFromIndex(index); }
    /// Given a primitive's data offset, return its index
    SYS_FORCE_INLINE
    GA_Index		 primitiveIndex(GA_Offset offset) const
			    { return myPrimitiveMap.indexFromOffset(offset); }

    /// Get the primitive by offset
    /// @{
    SYS_FORCE_INLINE
    GA_Primitive	*getPrimitive(GA_Offset prim_off)
			    { return myPrimitiveList.get(prim_off); }
    SYS_FORCE_INLINE
    const GA_Primitive	*getPrimitive(GA_Offset prim_off) const
			    { return myPrimitiveList.get(prim_off); }
    /// @}

    /// Get the primitive by index
    /// @{
    GA_Primitive	*getPrimitiveByIndex(GA_Index prim_idx)
			    { return myPrimitiveList.get(primitiveOffset(prim_idx)); }
    const GA_Primitive	*getPrimitiveByIndex(GA_Index prim_idx) const
			    { return myPrimitiveList.get(primitiveOffset(prim_idx)); }
    /// @}

    /// Returns a shallow copy of the vertex list of the primitive at the
    /// specified primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_OffsetListRef getPrimitiveVertexList(GA_Offset primoff) const;

    /// Returns the number of vertices in the primitive at the specified
    /// primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_Size getPrimitiveVertexCount(GA_Offset primoff) const;

    /// Returns the vertex offset in the primitive at the specified
    /// primitive offset in vertex list position i.
    /// If you're accessing all vertex offsets, it'll be faster to
    /// call getVertexList() once and either read from that or
    /// call forEach(fucntor) on it.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_Offset getPrimitiveVertexOffset(GA_Offset primoff, GA_Size i) const;

    /// Returns the type ID of the primitive at the specified
    /// primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    int getPrimitiveTypeId(GA_Offset primoff) const;

    /// Returns the "closed" flag for polygon, NURBS curve, or Bezier curve
    /// primitives, or false for other primitive types.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    bool getPrimitiveClosedFlag(GA_Offset primoff) const;

    /// Sets the "closed" flag for polygon primitives.
    /// For NURBS curve or Bezier curve primitives, use GEO_Face::close()
    /// or GEO_Face::open().
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    void setPrimitiveClosedFlag(GA_Offset primoff, bool closed);


    /// Reorder a primitive. The index of the primitive at the given data
    /// offset will be changed to the new order (provided that the ordered
    /// position is in a valid range).
    /// @return The new index of the primitive, or -1 if there was an error
    GA_Index		 reorderPrimitive(GA_Offset offset, GA_Index new_order);

    /// Swap the order of the two specified primitive data offsets.
    bool		 swapPrimitiveOrder(GA_Offset offset1,
					    GA_Offset offset2);

    /// Append a vertex (for the entire detail)
    SYS_FORCE_INLINE
    GA_Offset		 appendVertex() { return appendVertexBlock(1); }
    /// Append new vertices, returning the first offset of the contiguous block
    SYS_FORCE_INLINE
    GA_Offset		 appendVertexBlock(GA_Size nvertices)
			    { return myVertexMap.addElementBlock(nvertices); }
    /// Return the number verticies in the entire detail
    SYS_FORCE_INLINE
    GA_Size		 getNumVertices() const
			    { return myVertexMap.indexSize(); }
    /// This will be an exclusive, possibly loose, upper bound when iterating
    /// over vertex offsets.
    /// Every vertex offset in this detail will be strictly less than this.
    /// @note (getNumVertices() <= getNumVertexOffsets()) is always true.
    SYS_FORCE_INLINE
    GA_Offset		 getNumVertexOffsets() const
			    { return myVertexMap.offsetSize(); }
    /// Given a vertex's index (in append order), return its data offset
    SYS_FORCE_INLINE
    GA_Offset		 vertexOffset(GA_Index index) const
			    { return myVertexMap.offsetFromIndex(index); }
    /// Given a vertex's data offset, return its index
    SYS_FORCE_INLINE
    GA_Index		 vertexIndex(GA_Offset offset) const
			    { return myVertexMap.indexFromOffset(offset); }

    /// @private @see GA_WorkVertexBuffer
    GA_Offset		 appendTemporaryPoint();
    /// @private @see GA_WorkVertexBuffer
    GA_Offset		 appendTemporaryVertex();

    /// Given a vertex, return the point it references
    SYS_FORCE_INLINE
    GA_Offset vertexPoint(GA_Offset vertex) const
    {
        UT_ASSERT_P(!myVertexMap.isOffsetVacant(vertex));
        return myTopology.getPointRef()->getLink(vertex);
    }
    /// Given a vertex, return primitive it references
    /// Returns -1 if no topology attributes exist.
    SYS_FORCE_INLINE
    GA_Offset vertexPrimitive(GA_Offset vertex) const
    {
        UT_ASSERT_P(!myVertexMap.isOffsetVacant(vertex));
        const GA_ATITopology *pref = myTopology.getPrimitiveRef();
        UT_ASSERT_P(pref);
        return pref->getLink(vertex);
    }

    /// Given a point, returns *a* vertex that it references.
    /// More than one vertex may refer to a single point, and it is 
    /// quite possible that no vertices refer to the point.
    /// GA_INVALID_OFFSET is returned if no vertices match.
    /// See getVerticesReferencingPoint() for methods to get all the points.
    SYS_FORCE_INLINE
    GA_Offset pointVertex(GA_Offset point) const
    {
        UT_ASSERT_P(!myPointMap.isOffsetVacant(point));
        const GA_ATITopology *ref = myTopology.getVertexRef();
        UT_ASSERT_P(ref);
        return ref->getLink(point);
    }

    /// Given a vertex, returns the next one that shares the same
    /// point as this vertex.  GA_INVALID_OFFSET if no further
    /// vertices match
    SYS_FORCE_INLINE
    GA_Offset vertexToNextVertex(GA_Offset vtx) const
    {
        UT_ASSERT_P(!myVertexMap.isOffsetVacant(vtx));
        const GA_ATITopology *ref = myTopology.getVertexNextRef();
        UT_ASSERT_P(ref);
        return ref->getLink(vtx);
    }

    /// Given a vertex, returns the previous one that shares the same
    /// point as this vertex.  GA_INVALID_OFFSET if no further
    /// vertices match
    SYS_FORCE_INLINE
    GA_Offset vertexToPrevVertex(GA_Offset vtx) const
    {
        UT_ASSERT_P(!myVertexMap.isOffsetVacant(vtx));
        const GA_ATITopology *ref = myTopology.getVertexPrevRef();
        UT_ASSERT_P(ref);
        return ref->getLink(vtx);
    }

    /// Given a point and primitive, find the corresponding vertex.  Note that
    /// this is O(N) on the number of vertices which share the point.  If the
    /// primitive offset is -1, then the "first" vertex which references the
    /// point will be returned.  The order of the "first" vertex is arbitrary,
    /// though it will likely be the same for each call.
    ///
    /// Returns -1 if no topology attributes exist.
    GA_Offset		getVertexReferencingPoint(GA_Offset ptoff,
			    GA_Offset primoff = GA_INVALID_OFFSET) const;

    /// Given a point offset, return a list of all vertices which reference it.
    /// NOTE: The vertex offsets might be in ANY order, and may even be
    ///       different from run to run, so DO NOT rely on the order!
    GA_Size		 getVerticesReferencingPoint(
				GA_OffsetArray &vertices,
				GA_Offset point_offset) const;
    /// Given a point offset, return a list of all primitives which reference it.
    /// The primitive offsets will be in ascending order and have no duplicates,
    /// so you can do:
    ///     detail.getPrimitivesReferencingPoint(prims0, p0off);
    ///     detail.getPrimitivesReferencingPoint(prims1, p1off);
    ///     prims0.sortedIntersection(prims1);
    /// to quickly get a list of primitives that contain both p0off and p1off,
    /// (not necessarily as an edge, but this can be used as a first pass check.)
    GA_Size		 getPrimitivesReferencingPoint(
				GA_OffsetArray &primitives,
				GA_Offset point_offset) const;

    /// Returns false if any point is shared, ie, more than one vertex
    /// points to the same point.  This being true means each point
    /// has at most one vertex and belongs to at most one primitive.
    /// Note that points can belong to zero vertices and zero primitives 
    /// still.  Requires topology.
    bool		 hasNoSharedPoints() const;

    /// When destroying points, there are three different modes when dealing
    /// with primitives that reference the points.  These modes cause different
    /// behaviours when notifying primitives about point deletion.  The
    /// behaviour is determined by the result of @c
    /// primitive->dereferencePoint() or @c primitive->dereferencePoints().
    /// - GA_LEAVE_PRIMITIVES @n
    ///   Primitive is destroyed if it returns GA_DEREFERENCE_DESTROY.
    ///	  Point will @b not be destroyed if prim returns GA_DEREFERENCE_FAIL.
    /// - GA_DESTROY_DEGENERATE @n
    ///   Primitive is destroyed if it returns GA_DEREFERENCE_DEGENERATE or
    ///   GA_DEREFERENCE_DESTROY.  Point is not destroyed if prim returns
    ///   GA_DEREFERENCE_FAIL.
    /// - GA_DESTROY_DEGENERATE_INCOMPATIBLE @n
    ///   Primitive is destroyed if it returns GA_DEREFERENCE_DEGENERATE,
    ///   GA_DEREFERENCE_DESTROY, or GA_DEREFERENCE_FAIL.  The point will
    ///   always be destroyed.
    enum GA_DestroyPointMode
    {
	GA_LEAVE_PRIMITIVES,
	GA_DESTROY_DEGENERATE,
	GA_DESTROY_DEGENERATE_INCOMPATIBLE,
    };

    /// @{
    /// Destroy the given point.  This may leave dangling vertex references if
    /// there is no full topology.
    ///
    /// If the @c guarantee_no_vertex_references flag is true, the operation
    /// will be more efficient, but you must truly guarantee that there are no
    /// vertices referring to the point.
    bool		 destroyPointOffset(GA_Offset ptoff,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES,
				bool guarantee_no_vertex_references=false);
    bool		 destroyPointIndex(GA_Index index,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES,
				bool guarantee_no_vertex_references=false);
    /// @}
    /// Destroy the given vertex
    bool		 destroyVertexOffset(GA_Offset offset);
    /// Destroy the given primitive.  Optionally destroy any referenced points
    /// that will no longer be in use after this operation.  Note that this
    /// option requires full topology.
    virtual bool	 destroyPrimitiveOffset(GA_Offset offset,
					       bool and_points = false);
    bool		 destroyPrimitiveIndex(GA_Index index,
					      bool and_points = false);
    bool		 destroyPrimitive(GA_Primitive &prim,
					 bool and_points = false);

    /// Destroy the given primitives.  Optionally destroy any referenced points
    /// that will no longer be in use after this operation.  Note that this
    /// option requires full topology.
    virtual GA_Size	 destroyPrimitiveOffsets(const GA_Range &it,
						bool and_points = false);
    GA_Size		 destroyPrimitives(const GA_Range &it,
					  bool and_points = false)
			    { return destroyPrimitiveOffsets(it, and_points); }
    GA_Size		 destroyDegeneratePrimitives(const GA_Range &it,
				bool and_points = false);
    GA_Size		 destroyDegeneratePrimitives(const GA_PrimitiveGroup *prims=0, bool and_points = false);
					    

    /// Destroy unused points. If ptgrp is given, then only within the group
    GA_Size		 destroyUnusedPoints(const GA_PointGroup *ptgrp = 0)
			 {
			     return destroyUnusedPoints(getPointRange(ptgrp));
			 }
    /// @{
    /// Destroy unused points in given point range
    GA_Size		 destroyUnusedPoints(const GA_Range &point_range);
    GA_Size		 destroyPointOffsets(const GA_Range &range,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES,
				bool guarantee_no_vertex_references=false)
			 {
			     return destroyElements(range, mode,
				     guarantee_no_vertex_references);
			 }
    GA_Size		 destroyPoints(const GA_Range &range,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES,
				bool guarantee_no_vertex_references=false)
			 {
			     return destroyPointOffsets(range, mode,
				     guarantee_no_vertex_references);
			 }
    /// @}


    GA_Size		 destroyVertexOffsets(const GA_Range &range)
			    { return destroyElements(range); }
    GA_Size		 destroyVertices(const GA_Range &range)
			    { return destroyVertexOffsets(range); }

    /// Returns whether a point is referenced by any vertices
    /// This may be a slow operation if there is no topology allocated
    bool		 isPointUsed(GA_Offset point) const;
    /// Returns if a point is referenced.  Fails catastrophically
    /// if you do not have full topology.
    bool		 isPointUsedFast(GA_Offset point) const
    {
	return GAisValid(pointVertex(point));
    }

    /// Returns true if there are any unused points.
    /// If the offset list is given, it is set to all of the found
    /// unused points.
    bool 		 findUnusedPoints(GA_OffsetList *unused=nullptr) const;

    // -------------- Index Maps -----------------------------
    const GA_IndexMap	&getIndexMap(GA_AttributeOwner owner) const;
    GA_IndexMap		&getIndexMap(GA_AttributeOwner owner)
			{
			    return const_cast<GA_IndexMap &>(
				    static_cast<const GA_Detail *>(this)
					->getIndexMap(owner));
			}
    const GA_IndexMap	&getPointMap() const	    { return myPointMap; }
    const GA_IndexMap	&getVertexMap() const	    { return myVertexMap; }
    const GA_IndexMap	&getPrimitiveMap() const    { return myPrimitiveMap; }
    const GA_IndexMap	&getGlobalMap() const	    { return myGlobalMap; }

    /// This is a helpful class for getting the ranges of elements
    /// created after such an OffsetMarker is declared.  For example,
    /// 
    /// GA_Detail::OffsetMarker marker(*gdp);
    /// ... // Some code that creates points, vertices, or primitives
    /// for (GA_Iterator it(marker.pointRange()); !it.atEnd(); ++it)
    ///     ... // Do something to each created point
    /// for (GA_Iterator it(marker.vertexRange()); !it.atEnd(); ++it)
    ///     ... // Do something to each created vertex
    /// for (GA_Iterator it(marker.primitiveRange()); !it.atEnd(); ++it)
    ///     ... // Do something to each created primitive
    /// 
    /// NOTE: The code doing the creating can't delete elements from
    ///       the detail before adding any new elements.
    class OffsetMarker
    {
    public:
        OffsetMarker(const GA_Detail &detail)
            : myPointMarker(detail.getPointMap())
            , myVertexMarker(detail.getVertexMap())
            , myPrimitiveMarker(detail.getPrimitiveMap())
        {}
        GA_Range pointRange() const
        { return myPointMarker.getRange(); }
        GA_Range vertexRange() const
        { return myVertexMarker.getRange(); }
        GA_Range primitiveRange() const
        { return myPrimitiveMarker.getRange(); }
        GA_Offset pointBegin() const
        { return myPointMarker.getBegin(); }
        GA_Offset vertexBegin() const
        { return myVertexMarker.getBegin(); }
        GA_Offset primitiveBegin() const
        { return myPrimitiveMarker.getBegin(); }
        GA_Offset pointEnd() const
        { return myPointMarker.getEnd(); }
        GA_Offset vertexEnd() const
        { return myVertexMarker.getEnd(); }
        GA_Offset primitiveEnd() const
        { return myPrimitiveMarker.getEnd(); }
    private:
        const GA_IndexMap::Marker myPointMarker;
        const GA_IndexMap::Marker myVertexMarker;
        const GA_IndexMap::Marker myPrimitiveMarker;
    };

    const GA_PrimitiveList &getPrimitiveList() const
						{ return myPrimitiveList; }
    GA_PrimitiveList	&getPrimitiveList()
						{ return myPrimitiveList; }
    GA_AttributeSet	&getAttributes()	{ return myAttributes; }
    const GA_AttributeSet &getAttributes() const{ return myAttributes; }
    GA_Topology		&getTopology()		{ return myTopology; }
    const GA_Topology	&getTopology() const	{ return myTopology; }
    bool		 checkValidTopology() const;
    void		 createTopologyAttributes();
    void		 clearTopologyAttributes();

    /// This bumps all data IDs in this detail, including
    /// point, vertex, primitive, and detail attribute data IDs,
    /// the primitive list data ID, and edge group data IDs.
    void                 bumpAllDataIds();

    /// Call this to bump the appropriate topology data IDs when
    /// vertices have been rewired to different points.
    ///
    /// If vertices have been added or removed, or if they've been
    /// "stolen" by a different primitive,
    /// just call bumpDataIdsForAddOrRemove(____,true,____) instead.
    /// If points have been added or removed, but vertices have
    /// only been rewired, call bumpDataIdsForAddOrRemove(true,false,false)
    /// in addition to this.
    void                 bumpDataIdsForRewire();

    /// Call this to bump the appropriate data IDs when points, vertices,
    /// or primitives have been added or removed.  If you're not 100%
    /// sure whether an element type has been added or removed,
    /// it's better to err on the side of safety and pass true for it.
    void                 bumpDataIdsForAddOrRemove(
                            bool added_or_removed_points,
                            bool added_or_removed_vertices,
                            bool added_or_removed_primitives);

    /// @{
    /// Get access to the attributes for one element type
    GA_AttributeOwner	 getAttributeOwner(const GA_AttributeDict &dict) const;
    const GA_AttributeDict &getAttributeDict(GA_AttributeOwner owner) const
				{ return myAttributes.getDict(owner); }
    /// @}

    /// Defragment index maps
    /// NOTE: To make sure that it actually defragments when there is even just
    ///       one hole, you must pass a UT_Options with a "removeholes"
    ///       bool option set to true.
    /// NOTE: If any defragmenting occurs, this bumps data IDs for everything
    ///       that is modified.
    bool		 defragment(GA_AttributeOwner owner,
				const UT_Options *options = nullptr);
    /// Defragment everything.
    /// NOTE: To make sure that it actually defragments when there is even just
    ///       one hole, you must pass a UT_Options with a "removeholes"
    ///       bool option set to true.
    /// NOTE: If any defragmenting occurs, this bumps data IDs for everything
    ///       that is modified.
    bool		 defragment(const UT_Options *options = nullptr);

    /// Optimized layout of vertex index map.  This sorts the vertex index map
    /// by the primitive order use.  The map will be defragmented after this
    /// function is called.
    void		 sortVertexMapByPrimitiveUse();

    // -------------- Attributes ----------------------------
    
    /// @{
    /// createAttribute is very general purpose method, requiring
    /// some knowledge about ATIs, etc.  Unless you are creating
    /// your own user-defined attributes, it would likely be better
    /// to use something at the GEO_Detail level, like addFloatTuple.
    GA_Attribute        *createAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const UT_Options *create_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype)
			 {
			    return myAttributes.createAttribute(
					owner, scope, name, create_args,
					attribute_options, attribtype);
			 }
    GA_Attribute        *createAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				const UT_Options *create_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype)
			 {
			    return myAttributes.createAttribute(
					owner, name, create_args,
					attribute_options, attribtype);
			 }
    /// @}

    /// This will create detached attributes.  They will have the
    /// right number of elements at the moment of creation, but
    /// will not update with changes to the detail.  The caller
    /// is responsible for invoking delete on them as well.
    /// These attributes will have no name.
    /// @{
    SYS_FORCE_INLINE
    GA_ATINumeric *createDetachedTupleAttribute(
        GA_AttributeOwner owner,
        GA_Storage storage,
        int tuple_size,
        const GA_Defaults &defaults=GA_Defaults(0.0f),
        const GA_AttributeOptions *attribute_options = nullptr) const
    {
        return myAttributes.createDetachedTupleAttribute(
                    owner, storage, tuple_size,
                    defaults, attribute_options);
    }
    SYS_FORCE_INLINE
    GA_Attribute *createDetachedAttribute(
        GA_AttributeOwner owner,
        const UT_StringRef &attribtype,
        const UT_Options *create_args = nullptr,
        const GA_AttributeOptions *attribute_options = nullptr) const
    {
        return myAttributes.createDetachedAttribute(
                    owner, attribtype, create_args,
                    attribute_options);
    }
    SYS_FORCE_INLINE
    GA_Attribute *createDetachedAttribute(
        GA_AttributeOwner owner,
        const GA_AttributeType &attribtype,
        const UT_Options *create_args = nullptr,
        const GA_AttributeOptions *attribute_options = nullptr) const
    {
        return myAttributes.createDetachedAttribute(
                    owner, attribtype, create_args,
                    attribute_options);
    }
    /// @}

    SYS_FORCE_INLINE
    bool		 destroyAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name)
			 {
			     return myAttributes.destroyAttribute(owner,
								   scope, name);
			 }
    SYS_FORCE_INLINE
    bool		 destroyAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name)
			 {
			     return myAttributes.destroyAttribute(owner, name);
			 }
    SYS_FORCE_INLINE
    bool		 renameAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &fromName,
                                const UT_StringHolder &toName)
			 {
			     return myAttributes.renameAttribute(owner, scope,
							fromName, toName);
			 }

    // Convenience attribute methods
    GA_Attribute        *createTupleAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				GA_Storage storage,
				int tuple_size,
				const GA_Defaults &defaults = GA_Defaults(0.0f),
				const UT_Options *create_args = nullptr,
				const GA_AttributeOptions *attribute_options = nullptr)
			 {
			    return myAttributes.createTupleAttribute(
					    owner, scope, name, storage,
					    tuple_size, defaults,
					    create_args,
					    attribute_options);
			 }
    GA_Attribute        *createTupleAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				GA_Storage storage,
				int tuple_size,
				const GA_Defaults &defaults = GA_Defaults(0.0f),
				const UT_Options *create_args = nullptr,
				const GA_AttributeOptions *attribute_options = nullptr)
			 {
			    return myAttributes.createTupleAttribute(
					    owner, name, storage,
					    tuple_size, defaults,
					    create_args,
					    attribute_options);
			 }
    GA_Attribute        *createStringAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const UT_Options *create_args=nullptr,
				const GA_AttributeOptions *attribute_options=nullptr)
			 {
			    return myAttributes.createStringAttribute(
					    owner, scope, name, 1,
					    create_args,
					    attribute_options);
			 }
    GA_Attribute        *createStringAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				const UT_Options *create_args=nullptr,
				const GA_AttributeOptions *attribute_options=nullptr)
			 {
			    return myAttributes.createStringAttribute(
					    owner, name, 1, create_args,
					    attribute_options);
			 }

    /// Find an existing attribute, returning a read-only reference.
    ///
    SYS_FORCE_INLINE
    const GA_Attribute  *findAttribute(GA_AttributeScope scope,
				const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_size) const
			 {
			     return myAttributes.findAttribute(
					    scope, name, search_order,
					    search_size);
			 }
    SYS_FORCE_INLINE
    const GA_Attribute  *findAttribute(const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_size) const
			 {
			     return myAttributes.findAttribute(
					    name, search_order, search_size);
			 }
    SYS_FORCE_INLINE
    const GA_Attribute  *findAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name) const
			     { return findAttribute(scope, name, &owner, 1); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name) const
			     { return findAttribute(name, &owner, 1); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findPointAttribute(GA_AttributeScope s,
				const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_POINT, s, name); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findPointAttribute(const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_POINT, name); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findVertexAttribute(GA_AttributeScope s,
				const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_VERTEX,s, name); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findVertexAttribute(const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_VERTEX, name); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findPrimitiveAttribute(GA_AttributeScope s,
				const UT_StringRef &name) const
			     {return findAttribute(GA_ATTRIB_PRIMITIVE,s,name);}
    SYS_FORCE_INLINE
    const GA_Attribute  *findPrimitiveAttribute(const UT_StringRef &name) const
			     {return findAttribute(GA_ATTRIB_PRIMITIVE, name);}
    SYS_FORCE_INLINE
    const GA_Attribute  *findGlobalAttribute(GA_AttributeScope s,
				const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_GLOBAL,s, name); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findGlobalAttribute(const UT_StringRef &name) const
			     { return findAttribute(GA_ATTRIB_GLOBAL, name); }

    /// Find an existing attribute, returning a read-write reference.
    ///
    SYS_FORCE_INLINE
    GA_Attribute        *findAttribute(GA_AttributeScope scope,
				const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_size)
			 {
			     return myAttributes.findAttribute(
					    scope, name, search_order,
					    search_size);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *findAttribute(const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_size)
			 {
			     return myAttributes.findAttribute(
					    name, search_order, search_size);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *findAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name)
			     { return findAttribute(scope, name, &owner, 1); }
    SYS_FORCE_INLINE
    GA_Attribute        *findAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name)
			     { return findAttribute(name, &owner, 1); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPointAttribute(GA_AttributeScope s,
				const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_POINT, s, name); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPointAttribute(const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_POINT, name); }
    SYS_FORCE_INLINE
    GA_Attribute        *findVertexAttribute(GA_AttributeScope s,
				const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_VERTEX,s, name); }
    SYS_FORCE_INLINE
    GA_Attribute        *findVertexAttribute(const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_VERTEX, name); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPrimitiveAttribute(GA_AttributeScope s,
				const UT_StringRef &name)
			     {return findAttribute(GA_ATTRIB_PRIMITIVE,s,name);}
    SYS_FORCE_INLINE
    GA_Attribute        *findPrimitiveAttribute(const UT_StringRef &name)
			     {return findAttribute(GA_ATTRIB_PRIMITIVE, name);}
    SYS_FORCE_INLINE
    GA_Attribute        *findGlobalAttribute(GA_AttributeScope s,
				const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_GLOBAL,s, name); }
    SYS_FORCE_INLINE
    GA_Attribute        *findGlobalAttribute(const UT_StringRef &name)
			     { return findAttribute(GA_ATTRIB_GLOBAL, name); }

    /// Get/set all the point attribute data from/to a contiguous array
    // @{

    /// Valid types for T are:
    ///	  int32, int64, fpreal32, fpreal64,
    ///   UT_Vector2T<S>, UT_Vector3T<S>, UT_Vector4T<S>,
    ///   UT_QuaternionT<S>, UT_Matrix3T<S>, UT_Matrix4T<S>
    /// where S is one of: fpreal, fpreal32, fpreal64,
    /// For the UT_Vector classes, S can also be one of: int, int32, int64
    template <typename T>
    bool		 getAttributeAsArray(
			    const GA_Attribute *atr,
			    const GA_Range &range,
			    UT_Array<T> &result) const;

    /// Valid types for T are:
    ///	  int32, int64, fpreal32, fpreal64,
    ///   UT_Vector2T<S>, UT_Vector3T<S>, UT_Vector4T<S>,
    ///   UT_QuaternionT<S>, UT_Matrix3T<S>, UT_Matrix4T<S>
    /// where S is one of: fpreal, fpreal32, fpreal64,
    /// For the UT_Vector classes, S can also be one of: int, int32, int64
    template <typename T>
    bool		 setAttributeFromArray(
			    GA_Attribute *atr,
			    const GA_Range &range,
			    const UT_Array<T> &src);
    // @}

    // -------------- Groups ---------------------------------
    GA_ElementGroup	*createElementGroup(GA_AttributeOwner owner,
					    const UT_StringHolder &name,
					    bool ordered=false);
    GA_ElementGroup	*createInternalElementGroup(GA_AttributeOwner owner,
						    bool ordered=false);
    GA_ElementGroup	*findElementGroup(GA_AttributeOwner owner,
				const UT_StringRef &name);
    const GA_ElementGroup *findElementGroup(GA_AttributeOwner owner,
				const UT_StringRef &name) const;

    // Only accepts GA_ATTRIB_POINT,GA_ATTRIB_VERTEX,GA_ATTRIB_PRIMITIVE
    const GA_ElementGroupTable &
                         getElementGroupTable(GA_AttributeOwner owner) const;
    SYS_FORCE_INLINE GA_ElementGroupTable &
		         getElementGroupTable(GA_AttributeOwner owner)
			 { 
			    return const_cast<GA_ElementGroupTable &>(
					static_cast<const GA_Detail *>(this)
					    ->getElementGroupTable(owner));
			 }

    // Gets any group table.
    GA_GroupTable	*getGroupTable(GA_GroupType group_type);
    const GA_GroupTable	*getGroupTable(GA_GroupType group_type) const;

    /// @{
    /// Access point, primitive, vertex, or edge group tables
    GA_PointGroupTable		&pointGroups()
					{ return myPointGroups; }
    const GA_PointGroupTable	&pointGroups() const
					{ return myPointGroups; }
    GA_PrimitiveGroupTable	&primitiveGroups()
					{ return myPrimitiveGroups; }
    const GA_PrimitiveGroupTable&primitiveGroups() const
					{ return myPrimitiveGroups; }
    GA_VertexGroupTable		&vertexGroups()
					{ return myVertexGroups; }
    const GA_VertexGroupTable	&vertexGroups() const
					{ return myVertexGroups; }
    GA_EdgeGroupTable		&edgeGroups()
					{ return myEdgeGroups; }
    const GA_EdgeGroupTable	&edgeGroups() const
					{ return myEdgeGroups; }
    /// @}

    template <GA_AttributeOwner OWNER>
    SYS_FORCE_INLINE
	typename GA_ElementGroupTableT<OWNER>::GROUP_TYPE *
			 createElementGroup(const UT_StringHolder &name,
					    bool ordered=false)
			 {
			     return GA_ElementGroupTableT<OWNER>::castGroup(
				     createElementGroup(OWNER, name, ordered));
			 }

    template <GA_AttributeOwner OWNER>
    SYS_FORCE_INLINE
	typename GA_ElementGroupTableT<OWNER>::GROUP_TYPE *
			 createInternalElementGroup(bool ordered=false)
			 {
			     return GA_ElementGroupTableT<OWNER>::castGroup(
				     createInternalElementGroup(OWNER,ordered));
			 }

    template <GA_AttributeOwner OWNER>
    SYS_FORCE_INLINE
	typename GA_ElementGroupTableT<OWNER>::GROUP_TYPE *
			 findElementGroup(const UT_StringRef &name)
			 {
			     return GA_ElementGroupTableT<OWNER>::castGroup(
						findElementGroup(OWNER, name));
			 }
    template <GA_AttributeOwner OWNER>
    SYS_FORCE_INLINE
	const typename GA_ElementGroupTableT<OWNER>::GROUP_TYPE *
			 findElementGroup(const UT_StringRef &name) const
			 {
			     return GA_ElementGroupTableT<OWNER>::castGroup(
						findElementGroup(OWNER, name));
			 }

    void		 getElementGroupList(GA_AttributeOwner owner,
			      UT_Array<const GA_ElementGroup *> &list) const;
    void		 getElementGroupList(GA_AttributeOwner owner,
			      UT_Array<GA_ElementGroup *> &list);
    SYS_FORCE_INLINE
    bool		 destroyElementGroup(GA_AttributeOwner owner,
				const UT_StringRef &name)
			{ return getElementGroupTable(owner).destroy(name); }
    SYS_FORCE_INLINE
    bool		 destroyElementGroup(GA_AttributeOwner owner,
				const char *name)
			{ return getElementGroupTable(owner).destroy(name); }
    bool		 destroyElementGroup(GA_ElementGroup *group);
    bool		 destroyGroup(GA_Group *group);

    // Convenience group methods
    SYS_FORCE_INLINE
    const GA_PointGroup *findPointGroup(const UT_StringRef &name) const
    { return findElementGroup<GA_ATTRIB_POINT>(name); }
    SYS_FORCE_INLINE
    const GA_VertexGroup *findVertexGroup(const UT_StringRef &name) const
    { return findElementGroup<GA_ATTRIB_VERTEX>(name); }
    SYS_FORCE_INLINE
    const GA_PrimitiveGroup *findPrimitiveGroup(const UT_StringRef &name) const
    { return findElementGroup<GA_ATTRIB_PRIMITIVE>(name); }

    SYS_FORCE_INLINE
    GA_PointGroup *findPointGroup(const UT_StringRef &name)
    { return findElementGroup<GA_ATTRIB_POINT>(name); }
    SYS_FORCE_INLINE
    GA_VertexGroup *findVertexGroup(const UT_StringRef &name)
    { return findElementGroup<GA_ATTRIB_VERTEX>(name); }
    SYS_FORCE_INLINE
    GA_PrimitiveGroup *findPrimitiveGroup(const UT_StringRef &name)
    { return findElementGroup<GA_ATTRIB_PRIMITIVE>(name); }

    GA_EdgeGroup	*createEdgeGroup(const UT_StringHolder &name);
    GA_EdgeGroup	*createInternalEdgeGroup();
    const GA_EdgeGroup	*findEdgeGroup(const UT_StringRef &name) const;
    GA_EdgeGroup	*findEdgeGroup(const UT_StringRef &name);
    void		 getEdgeGroupList(
				UT_Array<const GA_EdgeGroup *> &list) const;
    void		 getEdgeGroupList(
				UT_Array<GA_EdgeGroup *> &list);
    SYS_FORCE_INLINE
    bool		 destroyEdgeGroup(const UT_StringRef &name)
			    { return myEdgeGroups.destroy(name); }
    SYS_FORCE_INLINE
    bool		 destroyEdgeGroup(GA_EdgeGroup *g)
			    { return g ? myEdgeGroups.destroy(g) : false; }

    // Utility group functions
    GA_Size		 destroyEmptyGroups(GA_AttributeOwner owner);
    GA_Size		 destroyAllEmptyGroups();

    /// Creates a non-internal group with the specified name,
    /// owned by this detail.
    /// @{
    GA_PointGroup *newPointGroup(const UT_StringHolder &name)
    {
        return static_cast<GA_PointGroup *>(pointGroups().newGroup(name));
    }
    GA_VertexGroup *newVertexGroup(const UT_StringHolder &name)
    {
        return static_cast<GA_VertexGroup *>(vertexGroups().newGroup(name));
    }
    GA_PrimitiveGroup *newPrimitiveGroup(const UT_StringHolder &name)
    {
        return static_cast<GA_PrimitiveGroup *>(primitiveGroups().newGroup(name));
    }
    GA_EdgeGroup *newEdgeGroup(const UT_StringHolder &name)
    {
        return static_cast<GA_EdgeGroup *>(edgeGroups().newGroup(name));
    }
    /// @}

    /// Creates an internal group with a unique name,
    /// owned by this detail.
    /// Be sure to call GA_Detail::destroyGroup (or destroyElementGroup)
    /// with this group to clean it up.
    /// @{
    GA_PointGroup *newInternalPointGroup()
    {
        return static_cast<GA_PointGroup *>(pointGroups().newInternalGroup());
    }
    GA_VertexGroup *newInternalVertexGroup()
    {
        return static_cast<GA_VertexGroup *>(vertexGroups().newInternalGroup());
    }
    GA_PrimitiveGroup *newInternalPrimitiveGroup()
    {
        return static_cast<GA_PrimitiveGroup *>(primitiveGroups().newInternalGroup());
    }
    GA_EdgeGroup *newInternalEdgeGroup()
    {
        return static_cast<GA_EdgeGroup *>(edgeGroups().newInternalGroup());
    }
    /// @}

    /// Creates a detached group with no name, owned by the caller.
    /// Be sure to do "delete group;" to clean it up.
    /// NOTE: The detail knows nothing about any detached groups that
    ///       are created.
    /// @{
    GA_PointGroup *newDetachedPointGroup() const
    {
        return new GA_PointGroup(*this);
    }
    GA_PrimitiveGroup *newDetachedPrimitiveGroup() const
    {
        return new GA_PrimitiveGroup(*this);
    }
    GA_VertexGroup *newDetachedVertexGroup() const
    {
        return new GA_VertexGroup(*this);
    }
    GA_EdgeGroup *newDetachedEdgeGroup() const
    {
        return new GA_EdgeGroup(*this);
    }
    /// @}

    /// TODO: Deprecate internal groups with user-specified names.
    /// @{
    GA_PointGroup *newPointGroup(const UT_StringHolder &name, bool internal)
    {
        return static_cast<GA_PointGroup *>(pointGroups().newGroup(name, internal));
    }
    GA_PrimitiveGroup *newPrimitiveGroup(const UT_StringHolder &name, bool internal)
    {
        return static_cast<GA_PrimitiveGroup *>(primitiveGroups().newGroup(name, internal));
    }
    GA_VertexGroup *newVertexGroup(const UT_StringHolder &name, bool internal)
    {
        return static_cast<GA_VertexGroup *>(vertexGroups().newGroup(name, internal));
    }
    GA_EdgeGroup *newEdgeGroup(const UT_StringHolder &name, bool internal)
    {
        return static_cast<GA_EdgeGroup *>(edgeGroups().newGroup(name, internal));
    }
    /// @}

    // -------------- Intrinsics ------------------------
    /// Register global intrinsics.
    /// Each subclass should have registration which looks like: @code
    ///	  SubClass::registerIntrinsics(GA_PrimitiveFactory &factory)
    ///	  {
    ///		GA_LocalIntrinsicMap	map(factory.getDetailIntrinsics());
    ///		BaseClass::registerIntrinsics(factory);
    ///		map.addAttribute(...);
    ///		map.addAttribute(...);
    ///	  }
    /// @endcode
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveFactory &factory);

    /// Get the intrinsic manager for global/detail intrinsics.
    /// 
    /// At the current time, these include:
    ///	- "string globaltokens[]"	@n List of all global/detail intrinsics
    ///	- "string primitivetokens[]"	@n List of all primitive intrinsics
    ///	- "int vertexcount"		@n Number of vertices in the detail
    ///	- "int pointcount"		@n Number of points in the detail
    ///	- "int primitivecount"		@n Number of primitives in the detail
    const GA_IntrinsicManager	&getIntrinsicManager() const
		    { return myPrimitiveFactory.getDetailIntrinsics(); }

    /// @{
    /// Query properties of the @b global/detail intrinsic
    /// Note that the tuple size calls the virtual method since the tuple size
    /// may be dynamic (i.e. basis knot vector)
    GA_LocalIntrinsic	 findIntrinsic(const char *nm) const
			    { return getIntrinsicManager().getLocalHandle(nm); }
    GA_LocalIntrinsic	 findIntrinsic(GA_GlobalIntrinsic h) const
			    { return getIntrinsicManager().getLocalHandle(h); }
    GA_GlobalIntrinsic	 findGlobalIntrinsic(GA_LocalIntrinsic h) const
			    { return getIntrinsicManager().getGlobalHandle(h); }

    const char		*getIntrinsicName(GA_LocalIntrinsic h) const
			    { return getIntrinsicManager().getName(h); }
    GA_Size		 getIntrinsicTupleSize(GA_LocalIntrinsic h) const;
    GA_StorageClass	 getIntrinsicStorage(GA_LocalIntrinsic h) const
			    { return getIntrinsicManager().getStorage(h); }
    bool		 getIntrinsicReadOnly(GA_LocalIntrinsic h) const
			    { return getIntrinsicManager().getReadOnly(h); }
    const UT_Options	*getIntrinsicOptions(GA_LocalIntrinsic h) const
			    { return getIntrinsicManager().getOptions(h); }
    /// @}

    /// @{
    /// Get the value of a global/detail intrinsic.
    /// These methods return the number of items retrieved.
    GA_Size	getIntrinsic(GA_LocalIntrinsic h, UT_String &val) const;
    GA_Size	getIntrinsic(GA_LocalIntrinsic h, UT_StringArray &val) const;
    GA_Size	getIntrinsic(GA_LocalIntrinsic h, int64 *v, GA_Size size) const;
    GA_Size	getIntrinsic(GA_LocalIntrinsic h, fpreal64 *v, GA_Size sz) const;
    /// @}

    /// @{
    /// Get the value of a global/detail intrinsic.
    /// Returns false if not the right type or not found.
    /// Always will clear out the provided value.
    bool	getIntrinsic(GA_LocalIntrinsic h, float &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, int &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Vector2 &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Vector3 &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Vector4 &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Matrix2 &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Matrix3 &v) const;
    bool	getIntrinsic(GA_LocalIntrinsic h, UT_Matrix4 &v) const;

    /// @}

    /// @{
    /// Set intrinsic.  This will fail if the intrinsic is read-only.
    /// These methods return the number of items set.
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const char *value);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const UT_StringArray &value);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const char **val, GA_Size s);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const int64 val);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const fpreal64 val);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const int64 *val, GA_Size sz);
    GA_Size	setIntrinsic(GA_LocalIntrinsic h, const fpreal64 *v, GA_Size sz);
    /// @}

    /// Calls functor on every active point offset.
    /// This is easiest to use with C++ lambdas, e.g.:
    /// detail.forEachPoint([&vec,&detail](GA_Offset ptoff) {
    ///     vec += detail.getPos3(ptoff);
    /// });
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPoint(FUNCTOR &&functor) const
    {
        myPointMap.forEachOffset(functor);
    }

    /// Calls functor on every point offset in a group.
    /// This is easiest to use with C++ lambdas, e.g.:
    /// detail.forEachPoint(group, [&vec,&detail](GA_Offset ptoff) {
    ///     vec += detail.getPos3(ptoff);
    /// });
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPoint(const GA_PointGroup *group, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPointMap, group);
    }

    /// Calls functor on every point offset in a group, (or not in a group
    /// if complement is true).
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPoint(const GA_PointGroup *group, bool complement, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPointMap, group, complement);
    }

    /// Like forEachPoint, except taking a functor that
    /// returns true to continue, and false to break.
    /// @{
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPointBreak(FUNCTOR &&functor) const
    {
        myPointMap.forEachOffsetBreak(functor);
    }
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPointBreak(const GA_PointGroup *group, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPointMap, group);
    }
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPointBreak(const GA_PointGroup *group, bool complement, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPointMap, group, complement);
    }
    /// @}

    /// Calls functor on every active primitive offset.
    /// This is easiest to use with C++ lambdas, e.g.:
    /// detail.forEachPrimitive([&tris,&detail](GA_Offset primoff) {
    ///     if (detail.getPrimitiveMap().getTypeId(primoff) != GA_PRIMPOLY)
    ///         return;
    ///     GA_OffsetListRef vertices = detail.getPrimitiveMap().getVertexList(primoff);
    ///     if (vertices.size() == 3 && vertices.getExtraFlag())
    ///         tris.addOffset(primoff);
    /// });
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitive(FUNCTOR &&functor) const
    {
        myPrimitiveMap.forEachOffset(functor);
    }

    /// Calls functor on every primitive offset in a group.
    /// This is easiest to use with C++ lambdas, e.g.:
    /// detail.forEachPrimitive(group, [&tris,&detail](GA_Offset primoff) {
    ///     if (detail.getPrimitiveMap().getTypeId(primoff) != GA_PRIMPOLY)
    ///         return;
    ///     GA_OffsetListRef vertices = detail.getPrimitiveMap().getVertexList(primoff);
    ///     if (vertices.size() == 3 && vertices.getExtraFlag())
    ///         tris.addOffset(primoff);
    /// });
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitive(const GA_PrimitiveGroup *group, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPrimitiveMap, group);
    }

    /// Calls functor on every primitive offset in a group, (or not in a group
    /// if complement is true).
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitive(const GA_PrimitiveGroup *group, bool complement, FUNCTOR &&functor) const
    {
        forEachOffset(functor, myPrimitiveMap, group, complement);
    }

    /// Like forEachPrimitive, except taking a functor that
    /// returns true to continue, and false to break.
    /// @{
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitiveBreak(FUNCTOR &&functor) const
    {
        myPrimitiveMap.forEachOffsetBreak(functor);
    }
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitiveBreak(const GA_PrimitiveGroup *group, FUNCTOR &&functor) const
    {
        forEachOffsetBreak(functor, myPrimitiveMap, group);
    }
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPrimitiveBreak(const GA_PrimitiveGroup *group, bool complement, FUNCTOR &&functor) const
    {
        forEachOffsetBreak(functor, myPrimitiveMap, group, complement);
    }
    /// @}

    template<typename FUNCTOR>
    static void forEachOffset(FUNCTOR &&functor, const GA_IndexMap &index_map, const GA_ElementGroup *group = nullptr, bool complement = false)
    {
        // Fall back to regular case if no group.
        if (!group)
        {
            if (!complement)
                index_map.forEachOffset(functor);
            return;
        }

        // Group order is only relevant if not complemented.
        if (!complement)
        {
            const GA_ElementGroupOrder *order = group->getOrdered();
            if (order)
            {
                for (GA_ElementGroupOrderIndex i(0), n(order->entries()); i != n; ++i)
                {
                    GA_Offset off = order->getElement(i);
                    functor(off);
                }
                return;
            }
        }

        // We have a group, treated as unordered.
        const GA_Offset veryend = index_map.offsetSize();
        GA_Offset off(0);
        while (true)
        {
            bool value;
            GA_Size span_size;
            group->getConstantSpan(off, veryend, span_size, value);
            if (span_size == 0)
                break;
            if (value == complement)
            {
                off += span_size;
                continue;
            }
            const GA_Offset span_end = off+span_size;
            while (true)
            {
                off = index_map.findActiveOffset(off, span_end);
                GA_Offset end = index_map.findInactiveOffset(off, span_end);
                if (off == end)
                    break;
                do
                {
                    functor(off);
                    ++off;
                } while (off != end);
            }
        }
    }

    template<typename FUNCTOR>
    static void forEachOffsetBreak(FUNCTOR &&functor, const GA_IndexMap &index_map, const GA_ElementGroup *group = nullptr, bool complement = false)
    {
        // Fall back to regular case if no group.
        if (!group)
        {
            if (!complement)
                index_map.forEachOffsetBreak(functor);
            return;
        }

        // Group order is only relevant if not complemented.
        if (!complement)
        {
            const GA_ElementGroupOrder *order = group->getOrdered();
            if (order)
            {
                for (GA_ElementGroupOrderIndex i(0), n(order->entries()); i != n; ++i)
                {
                    GA_Offset off = order->getElement(i);
                    if (!functor(off))
                        return;
                }
                return;
            }
        }

        // We have a group, treated as unordered.
        const GA_Offset veryend = index_map.offsetSize();
        GA_Offset off(0);
        while (true)
        {
            bool value;
            GA_Size span_size;
            group->getConstantSpan(off, veryend, span_size, value);
            if (span_size == 0)
                break;
            if (value == complement)
            {
                off += span_size;
                continue;
            }
            const GA_Offset span_end = off+span_size;
            while (true)
            {
                off = index_map.findActiveOffset(off, span_end);
                GA_Offset end = index_map.findInactiveOffset(off, span_end);
                if (off == end)
                    break;
                do
                {
                    if (!functor(off))
                        return;
                    ++off;
                } while (off != end);
            }
        }
    }

    // -------------- Range Accessors --------------------------
    /// Get a range of all points in the detail
    GA_Range	getPointRange(const GA_PointGroup *group = 0) const
		    { return GA_Range(getPointMap(), group); }
    /// Get a range of all primitives in the detail
    GA_Range	getPrimitiveRange(const GA_PrimitiveGroup *group = 0) const
		    { return GA_Range(getPrimitiveMap(), group); }

    /// Get ordered point range from base_ptnum to end_ptnum, or the end
    GA_Range	getPointRangeSlice(
			GA_Index begin_ptnum,
			GA_Index end_ptnum = GA_INVALID_INDEX) const
		    {
			GA_Range::ordered ordered;
			if (!GAisValid(end_ptnum))
			    end_ptnum = GA_Index(getNumPoints());
			return GA_Range(getPointMap(), begin_ptnum, end_ptnum,
					ordered);
		    }
    /// Get ordered primitive range from base_prim to end_prim, or the end
    GA_Range	getPrimitiveRangeSlice(
			GA_Index begin_prim,
			GA_Index end_prim = GA_INVALID_INDEX) const
		    {
			GA_Range::ordered ordered;
			if (!GAisValid(end_prim))
			    end_prim = GA_Index(getNumPrimitives());
			return GA_Range(getPrimitiveMap(),
					begin_prim, end_prim, ordered);
		    }

    /// Get a range of all vertices in the detail
    GA_Range	getVertexRange(const GA_VertexGroup *group = 0) const
		    { return GA_Range(getVertexMap(), group); }
    /// Get a range representing the global (detail) data
    GA_Range	getGlobalRange() const
		    { return GA_Range(getGlobalMap()); }

    // -------------- Common Operations ------------------------
    /// Compute memory usage (includes all shared memory)
    virtual int64	 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    // Computing the bounding box.  Depending on the element being iterated,
    // either the point or primitive bounding box will be computed.  When a
    // P attribute argument is not present, the detail's position attribute
    // is used.
    bool		 enlargeBoundingBox(UT_BoundingRect &box,
					const GA_Range &range) const;
    bool                 enlargeBoundingBox(UT_BoundingRect &box,
                                        const GA_Range &range,
                                        const GA_Attribute *P) const;
    bool		 enlargeBoundingBox(UT_BoundingBox &box,
					const GA_Range &range) const;
    bool                 enlargeBoundingBox(UT_BoundingBox &box,
                                        const GA_Range &range,
                                        const GA_Attribute *P) const;

    /// Compute the bounding box quickly by:
    ///  - Computing the point bounding box of "P"
    ///  - Computing the maximum extents of all primitives
    /// This requires a single pass through the P attribute along with a single
    /// pass through primitives.
    void		computeQuickBounds(UT_BoundingBox &box) const;

    /// Compute the bounding box quickly by:
    ///  - Computing the point bounding box of "P"
    ///  - Computing the maximum extents of all primitives
    ///  - Count the number of each type of primitive in the detail
    /// This requires a single pass through the P attribute along with a single
    /// pass through primitives.
    /// The @c counts buffer will be filled with the count of each primitive
    /// type in the detail.  The count_buf_size should be set to
    /// @c getPrimitiveFactory().getPrimTypeCount();
    void		computeQuickBounds(UT_BoundingBox &box,
				GA_Size counts[], GA_Size count_buf_size) const;

    /// Create a new detail that has all the same attributes, groups, etc. but
    /// has no elements.
    /// Subclasses should look at: cloneCopyGroupsAndAttributes()
    virtual GA_Detail	*cloneEmptyDetail(bool clone_attributes) const = 0;

    /// Copy groups and attribute definitions from the source detail.
    ///
    /// The caller should ensure that all attributes are cleared before calling
    /// this method (i.e.  call clear())
    void	 cloneCopyGroupsAndAttributes(const GA_Detail &src,
                        bool copydetailattribs = false);

    /// Copy all attribute values from srcoff to destoff for
    /// attributes accepted by filter.  If no filter is specified,
    /// all public attributes and non-internal groups are copied.
    ///
    /// NOTE: If you're copying multiple offsets, it's *much* faster to do:
    ///       For each attribute, copy data for all offsets;
    ///       rather than:
    ///       For each offset, copy data in all attributes.
    ///       These functions are only for if the former is too awkward
    ///       to do in some case.
    /// @{
    void copyAttributes(
        GA_AttributeOwner owner,
        GA_Offset destoff,
        GA_Offset srcoff,
        const GA_AttributeFilter *filter = nullptr);
    void copyAttributes(
        GA_AttributeOwner owner,
        GA_Offset destoff,
        const GA_Detail &srcdetail,
        GA_Offset srcoff,
        const GA_AttributeFilter *filter = nullptr);
    /// @}

    /// Copy all attribute values from srcptoff to destptoff for point
    /// attributes accepted by filter.  If no filter is specified,
    /// all public attributes and non-internal groups are copied.
    SYS_FORCE_INLINE
    void copyPoint(
        GA_Offset destptoff,
        GA_Offset srcptoff,
        const GA_AttributeFilter *filter = nullptr)
    {
        copyAttributes(GA_ATTRIB_POINT, destptoff, srcptoff, filter);
    }

    /// Copy all attribute values from srcvtxoff to destvtxoff for vertex
    /// attributes accepted by filter.  If no filter is specified,
    /// all public attributes and non-internal groups are copied.
    /// If ref_point_dont_copy is true, the point of srcvtxoff is
    /// referenced by destvtxoff, otherwise point attributes are also
    /// copied.
    /// NOTE: If you only want to copy vertex attributes, and not the
    ///       point, call copyAttributes(GA_ATTRIB_VERTEX, ...)
    void copyVertex(
        GA_Offset destvtxoff,
        GA_Offset srcvtxoff,
        bool ref_point_dont_copy,
        const GA_AttributeFilter *filter = nullptr)
    {
        GA_Offset srcptoff = vertexPoint(srcvtxoff);
        if (ref_point_dont_copy)
            myTopology.wireVertexPoint(destvtxoff, srcptoff);
        else
            copyPoint(vertexPoint(destvtxoff), srcptoff, filter);
        copyAttributes(GA_ATTRIB_VERTEX, destvtxoff, srcvtxoff, filter);
    }

    /// Merge the source detail into this detail (using default options)
    void	baseMerge(const GA_Detail &src);
    /// Merge the source detail into this detail with options
    void	baseMerge(const GA_Detail &src,
				GA_MergeOptions &options);

    /// Fast version of merge for the common case of completely replacing
    /// all of this detail with the source detail, using data IDs to avoid
    /// copying things that are the same.  This does not copy private
    /// scope attributes (except topology) or internal groups.
    /// When skip is given, all processing of attributes matching the filter
    /// will be skipped. The result is that skipped attributes may or may not
    /// exist afterwards, and they also won't be initialized in any expected
    /// way. The assumption is that the caller will fully handle creation,
    /// initialization, or deletion of skipped attributes.
    void        replaceWith(const GA_Detail &src,
			    GA_AttributeFilter *skip = nullptr);

    /// Fast version of merge for the case of completely replacing all of this
    /// detail with the points from the source detail, using data IDs to avoid
    /// copying things that are the same.  This does not copy private
    /// scope attributes (except topology) or internal groups.
    /// It will act as if replaceWith was called and then all primitives
    /// were deleted, so vertex/primitive attributes will be transferred,
    /// but there will be no vertices or primitives.
    /// When skip is given, all processing of attributes matching the filter
    /// will be skipped. The result is that skipped attributes may or may not
    /// exist afterwards, and they also won't be initialized in any expected
    /// way. The assumption is that the caller will fully handle creation,
    /// initialization, or deletion of skipped attributes.
    void        replaceWithPoints(const GA_Detail &src,
				  GA_AttributeFilter *skip = nullptr);

    /// This clears any caches that subclasses of GA_Detail may have, so that
    /// replacing the content of the detail doesn't cause crashes.
    /// NOTE: You may or may not also have to call refreshCachedHandles later
    ///       if P or Pw might be replaced or change storage.
    virtual void clearCaches()
    {
        clearInstanceMatrix();
        getPrimitiveList().destroyStashed();
    }

    /// Merge global attributes (using default options)
    void	mergeGlobalAttributes(const GA_Detail &src);
    /// Merge source global attributes using options
    void	mergeGlobalAttributes(const GA_Detail &src,
				GA_MergeOptions &options);

    /// Clone any missing attributes from the source detail, skipping P, any
    /// topology, and any group attributes.
    void	cloneMissingAttributes(const GA_Detail &src,
				       GA_AttributeOwner owner,
				       const GA_AttributeFilter &filter);
    /// Clone any missing groups from the source detail.
    void	cloneMissingGroups(const GA_Detail &src,
				   GA_AttributeOwner owner,
				   const GA_AttributeFilter &filter);

    // -------------- Stream I/O METHODS ------------------------
    /// Register an IO file format
    bool		registerIO(GA_IO *io) const
			    { return myPrimitiveFactory.registerIO(io); }

    /// Find an IO class.  If no name is specified, the "default" IO class will
    /// be returned (the one specified by HOUDINI_GEOMETRY_FORMAT).
    const GA_IO		*findIO(const char *name = nullptr) const
			    { return myPrimitiveFactory.findIO(name); }

    /// Get a list of all the IO names registered.  Returns the number of names
    /// in the array.
    void		 getIONames(UT_StringArray &names) const
			    { return myPrimitiveFactory.getIONames(names); }

    /// This class is used to return the status from I/O operations on a
    /// GA_Detail.
    class IOStatus
    {
    public:
	IOStatus(bool success=false)
	    : mySuccess(success)
	{}
	IOStatus(const IOStatus &src)
	    : mySuccess(src.mySuccess)
	{}
	~IOStatus() {}

	IOStatus	&operator=(const IOStatus &src)
			{
			    if (this != &src)
				mySuccess = src.mySuccess;
			    return *this;
			}

	/// Return a boolean for success or failure.
	/// true means success
	bool		success() const		{ return mySuccess; }

	/// bool operator
	SYS_SAFE_BOOL operator bool() const	{ return mySuccess; }

	/// For backward compatibility in older code, you can query the status
	/// as an integer, 0 for success, less than zero for failure.
	int		gbstatus() const	{ return mySuccess ? 0 : -1; }

    private:
	bool		mySuccess;
    };

    /// Save to a given filename.  An extension of .gz will enable zlib
    /// compression (for JSON format only)
    /// Options include:
    /// - string geo:format  @n Use the IO format specified by the string
    /// See also the options for the stream save
    IOStatus		 save(const char *filename,
				const GA_SaveOptions *options,
				UT_StringArray *errors = 0) const;

    /// Save to an existing ostream.  JSON format will check the options for:
    /// -  bool   geo:gzip      @n Enable gzip compression for JSON (false)
    /// -  bool   geo:saveinfo  @n Output optional "info" section [true]
    ///   -  string info:software - Software
    ///   -  string info:artist   - Artist
    ///   -  string info:hostname - Hostname comment
    ///   -  string info:comment  - Arbitrary Comment
    ///   -  any    info:token    - Saved in the "info" section
    /// -  bool   group:saveinternal @n Save "internal" groups (debugging)
    IOStatus		 save(std::ostream &os, bool binary,
				const GA_SaveOptions *options,
				UT_StringArray *errors = 0) const;
    /// @{
    /// Save in "hclassic" format.
    IOStatus	 saveClassic(const char *, const GA_SaveOptions *) const;
    IOStatus	 saveClassic(std::ostream &os, bool, const GA_SaveOptions *) const;
    /// @}

    /// @section JSON-GA_Detail JSON Schema: GA_Detail
    ///
    /// The JSON schema for a detail can be broken down into 5 major sections
    ///  - Header:  The global information about the detail
    ///  - References:  Topological references
    ///  - Attributes:  Attribute data for elements
    ///  - Primitives:  List of primitives in the detail
    ///  - Groups: Groups
    ///
    /// @code
    /// {
    ///     "name"          : "GA_Detail-Header",
    ///     "description"   : "Storage for an entire geometry object",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         // Header section
    ///         "file_version": {
    ///             "type"      : "string",
    ///             "description" : "File Version",
    ///             "optional"  : true,
    ///         },
    ///         "info": {
    ///             "type"        : "object",
    ///             "description" :
    ///                 "Optional tags including:
    ///                     'software' : Software used to create geometry,
    ///                     'artist'   : Artist name,
    ///                     'hostname' : Computer name,
    ///                     'comment'  : Arbitrary comment
    ///                     etc.",
    ///             "optional"    : true,
    ///         }
    ///         "pointcount": {
    ///             "type"        : "integer",
    ///             "description" : "The number of points in the geometry",
    ///         },
    ///         "vertexcount": {
    ///             "type"        : "integer",
    ///             "description" : "The number of vertices in the geometry",
    ///         },
    ///         "primitivecount": {
    ///             "type"        : "integer",
    ///             "description" : "The number of primitives in the geometry",
    ///         },
    ///
    ///         // Topology section
    ///         "topology" : {
    ///             "type"        : "orderedmap",
    ///             "description" : "Topological information",
    ///             "properties"  : {
    ///                 "pointref" : {
    ///                       "type"    : [{ "$ref" : "GA_ATITopology" }],
    ///                       "description" :
    ///                            "An list of points referenced by each
    ///                             vertex.  The array of 'vertexcount' items
    ///                             must contain integer values in the range of
    ///                             (0,pointcount-1)"
    ///                  }
    ///             }
    ///         },
    ///
    ///         // Attributes section
    ///         "attributes": {
    ///             "type"        : { "$ref" : "GA_AttributeSet" } ,
    ///             "description" : "Vertex attributes",
    ///         }
    ///
    ///         // Primitives section
    ///         "primitives": {
    ///             "type"        : { "$ref" : "GA_PrimitiveList" } ,
    ///             "description" : "An array of primitives",
    ///         }
    ///
    ///         // Groups section
    ///         "pointgroups": {
    ///             "type"        : { "$ref" : "GA_GroupTable" },
    ///             "description" : "The list of point groups",
    ///             "optional"    : true,
    ///         },
    ///         "primitivegroups": {
    ///             "type"        : { "$ref" : "GA_GroupTable" },
    ///             "description" : "The list of primitive groups",
    ///             "optional"    : true,
    ///         },
    ///         "vertexgroups": {
    ///             "type"        : { "$ref" : "GA_GroupTable" },
    ///             "description" : "The list of vertex groups",
    ///             "optional"    : true,
    ///         },
    ///         "edgegroups": {
    ///             "type"        : { "$ref" : "GA_GroupTable" },
    ///             "description" : "The list of edge groups",
    ///             "optional"    : true,
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    /// @note This method uses <tt>findIO(GA_IOTable::getJSONToken())</tt> and
    /// uses a dynamic cast to GA_IOJSON to actually do the writing.
    IOStatus		 save(UT_JSONWriter &w, const GA_SaveOptions *opts=0) const;

    /// Load a geometry file
    IOStatus		 load(const char *filename,
				const GA_LoadOptions *opts=0,
				UT_StringArray *errors=0);

    /// Load a geometry file given a UT_IStream.
    IOStatus		 load(UT_IStream &is,
				const GA_LoadOptions *opts=0,
				UT_StringArray *errors=0);

    /// Load geometry from a JSON stream.
    /// @note This method uses <tt>findIO(GA_IOTable::getJSONToken())</tt> and
    /// uses a dynamic cast to GA_IOJSON to actually do the loading.
    IOStatus		load(UT_JSONParser &p,
				const GA_LoadOptions *opts=0,
				UT_StringArray *errors=0);

    /// @{
    /// Stat a disk file rather than loading the entire geometry.
    ///   (see @ref GA_STAT_LEVEL)
    /// The method cannot be static since the primitive factory is required.
    /// The primitive factory stores the IO table for the geometry.
    ///
    /// When performing a stat on a disk file, if the format supports it, only
    /// the header of the file is loaded.  This means that the full
    /// information for the geometry isn't available.  Thus, this will give
    /// you different information than if you run stat() on geometry that's
    /// fully loaded into memory.
    ///
    /// If you inspect a .geo file, you can see the information that's
    /// available.  For example, you can get the "primcount_summary" using the
    /// GA_Stat::getInfoOptions() method.
    IOStatus	statFile(const char *filename, GA_Stat &stat,
				uint level=0xffff,
				const GA_LoadOptions *opts=nullptr) const;
    IOStatus	stat(UT_JSONParser &p, GA_Stat &stat, uint level) const;
    /// @}

    /// Private methods
    /// @private
    void	batchDeletion(GA_AttributeOwner owner);
    /// @private
    void	adjustArraySize(GA_AttributeOwner owner, GA_Offset size);
    /// @private
    void	constructElementBlock(GA_AttributeOwner owner, GA_Offset offset, GA_Offset nelements)
		    { myAttributes.constructElementBlock(owner, offset, nelements); }
    /// @private
    void	destructElement(GA_AttributeOwner owner, GA_Offset offset)
		    { myAttributes.destructElement(owner, offset); }

    /// @private mergeGrowArrays() is called during the merging process
    /// @b prior to the actual addition of new elements to the detail.
    /// At this point, attribute arrays are grown to store the incoming data
    void	mergeGrowArrays(GA_MergeMap &map, GA_AttributeOwner owner);

    /// Get information about the detail.
    /// @param stat   @n - Information about detail
    /// @param level  @n - Mask of information to retrieve
    ///   (see @ref GA_STAT_LEVEL)
    /// Note that since this method queries the geometry that's fully loaded
    /// into memory, you will get different information than the @c statFile()
    /// method.
    bool	stat(GA_Stat &stat, uint level=0xffff) const;

    /// Fill out only the attribute information on the stat
    void	statAttributes(GA_Stat &stat, uint level=0xffff) const;
    /// Fill out only the group information on the stat
    void	statGroups(GA_Stat &stat, uint level=0xffff) const;
    /// Fill out only the volume information.
    virtual void statVolumes(GA_Stat &stat, uint level=0xffff) const;

    /// Debugging output of detail
    static int64 printMemory(UT_WorkBuffer &buffer,
			    bool include_total=true,
			    int indent=3,
			    const char *title="Geometry Memory Tracking");

    /// @{
    /// Utility casting functions used by GA_GBMacros.h to maintain (source
    /// code) type compatibility with the old implementations that used the
    /// derived GD/GEO_PrimList objects.
    ///
    /// The first argument is used solely to infer the templatized type.
    /// Derived classes (GD_Detail/GEO_Detail) should contain embedded
    /// GB_MACRO_PRIM_TYPE typedef to specify
    /// the proper derived type to use for the cast.
    template <typename T> static inline const typename T::GB_MACRO_PRIM_TYPE *
	GB_MACRO_CAST(const T *, const GA_Primitive *prim)
	    { return static_cast<const typename T::GB_MACRO_PRIM_TYPE *>(prim);}
    template <typename T> static inline typename T::GB_MACRO_PRIM_TYPE *
	GB_MACRO_CAST(const T *, GA_Primitive *prim)
	    { return static_cast<typename T::GB_MACRO_PRIM_TYPE *>(prim); }
    /// @}

    typedef GA_Primitive GB_MACRO_PRIM_TYPE;

    /// @{
    /// @private
    /// Methods used by GEO primitives to save/load vertex references
    bool		 loadVertexPointReferenceH9(UT_IStream &is,
				GA_Offset &point_offset);
    bool		 saveVertexPointReferenceH9(std::ostream &os,
				int binary, GA_Offset vertex_offset) const;

    static bool		 loadAttribDataH9(UT_IStream &is, GA_Offset offset,
				const UT_Array<GA_AttribLoadDataH9> &l);
    static bool		 saveAttribDataH9(std::ostream &os, int binary,
				GA_Offset offset,
				const UT_Array<GA_AttribSaveDataH9> &l,
				char delim_ch = '(');
    /// Convenience methods for saving/loading both a vertex's point reference
    /// and its attribute values in a Houdini9 compatible style.  Note that the
    /// load wires the vertex to the loaded point.
    bool		 loadVertexH9(UT_IStream &is, GA_Offset vertex_offset,
				const UT_Array<GA_AttribLoadDataH9> &vtx_l);
    bool		 saveVertexH9(std::ostream &os, int binary,
				GA_Offset vertex_offset,
				const UT_Array<GA_AttribSaveDataH9> &vtx_l)
									const;
    /// @}

    /// Provide access to the factory.  This allows users to query information
    /// about factories.
    const GA_PrimitiveFactory	&getPrimitiveFactory() const
					{ return myPrimitiveFactory; }

    /// Refreshes handles cached in the detail.  This method must be called
    /// after changing the storage of the P attribute.
    virtual void	refreshCachedHandles();
    /// Refreshes the cached myP attribute.
    void		refreshCachedAttributes();

    /// @{
    /// Add/Remove an attribute to the tail-initialization list.
    ///
    /// When attributes grow in size, they usually initialize their values to
    /// the default value.  However, some attributes may not adhere to this
    /// rule, or due to space optimizations, may have the values set to
    /// non-default values.  In this case, they should be added to the
    /// tail-initialization list.  When new elements are appended to the
    /// detail, they will be told to re-initialize the new element.  Otherwise,
    /// the detail will assume that the element values are at their default
    /// value.
    ///
    /// When removing, the method will return @c true if the attribute was a
    /// tail initializer.
    ///
    /// @note These methods are thread-safe
    /// @note DO NOT CALL THESE: Use the GA_Attribute::setTailInitialization
    ///       instead.
    void	addTailInitializer(GA_Attribute *attrib);
    bool	removeTailInitializer(GA_Attribute *attrib);
    /// @}

    /// Tell all tail initialization attributes to initialize the given offset
    /// @note This method is not thread-safe
    void	constructTailInitializers(GA_AttributeOwner owner,
			    GA_Offset start, GA_Offset size=GA_Offset(1))
		{
		    if (myTailInitializers)
			doConstructTailInitializers(owner, start, size);
		}

    bool	containsPrimitiveType(const GA_PrimitiveTypeId &type) const
		    { return myPrimitiveList.containsType(type); }
    GA_Size	countPrimitiveType(const GA_PrimitiveTypeId &type) const
		    { return myPrimitiveList.countType(type); }
    bool	containsLocalTransformPrimitive() const
		    { return myPrimitiveList.
			     hasLocalTransform(myPrimitiveFactory); }
    bool	getPrimitivesOfType(const GA_PrimitiveTypeId &type,
			UT_Array<const GA_Primitive *> &prims) const
		    { return myPrimitiveList.getPrimitivesOfType(type, prims); }

    // ------------- Primitive Type Masks ----------------------
    
    /// Returns a primitive typemask that represents all known primitive type
    /// by the primitive factory that is associated with this detail.
    GA_PrimitiveTypeMask getPrimitiveTypeMaskAll() const;
    
    /// Returns the primitive typemask that represents all primitives in this
    /// detail.
    GA_PrimitiveTypeMask getPrimitiveTypeMask(
				    const GA_PrimitiveGroup *group = 0) const;
    
    
    /// Return cached attribute instance matrix pointer
    const GA_AttributeInstanceMatrix &getInstanceMatrix() const;

    /// Clear the attribute instance matrix pointer.  This is called
    /// automatically when point attributes are added or deleted.
    void				 clearInstanceMatrix();

    /// This function is entirely here so that GA has a way to check if a
    /// GA_Detail is a GEO_Detail (true) or GD_Detail (false).
    bool isPrimary() const
    {
        return myPrimitiveFactory.isForPrimaryDetail();
    }

    /// All GPU buffers that have been written to will be copied back
    /// to the CPU & marked clean.
    void		flushCEWriteCaches();

    /// Flush all GPU-backed CE attributes, clearing them from this
    /// geometry.  
    void		flushCECaches();

    /// Copy changed/non transforming attribute values from inputgeo
    /// owner specifies the attribute class to act upon
    /// This is used by SOP_Deform/SOP_AttribVop to speed up the duplication of input gdp.
    void	    copyChangedNonTransforming(const GA_Detail *inputgeo, GA_AttributeOwner owner);

    static const int64 INVALID_CACHE_COUNT = -1;

    void incrementMetaCacheCount() { myMetaCacheCount++; }
    int64 getMetaCacheCount() const { return myMetaCacheCount; }

    /// @{
    /// @private
    class GA_API MantraGeometry
    {
    public:
	MantraGeometry() {}
	virtual ~MantraGeometry() {}
	virtual void	refreshCachedHandles();
    };
    MantraGeometry		*mantraGeometry()
				    { return myMantraGeometry; }
    const MantraGeometry	*mantraGeometry() const
				    { return myMantraGeometry; }
    void			 setMantraGeometry(MantraGeometry *m)
				    { myMantraGeometry = m; }
    /// @}

protected:
    void	doConstructTailInitializers(GA_AttributeOwner owner,
			    GA_Offset start, GA_Offset size);
	
    bool	loadPrimitivesH9(UT_IStream &is, int count,
		      const GA_FileFormatH9::PrimTypeXlate &type_map,
		      const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
		      const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);
    bool	savePrimitivesH9(std::ostream &os, int binary,
		      const UT_Array<const GA_Primitive *> &list,
		      const GA_FileFormatH9::PrimTypeXlate &type_map,
		      const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
		      const UT_Array<GA_AttribSaveDataH9> &vx_attribs) const;

    /// @{
    /// Virtual implementation of intrinsic attributes.  See GA_IntrinsicEval
    /// for further details.
    virtual GA_Size	localIntrinsicTupleSize(GA_IntrinsicEval &eval) const;
    virtual GA_Size	localGetIntrinsicS(GA_IntrinsicEval &eval,
				UT_String &value) const;
    virtual GA_Size	localGetIntrinsicSA(GA_IntrinsicEval &eval,
				UT_StringArray &value) const;
    virtual GA_Size	localGetIntrinsicI(GA_IntrinsicEval &eval,
				int64 *value, GA_Size size) const;
    virtual GA_Size	localGetIntrinsicF(GA_IntrinsicEval &eval,
				fpreal64 *value, GA_Size size) const;
    virtual GA_Size	localSetIntrinsicS(GA_IntrinsicEval &eval,
				const char *value);
    virtual GA_Size	localSetIntrinsicSA(GA_IntrinsicEval &eval,
				const UT_StringArray &value);
    virtual GA_Size	localSetIntrinsicSS(GA_IntrinsicEval &eval,
				const char **value, GA_Size size);
    virtual GA_Size	localSetIntrinsicI(GA_IntrinsicEval &eval,
				const int64 *value, GA_Size size);
    virtual GA_Size	localSetIntrinsicF(GA_IntrinsicEval &eval,
				const fpreal64 *value, GA_Size size);
    /// @}

    // Methods used by GEO when loading old style geometry files.
    static void		finishLoadH9(
				const UT_Array<GA_AttribLoadDataH9> &attribs
			);

    friend class GA_AttributeSet;
    /// This is here just so that GA_AttributeSet::jsonSave() can call
    /// GEOcomputeNormals()
    virtual void privateComputeNormal(
        const GA_RWHandleV3 &normalattrib,
        const GA_Group *group,
        const float cuspangledegrees,
        const int method) const
    {}
    /// This is here so that GA_AttributeSet::jsonLoad() can call it
    /// after loading attributes, since VDB primitive loading tries to
    /// write to P, and we might as well have it correctly split up
    /// before anything might read/write it.
    void splitAnyFloat4P();

private:
    void	init(GA_Storage Pstore, bool isprimary, bool full_topology);

    GA_Offset	newPrimitiveOffset();

    void	clearData(bool for_deletion);

    /// Destroy elements.  When destroying points, if the
    /// guarantee_no_vertex_point_references is true, we don't bother
    /// dereferencing the points (which speeds up the operation).
    GA_Size	destroyElements(const GA_Range &range,
			GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES,
			bool guarantee_no_vertex_point_references=false);

    // Try to make primitives de-reference the point
    bool	dereferencePoint(GA_Offset point,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES);
    // Try to make primitives de-reference a range of points.  A return value
    // of true means that all points in the range are no longer referenced by
    // primitives, while a return value of false means that some points in the
    // range continue to be referenced.  This method may allocate a membership
    // query object, in which case, the caller is responsible for deleting it.
    bool	dereferencePoints(const GA_Range &point_range,
				  const GA_RangeMemberQuery *&point_range_query,
				  GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES);


    /// Called by GA_IO during loading of optimized formats
    bool	setLoadCounts(GA_Size npoint, GA_Size nvertex, GA_Size nprim,
				GA_LoadMap &load);

    friend class	GA_IO;
    friend class GA_PrimitiveList;

    GA_PrimitiveFactory		&myPrimitiveFactory;
    GA_IndexMap			 myPointMap;
    GA_IndexMap			 myVertexMap;
    GA_IndexMap			 myPrimitiveMap;
    GA_IndexMap			 myGlobalMap;	// Always has a single entry
    GA_PrimitiveList		 myPrimitiveList;
    GA_AttributeSet		 myAttributes;
    GA_Topology			 myTopology;
    GA_Attribute		*myP;
    GA_RWHandleV3		 myHandlePV3;
    GA_Attribute                *myPw;
    ga_TailInitializeTable	*myTailInitializers;
    MantraGeometry		*myMantraGeometry;
    mutable GA_AttributeInstanceMatrix *myInstanceMatrix;
    GA_PointGroupTable	 	 myPointGroups;
    GA_VertexGroupTable	 	 myVertexGroups;
    GA_PrimitiveGroupTable	 myPrimitiveGroups;
    GA_EdgeGroupTable		 myEdgeGroups;
    exint			 myUniqueId;


    /// Checking this meta cache count and the unique ID of the
    /// detail is sufficient to determine whether this detail
    /// has been modified since it was last examined, so long as it's
    /// not currently being used in a cook.  Any cook only needs to
    /// increment the meta cache count once, and only if the detail
    /// is actually modified, (e.g. don't need to inc if instancing).
    ///
    /// NOTE: Do not rely on the detail pointer to check for modification,
    ///       since a different detail could happen to be allocated
    ///       at the same address in memory.
    int64 myMetaCacheCount;
};

#endif
