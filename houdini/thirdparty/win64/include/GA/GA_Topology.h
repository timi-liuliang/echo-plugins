/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Topology.h ( GA Library, C++)
 *
 * COMMENTS:	This class keeps track of the topological attributes attached
 *		to a piece of geometry.
 */

#ifndef __GA_Topology__
#define __GA_Topology__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_ElementGroup.h"
#include "GA_ATITopology.h"

class GA_Detail;
class GA_MergeMap;
class GA_Attribute;
class GA_ATITopology;
class UT_JSONWriter;
class UT_JSONParser;
class GA_SaveMap;
class GA_LoadMap;
class GA_Defragment;
class UT_MemoryCounter;

class GA_API GA_Topology
{
public:
     GA_Topology(GA_Detail &detail);
    ~GA_Topology();

    // We store topology as 16 bit values for small geometry.  As the geometry
    // grows, we switch to 32 bit values, eventually switching to 64 bit values if
    // the geometry grows very large.  We do this by walking up and down through
    // this table.

    class ga_StorageSwitch
    {
    public:
	ga_StorageSwitch(GA_Storage store, GA_Size lo_water, GA_Size hi_water) :
	    myStorage(store), myLowWater(lo_water), myHighWater(hi_water)
	{
	}

	bool downShift(GA_Size size) const
	{
	    return size <= myLowWater;
	}
	bool upShift(GA_Size size) const
	{
	    return size >= myHighWater;
	}
	GA_Storage getStorage() const
	{
	    return myStorage;
	}

    private:
	GA_Storage myStorage;
	GA_Size myLowWater;
	GA_Size myHighWater;
    };

    #define NUM_STORAGE_LEVELS 3

    static inline int
    computeStorageDownShift(int storage_level, GA_Size size)
    {
	int change = 0;
	while (storage_level > 0
	       && theStorageLevels[storage_level].downShift(size))
	{
	    --change;
	    --storage_level;
	}

	return change;
    }

    static inline int
    computeStorageUpShift(int storage_level, GA_Size size)
    {
	int change = 0;
	while (storage_level < (NUM_STORAGE_LEVELS - 1)
	       && theStorageLevels[storage_level].upShift(size))
	{
	    ++change;
	    ++storage_level;
	}

	return change;
    }

    static inline GA_Storage
    getStorage(int level)
    {
	return theStorageLevels[level].getStorage();
    }

    /// Return the data ID for the topology attributes.
    GA_DataId	getDataId() const;


    /// Test whether all required attributes built
    bool	isFull() const;

    /// This method constructs the topology attributes for the detail.  It
    /// should only be called by GA_Detail.  Please use rebuildTopology() from
    /// other code.
    void	makeFull()	// Build the topological attributes
		{
		    if (!myPointRef)
			makePointRef();
		    if (!myPrimitiveRef)
			makePrimitiveRef();
		    if (!myVertexRef || !myVertexPrevRef || !myVertexNextRef)
			makeVertexRef();
		}

    /// Re-create the topology links.  This method will re-create all the
    /// topological information.  If the <tt>rebuild_topology</tt> option is
    /// true, then any missing attributes will be created. If support_hedge is
    /// true, then half-edge topology attributes are constructed.
    void	rebuildTopology(bool rebuild_topology=true,
        	                bool support_hedge = false);

    /// makeMinimal() deletes the non-essential topology attributes.  All
    /// optional attributes (the options which allow reverse lookup of
    /// elements) are deleted.  The attribute to map vertex to point
    /// (myPointRef) remains untouched.
    void	makeMinimal();

    /// The topology attributes try to store minimal amounts of information.
    /// That is, if there are only 4 points in the geometry, we won't store the
    /// indices as 64-bit integers.  This method will change the storage
    /// attributes if needed.
    bool	checkStorageChange();

    /// @{
    /// Called by GA_Detail::defragment when defragmenting offset lists
    ///
    /// NOTE: defragmentLinks will bump the data ID of myPointRef,
    ///       myVertexRef, or myPrimitiveRef if any of those are
    ///       modified, because they are modified when defragmenting
    ///       an index map that is different from their owner,
    ///       i.e. their *link* owner's index map.
    ///       defragmentElements does NOT bump any data IDs.
    void	defragmentElements(const GA_Defragment &defrag);
    void	defragmentLinks(const GA_Defragment &defrag);
    /// @}

    /// Functions to maintain topological information
    void	addVertex(GA_Offset vtx)
    { return addVertexBlock(vtx, 1); }
    void	addPoint(GA_Offset pt)
    { return addPointBlock(pt, 1); }
    void	addVertexBlock(GA_Offset vtx, GA_Size nvertices);
    void	addPointBlock(GA_Offset pt, GA_Size npoints);
    void	delPoint(GA_Offset pt);
    void	delVertex(GA_Offset vtx);

    /// @note When hedge attributes are present wireVertexPoint has to
    /// try to fix the changes to hedge equivalence classes that take place
    /// when a vertex is wired to a new point, namely, the two hedges
    /// incident to the given vertex must be unlinked from their previous
    /// equivalence classes and must be linked into respective new ones.
    /// This means that a search on the incident primitives of the new
    /// point must be carried out to determine the new associations. This
    /// is where having hedges costs the most. If the caller has access to
    /// the classes to other hedges from the classes into which the affected
    /// hedges must be linked, it must call wireVertexPointAndLinkHedges
    /// which runs in constant time.
    /// Alternatively, if the caller knows that no hedge information
    /// needs to be changed, wireVertexPointAndIgnoreHedges can be called.
    /// @see wireVertexPointAndRelinkHedges
    /// @see wireVertexPointAndIgnoreHedges
    void	wireVertexPoint(GA_Offset vtx, GA_Offset pt);

    /// Wires vertex to the point and links the the hedges whose head
    /// and tail vertices are vtx into prev_into_src_vtx and next_into_src_vtx
    /// respectively.
    ///
    /// @note This method is used to avoid the search that is performed by
    /// the standard wireVertexPoint to *find* hedges equivalent to those
    /// incident to vtx. It's useful when these hedges are known in advance.
    void 	wireVertexPointAndRelinkHedges(GA_Offset vtx, GA_Offset pt,
						GA_Offset prev_into_src_vtx,
						GA_Offset next_into_src_vtx);

    /// Wires vertex to the point and completely ignores the affected hedges.
    ///
    /// This is equivalent to wireVertexPoint before hedges were introduced,
    /// or when hedge attributes do not exist. Note that if hedge attributes
    /// are present, this should be called at the risk of violating hedge
    /// invariants. It is useful, for example, if all the vertices wired to a
    /// point are being wired to another point and therefore there is no need
    /// to update hedge attributes.
    void	wireVertexPointAndIgnoreHedges(GA_Offset vtx, GA_Offset pt);

    void	wireVertexPrimitive(GA_Offset vtx, GA_Offset prim);

    /// Test to see whether a point is used
    bool	isPointUsed(GA_Offset pt) const;
    /// Test to see whether a point is used by more than one vertex
    bool	isPointShared(GA_Offset pt) const;

    /// Make the attribute to store vertex->point reference (mandatory)
    void	makePointRef();
    /// Make the attribute to store vertex->primitive reference (optional)
    void	makePrimitiveRef();
    /// Make the attributes to store point->vertex references (optional)
    /// Note:  This method will create 3 attributes
    void	makeVertexRef();

    SYS_FORCE_INLINE
    const GA_ATITopology *getPointRef() const      { return myPointRef; }
    SYS_FORCE_INLINE
    const GA_ATITopology *getPrimitiveRef() const  { return myPrimitiveRef; }
    SYS_FORCE_INLINE
    const GA_ATITopology *getVertexRef() const     { return myVertexRef; }
    SYS_FORCE_INLINE
    const GA_ATITopology *getVertexPrevRef() const { return myVertexPrevRef; }
    SYS_FORCE_INLINE
    const GA_ATITopology *getVertexNextRef() const { return myVertexNextRef; }
    SYS_FORCE_INLINE
    GA_ATITopology *getPointRef()                  { return myPointRef; }
    SYS_FORCE_INLINE
    GA_ATITopology *getPrimitiveRef()              { return myPrimitiveRef; }
    SYS_FORCE_INLINE
    GA_ATITopology *getVertexRef()                 { return myVertexRef; }
    SYS_FORCE_INLINE
    GA_ATITopology *getVertexPrevRef()             { return myVertexPrevRef; }
    SYS_FORCE_INLINE
    GA_ATITopology *getVertexNextRef()             { return myVertexNextRef; }

    bool	jsonSavePointRef(UT_JSONWriter &w, const GA_SaveMap &s) const;
    bool	jsonLoadPointRef(UT_JSONParser &p, const GA_LoadMap &s);

    /// Make any changes to the topology attributes to accommodate the new data
    /// (i.e. change their sizes if need be).  This method is called before the
    /// merge begins.
    void	mergeRebuild(const GA_MergeMap &map);
    void	mergeGrowCheckStorageChange(GA_Size sz, bool cloned_data_id);

    /// Verify that the topologies are constructed correctly
    /// This is just for debugging purposes, and so may call printf
    bool        validate() const;

    /// Returns the next and previous boundary vertices of a given vertex in
    /// its primitive.
    /// @param vtx input vertex
    /// @param prev_vtx reference into which previous boundary vertex of
    ///        vtx will be written
    /// @param next_vtx reference into which next boundary vertex of vtx
    ///        will be written
    void	getAdjacentBoundaryVertices(GA_Offset vtx,
        	                            GA_Offset &prev_vtx,
        	                            GA_Offset &next_vtx) const;

    /// Returns true if hedge topology is maintained
    ///
    /// A *hedge* (short for half-edge) embodies the notion of an edge
    /// localised to a primitive incident to it. An edge incident to k
    /// primitives therefore leads to k distinct hedges, one belonging to
    /// each of the primitives. Unlike edges, hedges are oriented and therefore
    /// have distinct *tail* and *head* vertices. The points wired to the tail
    /// and head vertices of a hedge are respectively called its tail and head
    /// points and together make up the *endpoints* of the given hedge. Two
    /// hedges are called *equivalent* if and only if their endpoints (as sets)
    /// coincide, i.e. if and only if either they both have the same tail point
    /// and the same head point, or the head point of one is the same as the
    /// tail point of another and vice versa. Equivalence of hedges puts them
    /// into *equivalence classes*.
    bool 	isHedgeFull() const
		    { return (myHedgePrevRef != 0); }

    /// Creates hedge topology attributes if they don't already exist
    /// @see createHedgeTopologyLinks
    /// @see rebuildHedgeTopologyLinks
    void	makeHedgeFull()
    		{
    		    if (!myHedgePrevRef || !myHedgeNextRef || !mySecondaryHedges)
    			makeHedgeRef();
    		}

    /// Creates the hedge topology attributes
    ///
    /// @note This method will create 2 vertex topology attributes and 1 vertex group
    void	makeHedgeRef();

    /// Creates hedge topology attributes and computes them for the existing detail
    void	createHedgeTopologyLinks();

    /// Destroys the hedge topology attributes
    void	destroyHedgeTopologyLinks();

    /// Return true if the argument is the tail vertex of a "valid" hedge.
    ///
    /// A hedge is *valid* if and only if (1) it belongs to a primitive that
    /// supports hedges and (2) the primitive returns a non-negative vertex
    /// offset for its next boundary vertex. A valid hedge is *singleton* if
    /// it is not linked up with any other hedges into an equivalence class.
    /// NOTE 1: The validity of a hedge has *nothing* to do with its vertices
    /// being wired to points.
    /// NOTE 2: All valid hedges with unwired vertices are singletons.
    /// The next and previous hedge references of an invalid hedge must be
    /// GA_INVALID_OFFSET and those of a singleton hedge must be the hedge
    /// itself. This will be how we distinguish valid hedges from invalid ones.
    bool	isValidHedge(GA_Offset src_vtx) const;

    /// Returns true if the input hedge is a *primary* hedge.
    /// @note This method doesn't check for validity of the hedge. It returns
    /// true as long as the vertex is not marked as being secondary to another
    /// hedge.
    ///
    /// Each equivalence class of hedges (linked into a circular linked list
    /// using myHedgePrevRef and myHedgeNextRef) must have exactly one *primary*
    /// hedge. All other hedges in the class are marked secondary by being
    /// added to the group mySecondaryHedges. This is an invariant maintained
    /// at all times.
    /// @note The primary hedge of a class is elected arbitrarily. It stays
    /// primary until it is unlinked from the class, at which time another hedge
    /// becomes the primary hedge in that class. A singleton hedge is the
    /// primary of its own class.
    bool	isPrimaryHedge(GA_Offset src_vtx) const
		    { return !mySecondaryHedges->containsOffset(src_vtx); }

    /// Returns the previous hedge in the equivalence class of the given hedge.
    ///
    /// The equivalence class of a hedge consists of all hedges whose two
    /// endpoints coincide with those of the hedge in question (although they
    /// may be reversely paired). By *endpoints* of a hedge we mean the two
    /// points into which the tail and head vertex of a hedge are respectively
    /// wired.
    /// @note Starting with any valid hedge, repeated calls to hedgeToPrevHedge
    /// or hedgeToNextHedge traverses all hedges to the original one and
    /// ultimately wraps around.
    GA_Offset 	hedgeToPrevHedge(GA_Offset src_vtx) const
		    { return myHedgePrevRef->getLink(src_vtx); }

    /// Returns the next hedge in the equivalence class of the given hedge.
    /// @see hedgeToPrevHedge
    GA_Offset   hedgeToNextHedge(GA_Offset src_vtx) const
		    { return myHedgeNextRef->getLink(src_vtx); }

    /// returns the primary hedge in the equivalence class of a given hedge
    GA_Offset 	hedgeToPrimaryHedge(GA_Offset vtx);

    /// Same as linkHedge, but first unlinks hedge before calling linkHedge.
    /// @see linkHedge
    void	relinkHedge(GA_Offset src_vtx,
        	            GA_Offset dst_vtx,
        	            GA_Offset into_src_vtx = GA_INVALID_OFFSET);

    /// This is equivalent to calling relinkHedge on both of the hedges that
    /// are incident to a vtx.
    void	relinkVertexHedges(GA_Offset vtx);

    /// This is equivalent to calling relinkHedge on both of the hedges that
    /// are incident to a vtx. Instead of searching for equivalent hedges to
    /// link into, it respectively links the the hedges whose head and tail
    /// vertices are vtx into prev_into_src_vtx and next_into_src_vtx.
    void	relinkVertexHedges(GA_Offset vtx,
        	                   GA_Offset prev_into_src_vtx,
        	                   GA_Offset next_into_src_vtx);

    /// Tosses out existing topology links and rebuilds the whole thing from
    /// scratch.
    /// @see createHedgeTopologyLinks
    void	rebuildHedgeTopologyLinks();

    /// This counts memory using the given UT_MemoryCounter.
    /// NOTE: GA_Topology doesn't own any of the memory it points to,
    ///       so there's not much to count.
    void        countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Returns the current storage level as an int.
    /// Call GA_Topology::getStorage(topology.getStorageLevel())
    /// to get the GA_Storage.
    int         getStorageLevel() const
    { return myStorageLevel; }

private:

    /// Unlinks a hedge from the circular linked list that represents its
    /// equivalence class.
    ///
    /// @note We always want to maintain the invariant that at any given
    /// time each hedge be linked to other hedges in its equivalence class.
    /// Therefore, unlinking should only be used either right ahead of
    /// relinking into another class or when a hedge turns into a singleton
    /// class of its own.
    /// @note This method must not be called if hedge topology attributes are
    /// not created
    /// @see isHedgeFull
    void	unlinkHedge(GA_Offset tailv);


    /// Links a hedge into the equivalence class of hedges that share its
    /// endpoints.
    /// @param src_vtx The tail vertex of the hedge to be linked
    /// @param dst_vtx The head vertex of the hedge to be linked. This is
    /// made mandatory even though it can be found by querying the primitive
    /// because it has to be done anyway and most often the caller has it
    /// figured out before calling this method. dst_vtx equal to -1 makes
    /// the hedge invalid.
    /// @param into_src_vtx (Optional) the tail vertex of an already known
    /// hedge from the equivalence class. If specified, no searching takes
    /// place and the given hedge is simply linked into the class of the hedge
    /// determined by into_src_vtx. Note that no checks are made to ensure
    /// that the hedge truly belongs to the class. If into_src_vtx is not
    /// specified, we search for a hedge in the target equivalence class. The
    /// running time in this case is at least proportional to the number of
    /// vertices wired to the same point as src_vtx.
    ///
    /// @note No unlinking of any hedges is applied before linking them up
    /// into new ones. The caller must make sure the hedge to be linked is
    /// already unlinked from any class into which it might have been
    /// previously linked.
    ///
    /// @note It is OK if the hedge determined by src_vtx or the one
    /// determined by into_src_vtx are invalid (have negative previous
    /// and next hedge references) although this generally should be avoided.
    /// If invalid, they are first made valid before linking them into each
    /// other.
    void	linkHedge(GA_Offset src_vtx,
        	          GA_Offset dst_vtx,
        	          GA_Offset into_src_vtx = GA_INVALID_OFFSET);

    /// Called by defragmentation
    void	swapElements(GA_AttributeOwner owner,
			    GA_Offset a, GA_Offset b, GA_Size n);
    void	moveElements(GA_AttributeOwner owner,
			    GA_Offset a, GA_Offset b, GA_Size n);
    /// NOTE: This will bump the data ID of myPointRef, myVertexRef,
    ///       or myPrimitiveRef if any of those are modified,
    ///       because they are modified when defragmenting
    ///       an index map that is different from their owner,
    ///       i.e. their *link* owner's index map.
    void	swapLinks(GA_AttributeOwner owner, const GA_Defragment &d);
    static void swapTopologyElements(GA_ATITopology *atr,
                            GA_Offset a, GA_Offset b, GA_Size n);
    static void moveTopologyElements(GA_ATITopology *atr,
                            GA_Offset a, GA_Offset b, GA_Size n);
    static void swapTopologyLinks(GA_ATITopology *atr,
                            const GA_Defragment &defrag);
    bool	checkStorageChange(GA_Size nvertex,
				    GA_Size npoints,
				    GA_Size nprimitives);
    void	rebuildTopologyLinks(bool do_prim, bool do_vertex);
    void	mergeResolvePendingVertexRefs(const GA_MergeMap &map);

    void	unlinkVertex(GA_Offset point, GA_Offset vertex);
    void	linkVertex(GA_Offset point, GA_Offset vertex);
    void	shiftStorage(int change, bool cloned_data_id=false); // Lower storage requirements

    // GA_Detail::replace needs access to replaceStorageLevel.
    friend GA_Detail;

    void        replaceStorageLevel(const int storage_level)
    {
        myStorageLevel = storage_level;

        // The topology attributes should already have been replaced.
        UT_IF_ASSERT_P(GA_Storage storage = theStorageLevels[myStorageLevel].getStorage();)
        UT_ASSERT_P(!myPointRef || myPointRef->getStorage() == storage);
        UT_ASSERT_P(!myPrimitiveRef || myPrimitiveRef->getStorage() == storage);
        UT_ASSERT_P(!myVertexRef || myVertexRef->getStorage() == storage);
        UT_ASSERT_P(!myVertexPrevRef || myVertexPrevRef->getStorage() == storage);
        UT_ASSERT_P(!myVertexNextRef || myVertexNextRef->getStorage() == storage);
        UT_ASSERT_P(!myHedgePrevRef || myHedgePrevRef->getStorage() == storage);
        UT_ASSERT_P(!myHedgeNextRef || myHedgeNextRef->getStorage() == storage);
    }

    /// Traversing equivalent hedges without use of hedge topology attributes
    /// @note for validation of hedge topology references
    GA_Offset 	hedgeToNextHedgeSlow(GA_Offset tailvertex) const;


    static const ga_StorageSwitch theStorageLevels[NUM_STORAGE_LEVELS];

    GA_Detail		&myDetail;		// Reference to detail

    GA_ATITopology	*myPointRef;		// Vertex->point edge
    GA_ATITopology	*myPrimitiveRef;	// Vertex->primitive edge
    GA_ATITopology	*myVertexRef;		// Point->vertex edge
    GA_ATITopology	*myVertexPrevRef;	// Vertex->Vertex linked list
    GA_ATITopology	*myVertexNextRef;	// Vertex->Vertex linked list

    GA_ATITopology	*myHedgePrevRef;	// Hedge->Hedge circular linked list
    GA_ATITopology	*myHedgeNextRef;	// Hedge->Hedge circular linked list
    GA_VertexGroup	*mySecondaryHedges;	// secondary (non-primary) hedge marker

    int			 myStorageLevel;	// Storage level
};

#endif
