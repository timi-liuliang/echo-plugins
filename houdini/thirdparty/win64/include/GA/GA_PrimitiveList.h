/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveList.h (GA Library, C++)
 *
 * COMMENTS:	The list of a detail's primitives.
 */

#pragma once

#ifndef __GA_PrimitiveList__
#define __GA_PrimitiveList__

#define COMPRESSED_PRIM_LIST 1

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_PageArray.h"
#include "GA_PrimitiveTracker.h"
#include "GA_PrimitiveTypeId.h"
#include "GA_Types.h"
#include <UT/UT_Array.h>
#if COMPRESSED_PRIM_LIST
#include <UT/UT_TaskExclusive.h>
#endif
#include <SYS/SYS_Inline.h>

class GA_Defragment;
class GA_Detail;
struct GA_JSONUniformBlock;
class GA_LoadMap;
class GA_MergeMap;
class GA_PolyCounts;
class GA_Primitive;
class GA_PrimitiveDefinition;
class GA_SaveMap;

class UT_StringRef;
class UT_JSONParser;
class UT_JSONValue;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_StringArray;

/// @brief A list of primitives
///
/// Primitives for a detail are stored in a primitive list.  The list stores
/// pointers to primitive objects.
/// The list is ordered in the same order as the primitive attribute lists.
class GA_API GA_PrimitiveList
{
public:
    GA_PrimitiveList(GA_Detail &detail);

    SYS_FORCE_INLINE
    ~GA_PrimitiveList()
    {
        clear(true);
    }

    SYS_FORCE_INLINE
    const GA_Primitive *get(GA_Offset off) const
    {
        const GA_Primitive *const*prim_array = myList;
#if COMPRESSED_PRIM_LIST
        if (!prim_array)
            prim_array = hardenPrimitives();
#endif
        UT_ASSERT_P(GAisValid(off) && off < GA_Offset(getMyListSize()));
        return prim_array[off];
    }
    SYS_FORCE_INLINE
    GA_Primitive *get(GA_Offset off)
    {
        GA_Primitive *const*prim_array = myList;
#if COMPRESSED_PRIM_LIST
        if (!prim_array)
            prim_array = hardenPrimitives();
#endif
        UT_ASSERT_P(GAisValid(off) && off < GA_Offset(getMyListSize()));
        return prim_array[off];
    }

    /// Returns a shallow copy of the vertex list of the primitive at the
    /// specified primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_OffsetListRef getVertexList(GA_Offset primoff) const;

    /// Returns the number of vertices in the primitive at the specified
    /// primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_Size getVertexCount(GA_Offset primoff) const;

    /// Returns the vertex offset in the primitive at the specified
    /// primitive offset in vertex list position i.
    /// If you're accessing all vertex offsets, it'll be faster to
    /// call getVertexList() once and either read from that or
    /// call forEach(fucntor) on it.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    GA_Offset getVertexOffset(GA_Offset primoff, GA_Size i) const;

    /// Returns the type ID of the primitive at the specified
    /// primitive offset.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    int getTypeId(GA_Offset primoff) const;

    /// Returns the "closed" flag for polygon, NURBS curve, or Bezier curve
    /// primitives, or false for other primitive types.
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    bool getClosedFlag(GA_Offset primoff) const;

    /// Sets the "closed" flag for polygon primitives.
    /// For NURBS curve or Bezier curve primitives, use GEO_Face::close()
    /// or GEO_Face::open().
    /// You must include GA_Primitive.h to use this.
    SYS_FORCE_INLINE
    void setClosedFlag(GA_Offset primoff, bool closed);

    /// Report on memory usage
    int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void	countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Offset size of the list
    GA_Offset offsetSize() const
    {
#if COMPRESSED_PRIM_LIST
        return GA_Offset(myList ? getMyListSize() : myVertexLists.size());
#else
        return GA_Offset(getMyListSize());
#endif
    }

    /// Set capacity of primitive offsets in the list
    void	adjustArraySize(GA_Offset size);
    void	clear(bool for_deletion);

    void stashAll();
    void destroyStashed();

    /// Returns true iff the primitive list representation is as
    /// separate allocations for each primitive, instead of using the
    /// paged array format.
    bool isFullRepresentation() const
    {
        return myList != nullptr;
    }

    /// Returns true iff the specified vertex list page is constant.
    bool isVertexListPageConstant(GA_PageNum pagenum) const
    {
        return !myList && myVertexLists.isPageConstant(UT_PageNum(pagenum));
    }

    void	merge(GA_Detail &dgeo, const GA_MergeMap &map);

    /// NOTE: Since the primitive maps of dstgeo and srcgeo must have already
    ///       been made equal by the time this is called.
    void        replace(GA_Detail &dstgeo, const GA_Detail &srcgeo);

    /// Called by GA_Detail::defragment when defragmenting the primitive
    /// index map.
    /// NOTE: This bumps the primitive list data ID.
    void	defragment(const GA_Defragment &defrag);

    /// Called by GA_Detail::defragment when defragmenting the vertex
    /// index map.
    /// NOTE: This bumps the primitive list data ID.
    void        defragmentVertices(const GA_Defragment &defrag);

    /// Copy any modified caches from the GPU back to CPU cache.
    void	 flushCEWriteCaches(const GA_PrimitiveFactory &f);

    /// Copy any modified caches from the GPU to CPU.  Then delete the GPU
    /// cache.
    void	 flushCECaches(const GA_PrimitiveFactory &f);

    /// Save data to a JSON stream.
    /// @section JSON-GA_PrimitiveList JSON Schema: GA_PrimitiveList
    /// @code
    /// {
    ///     "name"          : "GA_PrimitiveList",
    ///     "description"   : "An array of primitive objects",
    ///     "type"          : "array",
    ///     "items"         : { "$ref":"GA_Primitive" },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat, GA_Primitive
    bool	jsonSave(UT_JSONWriter &w, const GA_SaveMap &save) const;

    /// Load from a JSON stream
    /// @see @ref JSON_FileFormat
    bool	jsonLoad(UT_JSONParser &p, GA_LoadMap &load, GA_Offset &load_end);

    /// Check to see whether the primitive list contains primitives of a given
    /// type
    bool	containsType(const GA_PrimitiveTypeId &type) const
		    { return myTracker.contains(type); }
    /// Return the number of primitives of the given type
    GA_Size	countType(const GA_PrimitiveTypeId &type) const
		    { return myTracker.count(type); }
    /// Test whether the primitive list contains any primitives that have a
    /// local transform.
    bool	hasLocalTransform(const GA_PrimitiveFactory &factory) const
		    { return myTracker.hasLocalTransform(factory); }
    /// Return the primitives of the given type. Unlike the functions
    /// above which use myTracker, this function requires iterating through
    /// all primitives.
    bool	getPrimitivesOfType(const GA_PrimitiveTypeId &type,
			UT_Array<const GA_Primitive *> &prims) const;

    /// Use this to mark primitives or their intrinsic data as dirty.
    void	bumpDataId();
    GA_DataId	getDataId() const
		{ return myDataID; }

    const GA_PrimitiveTracker	&getTracker() const
				 { return myTracker; }

    /// Goes through the primitive list to confirm that it's in a consistent
    /// state, returning true if it is, and false if it isn't.
    /// This is only for debugging purposes.
    bool validate(const GA_Detail &detail) const;

    // NOTE: These functor classes must be in the scope of GA_PrimitiveList
    //       so that it can access GA_Primitive::unstashImpl,
    //       GA_PrimitiveDefinition::getConstructor, and
    //       GA_PrimitiveDefinition::getBlockConstructor.
    //       They have to be public so that UTparallelReduce
    //       can be instantiated for GCC with it.
    class ga_ParallelCreateForMerge;
    class ga_ParallelCreatePagedForMerge;
    class ga_ReplacePrimitiveBlockParallel;

private:
    void create(GA_Detail &detail,
        const GA_PrimitiveDefinition &def,
        GA_Offset offset,
        bool check_stash);
    void createBlock(GA_Detail &detail,
        const GA_PrimitiveDefinition &def,
        GA_Size nprimitives,
        GA_Offset offset,
        bool check_stash);
    void initVertexLists(
        GA_Offset prim_block_start,
        GA_Offset vertex_block_start,
        GA_Size nprimitives,
        GA_Size nvertices_each,
        bool closed = false);
    void initVertexLists(
        GA_Offset prim_block_start,
        GA_Offset vertex_block_start,
        const GA_PolyCounts &vertex_counts,
        bool closed = false);

    /// To be called from GA_Detail::sortVertexMapByPrimitiveUse()
    void sortVertexMapByPrimitiveUse(GA_IndexMap &vertex_map);

    /// @{
    /// This class is non-copyable
    GA_PrimitiveList(const GA_PrimitiveList &);
    const GA_PrimitiveList &operator=(const GA_PrimitiveList &);
    /// @}

    bool loadPolyOrTetRun(
        UT_JSONParser &p,
        int runenum,
        GA_Offset &primoff,
        GA_Offset init_vtxoff,
        GA_Detail &detail);
    bool createLoadPrimitive(UT_JSONParser &p,
        GA_LoadMap &load,
        const UT_StringRef &type,
        GA_Offset &load_offset,
        const UT_StringArray *varying,
        const GA_JSONUniformBlock &uniform);

    SYS_FORCE_INLINE
    exint getMyListSize() const
    {
        return getMyPrimPtrSize(myList);
    }
    SYS_FORCE_INLINE
    exint getMyStashedSize() const
    {
        return getMyPrimPtrSize(myStashedPrims);
    }
    SYS_FORCE_INLINE
    static exint getMyPrimPtrSize(GA_Primitive**p)
    {
        UT_ASSERT_P(p);
        return ((const exint*)p)[-1];
    }
    void reallocMyList(exint newsize);
    SYS_FORCE_INLINE
    void freeMyList();

#if COMPRESSED_PRIM_LIST
    GA_OffsetList *hardenVertexListPage(GA_PageNum pagenum);
    void hardenAllVertexListPages();
    void clearVertexLists();
#endif
    void destroy(GA_Offset off);

#if COMPRESSED_PRIM_LIST
    GA_Primitive *const*hardenPrimitives() const;
    /// This can be faster than hardenPrimitives, if we're calling from a
    /// thread-unsafe function already.
    GA_Primitive *const*hardenPrimitivesThreadUnsafe();
    SYS_FORCE_INLINE
    GA_Primitive *const*hardenPrimitivesImpl(bool parallel) const;
    class HardenPrimitivesFunctor;

    void dump() const;
#endif

    friend class GA_Detail;

    /// This is nullptr when using myVertexLists and myPrimitiveTypes.
    /// It has an exint indicating the size immediately before
    /// where it points in memory.
    mutable GA_Primitive **myList;
#if COMPRESSED_PRIM_LIST
    GA_Detail &myDetail;
    GA_PageArray<int,1,false,false> myPrimitiveTypes;
    GA_PageArray<int64,2,false,false> myVertexLists;
    mutable UT_TaskExclusive<HardenPrimitivesFunctor> myHardenExclusive;
#endif
    GA_PrimitiveTracker		myTracker;
    GA_DataId                   myDataID;

    /// Array of temporarily stashed primitives.
    /// This is only used to reduce overhead of freeing and
    /// reallocating primitives if it's likely that the
    /// detail is going to be rebuilt with the same primitive
    /// types in the same order as the previous primitives.
    /// It has an exint indicating the size immediately before
    /// where it points in memory.
    mutable GA_Primitive **myStashedPrims;
};

#endif
