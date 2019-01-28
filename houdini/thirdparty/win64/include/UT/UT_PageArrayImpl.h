/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_PageArrayImpl.h (UT Library, C++)
 *
 * COMMENTS:    Implementations of functions of UT_PageArray that
 *              aren't needed in most places that use it.
 */

#pragma once

#ifndef __UT_PageArrayImpl__
#define __UT_PageArrayImpl__

#include "UT_PageArray.h"

#include "UT_Defaults.h"
#include "UT_MemoryCounter.h"
#include "UT_StackBuffer.h"
#include "UT_Storage.h"
#include "UT_Swap.h"

#include <SYS/SYS_Types.h>


template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setSize(IDX_T newsize, NotVoidType initval)
{
    UT_ASSERT_P(newsize >= IDX_T(0));
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

    setCapacityIfNeeded(newsize);
    hardenTable();
    PageTable *pages = myImpl.getPages();
    UT_ASSERT_P(pages || newsize == IDX_T(0));
    if (pages)
    {
        IDX_T oldsize = pages->size();

        if (!PAGESHARDENED || TSIZE >= 0)
            pages->setSize(newsize);
        else
            pages->setSize(newsize, myImpl.getTupleSize());

        if (newsize > oldsize)
        {
            if (TSIZE >= 1)
                pages->fill(oldsize, newsize, initval);
            else if (TSIZE == -1 && myImpl.getTupleSize() > 0)
                pages->fill(oldsize, newsize, initval, myImpl.getTupleSize());
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setSize(IDX_T newsize, const UT_FixedVector<NotVoidType,theSafeTupleSize> &initval)
{
    UT_ASSERT_P(newsize >= IDX_T(0));
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(getStorage() != UT_Storage::INVALID);
    UT_ASSERT_P(TSIZE >= 1);

    setCapacityIfNeeded(newsize);
    hardenTable();
    PageTable *pages = myImpl.getPages();
    UT_ASSERT_P(pages || newsize == IDX_T(0));
    if (pages)
    {
        IDX_T oldsize = pages->size();

        // No need to destruct if smaller, since it's a POD type.

        pages->setSize(newsize);

        if (newsize > oldsize)
            pages->fill(oldsize, newsize, initval);
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, THEPAGEBITS, IDX_T>::setSize(IDX_T newsize, const UT_Defaults &initval)
{
    UT_ASSERT_P(newsize >= IDX_T(0));

    setCapacityIfNeeded(newsize);
    hardenTable();
    PageTable *pages = myImpl.getPages();
    UT_ASSERT_P(pages || newsize == IDX_T(0));
    if (pages)
    {
        IDX_T oldsize = pages->size();

        // No need to destruct if smaller, since it's a POD type.

        if (!PAGESHARDENED || TSIZE >= 0)
            pages->setSize(newsize);
        else
            pages->setSize(newsize, myImpl.getTupleSize());

        if (newsize > oldsize)
            setConstant(oldsize, newsize, initval);
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setConstant(IDX_T start, IDX_T end, NotVoidType v)
{
    UT_ASSERT_P(end >= start);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= capacity());
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

    if (end <= start)
        return;

    hardenTable();
    PageTable *pages = myImpl.getPages();
    if (!pages)
        return;

    if (TSIZE >= 1)
        pages->fill(start, end, v);
    else if (TSIZE == -1 && myImpl.getTupleSize() > 0)
        pages->fill(start, end, v, myImpl.getTupleSize());
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setConstant(IDX_T start, IDX_T end, const UT_FixedVector<NotVoidType,theSafeTupleSize> &v)
{
    UT_ASSERT_P(end >= start);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= capacity());
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(TSIZE >= 1);

    if (end <= start)
        return;

    hardenTable();
    PageTable *pages = myImpl.getPages();
    if (!pages)
        return;
    pages->fill(start, end, v);
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setConstant(IDX_T start, IDX_T end, const UT_Defaults &v)
{
    UT_ASSERT_P(end >= start);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= capacity());

    auto &hard = hardenTable();

    UT_Storage storage = getStorage();

    // If the storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<DATA_T,void>())
    {
        switch (storage)
        {
            case UT_Storage::INT8:
                hard.template castType<int8>().setConstant(start, end, v); return;
            case UT_Storage::INT16:
                hard.template castType<int16>().setConstant(start, end, v); return;
            case UT_Storage::INT32:
                hard.template castType<int32>().setConstant(start, end, v); return;
            case UT_Storage::INT64:
                hard.template castType<int64>().setConstant(start, end, v); return;
            case UT_Storage::REAL16:
                hard.template castType<fpreal16>().setConstant(start, end, v); return;
            case UT_Storage::REAL32:
                hard.template castType<fpreal32>().setConstant(start, end, v); return;
            case UT_Storage::REAL64:
                hard.template castType<fpreal64>().setConstant(start, end, v); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
                break;
        }
        return;
    }

    if (end <= start)
        return;

    PageTable *pages = myImpl.getPages();
    if (!pages)
        return;

    const exint tuplesize = getTupleSize();
    if (tuplesize == 0)
        return;

    // UT_Defaults is almost always tuple size 1, so have a special case for it.
    if (v.getTupleSize() == 1 || tuplesize == 1)
    {
        if (TSIZE >= 1)
        {
            if (UTisIntStorage(storage))
                pages->fill(start, end, NotVoidType(v.getI(0)));
            else
                pages->fill(start, end, NotVoidType(v.getF(0)));
        }
        else
        {
            if (UTisIntStorage(storage))
                pages->fill(start, end, NotVoidType(v.getI(0)), tuplesize);
            else
                pages->fill(start, end, NotVoidType(v.getF(0)), tuplesize);
        }
    }
    else
    {
        UT_StackBuffer<NotVoidType,16*sizeof(fpreal64)> buf(tuplesize);
        if (UTisIntStorage(storage))
        {
            for (exint i = 0; i < tuplesize; ++i)
                buf[i] = NotVoidType(v.getI(i));
        }
        else
        {
            for (exint i = 0; i < tuplesize; ++i)
                buf[i] = NotVoidType(v.getF(i));
        }
        pages->fill(start, end, buf, tuplesize);
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setStorage(const UT_Storage newstorage)
{
    const UT_Storage oldstorage = getStorage();
    UT_ASSERT_P(newstorage != UT_Storage::INVALID);
    UT_ASSERT_MSG_P((SYS_IsSame<DATA_T,void>::value) || (newstorage == oldstorage), "Can't change the storage of an array whose type is fixed.");

    // Nothing to do if same type, or bad type
    if (newstorage == oldstorage || newstorage == UT_Storage::INVALID)
        return;

    PageTable *const oldpages = myImpl.getPages();

    // If there's no data, we only need to set the storage.
    exint tuplesize = getTupleSize();
    if (tuplesize == 0 || !oldpages)
    {
        myImpl.setStorage(newstorage);
        return;
    }

    UT_ASSERT_P(numPages(oldpages->capacity()) >= 1);

    // Copy the data into a new array with the new storage type
    ThisType newarray(getTupleSize(), newstorage);
    newarray.setCapacity(capacity());
    IDX_T n = size();
    newarray.setSize(n);
    newarray.moveRange(*this,IDX_T(0),IDX_T(0),IDX_T(n));

    // decRef depends on knowing the type
    switch (oldstorage)
    {
        case UT_Storage::INT8:
            castType<int8>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::INT16:
            castType<int16>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::INT32:
            castType<int32>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::INT64:
            castType<int64>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::REAL16:
            castType<fpreal16>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::REAL32:
            castType<fpreal32>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::REAL64:
            castType<fpreal64>().myImpl.getPages()->decRef(tuplesize); break;
        case UT_Storage::INVALID:
            // NOTE: Can't have a UT_PageArray with DATA_T void and invalid storage.
            myImpl.getPages()->decRef(tuplesize); break;
    }

    // Take ownership of the page table.
    PageTable *newpages = newarray.myImpl.getPages();
    UT_ASSERT_P(newpages);
    newpages->incRef();

    myImpl.setStorage(newstorage);
    myImpl.getPages() = newpages;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::setTupleSize(exint newtuplesize, const UT_Defaults &v)
{
    exint oldtuplesize = getTupleSize();
    UT_ASSERT_P(newtuplesize >= 0);
    UT_ASSERT_MSG_P((TSIZE == -1) || (newtuplesize == oldtuplesize), "Can't change the tuple size of an array whose tuple size is fixed.");

    // Nothing to do if same size, or bad size
    if (newtuplesize == oldtuplesize || newtuplesize < 0)
        return;

    PageTable *const oldpages = myImpl.getPages();

    // If there's no data, we only need to set the tuple size.
    if (!oldpages)
    {
        myImpl.setTupleSize(newtuplesize);
        return;
    }

    UT_ASSERT_P(numPages(oldpages->capacity()) >= 1);

    // Copy the data into a new array with the new storage type
    ThisType newarray(newtuplesize, getStorage());
    newarray.setCapacity(capacity());
    IDX_T n = size();
    newarray.setSize(n, v);
    newarray.moveRange(*this,IDX_T(0),IDX_T(0),IDX_T(n));

    // decRef depends on knowing the type
    switch (getStorage())
    {
        case UT_Storage::INT8:
            castType<int8>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::INT16:
            castType<int16>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::INT32:
            castType<int32>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::INT64:
            castType<int64>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::REAL16:
            castType<fpreal16>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::REAL32:
            castType<fpreal32>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::REAL64:
            castType<fpreal64>().myImpl.getPages()->decRef(oldtuplesize); break;
        case UT_Storage::INVALID:
            // NOTE: Can't have a UT_PageArray with DATA_T void and invalid storage.
            myImpl.getPages()->decRef(oldtuplesize); break;
    }

    // Take ownership of the page table.
    PageTable *newpages = newarray.myImpl.getPages();
    UT_ASSERT_P(newpages);
    newpages->incRef();

    myImpl.setTupleSize(newtuplesize);
    myImpl.getPages() = newpages;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
int64
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : 0;

    const PageTable *pages = myImpl.getPages();
    if (!pages)
        return mem;

    UT_PageNum npages = numPages(pages->capacity());
    mem += exint(npages) * sizeof(PageTableEntry);

    exint tuplebytes = ((getStorage() != UT_Storage::INVALID) ? UTstorageSize(getStorage()) : sizeof(NotVoidType))*getTupleSize();

    // Case for a single, possibly small page
    if (npages == UT_PageNum(1) && !pages->getFirstPage()->isConstant())
    {
        mem += sizeof(SYS_AtomicCounter) + tuplebytes*exint(pages->capacity());
        return mem;
    }

    for (UT_PageNum i(0); i < npages; ++i)
    {
        const PageTableEntry *const page = pages->getPPage(i);
        mem += page->getMemoryUsage(tuplebytes);
    }

    return mem;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::countMemory(UT_MemoryCounter &counter, bool inclusive) const
{
    if (counter.mustCountUnshared() && inclusive)
    {
        UT_MEMORY_DEBUG_LOG("UT_PageArray",int64(sizeof(*this)));
        counter.countUnshared(sizeof(*this));
    }

    const PageTable *pages = myImpl.getPages();
    if (!pages)
        return;

    UT_PageNum npages = numPages(pages->capacity());
    int64 tablemem = exint(npages) * sizeof(PageTableEntry);
    if (!pages->isShared())
    {
        if (counter.mustCountUnshared())
        {
            UT_MEMORY_DEBUG_LOG("UT_PageArray::PageTable",int64(tablemem));
            counter.countUnshared(tablemem);
        }
    }
    else
    {
        UT_ASSERT_MSG_P(pages->getRefCount() > 1, "Why is something unref'ing data while we're counting memory?");
        if (counter.mustCountShared())
        {
            UT_MEMORY_DEBUG_LOG_SHARED("UT_PageArray::PageTable",int64(tablemem),pages,pages->getRefCount());
            bool already_counted = counter.countShared(tablemem, pages->getRefCount(), pages);

            // If this counter has already counted a reference to this page
            // table, it's also already counted its pages, below, and since
            // this is the *same* set of references it's already counted,
            // not different references to the same pages we'd get incorrect
            // reference count tracking if we counted the pages again, so we
            // just return.
            if (already_counted)
                return;
        }
    }

    exint tuplebytes = ((getStorage() != UT_Storage::INVALID) ? UTstorageSize(getStorage()) : sizeof(NotVoidType))*getTupleSize();

    // Case for a single, possibly small page
    if (npages == UT_PageNum(1) && !pages->getFirstPage()->isConstant())
    {
        const PageTableEntry *const page = pages->getFirstPage();
        int64 pagemem = sizeof(SYS_AtomicCounter) + tuplebytes*exint(pages->capacity());
        if (!page->isShared())
        {
            if (counter.mustCountUnshared())
            {
                UT_MEMORY_DEBUG_LOG("UT_PageArray::Page0",int64(pagemem));
                counter.countUnshared(pagemem);
            }
        }
        else
        {
            UT_ASSERT_MSG_P(page->getRefCount() > 1, "Why is something unref'ing data while we're counting memory?");
            if (counter.mustCountShared())
            {
                const void *masked = page->isConstant() ? page->getMaskedPtrVoid() : page->getFirstPtrVoid();
                UT_MEMORY_DEBUG_LOG_SHARED("UT_PageArray::Page0",int64(pagemem),masked,page->getRefCount());
                counter.countShared(pagemem, page->getRefCount(), masked);
            }
        }
        return;
    }

    for (UT_PageNum i(0); i < npages; ++i)
    {
        const PageTableEntry *const page = pages->getPPage(i);
        int64 pagemem = page->getMemoryUsage(tuplebytes);
        if (!pagemem)
            continue;

        if (!page->isShared())
        {
            if (counter.mustCountUnshared())
            {
                UT_MEMORY_DEBUG_LOG("UT_PageArray::Page",int64(pagemem));
                counter.countUnshared(pagemem);
            }
        }
        else
        {
            UT_ASSERT_P(page->getRefCount() > 1);
            if (counter.mustCountShared())
            {
                const void *masked = page->isConstant() ? page->getMaskedPtrVoid() : page->getFirstPtrVoid();
                UT_MEMORY_DEBUG_LOG_SHARED("UT_PageArray::Page",int64(pagemem),masked,page->getRefCount());
                counter.countShared(pagemem, page->getRefCount(), masked);
            }
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::moveRange(IDX_T srcstart, IDX_T deststart, IDX_T nelements)
{
    // Just defer to the general function.
    // It handles overlapping ranges and constant pages correctly.
    moveRange(*this, srcstart, deststart, nelements);
}

// This is a very big function, but don't let it scare you.
// Much of the code is only applicable to particular template types.
// If it weren't for constant pages, this would be *much* simpler.
template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::moveRange(
    const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> &src,
    IDX_T srcstart, IDX_T deststart, IDX_T nelements)
{
    typedef const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> SrcType;
    typedef typename SrcType::PageTable SrcPageTable;
    typedef typename SrcType::PageTableEntry SrcPageTableEntry;
    typedef typename SrcType::NotVoidType SrcNotVoidType;

    UT_ASSERT_P(nelements >= IDX_T(0));
    UT_ASSERT_P(srcstart >= IDX_T(0) && deststart >= IDX_T(0));
    UT_ASSERT_P(srcstart+nelements <= src.size() && deststart+nelements <= size());

    UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()) || (getStorage() != UT_Storage::INVALID && src.getStorage() != UT_Storage::INVALID));

    // If there's even a chance we might write values, we should harden the
    // table and record it in the templates so that we don't harden again.
    auto &hard = hardenTable();

    // If the destination storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<DATA_T,void>())
    {
        UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INT8:
                hard.template castType<int8>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::INT16:
                hard.template castType<int16>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::INT32:
                hard.template castType<int32>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::INT64:
                hard.template castType<int64>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::REAL16:
                hard.template castType<fpreal16>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::REAL32:
                hard.template castType<fpreal32>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::REAL64:
                hard.template castType<fpreal64>().moveRange(src, srcstart, deststart, nelements); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
                break;
        }
        return;
    }

    // If the source storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<SRC_DATA_T,void>())
    {
        // Avoid switch on storage type if src is dest.
        if ((const void*)&src==(void*)this)
        {
            hard.moveRange(src.template castType<DATA_T>(), srcstart, deststart, nelements);
            return;
        }

        UT_Storage src_storage = src.getStorage();
        switch (src_storage)
        {
            case UT_Storage::INT8:
                hard.moveRange(src.template castType<int8>(), srcstart, deststart, nelements); return;
            case UT_Storage::INT16:
                hard.moveRange(src.template castType<int16>(), srcstart, deststart, nelements); return;
            case UT_Storage::INT32:
                hard.moveRange(src.template castType<int32>(), srcstart, deststart, nelements); return;
            case UT_Storage::INT64:
                hard.moveRange(src.template castType<int64>(), srcstart, deststart, nelements); return;
            case UT_Storage::REAL16:
                hard.moveRange(src.template castType<fpreal16>(), srcstart, deststart, nelements); return;
            case UT_Storage::REAL32:
                hard.moveRange(src.template castType<fpreal32>(), srcstart, deststart, nelements); return;
            case UT_Storage::REAL64:
                hard.moveRange(src.template castType<fpreal64>(), srcstart, deststart, nelements); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
        }
        return;
    }

    // We now have both the source type and the destination type known at compile time.
    UT_ASSERT_P((!SYSisSame<DATA_T,void>()) && (!SYSisSame<SRC_DATA_T,void>()));

    // Check if zero elements or moving data to location it's already in.
    if (nelements <= IDX_T(0) || (SYSisSame<DATA_T,SRC_DATA_T>() && (const void*)&src==(void*)this && srcstart == deststart))
        return;

    UT_PageOff srcoff = pageOff(srcstart);
    UT_PageOff destoff = pageOff(deststart);

    // Just copy the minimum of the tuple sizes.
    // Hopefully the compiler optimizes approriately if the values are
    // known at compile time.
    const exint srctuplesize = src.getTupleSize();
    const exint desttuplesize = getTupleSize();

    // Nothing to do if either tuple size is zero.
    if (srctuplesize == 0 || desttuplesize == 0)
        return;

    const SrcPageTable *srcpagetable = src.myImpl.getPages();
    PageTable *destpagetable = myImpl.getPages();

    // Since nelements is > 0, srcpagetable and destpagetable should be non-NULL.
    UT_ASSERT_P(srcpagetable && destpagetable);

    UT_PageNum srcpagenum = pageNum(srcstart);
    UT_PageNum destpagenum = pageNum(deststart);

    // NOTE: Shouldn't need to check for smaller first page here
    //       (until below), since that page boundary isn't allowed
    //       to be crossed by the ranges.
    if (srcoff+UT_PageOff(exint(nelements)) <= UT_PageOff(thePageSize) && destoff+UT_PageOff(exint(nelements)) <= UT_PageOff(thePageSize))
    {
        // *************************************************************
        // * CASE 1: Source and dest each confined to 1 page           *
        // *************************************************************

        // NOTE: We can dereference here because we don't pass any address into srcpage
        //       outside of this scope.
        const SrcPageTableEntry *const srcpage = srcpagetable->getPPage(srcpagenum);
        PageTableEntry *destpage = destpagetable->getPPage(destpagenum);

        // This is the only case that can have both srcpage and destpage be small pages.
        bool issmalldestpage = destpagetable->capacity() < IDX_T(thePageSize);
        UT_PageOff destpagecapacity(thePageSize);
        if (issmalldestpage)
            destpagecapacity = destpagetable->capacity();

        // If dest is a full page and src is also a full page or constant, just use replacePage.
        bool isfullpage = (nelements == IDX_T(thePageSize));
        if (!isfullpage && destoff == UT_PageOff(0) && deststart+nelements == size())
        {
            // If srcpage and destpage aren't the same capacity, destpage can't reference srcpage,
            // even if size() is much less than the capacity of either.
            bool issmallsrcpage = srcpagetable->capacity() < IDX_T(thePageSize);
            bool samecapacity = (!issmalldestpage && !issmallsrcpage) ||
                (issmalldestpage && issmallsrcpage && destpagetable->capacity() == srcpagetable->capacity());

            // destpage is a full destination page, but may not be replaceable by srcpage.
            // srcpage lines up if srcoff == 0, and always implicitly lines up if constant.
            // If either src or dest is small page and can't reference due to different capacity,
            // fall through to copyPartialPage, which won't reference.

            isfullpage = ((srcoff == UT_PageOff(0) && samecapacity) || srcpage->isConstant());
        }
        if (isfullpage)
        {
            replacePage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, nelements, destpagecapacity);
            return;
        }

        // If it's a partial page, just copy that part
        // NOTE: This handles overlapping ranges correctly.
        copyPartialPage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, destoff, srcoff, nelements, destpagecapacity);
        return;
    }

    // If overlapping and moving data to later addresses,
    // we need to do a backward loop, which is a big pain.
    bool isoverlappingmovelater = (
        SYSisSame<DATA_T,SRC_DATA_T>() &&
        (void*)this == (const void *)&src &&
        (deststart > srcstart && deststart < srcstart+nelements)
    );

    if (srcoff == destoff)
    {
        // *************************************************************
        // * CASE 2: Source and dest pages line up and at least one    *
        // *         page boundary is crossed.                         *
        // *************************************************************
        // Example for following along:
        // THEPAGEBITS is 3, so 8-tuple pages.
        // src and to are dest 5.
        // src  [# # #|# # # # # # # #|# # # # # # # #|# # # # # #]
        // dest [# # #|# # # # # # # #|# # # # # # # #|# # # # # #]

        // In this case, src and dest don't have any small pages,
        // because both have page boundary crossings.

        const SrcPageTableEntry *psrcpagep = srcpagetable->getPPage(srcpagenum);
        PageTableEntry *pdestpagep = destpagetable->getPPage(destpagenum);

        // If overlapping and moving data to later addresses,
        // we need to do a backward loop, which is a big pain.
        // It's not a very common case, so it doesn't have to be as optimized.
        if (isoverlappingmovelater)
        {
            UT_ASSERT_P(desttuplesize == srctuplesize);

            UT_PageOff ntuplesfirstpage(0);
            if (destoff != UT_PageOff(0))
            {
                ntuplesfirstpage = UT_PageOff(thePageSize)-destoff;
                nelements -= IDX_T(exint(ntuplesfirstpage));
            }

            // (nelements is now 3 less)
            // NOTE: Not numPages, since that'd include any partial page at end
            UT_PageNum nfullpages = pageNum(nelements);
            ++psrcpagep;
            ++pdestpagep;
            PageTableEntry *pdestend = pdestpagep + nfullpages;
            const SrcPageTableEntry *psrcend = psrcpagep + nfullpages;

            // Since backward, first, copy any incomplete last page
            // src  [     |               |               |# # # # # #]
            // dest [     |               |               |# # # # # #]
            UT_PageOff nleftover = pageOff(nelements);
            if (nleftover != UT_PageOff(0))
            {
                const SrcPageTableEntry *srcpage = psrcend;
                PageTableEntry *destpage = pdestend;

                // Remember that it may be effectively complete, if
                // the last page within the size of the array and nleftover is
                // the number of elements less than size() in that page.
                // If it's really a full page, just use replacePage.
                bool isfullpage = deststart+nelements == size();
                if (isfullpage)
                    replacePage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, nleftover, thePageSize);
                else
                    copyPartialPage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, UT_PageOff(0), UT_PageOff(0), nleftover, thePageSize);
            }

            // Still backward, copy/reference whole pages next:
            // src  [     |# # # # # # # #|# # # # # # # #|           ]
            // dest [     |# # # # # # # #|# # # # # # # #|           ]
            while (pdestpagep != pdestend)
            {
                --psrcend;
                --pdestend;
                const SrcPageTableEntry *srcpage = psrcend;
                PageTableEntry *destpage = pdestend;

                replacePage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, thePageSize, thePageSize);
            }

            // Still backward, lastly, copy any incomplete first page:
            // src  [# # #|               |               |           ]
            // dest [# # #|               |               |           ]
            if (destoff != UT_PageOff(0))
            {
                --psrcpagep;
                --pdestpagep;
                const SrcPageTableEntry *srcpage = psrcpagep;
                PageTableEntry *destpage = pdestpagep;

                copyPartialPage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, destoff, srcoff, ntuplesfirstpage, thePageSize);
            }

            return;
        }

        // In the common case of case 2, src and dest aren't overlapping,
        // or src is later than dest, so we can go forward.

        // First, copy any incomplete first page:
        // src  [# # #|               |               |           ]
        // dest [# # #|               |               |           ]
        if (destoff != UT_PageOff(0))
        {
            const SrcPageTableEntry *srcpage = psrcpagep;
            PageTableEntry *destpage = pdestpagep;

            UT_PageOff ntuplesfirstpage = UT_PageOff(thePageSize)-destoff;
            copyPartialPage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, destoff, srcoff, ntuplesfirstpage, thePageSize);

            nelements -= IDX_T(exint(ntuplesfirstpage));
            ++psrcpagep;
            ++pdestpagep;
        }

        // Copy/reference whole pages next:
        // src  [     |# # # # # # # #|# # # # # # # #|           ]
        // dest [     |# # # # # # # #|# # # # # # # #|           ]
        // (nelements is now 3 less)
        // NOTE: Not numPages, since that'd include any partial page at end
        UT_PageNum nfullpages = pageNum(nelements);
        PageTableEntry *pdestend = pdestpagep + nfullpages;
        for (; pdestpagep != pdestend; ++psrcpagep, ++pdestpagep)
        {
            const SrcPageTableEntry *srcpage = psrcpagep;
            PageTableEntry *destpage = pdestpagep;

            replacePage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, thePageSize, thePageSize);
        }

        // Lastly, copy any incomplete last page
        // src  [     |               |               |# # # # # #]
        // dest [     |               |               |# # # # # #]
        UT_PageOff nleftover = pageOff(nelements);
        if (nleftover != UT_PageOff(0))
        {
            const SrcPageTableEntry *srcpage = psrcpagep;
            PageTableEntry *destpage = pdestpagep;

            // Remember that it may be effectively complete, if
            // the last page within the size of the array and nleftover is
            // the number of elements less than size() in that page.
            // If it's really a full page, just use replacePage.
            bool isfullpage = deststart+nelements == size();
            if (isfullpage)
                replacePage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, nleftover, thePageSize);
            else
                copyPartialPage<SrcType>(destpage, srcpage, desttuplesize, srctuplesize, UT_PageOff(0), UT_PageOff(0), nleftover, thePageSize);
        }
        return;
    }
    else
    {
        // *************************************************************
        // * CASE 3: Source and dest pages don't line up and at least  *
        // *         one page boundary is crossed.                     *
        // *************************************************************
        // Example for following along:
        // THEPAGEBITS is 3, so 8-tuple pages.
        // src is 5; dest is 3.
        // src  [# # #|# # # # # # # #|# # # # # # # #|# # # # # #]
        // dest [# # # # #|# # # # # # # #|# # # # # # # #|# # # #]
        //                |<----6---->|<2>|
        //               spagestartind dpagestartins
        UT_PageOff spagestartind = pageOff(deststart-srcstart);
        UT_PageOff dpagestartins = pageOff(srcstart-deststart);

        // Because of the misalignment, we don't have to worry about
        // referencing pages, though we do have to worry about constant
        // pages.  If both src pages covering a full dest page are constant
        // and the same value, we can use makeConstantFrom using either
        // of the source pages.

        // REMEMBER: This case could have a small first page in either
        //           src or dest, but not both.
        // REMEMBER: Must handle overlapping ranges!

        const SrcPageTableEntry *psrcpagep = srcpagetable->getPPage(srcpagenum);
        PageTableEntry *pdestpagep = destpagetable->getPPage(destpagenum);

        // Case 3.0:
        // Overlapping range

        const SrcPageTableEntry *srcpage0 = psrcpagep;

        const exint mintuplesize = SYSmin(srctuplesize,desttuplesize);

        // Case 3.1:
        // src  [# # #|# #]
        // dest [# # # # #]
        // dest is in a single page; it could be a small page.
        // src is across two pages; they can't be small-capacity pages.
        if (destoff+UT_PageOff(exint(nelements)) <= UT_PageOff(thePageSize))
        {
            PageTableEntry *destpage = pdestpagep;

            bool issmalldestpage = destpagetable->capacity() < IDX_T(thePageSize);
            UT_PageOff destpagecapacity(thePageSize);
            if (issmalldestpage)
                destpagecapacity = destpagetable->capacity();

            const SrcPageTableEntry *srcpage1 = psrcpagep + 1;

            if (!PAGESHARDENED && srcpage0->isConstant() && srcpage1->isConstant())
            {
                const SrcNotVoidType *stuple0 = SrcType::getConstantPtr(srcpage0, 0, srctuplesize);
                const SrcNotVoidType *stuple1 = SrcType::getConstantPtr(srcpage1, 0, srctuplesize);
                if (SrcType::isEqualConst(stuple0, stuple1, srctuplesize))
                {
                    // If dest page is already constant and equal to both src pages, nothing to do.
                    if (destpage->isConstant() && isEqualConst(getConstantPtr(destpage, 0, desttuplesize), stuple0, mintuplesize))
                        return;

                    // If both src pages are constant and equal, and dest is a full
                    // page, make dest constant.
                    bool isfullpage = (nelements == IDX_T(thePageSize)) || (destoff == UT_PageOff(0) && deststart+nelements == size());
                    if (isfullpage)
                    {
                        makeConstantFrom<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize);
                        return;
                    }
                }
            }

            if (!PAGESHARDENED && destpage->isConstant())
                hardenConstantPage(destpage, destpagecapacity, desttuplesize);
            else if (!PAGESHARDENED && destpage->isShared())
                hardenSharedPage(destpage, destpagecapacity, desttuplesize);

            UT_PageOff n0 = UT_PageOff(thePageSize)-srcoff;
            if (isoverlappingmovelater)
            {
                copyPartialPage<SrcType>(destpage, srcpage1, desttuplesize, srctuplesize, destoff+n0, UT_PageOff(0), nelements-n0, destpagecapacity);
                copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, destoff,    srcoff,        n0,           destpagecapacity);
            }
            else
            {
                copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, destoff,    srcoff,        n0,           destpagecapacity);
                copyPartialPage<SrcType>(destpage, srcpage1, desttuplesize, srctuplesize, destoff+n0, UT_PageOff(0), nelements-n0, destpagecapacity);
            }

            return;
        }

        // There is at least one dest page boundary, so dest has full-capacity pages.

        if (isoverlappingmovelater)
        {
            // FIXME: Implement this!!!
            UT_ASSERT_MSG(0, "Implement this!!!  It should be like the code below, only copying chunks in reverse order.");

            return;
        }

        // Deal with tuples before the first full destination page.
        if (destoff > UT_PageOff(0))
        {
            PageTableEntry *destpage = pdestpagep;

            if (destoff < spagestartind)
            {
                //              srcpage0 srcpage1
                // src            [# # #|# # ...
                // dest           [# # # # #|...
                //                |<--->|<->|
                // spagestartind-destoff  dpagestartins

                UT_PageOff n0 = spagestartind - destoff;
                copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, destoff, srcoff, n0, UT_PageOff(thePageSize));

                srcoff = UT_PageOff(0);
                destoff += n0;
                ++psrcpagep;
                srcpage0 = psrcpagep;
                nelements -= IDX_T(exint(n0));
            }

            //              srcpage0
            // src            [# # # #...
            // dest           [# #|# #...
            //                |<->|
            //       thePageSize-destoff
            UT_PageOff n0 = UT_PageOff(thePageSize) - destoff;
            copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, destoff, srcoff, n0, UT_PageOff(thePageSize));
            ++pdestpagep;
            nelements -= IDX_T(exint(n0));
        }

        // Middle full destination pages
        for (; nelements >= IDX_T(thePageSize); nelements -= IDX_T(thePageSize), ++pdestpagep, ++psrcpagep, (srcpage0 = psrcpagep))
        {
            PageTableEntry *destpage = pdestpagep;

            //              srcpage0 srcpage1
            // src  [    # # # # # #|# #    ]
            // dest [   |# # # # # # # #|   ]
            //          |<--------->|<->|
            //         spagestartind dpagestartins

            const SrcPageTableEntry *srcpage1 = psrcpagep + 1;

            if (!PAGESHARDENED && srcpage0->isConstant() && srcpage1->isConstant())
            {
                const SrcNotVoidType *stuple0 = SrcType::getConstantPtr(srcpage0, 0, srctuplesize);
                const SrcNotVoidType *stuple1 = SrcType::getConstantPtr(srcpage1, 0, srctuplesize);
                if (SrcType::isEqualConst(stuple0, stuple1, srctuplesize))
                {
                    // If dest page is already constant and equal to both src pages, nothing to do.
                    if (destpage->isConstant() && isEqualConst(getConstantPtr(destpage, 0, desttuplesize), stuple0, mintuplesize))
                        continue;

                    // If both src pages are constant and equal, and dest is a full
                    // page, make dest constant.
                    makeConstantFrom<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize);
                    continue;
                }
            }

            if (!PAGESHARDENED && destpage->isConstant())
                hardenConstantPage(destpage, UT_PageOff(thePageSize), desttuplesize);
            else if (!PAGESHARDENED && destpage->isShared())
                hardenSharedPage(destpage, UT_PageOff(thePageSize), desttuplesize);

            copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, UT_PageOff(0), dpagestartins, spagestartind, UT_PageOff(thePageSize));
            copyPartialPage<SrcType>(destpage, srcpage1, desttuplesize, srctuplesize, spagestartind, UT_PageOff(0), dpagestartins, UT_PageOff(thePageSize));
        }

        // Final partial page, though may reach size()
        if (nelements > IDX_T(0))
        {
            PageTableEntry *destpage = pdestpagep;

            const bool isfullmaybeconstpage = !PAGESHARDENED && (deststart+nelements == size());

            if (nelements > IDX_T(exint(spagestartind)))
            {
                //              srcpage0 srcpage1
                // src  [    # # # # # #|#]
                // dest [   |# # # # # # #]
                //          |<--------->|-|
                //         spagestartind nelements-spagestartind

                const SrcPageTableEntry *srcpage1 = psrcpagep + 1;

                if (isfullmaybeconstpage && srcpage0->isConstant() && srcpage1->isConstant())
                {
                    const SrcNotVoidType *stuple0 = SrcType::getConstantPtr(srcpage0, 0, srctuplesize);
                    const SrcNotVoidType *stuple1 = SrcType::getConstantPtr(srcpage1, 0, srctuplesize);
                    if (SrcType::isEqualConst(stuple0, stuple1, srctuplesize))
                    {
                        // If dest page is already constant and equal to both src pages, nothing to do.
                        if (destpage->isConstant() && isEqualConst(getConstantPtr(destpage, 0, desttuplesize), stuple0, mintuplesize))
                            return;

                        // If both src pages are constant and equal, and dest is a full
                        // page, make dest constant.
                        makeConstantFrom<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize);
                        return;
                    }
                }

                if (!PAGESHARDENED && destpage->isConstant())
                    hardenConstantPage(destpage, UT_PageOff(thePageSize), desttuplesize);
                else if (!PAGESHARDENED && destpage->isShared())
                    hardenSharedPage(destpage, UT_PageOff(thePageSize), desttuplesize);

                copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, UT_PageOff(0), dpagestartins, spagestartind, UT_PageOff(thePageSize));
                copyPartialPage<SrcType>(destpage, srcpage1, desttuplesize, srctuplesize, spagestartind, UT_PageOff(0), UT_PageOff(exint(nelements))-spagestartind, UT_PageOff(thePageSize));
            }
            else
            {
                //              srcpage0
                // src  [    # # # # #]
                // dest [   |# # # # #]
                //          |<------->|
                //           nelements

                if (isfullmaybeconstpage && srcpage0->isConstant())
                {
                    const SrcNotVoidType *stuple0 = SrcType::getConstantPtr(srcpage0, 0, srctuplesize);
                    // If dest page is already constant and equal to both src pages, nothing to do.
                    if (destpage->isConstant() && isEqualConst(getConstantPtr(destpage, 0, desttuplesize), stuple0, mintuplesize))
                        return;

                    // If both src pages are constant and equal, and dest is a full
                    // page, make dest constant.
                    makeConstantFrom<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize);
                    return;
                }

                if (!PAGESHARDENED && destpage->isConstant())
                    hardenConstantPage(destpage, UT_PageOff(thePageSize), desttuplesize);
                else if (!PAGESHARDENED && destpage->isShared())
                    hardenSharedPage(destpage, UT_PageOff(thePageSize), desttuplesize);

                copyPartialPage<SrcType>(destpage, srcpage0, desttuplesize, srctuplesize, UT_PageOff(0), dpagestartins, UT_PageOff(exint(nelements)), UT_PageOff(thePageSize));
            }
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::swapRange(IDX_T astart, IDX_T bstart, IDX_T nelements)
{
    UT_ASSERT_P(nelements >= IDX_T(0));
    UT_ASSERT_P(astart >= IDX_T(0) && bstart >= IDX_T(0));
    UT_ASSERT_P(astart+nelements <= size() && bstart+nelements <= size());
    UT_ASSERT_MSG_P(astart >= bstart+nelements || bstart >= astart+nelements, "Ranges can't overlap when swapping!");
    if (nelements <= IDX_T(0))
        return;
    auto &hard = hardenTable();
    if (!SYSisSame<DATA_T,void>())
    {
        // Easy case, where the storage type is known at compile time.
        exint tuplesize = getTupleSize();
        for (IDX_T i(0); i < nelements; ++i)
        {
            for (exint component = 0; component < tuplesize; ++component)
            {
                UTswap(hard(astart+i, component), hard(bstart+i, component));
            }
        }
        return;
    }

    // Hard case, where the storage type is not known at compile time.
    UT_Storage storage = getStorage();
    switch (storage)
    {
        case UT_Storage::INT8:
            hard.template castType<int8>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::INT16:
            hard.template castType<int16>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::INT32:
            hard.template castType<int32>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::INT64:
            hard.template castType<int64>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::REAL16:
            hard.template castType<fpreal16>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::REAL32:
            hard.template castType<fpreal32>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::REAL64:
            hard.template castType<fpreal64>().swapRange(astart, bstart, nelements); return;
        case UT_Storage::INVALID:
            UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
            break;
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcType>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::replacePage(
    PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize, UT_PageOff destpagesize, UT_PageOff destpagecapacity)
{
    typedef typename SrcType::DataType SRC_DATA_T;
    typedef typename SrcType::NotVoidType SrcNotVoidType;
    UT_IF_ASSERT_P(const exint SRC_TSIZE = SrcType::theTupleSize;)
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
    UT_ASSERT_P(!(SYSisSame<SRC_DATA_T,void>()));
    UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()) || (UT_StorageNum<DATA_T>::theStorage != UT_Storage::INVALID && UT_StorageNum<SRC_DATA_T>::theStorage != UT_Storage::INVALID));
    UT_ASSERT_P(thePageBits == SrcType::thePageBits);
    UT_ASSERT_P((SYSisSame<IndexType, typename SrcType::IndexType>()));
    UT_ASSERT_P(desttuplesize > 0 && srctuplesize > 0);
    UT_ASSERT_P((TSIZE == -1 || TSIZE == desttuplesize) && (SRC_TSIZE == -1 || SRC_TSIZE == srctuplesize));

    // If the source page is constant,
    if (src->isConstant())
    {
        if (!PAGESHARDENED && (dest->isConstant() || desttuplesize <= srctuplesize))
            makeConstantFrom<SrcType>(dest, src, desttuplesize, srctuplesize);
        else
        {
            // This codepath is primarily for the awkward case where we can't
            // easily make the destination page constant, because
            // it's not currently constant and the tuple size is larger.
            // However, it's also used for filling a page that isn't allowed to be
            // constant-compressed with the tuple from a constant-compressed source.
            UT_ASSERT_P(PAGESHARDENED || (!dest->isConstant() && desttuplesize > srctuplesize));

            if (!PAGESHARDENED && dest->isShared())
                hardenSharedPage(dest, destpagecapacity, desttuplesize);

            // Fill range in dest with value from src.
            NotVoidType *destpagedata = dest->getFirstPtr();
            // NOTE: This is destpagesize instead of capacity, because it's just used for filling in data.
            NotVoidType *destpageend = destpagedata + (desttuplesize*destpagesize);

            const SrcNotVoidType *stuple = SrcType::getConstantPtr(src, 0, srctuplesize);

            const exint mintuplesize = SYSmin(srctuplesize,desttuplesize);
            const exint desttupleextra = desttuplesize-mintuplesize;

            fillNonConstWithConst(destpagedata, destpageend, stuple, mintuplesize, desttupleextra);
        }
    }
    else if (!PAGESHARDENED && SYSisSame<DATA_T,SRC_DATA_T>() && desttuplesize == srctuplesize)
    {
        // Nothing to do if already referencing the same data.
        // This pointer comparison works because we know that
        // the types and tuple sizes are the same,
        // and the src is non-constant, (so if dest is constant,
        // it won't be equal).
        if (src->getFirstPtrVoid() == dest->getFirstPtrVoidUnsafe())
            return;

        exint bytesize = desttuplesize*sizeof(NotVoidType);
        if (dest->isRefd(bytesize))
            dest->decRef();

        // Reference the source page
        SYSconst_cast(src)->incRef();

        // Still need to cast to PageTableEntry*, because the compiler needs to
        // compile this line when the condition is false.
        *dest = *(const PageTableEntry *)src;
    }
    else
    {
        if (!PAGESHARDENED && dest->isConstant())
            hardenConstantPage(dest, destpagecapacity, desttuplesize);
        else if (!PAGESHARDENED && dest->isShared())
            hardenSharedPage(dest, destpagecapacity, desttuplesize);

        // Copy data from src to dest
        NotVoidType *destpagedata = dest->getFirstPtr();
        const SrcNotVoidType *srcpagedata = src->getFirstPtr();
        // NOTE: This must be destpagesize instead of capacity, else it might access the source out of bounds.
        copyNonConst(destpagedata, srcpagedata, desttuplesize, srctuplesize, destpagesize);
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcType>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::copyPartialPage(
    PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize,
    UT_PageOff destoff, UT_PageOff srcoff, UT_PageOff ntuples, UT_PageOff destpagecapacity)
{
    typedef typename SrcType::DataType SRC_DATA_T;
    typedef typename SrcType::NotVoidType SrcNotVoidType;
    UT_IF_ASSERT_P(const exint SRC_TSIZE = SrcType::theTupleSize;)
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
    UT_ASSERT_P(!(SYSisSame<SRC_DATA_T,void>()));
    UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()) || (UT_StorageNum<DATA_T>::theStorage != UT_Storage::INVALID && UT_StorageNum<SRC_DATA_T>::theStorage != UT_Storage::INVALID));
    UT_ASSERT_P(thePageBits == SrcType::thePageBits);
    UT_ASSERT_P((SYSisSame<IndexType, typename SrcType::IndexType>()));
    UT_ASSERT_P(desttuplesize > 0 && srctuplesize > 0);
    UT_ASSERT_P((TSIZE == -1 || TSIZE == desttuplesize) && (SRC_TSIZE == -1 || SRC_TSIZE == srctuplesize));
    UT_ASSERT_P(ntuples > UT_PageOff(0));

    // NOTE: Don't have to check for full page.  The caller is responsible
    //       for that if they want referencing or constant overwriting.

    // If the source page is constant,
    if (src->isConstant())
    {
        const SrcNotVoidType *stuple = SrcType::getConstantPtr(src, 0, srctuplesize);

        const exint mintuplesize = SYSmin(srctuplesize,desttuplesize);

        // If the destination page is constant,
        if (dest->isConstant())
        {
            const NotVoidType *tuple = getConstantPtr(dest, 0, desttuplesize);

            // If the const pages are equal, there's nothing to do.
            if (isEqualConst(tuple, stuple, mintuplesize))
                return;

            hardenConstantPage(dest, destpagecapacity, desttuplesize);
        }

        if (!PAGESHARDENED && dest->isShared())
            hardenSharedPage(dest, destpagecapacity, desttuplesize);

        // Fill range in dest with value from src.
        NotVoidType *destpagedata = dest->getFirstPtr() + (desttuplesize*destoff);
        NotVoidType *destpageend = destpagedata + (desttuplesize*ntuples);

        const exint desttupleextra = desttuplesize-mintuplesize;

        fillNonConstWithConst(destpagedata, destpageend, stuple, mintuplesize, desttupleextra);

        return;
    }

    if (!PAGESHARDENED && dest->isConstant())
        hardenConstantPage(dest, destpagecapacity, desttuplesize);
    else if (!PAGESHARDENED && dest->isShared())
        hardenSharedPage(dest, destpagecapacity, desttuplesize);

    // Remember that the ranges could overlap if same page
    // NOTE: Since dest was hardened if shared, dest can only equal src if same table.
    if (SYSisSame<DATA_T,SRC_DATA_T>() && dest->getFirstPtrVoidUnsafe() == src->getFirstPtrVoid() && (srcoff < destoff+UT_PageOff(exint(ntuples)) && destoff < srcoff+UT_PageOff(exint(ntuples))))
    {
        // Overlapping, so be careful!

        UT_ASSERT_P(desttuplesize == srctuplesize);

        // Nothing to do if exactly same range.
        // This could happen even if caller checked the global offsets,
        // and even if they're separate arrays, because the same page
        // can appear in multiple locations.
        if (srcoff == destoff)
            return;

        NotVoidType *destpagedata = dest->getFirstPtr();
        destpagedata += desttuplesize*destoff;
        NotVoidType *destend = destpagedata + desttuplesize*ntuples;

        const SrcNotVoidType *srcpagedata = src->getFirstPtr();
        srcpagedata += srctuplesize*srcoff;

        // If moving to earlier addresses, can copy in forward loop
        if (destoff < srcoff)
        {
            do
            {
                *destpagedata = *srcpagedata;
                ++srcpagedata;
                ++destpagedata;
            } while (destpagedata != destend);
        }
        // If moving to later addresses, must copy in backward loop
        else
        {
            const SrcNotVoidType *srcend = srcpagedata + srctuplesize*ntuples;
            do
            {
                --srcend;
                --destend;
                *destend = *srcend;
            } while (destpagedata != destend);
        }
    }
    else
    {
        // The two ranges don't overlap, so just copy
        NotVoidType *destpagedata = dest->getFirstPtr();
        destpagedata += desttuplesize*destoff;

        const SrcNotVoidType *srcpagedata = src->getFirstPtr();
        srcpagedata += srctuplesize*srcoff;

        copyNonConst(destpagedata, srcpagedata, desttuplesize, srctuplesize, UT_PageOff(ntuples));
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcType>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::makeConstantFrom(
    PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize)
{
    typedef typename SrcType::DataType SRC_DATA_T;
    //typedef UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> SrcType;
    typedef typename SrcType::PageTableEntry SrcPageTableEntry;
    typedef typename SrcType::NotVoidType SrcNotVoidType;

    const exint SRC_TSIZE = SrcType::theTupleSize;
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
    UT_ASSERT_P(!(SYSisSame<SRC_DATA_T,void>()));
    UT_ASSERT_P(src->isConstant());
    UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()) || (UT_StorageNum<DATA_T>::theStorage != UT_Storage::INVALID && UT_StorageNum<SRC_DATA_T>::theStorage != UT_Storage::INVALID));
    UT_ASSERT_P(!PAGESHARDENED);
    UT_ASSERT_P(thePageBits == SrcType::thePageBits);
    UT_ASSERT_P((SYSisSame<IndexType, typename SrcType::IndexType>()));
    UT_ASSERT_P(desttuplesize > 0 && srctuplesize > 0);
    UT_ASSERT_P((TSIZE == -1 || TSIZE == desttuplesize) && (SRC_TSIZE == -1 || SRC_TSIZE == srctuplesize));
    UT_ASSERT_MSG_P(desttuplesize <= srctuplesize || dest->isConstant(), "The higher component values may not have been constant!");

    if (PAGESHARDENED)
        return;

    // Do nothing in very common case of copying zero to zero.
    // NOTE: It may seem like this could be generalized to
    //       if (*src == *dest)
    //       but, they could be different types or tuple sizes,
    //       so, for example, one could be an inline constant page
    //       that just happens to equal a pointer for a non-inline
    //       constant page, or two inline tuples may be different
    //       but produce equal pointers, e.g. (-2,-2) in int16
    //       would match (-65538) in int32.
    const bool issrczero = src->isConstantAndZeroSafe();
    if (dest->isConstantAndZeroSafe() && issrczero)
        return;

    bool wasconst = dest->isConstant();
    if (!wasconst)
        dest->decRef();

    // Common case of exact match can just reference the same constant page
    if (((TSIZE == SRC_TSIZE && TSIZE != -1) || (desttuplesize == srctuplesize)) && SYSisSame<DATA_T,SRC_DATA_T>())
    {
        // Now that we know that the types and tuple sizes are the same,
        // we can just check the pointers to see if they're the same (constant) page.
        // The typecast on src is just so that it will compile when the types don't match.
        if (*dest == *(const PageTableEntry*)src)
            return;

        const bool typefitsinline = PageTableEntry::typeFitsInline(desttuplesize);
        if (!typefitsinline && wasconst && !dest->isConstantZero())
        {
            dest->decRef();
        }

        // Still need to cast to PageTableEntry*, because the compiler needs to
        // compile this line when the condition is false.
        *dest = *(const PageTableEntry*)src;

        if (!typefitsinline && !dest->isConstantZero())
        {
            dest->incRef();
        }

        return;
    }

    // Either the type doesn't match or the tuple size doesn't match.

    const SrcNotVoidType *const srctuple = SrcType::getConstantPtr(src, 0, srctuplesize);

    const exint mintuplesize = SYSmin(desttuplesize, srctuplesize);

    // Easy for inline case, checked at compile-time.
    if (PageTableEntry::typeFitsInline(desttuplesize))
    {
        // If dest was already constant, we don't need to write
        // theConstantPageBit, and we don't want to blow away any
        // components that are between srctuplesize and desttuplesize, in the
        // unlikely event that desttuplesize > srctuplesize.
        if (!wasconst)
        {
            // This sets the constant bit and makes sure that the
            // space between that bit and tuple component 0 is zeroed.
            dest->initZero();

            // Since initZero sets everything to zero, if src
            // is all zero, we can early exit.
            if (issrczero)
                return;
        }
        NotVoidType *tuple = dest->getInlinePtr(desttuplesize);
        if (issrczero)
        {
            memset(tuple, 0, mintuplesize*sizeof(NotVoidType));
        }
        else
        {
            for (exint i = 0; i < mintuplesize; ++i)
                tuple[i] = UTconvertStorage<NotVoidType>(srctuple[i]);
        }
        return;
    }

    // In other cases, we may or may not have to unref constant page
    if (wasconst)
    {
        if (dest->isConstantZero())
        {
            // Fairly common case: already zero, making zero.
            if (issrczero)
                return;
        }
        else
        {
            if (desttuplesize <= srctuplesize && issrczero)
            {
                // No longer need this old constant page
                dest->decRef();
            }
            else if (dest->isShared())
            {
                NotVoidType *tuple = dest->getMaskedPtr();
                bool equal = true;
                if (desttuplesize > srctuplesize && issrczero)
                {
                    equal = isZero(tuple, mintuplesize);
                }
                else
                {
                    for (exint i = 0; i < mintuplesize; ++i)
                    {
                        if (tuple[i] != UTconvertStorage<NotVoidType>(srctuple[i]))
                        {
                            equal = false;
                            break;
                        }
                    }
                }

                if (equal)
                {
                    // Already equal; nothing to do
                    return;
                }

                // Need to allocate new constant page before ditching the old one
                // if desttuplesize is larger, because some elements need to be kept.
                if (desttuplesize > srctuplesize)
                {
                    // Need to save the pointers so that we can decRef below
                    // after calling alloc.
                    PageTableEntry orig(*dest);

                    dest->alloc(UT_PageOff(1), desttuplesize);
                    NotVoidType *newtuple = dest->getFirstPtr();
                    // Copy lower components from src
                    if (issrczero)
                    {
                        memset(newtuple, 0, srctuplesize*sizeof(NotVoidType));
                    }
                    else
                    {
                        for (exint i = 0; i < srctuplesize; ++i)
                            newtuple[i] = UTconvertStorage<NotVoidType>(srctuple[i]);
                    }
                    // Copy higher components from dest
                    for (exint i = srctuplesize; i < desttuplesize; ++i)
                        newtuple[i] = tuple[i];

                    orig.decRef();
                    dest->setConstantBit();
                    return;
                }

                // No longer need this old constant page
                dest->decRef();
            }
            else
            {
                // Reuse the unshared constant page
                NotVoidType *tuple = dest->getMaskedPtr();
                if (issrczero)
                {
                    memset(tuple, 0, mintuplesize*sizeof(NotVoidType));
                }
                else
                {
                    for (exint i = 0; i < mintuplesize; ++i)
                        tuple[i] = UTconvertStorage<NotVoidType>(srctuple[i]);
                }
                return;
            }
        }
    }

    if (desttuplesize <= srctuplesize && issrczero)
    {
        dest->initZero();
        return;
    }

    // Need to allocate new constant page
    dest->alloc(UT_PageOff(1), desttuplesize);
    NotVoidType *tuple = dest->getFirstPtr();
    if (issrczero)
    {
        memset(tuple, 0, desttuplesize*sizeof(NotVoidType));
    }
    else
    {
        for (exint i = 0; i < mintuplesize; ++i)
            tuple[i] = UTconvertStorage<NotVoidType>(srctuple[i]);

        if (desttuplesize > srctuplesize)
        {
            // dest was already zero when here, or !wasconst, so zero out the extra components not copied from src.
            memset(tuple+srctuplesize, 0, (desttuplesize-srctuplesize)*sizeof(NotVoidType));
        }
    }
    dest->setConstantBit();
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcNotVoidType>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::fillNonConstWithConst(
    NotVoidType *__restrict destpagedata,
    NotVoidType *destpageend,
    const SrcNotVoidType *__restrict stuple,
    exint mintuplesize,
    exint desttupleextra)
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    // Fill range in dest with value from stuple.
    if (stuple)
    {
        if (!SYSisSame<NotVoidType,SrcNotVoidType>())
        {
            do
            {
                for (exint i = 0; i < mintuplesize; ++i, ++destpagedata)
                    *destpagedata = UTconvertStorage<NotVoidType>(stuple[i]);
                destpagedata += desttupleextra;
            } while(destpagedata < destpageend);
        }
        else
        {
            do
            {
                for (exint i = 0; i < mintuplesize; ++i, ++destpagedata)
                    *destpagedata = stuple[i];
                destpagedata += desttupleextra;
            } while(destpagedata < destpageend);
        }
    }
    else
    {
        if (!SYSisSame<NotVoidType,SrcNotVoidType>())
        {
            do
            {
                for (exint i = 0; i < mintuplesize; ++i, ++destpagedata)
                    *destpagedata = NotVoidType();
                destpagedata += desttupleextra;
            } while(destpagedata < destpageend);
        }
        else
        {
            do
            {
                if (SYSisPOD<NotVoidType>())
                {
                    for (exint i = 0; i < mintuplesize; ++i, ++destpagedata)
                        ::memset(destpagedata, 0, sizeof(NotVoidType));
                }
                else
                {
                    NotVoidType v = NotVoidType();
                    for (exint i = 0; i < mintuplesize; ++i, ++destpagedata)
                        *destpagedata = v;
                }
                destpagedata += desttupleextra;
            } while(destpagedata < destpageend);
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcNotVoidType>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::copyNonConst(
    NotVoidType *__restrict destpagedata,
    const SrcNotVoidType *__restrict srcpagedata,
    const exint desttuplesize,
    const exint srctuplesize,
    UT_PageOff ntuples)
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    NotVoidType *destpageend = destpagedata + ntuples*desttuplesize;

    if (desttuplesize == srctuplesize)
    {
        // Copy values from srcpagedata to destpagedata.
        if (SYSisSame<NotVoidType,SrcNotVoidType>())
        {
            do
            {
                *destpagedata = *srcpagedata;
                ++srcpagedata;
                ++destpagedata;
            } while(destpagedata < destpageend);
        }
        else
        {
            do
            {
                *destpagedata = UTconvertStorage<NotVoidType>(*srcpagedata);
                ++srcpagedata;
                ++destpagedata;
            } while(destpagedata < destpageend);
        }
    }
    else
    {
        const exint mintuplesize = SYSmin(desttuplesize, srctuplesize);
        const exint srctupleextra = srctuplesize - mintuplesize;
        const exint desttupleextra = desttuplesize - mintuplesize;

        // Copy values from srcpagedata to destpagedata.
        if (SYSisSame<NotVoidType,SrcNotVoidType>())
        {
            do
            {
                for (exint i = 0; i < mintuplesize; ++i, ++srcpagedata, ++destpagedata)
                    *destpagedata = *srcpagedata;
                destpagedata += desttupleextra;
                srcpagedata += srctupleextra;
            } while(destpagedata < destpageend);
        }
        else
        {
            do
            {
                for (exint i = 0; i < mintuplesize; ++i, ++srcpagedata, ++destpagedata)
                    *destpagedata = UTconvertStorage<NotVoidType>(*srcpagedata);
                destpagedata += desttupleextra;
                srcpagedata += srctupleextra;
            } while(destpagedata < destpageend);
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SrcNotVoidType>
bool
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::isEqualConst(const NotVoidType *tuple, const SrcNotVoidType *stuple, exint mintuplesize)
{
    UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

    // If they're the same page, or they're both NULL,
    // there's nothing to do.
    if ((const void*)stuple == (void*)tuple)
        return true;

    if (!stuple || !tuple)
        return false;

    UT_ASSERT_P(mintuplesize > 0);

    bool isequal;
    if (!SYSisSame<NotVoidType,SrcNotVoidType>())
    {
        // Cast to the destination type, since it's
        // supposed to represent whether the destination
        // wouldn't change if assigned.
        isequal = (tuple[0] == UTconvertStorage<NotVoidType>(stuple[0]));
        for (exint i = 1; i < mintuplesize; ++i)
            isequal &= (tuple[i] == UTconvertStorage<NotVoidType>(stuple[i]));
    }
    else
    {
        // NOTE: Don't want to copy-construct non-POD types
        //       unnecessarily by casting to NotVoidType.
        isequal = (tuple[0] == stuple[0]);
        for (exint i = 1; i < mintuplesize; ++i)
            isequal &= (tuple[i] == stuple[i]);
    }
    // If they're equal, nothing to do
    return isequal;
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable::fill(IDX_T start, IDX_T end, const NotVoidType &val)
{
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(start < end);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= myCapacity);
    UT_ASSERT_P(TSIZE >= 1);
    UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");

    UT_PageNum startpage = pageNum(start);
    UT_PageOff startoff = pageOff(start);
    UT_PageNum endpage = pageNum(end);
    UT_PageOff endoff = pageOff(end);

    bool valiszero = !PAGESHARDENED && (startoff != UT_PageOff(0) || endoff != UT_PageOff(0));
    if (valiszero)
    {
        valiszero = isZero(val);
    }

    UT_PageOff pagecapacity(thePageSize);
    if (startpage == UT_PageNum(0) && endpage == UT_PageNum(0) && exint(capacity()) < thePageSize)
        pagecapacity = capacity();

    // Handle incomplete first page
    if (startoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page);
                // Nothing to do if equal already.
                if (tuple)
                {
                    equal = true;
                    for (exint i = 0; i < TSIZE; ++i)
                        equal &= (tuple[i] == val);
                }
                else
                {
                    equal = valiszero;
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + TSIZE*((endpage != startpage) ? pagecapacity : endoff);
            data += TSIZE*startoff;
            for (; data != end; ++data)
                *data = val;
        }
        if (endpage == startpage)
            return;

        ++startpage;
    }

    // Handle complete middle pages
    for (; startpage < endpage; ++startpage)
    {
        PageTableEntry *page = getPPage(startpage);
        // FIXME: Need a makeConstant that takes a single value for non-POD types
        if (!PAGESHARDENED)
            makeConstant(page, val);
        else
        {
            NotVoidType *data = page->getFirstPtr();
            // NOTE: This isn't a small page, so we can use thePageSize
            NotVoidType *end = data + TSIZE*thePageSize;
            for (; data != end; ++data)
                *data = val;
        }
    }

    // Handle incomplete last page
    if (endoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        // If end page, and goes to end, can still make constant.
        if (!PAGESHARDENED && startpage >= numPages(mySize)-1 && endoff >= pageOff(mySize-1)+1)
        {
            makeConstant(page, val);
            return;
        }
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page);
                // Nothing to do if equal already.
                if (tuple)
                {
                    equal = true;
                    for (exint i = 0; i < TSIZE; ++i)
                        equal &= (tuple[i] == val);
                }
                else
                {
                    equal = valiszero;
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + TSIZE*endoff;
            for (; data != end; ++data)
                *data = val;
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable::fill(IDX_T start, IDX_T end, NotVoidType val, exint tuplesize)
{
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(start < end);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= myCapacity);
    UT_ASSERT_P(TSIZE == -1);
    UT_ASSERT_P(tuplesize >= 1);
    UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");

    // Fast paths for small sizes.
    if (tuplesize <= 4)
    {
        if (tuplesize == 1)
        {
            ((typename UT_PageArray<DATA_T,1,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable*)
                this)->fill(start, end, val);
        }
        else if (tuplesize == 2)
        {
            ((typename UT_PageArray<DATA_T,2,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable*)
                this)->fill(start, end, val);
        }
        else if (tuplesize == 3)
        {
            ((typename UT_PageArray<DATA_T,3,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable*)
                this)->fill(start, end, val);
        }
        else //if (tuplesize == 4)
        {
            ((typename UT_PageArray<DATA_T,4,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable*)
                this)->fill(start, end, val);
        }
        return;
    }

    UT_PageNum startpage = pageNum(start);
    UT_PageOff startoff = pageOff(start);
    UT_PageNum endpage = pageNum(end);
    UT_PageOff endoff = pageOff(end);

    bool valiszero = !PAGESHARDENED && (startoff != UT_PageOff(0) || endoff != UT_PageOff(0));
    if (valiszero)
    {
        valiszero = isZero(val);
    }

    UT_PageOff pagecapacity(thePageSize);
    if (startpage == UT_PageNum(0) && endpage == UT_PageNum(0) && exint(capacity()) < thePageSize)
        pagecapacity = capacity();

    // Handle incomplete first page
    if (startoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page, 0, tuplesize);
                // Nothing to do if equal already.
                if (tuple)
                {
                    equal = true;
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (tuple[i] == val);
                }
                else
                {
                    equal = valiszero;
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity, tuplesize);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity, tuplesize);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + tuplesize*((endpage != startpage) ? pagecapacity : endoff);
            data += tuplesize*startoff;
            for (; data != end; ++data)
                *data = val;
        }
        if (endpage == startpage)
            return;
        ++startpage;
    }

    // Handle complete middle pages
    for (; startpage < endpage; ++startpage)
    {
        PageTableEntry *page = getPPage(startpage);
        if (!PAGESHARDENED)
            makeConstant(page, val, tuplesize);
        else
        {
            NotVoidType *data = page->getFirstPtr();
            // NOTE: This isn't a small page, so we can use thePageSize
            NotVoidType *end = data + tuplesize*thePageSize;
            for (; data != end; ++data)
                *data = val;
        }
    }

    // Handle incomplete last page
    if (endoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        // If end page, and goes to end, can still make constant.
        if (!PAGESHARDENED && startpage >= numPages(mySize)-1 && endoff >= pageOff(mySize-1)+1)
        {
            makeConstant(page, val, tuplesize);
            return;
        }
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page, 0, tuplesize);
                // Nothing to do if equal already.
                if (tuple)
                {
                    equal = true;
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (tuple[i] == val);
                }
                else
                {
                    equal = valiszero;
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity, tuplesize);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity, tuplesize);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + tuplesize*endoff;
            for (; data != end; ++data)
                *data = val;
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable::fill(IDX_T start, IDX_T end, const NotVoidType *values, exint tuplesize)
{
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(start < end);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= myCapacity);
    UT_ASSERT_P(TSIZE == -1 || TSIZE==tuplesize);
    UT_ASSERT_P(tuplesize >= 1);
    UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");

    UT_PageNum startpage = pageNum(start);
    UT_PageOff startoff = pageOff(start);
    UT_PageNum endpage = pageNum(end);
    UT_PageOff endoff = pageOff(end);

    UT_PageOff pagecapacity(thePageSize);
    if (startpage == UT_PageNum(0) && endpage == UT_PageNum(0) && exint(capacity()) < thePageSize)
        pagecapacity = capacity();

    // Handle incomplete first page
    if (startoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page, 0, tuplesize);
                // Nothing to do if equal already.
                equal = true;
                if (tuple)
                {
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (tuple[i] == values[i]);
                }
                else
                {
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (NotVoidType(0) == values[i]);
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity, tuplesize);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity, tuplesize);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + tuplesize*((endpage != startpage) ? pagecapacity : endoff);
            data += tuplesize*startoff;
            while (data != end)
            {
                for (exint i = 0; i < tuplesize; ++i, ++data)
                    *data = values[i];
            }
        }
        if (endpage == startpage)
            return;
        ++startpage;
    }

    // Handle complete middle pages
    for (; startpage < endpage; ++startpage)
    {
        PageTableEntry *page = getPPage(startpage);
        if (!PAGESHARDENED)
            makeConstant(page, values, tuplesize);
        else
        {
            NotVoidType *data = page->getFirstPtr();
            // NOTE: This isn't a small page, so we can use thePageSize
            NotVoidType *end = data + tuplesize*thePageSize;
            while (data != end)
            {
                for (exint i = 0; i < tuplesize; ++i, ++data)
                    *data = values[i];
            }
        }
    }

    // Handle incomplete last page
    if (endoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        // If end page, and goes to end, can still make constant.
        if (!PAGESHARDENED && startpage >= numPages(mySize)-1 && endoff >= pageOff(mySize-1)+1)
        {
            makeConstant(page, values, tuplesize);
            return;
        }
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const NotVoidType *tuple = getConstantPtr(page, 0, tuplesize);
                // Nothing to do if equal already.
                equal = true;
                if (tuple)
                {
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (tuple[i] == values[i]);
                }
                else
                {
                    for (exint i = 0; i < tuplesize; ++i)
                        equal &= (NotVoidType(0) == values[i]);
                }
                if (!equal)
                    hardenConstantPage(page, pagecapacity, tuplesize);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity, tuplesize);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            NotVoidType *data = page->getFirstPtr();
            NotVoidType *end = data + tuplesize*endoff;
            while (data != end)
            {
                for (exint i = 0; i < tuplesize; ++i, ++data)
                    *data = values[i];
            }
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
void
UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTable::fill(IDX_T start, IDX_T end, const UT_FixedVector<NotVoidType,theSafeTupleSize> &val)
{
    UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
    UT_ASSERT_P(start < end);
    UT_ASSERT_P(start >= IDX_T(0));
    UT_ASSERT_P(end <= myCapacity);
    UT_ASSERT_P(TSIZE >= 1);
    UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");

    UT_PageNum startpage = pageNum(start);
    UT_PageOff startoff = pageOff(start);
    UT_PageNum endpage = pageNum(end);
    UT_PageOff endoff = pageOff(end);

    UT_PageOff pagecapacity(thePageSize);
    if (startpage == UT_PageNum(0) && endpage == UT_PageNum(0) && exint(capacity()) < thePageSize)
        pagecapacity = capacity();

    // Handle incomplete first page
    if (startoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const UT_FixedVector<NotVoidType,theSafeTupleSize> *tuple = (const UT_FixedVector<NotVoidType,theSafeTupleSize> *)getConstantPtr(page);
                // Nothing to do if equal already.
                equal = tuple ? (*tuple == val) : val.isZero();
                if (!equal)
                    hardenConstantPage(page, pagecapacity);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            UT_FixedVector<NotVoidType,theSafeTupleSize> *data = (UT_FixedVector<NotVoidType,theSafeTupleSize> *)page->getFirstPtr();
            UT_FixedVector<NotVoidType,theSafeTupleSize> *end = data + ((endpage != startpage) ? pagecapacity : endoff);
            data += startoff;
            for (; data != end; ++data)
                *data = val;
        }
        if (endpage == startpage)
            return;

        ++startpage;
    }

    // Handle complete middle pages
    for (; startpage < endpage; ++startpage)
    {
        PageTableEntry *page = getPPage(startpage);
        if (!PAGESHARDENED)
            makeConstant(page, val);
        else
        {
            UT_FixedVector<NotVoidType,theSafeTupleSize> *data = (UT_FixedVector<NotVoidType,theSafeTupleSize> *)page->getFirstPtr();
            // NOTE: This isn't a small page, so we can use thePageSize
            UT_FixedVector<NotVoidType,theSafeTupleSize> *end = data + thePageSize;
            for (; data != end; ++data)
                *data = val;
        }
    }

    // Handle incomplete last page
    if (endoff != UT_PageOff(0))
    {
        PageTableEntry *page = getPPage(startpage);
        // If end page, and goes to end, can still make constant.
        if (!PAGESHARDENED && startpage >= numPages(mySize)-1 && endoff >= pageOff(mySize-1)+1)
        {
            makeConstant(page, val);
            return;
        }
        bool equal = false;
        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                const UT_FixedVector<NotVoidType,theSafeTupleSize> *tuple = (const UT_FixedVector<NotVoidType,theSafeTupleSize> *)getConstantPtr(page);
                // Nothing to do if equal already.
                equal = tuple ? (*tuple == val) : val.isZero();
                if (!equal)
                    hardenConstantPage(page, pagecapacity);
            }
            else if (page->isShared())
                hardenSharedPage(page, pagecapacity);
        }
        if (!equal)
        {
            UT_ASSERT_P(!page->isConstant());
            UT_ASSERT_P(!page->isShared());
            UT_FixedVector<NotVoidType,theSafeTupleSize> *data = (UT_FixedVector<NotVoidType,theSafeTupleSize> *)page->getFirstPtr();
            UT_FixedVector<NotVoidType,theSafeTupleSize> *end = data + endoff;
            for (; data != end; ++data)
                *data = val;
        }
    }
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename DEST_DATA_T,exint DEST_TSIZE,bool DEST_INSTANTIATED>
void
UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, THEPAGEBITS, IDX_T>::getVectorRange(IDX_T srcstart, IDX_T nelements, UT_FixedVector<DEST_DATA_T,DEST_TSIZE,DEST_INSTANTIATED> *dest) const
{
    // If the source storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<DATA_T,void>())
    {
        UT_Storage storage = getStorage();

        // Probably matches destination type
        if (storage == UT_StorageNum<DEST_DATA_T>::theStorage)
        {
            castType<DEST_DATA_T>().getVectorRange(srcstart, nelements, dest);
            return;
        }

        switch (storage)
        {
            case UT_Storage::INT8:
                castType<int8>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::INT16:
                castType<int16>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::INT32:
                castType<int32>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::INT64:
                castType<int64>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().getVectorRange(srcstart, nelements, dest); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
                break;
        }
        return;
    }

    // We now have both the source type and the destination type known at compile time.
    UT_ASSERT_P((!SYSisSame<DATA_T,void>()));

    // Tuple size probably matches
    if (TSIZE == -1 && myImpl.getTupleSize() == DEST_TSIZE)
    {
        castTupleSize<DEST_TSIZE>().getVectorRange(srcstart, nelements, dest);
        return;
    }

    auto vdest = reinterpret_cast<UT_FixedVector<DEST_DATA_T,DEST_TSIZE> *>(dest);

    // TODO: Implement this more efficiently, e.g. only check once whether each page is constant or shared.
    for (IDX_T srcend(srcstart+nelements); srcstart < srcend; ++srcstart, ++vdest)
        *vdest = getVector<DEST_DATA_T,DEST_TSIZE>(srcstart);
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_INSTANTIATED>
void
UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, THEPAGEBITS, IDX_T>::setVectorRange(IDX_T deststart, IDX_T nelements, const UT_FixedVector<SRC_DATA_T,SRC_TSIZE,SRC_INSTANTIATED> *src)
{
    if (nelements <= IDX_T(0))
        return;

    auto &hard = hardenTable();

    UT_ASSERT_MSG_P((!SYSisSame<SRC_DATA_T,void>()), "Source type must be known.");

    // If the destination storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<DATA_T,void>())
    {
        UT_Storage storage = getStorage();

        // Probably matches source type
        if (storage == UT_StorageNum<SRC_DATA_T>::theStorage)
        {
            hard.template castType<SRC_DATA_T>().setVectorRange(deststart, nelements, src);
            return;
        }

        switch (storage)
        {
            case UT_Storage::INT8:
                hard.template castType<int8>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::INT16:
                hard.template castType<int16>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::INT32:
                hard.template castType<int32>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::INT64:
                hard.template castType<int64>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::REAL16:
                hard.template castType<fpreal16>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::REAL32:
                hard.template castType<fpreal32>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::REAL64:
                hard.template castType<fpreal64>().setVectorRange(deststart, nelements, src); return;
            case UT_Storage::INVALID:
                UT_ASSERT_MSG(0, "Can't have a UT_PageArray with DATA_T void and invalid storage!");
                break;
        }
        return;
    }

    // We now have both the source type and the destination type known at compile time.
    UT_ASSERT_P((!SYSisSame<DATA_T,void>()));

    // Tuple size probably matches
    if (TSIZE == -1 && myImpl.getTupleSize() == SRC_TSIZE)
    {
        hard.template castTupleSize<SRC_TSIZE>().setVectorRange(deststart, nelements, src);
        return;
    }

    // TODO: Implement this more efficiently, e.g. only check once whether each page is constant or shared.
    for (IDX_T destend(deststart+nelements); deststart < destend; ++deststart, ++src)
        setVector(deststart, *src);
}

template<typename DATA_T,exint TSIZE,bool TABLEHARDENED,bool PAGESHARDENED,exint THEPAGEBITS,typename IDX_T>
bool
UT_PageArray<DATA_T, TSIZE, TABLEHARDENED, PAGESHARDENED, THEPAGEBITS, IDX_T>::hasNanInRange(IDX_T start, IDX_T end) const
{
    if (!UTisFloatStorage(getStorage()))
        return false;

    // If the storage type is not known at compile time,
    // switch, cast, and call again.
    if (SYSisSame<DATA_T,void>())
    {
        UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::REAL16:
                return castType<fpreal16>().hasNanInRange(start, end);
            case UT_Storage::REAL32:
                return castType<fpreal32>().hasNanInRange(start, end);
            case UT_Storage::REAL64:
                return castType<fpreal64>().hasNanInRange(start, end);
            default:
                UT_ASSERT_MSG(0, "Only 16-bit, 32-bit, and 64-bit floats should be considered float types!");
                break;
        }
        return false;
    }

    UT_ASSERT_P(start >= IDX_T(0) && start <= size());
    UT_ASSERT_P(end >= IDX_T(0) && end <= size());
    UT_ASSERT_P(start <= end);

    if (start >= end)
        return false;

    const PageTable *pages = myImpl.getPages();
    UT_ASSERT_P(pages);

    UT_PageNum pagenum = pageNum(start);
    UT_PageOff pageoff = pageOff(start);
    UT_PageNum endpagenum = pageNum(end);
    UT_PageOff endpageoff = pageOff(end);

    exint tuplesize = getTupleSize();

    if (endpageoff == UT_PageOff(0))
    {
        --endpagenum;
        endpageoff = UT_PageOff(thePageSize);
    }

    for (; pagenum <= endpagenum; ++pagenum)
    {
        const PageTableEntry *const page = pages->getPPage(pagenum);
        if (page->isConstant())
        {
            const NotVoidType *data = getConstantPtr(page, 0, tuplesize);
            // Special case for zero page is always a number.
            if (!data)
                continue;
            for (exint i = 0; i < tuplesize; ++i)
            {
                if (SYSisNan(data[i]))
                    return true;
            }
        }
        else
        {
            const NotVoidType *data = page->getFirstPtr();
            const NotVoidType *end = data + ((pagenum == endpagenum) ? endpageoff : thePageSize*tuplesize);
            data += pageoff;
            for (; data != end; ++data)
            {
                if (SYSisNan(*data))
                    return true;
            }
        }
        pageoff = UT_PageOff(0);
    }

    return false;
}


#endif
