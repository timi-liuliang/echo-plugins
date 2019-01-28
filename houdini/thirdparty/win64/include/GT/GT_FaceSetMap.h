/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_FaceSetMap.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_FaceSetMap__
#define __GT_FaceSetMap__

#include "GT_API.h"

#include <UT/UT_StringMap.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Set.h>
#include "GT_FaceSet.h"

class UT_JSONWriter;
class GT_FaceSetMap;

using GT_FaceSetMapPtr =  UT_IntrusivePtr<GT_FaceSetMap>;

/// Class to store "face sets" on GT mesh primitives
class GT_API GT_FaceSetMap
    : public UT_IntrusiveRefCounter<GT_FaceSetMap>
    , public UT_NonCopyable
{
public:
    using MapType = UT_StringMap<GT_FaceSetPtr>;

    GT_FaceSetMap();
    ~GT_FaceSetMap();

    /// @{
    /// Map access.  These methods are @b not thread-safe
    GT_FaceSetPtr	find(const UT_StringRef &name) const;
    GT_FaceSetPtr	add(const UT_StringHolder &name,
				const GT_FaceSetPtr &set,
				bool replace_existing=true);
    GT_Size		entries() const { return myMap.size(); }
    /// @}

    /// Save sets to a JSON stream
    bool	save(UT_JSONWriter &w) const;
    void	dump() const;

    class GT_API iterator
    {
    public:
	using MapIterator = GT_FaceSetMap::MapType::const_iterator;
	iterator()
	    : myIt()
	    , myEnd()
	{
	};
	~iterator() {}

	const UT_StringHolder	&name() const { return myIt->first; }
	const GT_FaceSetPtr	&faceSet() const { return myIt->second; }

	iterator	&operator++() { ++myIt; return *this; }
	iterator	&operator=(const iterator &src)
			{
			    myIt = src.myIt;
			    myEnd = src.myEnd;
			    return *this;
			}
	bool		operator==(const iterator &cmp) const
			    { return myIt == cmp.myIt; }
	bool		operator!=(const iterator &cmp) const
			    { return myIt != cmp.myIt; }
	bool		atEnd() const	{ return myIt == myEnd; }
	void		advance()	{ ++myIt; }
    private:
	iterator(const MapIterator &it, const MapIterator &end)
	    : myIt(it)
	    , myEnd(end)
	{
	}
	MapIterator	myIt;
	MapIterator	myEnd;
	friend class GT_FaceSetMap;
    };
    iterator	begin() const	{ return iterator(myMap.begin(), myMap.end()); }
    iterator	end() const	{ return iterator(myMap.end(), myMap.end()); }

    /// Delete polygons from the face set.  The second argument is an array
    /// mapping the old face numbers to the new face numbers.  For example,
    /// deleting polygon 2 from a set of [0,1,2,3] would expect a remapping
    /// array of [0,1,x,2] (the value of 2 doesn't actually matter)
    GT_FaceSetMapPtr	deleteFaces(const UT_Set<int> &faces,
				const GT_Int32Array &mapping) const;


private:
    MapType		myMap;
};

#endif
