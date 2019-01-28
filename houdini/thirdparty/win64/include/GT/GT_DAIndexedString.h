/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAIndexedString.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAIndexedString__
#define __GT_DAIndexedString__

#include "GT_API.h"
#include "GT_Handles.h"
#include "GT_DataArray.h"
#include "GT_Memory.h"
#include <UT/UT_Assert.h>
#include <UT/UT_IndexedHashMapT.h>

class GT_API GT_DAIndexedString : public GT_DataArray
{
public:
    GT_DAIndexedString(GT_Size array_size, int tuple_size=1);
    virtual ~GT_DAIndexedString();

    virtual const char *className() const { return "GT_DAIndexedString"; }
    
    /// @{
    /// Methods defined on GT_DataArray
    /// An indexed string array is hard to begin with
    virtual GT_DataArrayHandle	harden() const
			    { return GT_DataArrayHandle(SYSconst_cast(this)); }
    virtual GT_Storage	getStorage() const	{ return GT_STORE_STRING; }
    virtual GT_Size	getTupleSize() const	{ return myTupleSize; }
    virtual GT_Size	entries() const		{ return mySize; }
    virtual GT_Type	getTypeInfo() const	{ return GT_TYPE_NONE; }
    virtual int64	getMemoryUsage() const
			{
#if 0
			    return myStrings.getMemoryUsage() +
					sizeof(int32)*mySize*myTupleSize;
#else
			    return sizeof(int32)*mySize*myTupleSize;
#endif
			}

    virtual uint8	getU8(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual int32	getI32(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual int64	getI64(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual fpreal16	getF16(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual fpreal32	getF32(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual fpreal64	getF64(GT_Offset offset, int index=0) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    offset = offset*myTupleSize + index;
			    return myData[offset];
			}
    virtual GT_String	getS(GT_Offset offset, int index) const
			{
			    UT_ASSERT_P(offset >= 0 && offset < mySize);
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    // Find out which string
			    int					 sidx;
			    const UT_IndexedHashMapStringItem	*item;
			    const char				*str = "";
			    sidx = getI32(offset, index);
			    if (sidx >= 0 && (item = myStrings.get(sidx)))
				str = item->getKey().getString();
			    return str;
			}
    virtual GT_Size	getStringIndexCount() const
			{
			    return myStrings.getItemIdUpperBound()+1;
			}
    virtual GT_Offset	getStringIndex(GT_Offset offset, int idx) const
			{
			    return getI32(offset, idx);
			}
    virtual void	getIndexedStrings(UT_StringArray &strings,
				UT_IntArray &indices) const;
    virtual void	getStrings(UT_StringArray &strings) const;
    /// @}

    void	resize(GT_Size size)
		{
		    if (!size)
		    {
			GT_Memory::Free(GT_MEM_DATA, myData,
				mySize*myTupleSize*sizeof(int32));
			myStrings.clear();
		    }
		    else
		    {
			for (GT_Size i = size; i < mySize; ++i)
			{
			    for (GT_Size j = 0; j < myTupleSize; ++j)
				freeString(i, j);
			}
			myData = (int32 *)GT_Memory::Realloc(GT_MEM_DATA,
					    myData,
					    mySize*myTupleSize*sizeof(int32),
					    size*myTupleSize*sizeof(int32));
			for (GT_Size i = mySize; i < size; ++i)
			{
			    for (GT_Size j = 0; j < myTupleSize; ++j)
				myData[i*myTupleSize+j] = -1;
			}
			mySize = size;
		    }
		}
    /// Set an index to a value
    void	setString(GT_Size offset, int index, const char *str)
		{
		    UT_ASSERT_P(offset >= 0 && offset < mySize);
		    UT_ASSERT_P(index >= 0 && index < myTupleSize);
		    if (!str || *str == 0)
			freeString(offset, index);
		    else
		    {
			int			old_id;
			UT_IndexedHashMapItemId new_id;
			offset = offset*myTupleSize + index;
			old_id = myData[offset];
			myStrings.add(str, NULL, &new_id);
			if (old_id >= 0)
			    myStrings.remove(old_id);
			myData[offset] = new_id;
		    }
		}
private:
    void	freeString(GT_Size offset, int index)
		{
		    UT_ASSERT_P(offset >= 0 && offset < mySize);
		    UT_ASSERT_P(index >= 0 && index < myTupleSize);
		    offset = offset*myTupleSize + index;
		    if (myData[offset] >= 0)
		    {
			myStrings.remove(myData[offset]);
			myData[offset] = -1;
		    }
		}
    UT_IndexedStringMap	 myStrings;	// Shared string table
    int32		*myData;
    GT_Size		 mySize;
    GT_Size		 myTupleSize;
};

#endif
