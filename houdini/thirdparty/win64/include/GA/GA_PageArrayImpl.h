/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_PageArrayImpl.h (GA Library, C++)
 *
 * COMMENTS:    An array class with special handling of constant pages and
 *              shared page data, specialized for GA_Offset.
 */

#pragma once

#ifndef __GA_PageArrayImpl__
#define __GA_PageArrayImpl__

#include "GA_PageArray.h"

#include "GA_API.h"
#include "GA_Defaults.h"
#include "GA_Defragment.h"
#include "GA_Iterator.h"
#include "GA_LoadMap.h"
#include "GA_MergeMap.h"
#include "GA_Range.h"
#include "GA_SaveOptions.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_FixedVector.h>
#include <UT/UT_JSONDefines.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONWriter.h>
#include <UT/UT_StackBuffer.h>
#include <UT/UT_Storage.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_CallIf.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

#include <string.h>


// Separate namespace for these, because they shouldn't be duplicated per
// template instantiation.
namespace GA_PageArrayIO
{
    // JSON tokens
    enum JDTupleToken
    {
        GA_JDTUPLE_SIZE,
        GA_JDTUPLE_STORAGE,
        GA_JDTUPLE_ARRAYS,
        GA_JDTUPLE_TUPLES,
        GA_JDTUPLE_PAGESIZE,
        GA_JDTUPLE_PACKING,
        GA_JDTUPLE_CONSTPAGEFLAGS,
        GA_JDTUPLE_RAWPAGEDATA
    };
    GA_API const char *getJSONToken(JDTupleToken tokenID);
    GA_API JDTupleToken getJSONTokenID(const char *token);
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
void
GA_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED>::defragment(const GA_Defragment &defrag)
{
    auto &hard = hardenTable();
    for (GA_Defragment::const_iterator it=defrag.begin(); !it.atEnd(); ++it)
    {
        GA_Offset a = it.getA();
        GA_Offset b = it.getB();
        GA_Size n = it.getN();
        switch (it.getOp())
        {
            case GA_Defragment::SWAP_AB:
                hard.swapRange(a, b, GA_Offset(n));
                break;
            case GA_Defragment::MOVE_A_TO_B:
                hard.moveRange(a, b, GA_Offset(n));
                break;
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
void
GA_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED>::mergeGrowArrayAndCopy(
    const GA_MergeMap &map,
    GA_AttributeOwner owner,
    const GA_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED> &src,
    const GA_Defaults &defaults)
{
    if (SYSisSame<DATA_T,void>())
    {
        SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,&map,owner,&src,&defaults](SYS_CALLIF_AUTO){
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = this->Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    this->castType<int8>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::INT16:
                    this->castType<int16>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::INT32:
                    this->castType<int32>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::INT64:
                    this->castType<int64>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::REAL16:
                    this->castType<fpreal16>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::REAL32:
                    this->castType<fpreal32>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::REAL64:
                    this->castType<fpreal64>().mergeGrowArrayAndCopy(map, owner, src, defaults); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    return;
            }
        });
        return;
    }

    UT_IF_ASSERT( GA_Offset osize = map.getDestInitCapacity(owner); )
    GA_Offset nsize = map.getDestCapacity(owner);

    // Ideally we could assert that capacity() == ocapacity, but this method is
    // sometimes called by implementations of GA_AIFMerge::copyArray(),
    // after GA_AIFMerge::growArray() has already been called.
    UT_ASSERT(osize <= size());
    UT_ASSERT(osize <= nsize || (osize == GA_Offset(0) && nsize <= GA_Offset(0)));

    if (nsize <= GA_Offset(0))
        return;

    GA_Offset dststart = map.getDestStart(owner);
    GA_Offset dstend = map.getDestEnd(owner)+1;

    UT_ASSERT(dstend - dststart <= src.size());
    UT_ASSERT(GAisValid(dststart) && dststart < nsize);
    UT_ASSERT(GAisValid(dstend) && dstend <= nsize);
    UT_ASSERT(dststart < dstend);

    UT_ASSERT_MSG(GAgetPageOff(dststart) == 0, "mergeGrowArrayAndCopy should only be used when dststart is at a page boundary");
    if (nsize > size())
    {
        setSize(nsize, defaults);
    }

    // As odd as it may seem, apparently mergeGrowArrayAndCopy has only ever
    // supported copying from source offset 0 onward, regardless of
    // map.getSourceRange(owner).  For example, GA_DataArray::
    // mergeGrowArrayAndCopy and GA_DataBitArray::mergeGrowArrayAndCopy
    // both assume this too.
    moveRange(src, GA_Offset(0), dststart, dstend - dststart);
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonSave(
    UT_JSONWriter &w, const GA_Range &range,
    const GA_SaveOptions *options,
    const UT_IntArray *map, int defvalue) const
{
    if (SYSisSame<DATA_T,void>())
    {
        bool success;
        SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,&w,&range,options,map,defvalue,&success](SYS_CALLIF_AUTO){
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = this->Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    success = this->castType<int8>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::INT16:
                    success = this->castType<int16>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::INT32:
                    success = this->castType<int32>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::INT64:
                    success = this->castType<int64>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::REAL16:
                    success = this->castType<fpreal16>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::REAL32:
                    success = this->castType<fpreal32>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::REAL64:
                    success = this->castType<fpreal64>().jsonSave(w, range, options, map, defvalue); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    success = false;
                    return;
            }
            success = false;
        });
        return success;
    }

    int tuplesize = getTupleSize();

    // Cast to optimize for small tuple sizes
    if (TSIZE == -1 && tuplesize <= 3 && tuplesize >= 1)
    {
        bool success;
        SYS_CallIf<TSIZE == -1>::call([this,&w,&range,options,map,defvalue,tuplesize,&success](SYS_CALLIF_AUTO){
            if (tuplesize == 3)
                success = this->castTupleSize<3>().jsonSave(w, range, options, map, defvalue);
            else if (tuplesize == 1)
                success = this->castTupleSize<1>().jsonSave(w, range, options, map, defvalue);
            else
            {
                UT_ASSERT_P(tuplesize == 2);
                success = this->castTupleSize<2>().jsonSave(w, range, options, map, defvalue);
            }
        });
        return success;
    }

    GA_Storage ga_storage = getStorage();
    if (map && !GAisIntStorage(ga_storage))
        map = nullptr;

    UT_JID jid = GAStorageToJID(ga_storage);

    bool ok = true;

    ok = ok && w.jsonBeginArray();

    ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_SIZE));
    ok = ok && w.jsonInt(tuplesize);

    ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_STORAGE));
    ok = ok && w.jsonStringToken(GAstorage(ga_storage));

    bool savepaged = w.getBinary();
    if (options)
        options->importSavePaged(savepaged);

    if (savepaged)
    {
        ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_PAGESIZE));
        UT_ASSERT_COMPILETIME(thePageSize == GA_PAGE_SIZE);
        ok = ok && w.jsonInt(thePageSize);

#if 0
        // For max compatibility with GA_DataArrayTuple, we try to match the old
        // packing behaviour: 1; 2 -> 1,1; 3; 4 -> 3,1; 5 -> 3,1,1; 6 -> 3,1,1,1
        // though only for fpreal32 and fpreal64 types.  Every other type
        // had each component stored separately.
        //
        // TODO: Check if older versions will load data that is saved
        //       with everything as array-of-structs, avoiding the
        //       need for this.
        bool hasfirst3packed = (tuplesize >= 3) &&
            (ga_storage == GA_STORE_REAL32 || ga_storage == GA_STORE_REAL64);

        // The GA_JDTUPLE_PACKING field is optional and only needed if we
        // need a data layout other than the default array-of-structs.
        int n_packing_entries = tuplesize - (hasfirst3packed ? 2 : 0);
        if (n_packing_entries > 1)
        {
            ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_PACKING));
            ok = ok && w.beginUniformArray(n_packing_entries, UT_JID_UINT8);

            // First is 3 or 1; every other one is 1.
            ok = ok && w.uniformWrite(uint8(hasfirst3packed ? 3 : 1));
            for (int i = 1; i < n_packing_entries; i++)
            {
                ok = ok && w.uniformWrite(uint8(1));
            }

            ok = ok && w.endUniformArray();
        }
#else
        // I think GA_DataArrayTuple::jsonLoad supports loading
        // array-of-structs, regardless of the tuplesize, so let's try it
        // for now, and we can always fall back later.

        // I don't think the packing entry array is needed if there's only one entry.
#if 0
        int n_packing_entries = 1;

        ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_PACKING));
        ok = ok && w.beginUniformArray(n_packing_entries, UT_JID_INT32);
        ok = ok && w.uniformWrite(int32(tuplesize));
        ok = ok && w.endUniformArray();
#endif
#endif

        // constpagecheck:
        //  0 - none
        //  1 - use page state
        //  2 - full data scan
        exint const_page_check = 2;
        if (options)
            const_page_check = options->constPageCheck();

        UT_UniquePtr<UT_BitArray> const_page_flags(nullptr);
        if (tuplesize > 0)
        {
            if (const_page_check >= 2)
            {
                ok = ok && jsonSaveConstantOutputPageFlags<
                    UT_Array<ga_SubPageBlock> >(
                        w, range, const_page_flags);
            }
            else if (const_page_check == 1)
            {
                ok = ok && jsonSaveConstantOutputPageFlags<
                    UT_Array<GA_PageNum> >(
                        w, range, const_page_flags);
            }
        }

        ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_RAWPAGEDATA));

        ok = ok && jsonSaveRawPageData(w, range,
            const_page_flags.get(), jid, map, defvalue);
    }
    else if (tuplesize <= 1)
    {
        // No reason to save an array of tuples if it's a scalar
        ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_ARRAYS));
        ok = ok && w.jsonBeginArray();

        if (tuplesize != 0)
            ok = ok && jsonSaveAsArray<false>(w, range, jid, map, defvalue);

        ok = ok && w.jsonEndArray();
    }
    else
    {
        // Store as an array of structs
        ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_TUPLES));
        ok = ok && w.jsonBeginArray();

        ok = ok && jsonSaveAsArray<true>(w, range, jid, map, defvalue);

        ok = ok && w.jsonEndArray();
    }
    return ok && w.jsonEndArray();
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
template<typename MAP_ARRAY_CLASS>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonSaveConstantOutputPageFlags(
    UT_JSONWriter &w, const GA_Range &range, UT_UniquePtr<UT_BitArray> &output_page_flags) const
{
    bool ok = true;

    MAP_ARRAY_CLASS output_to_internal_page_map;
    buildOutputToInternalPageMap(range, output_to_internal_page_map);

    int64 n_output_pages = ((range.getEntries() + thePageSize-1) / thePageSize);
    UT_BitArray constant_flags(n_output_pages);

    GA_Size n_constant_pages = marshallConstantFlagsForOutputPages(
        output_to_internal_page_map, constant_flags);
    if (n_constant_pages == 0)
        return ok;

    ok = ok && w.jsonKeyToken(GA_PageArrayIO::getJSONToken(GA_PageArrayIO::GA_JDTUPLE_CONSTPAGEFLAGS));
    ok = ok && w.jsonBeginArray();

    ok = ok && w.jsonUniformArray(constant_flags.size(), constant_flags);
    output_page_flags.reset(new UT_BitArray);
    constant_flags.swap(*output_page_flags);

    ok = ok && w.jsonEndArray();

    return ok;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonWriteDataSpan(
    UT_JSONWriter &w,
    const NotVoidType *page_data,
    exint length, exint tuplesize,
    bool const_output, bool const_input,
    const UT_IntArray *map, int defvalue,
    NotVoidType *buffer)
{
    // NOTE: nullptr page_data should be dealt with by caller, using buffer.
    UT_ASSERT_P(page_data);

    if (!const_output && !const_input)
    {
        if (!map)
        {
            // Simple case
            return w.uniformBlockWrite(page_data, length * tuplesize);
        }
        else
        {
            for (exint i = 0; i < length; ++i)
            {
                for (exint component = 0; component < tuplesize; ++component, ++page_data)
                {
                    NotVoidType val = *page_data;
                    buffer[component] = (val < 0 || val >= map->size())
                                      ? defvalue
                                      : (*map)(val);
                }
                if (!w.uniformBlockWrite(buffer, tuplesize))
                    return false;
            }
            return true;
        }
    }

    // Every case left has a single input value to read
    const NotVoidType *data = page_data;
    if (map)
    {
        for (exint component = 0; component < tuplesize; ++component, ++page_data)
        {
            NotVoidType val = *page_data;
            buffer[component] = (val < 0 || val >= map->size())
                              ? defvalue
                              : (*map)(val);
        }
        data = buffer;
    }

    if (const_output)
    {
        return w.uniformBlockWrite(data, tuplesize);
    }
    else
    {
        // const_input and !const_output, so repeat same tuple, length times
        for (exint i = 0; i < length; ++i)
        {
            if (!w.uniformBlockWrite(data, tuplesize))
                return false;
        }
        return true;
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonSaveRawPageData(
    UT_JSONWriter &w, const GA_Range &range,
    const UT_BitArray *const_page_flags,
    UT_JID jid_storage,
    const UT_IntArray *map, int defvalue) const
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    exint ntotal = range.getEntries();

    exint collapsedsize = ntotal;
    if (const_page_flags && ntotal > 0)
    {
        exint n_const_pages = const_page_flags->numBitsSet();

        // Special handling for last page, since it's not always the same size
        if (const_page_flags->getBitFast(const_page_flags->size()-1))
        {
            collapsedsize = (const_page_flags->size()-n_const_pages)*thePageSize
                          + n_const_pages;
        }
        else
        {
            // NOTE: ((ntotal-1) & thePageMask) + 1 ensures that we get
            //       thePageSize if ntotal is a multiple of thePageSize.
            collapsedsize = (const_page_flags->size()-n_const_pages-1)*thePageSize
                          + n_const_pages
                          + ((ntotal-1) & thePageMask) + 1;
        }
    }
    const exint tuplesize = getTupleSize();
    collapsedsize *= tuplesize;

    bool ok = true;
    ok = ok && w.beginUniformArray(collapsedsize, jid_storage);

    // Don't even try to go through the pages if tuplesize is 0.
    // Only bugs will ensue.  Might as well check this implicitly
    // by checking collapsedsize, since it's multiplied by tuplesize.
    if (collapsedsize == 0)
    {
        ok = ok && w.endUniformArray();
        return ok;
    }

    bool const_page_data;
    const NotVoidType *page_data;
    UT_StackBuffer<NotVoidType> buffer(tuplesize);
    const GA_Size n_output_pages = (ntotal+thePageSize-1) / thePageSize;

    GA_Iterator it(range);
    GA_PageNum last_page_num(-1);
    GA_Offset block_start = GA_INVALID_OFFSET;
    GA_PageOff block_start_pageoff;
    GA_Offset block_end = GA_INVALID_OFFSET;
    for (GA_Size output_page_num = 0; ok && output_page_num < n_output_pages; ++output_page_num)
    {
        const bool output_page_const = const_page_flags && const_page_flags->getBitFast(output_page_num);

        GA_Size output_page_offset = 0;
        do
        {
            if (block_start == block_end)
            {
                bool more_data = it.blockAdvance(block_start, block_end);
                if (!more_data)
                {
                    UT_ASSERT_P(output_page_num == n_output_pages-1);
                    UT_ASSERT_P(GA_Size(GAgetPageOff(GA_Offset(ntotal))) == output_page_offset);
                    break;
                }

                GA_PageNum page_num = GAgetPageNum(block_start);
                block_start_pageoff = GAgetPageOff(block_start);

                // Fetch the page data if we don't already have it.
                if (page_num != last_page_num)
                {
                    const_page_data = isPageConstant(page_num);
                    page_data = getPageData(page_num);

                    // Deal with nullptr here, to avoid having to deal with it in
                    // multiple codepaths.
                    if (!page_data)
                    {
                        UT_ASSERT_P(const_page_data);
                        memset((NotVoidType*)buffer,0,sizeof(NotVoidType)*tuplesize);
                        page_data = (NotVoidType*)buffer;
                    }
                    last_page_num = page_num;
                }
            }

            const GA_Size copy_size = SYSmin(GA_Size(block_end-block_start), thePageSize-GA_Size(output_page_offset));

            if (!output_page_const)
            {
                const NotVoidType *copy_data = page_data;
                if (!const_page_data)
                    copy_data += GA_Size(block_start_pageoff)*tuplesize;
                ok = ok && jsonWriteDataSpan(
                    w, copy_data, copy_size, tuplesize,
                    false, const_page_data, map, defvalue, (NotVoidType*)buffer);
            }

            output_page_offset += copy_size;
            block_start += copy_size;
            block_start_pageoff += copy_size;
        } while (ok && output_page_offset != thePageSize);

        if (output_page_const)
        {
            const NotVoidType *copy_data = page_data;
            if (!const_page_data)
            {
                // The -1 is because we added copy_size, which is at least 1, to block_start_pageoff,
                // and block_start_pageoff may now be at a page offset that is not the same value,
                // or may even be at the page offset of block_end.
                copy_data += GA_Size(block_start_pageoff-1)*tuplesize;
            }
            ok = ok && jsonWriteDataSpan(
                w, copy_data, 1, tuplesize,
                true, const_page_data, map, defvalue, (NotVoidType*)buffer);
        }
    }

    ok = ok && w.endUniformArray();
    return ok;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
class GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::ga_SubPageBlock
{
public:
    ga_SubPageBlock() {}
    ga_SubPageBlock(GA_PageNum page, GA_PageOff start, GA_PageOff end)
        : myPage(page), myStartOffset(start), myEndOffset(end) {}

    GA_PageNum myPage;
    GA_PageOff myStartOffset;
    GA_PageOff myEndOffset;
};

// --------------------------------------------------------------------------
// Compute a mapping to keep track of which internal pages affect which output
// pages.  We store this mapping as an ordered list of the input pages as they
// are traversed in building the output pages, with the start of each output
// page indicated by a negative value (-(input_page + 1)).
//
// NB: We don't keep track of the page offsets in the mapping so this is
//     really only useful for internal pages that are flagged as constant.
template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
void
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::buildOutputToInternalPageMap(
    const GA_Range &range,
    UT_Array<GA_PageNum> &map)
{
    GA_Iterator it(range);
    GA_Size output_page_offset = 0;
    GA_Size block_size = 0;
    GA_PageNum page_num;
    GA_PageOff page_offset;
    GA_PageNum last_page_num(-1);

    while (true)
    {
        if (output_page_offset == thePageSize)
        {
            output_page_offset = 0;
        }

        if (block_size == 0) // need new block
        {
            GA_Offset block_start, block_end;
            if (!it.blockAdvance(block_start, block_end))
                break;

            page_num = GAgetPageNum(block_start);
            page_offset = GAgetPageOff(block_start);
            block_size = block_end - block_start;
        }

        GA_Size output_size = SYSmin(block_size,
            thePageSize-output_page_offset);

        if (output_page_offset == 0)
        {
            map.append(-(page_num+1));
            last_page_num = page_num;
        }
        else if (page_num != last_page_num)
        {
            map.append(page_num);
            last_page_num = page_num;
        }

        page_offset += output_size;
        block_size -= output_size;
        output_page_offset += output_size;
    }
}

// Compute a mapping to keep track of which internal page data blocks affect
// which output pages.  We store this mapping as an ordered list of the sub
// page blocks as they are traversed in building the output pages, with the
// start of each output page indicated by a negative page number
// (-(input_page + 1)).
//
// TODO: We could keep track of block start/end, recomputing the internal
//       page number at need?
template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
void
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::buildOutputToInternalPageMap(
    const GA_Range &range,
    UT_Array<ga_SubPageBlock> &map)
{
    GA_Iterator it(range);
    GA_Size output_page_offset = 0;
    GA_Size block_size = 0;
    GA_PageNum page_num;
    GA_PageOff page_offset;

    while (true)
    {
        if (output_page_offset == thePageSize)
        {
            output_page_offset = 0;
        }

        if (block_size == 0) // need new block
        {
            GA_Offset block_start, block_end;
            if (!it.blockAdvance(block_start, block_end))
                break;

            page_num = GAgetPageNum(block_start);
            page_offset = GAgetPageOff(block_start);
            block_size = block_end - block_start;
        }

        GA_Size output_size = SYSmin(block_size,
            thePageSize-output_page_offset);

        if (output_page_offset == 0)
        {
            map.append(ga_SubPageBlock(
                GA_PageNum(-(page_num+1)), page_offset,
                page_offset + output_size));
        }
        else
        {
            map.append(ga_SubPageBlock(
                page_num, page_offset,
                page_offset + output_size));
        }

        page_offset += output_size;
        block_size -= output_size;
        output_page_offset += output_size;
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
GA_Size
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::marshallConstantFlagsForOutputPages(
    const UT_Array<GA_PageNum> &internal_page_map,
    UT_BitArray &constant_flags) const
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    GA_Size count = 0;
    GA_Size output_page = -1;
    bool output_page_flag = false;
    const NotVoidType *constant_value;
    const exint tuplesize = getTupleSize();

    constant_flags.setAllBits(false);
    for (GA_Size i = 0; i < internal_page_map.size(); i++)
    {
        GA_PageNum internal_page = internal_page_map(i);
        // A negative internal page is used to mark the start of a new
        // output page.
        if (internal_page < 0)
        {
            if (output_page >= 0 && output_page_flag)
            {
                constant_flags.setBit(output_page, output_page_flag);
                ++count;
            }

            ++output_page;
            UT_ASSERT_P(output_page <= constant_flags.size());
            internal_page = -(internal_page + 1);
            output_page_flag = isPageConstant(internal_page);
            if (output_page_flag)
            {
                constant_value = getPageData(internal_page);
            }
        }
        else if (output_page_flag)
        {
            if (!isPageConstant(internal_page))
                output_page_flag = false;
            else
            {
                const NotVoidType *new_constant_value = getPageData(internal_page);
                if ((new_constant_value==nullptr) != (constant_value==nullptr))
                    output_page_flag = false;
                else if (constant_value != new_constant_value)
                    output_page_flag = isEqual(constant_value, new_constant_value, tuplesize);
            }
        }
    }
    if (output_page >= 0 && output_page_flag)
    {
        constant_flags.setBit(output_page, output_page_flag);
        ++count;
    }
    return count;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
GA_Size
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::marshallConstantFlagsForOutputPages(
    const UT_Array<ga_SubPageBlock> &internal_page_map,
    UT_BitArray &constant_flags) const
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    GA_Size count = 0;
    GA_Size output_page = -1;
    bool output_page_flag = false;
    const NotVoidType *constant_value;
    const exint tuplesize = getTupleSize();

    constant_flags.setAllBits(false);
    for (GA_Size i = 0; i < internal_page_map.entries(); i++)
    {
        GA_PageNum internal_page = internal_page_map(i).myPage;
        // A negative internal page is used to mark the start of a new
        // output page.
        if (internal_page < 0)
        {
            if (output_page >= 0 && output_page_flag)
            {
                constant_flags.setBit(output_page, output_page_flag);
                ++count;
            }

            ++output_page;
            UT_ASSERT_P(output_page <= constant_flags.size());
            internal_page = -(internal_page + 1);
            output_page_flag = isPageConstant(internal_page);
            constant_value = getPageData(internal_page);
            if (!output_page_flag)
            {
                GA_PageOff start = internal_page_map(i).myStartOffset;
                GA_PageOff end = internal_page_map(i).myEndOffset;
                const NotVoidType *page = constant_value;
                constant_value += start;
                output_page_flag = isSubPageConstant(page, start+1, end,
                    tuplesize, constant_value);
            }
        }
        else if (output_page_flag)
        {
            const bool page_constant = isPageConstant(internal_page);
            const NotVoidType *page = getPageData(internal_page);
            if (page_constant)
            {
                if ((page==nullptr) != (constant_value==nullptr))
                    output_page_flag = false;
                else if (constant_value != page)
                    output_page_flag = isEqual(constant_value, page, tuplesize);
            }
            else
            {
                if (!isSubPageConstant(page,
                    internal_page_map(i).myStartOffset,
                    internal_page_map(i).myEndOffset,
                    tuplesize,
                    constant_value))
                    output_page_flag = false;
            }
        }
    }
    if (output_page >= 0 && output_page_flag)
    {
        constant_flags.setBit(output_page, output_page_flag);
        ++count;
    }
    return count;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::isSubPageConstant(
    const NotVoidType *page,
    GA_PageOff start, GA_PageOff end,
    const exint tuplesize,
    const NotVoidType *value)
{
    if (value == nullptr)
    {
        for (GA_PageOff cur = start; cur < end; cur++)
        {
            if (!isZero(page+cur*tuplesize, tuplesize))
                return false;
        }
    }
    else
    {
        for (GA_PageOff cur = start; cur < end; cur++)
        {
            if (!isEqual(page+cur*tuplesize, value, tuplesize))
                return false;
        }
    }
    return true;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
template<bool ARRAY_OF_ARRAYS>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonSaveAsArray(
    UT_JSONWriter &w, const GA_Range &range, UT_JID jid_storage,
    const UT_IntArray *map, int defvalue) const
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    int tuplesize = getTupleSize();

    if (!ARRAY_OF_ARRAYS)
    {
        if (!w.beginUniformArray(tuplesize*range.getEntries(), jid_storage))
            return false;
    }

    UT_StackBuffer<NotVoidType> buffer(ARRAY_OF_ARRAYS ? tuplesize : 0);

    GA_Offset start;
    GA_Offset end;
    for (GA_Iterator it(range); it.blockAdvance(start, end); )
    {
        if (map)
        {
            for (GA_Offset ai = start; ai < end; ++ai)
            {
                if (ARRAY_OF_ARRAYS)
                {
                    for (int component = 0; component < tuplesize; ++component)
                    {
                        NotVoidType v = this->template get<NotVoidType>(ai, component);
                        v = NotVoidType((v < 0 || v >= map->size()) ? defvalue : (*map)(v));
                        buffer[component] = v;
                    }
                    if (!w.jsonUniformArray(tuplesize, buffer))
                        return false;
                }
                else
                {
                    for (int component = 0; component < tuplesize; ++component)
                    {
                        NotVoidType v = this->template get<NotVoidType>(ai, component);
                        v = NotVoidType((v < 0 || v >= map->size()) ? defvalue : (*map)(v));
                        if (!w.uniformWrite(v))
                            return false;
                    }
                }
            }
        }
        else
        {
            // No map
            for (GA_Offset ai = start; ai < end; ++ai)
            {
                if (ARRAY_OF_ARRAYS)
                {
                    for (int component = 0; component < tuplesize; ++component)
                        buffer[component] = this->template get<NotVoidType>(ai, component);

                    if (!w.jsonUniformArray(tuplesize, buffer))
                        return false;
                }
                else
                {
                    for (int component = 0; component < tuplesize; ++component)
                    {
                        NotVoidType v = this->template get<NotVoidType>(ai, component);

                        if (!w.uniformWrite(v))
                            return false;
                    }
                }
            }
        }
    }

    if (ARRAY_OF_ARRAYS)
        return true;

    return w.endUniformArray();
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
UT_JID
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::GAStorageToJID(GA_Storage storage)
{
    switch (storage)
    {
    case GA_STORE_BOOL:
        return UT_JID_BOOL;
    case GA_STORE_INVALID:
        return UT_JID_NULL;
    case GA_STORE_STRING:
        return UT_JID_STRING;
    case GA_STORE_INT8:
        return UT_JID_INT8;
    case GA_STORE_UINT8:
        return UT_JID_UINT8;
    case GA_STORE_INT16:
        return UT_JID_INT16;
    case GA_STORE_INT32:
        return UT_JID_INT32;
    case GA_STORE_INT64:
        return UT_JID_INT64;
    case GA_STORE_REAL16:
        return UT_JID_REAL16;
    case GA_STORE_REAL32:
        return UT_JID_REAL32;
    case GA_STORE_REAL64:
        return UT_JID_REAL64;
    }
    UT_ASSERT_MSG_P(0, "Unhandled GA_Storage value!");
    return UT_JID_NULL;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonLoad(
    UT_JSONParser &p,
    const GA_LoadMap &map,
    GA_AttributeOwner owner)
{
    if (SYSisSame<DATA_T,void>())
    {
        bool success;
        SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,&p,&map,owner,&success](SYS_CALLIF_AUTO){
            // Hard case, where the storage type is not known at compile time.
            UT_Storage storage = this->Base::getStorage();
            switch (storage)
            {
                case UT_Storage::INT8:
                    success = this->castType<int8>().jsonLoad(p, map, owner); return;
                case UT_Storage::INT16:
                    success = this->castType<int16>().jsonLoad(p, map, owner); return;
                case UT_Storage::INT32:
                    success = this->castType<int32>().jsonLoad(p, map, owner); return;
                case UT_Storage::INT64:
                    success = this->castType<int64>().jsonLoad(p, map, owner); return;
                case UT_Storage::REAL16:
                    success = this->castType<fpreal16>().jsonLoad(p, map, owner); return;
                case UT_Storage::REAL32:
                    success = this->castType<fpreal32>().jsonLoad(p, map, owner); return;
                case UT_Storage::REAL64:
                    success = this->castType<fpreal64>().jsonLoad(p, map, owner); return;
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG(0, "Can't have a GA_PageArray with invalid storage!");
                    success = false;
                    return;
            }
            success = false;
        });
        return success;
    }

    int64 tuple_size = getTupleSize();

    // Cast to optimize for small tuple sizes
    if (TSIZE == -1 && tuple_size <= 3 && tuple_size >= 1)
    {
        bool success;
        SYS_CallIf<TSIZE == -1>::call([this,&p,&map,owner,tuple_size,&success](SYS_CALLIF_AUTO){
            if (tuple_size == 3)
                success = this->castTupleSize<3>().jsonLoad(p, map, owner);
            else if (tuple_size == 1)
                success = this->castTupleSize<1>().jsonLoad(p, map, owner);
            else
            {
                UT_ASSERT_P(tuple_size == 2);
                success = this->castTupleSize<2>().jsonLoad(p, map, owner);
            }
        });
        return success;
    }

    UT_WorkBuffer key;
    int64 page_size = -1;
    GA_Storage ga_storage = GA_STORE_INVALID;
    UT_StackBuffer<int> packing(tuple_size);
    int n_packing_entries = 0;
    UT_StackBuffer<UT_UniquePtr<UT_BitArray> > constant_page_flags(tuple_size);
    bool constant_page_flags_init = false;

    bool ok = true;
    bool done = false;
    for (UT_JSONParser::traverser mi = p.beginArray(); ok && !mi.atEnd(); ++mi)
    {
        if (!mi.getLowerKey(key))
        {
            ok = false;
            break;
        }
        switch (GA_PageArrayIO::getJSONTokenID(key.buffer()))
        {
            case GA_PageArrayIO::GA_JDTUPLE_SIZE:
            {
                int64 local_tuple_size = -1;
                ok = p.parseInteger(local_tuple_size);
                if (ok && local_tuple_size != tuple_size)
                {
                    p.addWarning("Inconsistent tuple size specification");
                    ok = false;
                }
                break;
            }
            case GA_PageArrayIO::GA_JDTUPLE_STORAGE:
                ok = p.parseString(key);
                if (ok)
                    ga_storage = GAstorage(key.buffer());
                break;
            case GA_PageArrayIO::GA_JDTUPLE_ARRAYS:
                // Tuple size and storage type are supposed to have already been set.
                if (done || tuple_size != getTupleSize() || ga_storage != getStorage())
                {
                    p.addWarning("Bad data type/size specification");
                    ok = p.skipNextObject();
                }
                else
                {
                    // Load as a struct of arrays
                    UT_JSONParser::traverser ai = p.beginArray();
                    for (exint component = 0; ok && !ai.atEnd(); ++component, ++ai)
                    {
                        if (component < tuple_size)
                        {
                            GA_Offset startoff = map.getLoadOffset(owner);
                            LoadComponentArrayFunctor op(*this, startoff, component);
                            if (GAisIntStorage(ga_storage))
                                ok = p.loadPODArray<LoadComponentArrayFunctor, int64>(op);
                            else
                                ok = p.loadPODArray<LoadComponentArrayFunctor, fpreal64>(op);
                        }
                        else
                        {
                            if (component == tuple_size)
                                p.addWarning("Too many tuple items in data array");
                            ok = p.skipNextObject();
                        }
                    }
                    done = true;
                }
                break;
            case GA_PageArrayIO::GA_JDTUPLE_TUPLES:
                if (done || tuple_size != getTupleSize() || ga_storage != getStorage())
                {
                    p.addWarning("Bad data type/size specification");
                    ok = p.skipNextObject();
                }
                else
                {
                    // Load as an array of structs
                    UT_JSONParser::traverser ai = p.beginArray();
                    GA_Offset offset = map.getLoadOffset(owner);
                    GA_Size ppage = GAgetPageNum(offset);
                    for ( ; ok && !ai.atEnd(); ++offset, ++ai)
                    {
                        GA_PageNum newpagenum = GAgetPageNum(offset);
                        if (newpagenum != ppage)
                        {
                            // We compress previous page
                            tryCompressPage(ppage);
                            ppage = newpagenum;
                        }

                        UT_StackBuffer<NotVoidType> buffer(tuple_size);
                        exint nread = p.parseUniformArray<NotVoidType>(buffer, tuple_size);
                        if (nread < tuple_size)
                        {
                            ok = false;
                            break;
                        }
                        if (nread > tuple_size)
                            p.addWarning("Extra data found in array tuple");

                        if (TSIZE >= 1)
                        {
                            setVector(offset, *(const UT_FixedVector<NotVoidType,theSafeTupleSize>*)buffer.array());
                        }
                        else
                        {
                            for (int component = 0; component < tuple_size; ++component)
                                set(offset, component, buffer[component]);
                        }
                    }
                    tryCompressPage(ppage);
                    done = true;
                }
                break;
            case GA_PageArrayIO::GA_JDTUPLE_PAGESIZE:
                ok = p.parseInteger(page_size);
                break;
            case GA_PageArrayIO::GA_JDTUPLE_PACKING:
                if (tuple_size != getTupleSize())
                {
                    p.addWarning("Packing requires valid size specification");
                    ok = p.skipNextObject();
                }
                else
                {
                    // NB: p.parseUniformArray() might return a greater value
                    //     than expected, but it won't write the extra values
                    //     to packing.array().
                    n_packing_entries = p.parseUniformArray(packing.array(), tuple_size);

                    if (constant_page_flags_init && n_packing_entries != (tuple_size > 0 ? 1 : 0))
                    {
                        p.addWarning("Non-trivial packing specification must come before constant page flags");
                        n_packing_entries = 0;
                        ok = false;
                    }
                    else if (n_packing_entries >= 0)
                    {
                        int total_packed_size = 0;
                        for (int i = 0; i < n_packing_entries; ++i)
                        {
                            total_packed_size += packing[i];
                        }
                        if (total_packed_size != tuple_size ||
                            n_packing_entries > tuple_size)
                        {
                            p.addWarning("Invalid packing specification");
                            n_packing_entries = -1;
                            ok = false;
                        }
                    }
                }
                break;
            case GA_PageArrayIO::GA_JDTUPLE_CONSTPAGEFLAGS:
                if (tuple_size != getTupleSize() || 
                    page_size <= 0 || n_packing_entries < 0)
                {
                    p.addWarning("Bad data type/size specification");
                    ok = p.skipNextObject();
                }
                else
                {
                    int i = 0;
                    UT_BitArray scratch_array;

                    int n_arrays = n_packing_entries ? n_packing_entries
                                 : (tuple_size > 0 ? 1 : 0);
                    int64 n_input_pages = (map.getLoadCount(owner)+page_size-1) / page_size;

                    for (UT_JSONParser::traverser it = p.beginArray(); !it.atEnd(); ++it, ++i)
                    {
                        if (i < n_arrays)
                        {
                            int64 n_loaded = p.parseUniformBoolArray(scratch_array, n_input_pages);

                            // We allow an empty array when no pages are constant.
                            if (n_loaded == 0)
                            {
                                constant_page_flags[i].reset(nullptr);
                            }
                            else
                            {
                                constant_page_flags[i].reset(new UT_BitArray());
                                scratch_array.swap(*constant_page_flags[i]);
                            }
                        }
                        else
                        {
                            p.skipNextObject();
                            UT_ASSERT(0);
                        }
                    }
                    ok = (i == n_arrays);
                    constant_page_flags_init = true;
                }
                break;
            case GA_PageArrayIO::GA_JDTUPLE_RAWPAGEDATA:
                // Load as an array of structs with tuples whose pages may be compressed
                if (done || tuple_size != getTupleSize() || ga_storage != getStorage() ||
                    page_size <= 0 || n_packing_entries < 0)
                {
                    p.addWarning("Bad data type/size specification");
                    ok = p.skipNextObject();
                }
                else
                {
                    // We default to a full vector when a GA_JDTUPLE_PACKING
                    // field is missing.
                    if (n_packing_entries == 0 && tuple_size > 0)
                    {
                        packing[0] = tuple_size;
                        n_packing_entries = 1;
                    }
                    done = true;
                    ok = jsonLoadRawPageData(p, map, owner,
                        GA_Size(page_size),
                        packing.array(), n_packing_entries,
                        constant_page_flags.array());
                }
                break;
            default:
                p.addWarning("Data Array Tuple unknown key '%s'", key.buffer());
                break;
        }
    }
    if (!done)
        p.addWarning("Missing data for data array");
    return ok;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
class GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::LoadComponentArrayFunctor
{
public:
    typedef GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED> PageArray;

    LoadComponentArrayFunctor(PageArray &dest,GA_Offset startoff,exint component)
        : myDest(dest)
        , myStartOffset(startoff)
        , myComponent(component)
        , myDestSize(dest.size()-startoff)
    {}

    template <typename T> SYS_FORCE_INLINE bool
    set(int64 i, T val) const
    {
        if (GA_Offset(i) >= myDestSize)
            return false;
        myDest.set(myStartOffset+(GA_Size)i, myComponent, val);
        return true;
    }

    template <typename T> SYS_FORCE_INLINE bool
    setArray(const T *data, int64 size) const
    {
        bool outofbounds = false;
        if (GA_Offset(size) > myDestSize)
        {
            size = int64(myDestSize);
            outofbounds = true;
        }

        // Fast path for single component
        if (TSIZE == 1)
        {
            myDest.setRange(myStartOffset, GA_Offset(size), data);
            return !outofbounds;
        }

        GA_Offset end = myStartOffset + GA_Size(size);

        for (GA_Offset off = myStartOffset; off < end; ++off, ++data)
        {
            myDest.set(off, myComponent, *data);
        }

        return !outofbounds;
    }

    PageArray &myDest;
    const GA_Offset myStartOffset;
    const exint myComponent;
    const GA_Offset myDestSize;
};

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED>
bool
GA_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED>::jsonLoadRawPageData(
    UT_JSONParser &p,
    const GA_LoadMap &map,
    GA_AttributeOwner owner,
    GA_Size page_size,
    const int *packing,
    int n_packing_entries,
    const UT_UniquePtr<UT_BitArray> *const constant_page_flags)
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    UT_JSONParser::traverser it = p.beginArray();
    if (it.getErrorState())
        return false;

    UT_JID jid = p.getUniformArrayType();
    bool istypematch = (jid == GAStorageToJID(getStorage()));

    GA_Size num_input_elements = map.getLoadCount(owner);
    GA_Offset load_offset = map.getLoadOffset(owner);
    const GA_PageNum start_page_num = GAgetPageNum(load_offset);
    GA_PageOff page_offset = GAgetPageOff(load_offset);
    const exint tuple_size = getTupleSize();
    const exint num_page_values = tuple_size*thePageSize;

    UT_StackBuffer<NotVoidType> single_tuple(tuple_size);

    if (n_packing_entries == 1 && page_size == thePageSize)
    {
        UT_ASSERT(packing[0] == tuple_size);

        const UT_BitArray *constpagebits = constant_page_flags[0].get();

        if (page_offset == GA_PageOff(0))
        {
            // Loading at the beginning of a page, making things much simpler
            GA_Size num_full_new_pages = (num_input_elements >> GA_PAGE_BITS);
            GA_PageOff end_page_offset = GAgetPageOff(GA_Offset(num_input_elements));

            // First, fill in all complete, full-size pages
            GA_PageNum pagenum = start_page_num;
            for (GA_Size input_pagei = 0; input_pagei < num_full_new_pages; ++input_pagei, ++pagenum)
            {
                if (constpagebits && constpagebits->getBitFast(input_pagei))
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(single_tuple.array(), tuple_size))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, single_tuple.array(), tuple_size) != tuple_size)
                            return false;
                    }
                    setPageConstant(pagenum, single_tuple.array());
                }
                else
                {
                    NotVoidType *data = hardenPageNoInit(pagenum);
                    if (istypematch)
                    {
                        if (!it.readUniformArray(data, num_page_values))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, data, num_page_values) != num_page_values)
                            return false;
                    }
                }
            }

            // Handle any final incomplete or not-full-size page
            if (end_page_offset != GA_PageOff(0))
            {
                if (constpagebits && constpagebits->getBitFast(num_full_new_pages))
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(single_tuple.array(), tuple_size))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, single_tuple.array(), tuple_size) != tuple_size)
                            return false;
                    }
                    if (load_offset+num_input_elements == size())
                        setPageConstant(pagenum, single_tuple.array());
                    else
                    {
                        // I don't know if this path will ever be taken; I'm guessing not.

                        bool equal = false;
                        if (isPageConstant(pagenum))
                        {
                            const NotVoidType *current_tuple = getPageData(pagenum);
                            if (current_tuple)
                            {
                                if (isEqual(single_tuple.array(),current_tuple,tuple_size))
                                    equal = true;
                            }
                            else
                            {
                                if (isZero(single_tuple.array(),tuple_size))
                                    equal = true;
                            }
                        }
                        if (!equal)
                        {
                            NotVoidType *data = hardenPage(pagenum);
                            for (GA_PageOff pageoff(0); pageoff < end_page_offset; ++pageoff)
                            {
                                for (exint component = 0; component < tuple_size; ++component, ++data)
                                {
                                    *data = single_tuple[component];
                                }
                            }
                        }
                    }
                }
                else
                {
                    // This could be optimized to avoid a bit of redundant initialization,
                    // but hopefully it's not too much of an issue.
                    NotVoidType *data = hardenPage(pagenum);
                    const exint num_left_values = tuple_size*end_page_offset;
                    if (istypematch)
                    {
                        if (!it.readUniformArray(data, num_left_values))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, data, num_left_values) != num_left_values)
                            return false;
                    }
                }
            }
        }
        else
        {
            // Loading with matching packing (only 1 tuple) and matching page size,
            // but not loading at a page boundary.

            // TODO: Optimize this case for that we know that pages are the same size,
            // e.g. to try to preserve constant pages or load directly into destination.

            UT_StackBuffer<NotVoidType> buffer(thePageSize*tuple_size);
            const exint num_input_pages = (num_input_elements + thePageSize-1) / thePageSize;
            for (GA_Size input_pagei = 0; input_pagei < num_input_pages; ++input_pagei)
            {
                exint inputi = thePageSize*input_pagei;
		// NB: Base::thePageSize is needed (as opposed to thePageSize) to avoid
		//     MSVC 19.14.26428.1 from crashing with /permissive-
                const exint num_page_elements = SYSmin(Base::thePageSize, num_input_elements-inputi);
                const exint num_page_values = tuple_size*num_page_elements;

                const bool constant_page = constpagebits && constpagebits->getBitFast(input_pagei);
                if (constant_page)
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(buffer.array(), tuple_size))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, buffer.array(), tuple_size) != tuple_size)
                            return false;
                    }

                    for (exint element = 0; element < num_page_elements; ++element, ++inputi)
                    {
                        for (exint component = 0; component < tuple_size; ++component)
                        {
                            set(load_offset+inputi, component, buffer[component]);
                        }
                    }
                }
                else
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(buffer.array(), num_page_values))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, buffer.array(), num_page_values) != num_page_values)
                            return false;
                    }

                    exint i = 0;
                    for (exint element = 0; element < num_page_elements; ++element, ++inputi)
                    {
                        for (exint component = 0; component < tuple_size; ++component, ++i)
                        {
                            set(load_offset+inputi, component, buffer[i]);
                        }
                    }
                }
            }
        }
    }
    else
    {
        UT_StackBuffer<NotVoidType> buffer(page_size*tuple_size);
        const exint num_input_pages = (num_input_elements + page_size-1) / page_size;
        for (GA_Size input_pagei = 0; input_pagei < num_input_pages; ++input_pagei)
        {
            exint start_component = 0;
            for (exint packingi = 0; packingi < n_packing_entries; ++packingi)
            {
                exint inputi = page_size*input_pagei;
                const exint num_page_elements = SYSmin(page_size, num_input_elements-inputi);
                const exint input_tuple_size = packing[packingi];
                const exint num_page_values = input_tuple_size*num_page_elements;

                const UT_BitArray *constpagebits = constant_page_flags[packingi].get();

                const bool constant_page = constpagebits && constpagebits->getBitFast(input_pagei);
                if (constant_page)
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(buffer.array(), input_tuple_size))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, buffer.array(), input_tuple_size) != input_tuple_size)
                            return false;
                    }

                    for (exint element = 0; element < num_page_elements; ++element, ++inputi)
                    {
                        for (exint component = 0; component < input_tuple_size; ++component)
                        {
                            set(load_offset+inputi, start_component+component, buffer[component]);
                        }
                    }
                }
                else
                {
                    if (istypematch)
                    {
                        if (!it.readUniformArray(buffer.array(), num_page_values))
                            return false;
                    }
                    else
                    {
                        if (p.parseArrayValues(it, buffer.array(), num_page_values) != num_page_values)
                            return false;
                    }

                    exint i = 0;
                    for (exint element = 0; element < num_page_elements; ++element, ++inputi)
                    {
                        for (exint component = 0; component < input_tuple_size; ++component, ++i)
                        {
                            set(load_offset+inputi, start_component+component, buffer[i]);
                        }
                    }
                }

                start_component += input_tuple_size;
            }
        }
    }

    return  it.atEnd();
}

#endif
