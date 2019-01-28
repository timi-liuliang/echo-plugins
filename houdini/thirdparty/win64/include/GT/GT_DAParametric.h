/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAParametric.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAParametric__
#define __GT_DAParametric__

#include "GT_API.h"
#include "GT_DataArray.h"
#include <SYS/SYS_Math.h>

/// @brief Provide uniform parametric coordinates for a curve/surface
///
/// An efficient way of representing parametric coordinates
/// For a patch: @code
///	uv = GT_DAParamtric(nu, nv);
/// @endcode
/// For a curve primitive: @code
///	uv = GT_DAParamtric(nu, 1);
/// @endcode
/// 
class GT_API GT_DAParametric : public GT_DataArray
{
public:
    /// Default constructor
    GT_DAParametric()
	: GT_DataArray()
    {
	init(2, 2);
    }
    /// Useful constructor
    GT_DAParametric(GT_Size nu, GT_Size nv,
	    fpreal64 u0=0, fpreal64 u1=1,
	    fpreal64 v0=0, fpreal64 v1=1)
	: GT_DataArray()
    {
	init(nu, nv, u0, u1, v0, v1);
    }
    /// Copy constructor
    GT_DAParametric(const GT_DAParametric &src)
	: myNu(src.myNu),
	  myNv(src.myNv),
	  myU0(src.myU0),
	  myUinc(src.myUinc),
	  myV0(src.myV0),
	  myVinc(src.myVinc),
	  GT_DataArray()
    {
    }
    virtual ~GT_DAParametric();

    virtual const char *className() const { return "GT_DAParametric"; }
    
    /// Initialize with given data
    void		init(GT_Size nu, GT_Size nv,
			    fpreal64 u0=0, fpreal64 u1=1,
			    fpreal64 v0=0, fpreal64 v1=1)
			{
			    myNu = SYSmax(nu, (GT_Size)1);
			    myNv = SYSmax(nv, (GT_Size)1);
			    myU0 = u0;
			    myV0 = v0;
			    myUinc = myNu > 1 ? (u1-u0)/fpreal64(myNu - 1) : 1;
			    myVinc = myNv > 1 ? (v1-v0)/fpreal64(myNv - 1) : 1;
			}
    /// @{
    /// After being duplicated, you can perform a binary split
    void	splitU0()	{ myUinc *= 0.5; }
    void	splitU1()	{ myUinc *= 0.5; myU0 += myUinc*myNu; }
    void	splitV0()	{ myVinc *= 0.5; }
    void	splitV1()	{ myVinc *= 0.5; myV0 += myVinc*myNv; }
    /// @}

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			    { return GT_STORE_REAL64; }
    virtual GT_Size	getTupleSize() const
			    { return 2; }
    virtual int64	getMemoryUsage() const
			    { return sizeof(*this); }
    virtual GT_Type	getTypeInfo() const
			    { return GT_DataArray::getTypeInfo(); }
    virtual GT_Size	entries() const
			    { return myNu*myNv; }
    virtual uint8	getU8(GT_Offset offset, int idx) const
			    { return getF64(offset, idx) > 0.5; }
    virtual int32	getI32(GT_Offset offset, int idx) const
			    { return getF64(offset, idx) > 0.5; }
    virtual int64	getI64(GT_Offset offset, int idx) const
			    { return getF64(offset, idx) > 0.5; }
    virtual fpreal16	getF16(GT_Offset offset, int idx) const
			    { return idx ? getV(offset) : getU(offset); }
    virtual fpreal32	getF32(GT_Offset offset, int idx) const
			    { return idx ? getV(offset) : getU(offset); }
    virtual fpreal64	getF64(GT_Offset offset, int idx) const
			    { return idx ? getV(offset) : getU(offset); }
    virtual GT_String	getS(GT_Offset, int) const		{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}
    virtual GT_DataArrayHandle	harden() const
			    { return GT_DataArrayHandle(SYSconst_cast(this)); }
    /// @}

private:
    /// Map the requested offset to the coordinate in my data
    inline fpreal64	getU(GT_Offset offset) const
			{
			    return myU0 + fpreal64(offset % myNu) * myUinc;
			}
    inline fpreal64	getV(GT_Offset offset) const
			{
			    return myV0 + fpreal64(offset / myNu) * myVinc;
			}

    fpreal64		myU0, myV0;
    fpreal64		myUinc, myVinc;
    GT_Size		myNu, myNv;
};

#endif
