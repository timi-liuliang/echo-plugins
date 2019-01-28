/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_EdgeGroup.h (GA Library, C++)
 *
 * COMMENTS: 	GA_EdgeGroup is a group container for GA_Edge entities
 * 		and optional primitive references.
 */

#pragma once

#ifndef __GA_EdgeGroup_h__
#define __GA_EdgeGroup_h__

#include "GA_API.h"

#include "GA_Edge.h"
#include "GA_Group.h"
#include "GA_Types.h"

#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_ArrayMap.h>
#include <SYS/SYS_Hash.h>
#include <SYS/SYS_Types.h>

#include <list>
#include <iterator>

class GA_Detail;
class GA_Primitive;
class GA_LoadMap;
class GA_SaveMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;


class GA_API GA_EdgeGroup : public GA_Group
{
private:
    // Only GA_EdgeGroupTable can make non-detached edge groups.
    friend class GA_EdgeGroupTable;

    GA_EdgeGroup(const GA_Detail &gdp,
		 const UT_StringHolder &name,
		 bool internal,
		 bool detached);
public:
    class PrimEdge;
private:
    using EdgeList = std::list<PrimEdge>;
public:
    /// The edge data we store internally
    class PrimEdge
    {
    public:
        PrimEdge() {}
        PrimEdge(const GA_Edge &e, GA_Offset prim = GA_INVALID_OFFSET)
            : myEdge(e)
            , myPrim(prim)
        {}
        PrimEdge(const PrimEdge &e)
            : myEdge(e.myEdge)
            , myPrim(e.myPrim)
        {} 

	bool operator==(const PrimEdge &o) const
	{ return myEdge == o.myEdge && myPrim == o.myPrim; }

	bool operator!=(const PrimEdge &o) const
	{ return myEdge != o.myEdge || myPrim != o.myPrim; }
	
	SYS_HashType hash() const
	{
	    SYS_HashType hash = myEdge.hash();

	    SYShashCombine(hash, myPrim);
	    return hash;
	}
	
	struct Hasher
	{
	    size_t operator()(const PrimEdge &key) const { return key.hash(); }
	};
        struct Clearer
        {
            using PairType = std::pair<PrimEdge,EdgeList::iterator>;
            static SYS_FORCE_INLINE void clear(PairType &v)
            {
                clear(v.first);
                v.second = EdgeList::iterator();
            }
            static SYS_FORCE_INLINE void clear(PrimEdge &v)
            {
                v.myEdge = GA_Edge(GA_INVALID_OFFSET, GA_INVALID_OFFSET);
                v.myPrim = GA_INVALID_OFFSET;
            }
            /// Only need to actually check the key,
            /// even though clear and clearConstruct will clear both.
            static SYS_FORCE_INLINE bool isClear(const PairType &v)
            {
                return isClear(v.first);
            }
            /// An overload for when there's only a key.
            static SYS_FORCE_INLINE bool isClear(const PrimEdge &v)
            {
                return !GAisValid(v.myEdge.p0()) && !GAisValid(v.myEdge.p1()) && !GAisValid(v.myPrim);
            }
            static SYS_FORCE_INLINE void clearConstruct(PairType *p) { clear(*p); }
            static SYS_FORCE_INLINE void clearConstruct(PrimEdge *p) { clear(*p); }

            static const bool clearNeedsDestruction = false;
        };
	
	GA_Edge myEdge;
	GA_Offset myPrim;
    };
    
private:
    using EdgeMap = UT::ArrayMap<PrimEdge, EdgeList::iterator, false, 128, PrimEdge::Clearer, PrimEdge::Hasher>;

protected:
    template<typename T, typename IT>
    class base_iterator
	: public std::iterator<std::bidirectional_iterator_tag, T>
    {
	public:
	    /// Copy constructor, allows constructing a const_iterator
	    /// from a regular iterator.
	    template<typename ET, typename EIT>
	    base_iterator(const base_iterator<ET, EIT> &src)
	    {
		myCurrent = src.myCurrent;
		myEnd = src.myEnd;
	    }
	    
	    /// Assignment operator
	    template<typename ET, typename EIT>
	    const base_iterator	&operator=(const base_iterator<ET, EIT> &src)
	    {
		myCurrent = src.myCurrent;
		myEnd = src.myEnd;
		return *this;
	    }
	    
	    /// Compare this iterator with another and return \c true if they
	    /// both point to the same element.
	    bool	operator==(const base_iterator &cmp) const
			{ return myCurrent == cmp.myCurrent; }
	    
	    /// Compare this iterator with another and return \c true if they
	    /// point to different elements.
	    bool	operator!=(const base_iterator &cmp) const
			{ return myCurrent != cmp.myCurrent;  }
	    
	    /// Pre-increment operator. Advances the iterator to the next
	    /// element in the list.
	    base_iterator &operator++()
			{ ++myCurrent; return *this; }

	    /// Pre-decrement operator. Moves the iterator to the previous
	    /// element in the list.
	    base_iterator &operator--()
				{ --myCurrent; return *this; }
	    
	    bool	atEnd() const
			{ return myCurrent == myEnd; }
	    
	    void	advance()
			{
			    ++myCurrent;
			}

	    T		&operator*() const { return myCurrent->myEdge; }
	    T		*operator->() const { return &myCurrent->myEdge; }
	    T		&getEdge()	{ return myCurrent->myEdge; }
	    GA_Offset    getPrimitive()	{ return myCurrent->myPrim; }

	private:
	    friend class GA_EdgeGroup;
	    
	    base_iterator(IT current, IT end)
		: myCurrent(current), myEnd(end) {}

	    IT			myCurrent;
	    IT			myEnd;
    };
    
public:
    typedef base_iterator<const GA_Edge, EdgeList::iterator> iterator;
    typedef base_iterator<const GA_Edge, EdgeList::const_iterator> const_iterator;
    
    /// Default constructor. 
    GA_EdgeGroup(const GA_Detail &gdp,
		  const char *name="");
    virtual ~GA_EdgeGroup();

    virtual const GA_Detail &getDetail() const { return myGdp; }
    
    /// Report memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Returns an iterator pointing at the beginning of the list of edges
    /// of this edge group. 
    iterator begin() 
    {
        // NOTE: We don't need to harden yet, because the only difference
        //       between iterator and const_iterator is that iterator
        //       can be passed to remove(it).
        return iterator(myData->myEdgeList.begin(), myData->myEdgeList.end());
    }
    
    /// Returns an iterator pointing at the beginning of the list of edges
    /// of this edge group. 
    iterator end() 
    { return iterator(myData->myEdgeList.end(), myData->myEdgeList.end()); }
    
    const_iterator begin() const 
    { return const_iterator(myData->myEdgeList.begin(), myData->myEdgeList.end()); }
    
    const_iterator end() const	
    { return const_iterator(myData->myEdgeList.end(), myData->myEdgeList.end()); }
    
    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Search for an edge matching the input. If the edge was found, it 
    /// returns an iterator pointing at the edge in the list. Otherwise returns
    /// the \c end iterator.
    /// @{
    const_iterator find(const GA_Edge &edge,
                        GA_Offset primoff = GA_INVALID_OFFSET) const;
    const_iterator find(const GA_Edge &edge,
			const GA_Primitive *prim) const;
    /// @}

    /// Search for an edge matching the input. If the edge was found, it 
    /// returns an iterator pointing at the edge in the list. Otherwise returns
    /// the \c end iterator.
    /// @{
    iterator     find(const GA_Edge &edge,
                      GA_Offset primoff = GA_INVALID_OFFSET);
    iterator 	 find(const GA_Edge &edge,
		      const GA_Primitive *prim);
    /// @}

    /// Returns \c true if an edge exists in this group, matching the input 
    /// data.
    /// @{
    bool         contains(const GA_Edge &edge,
                          GA_Offset primoff = GA_INVALID_OFFSET) const;
    bool 	 contains(const GA_Edge &edge,
			  const GA_Primitive *prim) const;
    /// @}

    /// Add an edge to this group. Returns \c true if the edge was added,
    /// and \c false if the edge already existed.
    /// @{
    bool         add(const GA_Edge &edge, 
                     GA_Offset primoff = GA_INVALID_OFFSET);
    bool         add(GA_Offset p0, GA_Offset p1, 
                     GA_Offset primoff = GA_INVALID_OFFSET);
    bool	 add(const GA_Edge &edge, 
		     const GA_Primitive *prim);
    bool 	 add(GA_Offset p0, GA_Offset p1, 
		     const GA_Primitive *prim);
    /// @}

    /// Toggle the existence of all edges of the detail for this group.
    void	 toggle();
    void	 toggleEntries() { toggle(); }
    
    /// Toggles the existence of a given edge in the group. If the edge got 
    /// removed, then this function returns \c false, if it got added, however,
    /// \c true is returned.
    /// @{
    bool         toggle(const GA_Edge &edge,
                        GA_Offset primoff = GA_INVALID_OFFSET);
    bool         toggle(const GA_Edge &edge,
                        const GA_Primitive *prim);
    /// @}

    /// Remove the edge pointed to by the iterator, and return an iterator
    /// pointing to the next edge in the group.
    iterator	 remove(iterator it);
    
    /// Remove the entry corresponding with the given edge and the optional
    /// primitive. Returns \c true if an edge was deleted based on the 
    /// search data.
    /// @{
    bool         remove(const GA_Edge &edge, 
                        GA_Offset primoff = GA_INVALID_OFFSET);
    bool         remove(const GA_Edge &edge, 
                        const GA_Primitive *prim);
    /// @}

    /// Returns the number of edges in this group.
    virtual GA_Size 	 entries() const {return myData->myEdgeList.size();}
    
    /// Returns \c true if this edge group is empty.
    bool                 isEmpty() const { return myData->myEdgeList.empty(); }
    
    /// Remove all edges from this group.
    virtual void	 clear();
    
    /// Add all edges from the associated detail into this edge group.
    virtual void	 addAll();
    
    /// Returns a boolean indicating whether this group is ordered or not. 
    /// Edge groups are always ordered.
    virtual bool	 isOrdered() const;
    
    /// Returns a boolean indicating whether this group is mixed. Edge groups
    /// are never mixed, so this function always returns \c false.
    virtual bool	 isMixed() const;

    GA_Size		 entriesWithPrimitive() const
					{ return myData->myPrimEntryCount; }

    /// Combine another group with this edge group. Currently, only edge groups
    /// are supported. Returns \c true if the other group was combined with
    /// this one.
    virtual bool	 combine(const GA_Group * inputGroup);

    /// Save data to a JSON stream.
    /// @section JSON-GA_EdgeGroup JSON Schema: GA_EdgeGroup
    /// Private data for an edge group
    /// @code
    ///     "name"          : "GA_EdgeGroup",
    ///     "description"   : "Private data for edge groups",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "points": {
    ///             "type"   : "array",
    ///             "items"  : "integer",
    ///             "description" :
    ///                 "Flat array of points, 2 points per edge.  The points
    ///                  are stored in interleaved pairs representing the
    ///                  start/end points of each edge.",
    ///         }
    ///         "primitives": {
    ///             "type"   : "array",
    ///             "items"  : "integer",
    ///             "description" : "Primitives tied to the edge. May be -1.",
    ///             "optional"  : true,
    ///         }
    ///     },
    /// @endcode
    ///
    /// @see @ref JSON_FileFormat, GA_GroupTable
    virtual bool	jsonSaveData(UT_JSONWriter &w,
				    const GA_SaveMap &m) const;
    /// Load the private JSON data
    virtual bool	jsonLoadData(UT_JSONParser &w,
				    const GA_LoadMap &m);

    /// Combinatorial operatos. 
    GA_EdgeGroup	&operator|=(const GA_EdgeGroup &inputGroup);
    GA_EdgeGroup	&operator&=(const GA_EdgeGroup &inputGroup);
    GA_EdgeGroup	&operator-=(const GA_EdgeGroup &inputGroup);
    GA_EdgeGroup	&operator+=(const GA_EdgeGroup &inputGroup);
    GA_EdgeGroup	&operator^=(const GA_EdgeGroup &inputGroup);
    GA_EdgeGroup	&operator =(const GA_EdgeGroup &inputGroup);

    GA_DataId getDataId() const
    {
        return myData->myDataID;
    }
    void bumpDataId();
    void cloneDataId(const GA_EdgeGroup &src);

    /// NOTE: You don't need to call this for groups in a SOP's output detail,
    ///       because it will call GA_EdgeGroupTable::makeAllEdgesValid(),
    ///       which can be more efficient than calling this for each edge
    ///       group, (if there are multiple).
    ///       This also won't do anything if the max of the topology data ID
    ///       and the primitive list data ID is equal to myLastValidTopoId,
    ///       so you have to make sure that any necessary bumping has occurred.
    ///       This was added for use by GU_EdgeSelection::validateEdges().
    void makeAllEdgesValid();

    GA_DataId getLastValidTopoId() const
    {
        return myData->myLastValidTopoId;
    }
    void setValidTopoId(GA_DataId topology_data_id)
    {
        harden()->myLastValidTopoId = topology_data_id;
    }
    
    GA_DataId getLastValidPrimListId() const
    {
        return myData->myLastValidPrimListId;
    }
    void setValidPrimListId(GA_DataId primlist_data_id)
    {
        harden()->myLastValidPrimListId = primlist_data_id;
    }

private:
    bool	 addEntry(const PrimEdge &edge);
    void 	 addEntryNoDuplicateCheck(const PrimEdge &edge);

    struct SharedData : public UT_IntrusiveRefCounter<SharedData>
    {
        SharedData();
        SharedData(const SharedData &that)
            : UT_IntrusiveRefCounter() // Don't copy ref counter
            , myEdgeList(that.myEdgeList)
            , myEdgeMap(that.myEdgeMap)
            , myPrimEntryCount(that.myPrimEntryCount)
            , myDataID(that.myDataID)
            , myLastValidTopoId(that.myLastValidTopoId)
            , myLastValidPrimListId(that.myLastValidPrimListId)
        {
            // The map maps to iterators in the list, so we have to update them.
            for (auto it = myEdgeList.begin(); it != myEdgeList.end(); ++it)
            {
                myEdgeMap[*it] = it;
            }
        }

        EdgeList myEdgeList;
        EdgeMap  myEdgeMap;
        GA_Size  myPrimEntryCount;
        GA_DataId    myDataID;

        /// This is the last topology data ID for which
        /// this edge group was validated, or -1 if it has never been validated.
        /// If getTopology().getDataId() returns something *not equal* to this
        /// (not just greater than), it should be re-validated.
        GA_DataId    myLastValidTopoId;

        /// This is the last primitive list data ID for which
        /// this edge group was validated, or -1 if it has never been validated.
        /// If getPrimitiveList().getDataId() returns something *not equal* to this
        /// (not just greater than), it should be re-validated.
        GA_DataId    myLastValidPrimListId;
    };

    SharedData *harden()
    {
        if (myData->use_count() > 1)
            myData.reset(new SharedData(*myData));
        return myData.get();
    }

    const GA_Detail		&myGdp;
    UT_IntrusivePtr<SharedData>  myData;
};
#endif
