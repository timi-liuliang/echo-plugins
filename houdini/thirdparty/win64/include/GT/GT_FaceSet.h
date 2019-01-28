/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_FaceSet.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_FaceSet__
#define __GT_FaceSet__

#include "GT_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Lock.h>
#include <UT/UT_Set.h>
#include "GT_Types.h"
#include "GT_Handles.h"
#include "GT_DANumeric.h"

class UT_JSONWriter;
class GT_FaceSet;

using GT_FaceSetPtr = UT_IntrusivePtr<GT_FaceSet>;

/// Class to store "face sets" on GT mesh primitives
class GT_API GT_FaceSet
    : public UT_IntrusiveRefCounter<GT_FaceSet>
    , public UT_NonCopyable
{
public:
    class SetType
    {
    public:
	// Thread-safety semantics
	// Only one thread can write to the set at a time
	// No other threads can read during creation
	// After creation, multiple threads should be able to access (search,
	// iterate)
	using ArrayType = UT_Array<GT_Offset>;
	 SetType()
	     : myOffsets()
	     , myDirty(false)
	 {}
	 SetType(const SetType &src)
	     : myOffsets(src.myOffsets)
	     , myDirty(src.myDirty)
	 {
	 }
	~SetType() {}

	void	insert(GT_Offset offset)
	{
	    if (myOffsets.entries() && myOffsets.last() >= offset)
	    {
		if (myOffsets.last() == offset)
		    return;	// No duplicates, so we can just return
		myDirty = true;	// Flag as un-ordered
	    }
	    myOffsets.append(offset);
	}
	template <typename T>
	void	multipleInsert(const T *list, exint size)
	{
	    if (size < 2)
	    {
		if (size == 1)
		    insert(list[0]);	// Don't automatically mark as dirty
		return;
	    }
	    // Instead of keeping track of whether the inserts are ordered or
	    // not, just mark the list as dirty.
	    myDirty = true;
	    exint	start = myOffsets.entries();
	    exint	end = start + size;
	    myOffsets.setCapacity(end);
	    myOffsets.entries(end);
	    for (exint i = start; i < end; ++i, ++list)
		myOffsets(i) = list[0];
	}
	exint	size() const	{ return myOffsets.entries(); }
	int	count(GT_Offset idx) const
	{
	    return findOffset(idx) >= 0 ? 1 : 0;
	}
	void	erase(GT_Offset item)
	{
	    exint	idx = findOffset(item);
	    if (idx >= 0)
		myOffsets.removeIndex(idx);
	}
	using const_iterator = ArrayType::const_iterator;
	const_iterator	begin() const
	{
	    // If we begin iteration, and another thread does a find, we need
	    // to be sorted.
	    ensureSorted();
	    return myOffsets.begin();
	}
	const_iterator	end() const	{ return myOffsets.end(); }

	// Methods not on std::set
	template <typename T> void
	fillOrderedArray(T *list) const
	{
	    ensureSorted();
	    for (exint i = 0; i < myOffsets.entries(); ++i)
		list[i] = myOffsets(i);
	}

    private:
	inline void	ensureSorted() const
	{
	    if (myDirty)
		SYSconst_cast(this)->sortOffsets();
	}
	void	sortOffsets()
	{
	    // TODO: Fence this?
	    UT_AutoLock	lock(myLock);
	    myOffsets.stdsort(std::less<exint>());
	    myOffsets.sortedRemoveDuplicates();
	    myDirty = false;
	}
	exint	findOffset(GT_Offset item) const
	{
	    ensureSorted();
	    return myOffsets.uniqueSortedFind(item, std::less<exint>());
	}
	ArrayType	myOffsets;
	UT_Lock		myLock;
	bool		myDirty;
    };

    GT_FaceSet();
    GT_FaceSet(const GT_DataArrayHandle &members);
    ~GT_FaceSet();

    /// @{
    /// Number of faces in the face set
    GT_Size	entries() const	{ return myMembers.size(); }
    GT_Size	size() const	{ return myMembers.size(); }
    /// @}

    /// Test whether a face is in the set
    bool	contains(GT_Offset face_index) const
		    { return myMembers.count(face_index) > 0; }

    /// Add a face to the set
    void	addFace(GT_Offset face_index);

    /// @{
    /// Add a list of faces
    void	addFaces(const int32 *indices, exint count);
    void	addFaces(const int64 *indices, exint count);
    /// @}

    /// Remove a face from the set
    void	removeFace(GT_Offset face_index);

    /// Check whether any of the faces in this set are in the list of faces to
    /// be deleted.
    bool		anyIntersect(const UT_Set<int> &faces) const;

    /// Delete polygons from the face set.  The second argument is an array
    /// mapping the old face numbers to the new face numbers.  For example,
    /// deleting polygon 2 from a set of [0,1,2,3] would expect a remapping
    /// array of [0,1,x,2] (the value of 2 doesn't actually matter)
    GT_FaceSetPtr	deleteFaces(const UT_Set<int> &faces,
				const GT_Int32Array &mapping) const;

    /// Extract the member list as a sorted array of integers
    GT_DataArrayHandle	extractMembers() const;

    /// Save sets to a JSON stream
    bool	save(UT_JSONWriter &w) const;

    SetType::const_iterator	begin() const	{ return myMembers.begin(); }
    SetType::const_iterator	end() const	{ return myMembers.end(); }

private:
    SetType		myMembers;
    bool		my64bit;
};

#endif
