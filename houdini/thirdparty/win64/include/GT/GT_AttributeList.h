/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_AttributeList.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_AttributeList__
#define __GT_AttributeList__

#include "GT_API.h"
#include "GT_AttributeMap.h"
#include "GT_DataArray.h"
#include "GT_Handles.h"
#include "GT_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>

#include <utility>


class GT_DataArray;
class GT_AttributeMerge;
class UT_JSONWriter;
class GU_Detail;

class GT_API GT_AttributeList
    : public UT_IntrusiveRefCounter<GT_AttributeList>
{
public:
    /// An attribute list stores a list of GT_DataArray values for each
    /// attribute defined in the map.  @c motion_segments specifies the number
    /// of different motion segments.
     GT_AttributeList(const GT_AttributeMapHandle &map, int motion_segments=1);
     GT_AttributeList(const GT_AttributeList &src);
    ~GT_AttributeList();

    /// Create an attribute list using varying number of
    /// (const UT_StringHolder &name, const GT_DataArrayHandle &data) pairs.
    /// For example: @code
    ///	    GT_AttributeList::createAttributeList(
    ///			"P", new GT_Real32Array(...),
    ///			"N", new GT_Real32Array(...),
    ///			"uv", new GT_Real32Array(...));
    /// @endcode
    template <typename... Pairs>
    static GT_AttributeListHandle createAttributeList(Pairs&&... pairs)
    {
	int n = gt_CountAttribs(std::forward<Pairs>(pairs)...);
	GT_AttributeListHandle
	    list(new GT_AttributeList(n, new GT_AttributeMap(n), 1));
	gt_CreateAttributeList(*list, *list->getMap(),
			       std::forward<Pairs>(pairs)...);
	return list;
    }

    /// Test whether two attribute lists match in their definition.  If both
    /// attribute lists are empty, this will count as a match.
    static bool		matchDefinition(const GT_AttributeListHandle &a,
				const GT_AttributeListHandle &b);


    /// Create a new attribute list, adding a new attribute in the process.
    GT_AttributeListHandle	addAttribute(const UT_StringHolder &name,
					const GT_DataArrayHandle &h,
					bool replace_existing) const;
    
    /// Remove an attribute from this list, returning a new attribute list.
    GT_AttributeListHandle	removeAttribute(const UT_StringRef &name) const;

    /// Remove multiple attributes from this, returning a new attribute list.
    GT_AttributeListHandle	removeAttributes(const UT_StringArray &name) const;


    /// Harden all the attribute lists
    void	harden();

    /// Harden a single attribute
    void	harden(int idx);

    /// Get a handle to the name map for this list.
    const GT_AttributeMapHandle	&getMap() const { return myMap; }

    /// Return the number of attributes in the list.
    int				entries() const
				    { return myMap->entries(); }

    /// Return the approximate memory usage.  Because data can be shared, this
    /// may not reflect the usage accurately.
    int64			getMemoryUsage() const;

    /// Return the number of motion segements
    int				getSegments() const
				    { return myMotionSegments; }

    /// Return the list of names
    const UT_StringArray	&getNames() const
				    { return myMap->getNames(); }

    /// Return the data for the named array
    const GT_DataArrayHandle	&get(const UT_StringRef &name,
				     int segment=0) const
				    {
					return get(myMap->get(name), segment);
				    }
    /// Return the data for a given index
    const GT_DataArrayHandle	&get(int index, int motion_segment=0) const;

    /// Return the name for a given index
    const UT_StringHolder	&getName(int index) const
				    { return myMap->getName(index); }

    /// Return the export name for a given index.
    const UT_StringHolder	&getExportName(int index) const
				    { return myMap->getExportName(index); }

    /// Get original attribute owner for the given attribute.
    GT_Owner			 getOriginalOwner(int idx) const
				    { return myMap->getOriginalOwner(idx); }

    /// Return the index for a given name (returns -1 if miss)
    int				getIndex(const UT_StringRef &name) const
				    { return myMap->get(name); }

    /// Test of an attribute name exists
    bool			hasName(const UT_StringRef &name) const
				    { return myMap->hasName(name); }

    /// Assign an array to a given index in the list.
    void			set(int idx, const GT_DataArrayHandle &h,
					int motion_segment=0);

    /// Assign an array to a given index in the list for @b all motion segments
    void			setAllSegments(int idx,
					const GT_DataArrayHandle &h);

    /// Copies an entire segment of data arrays from a source list to this list.
    /// This is useful when combining multiple primitives into a single motion
    /// blurred primitive for example.
    /// @param dest_segment  @n
    ///	  Data arrays are copied to this segment in this list
    /// @param source_list @n
    ///   Data arrays are copied from this source list
    /// @param source_segment @n
    ///	  Data arrays are copied from this segment in the source_list
    /// @warning The source attribute list @b must have the same maps.  The
    ///   code does @b not verify that the maps are the same.
    bool	copySegment(int dest_segment,
				const GT_AttributeListHandle &source_list,
				int source_segment=0);

    /// Create a transformed version of the attributes
    GT_AttributeListHandle	transform(const GT_TransformHandle &x) const;

    /// Create a new attribute list containing
    GT_AttributeListHandle	createConstant(GT_Offset offset,
						GT_Size size) const;

    /// Create an attribute list of a subset of the contents
    GT_AttributeListHandle	createSubArray(GT_Offset start,
					GT_Size size) const;

    /// Create a new attribute list based on the indirect indicies in the
    /// @c list
    GT_AttributeListHandle	createIndirect(
					const GT_DataArrayHandle &indirect,
					bool flatten_nested = true) const;

    /// Merge attributes which don't already exist in this attribute list
    GT_AttributeListHandle	mergeNewAttributes(
					const GT_AttributeListHandle &s) const;

    /// Create the "s" and "t" attributes for the given u/v vertex count.
    /// The attributes will only be created if @c nu or @c nv is greater than 0
    GT_AttributeListHandle	createUV(int nu, int nv,
					 fpreal64 s0=0, fpreal64 s1=1,
					 fpreal64 t0=0, fpreal64 t1=1) const;

    /// Create a merged list of the two lists.  This will merge a sub-array of
    /// the first list and an indirect mapping of the second list.
    /// This is used to merge a vertex list (l0) with a point list (l1)
    static GT_AttributeListHandle createMerged(
					const GT_AttributeMerge &map,
					const GT_AttributeListHandle &l0,
					GT_Offset l0_offset, GT_Size l0_size,
					const GT_AttributeListHandle &l1,
					const GT_DataArrayHandle &l1_indirect);

    /// Merge two lists, expanding the constant list to have arrays matching
    /// the length of the varying array.
    static GT_AttributeListHandle expandConstantMerge(
					const GT_AttributeMerge &map,
					const GT_AttributeListHandle &varying,
					const GT_AttributeListHandle &constant,
					GT_Offset const_index=0);

    /// Merge two lists, creating a constant value list, selecting a single
    /// element from a varying list.
    static GT_AttributeListHandle createConstantMerge(
					const GT_AttributeMerge &map,
					const GT_AttributeListHandle &varying,
					GT_Offset offset,
					const GT_AttributeListHandle &constant);

    /// Merge an array of N lists to form a single attribute list.  All the
    /// data is concatenated in order.
    ///
    /// This method will fail (return an empty list) if any lists don't have
    /// matching attributes (including motion segments & order of attributes)
    ///
    /// The list may have NULL handles.
    static GT_AttributeListHandle concatenateLists(
			const UT_Array<GT_AttributeListHandle> &lists);

    /// Get a 64b hash of all data ids in the list. Return false if any attribs
    /// have an invalid data id (-1)
    bool		getDataIdHash(int64 &hash, int segment=0) const;

    /// Refresh the any detail references and data IDs on all data arrays.
    void		updateGeoDetail(const GU_ConstDetailHandle &dtl,
					GT_Owner scope) const;

    /// Debug method to dump the list to stdout
    void		dumpList(const char *label="", bool data_too = true);

    /// Save to a JSON stream
    bool		save(UT_JSONWriter &w) const;

    class GT_API iterator
    {
	public:
	    iterator()
		: myList(NULL), myCurr(0), mySize(0), mySegment(0) {}
	    iterator(const iterator &src)
		{ *this = src; }
	    ~iterator() {}

	    const GT_DataArrayHandle	&getData() const
					{
					    return myList->get(myCurr,
							mySegment);
					}
	    const UT_StringHolder	&getName() const
					{
					    return myList->getName(myCurr);
					}

	    void	rewind()	{ myCurr = 0; }
	    void	advance()	{ myCurr++; }
	    bool	atEnd() const	{ return myCurr >= mySize; }

	    iterator	&operator++()	{ advance(); return *this; }
	    // No post increment as it is dangerous.
	    const iterator	&operator=(const iterator &src)
				{
				    myList = src.myList;
				    myCurr = src.myCurr;
				    mySize = src.mySize;
				    return *this;
				}

	private:
	    iterator(const GT_AttributeList *list, int segment)
		: myList(list),
		  mySegment(segment),
		  myCurr(0),
		  mySize(list->entries()) {}

	    const GT_AttributeList	*myList;
	    int				 myCurr, mySize, mySegment;
	    friend class GT_AttributeList;
    };
    /// Traverse over all attributes for a given motion segment
    iterator	begin(int segment=0) const { return iterator(this, segment); }

    // Forward declare nested class (and make it a friend)
    class		gt_CreateIndirectTask;
    friend class	gt_CreateIndirectTask;

private:

    // Private constructor for createAttributeList() only
    GT_AttributeList(int size, const GT_AttributeMapHandle &map,
		     int motion_segments);

    static SYS_FORCE_INLINE int
    gt_CountAttribs()
    {
	return 0;
    }
    template <typename DataT, typename... Pairs>
    static int
    gt_CountAttribs(const UT_StringRef &name, DataT&& data,
		    Pairs&&... pairs)
    {
	return (data ? 1 : 0) + gt_CountAttribs(std::forward<Pairs>(pairs)...);
    }

    static SYS_FORCE_INLINE void
    gt_CreateAttributeList(GT_AttributeList &, GT_AttributeMap &)
    {
    }
    template <typename DataT, typename... Pairs>
    static void
    gt_CreateAttributeList(
	    GT_AttributeList &list, GT_AttributeMap &map,
	    const UT_StringHolder &name, DataT&& data,
	    Pairs&&... pairs)
    {
	// Ignore NULL data arrays. Makes it easier to make lists where it's
	// not known if some of the data arrays are valid at compile time.
	if (data)
	{
	    int i = map.add(name, /*replace*/false);
	    UT_ASSERT_MSG(i >= 0, "Should not have duplicates!");
	    list.set(i, data);
	}
	gt_CreateAttributeList(list, map, std::forward<Pairs>(pairs)...);
    }

private:
    /// Create a new constant data array for the i'th element
    const GT_DataArrayHandle	createConstant(int idx, GT_Offset offset,
					       GT_Size size,
					       int segment) const;

    /// Create a new sub-array data array for the i'th element
    const GT_DataArrayHandle	createSubArray(int idx, GT_Offset start,
					       GT_Size size,
					       int segment) const;

    /// Create a new sub-array data array for the i'th element
    const GT_DataArrayHandle	createIndirect(
				    int idx,
				    const GT_DataArrayHandle &indirect,
				    int segment,
				    bool flatten_nested) const;

    inline int		getSegmentIndex(int index, int segment) const
			    { return index + segment*entries(); }

    // Keep the 32 bit int first in the structure.  Since we inherit from
    // GA_IntrusiveRefCounter, this alignment makes the object smaller.
    int				 myMotionSegments;
    const GT_AttributeMapHandle	 myMap;
    GT_DataArrayHandle		*myData;

    // For threading of creating indirect arrays
};

#endif
