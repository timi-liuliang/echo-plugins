/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOSupport.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOSupport__
#define __GT_GEOSupport__

#include "GT_API.h"
#include <UT/UT_Array.h>
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <GA/GA_TempBuffer.h>
#include "GT_Types.h"
#include "GT_Handles.h"

/// Wrapper around an array of GA_Offsets.  Provides a way to allocate a
/// GT_DataArray
class GT_API GT_GEOOffsetList
{
public:
    class pointfromvertex {};

     GT_GEOOffsetList()
	: myList()
	, my32Bit(true)
	, myMonotonic(true)
     {}
     GT_GEOOffsetList(const GU_Detail &gdp, GA_AttributeOwner owner);
     GT_GEOOffsetList(const GU_Detail &gdp, const GA_Range &range);
     GT_GEOOffsetList(const GA_OffsetList &offsets);
     GT_GEOOffsetList(const GA_Offset *offsets, GA_Size noffsets);
     GT_GEOOffsetList(const GT_GEOOffsetList &src)
	 : myList(src.myList)
	 , my32Bit(src.my32Bit)
	 , myMonotonic(src.myMonotonic)
     {
     }
     // Given an offset list of vertices, create a new list of their points.
     // If there are vertices which reference the same point, the point will be
     // duplicated in the new offset list.
     GT_GEOOffsetList(const GU_ConstDetailHandle &gdp,
		    const GT_GEOOffsetList &vertex_list,
		    pointfromvertex);

     /// Extract a sub-array from the given offset list
     GT_GEOOffsetList(const GT_GEOOffsetList &src, exint start, exint end);
    ~GT_GEOOffsetList();

    void	clear()				{ myList.clear(); }
    void	reserve(exint numtoadd, bool setentries=false)
    {
        // NOTE: GA_OffsetList::setEntries hardens, and GA_OffsetList::reserve
        //       doesn't. GA_OffsetList::reserve also doesn't set entries

        exint newentries = myList.entries() + numtoadd;
        if (newentries > myList.capacity() && !myList.isTrivial())
        {
            exint newcapacity = SYSmax(newentries, UTbumpAlloc(myList.capacity()));

            myList.reserve(newcapacity);
        }
        if (setentries)
        {
            myList.setEntries(newentries, false);
            my32Bit = my32Bit && (myList.entries() <= (1<<30));
            myMonotonic = false;
        }
    }
    void	append(GA_Offset offset)
    {
        if (myList.entries() && offset < myList.last())
            myMonotonic = false;
        myList.append(offset);
        my32Bit = my32Bit && (offset < GA_Offset(1<<30));
    }
    void	checkMonotonic();
    void	concat(const GT_GEOOffsetList &src);
    bool	isTrivial() const		{ return myList.isTrivial(); }
    GT_Size	entries() const			{ return myList.entries(); }
    GA_Offset	get(exint i) const		{ return GA_Offset(myList(i)); }
    void	set(exint i, GA_Offset value)	{ myList.set(i, value); }
    GA_Offset	operator()(exint i) const	{ return get(i); }
    bool	is32Bit() const			{ return my32Bit; }
    int64	getMemoryUsage() const
    { return sizeof(*this) + myList.getMemoryUsage(false); }

    bool		monotonic() const	{ return myMonotonic; }
    const GA_OffsetList	&offsetList() const	{ return myList; }

    /// Create a GT_DataArray for the offsets
    GT_DataArrayHandle	 allocateArray() const;

    /// Assuming that this offset list contains GA_Offset for vertices in the
    /// given gdp, create a data array for the points referred to by the
    /// vertices.
    GT_DataArrayHandle	 createVertexPointArray(const GA_Detail &gdp) const;
    GT_DataArrayHandle	createVertexPointArray(const GU_ConstDetailHandle &gdh) const;

    /// Debug
    void	dump(const char *msg="", bool full=true) const;

private:
    GA_OffsetList	myList;
    bool		my32Bit;
    bool		myMonotonic;
};

/// Simple wrapper on an array of sizes, provides a method to allocate a
/// GT_DataArray.
class GT_GEOSizeList
{
public:
     GT_GEOSizeList()
	: myList()
	, myMin(0)
	, myMax(0)
     {}
     GT_GEOSizeList(const GT_GEOSizeList &src)
	 : myList(src.myList)
	 , myMin(src.myMin)
	 , myMax(src.myMax)
     {
     }
    ~GT_GEOSizeList();

    void	clear()
		{
		    myList.clear();
		    myMin = myMax = 0;
		}
    void	append(GT_Size item)
		{
		    if (myList.entries() == 0)
		    {
			myMin = myMax = item;
		    }
		    else
		    {
			myMin = SYSmin(myMin, item);
			myMax = SYSmax(myMax, item);
		    }
		    myList.append(item);
		}
    bool	is32Bit() const	{ return myMax < (1<<30) && myMin >= -(1<<30); }
    GT_Size	min() const			{ return myMin; }
    GT_Size	max() const			{ return myMax; }
    GT_Size	entries() const			{ return myList.entries(); }
    GT_Size	get(exint i) const		{ return GA_Offset(myList(i)); }
    // NOTE: If using set, you must use setMin and setMax
    void	set(exint i, GA_Size value)	{ myList.setValue(i, value); }
    void	setMin(GA_Size value)	{ myMin = value; }
    void	setMax(GA_Size value)	{ myMax = value; }
    GT_Size	operator()(exint i) const	{ return get(i); }
    void	reserve(exint numtoadd, bool setentries=false)
		{
                    myList.reserve(myList.entries() + numtoadd, setentries);
		}

    void		concat(const GT_GEOSizeList &src);
    GT_DataArrayHandle	allocateArray() const;

    /// Debug
    void	dump(const char *msg="", bool full=true) const;

private:
    GA_TempBuffer<GT_Size>	myList;
    GT_Size			myMin;
    GT_Size			myMax;
};

#endif
