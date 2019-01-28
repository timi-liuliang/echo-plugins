/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAList.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAList__
#define __GT_DAList__

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_CountArray.h"
#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>

/// @brief A list which "merges" multiple data arrays into a single array
class GT_API GT_DAList : public GT_DataArray {
public:
    /// Default constructor
    GT_DAList()
	: myFirst(NULL)
	, myStorage(GT_STORE_INVALID)
	, GT_DataArray()
    {
    }
    /// Useful constructor
    GT_DAList(const UT_Array<GT_DataArrayHandle> &list)
	: myFirst(NULL)
	, myStorage(GT_STORE_INVALID)
	, GT_DataArray()
    {
	init(list);
    }
    /// Copy constructor
    GT_DAList(const GT_DAList &src)
	: myFirst(NULL)
	, myStorage(src.getStorage())
	, GT_DataArray(src)
    {
	init(src.myList);
    }
    /// Destructor
    virtual ~GT_DAList();

    virtual const char	*className() const { return "GT_DAList"; }
    
    /// Initialize with given data
    void	init(const UT_Array<GT_DataArrayHandle> &list);

    /// Test whether the array is valid
    bool	isValid() const	{ return myFirst != NULL; }

    /// @{
    /// Method defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			    { return myFirst->getStorage(); }
    virtual GT_Size	getTupleSize() const
			    { return myFirst->getTupleSize(); }
    virtual GT_Type	getTypeInfo() const
			    { return myFirst->getTypeInfo(); }
    virtual GT_Size	entries() const
			    { return myTotalEntries; }

    virtual int64	getMemoryUsage() const;
    virtual bool	getPointerAliasing(const void *data) const;
    /// @}

    /// @{
    /// Not supported since there's no easy way to merge string indices
    virtual GT_Size	getStringIndexCount() const	{ return 0; }
    virtual GT_Offset	getStringIndex(GT_Offset, int idx) const { return -1; }
    virtual void	getIndexedStrings(UT_StringArray &, UT_IntArray&) const
			    {}
    /// @}

protected:
    /// @{
    /// Data accessor
    virtual fpreal16	getF16(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getF16(offset, idx);
			}
    virtual fpreal32	getF32(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getF32(offset, idx);
			}
    virtual fpreal64	getF64(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getF64(offset, idx);
			}
    virtual uint8	getU8(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getU8(offset, idx);
			}
    virtual int32	getI32(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getI32(offset, idx);
			}
    virtual int64	getI64(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getI64(offset, idx);
			}
    virtual GT_String	getS(GT_Offset offset, int idx=0) const
			{
			    const GT_DataArray	*h = getList(offset);
			    return h->getS(offset, idx);
			}

    virtual void	doImport(GT_Offset off, uint8 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, int8 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, int16 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, int32 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, int64 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, fpreal16 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, fpreal32 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}
    virtual void	doImport(GT_Offset off, fpreal64 *d, GT_Size sz) const
			{
			    const GT_DataArray	*h = getList(off);
			    h->import(off, d, sz);
			}

    virtual void	doFillArray(uint8 *data, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(int8 *data, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(int16 *data, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(int32 *data, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(int64 *data, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(fpreal16 *dat, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(fpreal32 *dat, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	doFillArray(fpreal64 *dat, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    /// @}


private:
    const GT_DataArray *getList(GT_Offset &offset) const
			{
			    // Figure out which list is 
			    GT_Offset	list = myListFromOffset(offset);
			    // Now, adjust the offset to be relative for the
			    // list in question.
			    offset -= myListCounts.getOffset(list);
			    return myList(list).get();
			}

    const GT_DataArray			*myFirst;
    UT_Array<GT_DataArrayHandle>	 myList;
    UT_IntArray				 myListFromOffset;
    GT_CountArray			 myListCounts;
    GT_Size				 myTotalEntries;
    GT_Storage				 myStorage;
};

#endif
