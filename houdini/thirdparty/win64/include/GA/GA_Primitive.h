/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_Primitive.h (GA Library, C++)
 *
 * COMMENTS:    Interface for primitives.
 */

#pragma once

#ifndef __GA_Primitive__
#define __GA_Primitive__

#define GA_PRIMITIVE_VERTEXLIST 1

#include "GA_API.h"
#include "GA_Detail.h"
#include "GA_IntrinsicManager.h"
#include "GA_OffsetList.h"
#include "GA_PrimCompat.h"
#include "GA_PrimitiveDefinition.h"
#include "GA_PrimitiveFamilyMask.h"
#include "GA_PrimitiveTypeId.h"
#include "GA_SharedDataHandle.h"
#include "GA_SharedLoadData.h"
#include "GA_Range.h"
#include "GA_Topology.h"
#include "GA_Types.h"

#include <UT/UT_BoundingBox.h>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <functional>
#include <iosfwd>
#include <stddef.h>

class GA_AttribLoadDataH9;
class GA_AttribSaveDataH9;
class GA_Attribute;
class GA_Defragment;
class GA_Detail;
class GA_IntrinsicEval;
class GA_MergeMap;
class GA_PrimitiveJSON;
class GA_PrimitiveList;
class GA_RangeMemberQuery;
class GA_GeometryIndex;

class UT_BoundingSphere;
class UT_IStream;
class UT_MemoryCounter;
class UT_Options;
class UT_String;
class UT_StringArray;
template <typename T> class UT_Array;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;


class GA_Primitive;
typedef void (* GA_EdgeApplyFunc)(const GA_Primitive &prim,
				GA_Offset pt_a, GA_Offset pt_b, void *data);
typedef void (* GA_EdgeApplyIndexFunc)(const GA_Primitive &prim,
				GA_Size v1, GA_Size v2, void *data);

using GA_IterateEdgesFunc = std::function<bool(const GA_Edge &edge)>;
using GA_IterateEdgesByVertexFunc = std::function<bool(GA_Size, GA_Size)>; 

#define GA_NO_OVERRIDE

/// Declare intrinsic callback functions in header file.  See
/// GA_IntrinsicMacros.h for macros to help implement intrinsics.
#define GA_DECLARE_INTRINSICS(OVERRIDE) \
  static GA_IntrinsicManager::Registrar \
			registerIntrinsics(GA_PrimitiveDefinition &); \
  virtual GA_Size localIntrinsicTupleSize(const GA_IntrinsicEval &) const \
			OVERRIDE; \
  virtual GA_Size localGetIntrinsicI(const GA_IntrinsicEval &, \
			int64 *, GA_Size) const OVERRIDE; \
  virtual GA_Size localGetIntrinsicF(const GA_IntrinsicEval &, \
			fpreal64 *, GA_Size) const OVERRIDE; \
  virtual GA_Size localGetIntrinsicS(const GA_IntrinsicEval &, \
			UT_String &) const OVERRIDE; \
  virtual GA_Size localGetIntrinsicSA(const GA_IntrinsicEval &, \
			UT_StringArray &) const OVERRIDE; \
  virtual GA_Size localSetIntrinsicI(const GA_IntrinsicEval &, \
			const int64 *, GA_Size) OVERRIDE; \
  virtual GA_Size localSetIntrinsicF(const GA_IntrinsicEval &, \
			const fpreal64 *, GA_Size) OVERRIDE; \
  virtual GA_Size localSetIntrinsicSS(const GA_IntrinsicEval &, \
			const char **, GA_Size) OVERRIDE; \
  virtual GA_Size localSetIntrinsicSA(const GA_IntrinsicEval &, \
			const UT_StringArray &) OVERRIDE;

class GA_API GA_Primitive : UT_NonCopyable
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    SYS_FORCE_INLINE
    GA_Primitive(GA_Detail &detail, GA_Offset offset=GA_INVALID_OFFSET)
        : myDetail(&detail)
        , myOffset(offset)
    {}

public:
    /// NOTE: The destructor should only be called from subclass
    ///       destructors; only GA_PrimitiveList should be calling
    ///       delete on GA_Primitive pointers.
    virtual ~GA_Primitive()
    {
        // The detail should have already deleted the vertices, and
        // myVertexList is automatically cleaned up in its destructor.
    }

    virtual const GA_PrimitiveDefinition &getTypeDef() const = 0;

    const char *getTypeName() const
    { return getTypeDef().getToken(); }

    /// Gets the detail containing this primitive.
    ///
    /// FIXME: This should return a const detail, and a non-const version should
    ///        return a non-const detail.
    SYS_FORCE_INLINE
    GA_Detail &getDetail() const
    { return *myDetail; }

    /// Gets the offset of this primitive in the detail containing it.
    SYS_FORCE_INLINE
    GA_Offset getMapOffset() const
    { return myOffset; }

    /// Gets the index of this primitive in the detail containing it.
    SYS_FORCE_INLINE
    GA_Index getMapIndex() const
    { return myDetail->primitiveIndex(myOffset); }

    /// Gets the index map for primitives in the detail containing this primitive.
    SYS_FORCE_INLINE
    const GA_IndexMap &getIndexMap() const
    { return myDetail->getPrimitiveMap(); }

    SYS_DEPRECATED_HDK(13.0)
    GA_Index getNum() const
    { return getMapIndex(); }

    /// For defragmentation, we need to update the offset
    void swapOffsetValue(const GA_Defragment &defrag);

    /// Returns true if the primitive is part of a 3D geometry detail
    /// (GEO_Detail), and false if it is part of a 2D geometry detail
    /// (GD_Detail) for trim/projection curves.
    bool isPrimary() const
    { return myDetail->isPrimary(); }

    // FIXME: Switch this back to SYS_FORCE_INLINE when Microsoft fixes
    //        Visual Studio bug report ID 2154929.
    //        It seems to be fixed in Update 3, so I updated the define,
    //        but we haven't switched our builds over yet from Update 1.
    SYS_FORCE_INLINE
    const GA_PrimitiveTypeId &getTypeId() const
			    { return getTypeDef().getId(); }
    GA_PrimitiveFamilyMask	getFamilyMask() const
			    { return getTypeDef().getFamilyMask(); }
    bool		 isFamily(unsigned family_mask) const
			    { return ((getFamilyMask() & family_mask) != 0); }

    /// Whether the primitive has a transform associated with it
    bool		hasLocalTransform() const
			    { return getTypeDef().hasLocalTransform(); }
    /// Return the local transform matrix for the primitive.  Some primitives
    /// don't store transforms (see @c hasLocalTransform()).  The default
    /// behaviour is to make the transform an identity.
    virtual void	getLocalTransform(UT_Matrix3D &matrix) const;

    /// Return the local transform matrix for the primitive, but with the
    /// translates specified by the position of the first vertex of the
    /// primitive.  This only includes the local transform and the translate
    /// due to the point position.
    virtual void	getLocalTransform4(UT_Matrix4D &matrix) const;

    /// Set the local transform.  The default implementation does nothing.
    virtual void	setLocalTransform(const UT_Matrix3D &matrix);

    /// @deprecated This method is only for transitional backward compatibility
    /// for the HDK and will be removed in a future release.
    /// TODO: Should be pure virtual here, overwritten at GEO_Primitive.
    virtual GA_PrimCompat::TypeMask	 getPrimitiveId() const
			{ return primCompatMaskFromTypeId(getTypeId().get()); }

    /// Report approximate memory usage, including sizeof(*this) and
    /// any shared memory.
    virtual int64 getMemoryUsage() const
    {
        return sizeof(*this) + getBaseMemoryUsage();
    }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    /// @{
    /// This method is called when the vertex index map is being defragmented.
    /// Since the defragmentation process moves vertex offsets, primitives need
    /// to update their references with the new values.  @code
    ///	   new_vtx_offset = defrag.mapOffset(new_vtx_offset);
    ///	   myOffsetList.swapOffsetValues(defrag);
    ///	   myOffsetMatrix.swapOffsetValues(defrag);
    /// @endcode
    virtual void swapVertexOffsets(const GA_Defragment &defrag);
    /// @}

    /// Return the number of vertices used by this primitive
    SYS_FORCE_INLINE
    GA_Size getVertexCount() const
    {
        return myVertexList.size();
    }

    /// Given an vertex number (into the primitive's vertex list), return the
    /// vertex offset.
    SYS_FORCE_INLINE
    GA_Offset getVertexOffset(GA_Size primvertexnum) const
    {
        UT_ASSERT_P(primvertexnum >= 0 && primvertexnum < myVertexList.size());
        return myVertexList.get(primvertexnum);
    }
    SYS_FORCE_INLINE
    GA_Index getVertexIndex(GA_Size primvertexnum) const
    {
        return getDetail().vertexIndex(getVertexOffset(primvertexnum));
    }

    /// Given a vertex number (into the primitive's vertex list), return the
    /// point offset.
    SYS_FORCE_INLINE
    GA_Offset getPointOffset(GA_Size i) const
    { return getDetail().vertexPoint(getVertexOffset(i)); }

    /// Given a vertex number (into the primitive's vertex list), set the
    /// corresponding point offset.
    SYS_FORCE_INLINE
    void setPointOffset(GA_Size i, GA_Offset ptoff)
    {
        getDetail().getTopology().wireVertexPoint(getVertexOffset(i), ptoff);
    }

    /// Given a vertex number (into the primitive's vertex list), return its
    /// point index number.
    SYS_FORCE_INLINE
    GA_Index getPointIndex(GA_Size i) const
    { return getDetail().pointIndex(getPointOffset(i)); }

    /// Given a vertex number (into the primitive's vertex list), return its
    /// point's position.
    SYS_FORCE_INLINE
    UT_Vector3 getPos3(GA_Size i) const
    { return getDetail().getPos3(getPointOffset(i)); }

    /// Given a vertex number (into the primitive's vertex list),
    /// move the point to the specified position.
    SYS_FORCE_INLINE
    void setPos3(GA_Size i, const UT_Vector3 &pos) const
    { getDetail().setPos3(getPointOffset(i), pos); }

    /// Given a vertex number (into the primitive's vertex list), return its
    /// point's position.
    SYS_FORCE_INLINE
    UT_Vector4 getPos4(GA_Size i) const
    { return getDetail().getPos4(getPointOffset(i)); }

    /// Given a vertex number (into the primitive's vertex list),
    /// move the point to the specified position.
    SYS_FORCE_INLINE
    void setPos4(GA_Size i, const UT_Vector4 &pos) const
    { getDetail().setPos4(getPointOffset(i), pos); }

    /// Get a range of all the vertices accessed by the primitive
    GA_Range	getVertexRange(bool harden=false) const
		{
		    return GA_Range(getDetail(), getMapOffset(),
				GA_ATTRIB_VERTEX, GA_Range::primitiveref(),
				harden);
		}
    /// Get a range of all the points accessed by the primitive.  Note, the
    /// range may visit points multiple times.
    GA_Range	getPointRange(bool harden=false) const
		{
		    return GA_Range(getDetail(), getMapOffset(),
				GA_ATTRIB_POINT, GA_Range::primitiveref(),
				harden);
		}

    /// Return whether a point is referenced by a primitive.  This simply
    /// iterates over the vertices, checking if any vertices reference the
    /// point.
    bool	isPointUsed(GA_Offset ptoff) const
    {
        GA_Size nvtx = getVertexCount();
        for (GA_Size i = 0; i < nvtx; ++i)
        {
            if (getPointOffset(i) == ptoff)
                return true;
        }
        return false;
    }

    /// Add all points referenced by the primitive to the given group.  This
    /// defaults to iterating over the vertices and adding their referenced
    /// points.
    void addPointRefToGroup(GA_PointGroup &grp) const;

    /// Method to determine if a primitive has an edge (undirected).
    virtual bool	 hasEdge(const GA_Edge &edge) const;


    /// Calls apply for each directed edge in this primitive
    /// @deprecated Use @c iterateEdges instead.
    SYS_DEPRECATED_HDK_REPLACE(16.0, iterateEdges)
    void edgeApply(GA_EdgeApplyFunc apply, void  *data = nullptr) const;
    
    /// Calls apply for each directed edge in this primitive
    /// @deprecated Use @c iterateEdgesByVertex instead.
    SYS_DEPRECATED_HDK_REPLACE(16.0, iterateEdgesByVertex)
    void edgeApplyIndex(GA_EdgeApplyIndexFunc apply, void *data = nullptr) const;
    
    /// Calls @c apply_func for each directed edge on the primitive. 
    /// If the @c apply_func function returns @c false, then the 
    /// traversal will be stopped. 
    virtual void iterateEdges(GA_IterateEdgesFunc apply_func) const 
    { 
	// Default implementation does nothing. 
	// Re-implemented by relevant primitives.
    }
    
    /// Calls @c apply_func for each directed edge on the primitive, 
    /// passing in the @c GA_Size primitive vertex index for each point on the
    /// edge. If the @c apply_func function returns @c false, then the 
    /// traversal will be stopped. 
    virtual void iterateEdgesByVertex(GA_IterateEdgesByVertexFunc apply_func) const
    { 
	// Default implementation does nothing. 
	// Re-implemented by relevant primitives.
    }
    
    /// Calls edgeApplyIndex to find the vertices within this primitive,
    /// corresponding with the first undirected edge between ptoff0 and ptoff1.
    /// If the edge is not in this primitive, vtx0 and vtx1 will not be
    /// written-to, and the function will return false.
    /// When written, vtx0->vtx1 will always be the direction the edge
    /// appears in this primitive, which may be *opposite* ptoff0->ptoff1.
    bool findEdgePoints(GA_Offset ptoff0, GA_Offset ptoff1,
        GA_Size &vtx0, GA_Size &vtx1) const;

    /// The return status of @c dereferencePoint()
    /// - GA_DEREFERENCE_FAIL @n
    ///	  The primitive cannot detach itself from the points and thus the
    ///	  points cannot be deleted.  It's possible that the primitive may be
    ///	  destroyed in this case if the user specified this behaviour (see
    ///	  GA_Detail::GA_DestroyPointMode).
    /// - GA_DEREFERENCE_OK @n
    ///   The primitive has detached the points and is still valid.
    /// - GA_DEREFERENCE_DEGENERATE @n
    ///	  The primitive has detached the points but is now degenerate.
    /// - GA_DEREFERENCE_DESTROY @n
    ///	  The primitive has detached the points and should now be destroyed.
    enum GA_DereferenceStatus
    {
	GA_DEREFERENCE_FAIL,
	GA_DEREFERENCE_OK,
	GA_DEREFERENCE_DEGENERATE,
	GA_DEREFERENCE_DESTROY,
    };
    /// Before points are deleted, all primitives referencing the points will
    /// be notified using @c dereferencePoint or @c dereferencePoints.  These
    /// methods should return the appropriate DeferefenceStatus.
    /// @example
    ///	 A quadric primitive will return
    ///	 - GA_DEREFERENCE_OK @n
    ///    If it's point is not in the point group
    ///	 - GA_DEREFERENCE_DESTROY @n
    ///    If it destroyed its vertex
    ///	 A hull primitive will return
    ///  - GA_DEREFERENCE_OK @n
    ///    If a full row or column was dereferenced
    ///  - GA_DEREFERENCE_DEGENERATE @n
    ///    If a full row/column was dereferenced, leaving a degenerate
    ///    primitive.
    ///  - GA_DEREFERENCE_FAIL @n
    ///	   If a single point from within the hull cannot be dereferenced
    /// The @c dry_run parameter will prevent the point from actually being
    /// detached.
    virtual GA_DereferenceStatus	 dereferencePoint(GA_Offset point,
						bool dry_run=false) = 0;
    virtual GA_DereferenceStatus	 dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false) = 0;

    /// @{
    /// Enlarge a bounding box by the bounding box of the primitive.  A
    /// return value of false indicates an error in the operation, most
    /// likely an invalid P.  By default, these methods simply enlarge the
    /// bounding box based on the vertices.
    virtual bool		enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool		enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    /// @}
    /// Enlarge a bounding sphere to encompass the primitive.  A return value
    /// of false indicates an error in the operation, most likely an invalid
    /// P.  By default, this method simply enlarges the bounding box based on
    /// the vertices.
    virtual bool		enlargeBoundingSphere(UT_BoundingSphere &b,
					const GA_Attribute *P) const;

    /// Enlarge a point bounding box for implicit primitive bounds.  If a
    /// primitive (such as a sphere) has a bounding box larger than its point
    /// hull, then it should implement this method.  Otherwise, it can rely on
    /// the default behaviour.
    virtual void		enlargePointBounds(UT_BoundingBox &box) const;

    /// Is the primitive degenerate
    virtual bool		isDegenerate() const = 0;

    /// Copy the specified primitive as part of the merge.  The source
    /// primitive will be of the same type.
    ///
    /// Since the vertex offsets will change for the merged geometry, the map
    /// should be used to lookup the new vertex offsets.  For example
    /// @code
    ///    map.mapDestFromSource( GA_ATTRIB_VERTEX, source_vertex );
    /// @endcode
    ///
    /// @see GA_MergeMap
    virtual void copyUnwiredForMerge(
        const GA_Primitive *src, const GA_MergeMap &map);

    /// This method returns the JSON interface for saving/loading the primitive
    /// If the method returns a NULL pointer, then the primitive will @b not
    /// be saved to geometry files (and thus cannot be loaded).
    virtual const GA_PrimitiveJSON		*getJSON() const = 0;

    /// Some primitives can have common/shared data between different
    /// instances of the primitives.  This method is invoked during the save
    /// process to save data.  The method requires you to save two objects
    /// sequentially (the data is stored in a list) for each shared key:
    ///   - The primitive typedef name.  This is used at load time to find the
    ///     definition which knows how to load the data.
    ///   - The data required to represent the shared object.  This is
    ///     typically a list or map containing the type of data, the key value
    ///     (used for loading) and the actual object data.
    ///
    /// If there are multiple shared data pointers, the array can continue with name/value pairs.
    ///
    /// For example, you might have something like: @code
    ///   const char *key = makeKeyForSharedPtr(ptr);
    ///   bool        ok = true;
    ///   if (!save.hasSavedSharedData(key))
    ///   {
    ///      save.setSavedSharedData(key);     // Flag this data as saved
    ///      ok = ok && w.jsonStringToken(getTypeName());
    ///      ok = ok && w.jsonBeginArray();
    ///      ok = ok && w.jsonStringToken("datatype");
    ///      ok = ok && w.jsonStringToken(key);
    ///      ok = ok && saveSharedData(ptr);
    ///      ok = ok && w.jsonEndArray();
    ///   }
    ///   return ok;
    /// @endcode
    /// Then in the primitive JSON, you'd likely want to have two fields, one
    /// for the shared data pointer, the other for an expanded version of the
    /// data.  The @c shouldSaveField() method would be something like: @code
    ///     if (index == SHARED_DATA_KEY)
    ///         return save.hasSavedSharedData(key);
    ///     if (index == EXPANDED_DATA_KEY)
    ///         return !save.hasSavedSharedData(key);
    /// @endcode
    ///
    /// Then the saveField() method of the primitive JSON would look something
    /// like: @code
    ///   switch (index)
    ///   {
    ///         case SHARED_DATA_KEY:
    ///            UT_ASSERT(save.hasSaveSharedData(key));
    ///            return w.jsonString(key);
    ///         case EXPANDED_DATA_KEY:
    ///            UT_ASSERT(!save.hasSaveSharedData(key));
    ///            return saveSharedData(ptr);
    ///  }
    /// @endcode
    ///
    /// During the loading process, you can just get the shared data from the
    /// load map. @code
    ///   MySharedDataContainer *data;
    ///   data = load.sharedLoadDataAs<MySharedDataContainer>(key);
    ///   if (data)
    ///       setSharedPointer(data->getSharedData();
    ///   else
    ///       return false;
    /// @endcode
    virtual bool	saveSharedLoadData(UT_JSONWriter &w, GA_SaveMap &save, GA_GeometryIndex* geometryIndex) const;


    virtual bool	registerSharedLoadData(int load_data_type, GA_SharedDataHandlePtr item);

    virtual bool	loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);
    virtual bool	saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;

    // ------------ Intrinsic interface ------------
    /// Get the intrinsic manager for primitive intrinsics.
    /// All primitives have at least the following intrinsics:
    ///	- "string typename"	@n The type name of the primitive
    ///	- "int typeid"		@n The type id (may change per run)
    ///	- "int vertexcount"	@n The number of vertices in the primitive
    const GA_IntrinsicManager	&getIntrinsicManager() const
				{ return getTypeDef().getIntrinsicManager(); }

    /// @{
    /// Query properties of the primitive intrinsic.
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
    /// Get the value of a primitive intrinsic.
    /// These methods return the number of items retrieved.
    GA_Size		getIntrinsic(GA_LocalIntrinsic h, UT_String &val) const;
    GA_Size		getIntrinsic(GA_LocalIntrinsic h, UT_StringArray &val) const;
    GA_Size		getIntrinsic(GA_LocalIntrinsic h, int64 *v, GA_Size size) const;
    GA_Size		getIntrinsic(GA_LocalIntrinsic h, fpreal64 *v, GA_Size sz) const;
    /// @}

    /// @{
    /// Get the value of a primitive intrinsic.
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
    /// Set intrinsic attributes.  This will fail if the intrinsic is read-only.
    /// These methods return the number of items set.
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const char *value);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const UT_StringArray &value);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const char **val, GA_Size s);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const int64 val);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const fpreal64 val);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const int64 *val, GA_Size sz);
    GA_Size		setIntrinsic(GA_LocalIntrinsic h, const fpreal64 *v, GA_Size sz);
    /// @}

    /// Private interface
    /// The clearForDeletion() method is called just prior to the detail being
    /// cleared out.  This allows the primitive to simplify its data structures,
    /// knowing that topology doesn't need to be maintained (i.e. unwiring all
    /// vertices);
    virtual void	clearForDeletion() {}

    /// Return the next and previous vertex on the "boundary" of the primitive
    /// (as understood by the primitive) relative to a given vertex. These are
    /// needed to maintain half-edge topology attributes.
    /// They must be implemented for primitives that realize a notion of a
    /// boundary composed of one or more closed cycles of vertices if we want
    /// to be able to use hedges to quickly move from such a primitive to
    /// another sharing an edge with it. A GA_INVALID_OFFSET for prev_vtx or
    /// next_vtx means that either the primitive doesn't care about hedges,
    /// the input vertex is not a boundary vertex in the primitive (or not a
    /// vertex at all), or vtx is at an end of a boundary (e.g. open polys).
    virtual void	getAdjacentBoundaryVertices(GA_Offset vtx,
						    GA_Offset &prev_vtx,
						    GA_Offset &next_vtx) const
			    { prev_vtx = next_vtx = GA_INVALID_OFFSET; }

    /// Primitives that implement getAdjacentBoundaryVertices must respond true
    /// to supportsHedge().
    virtual bool	supportsHedge() const
			    { return false; }

    /// Release the given vertex. This is equivalent to deleting a vertex
    /// without modifying its topology attributes. In other words, the primitive
    /// doesn't use the vertex anymore, but the vertex stays wired to the
    /// primitive and to the point it was wired to. The intended use is with
    /// stealVertex methods, so that the same vertex can be aquired by another
    /// primitive, thus maintaining its offset number and attributes The return
    /// value is the vertex offset (same as input) if successful and
    /// GA_INVALID_OFFSET otherwise.
    virtual GA_Offset	releaseVertex(GA_Offset vtx)
        			    { return GA_INVALID_OFFSET; }


    /// Backward compatible methods for GD library
    /// Determine whether the primitive is visisble on the interval of the
    /// parent surface.  Partial visiblity should return true as well.
    /// @return
    ///	 - @c 0: Not visible on parent surface
    ///	 - @c 1: Full visibility (@b default)
    ///	 - @c 2: Partial visibility
    virtual int		isVisible() const;

    /// A primitive may support any number of secondary details.
    virtual unsigned		 getNumSecondaryDetails() const;
    virtual const GA_Detail	*getSecondaryDetail(GA_Index i) const;
    virtual       GA_Detail	*getSecondaryDetail(GA_Index i);

    GA_Index		 getNumSecondary(GA_Index detail) const;
    const GA_Primitive	*getSecondaryByIndex(GA_Index detail, GA_Index i) const;
    GA_Primitive	*getSecondaryByIndex(GA_Index detail, GA_Index i);
    const GA_Primitive	*getSecondaryByOffset(GA_Index detail,
					      GA_Offset o) const;
    GA_Primitive	*getSecondaryByOffset(GA_Index detail, GA_Offset o);

    /// @{
    /// Primitives must provide these methods
    virtual void	 reverse() = 0;
    /// @}

    /// @{
    /// Measure the primitive.
    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const
			    { return 0; }
    virtual fpreal	calcArea() const		{ return 0; }
    virtual fpreal	calcPerimeter() const	{ return 0; }
    /// @}

    /// A trivial vertex list is a uniform ascending list, so
    /// getFastVertexOffset(i) == getFastVertexOffset(0) + i
    SYS_FORCE_INLINE
    bool		isVertexListTrivial() const { return myVertexList.isTrivial(); }
    /// A trivial point list is doubly-trivial: 
    /// getPointOffset(i) == getPointOffset(0) + i
    /// This requires testing every point so is slower than isVertexListTrivial.
    bool		isPointListTrivial() const;

    /// Calls a functor (e.g. a lambda) for each vertex offset in this
    /// primitive, in the order they occur in the vertex list.
    /// This is optimized to only check triviality once.
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachVertex(FUNCTOR &&functor) const
    {
        myVertexList.forEach(functor);
    }

    /// Calls a functor (e.g. a lambda) for each point offset in this
    /// primitive, in the order they occur in the vertex list.
    /// This is optimized to only check triviality once and only look up
    /// the vertex-to-point topology attribute once.
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachPoint(FUNCTOR &&functor) const
    {
        const GA_ATITopology *vtx_to_pt = myDetail->getTopology().getPointRef();
        myVertexList.forEach([vtx_to_pt,&functor](GA_Offset vtx){
            functor(vtx_to_pt->getLink(vtx));
        });
    }

    bool vertexApply(bool(*apply)(GA_Offset vtx, void *),
        void *data = nullptr) const
    {
        for (GA_Size i = 0, n = getVertexCount(); i < n; ++i)
        {
            if (apply(getVertexOffset(i), data))
                return true;
        }
        return false;
    }

    /// Stash (deactivate) or unstash (reactivate) the primitive.
    ///
    /// Stashing a primitive will set its offset to that specified, typically
    /// GA_INVALID_OFFSET.  Primitives are only stashed as part of clearing a
    /// detail, so there is no need to explicitly deallocate vertices.  It is
    /// safe to delete a stashed primitive.
    ///
    /// Unstashing a primitive will re-register that primitive with its parent
    /// detail at the specified offset (or a new offset if GA_INVALID_OFFSET).
    virtual void	 stashed(bool beingstashed,
					GA_Offset offset = GA_INVALID_OFFSET);

    /// Copy any modified caches from the GPU back to CPU cache.
    virtual void	 flushCEWriteCaches() {}

    /// Copy any modified caches from the GPU to CPU.  Then delete the GPU
    /// cache.
    virtual void	 flushCECaches() {}

    static SYS_FORCE_INLINE
    GA_PrimCompat::TypeMask
    primCompatMaskFromTypeId(int type_id) 
    {
	GA_PrimCompat::value_type m = 0;
	if (type_id < GA_PrimCompat::TypeMaskBits)
	    m = (GA_PrimCompat::value_type)1 << type_id;
	return GA_PrimCompat::TypeMask(m);
    }

protected:
    void setNumVertices(GA_Size nvertices);

    /// allocateVertex() will call wireVertex() if the point given is not -1
    GA_Offset	 allocateVertex(GA_Offset point=GA_INVALID_OFFSET);
    void	 destroyVertex(GA_Offset vertex);
    void	 wireVertex(GA_Offset vertex, GA_Offset point);

    /// Create the topological binding between preallocated vertex and
    /// primitive.
    void	 registerVertex(GA_Offset vertex);

    /// Report approximate memory usage for myVertexList for subclasses.
    SYS_FORCE_INLINE
    int64 getBaseMemoryUsage() const
    {
        return myVertexList.getMemoryUsage(false);
    }

    /// This is called by the subclasses to count the
    /// memory used by myVertexList
    void countBaseMemory(UT_MemoryCounter &counter) const;

    /// This is the implementation of this level of copyUnwiredForMerge,
    /// and enables GA_PrimitiveList to set the vertex list without a
    /// source primitive.
    void copyVertexListForMerge(
        const GA_OffsetListRef &src_vertices, const GA_MergeMap &map);

    /// @{
    /// Implementation of intrinsic attributes.  See GA_IntrinsicEval
    /// for further details.  See GA_IntrinsicMacros.h for implementation of
    /// intrinsics.
    /// @note When implementing support for string intrinsic attributes, please
    /// ensure to implement for @b all methods (localGetIntrinsicS,
    /// localGetIntrinsicSA for reading and localSetIntrinsicSA,
    /// localSetIntrinsicSS for writing).  You should do this even if the
    /// string tuple size is 1.
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)
    /// @}

private:

    SYS_FORCE_INLINE
    void unstashImpl(GA_Offset offset)
    {
        UT_ASSERT_P(myVertexList.size() == 0);
        myOffset = offset;
    }

    friend class GA_PrimitiveList;

    GA_Detail *myDetail;
    GA_Offset myOffset;
protected:
    GA_OffsetList myVertexList;
};

SYS_FORCE_INLINE
GA_OffsetListRef
GA_Detail::getPrimitiveVertexList(GA_Offset primoff) const
{
    return myPrimitiveList.getVertexList(primoff);
}

SYS_FORCE_INLINE
GA_Size
GA_Detail::getPrimitiveVertexCount(GA_Offset primoff) const
{
    return myPrimitiveList.getVertexCount(primoff);
}

SYS_FORCE_INLINE
GA_Offset
GA_Detail::getPrimitiveVertexOffset(GA_Offset primoff, GA_Size i) const
{
    return myPrimitiveList.getVertexOffset(primoff, i);
}

SYS_FORCE_INLINE
int
GA_Detail::getPrimitiveTypeId(GA_Offset primoff) const
{
    return myPrimitiveList.getTypeId(primoff);
}

SYS_FORCE_INLINE
bool
GA_Detail::getPrimitiveClosedFlag(GA_Offset primoff) const
{
    return myPrimitiveList.getClosedFlag(primoff);
}

SYS_FORCE_INLINE
void
GA_Detail::setPrimitiveClosedFlag(GA_Offset primoff, bool closed)
{
    myPrimitiveList.setClosedFlag(primoff, closed);
}

#if COMPRESSED_PRIM_LIST
SYS_FORCE_INLINE
GA_OffsetListRef
GA_PrimitiveList::getVertexList(GA_Offset primoff) const
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    const GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        GA_PageNum pagenum = GAgetPageNum(primoff);
        GA_PageOff pageoff = GAgetPageOff(primoff);
        auto p = myVertexLists.getPageData(pagenum);
        UT_ASSERT_COMPILETIME(2*sizeof(*p) == sizeof(GA_OffsetListRef));
        auto po = reinterpret_cast<const GA_OffsetListRef*>(p);
        if (myVertexLists.isPageConstant(pagenum))
        {
            exint size = po->size();
            return GA_OffsetListRef(po->trivialStart() + size*pageoff, size, po->getExtraFlag());
        }
        else
        {
            return po[pageoff];
        }
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return GA_OffsetListRef(prim_array[primoff]->myVertexList);
    }
}

SYS_FORCE_INLINE
GA_Size
GA_PrimitiveList::getVertexCount(GA_Offset primoff) const
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    const GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        GA_PageNum pagenum = GAgetPageNum(primoff);
        UT_ASSERT_COMPILETIME(2*sizeof(int64) == sizeof(GA_OffsetList));
        auto p = reinterpret_cast<const GA_OffsetList*>(myVertexLists.getPageData(pagenum));
        if (!myVertexLists.isPageConstant(pagenum))
        {
            p += GAgetPageOff(primoff);
        }
        return p->size();
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return prim_array[primoff]->getVertexCount();
    }
}

SYS_FORCE_INLINE
GA_Offset
GA_PrimitiveList::getVertexOffset(GA_Offset primoff, GA_Size i) const
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    const GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        GA_PageNum pagenum = GAgetPageNum(primoff);
        GA_PageOff pageoff = GAgetPageOff(primoff);
        UT_ASSERT_COMPILETIME(2*sizeof(int64) == sizeof(GA_OffsetList));
        auto p = reinterpret_cast<const GA_OffsetList*>(myVertexLists.getPageData(pagenum));
        if (!myVertexLists.isPageConstant(pagenum))
        {
            return p[pageoff].get(i);
        }
        return p->trivialStart() + p->size()*pageoff + i;
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return prim_array[primoff]->getVertexOffset(i);
    }
}

SYS_FORCE_INLINE
int
GA_PrimitiveList::getTypeId(GA_Offset primoff) const
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    const GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        return myPrimitiveTypes.get(primoff);
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return prim_array[primoff]->getTypeId().get();
    }
}

SYS_FORCE_INLINE
bool
GA_PrimitiveList::getClosedFlag(GA_Offset primoff) const
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    const GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        GA_PageNum pagenum = GAgetPageNum(primoff);
        auto p = myVertexLists.getPageData(pagenum);
        if (!myVertexLists.isPageConstant(pagenum))
        {
            p += 2*GAgetPageOff(primoff);
        }
        UT_ASSERT_COMPILETIME(2*sizeof(*p) == sizeof(GA_OffsetList));
        return reinterpret_cast<const GA_OffsetList*>(p)->getExtraFlag();
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return prim_array[primoff]->myVertexList.getExtraFlag();
    }
}

SYS_FORCE_INLINE
void
GA_PrimitiveList::setClosedFlag(GA_Offset primoff, bool closed)
{
    UT_ASSERT_P(!myDetail.getPrimitiveMap().isOffsetVacant(primoff));
    GA_Primitive *const*prim_array = myList;
    if (!prim_array)
    {
        GA_PageNum pagenum = GAgetPageNum(primoff);
        auto p = hardenVertexListPage(pagenum);
        GA_PageOff pageoff = GAgetPageOff(primoff);
        return p[pageoff].setExtraFlag(closed);
    }
    else
    {
        UT_ASSERT_P(GAisValid(primoff) && primoff < GA_Offset(getMyListSize()));
        return prim_array[primoff]->myVertexList.setExtraFlag(closed);
    }
}
#else
SYS_FORCE_INLINE
GA_OffsetListRef
GA_PrimitiveList::getVertexList(GA_Offset off) const
{
    return GA_OffsetListRef(myList[off]->myVertexList);
}

SYS_FORCE_INLINE
GA_Size
GA_PrimitiveList::getVertexCount(GA_Offset off) const
{
    return myList[off]->getVertexCount();
}

SYS_FORCE_INLINE
GA_Offset
GA_PrimitiveList::getVertexOffset(GA_Offset primoff, GA_Size i) const
{
    return myList[primoff]->getVertexOffset(i);
}

SYS_FORCE_INLINE
int
GA_PrimitiveList::getTypeId(GA_Offset off) const
{
    return myList[off]->getTypeId().get();
}

SYS_FORCE_INLINE
void
GA_PrimitiveList::getVertexList(GA_Offset off, GA_OffsetListRef &list) const
{
    list = myList[off]->myVertexList;
}

SYS_FORCE_INLINE
bool
GA_PrimitiveList::getClosedFlag(GA_Offset off) const
{
    return myList[off]->myVertexList.getExtraFlag();
}

SYS_FORCE_INLINE
void
GA_PrimitiveList::setClosedFlag(GA_Offset primoff, bool closed)
{
    return myList[primoff]->myVertexList.setExtraFlag(closed);
}
#endif

#endif
