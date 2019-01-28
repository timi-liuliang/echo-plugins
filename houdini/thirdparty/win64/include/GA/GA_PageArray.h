/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_PageArray.h (GA Library, C++)
 *
 * COMMENTS:    An array class with special handling of constant pages and
 *              shared page data, specialized for GA_Offset.
 */

#pragma once

#ifndef __GA_PageArray__
#define __GA_PageArray__

#include "GA_Defaults.h"
#include "GA_Iterator.h"
#include "GA_Range.h"
#include "GA_Types.h"
#include <UT/UT_Assert.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_FixedVector.h>
#include <UT/UT_JSONDefines.h>
#include <UT/UT_PageArray.h>
#include <UT/UT_Storage.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

class GA_Defragment;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveOptions;
class UT_JSONParser;
class UT_JSONWriter;
template <typename T> class UT_Array;

SYS_FORCE_INLINE
GA_Storage UTStorageToGAStorage(UT_Storage storage)
{
    switch (storage)
    {
        case UT_Storage::INVALID:
            return GA_STORE_INVALID;
        case UT_Storage::INT8:
            return GA_STORE_INT8;
        case UT_Storage::INT16:
            return GA_STORE_INT16;
        case UT_Storage::INT32:
            return GA_STORE_INT32;
        case UT_Storage::INT64:
            return GA_STORE_INT64;
        case UT_Storage::REAL16:
            return GA_STORE_REAL16;
        case UT_Storage::REAL32:
            return GA_STORE_REAL32;
        case UT_Storage::REAL64:
            return GA_STORE_REAL64;
    }
    UT_ASSERT_MSG_P(0, "Unhandled UT_Storage value!");
    return GA_STORE_INVALID;
}

SYS_FORCE_INLINE
UT_Storage GAStorageToUTStorage(GA_Storage storage)
{
    switch (storage)
    {
        case GA_STORE_BOOL:
        case GA_STORE_INVALID:
        case GA_STORE_STRING:
            return UT_Storage::INVALID;
        case GA_STORE_INT8:
        case GA_STORE_UINT8:
            return UT_Storage::INT8;
        case GA_STORE_INT16:
            return UT_Storage::INT16;
        case GA_STORE_INT32:
            return UT_Storage::INT32;
        case GA_STORE_INT64:
            return UT_Storage::INT64;
        case GA_STORE_REAL16:
            return UT_Storage::REAL16;
        case GA_STORE_REAL32:
            return UT_Storage::REAL32;
        case GA_STORE_REAL64:
            return UT_Storage::REAL64;
    }
    UT_ASSERT_MSG_P(0, "Unhandled GA_Storage value!");
    return UT_Storage::INVALID;
}

template<typename DATA_T=void,exint TSIZE=-1,bool TABLEHARDENED=true,bool PAGESHARDENED=false>
class GA_PageArray : public UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, GA_PAGE_BITS, GA_Offset>
{
public:
    typedef UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, GA_PAGE_BITS, GA_Offset> Base;
    typedef GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED> ThisType;
    using Base::get;
    using Base::getPageData;
    using Base::getRange;
    using Base::getTupleSize;
    using Base::getVector;
    using Base::hardenPage;
    using Base::hardenPageNoInit;
    using Base::isPageConstant;
    using Base::moveRange;
    using Base::numPages;
    using Base::set;
    using Base::setConstant;
    using Base::setPageConstant;
    using Base::setRange;
    using Base::setSize;
    using Base::setVector;
    using Base::size;
    using Base::swapRange;
    using Base::thePageSize;
    using Base::thePageMask;
    using Base::tryCompressPage;
    using Base::hardenTable;
private:
    using typename Base::NotVoidType;
    using Base::theSafeTupleSize;
    using Base::isEqual;
    using Base::isZero;
public:

    /// The default constructor can only be used if the tuple size
    /// and storage type are known at compile time.
    /// Unfortunately, this can't be asserted at compile time, because
    /// compilers aren't all that bright, but Base asserts it at runtime
    /// if paranoid asserts are on.
    GA_PageArray() : Base()
    {}

    /// This constructor can only be used if the storage type is
    /// known at compile time, but the tuple size is not.
    GA_PageArray(exint tuplesize) : Base(tuplesize)
    {}

    /// This constructor can only be used if the storage type is
    /// known at compile time, but the tuple size is not.
    GA_PageArray(GA_Storage storage) : Base(GAStorageToUTStorage(storage))
    {}

    /// This constructor can only be used if the tuple size and
    /// storage type are unknown at compile time.
    GA_PageArray(exint tuplesize, UT_Storage storage) : Base(tuplesize, storage)
    {}

    /// This constructor can only be used if the tuple size and
    /// storage type are unknown at compile time.
    GA_PageArray(exint tuplesize, GA_Storage storage) : Base(tuplesize, GAStorageToUTStorage(storage))
    {}

    /// Get the storage type for each component of this GA_PageArray
    SYS_FORCE_INLINE
    GA_Storage getStorage() const
    {
        UT_Storage storage = Base::getStorage();
        return UTStorageToGAStorage(storage);
    }
    /// Set the storage type for each component of this GA_PageArray
    void setStorage(GA_Storage storage)
    {
        UT_Storage utstorage = GAStorageToUTStorage(storage);
        Base::setStorage(utstorage);
    }

    template<typename SRC_T>
    void setConstant(const GA_Range &range, SRC_T val)
    {
        if (!SYSisSame<DATA_T,void>())
        {
            // Easy case, where the storage type is known at compile time.
            NotVoidType dval(val);

            GA_Offset start;
            GA_Offset end;
            for (GA_Iterator it(range); it.fullBlockAdvance(start, end); )
            {
                Base::setConstant(start, end, dval);
            }
            return;
        }

        // Hard case, where the storage type is not known at compile time.
        UT_Storage storage = Base::getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().setConstant(range, int8(val)); return;
            case UT_Storage::INT16:
                castType<int16>().setConstant(range, int16(val)); return;
            case UT_Storage::INT32:
                castType<int32>().setConstant(range, int32(val)); return;
            case UT_Storage::INT64:
                castType<int64>().setConstant(range, int64(val)); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setConstant(range, fpreal16(val)); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setConstant(range, fpreal32(val)); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setConstant(range, fpreal64(val)); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                return;
        }
    }

    void setConstant(const GA_Range &range, const GA_Defaults &defaults)
    {
        if (!SYSisSame<DATA_T,void>())
        {
            GA_Offset start;
            GA_Offset end;
            for (GA_Iterator it(range); it.fullBlockAdvance(start, end); )
            {
                Base::setConstant(start, end, defaults);
            }
            return;
        }

        // Hard case, where the storage type is not known at compile time.
        UT_Storage storage = Base::getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().setConstant(range, defaults); return;
            case UT_Storage::INT16:
                castType<int16>().setConstant(range, defaults); return;
            case UT_Storage::INT32:
                castType<int32>().setConstant(range, defaults); return;
            case UT_Storage::INT64:
                castType<int64>().setConstant(range, defaults); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setConstant(range, defaults); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setConstant(range, defaults); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setConstant(range, defaults); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                return;
        }
    }

    SYS_FORCE_INLINE
    void setConstant(GA_Offset start, GA_Offset end, const GA_Defaults &v)
    {
        Base::setConstant(start, end, v);
    }

    template<typename SRC_T>
    void setConstant(GA_Offset start, GA_Offset end, SRC_T val)
    {
        if (!SYSisSame<DATA_T,void>())
        {
            // Easy case, where the storage type is known at compile time.
            NotVoidType dval(val);

            Base::setConstant(start, end, dval);
            return;
        }

        // Hard case, where the storage type is not known at compile time.
        UT_Storage storage = Base::getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().setConstant(start, end, int8(val)); return;
            case UT_Storage::INT16:
                castType<int16>().setConstant(start, end, int16(val)); return;
            case UT_Storage::INT32:
                castType<int32>().setConstant(start, end, int32(val)); return;
            case UT_Storage::INT64:
                castType<int64>().setConstant(start, end, int64(val)); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setConstant(start, end, fpreal16(val)); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setConstant(start, end, fpreal32(val)); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setConstant(start, end, fpreal64(val)); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                return;
        }
    }

    template<typename SRC_T>
    void setConstantVector(const GA_Range &range, const UT_FixedVector<SRC_T,theSafeTupleSize> &val)
    {
        if (!SYSisSame<DATA_T,void>())
        {
            // Easy case, where the storage type is known at compile time.
            const UT_FixedVector<NotVoidType,theSafeTupleSize> dval(val);

            GA_Offset start;
            GA_Offset end;
            for (GA_Iterator it(range); it.fullBlockAdvance(start, end); )
            {
                Base::setConstant(start, end, dval);
            }
            return;
        }

        // Hard case, where the storage type is not known at compile time.
        UT_Storage storage = Base::getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().setConstantVector(range, UT_FixedVector<int8,theSafeTupleSize>(val)); return;
            case UT_Storage::INT16:
                castType<int16>().setConstantVector(range, UT_FixedVector<int16,theSafeTupleSize>(val)); return;
            case UT_Storage::INT32:
                castType<int32>().setConstantVector(range, UT_FixedVector<int32,theSafeTupleSize>(val)); return;
            case UT_Storage::INT64:
                castType<int64>().setConstantVector(range, UT_FixedVector<int64,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setConstantVector(range, UT_FixedVector<fpreal16,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setConstantVector(range, UT_FixedVector<fpreal32,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setConstantVector(range, UT_FixedVector<fpreal64,theSafeTupleSize>(val)); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                return;
        }
    }

    template<typename SRC_T>
    void setConstantVector(GA_Offset start, GA_Offset end, const UT_FixedVector<SRC_T,theSafeTupleSize> &val)
    {
        if (!SYSisSame<DATA_T,void>())
        {
            // Easy case, where the storage type is known at compile time.
            const UT_FixedVector<NotVoidType,theSafeTupleSize> dval(val);

            Base::setConstant(start, end, dval);
            return;
        }

        // Hard case, where the storage type is not known at compile time.
        UT_Storage storage = Base::getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().setConstantVector(start, end, UT_FixedVector<int8,theSafeTupleSize>(val)); return;
            case UT_Storage::INT16:
                castType<int16>().setConstantVector(start, end, UT_FixedVector<int16,theSafeTupleSize>(val)); return;
            case UT_Storage::INT32:
                castType<int32>().setConstantVector(start, end, UT_FixedVector<int32,theSafeTupleSize>(val)); return;
            case UT_Storage::INT64:
                castType<int64>().setConstantVector(start, end, UT_FixedVector<int64,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setConstantVector(start, end, UT_FixedVector<fpreal16,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setConstantVector(start, end, UT_FixedVector<fpreal32,theSafeTupleSize>(val)); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setConstantVector(start, end, UT_FixedVector<fpreal64,theSafeTupleSize>(val)); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                return;
        }
    }

    /// Include GA_PageArrayImpl.h to call this.
    void defragment(const GA_Defragment &defrag);

    /// Include GA_PageArrayImpl.h to call this.
    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    void mergeGrowArrayAndCopy(const GA_MergeMap &map, GA_AttributeOwner owner, const GA_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED> &src, const GA_Defaults &defaults);

    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    void copy(const GA_Range &destrange, const GA_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED> &src, const GA_Range &srcrange)
    {
        GA_Iterator destit(destrange);
        GA_Iterator srcit(srcrange);

        GA_Offset deststart = GA_INVALID_OFFSET;
        GA_Offset destend = GA_INVALID_OFFSET;
        GA_Offset srcstart = GA_INVALID_OFFSET;
        GA_Offset srcend = GA_INVALID_OFFSET;

        while (true)
        {
            // If either range runs out, stop.
            if (deststart == destend)
            {
                if (!destit.fullBlockAdvance(deststart, destend))
                    break;
            }
            if (srcstart == srcend)
            {
                if (!srcit.fullBlockAdvance(srcstart, srcend))
                    break;
            }

            GA_Offset nelements = SYSmin(destend-deststart, srcend-srcstart);

            moveRange(src, srcstart, deststart, nelements);

            deststart += nelements;
            srcstart += nelements;
        }
    }

    template<typename T>
    void getRange(const GA_Range &srcrange, T *dest) const
    {
        if (SYSisSame<DATA_T,void>())
        {
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    castType<int8>().getRange(srcrange, dest); return;
                case UT_Storage::INT16:
                    castType<int16>().getRange(srcrange, dest); return;
                case UT_Storage::INT32:
                    castType<int32>().getRange(srcrange, dest); return;
                case UT_Storage::INT64:
                    castType<int64>().getRange(srcrange, dest); return;
                case UT_Storage::REAL16:
                    castType<fpreal16>().getRange(srcrange, dest); return;
                case UT_Storage::REAL32:
                    castType<fpreal32>().getRange(srcrange, dest); return;
                case UT_Storage::REAL64:
                    castType<fpreal64>().getRange(srcrange, dest); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    return;
            }
            return;
        }

        int tuplesize = getTupleSize();

        // Cast to optimize for small tuple sizes
        if (TSIZE == -1 && tuplesize <= 3)
        {
            if (tuplesize == 1)
                castTupleSize<1>().getRange(srcrange, dest);
            else if (tuplesize == 2)
                castTupleSize<2>().getRange(srcrange, dest);
            else if (tuplesize == 3)
                castTupleSize<3>().getRange(srcrange, dest);
            return;
        }

        GA_Offset start;
        GA_Offset end;
        for (GA_Iterator it(srcrange); it.fullBlockAdvance(start, end); )
        {
            GA_Offset nelements = end-start;
            getRange(start, nelements, dest);
            dest += GA_Size(nelements);
        }
    }

    template<typename T>
    void setRange(const GA_Range &destrange, const T *src)
    {
        if (SYSisSame<DATA_T,void>())
        {
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    castType<int8>().setRange(destrange, src); return;
                case UT_Storage::INT16:
                    castType<int16>().setRange(destrange, src); return;
                case UT_Storage::INT32:
                    castType<int32>().setRange(destrange, src); return;
                case UT_Storage::INT64:
                    castType<int64>().setRange(destrange, src); return;
                case UT_Storage::REAL16:
                    castType<fpreal16>().setRange(destrange, src); return;
                case UT_Storage::REAL32:
                    castType<fpreal32>().setRange(destrange, src); return;
                case UT_Storage::REAL64:
                    castType<fpreal64>().setRange(destrange, src); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    return;
            }
            return;
        }

        int tuplesize = getTupleSize();

        // Cast to optimize for small tuple sizes
        if (TSIZE == -1 && tuplesize <= 3)
        {
            if (tuplesize == 1)
                castTupleSize<1>().setRange(destrange, src);
            else if (tuplesize == 2)
                castTupleSize<2>().setRange(destrange, src);
            else if (tuplesize == 3)
                castTupleSize<3>().setRange(destrange, src);
            return;
        }

        GA_Offset start;
        GA_Offset end;
        for (GA_Iterator it(destrange); it.fullBlockAdvance(start, end); )
        {
            GA_Offset nelements = end-start;
            setRange(start, nelements, src);
            src += GA_Size(nelements);
        }
    }

    /// For each page, this sets a bit in bits iff there's a chance that
    /// the data in a page of this may be different from the data in the
    /// corresponding page of that.  It will not clear bits.
    /// bits must be pre-sized to at least the number of pages in
    /// this, (which must be equal to the size of that).  that must
    /// have the same tuple size and storage type as this.
    /// It will only check pointers and constant values.  It will not
    /// compare full pages of data.
    void comparePages(UT_BitArray &bits, const ThisType &that) const
    {
        // Calling getPageData requires that DATA_T be defined, so switch.
        if (SYSisSame<DATA_T,void>())
        {
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    castType<int8>().comparePages(bits, that.castType<int8>()); return;
                case UT_Storage::INT16:
                    castType<int16>().comparePages(bits, that.castType<int16>()); return;
                case UT_Storage::INT32:
                    castType<int32>().comparePages(bits, that.castType<int32>()); return;
                case UT_Storage::INT64:
                    castType<int64>().comparePages(bits, that.castType<int64>()); return;
                case UT_Storage::REAL16:
                    castType<fpreal16>().comparePages(bits, that.castType<fpreal16>()); return;
                case UT_Storage::REAL32:
                    castType<fpreal32>().comparePages(bits, that.castType<fpreal32>()); return;
                case UT_Storage::REAL64:
                    castType<fpreal64>().comparePages(bits, that.castType<fpreal64>()); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    return;
            }
            return;
        }

        UT_ASSERT(getTupleSize() == that.getTupleSize());
        UT_ASSERT(getStorage() == that.getStorage());

        UT_PageNum npages = numPages(size());
        UT_ASSERT(npages == numPages(that.size()));
        for (UT_PageNum pagei = 0; pagei < npages; ++pagei)
        {
            const NotVoidType *thisdata = getPageData(pagei);
            const NotVoidType *thatdata = that.getPageData(pagei);
            if (thisdata == thatdata)
                continue;

            if (!thisdata || !thatdata ||
                !isPageConstant(pagei) || !that.isPageConstant(pagei) ||
                !isEqual(thisdata, thatdata, getTupleSize()))
            {
                bits.setBitFast(pagei, true);
            }
        }
    }

    /// Save data to a JSON stream
    /// Include GA_PageArrayImpl.h to call this.
    bool jsonSave(UT_JSONWriter &w, const GA_Range &range,
        const GA_SaveOptions *options = nullptr,
        const UT_IntArray *map = nullptr, int defvalue=-1) const;

    /// Load data from a JSON stream
    /// Include GA_PageArrayImpl.h to call this.
    bool jsonLoad(UT_JSONParser &p,
        const GA_LoadMap &map, GA_AttributeOwner owner);

private:
    /// These are just used internally by jsonSave
    /// @{
    template<typename MAP_ARRAY_CLASS>
    bool jsonSaveConstantOutputPageFlags(
        UT_JSONWriter &w, const GA_Range &range,
        UT_UniquePtr<UT_BitArray> &const_page_flags) const;
    static bool
    jsonWriteDataSpan(
        UT_JSONWriter &w,
        const NotVoidType *page_data,
        exint length, exint tuplesize,
        bool const_output, bool const_input,
        const UT_IntArray *map, int defvalue,
        NotVoidType *buffer);
    bool jsonSaveRawPageData(
        UT_JSONWriter &w, const GA_Range &range,
        const UT_BitArray *const_page_flags,
        UT_JID jid_storage,
        const UT_IntArray *map, int defvalue) const;

    class ga_SubPageBlock;

    static void buildOutputToInternalPageMap(
        const GA_Range &range,
        UT_Array<GA_PageNum> &map);
    static void buildOutputToInternalPageMap(
        const GA_Range &range,
        UT_Array<ga_SubPageBlock> &map);
    GA_Size marshallConstantFlagsForOutputPages(
        const UT_Array<GA_PageNum> &map,
        UT_BitArray &flags) const;
    GA_Size marshallConstantFlagsForOutputPages(
        const UT_Array<ga_SubPageBlock> &map,
        UT_BitArray &flags) const;
    SYS_FORCE_INLINE
    static bool isSubPageConstant(
        const NotVoidType *page,
        GA_PageOff start, GA_PageOff end,
        const exint tuplesize,
        const NotVoidType *value);

    template<bool ARRAY_OF_ARRAYS>
    bool jsonSaveAsArray(UT_JSONWriter &w, const GA_Range &range, UT_JID jid_storage,
        const UT_IntArray *map, int defvalue) const;
    /// @}


    /// These are just used internally by jsonLoad
    /// @{
    class LoadComponentArrayFunctor;

    bool jsonLoadRawPageData(
        UT_JSONParser &p,
        const GA_LoadMap &map,
        GA_AttributeOwner owner,
        GA_Size page_size,
        const int *packing,
        int n_packing_entries,
        const UT_UniquePtr<UT_BitArray> *const constant_page_flags);
    /// @}

    SYS_FORCE_INLINE
    static UT_JID GAStorageToJID(GA_Storage storage);

public:

    /// This is for setting the DATA_T template after checking getStorage(),
    /// if DATA_T wasn't already known.
    /// @{
    template<typename DEST_DATA_T>
    SYS_FORCE_INLINE const GA_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED> &
    castType() const
    {
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()) || (Base::getStorage() != UT_Storage::INVALID && Base::getStorage() == UT_StorageNum<DEST_DATA_T>::theStorage));
        return *(const GA_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED> *)this;
    }
    template<typename DEST_DATA_T>
    SYS_FORCE_INLINE GA_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED> &
    castType()
    {
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()) || (Base::getStorage() != UT_Storage::INVALID && Base::getStorage() == UT_StorageNum<DEST_DATA_T>::theStorage));
        return *(GA_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED> *)this;
    }
    /// @}

    /// This is for setting the TSIZE template after checking getTupleSize(),
    /// if TSIZE wasn't already known.
    /// @{
    template<exint DEST_TSIZE>
    SYS_FORCE_INLINE const GA_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED> &
    castTupleSize() const
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == DEST_TSIZE);
        UT_ASSERT_P(getTupleSize() == DEST_TSIZE);
        return *(const GA_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED> *)this;
    }
    template<exint DEST_TSIZE>
    SYS_FORCE_INLINE GA_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED> &
    castTupleSize()
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == DEST_TSIZE);
        UT_ASSERT_P(getTupleSize() == DEST_TSIZE);
        return *(GA_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED> *)this;
    }
    /// @}
};

#endif
