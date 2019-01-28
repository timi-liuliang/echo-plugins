/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_PrimitiveP.h (GA Library, C++)
 *
 * COMMENTS:    Wrapper for GA_Primitive to emulate a pointer,
 *              avoiding hardening the primitive list in a few cases.
 */

#pragma once

#include "GA_Detail.h"
#include "GA_PrimCompat.h"
#include "GA_Primitive.h"
#include "GA_Types.h"
#include <UT/UT_Assert.h>
#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Inline.h>
#include <type_traits>

template<typename PrimT,typename DetailT>
class GA_PrimitiveP_T
{
public:
    SYS_FORCE_INLINE
    GA_PrimitiveP_T()
        : myDetail(nullptr)
        , myOffset(GA_INVALID_OFFSET)
    {
        UT_ASSERT_COMPILETIME(std::is_const<PrimT>::value
				== std::is_const<DetailT>::value);
    }
    SYS_FORCE_INLINE
    GA_PrimitiveP_T(DetailT *detail, GA_Offset offset)
        : myDetail(detail)
        , myOffset(offset)
    {
    }
    template<typename OtherPrimT,typename OtherDetailT>
    SYS_FORCE_INLINE
    GA_PrimitiveP_T(const GA_PrimitiveP_T<OtherPrimT,OtherDetailT> &that)
        : myDetail(that.myDetail)
        , myOffset(that.myOffset)
    {
        // Can't assign (GA_Primitive*) = (const GA_Primitive*)
        // All 3 other combinations are fine.
        UT_ASSERT_COMPILETIME(std::is_const<PrimT>::value
				|| !std::is_const<OtherPrimT>::value);
    }
    SYS_FORCE_INLINE
    GA_PrimitiveP_T(PrimT *that)
        : myDetail(that ? (DetailT*)&that->getDetail() : nullptr)
        , myOffset(that ? that->getMapOffset() : GA_INVALID_OFFSET)
    {}
    template<typename OtherPrimT,typename OtherDetailT>
    SYS_FORCE_INLINE
    GA_PrimitiveP_T &operator=(const GA_PrimitiveP_T<OtherPrimT,OtherDetailT> &that)
    {
        // Can't assign (GA_Primitive*) = (const GA_Primitive*)
        // All 3 other combinations are fine.
        UT_ASSERT_COMPILETIME(std::is_const<PrimT>::value
				|| !std::is_const<OtherPrimT>::value);
        myDetail = that.myDetail;
        myOffset = that.myOffset;
    }
    SYS_FORCE_INLINE
    GA_PrimitiveP_T &operator=(PrimT *that)
    {
        if (that == nullptr)
        {
            myDetail = nullptr;
            myOffset = GA_INVALID_OFFSET;
        }
        else
        {
            myDetail = (DetailT*)&that->getDetail();
            myOffset = that->getMapOffset();
        }
        return *this;
    }
    template<typename OtherPrimT,typename OtherDetailT>
    SYS_FORCE_INLINE
    bool operator==(const GA_PrimitiveP_T<OtherPrimT,OtherDetailT> &that) const
    {
        return myDetail == that.myDetail && myOffset == that.myOffset;
    }
    template<typename OtherPrimT,typename OtherDetailT>
    SYS_FORCE_INLINE
    bool operator!=(const GA_PrimitiveP_T<OtherPrimT,OtherDetailT> &that) const
    {
        return !((*this) == that);
    }
    SYS_FORCE_INLINE
    bool operator==(const PrimT *that) const
    {
        if (that == nullptr)
            return !(*this);
        return myDetail == &that->getDetail() && myOffset == that->getMapOffset();
    }
    SYS_FORCE_INLINE
    bool operator!=(const PrimT *that) const
    {
        return !((*this) == that);
    }
    SYS_FORCE_INLINE
    SYS_SAFE_BOOL operator bool() const
    {
        return myDetail && GAisValid(myOffset);
    }
    SYS_FORCE_INLINE
    bool operator!() const
    {
        return !myDetail || !GAisValid(myOffset);
    }
    SYS_FORCE_INLINE
    PrimT &operator*() const
    {
        UT_ASSERT_P(myDetail && GAisValid(myOffset));
        return *(PrimT *)myDetail->getPrimitive(myOffset);
    }
    SYS_FORCE_INLINE
    PrimT *operator->() const
    {
        UT_ASSERT_P(myDetail && GAisValid(myOffset));
        return (PrimT *)myDetail->getPrimitive(myOffset);
    }
    SYS_FORCE_INLINE
    operator PrimT*() const
    {
        if (!myDetail || !GAisValid(myOffset))
            return nullptr;
        return (PrimT *)myDetail->getPrimitive(myOffset);
    }
    template <typename OtherPrimT>
    SYS_FORCE_INLINE
    OtherPrimT castPtr() const
    {
        if (!myDetail || !GAisValid(myOffset))
            return nullptr;
        return UTverify_cast<OtherPrimT>(myDetail->getPrimitive(myOffset));
    }
    SYS_FORCE_INLINE
    DetailT *detail() const
    {
        return myDetail;
    }
    SYS_FORCE_INLINE
    DetailT &detailRef() const
    {
	UT_ASSERT_P(myDetail);
        return *myDetail;
    }
    SYS_FORCE_INLINE
    GA_Offset offset() const
    {
        return myOffset;
    }
    void set(DetailT *detail, GA_Offset offset)
    {
        myDetail = detail;
        myOffset = offset;
    }
    SYS_FORCE_INLINE
    int getTypeId() const
    {
	return myDetail->getPrimitiveTypeId(myOffset);
    }
    SYS_FORCE_INLINE
    GA_PrimCompat::TypeMask getPrimitiveId() const
    {
	return GA_Primitive::primCompatMaskFromTypeId(getTypeId());
    }
    SYS_FORCE_INLINE
    bool isClosed() const
    {
	return myDetail->getPrimitiveClosedFlag(myOffset);
    }

private:
    DetailT *myDetail;
    GA_Offset myOffset;
};

using GA_ConstPrimitiveP = GA_PrimitiveP_T<const GA_Primitive, const GA_Detail>;
using GA_PrimitiveP = GA_PrimitiveP_T<GA_Primitive, GA_Detail>;
