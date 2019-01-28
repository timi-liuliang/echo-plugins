/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DABool.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DABool__
#define __GT_DABool__

#include "GT_API.h"
#include <UT/UT_BitArray.h>
#include "GT_DataArray.h"

/// GT Data Array for booleans
class GT_API GT_DABool : public GT_DataArray
{
public:
    GT_DABool(GT_Size array_size, GT_Size tuple_size=1,
		GT_Type type=GT_TYPE_NONE);
    virtual ~GT_DABool();

    /// @{
    /// Virtual interface defined on GT_DataArray
    virtual const char		*className() const { return "GT_DABool"; }
    virtual GT_DataArrayHandle	 harden() const;
    virtual GT_Size		 entries() const { return myBits[0]->size(); }
    virtual GT_Storage		 getStorage() const { return GT_STORE_UINT8; }
    virtual GT_Size		 getTupleSize() const { return myTupleSize; }
    virtual int64		 getMemoryUsage() const;
    virtual GT_Type		 getTypeInfo() const { return myType; }

    virtual uint8	getU8(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual int32	getI32(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual int64	getI64(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual fpreal16	getF16(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual fpreal32	getF32(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual fpreal64	getF64(GT_Offset i, int tidx=0) const
			    { return getBit(i, tidx); }
    virtual GT_String	getS(GT_Offset, int) const	{ return NULL; }
    virtual GT_Size	getStringIndexCount() const	{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				UT_IntArray &) const {}
    /// @}

    /// Get the bit at the given index
    bool	getBit(GT_Offset i, GT_Size tidx=0) const
		{
		    UT_ASSERT(tidx >= 0 && tidx < myTupleSize);
		    return myBits[tidx]->getBit(i);
		}
    /// Set the bit at the given index -- returns the previous value
    bool	setBit(GT_Offset i, bool v, GT_Size tidx=0)
		{
		    UT_ASSERT(tidx >= 0 && tidx < myTupleSize);
		    return myBits[tidx]->setBit(i, v);
		}
    void	setAllBits(bool v, GT_Size tidx=0)
		{
		    UT_ASSERT(tidx >= 0 && tidx < myTupleSize);
		    myBits[tidx]->setAllBits(v);
		}

protected:
    /// {@
    /// Import data
    virtual void	doImport(GT_Offset i, uint8 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, int8 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, int16 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, int32 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, int64 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, fpreal16 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, fpreal32 *data, GT_Size sz) const;
    virtual void	doImport(GT_Offset i, fpreal64 *data, GT_Size sz) const;

    virtual void	dofillArray(uint8 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	dofillArray(int32 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	dofillArray(int64 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	dofillArray(fpreal16 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	dofillArray(fpreal32 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    virtual void	dofillArray(fpreal64 *buf, GT_Offset start, GT_Size len,
				int tsize, int stride) const;
    /// @}

    /// @{
    /// Virtual interface implemented by calling existing methods
    virtual void	doFillQuantizedArray(uint8 *data, GT_Offset start,
				GT_Size length, int tsize, int stride,
				fpreal, fpreal) const
			{
			    doFillArray(data, start, length, tsize, stride);
			}
    virtual void	doExtendedQuantizedFill(uint8 *data, GT_Offset start,
				GT_Size length, int nrepeats,
				int tsize, int stride,
				fpreal black, fpreal white) const
			{
			    if (nrepeats == 1)
			    {
				doFillQuantizedArray(data, start, length, tsize,
					    stride, black, white);
			    }
			    else
			    {
				GT_DataArray::doExtendedQuantizedFill(data,
				    start, length, tsize,
				    nrepeats, stride, black, white);
			    }
			}
    /// @}

private:
    UT_BitArray **myBits;
    GT_Size	  myTupleSize;
    GT_Type	  myType;
};

#endif
