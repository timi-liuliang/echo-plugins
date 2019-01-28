/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_PageArray.h (UT Library, C++)
 *
 * COMMENTS:    An array class with special handling of constant pages and
 *              shared page data.
 */

#pragma once

#ifndef __UT_PageArray__
#define __UT_PageArray__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_SmallArray.h"
#include "UT_Storage.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_BitUtil.h>
#include <SYS/SYS_CallIf.h>
#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
// Default template arguments on functions are legal.  Don't error on them!
#pragma warning(disable: 4519)
#endif


typedef exint UT_PageNum;
typedef exint UT_PageOff;


class UT_Defaults;
class UT_MemoryCounter;


/// @internal
///                     W  E  L  C  O  M  E
///                          to scenic
///                        Template Hell
///
///                       Population: You
///
/// If you're just using this class, hopefully you'll have a short and
/// not-so-painful visit!
///
/// If, however, you're attempting to modify this class, BEWARE!
/// Any missteps can and likely will result in application of the
/// Doughnut Rules, so be sure to review them before visiting.
/// It may take several days to escape, so it may be best to just
/// revert and make absolutely sure that it works on all platforms
/// before committing again.
///
/// This class is the result of a wide variety of needs for a wide variety
/// of use cases, most of which need both good performance and flexibility.
/// You may want to skip reading this gigantic comment, but whether it's
/// immediately or in a few months, you'll realize that skipping reading
/// this was a huge mistake.
///
/// This array consists of a pointer to a page table, optionally a
/// tuple size (if TSIZE==-1), and optionally a UT_Storage indicating
/// the storage type (if DATA_T is void).  The page table, if not nullptr,
/// can be shared, and has the format: @code
/// +-------------------+
/// | ref count         | (1 if unshared; >1 if shared)
/// +-------------------+
/// | array size        | (number of real elements/tuples in the array)
/// +-------------------+
/// | array capacity    | (number of allocated elements/tuples if all pages of the table were hardened)
/// +-------------------+
/// | page 0 pointer    | (pointer to page if bit 0 is 0, else constant page representation)
/// +-------------------+
/// | page 1 pointer    |
/// +-------------------+
/// | page 2 pointer    |
/// +-------------------+
/// |       ...         |
/// +-------------------+
/// @endcode
///
/// If a page pointer has bit 0 clear, (i.e. an address aligned to a 2-byte boundary),
/// it is a non-constant page, it can be shared, and has the format: @code
/// +-------------------+
/// | ref count         | (1 if unshared; >1 if shared)
/// +-------------------+
/// | (unused)          | (padding so that data starts on a 16-byte boundary)
/// +-------------------+
/// | tuple 0           |
/// +-------------------+
/// | tuple 1           |
/// +-------------------+
/// | tuple 2           |
/// +-------------------+
/// |       ...         |
/// +-------------------+
/// @endcode
///
/// A normal capacity page contains 2^THEPAGEBITS tuples, e.g. the default
/// THEPAGEBITS==10 gives a page size of 1024 tuples.  If there is exactly
/// one page, it may have a capacity that is any power of two up to
/// THEPAGEBITS, which can be checked by checking if the overall array has
/// capacity less than 2^THEPAGEBITS.
///
/// If, instead, the page pointer has bit 0 set to 1, which would be an odd
/// address, all tuples in the page are the same.  There are 3 ways this may
/// be represented:
///
/// 1) If the size of a full tuple is strictly smaller than the size of a
/// pointer, (e.g. 7 bytes or fewer on a 64-bit platform), the constant value
/// will be stored inside the space that would have been a pointer, aligned
/// to the end of the pointer.  That ensures that 4-byte tuples will be
/// aligned to 4 bytes, and 2- or 6-byte tuples will be aligned to 2 bytes.
/// This is most useful for fpreal32[1] and int32[1] (i.e. 4-byte scalars).
///
/// 2) If the page pointer is equal to 1 (i.e. nullptr apart from bit 0 being 1),
/// the value is all zero.  This covers the extremely
/// common case of a zero default value, especially for fpreal32[3] and
/// fpreal64[3].
///
/// 3) Otherwise, the page pointer, ignoring bit zero, points to a page
/// that contains just a single tuple.
///
/// That covers the basic structure.  It sounds so nice and simple...
/// but then there are the templates.
///
/// DATA_T:
///
/// If DATA_T is something other than void, that type is the type of each
/// component of each tuple.  This is the only way that non-numeric types
/// are possible; DATA_T can also be a numeric type.  If DATA_T is void,
/// myImpl contains a UT_Storage variable indicating what the data type is;
/// it must be a numeric type, i.e. it cannot be UT_Storage::INVALID.
/// The ...UnknownType functions switch on the UT_Storage to determine the
/// data type and call the corresponding function.  If DATA_T is non-void,
/// it saves a lot of extra work at runtime.  Because the codepaths that
/// will never be called due to DATA_T checks still need to compile with
/// DATA_T being void, NotVoidType ensures that there is some arbitrary type
/// in place so that the code will compile, even though it will never be
/// called.  The same issue prevents using UT_ASSERT_COMPILETIME to verify
/// DATA_T, so UT_ASSERT_P is used instead.
/// myImpl is always structured so that if the type becomes known, by checking
/// getStorage(), this can be cast to have the now-known DATA_T set.
/// For example: @code
/// if (array.getStorage() == UT_Storage::REAL32)
///     array.castType<fpreal32>().someFunctionCall();
/// @endcode
///
/// TSIZE:
///
/// If TSIZE >= 1, it is the tuple size (the number of components in each
/// tuple), and is known at compile time, avoiding extra work at compile-time
/// that depends on the tuple size, especially if TSIZE is 1.  Odds are that
/// for non-numeric types, TSIZE will almost always be 1, since it rarely
/// makes sense to be something else.  If TSIZE is 0, something is almost
/// certainly wrong.  If the tuple size is not known at compile-time, TSIZE
/// must be -1.  In that case, myImpl contains an integer indicating the tuple
/// size.  Like with DATA_T, because the codepaths that will never be called
/// due to TSIZE checks still need to compile with other TSIZE values, so
/// UT_ASSERT_COMPILETIME can't be used to verify TSIZE; UT_ASSERT_P is used
/// instead.  There are a number of functions that require TSIZE to be the
/// tuple size, in order to avoid having to send an extra parameter and
/// incurring the slowdowns of their counterparts that assume that the tuple
/// size is what's passed in.  If you modify one, REMEMBER TO MODIFY THE OTHER!
/// myImpl is always structured so that if the tuple size becomes
/// known, by checking the getTupleSize(), this can be cast to have the
/// now-known TSIZE set.
/// For example: @code
/// if (array.getTupleSize() == 3)
///     array.castTupleSize<3>().someFunctionCall();
/// @endcode
///
/// TABLEHARDENED:
///
/// Since page tables can be shared if all of the data are identical, modifying
/// the array in any way requires copying (hardening) the page table and may
/// require copying one or more shared pages or hardening constant pages.
/// Since every modification requires hardening the page table, it's useful
/// to harden the page table once and be able to skip checking whether it needs
/// hardening for all modifications after that.  Calling hardenTable()
/// will ensure that the table is hardened, and return this with TABLEHARDENED
/// set to true, so that further calls on it to modify the array won't need to
/// check whether the table needs hardening.  The page table should be
/// quite light compared to the data, so it's not the end of the world if
/// the table is hardened pre-emptively and there end up being no actual
/// modifications.
///
/// PAGESHARDENED:
///
/// When writing data in parallel to random locations, all pages that could
/// be written-to must be hardened in advance, to avoid a race condition on
/// the hardening of any page.  Also, if it's known that most pages will
/// likely be hardened anyway when writing data, it'd be advantageous to
/// be able to harden all pages in advance and skip checking when making
/// modifications.  The code for hardening pages just-in-time can
/// dramatically increase code size and time overhead.  To avoid this,
/// hardenAllPages() will ensure that the table and all pages are hardened,
/// and will return this with TABLEHARDENED and PAGESHARDENED both set to true.
/// Any calls to modify page data made on that returned pointer won't check
/// whether any pages need hardening, because it will know that all pages are
/// already hardened.  If you use this, be sure not to use the pointer
/// with PAGESHARDENED set to true after any call to tryCompressAllPages()
/// on the original array pointer.
///
/// THEPAGEBITS:
///
/// Full pages will contain 2^THEPAGEBITS tuples, e.g. THEPAGEBITS==10
/// means there will be 1024 tuples per full page.
///
/// IDX_T:
///
/// This allows templating on the array index type so that strict types
/// can be used, e.g. GA_Offset.
///
/// FIXME BEFORE USING:
/// - Should make generic UT_R[OW]Handle.+ classes for easy accessing
///   of runtime-determined data type and tuple size data as tuples
///   with type and tuple size determined at compile-time.
/// - Make UT_PageNum and UT_PageOff strict types when strict types are
///   enabled.
/// - Add more functions for page-level access and block (bulk sequential)
///   access, for easy reading/writing to/from flat arrays.
/// @endinternal
template<typename DATA_T,exint TSIZE=1,bool TABLEHARDENED=false,bool PAGESHARDENED=false,exint THEPAGEBITS=10,typename IDX_T=exint>
class UT_PageArray
{
protected:
    /// This class is just to get code to compile when DATA_T is void,
    /// by picking any type other than void if it is void.
    template<typename T>
    struct NotVoid
    { typedef typename SYS_SelectType<T,float,SYS_IsSame<T,void>::value>::type type; };

    typedef typename NotVoid<DATA_T>::type NotVoidType;

    // These friend declarations are needed for calling protected functions on hardened/cast types.
    template<typename DATA_T2,exint TSIZE2,bool TABLEHARDENED2,bool PAGESHARDENED2,exint THEPAGEBITS2,typename IDX_T2>
    friend class UT_PageArray;
public:
    typedef UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> ThisType;
    typedef DATA_T DataType;
    static const UT_Storage theStorage = UT_StorageNum<DATA_T>::theStorage;
    static const exint theTupleSize = TSIZE;
    static const exint theSafeTupleSize = (TSIZE==-1) ? 3 : TSIZE;
    static const exint thePageBits = THEPAGEBITS;
    static const exint thePageSize = (exint(1)<<thePageBits);
    static const exint thePageMask = thePageSize-1;
    typedef IDX_T IndexType;

    /// The default constructor can only be used if the tuple size
    /// and storage type are known at compile time.
    UT_PageArray()
    {
        UT_ASSERT_P(TSIZE >= 1);
        UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
        myImpl.getPages() = nullptr;
    }

    /// This constructor can only be used if the storage type is
    /// known at compile time, but the tuple size is not.
    UT_PageArray(exint tuplesize)
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == tuplesize);
        UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
        // NOTE: This is one case where a tuple size of zero is okay.
        UT_ASSERT_P(tuplesize >= 0);
        myImpl.getPages() = nullptr;
        myImpl.setTupleSize(tuplesize);
    }

    /// This constructor can only be used if the storage type is
    /// known at compile time, but the tuple size is not.
    UT_PageArray(UT_Storage storage)
    {
        UT_ASSERT_P(TSIZE >= 1);
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value) || (storage == UT_StorageNum<DATA_T>::theStorage));
        UT_ASSERT_P(storage != UT_Storage::INVALID);
        myImpl.getPages() = nullptr;
        myImpl.setStorage(storage);
    }

    /// This constructor can only be used if the tuple size and
    /// storage type are unknown at compile time.
    UT_PageArray(exint tuplesize,UT_Storage storage)
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == tuplesize);
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value) || (storage == UT_StorageNum<DATA_T>::theStorage));
        // NOTE: This is one case where a tuple size of zero is okay.
        UT_ASSERT_P(tuplesize >= 0);
        UT_ASSERT_P(storage != UT_Storage::INVALID);
        myImpl.getPages() = nullptr;
        myImpl.setStorage(storage);
        myImpl.setTupleSize(tuplesize);
    }

    /// This just references that's page table, unless TABLEHARDENED
    /// or PAGESHARDENED.  If TABLEHARDENED and !PAGESHARDENED,
    /// the pages are referenced, and if PAGESHARDENED, the pages
    /// are copied.
    UT_PageArray(const ThisType &that)
    {
        myImpl.getPages() = nullptr;
        *this = that;
    }

    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    explicit UT_PageArray(const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> &that)
    {
        myImpl.getPages() = nullptr;
        *this = that;
    }

    SYS_FORCE_INLINE
    ~UT_PageArray()
    {
        clearAndDestroy();
    }

    SYS_FORCE_INLINE
    void clear()
    {
        if (myImpl.getPages())
            myImpl.getPages()->setSize(IDX_T(0));
    }

    void clearAndDestroy()
    {
        if (!myImpl.getPages())
            return;

        const exint tuplesize = myImpl.getTupleSize();

        // Just decrement the reference count to free the table safely
        if (!SYSisSame<DATA_T,void>())
            myImpl.getPages()->decRef(tuplesize);
        else
        {
            const UT_Storage storage = myImpl.getStorage();
            switch (storage)
            {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                return;
            case UT_Storage::INT8:
                castType<int8>().myImpl.getPages()->decRef(tuplesize);     return;
            case UT_Storage::INT16:
                castType<int16>().myImpl.getPages()->decRef(tuplesize);    return;
            case UT_Storage::INT32:
                castType<int32>().myImpl.getPages()->decRef(tuplesize);    return;
            case UT_Storage::INT64:
                castType<int64>().myImpl.getPages()->decRef(tuplesize);    return;
            case UT_Storage::REAL16:
                castType<fpreal16>().myImpl.getPages()->decRef(tuplesize); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().myImpl.getPages()->decRef(tuplesize); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().myImpl.getPages()->decRef(tuplesize); return;
            }
        }
        myImpl.getPages() = nullptr;
    }

    /// NOTE: Compilers seem not to use the operator= below when the template
    ///       arguments match, so this operator= is necessary in order to
    ///       avoid the compiler silently using its implicitly-defined
    ///       assignment operator.
    SYS_FORCE_INLINE
    ThisType &operator=(const ThisType &that)
    {
        replace(that);
        return *this;
    }

    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    SYS_FORCE_INLINE
    ThisType &operator=(const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> &that)
    {
        replace(that);
        return *this;
    }

    /// Completely replace this UT_PageArray with that, referencing
    /// the pages or table, if allowed by the template arguments.
    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    void replace(const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> &that)
    {
        // NOTE: Careful if you add template parameter checks on this,
        //       because, e.g., a UT_PageArray<void,-1> could refer to the
        //       same exact array as a UT_PageArray<float 3>.
        if ((void *)this == (const void *)&that)
            return;
        // NOTE: The storage and tuple size checks are so that if both
        //       tables are nullptr, it still copies the storage and tuple
        //       size below.
        if (!TABLEHARDENED &&
                myImpl.getStorage() == that.myImpl.getStorage() &&
                myImpl.getTupleSize() == that.myImpl.getTupleSize() &&
                (void *)myImpl.getPages() == (const void *)that.myImpl.getPages())
            return;

        clearAndDestroy();

        if (SYSisSame<DATA_T,void>())
            myImpl.setStorage(that.myImpl.getStorage());
        if (TSIZE == -1)
            myImpl.setTupleSize(that.myImpl.getTupleSize());

        // May be able to share pages or table if matching tuple size and storage type
        if ((SYSisSame<DATA_T,void>() || SYSisSame<DATA_T,SRC_DATA_T>()) && (TSIZE == -1 || TSIZE == SRC_TSIZE))
        {
            PageTable *pages = reinterpret_cast<PageTable *>(SYSconst_cast(that.myImpl.getPages()));
            myImpl.getPages() = pages;
            if (!pages)
                return;
            pages->incRef();
            if (TABLEHARDENED || PAGESHARDENED || SRC_TABLEHARDENED || SRC_PAGESHARDENED)
            {
                forceHardenTable(pages->capacity());
                if (PAGESHARDENED || SRC_PAGESHARDENED)
                {
                    // If we don't cast to PAGESHARDENED==false, it won't do anything.
                    ((UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,false,THEPAGEBITS,IDX_T>*)this)
                        ->template hardenAllPages<false>();
                }
            }
        }
        else
        {
            // Can't share table or pages, so just use moveRange
            IDX_T capacity = that.capacity();
            IDX_T size = that.size();
            forceHardenTable(capacity);
            moveRange(that, IDX_T(0), IDX_T(0), size);
        }
    }

    /// Read element i's specified component (default component 0).
    SYS_FORCE_INLINE NotVoidType operator()(IDX_T i,exint component=0) const
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE != 0);
        UT_ASSERT_P(myImpl.getTupleSize() >= 1);
        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        UT_ASSERT_P(component >= 0 && component < myImpl.getTupleSize());

        UT_PageNum pagenum = pageNum(i);
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);

        // If we know that the pages are hardened, we can omit
        // the constant page case at compile time.
        bool isconst = !PAGESHARDENED && page->isConstant();

        if (TSIZE >= 1)
        {
            if (!isconst)
            {
                UT_PageOff pageoff = pageOff(i);
                return page->getFirstPtr()[TSIZE*pageoff + component];
            }
            return getConstant(page, component);
        }
        else
        {
            if (!isconst)
            {
                UT_PageOff pageoff = pageOff(i);
                return page->getFirstPtr()[myImpl.getTupleSize()*pageoff + component];
            }
            return getConstant(page, component, myImpl.getTupleSize());
        }
    }

    /// This get function may seem redundant, given operator(), but it gives a
    /// simple way to read from a non-const UT_PageArray without calling the
    /// non-const operator() and potentially hardening pages.  It just
    /// explicitly adds const.  It also allows data type conversion and
    /// reading when the type is unknown at compile time.
    /// It should hopefully be inlined in the no-conversion case;
    /// it's less critical in the conversion case.
    template<typename DEST_DATA_T=DATA_T>
    SYS_FORCE_INLINE DEST_DATA_T get(IDX_T i,exint component=0) const
    {
        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        if (!(SYSisSame<DATA_T,void>()) && (SYSisSame<DEST_DATA_T,DATA_T>() || theStorage != UT_Storage::INVALID))
            return UTconvertStorage<DEST_DATA_T>((*this)(i,component));

        return getUnknownType<DEST_DATA_T>(i,component);
    }

    template<typename DEST_DATA_T=NotVoidType,exint DEST_TSIZE=theSafeTupleSize>
    SYS_FORCE_INLINE UT_FixedVector<DEST_DATA_T,DEST_TSIZE> getVector(IDX_T i) const
    {
        if (SYSisSame<DATA_T,void>())
        {
            UT_FixedVector<DEST_DATA_T,DEST_TSIZE> dest;
            SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,&dest,i](SYS_CALLIF_AUTO){
                dest = this->getVectorUnknownType<DEST_DATA_T,DEST_TSIZE>(i);
            });
            return dest;
        }
        if (!SYSisSame<DEST_DATA_T,DATA_T>())
        {
            UT_FixedVector<DEST_DATA_T,DEST_TSIZE> dest;
            SYS_CallIf<!SYSisSame<DEST_DATA_T,DATA_T>()>::call([this,&dest,i](SYS_CALLIF_AUTO){
                dest = this->convertVectorStorage<DEST_DATA_T>(this->getVector<NotVoidType,DEST_TSIZE>(i));
            });
            return dest;
        }

        if (TSIZE==-1)
        {
            UT_FixedVector<DEST_DATA_T,DEST_TSIZE> dest;
            SYS_CallIf<TSIZE==-1>::call([this,&dest,i](SYS_CALLIF_AUTO){
                dest = this->getVectorUnknownSize<DEST_DATA_T,DEST_TSIZE>(i);
            });
            return dest;
        }
        if (TSIZE!=DEST_TSIZE)
        {
            UT_FixedVector<DEST_DATA_T,DEST_TSIZE> dest;
            SYS_CallIf<TSIZE!=DEST_TSIZE>::call([this,&dest,i](SYS_CALLIF_AUTO){
                dest = this->getVectorMismatchedSize<DEST_DATA_T,DEST_TSIZE>(i);
            });
            return dest;
        }

        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        UT_ASSERT_P(TSIZE==DEST_TSIZE);
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()));

        // Types and tuple sizes are known and match
        UT_PageNum pagenum = pageNum(i);
        // NOTE: We can dereference here because we're returning by value.
        //       getConstantPtr may return a pointer into this stack variable
        //       if the page is constant and inline.
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);

        // If we know that the pages are hardened, we can omit
        // the constant page case at compile time.
        bool isconst = !PAGESHARDENED && page->isConstant();

        if (!isconst)
        {
            UT_PageOff pageoff = pageOff(i);
            return ((const UT_FixedVector<DEST_DATA_T,DEST_TSIZE>*)page->getFirstPtr())[pageoff];
        }
        const NotVoidType *constdata = getConstantPtr(page);
        if (!constdata)
            return UT_FixedVector<DEST_DATA_T,DEST_TSIZE>(DEST_DATA_T(0));

        return *(const UT_FixedVector<DEST_DATA_T,DEST_TSIZE>*)constdata;
    }

    /// Get a non-const reference to element i's specified component
    /// (default component 0).
    /// WARNING: DO NOT call this if DATA_T is void!
    /// NOTE: Unlike set(), this must always harden, even if you're just
    ///       writing a value that is already there.
    SYS_FORCE_INLINE NotVoidType &operator()(IDX_T i,exint component=0)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE != 0);
        UT_ASSERT_P(myImpl.getTupleSize() >= 1);
        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        UT_ASSERT_P(component >= 0 && component < myImpl.getTupleSize());

        // NOTE: This is a no-op if TABLEHARDENED is already true.
        hardenTable();

        UT_PageNum pagenum = pageNum(i);
        UT_PageOff pageoff = pageOff(i);
        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                if (TSIZE >= 1)
                    hardenConstantPage(pages, page);
                else
                    hardenConstantPage(pages, page, myImpl.getTupleSize());
            }
            else if (page->isShared())
            {
                if (TSIZE >= 1)
                    hardenSharedPage(pages, page);
                else
                    hardenSharedPage(pages, page, myImpl.getTupleSize());
            }
        }

        NotVoidType *data = page->getFirstPtr()
                + pageoff*((TSIZE >= 1) ? TSIZE : myImpl.getTupleSize())
                + component;

        return *data;
    }

    /// component == 0 in this version
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void set(IDX_T i,SRC_DATA_T v)
    {
        set(i, 0, v);
    }
    /// component == 0 in this version
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void add(IDX_T i,SRC_DATA_T v)
    {
        add(i, 0, v);
    }

    /// This looks gigantic, but it's mostly because of the different
    /// behaviour for different template values.  If
    /// PAGESHARDENED is true, it'll skip checking for constant
    /// or shared pages altogether.
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void set(IDX_T i,exint component,SRC_DATA_T v)
    {
        op<0>(i, component, v);
    }
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void add(IDX_T i,exint component,SRC_DATA_T v)
    {
        op<1>(i, component, v);
    }
    template<int OP,typename SRC_DATA_T>
    SYS_FORCE_INLINE void op(IDX_T i,exint component,SRC_DATA_T v)
    {
        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        UT_ASSERT_P(component >= 0 && component < getTupleSize());
        UT_ASSERT_MSG_P(OP == 0 || OP == 1, "Only OP 0 (set) and 1 (add) are defined!");

        if (SYSisSame<DATA_T,void>())
        {
            opUnknownType<OP>(i, component, v);
            return;
        }

        // Make sure that v is the correct type before comparing.
        NotVoidType vt(UTconvertStorage<NotVoidType>(v));

        UT_ASSERT_P(TSIZE != 0);
        UT_ASSERT_P(myImpl.getTupleSize() >= 1);

        hardenTable();

        UT_PageNum pagenum = pageNum(i);
        UT_PageOff pageoff = pageOff(i);
        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                // If we're setting, check to see if it's a new value
                if (OP == 0)
                {
                    if (TSIZE >= 1)
                    {
                        if (getConstant(page, component) == vt)
                            return;
                    }
                    else
                    {
                        if (getConstant(page, component, myImpl.getTupleSize()) == vt)
                            return;
                    }
                }
                if (TSIZE >= 1)
                    hardenConstantPage(pages, page);
                else
                    hardenConstantPage(pages, page, myImpl.getTupleSize());
            }
            else if (page->isShared())
            {
                // If we're setting, check to see if it's a new value
                if (OP == 0)
                {
                    NotVoidType *data = page->getFirstPtr()
                            + pageoff*((TSIZE >= 1) ? TSIZE : myImpl.getTupleSize())
                            + component;
                    if (*data == vt)
                        return;
                }
                if (TSIZE >= 1)
                    hardenSharedPage(pages, page);
                else
                    hardenSharedPage(pages, page, myImpl.getTupleSize());
            }
        }

        NotVoidType *data = page->getFirstPtr()
                + pageoff*((TSIZE >= 1) ? TSIZE : myImpl.getTupleSize())
                + component;

        if (OP == 0)
            *data = vt;
        else if (OP == 1)
            *data += vt;
        else
        {
            UT_ASSERT_MSG_P(0,"Only OP 0 (set) and 1 (add) are defined!");
        }
    }

    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_INSTANTIATED>
    SYS_FORCE_INLINE void setVector(IDX_T i,const UT_FixedVector<SRC_DATA_T,SRC_TSIZE,SRC_INSTANTIATED> &v)
    {
        opVector<0>(i, reinterpret_cast<const UT_FixedVector<SRC_DATA_T,SRC_TSIZE> &>(v));
    }
    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_INSTANTIATED>
    SYS_FORCE_INLINE void addVector(IDX_T i,const UT_FixedVector<SRC_DATA_T,SRC_TSIZE,SRC_INSTANTIATED> &v)
    {
        opVector<1>(i, reinterpret_cast<const UT_FixedVector<SRC_DATA_T,SRC_TSIZE> &>(v));
    }
    /// This looks gigantic, but it's mostly because of the different
    /// behaviour for different template values.  If
    /// PAGESHARDENED is true, it'll skip checking for constant
    /// or shared pages altogether.
    template<int OP,typename SRC_DATA_T,exint SRC_TSIZE>
    SYS_FORCE_INLINE void opVector(IDX_T i,const UT_FixedVector<SRC_DATA_T,SRC_TSIZE> &v)
    {
        UT_ASSERT_P(i >= IDX_T(0) && i < size());
        UT_ASSERT_MSG_P(OP == 0 || OP == 1, "Only OP 0 (set) and 1 (add) are defined!");

        if (SYSisSame<DATA_T,void>())
        {
            SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,i,&v](SYS_CALLIF_AUTO){
                this->opVectorUnknownType<OP>(i, v);
            });
            return;
        }

        const exint tuplesize = myImpl.getTupleSize();
        if (TSIZE == -1 && tuplesize == SRC_TSIZE)
        {
            SYS_CallIf<TSIZE == -1>::call([this,i,&v](SYS_CALLIF_AUTO){
                this->castTupleSize<SRC_TSIZE>().template opVector<OP>(i, v);
            });
            return;
        }
        const exint mintuplesize = SYSmin(tuplesize, SRC_TSIZE);

        // Make sure that v is the correct type before comparing.
        // Hopefully this will avoid copying if SRC_DATA_T is NotVoidType.
        constexpr bool TYPE_MATCH = SYSisSame<SRC_DATA_T,NotVoidType>();
        UT_FixedVector<NotVoidType, SRC_TSIZE> converted_v;
        if (!TYPE_MATCH)
            converted_v = convertVectorStorage<NotVoidType>(v);

        UT_ASSERT_P(TSIZE != 0);
        UT_ASSERT_P(myImpl.getTupleSize() >= 1);

        hardenTable();

        UT_PageNum pagenum = pageNum(i);
        UT_PageOff pageoff = pageOff(i);
        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        if (!PAGESHARDENED)
        {
            if (page->isConstant())
            {
                // If we're setting, check to see if it's a new value
                if (OP == 0)
                {
                    const NotVoidType *data;
                    if (PageTableEntry::typeFitsInline(tuplesize))
                        data = page->getInlinePtr(tuplesize);
                    else
                        data = page->getMaskedPtr();
                    if (data)
                    {
                        if (TYPE_MATCH)
                        {
                            if (isEqual(data, v.data(), tuplesize))
                                return;
                        }
                        else
                        {
                            if (isEqual(data, converted_v.data(), tuplesize))
                                return;
                        }
                    }
                    else
                    {
                        if (TYPE_MATCH)
                        {
                            if (isZero(v.data(), tuplesize))
                                return;
                        }
                        else
                        {
                            if (isZero(converted_v.data(), tuplesize))
                                return;
                        }
                    }
                }
                if (TSIZE >= 1)
                    hardenConstantPage(pages, page);
                else
                    hardenConstantPage(pages, page, tuplesize);
            }
            else if (page->isShared())
            {
                // If we're setting, check to see if it's a new value
                if (OP == 0)
                {
                    const NotVoidType *data = page->getFirstPtr()
                            + pageoff*((TSIZE >= 0) ? TSIZE : tuplesize);
                    if (TYPE_MATCH)
                    {
                        if (isEqual(data, v.data(), tuplesize))
                            return;
                    }
                    else
                    {
                        if (isEqual(data, converted_v.data(), tuplesize))
                            return;
                    }
                }
                if (TSIZE >= 1)
                    hardenSharedPage(pages, page);
                else
                    hardenSharedPage(pages, page, tuplesize);
            }
        }

        NotVoidType *data = page->getFirstPtr() + pageoff*((TSIZE >= 0) ? TSIZE : tuplesize);
        for (exint component = 0; component < mintuplesize; ++component)
        {
            if (OP == 0)
            {
                if (TYPE_MATCH)
                    data[component] = v[component];
                else
                    data[component] = converted_v[component];
            }
            else if (OP == 1)
            {
                if (TYPE_MATCH)
                    data[component] += v[component];
                else
                    data[component] += converted_v[component];
            }
            else
            {
                UT_ASSERT_MSG_P(0,"Only OP 0 (set) and 1 (add) are defined!");
            }
        }
    }

    SYS_FORCE_INLINE IDX_T size() const
    {
        const PageTable *pages = myImpl.getPages();
        if (!pages)
            return IDX_T(0);
        return pages->size();
    }

    SYS_FORCE_INLINE IDX_T capacity() const
    {
        const PageTable *pages = myImpl.getPages();
        if (!pages)
            return IDX_T(0);
        return pages->capacity();
    }

    /// This helper function forces a (non-negative) capacity to be an
    /// acceptable value: either
    /// A) a power of two less than a full page in size, or
    /// B) a multiple of the page size.
    SYS_FORCE_INLINE
    static IDX_T roundUpCapacity(IDX_T capacity)
    {
        UT_ASSERT_P(capacity >= IDX_T(0));

        // Round up to page size if more than half of one page.
        if (capacity > IDX_T(thePageSize>>1))
            return IDX_T((capacity + thePageMask) & ~thePageMask);

        // Round up to power of two if half a page or less.
        // (Zero stays zero.)
        if (capacity <= IDX_T(2))
            return capacity;
        return IDX_T(SYSmakePow2(exint(capacity)));
    }

    void setCapacity(IDX_T newcapacity)
    {
        UT_ASSERT_P(newcapacity >= IDX_T(0));
        newcapacity = roundUpCapacity(newcapacity);
        if (newcapacity == capacity())
            return;
        if (newcapacity < size())
        {
            setSize(newcapacity);
        }
        forceHardenTable(newcapacity);
    }

    void setCapacityIfNeeded(IDX_T newcapacity)
    {
        UT_ASSERT_P(newcapacity >= IDX_T(0));
        if (newcapacity <= capacity())
            return;
        newcapacity = roundUpCapacity(newcapacity);
        forceHardenTable(newcapacity);
    }

    /// Sets the size of the array without initializing any added
    /// elements.
    void setSize(IDX_T newsize)
    {
        UT_ASSERT_P(newsize >= IDX_T(0));

        setCapacityIfNeeded(newsize);
        hardenTable();
        PageTable *pages = myImpl.getPages();
        UT_ASSERT_P(pages || newsize == IDX_T(0));
        if (pages)
            pages->setSize(newsize);
    }

    /// Sets the size of the array and initializes any new elements to initval.
    /// This version of setSize only works if DATA_T is a numeric type.
    /// If the tuple size is > 1, it will write initval to all components.
    /// Include UT_PageArrayImpl.h to call this.
    void setSize(IDX_T newsize, NotVoidType initval);

    /// Sets the size of the array and initializes any new elements
    /// to initval.
    /// This version of setSize only works if DATA_T is a numeric type
    /// and TSIZE >= 1.
    /// Include UT_PageArrayImpl.h to call this.
    void setSize(IDX_T newsize, const UT_FixedVector<NotVoidType,theSafeTupleSize> &initval);

    /// Sets the size of the array and initializes any new elements to initval.
    /// Include UT_PageArrayImpl.h to call this.
    void setSize(IDX_T newsize, const UT_Defaults &initval);

    /// Set all of the values in the range [start,end) to v.
    /// This will constant-compress pages if allowed and the pages are
    /// completely overwritten with the given value.
    /// This version of setConstant only works if DATA_T is a numeric type.
    /// If the tuple size is > 1, it will write v to all components.
    void setConstant(IDX_T start, IDX_T end, NotVoidType v);

    /// Set all of the values in the range [start,end) to v.
    /// This will constant-compress pages if allowed and the pages are
    /// completely overwritten with the given value.
    /// This version of setConstant only works if DATA_T is a numeric type
    /// and TSIZE >= 1.
    void setConstant(IDX_T start, IDX_T end, const UT_FixedVector<NotVoidType,theSafeTupleSize> &v);

    /// Set all of the values in the range [start,end) to v.
    /// This will constant-compress pages if allowed and the pages are
    /// completely overwritten with the given value.
    void setConstant(IDX_T start, IDX_T end, const UT_Defaults &v);

    /// Changes the storage type and converts the data to the new type.
    /// NOTE: This only makes sense if DATA_T is void and newstorage is
    ///       something other than INVALID.
    /// Include UT_PageArrayImpl.h to call this.
    void setStorage(const UT_Storage newstorage);

    /// Changes the tuple size, and if the new size is larger, pads with
    /// values from the UT_Defaults.
    /// NOTE: This only makes sense if TSIZE == -1.
    /// Include UT_PageArrayImpl.h to call this.
    void setTupleSize(exint newtuplesize, const UT_Defaults &v);

    /// Get the total of all memory referred-to by this.
    /// NOTE: This does not handle shared memory correctly.
    /// Include UT_PageArrayImpl.h to call this.
    int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    /// Include UT_PageArrayImpl.h to call this.
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Copies the nelements from srcstart to deststart
    /// The two ranges *are* allowed to overlap.
    /// Include UT_PageArrayImpl.h to call this.
    void moveRange(IDX_T srcstart, IDX_T deststart, IDX_T nelements);

    /// Copies the nelements from fromstart in src to tostart in this.
    /// The two ranges *are* allowed to overlap, even if &src==this.
    /// Include UT_PageArrayImpl.h to call this.
    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_TABLEHARDENED,bool SRC_PAGESHARDENED>
    void moveRange(const UT_PageArray<SRC_DATA_T,SRC_TSIZE,SRC_TABLEHARDENED,SRC_PAGESHARDENED,THEPAGEBITS,IDX_T> &src, IDX_T srcstart, IDX_T deststart, IDX_T nelements);

    /// Swaps the nelements at astart with the nelements at bstart.
    /// The two ranges *must* be non-overlapping, else swapping is ill-defined.
    /// Include UT_PageArrayImpl.h to call this.
    void swapRange(IDX_T astart, IDX_T bstart, IDX_T nelements);

    /// Copies a single element from src to dest within this
    void copy(IDX_T dest, IDX_T src)
    {
        UT_ASSERT_P(dest >= IDX_T(0) && dest < size());
        UT_ASSERT_P(src >= IDX_T(0) && src < size());

        hardenTable();

        // Because no conversions are necessary, only the element size
        // in bytes is needed.
        const exint bytesize = (SYSisSame<DATA_T,void>() ? UTstorageSize(getStorage()) : sizeof(NotVoidType))
                       * getTupleSize();

        UT_PageNum destpagenum = pageNum(dest);
        UT_PageNum srcpagenum = pageNum(src);

        PageTable *pages = myImpl.getPages();
        PageTableEntry *destpage = pages->getPPage(destpagenum);
        const PageTableEntry *srcpage = pages->getPPage(srcpagenum);

        const int8 *srcdata;
        if (srcpage->isConstant())
        {
            if (bytesize < sizeof(PageTableEntry))
            {
                // Copying within equal constant pages does nothing
                if (*srcpage == *destpage)
                    return;

                srcdata = ((const int8 *)(srcpage+1)) - bytesize;
            }
            else if (srcpage->getUnmaskedPtrVoid() == destpage->getUnmaskedPtrVoid())
            {
                // Copying within equal constant pages does nothing
                return;
            }
            else
            {
                // NOTE: srcdata may be set to nullptr here.
                srcdata = (const int8 *)srcpage->getMaskedPtrVoid();
            }
        }
        else
        {
            srcdata = ((const int8*)srcpage->getFirstPtrVoid()) + pageOff(src)*bytesize;
        }

        int8 *destdata;
        if (destpage->isConstant())
        {
            if (bytesize < sizeof(PageTableEntry))
                destdata = ((int8 *)(destpage+1)) - bytesize;
            else
            {
                void *destmasked = destpage->getMaskedPtrVoid();
                if (destmasked == nullptr)
                {
                    if (isZero(srcdata, bytesize))
                        return;
                    destdata = nullptr;
                }
                else
                    destdata = (int8*)destmasked;
            }
            if (destdata != nullptr && isEqual(srcdata, destdata, bytesize))
                return;

            if (!SYSisSame<DATA_T,void>())
                hardenConstantPage(pages, destpage, getTupleSize());
            else
            {
                switch (getStorage())
                {
                    case UT_Storage::INT8:
                    {
                        auto &a = castType<int8>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenConstantPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT16:
                    case UT_Storage::REAL16:
                    {
                        auto &a = castType<int16>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenConstantPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT32:
                    case UT_Storage::REAL32:
                    {
                        auto &a = castType<int32>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenConstantPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT64:
                    case UT_Storage::REAL64:
                    {
                        auto &a = castType<int64>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenConstantPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INVALID:
                        UT_ASSERT_MSG(0, "Storage must be valid if DATA_T is void.");
                        break;
                }
            }
        }
        else if (!PAGESHARDENED && destpage->isShared())
        {
            destdata = ((int8*)destpage->getFirstPtrVoid()) + pageOff(dest)*bytesize;
            if (srcdata)
            {
                if (isEqual(srcdata, destdata, bytesize))
                    return;
            }
            else
            {
                if (isZero(destdata, bytesize))
                    return;
            }

            if (!SYSisSame<DATA_T,void>())
                hardenSharedPage(pages, destpage, getTupleSize());
            else
            {
                switch (getStorage())
                {
                    case UT_Storage::INT8:
                    {
                        auto &a = castType<int8>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenSharedPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT16:
                    case UT_Storage::REAL16:
                    {
                        auto &a = castType<int16>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenSharedPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT32:
                    case UT_Storage::REAL32:
                    {
                        auto &a = castType<int32>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenSharedPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INT64:
                    case UT_Storage::REAL64:
                    {
                        auto &a = castType<int64>();
                        auto a_pages = a.myImpl.getPages();
                        auto a_destpage = a_pages->getPPage(destpagenum);
                        a.hardenSharedPage(a_pages, a_destpage, getTupleSize());
                        break;
                    }
                    case UT_Storage::INVALID:
                        UT_ASSERT_MSG(0, "Storage must be valid if DATA_T is void.");
                        break;
                }
            }
        }
        destdata = ((int8*)destpage->getFirstPtrVoid()) + pageOff(dest)*bytesize;

        for (const int8 *srcdataend = srcdata+bytesize; srcdata != srcdataend; ++srcdata, ++destdata)
            *destdata = *srcdata;
    }

    /// Copies nelements from srcstart in this to dest array
    /// Include UT_PageArrayImpl.h to call this.
    template<typename T>
    SYS_FORCE_INLINE void getRange(IDX_T srcstart, IDX_T nelements, T *dest) const
    {
        // Always cast to associated UT_FixedVector type, so that
        // we can just delegate to getVectorRange that always uses UT_FixedVector
        getVectorRange(srcstart, nelements, (typename UT_FixedVectorTraits<T>::FixedVectorType*)dest);
    }

    /// Copies nelements from src array to deststart in this
    /// Include UT_PageArrayImpl.h to call this.
    template<typename T>
    SYS_FORCE_INLINE void setRange(IDX_T deststart, IDX_T nelements, const T *src)
    {
        // Always cast to associated UT_FixedVector type, so that
        // we can just delegate to getVectorRange that always uses UT_FixedVector
        setVectorRange(deststart, nelements, (const typename UT_FixedVectorTraits<T>::FixedVectorType*)src);
    }

    /// Copies nelements from srcstart in this to dest array
    /// Include UT_PageArrayImpl.h to call this.
    template<typename DEST_DATA_T,exint DEST_TSIZE,bool DEST_INSTANTIATED>
    void getVectorRange(IDX_T srcstart, IDX_T nelements, UT_FixedVector<DEST_DATA_T,DEST_TSIZE,DEST_INSTANTIATED> *dest) const;

    /// Copies nelements from src array to deststart in this
    /// Include UT_PageArrayImpl.h to call this.
    template<typename SRC_DATA_T,exint SRC_TSIZE,bool SRC_INSTANTIATED>
    void setVectorRange(IDX_T deststart, IDX_T nelements, const UT_FixedVector<SRC_DATA_T,SRC_TSIZE,SRC_INSTANTIATED> *src);

    SYS_FORCE_INLINE static UT_PageNum pageNum(IDX_T i)
    {
        return UT_PageNum(exint(i) >> thePageBits);
    }
    SYS_FORCE_INLINE static UT_PageNum numPages(IDX_T nelements)
    {
        return UT_PageNum((exint(nelements)+thePageMask) >> thePageBits);
    }
    SYS_FORCE_INLINE static UT_PageOff pageOff(IDX_T i)
    {
        return UT_PageOff(exint(i) & thePageMask);
    }
    SYS_FORCE_INLINE static UT_PageOff pageStart(IDX_T page)
    {
	return IDX_T(exint(page) << thePageBits);
    }

    /// Returns true iff the table isn't shared with any other UT_PageArray's.
    SYS_FORCE_INLINE bool isTableHardened() const
    {
        // If TABLEHARDENED, assert that the page table hasn't become shared
        // after the fact!  Nobody should share the page table after templating
        // on having hardened!
        UT_ASSERT_P(!TABLEHARDENED || !myImpl.getPages() || !myImpl.getPages()->isShared());

        return TABLEHARDENED || !myImpl.getPages() || !myImpl.getPages()->isShared();
    }

    /// This ensures that the table itself is not shared.
    /// It's a good idea to do this before writing, to avoid having to check
    /// every time.  Remember to use the returned reference, since it
    /// records in the template that it doesn't need to be hardened again.
    /// It should be a no-op if TABLEHARDENED is already true.
    SYS_FORCE_INLINE UT_PageArray<DATA_T,TSIZE,true,PAGESHARDENED,THEPAGEBITS,IDX_T> &hardenTable()
    {
        if (!isTableHardened())
        {
            // Keep same capacity
            forceHardenTable(myImpl.getPages()->capacity());
        }

        return *(UT_PageArray<DATA_T,TSIZE,true,PAGESHARDENED,THEPAGEBITS,IDX_T> *)this;
    }

    /// This forces all pages to be hardened, optionally including pages
    /// that are in the capacity but outside the size.
    template <bool including_capacity=false>
    SYS_FORCE_INLINE UT_PageArray<DATA_T,TSIZE,true,true,THEPAGEBITS,IDX_T> &hardenAllPages(IDX_T start = IDX_T(0), IDX_T end = IDX_T(-1))
    {
        // Nothing to do if PAGESHARDENED
        PageTable *pages = myImpl.getPages();
        if (PAGESHARDENED || (pages == nullptr))
            return *(UT_PageArray<DATA_T,TSIZE,true,true,THEPAGEBITS,IDX_T> *)this;

        auto &hard = hardenTable();

        if (end < IDX_T(0))
            end = including_capacity ? pages->capacity() : pages->size();

        UT_ASSERT_P(start >= IDX_T(0));
        UT_ASSERT_P(end <= pages->capacity());

        if (!SYSisSame<DATA_T,void>())
        {
            if (TSIZE >= 1)
                pages->hardenAllPages(start, end);
            else
                pages->hardenAllPages(start, end, myImpl.getTupleSize());
        }
        else
        {
            // Don't want to have to re-check that the table is hardened,
            // so call hardenAllPagesUnknownType on hard.
            hard.hardenAllPagesUnknownType(start, end);
        }

        return *(UT_PageArray<DATA_T,TSIZE,true,true,THEPAGEBITS,IDX_T> *)this;
    }

    template <bool including_capacity=false>
    SYS_FORCE_INLINE void tryCompressAllPages(IDX_T start = IDX_T(0), IDX_T end = IDX_T(-1))
    {
        // Can't have compressed pages if PAGESHARDENED
        PageTable *pages = myImpl.getPages();
        if (PAGESHARDENED || (pages == nullptr))
            return;

        auto &hard = hardenTable();

        if (end < IDX_T(0))
            end = including_capacity ? pages->capacity() : pages->size();

        UT_ASSERT_P(start >= IDX_T(0));
        UT_ASSERT_P(end <= pages->capacity());

        if (!SYSisSame<DATA_T,void>())
        {
            if (TSIZE >= 1)
                pages->tryCompressAllPages(start, end);
            else
                pages->tryCompressAllPages(start, end, myImpl.getTupleSize());
        }
        else
        {
            // Don't want to have to re-check that the table is hardened,
            // so call hardenAllPagesUnknownType on hard.
            hard.tryCompressAllPagesUnknownType(start, end);
        }
    }

    /// This is for setting the DATA_T template after checking getStorage(),
    /// if DATA_T wasn't already known.
    /// @{
    template<typename DEST_DATA_T>
    SYS_FORCE_INLINE const UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> &
    castType() const
    {
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()) || (getStorage() != UT_Storage::INVALID && getStorage() == UT_StorageNum<DEST_DATA_T>::theStorage));
        return *(const UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> *)this;
    }
    template<typename DEST_DATA_T>
    SYS_FORCE_INLINE UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> &
    castType()
    {
        UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()) || (getStorage() != UT_Storage::INVALID && getStorage() == UT_StorageNum<DEST_DATA_T>::theStorage));
        return *(UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> *)this;
    }
    /// @}

    /// This is for setting the TSIZE template after checking getTupleSize(),
    /// if TSIZE wasn't already known.
    /// @{
    template<exint DEST_TSIZE>
    SYS_FORCE_INLINE const UT_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> &
    castTupleSize() const
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == DEST_TSIZE);
        UT_ASSERT_P(getTupleSize() == DEST_TSIZE);
        return *(const UT_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> *)this;
    }
    template<exint DEST_TSIZE>
    SYS_FORCE_INLINE UT_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> &
    castTupleSize()
    {
        UT_ASSERT_P(TSIZE == -1 || TSIZE == DEST_TSIZE);
        UT_ASSERT_P(getTupleSize() == DEST_TSIZE);
        return *(UT_PageArray<DATA_T,DEST_TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> *)this;
    }
    /// @}

    SYS_FORCE_INLINE exint getTupleSize() const
    {
        return myImpl.getTupleSize();
    }
    SYS_FORCE_INLINE UT_Storage getStorage() const
    {
        return myImpl.getStorage();
    }

    /// Returns true iff the specified page is constant-compressed
    SYS_FORCE_INLINE bool isPageConstant(UT_PageNum pagenum) const
    {
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());
        return myImpl.getPages()->getPPage(pagenum)->isConstant();
    }

    /// Returns true iff the entire array is constant & zero
    SYS_FORCE_INLINE bool isArrayZero() const
    {
	// Zero capacity array is zero:
	if (myImpl.getPages() == nullptr)
	    return true;

	const PageTable *pages = myImpl.getPages();

	UT_PageNum	npages = numPages(pages->capacity());
	for (UT_PageNum pagenum = 0; pagenum < npages; pagenum++)
	{
	    const PageTableEntry *page = pages->getPPage(pagenum);
	    if (!page->isConstantAndZeroSafe())
		return false;
	}
	return true;
    }

    /// Sets all components of all elements of the specified page to
    /// the given value.
    SYS_FORCE_INLINE void setPageConstant(UT_PageNum pagenum, const NotVoidType &val)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        hardenTable();

        PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        if (TSIZE >= 1)
            makeConstant(page, val);
        else
            makeConstant(page, val, myImpl.getTupleSize());
    }

    /// Sets all components of all elements of the specified page to
    /// the given value.
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void setPageConstant(UT_PageNum pagenum, const UT_FixedVector<SRC_DATA_T,theSafeTupleSize> &val)
    {
        if (SYSisSame<DATA_T,void>())
        {
            SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,pagenum,&val](SYS_CALLIF_AUTO){
                setPageConstantUnknownType(pagenum, val);
            });
            return;
        }

        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

        if (!SYSisSame<DATA_T,SRC_DATA_T>())
        {
            SYS_CallIf<!SYSisSame<DATA_T,SRC_DATA_T>()>::call([this,pagenum,&val](SYS_CALLIF_AUTO){
                UT_FixedVector<NotVoidType,theSafeTupleSize> new_val;
                for (exint i = 0; i < theSafeTupleSize; ++i)
                    new_val[i] = UTconvertStorage<NotVoidType>(val[i]);
                this->setPageConstant(pagenum, new_val);
            });
            return;
        }

        UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()));
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());
        UT_ASSERT_P(TSIZE >= 1);

        hardenTable();

        PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        makeConstant(page, val);
    }

    /// Sets all components of all elements of the specified page to
    /// the given values.
    /// NOTE: The length of values must be equal to the tuple size.
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void setPageConstant(UT_PageNum pagenum, const SRC_DATA_T *values)
    {
        if (SYSisSame<DATA_T,void>())
        {
            SYS_CallIf<SYSisSame<DATA_T,void>()>::call([this,pagenum,values](SYS_CALLIF_AUTO){
                this->setPageConstantUnknownType(pagenum, values);
            });
            return;
        }

        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

        if (!SYSisSame<DATA_T,SRC_DATA_T>())
        {
            SYS_CallIf<!SYSisSame<DATA_T,SRC_DATA_T>()>::call([this,pagenum,values](SYS_CALLIF_AUTO){
                this->setPageConstantMismatchedType(pagenum, values);
            });
            return;
        }

        UT_ASSERT_P((SYSisSame<DATA_T,SRC_DATA_T>()));
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        hardenTable();

        PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        // NOTE: The cast will never actually be hit unless DATA_T is SRC_DATA_T,
        //       as asserted above.  It's just to get this to compile in other cases.
        makeConstant(page, (const NotVoidType *)values, myImpl.getTupleSize());
    }

    /// Returns true iff the specified page is "hard",
    /// i.e. NOT constant-compressed and NOT shared.
    /// NOTE: The *table* may or may not be hard.
    SYS_FORCE_INLINE bool isPageHard(UT_PageNum pagenum) const
    {
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());
        if (PAGESHARDENED)
            return true;
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        return !page->isConstant() && !page->isShared();
    }

    /// Hardens the specified page if it is constant-compressed or shared.
    /// It returns a pointer to the data, but it will only be the correct
    /// type if DATA_T is not void.
    SYS_FORCE_INLINE NotVoidType *hardenPage(UT_PageNum pagenum)
    {
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        hardenTable();

        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        if (PAGESHARDENED)
            return page->getFirstPtr();

        if (page->isConstant())
            hardenConstantPage(pages, page, myImpl.getTupleSize());
        else if (page->isShared())
            hardenSharedPage(pages, page, myImpl.getTupleSize());
        return page->getFirstPtr();
    }

    SYS_FORCE_INLINE NotVoidType *hardenPageNoInit(UT_PageNum pagenum)
    {
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        hardenTable();

        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        if (PAGESHARDENED)
            return page->getFirstPtr();

        if (page->isConstant())
            hardenConstantPageNoInit(pages, page, myImpl.getTupleSize());
        else if (page->isShared())
            hardenSharedPageNoInit(pages, page, myImpl.getTupleSize());
        return page->getFirstPtr();
    }

    /// If the specified page is constant and shared, (only relevant for
    /// nonzero tuples that are at least the size of a pointer), this
    /// keeps the page constant, but unshares it, so that the constant value
    /// can be modified.
    /// NOTE: This should ONLY be called on constant pages!  Check
    ///       isPageConstant(pagenum).
    SYS_FORCE_INLINE NotVoidType *unshareConstantPage(UT_PageNum pagenum)
    {
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        hardenTable();

        PageTable *pages = myImpl.getPages();
        PageTableEntry *page = pages->getPPage(pagenum);

        UT_ASSERT_P(!PAGESHARDENED && page->isConstant());

        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == -1 || TSIZE >= 0);

        const exint tuplesize = (TSIZE >= 0) ? TSIZE : myImpl.getTupleSize();
        if (PageTableEntry::typeFitsInline(tuplesize))
            return page->getInlinePtr(tuplesize);

        NotVoidType *src = page->getMaskedPtr();
        if (!page->isShared())
            return src;

        PageTableEntry newpage;
        newpage.alloc(UT_PageOff(1), tuplesize);
        NotVoidType *dest = newpage.getFirstPtr();
        memcpy(dest, src, tuplesize*sizeof(NotVoidType));
        newpage.setConstantBit();
        page->decRef();
        (*page) = newpage;

        return dest;
    }

    /// Checks if the specified page is all a single value,
    /// and if so, makes it a constant page.
    SYS_FORCE_INLINE void tryCompressPage(UT_PageNum pagenum)
    {
        // Can't have compressed pages if PAGESHARDENED
        PageTable *pages = myImpl.getPages();
        if (PAGESHARDENED || (pages == nullptr))
            return;

        auto &hard = hardenTable();

        UT_ASSERT_P(pagenum >= UT_PageNum(0));
        UT_ASSERT_P(pagenum < numPages(pages->capacity()));

        if (!SYSisSame<DATA_T,void>())
        {
            if (TSIZE >= 1)
                pages->tryCompressPage(pagenum);
            else
                pages->tryCompressPage(pagenum, myImpl.getTupleSize());
        }
        else
        {
            // Don't want to have to re-check that the table is hardened,
            // so call hardenAllPagesUnknownType on hard.
            hard.tryCompressPageUnknownType(pagenum);
        }
    }

    /// Returns a pointer to the data of a page.
    /// WARNING: DO NOT call this if DATA_T is void!  The pointer returned
    ///          can depend on the type.
    /// This will return nullptr if the page is constant compressed to zero.
    /// If it's non-nullptr and isPageConstant(pagenum) returns true, there
    /// will only be one element (tuple) at that address.
    const NotVoidType *getPageData(UT_PageNum pagenum) const
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        if (!page->isConstant())
            return page->getFirstPtr();
        if (TSIZE >= 1)
            return getConstantPtr(page);
        return getConstantPtr(page, 0, getTupleSize());
    }

    /// Returns a pointer to the data of a page.
    /// WARNING: DO NOT call this if DATA_T is void!  The pointer returned
    ///          can depend on the type.
    /// NOTE: DO NOT MODIFY THE DATA if there's any chance the page or the table
    ///       might be shared.  Check isTableHard() and isPageHard(pagenum).
    /// This will return nullptr if the page is constant compressed to zero.
    /// If it's non-nullptr and isPageConstant(pagenum) returns true, there
    /// will only be one element (tuple) at that address.
    NotVoidType *getPageData(UT_PageNum pagenum)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());
        PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        if (!page->isConstant())
            return page->getFirstPtr();
        if (TSIZE >= 1)
            return getConstantPtr(page);
        return getConstantPtr(page, 0, getTupleSize());
    }

    /// NOTE: Don't use this unless you know what you're doing!
    /// This is for the case where a non-POD type that requires destruction
    /// is masquerading as a POD type in here, e.g. GA_OffsetList as
    /// UT_FixedVector<int64,2>.  When destroying a table, this first needs
    /// to be called.  If it returns true, the table was shared,
    /// so it was just decRef'd, meaning that the data don't need to be
    /// destructed.  If it returns false, the table was not shared,
    /// so the data need to be destructed before deleting the table.
    bool decRefIffSharedTable()
    {
        if (TABLEHARDENED)
            return false;

        PageTable *table = myImpl.getPages();
        if (!table)
            return false;

        bool decremented = table->decRefIffShared();
        if (decremented)
            myImpl.getPages() = nullptr;
        return decremented;
    }

    /// NOTE: Don't use this unless you know what you're doing!
    /// This is for the case where a non-POD type that requires destruction
    /// is masquerading as a POD type in here, e.g. GA_OffsetList as
    /// UT_FixedVector<int64,2>.  When destroying a page, this first needs
    /// to be called.  If it returns true, the page was shared,
    /// so it was just decRef'd, meaning that the data don't need to be
    /// destructed.  If it returns false, the page was not shared,
    /// so the data need to be destructed before deleting the page.
    bool decRefIffSharedPage(UT_PageNum pagenum)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE >= 1);
        UT_ASSERT_P(myImpl.getPages() != nullptr);
        UT_ASSERT_P((exint)pagenum >= 0 && IDX_T((exint)pagenum << thePageBits) < size());

        if (PAGESHARDENED)
            return false;

        hardenTable();

        PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);
        if (page->isConstant())
        {
            if (sizeof(NotVoidType)*TSIZE < sizeof(PageTableEntry))
                return false;
        }
        bool decremented = page->decRefIffShared();
        if (decremented)
            page->initZero();
        return decremented;
    }

    /// Only call this if DATA_T is void and you are absolutely, 100% CERTAIN
    /// that the storage type is int16, int32, or int64.
    /// NOTE: Really!  Be careful!
    SYS_FORCE_INLINE
    int64 getGuaranteedInt(IDX_T i) const
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == 1);
        UT_ASSERT_P(getStorage() == UT_Storage::INT16 || getStorage() == UT_Storage::INT32 || getStorage() == UT_Storage::INT64);

        UT_PageNum pagenum = pageNum(i);
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);

        // If we know that the pages are hardened, we can omit
        // the constant page case at compile time.
        bool isconst = !PAGESHARDENED && page->isConstant();

        const UT_Storage storage = getStorage();
        if (!isconst)
        {
            UT_PageOff pageoff = pageOff(i);
            const void *first = page->getFirstPtrVoid();
            switch (storage)
            {
                default:
                    UT_ASSERT_MSG_P(0, "getGuaranteedInt() only supports int16, int32, and int64.");
                    SYS_FALLTHROUGH;
                case UT_Storage::INT16:
                    return (int64)((int16*)first)[pageoff];
                case UT_Storage::INT32:
                    return (int64)((int32*)first)[pageoff];
                case UT_Storage::INT64:
                    return ((int64*)first)[pageoff];
            }
        }

        // Special case for zero.
        if (!page->isConstantZero())
            return int64(0);

        // Special case for if the constant value is too big to fit in a PageTableEntry.
        // At the moment, PageTableEntry is 16 bytes on 64-bit platforms,
        // and 8 bytes on 32-bit platforms, so this case will not occur.
        if ((sizeof(PageTableEntry) < 9 && storage == UT_Storage::INT64) ||
            (sizeof(PageTableEntry) < 5 && storage == UT_Storage::INT32))
        {
            const void *v = page->getMaskedPtrVoid();
            if (sizeof(PageTableEntry) < 5 && storage == UT_Storage::INT32)
            {
                // This will not be run on 64-bit platforms
                return (int64)*(const int32 *)v;
            }
            return *(const int64 *)v;
        }
        else
        {
            // Any types that are strictly smaller than a pointer will be stored inline.
            // This is checked at compile time, so there'll be no op at runtime.
            if (sizeof(PageTableEntry) >= 9 && storage == UT_Storage::INT64)
            {
                return *page->template castType<int64>()->getInlinePtr(1);
            }
            if (sizeof(PageTableEntry) >= 5 && storage == UT_Storage::INT32)
            {
                return (int64)*page->template castType<int32>()->getInlinePtr(1);
            }
            return (int64)*page->template castType<int16>()->getInlinePtr(1);
        }
    }

    /// Returns true iff there is at least one NaN value in the given range.
    /// Include UT_PageArrayImpl.h to call this.
    bool hasNanInRange(IDX_T start, IDX_T end) const;

protected:
    template<typename DEST_DATA_T=DATA_T>
    DEST_DATA_T getUnknownType(IDX_T i,exint component=0) const
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE != 0);

        // Make sure common work is extracted out where possible,
        // to avoid duplication.

        UT_PageNum pagenum = pageNum(i);
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);

        // If we know that the pages are hardened, we can omit
        // the constant page case at compile time.
        bool isconst = !PAGESHARDENED && page->isConstant();

        const UT_Storage storage = getStorage();
        if (!isconst)
        {
            UT_PageOff pageoff = pageOff(i);
            const void *first = page->getFirstPtrVoid();
            exint i;
            if (TSIZE == 1)
                i = (exint)pageoff;
            else if (TSIZE > 1)
                i = TSIZE*pageoff + component;
            else
                i = myImpl.getTupleSize()*pageoff + component;
            switch (storage)
            {
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG_P(0, "Can't read a non-numeric type with a cast to a different type.");
                    return DEST_DATA_T();
                case UT_Storage::INT8:
                    return (DEST_DATA_T)((int8*)first)[i];
                case UT_Storage::INT16:
                    return (DEST_DATA_T)((int16*)first)[i];
                case UT_Storage::INT32:
                    return (DEST_DATA_T)((int32*)first)[i];
                case UT_Storage::INT64:
                    return (DEST_DATA_T)((int64*)first)[i];
                case UT_Storage::REAL16:
                    return (DEST_DATA_T)((fpreal16*)first)[i];
                case UT_Storage::REAL32:
                    return (DEST_DATA_T)((fpreal32*)first)[i];
                case UT_Storage::REAL64:
                    return (DEST_DATA_T)((fpreal64*)first)[i];
            }
            UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
            return DEST_DATA_T();
        }

        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't read a non-numeric type with a cast to a different type.");
                return DEST_DATA_T();
            case UT_Storage::INT8:
                return (DEST_DATA_T)castType<int8>().getConstant(page->template castType<int8>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::INT16:
                return (DEST_DATA_T)castType<int16>().getConstant(page->template castType<int16>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::INT32:
                return (DEST_DATA_T)castType<int32>().getConstant(page->template castType<int32>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::INT64:
                return (DEST_DATA_T)castType<int64>().getConstant(page->template castType<int64>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::REAL16:
                return (DEST_DATA_T)castType<fpreal16>().getConstant(page->template castType<fpreal16>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::REAL32:
                return (DEST_DATA_T)castType<fpreal32>().getConstant(page->template castType<fpreal32>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
            case UT_Storage::REAL64:
                return (DEST_DATA_T)castType<fpreal64>().getConstant(page->template castType<fpreal64>(), component, (TSIZE >= 1 ? TSIZE : myImpl.getTupleSize()));
        }
        return DEST_DATA_T();
    }

    template<int OP,typename SRC_DATA_T>
    void opUnknownType(IDX_T i,exint component,SRC_DATA_T v)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't write a non-numeric type with opUnknownType.");
                return;
            case UT_Storage::INT8:
                castType<int8    >().template op<OP>(i,component, UTconvertStorage<int8>(v)); return;
            case UT_Storage::INT16:
                castType<int16   >().template op<OP>(i,component, UTconvertStorage<int16>(v)); return;
            case UT_Storage::INT32:
                castType<int32   >().template op<OP>(i,component, UTconvertStorage<int32>(v)); return;
            case UT_Storage::INT64:
                castType<int64   >().template op<OP>(i,component, UTconvertStorage<int64>(v)); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().template op<OP>(i,component, (fpreal16)v); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().template op<OP>(i,component, (fpreal32)v); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().template op<OP>(i,component, (fpreal64)v); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }
    template<int OP,typename SRC_DATA_T,exint SRC_TSIZE>
    void opVectorUnknownType(IDX_T i,const UT_FixedVector<SRC_DATA_T,SRC_TSIZE> &v)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't write a non-numeric type with opVectorUnknownType.");
                return;
            case UT_Storage::INT8:
                castType<int8    >().template opVector<OP>(i,v); return;
            case UT_Storage::INT16:
                castType<int16   >().template opVector<OP>(i,v); return;
            case UT_Storage::INT32:
                castType<int32   >().template opVector<OP>(i,v); return;
            case UT_Storage::INT64:
                castType<int64   >().template opVector<OP>(i,v); return;
            case UT_Storage::REAL16:
                castType<fpreal16>().template opVector<OP>(i,v); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().template opVector<OP>(i,v); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().template opVector<OP>(i,v); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }
    template<typename DEST_DATA_T,exint DEST_TSIZE>
    UT_FixedVector<DEST_DATA_T,DEST_TSIZE> getVectorUnknownType(IDX_T i) const
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));

        const UT_Storage storage = getStorage();

        // First, check for match on destination type
        if (storage == UT_StorageNum<DEST_DATA_T>::theStorage)
            return castType<DEST_DATA_T>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);

        // Then, try second-guess type (e.g. reading double as float or vice versa)
        if (storage == UT_StorageNum<typename UT_StorageNum<DEST_DATA_T>::SecondGuess>::theStorage)
            return castType<typename UT_StorageNum<DEST_DATA_T>::SecondGuess>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);

        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't read a non-numeric type with getVectorUnknownType.");
                break;
            case UT_Storage::INT8:
                return castType<int8    >().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::INT16:
                return castType<int16   >().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::INT32:
                return castType<int32   >().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::INT64:
                return castType<int64   >().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::REAL16:
                return castType<fpreal16>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::REAL32:
                return castType<fpreal32>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
            case UT_Storage::REAL64:
                return castType<fpreal64>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
        return UT_FixedVector<DEST_DATA_T,DEST_TSIZE>(DEST_DATA_T(0));
    }
    template<typename DEST_DATA_T,exint DEST_TSIZE>
    UT_FixedVector<DEST_DATA_T,DEST_TSIZE> getVectorUnknownSize(IDX_T i) const
    {
        UT_ASSERT_P(TSIZE==-1);
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()));

        const exint tuplesize = getTupleSize();

        // First, check for match on destination type
        if (tuplesize == DEST_TSIZE)
            return castTupleSize<DEST_TSIZE>().template getVector<DEST_DATA_T,DEST_TSIZE>(i);

        // Then try small sizes
        if (tuplesize == 1)
            return castTupleSize<1>().template getVectorMismatchedSize<DEST_DATA_T,DEST_TSIZE>(i);
        if (tuplesize == 2)
            return castTupleSize<2>().template getVectorMismatchedSize<DEST_DATA_T,DEST_TSIZE>(i);
        if (tuplesize == 3)
            return castTupleSize<3>().template getVectorMismatchedSize<DEST_DATA_T,DEST_TSIZE>(i);
        if (tuplesize == 4)
            return castTupleSize<4>().template getVectorMismatchedSize<DEST_DATA_T,DEST_TSIZE>(i);

        // Mismatched large, unknown tuplesize (uncommon case)
        UT_FixedVector<DEST_DATA_T,DEST_TSIZE> v(DEST_DATA_T(0));
        const exint minsize = SYSmin(tuplesize,DEST_TSIZE);
        for (exint component = 0; component < minsize; ++component)
            v[component] = get<DEST_DATA_T>(i, component);

        return v;
    }
    template<typename DEST_DATA_T,exint DEST_TSIZE>
    UT_FixedVector<DEST_DATA_T,DEST_TSIZE> getVectorMismatchedSize(IDX_T i) const
    {
        UT_ASSERT_P(TSIZE!=-1);
        UT_ASSERT_P(TSIZE!=DEST_TSIZE);
        UT_ASSERT_P((SYSisSame<DEST_DATA_T,DATA_T>()));

        // Types and tuple sizes are known, but don't match
        UT_PageNum pagenum = pageNum(i);
        const PageTableEntry *page = myImpl.getPages()->getPPage(pagenum);

        // If we know that the pages are hardened, we can omit
        // the constant page case at compile time.
        bool isconst = !PAGESHARDENED && page->isConstant();

        const NotVoidType *srctuple;
        if (!isconst)
        {
            UT_PageOff pageoff = pageOff(i);
            srctuple = (const NotVoidType *)(((const UT_FixedVector<NotVoidType,theSafeTupleSize>*)page->getFirstPtr()) + pageoff);
        }
        else
        {
            srctuple = getConstantPtr(page);
            if (!srctuple)
                return UT_FixedVector<DEST_DATA_T,DEST_TSIZE>(DEST_DATA_T(0));
        }

        if (DEST_TSIZE < TSIZE)
            return *(const UT_FixedVector<DEST_DATA_T,DEST_TSIZE>*)srctuple;

        UT_FixedVector<DEST_DATA_T,DEST_TSIZE> dsttuple;
        for (exint component = 0; component < TSIZE; ++component)
            dsttuple[component] = srctuple[component];
        for (exint component = TSIZE; component < DEST_TSIZE; ++component)
            dsttuple[component] = DEST_DATA_T(0);
        return dsttuple;
    }
    void hardenAllPagesUnknownType(IDX_T start, IDX_T end)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                return;
            case UT_Storage::INT8:
                castType<int8>().template hardenAllPages<false>(start, end);     return;
            case UT_Storage::INT16:
                castType<int16>().template hardenAllPages<false>(start, end);    return;
            case UT_Storage::INT32:
                castType<int32>().template hardenAllPages<false>(start, end);    return;
            case UT_Storage::INT64:
                castType<int64>().template hardenAllPages<false>(start, end);    return;
            case UT_Storage::REAL16:
                castType<fpreal16>().template hardenAllPages<false>(start, end); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().template hardenAllPages<false>(start, end); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().template hardenAllPages<false>(start, end); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }

    void tryCompressAllPagesUnknownType(IDX_T start, IDX_T end)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                return;
            case UT_Storage::INT8:
                castType<int8>().template tryCompressAllPages<false>(start, end);     return;
            case UT_Storage::INT16:
                castType<int16>().template tryCompressAllPages<false>(start, end);    return;
            case UT_Storage::INT32:
                castType<int32>().template tryCompressAllPages<false>(start, end);    return;
            case UT_Storage::INT64:
                castType<int64>().template tryCompressAllPages<false>(start, end);    return;
            case UT_Storage::REAL16:
                castType<fpreal16>().template tryCompressAllPages<false>(start, end); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().template tryCompressAllPages<false>(start, end); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().template tryCompressAllPages<false>(start, end); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }

    void tryCompressPageUnknownType(UT_PageNum pagenum)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
        case UT_Storage::INVALID:
            UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
            return;
        case UT_Storage::INT8:
            castType<int8>().tryCompressPage(pagenum);     return;
        case UT_Storage::INT16:
            castType<int16>().tryCompressPage(pagenum);    return;
        case UT_Storage::INT32:
            castType<int32>().tryCompressPage(pagenum);    return;
        case UT_Storage::INT64:
            castType<int64>().tryCompressPage(pagenum);    return;
        case UT_Storage::REAL16:
            castType<fpreal16>().tryCompressPage(pagenum); return;
        case UT_Storage::REAL32:
            castType<fpreal32>().tryCompressPage(pagenum); return;
        case UT_Storage::REAL64:
            castType<fpreal64>().tryCompressPage(pagenum); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }

    template<typename SRC_DATA_T>
    void setPageConstantUnknownType(UT_PageNum pagenum, const UT_FixedVector<SRC_DATA_T,theSafeTupleSize> &val)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                return;
            case UT_Storage::INT8:
                castType<int8>().setPageConstant(pagenum, val);     return;
            case UT_Storage::INT16:
                castType<int16>().setPageConstant(pagenum, val);    return;
            case UT_Storage::INT32:
                castType<int32>().setPageConstant(pagenum, val);    return;
            case UT_Storage::INT64:
                castType<int64>().setPageConstant(pagenum, val);    return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setPageConstant(pagenum, val); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setPageConstant(pagenum, val); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setPageConstant(pagenum, val); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }

    template<typename SRC_DATA_T>
    void setPageConstantUnknownType(UT_PageNum pagenum, const SRC_DATA_T *values)
    {
        UT_ASSERT_P((SYSisSame<DATA_T,void>()));
        const UT_Storage storage = getStorage();
        switch (storage)
        {
            case UT_Storage::INVALID:
                UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                return;
            case UT_Storage::INT8:
                castType<int8>().setPageConstant(pagenum, values);     return;
            case UT_Storage::INT16:
                castType<int16>().setPageConstant(pagenum, values);    return;
            case UT_Storage::INT32:
                castType<int32>().setPageConstant(pagenum, values);    return;
            case UT_Storage::INT64:
                castType<int64>().setPageConstant(pagenum, values);    return;
            case UT_Storage::REAL16:
                castType<fpreal16>().setPageConstant(pagenum, values); return;
            case UT_Storage::REAL32:
                castType<fpreal32>().setPageConstant(pagenum, values); return;
            case UT_Storage::REAL64:
                castType<fpreal64>().setPageConstant(pagenum, values); return;
        }
        UT_ASSERT_MSG_P(0, "Unhandled UT_Storage enum value!");
    }

    template<typename SRC_DATA_T>
    void setPageConstantMismatchedType(UT_PageNum pagenum, const SRC_DATA_T *values)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(!(SYSisSame<DATA_T,SRC_DATA_T>()));

        // This could be done directly, instead of converting
        // to a UT_SmallArray<DATA_T>, but this approach is simpler for now.

        UT_SmallArray<NotVoidType, ((TSIZE >= 1) ? TSIZE : 16)*sizeof(NotVoidType)> dest_values;
        const exint tuplesize = myImpl.getTupleSize();
        dest_values.setSize(tuplesize);

        for (exint i = 0; i < tuplesize; ++i)
            dest_values(i) = UTconvertStorage<NotVoidType>(values[i]);

        setPageConstant(pagenum, dest_values.array());
    }

    class PageTableEntry
    {
    public:
        typedef PageTableEntry ThisType;
        typedef UT_FixedVector<NotVoidType,theSafeTupleSize> Tuple;
        static const uintptr_t theConstantPageBit = 1;

        /// This is always valid to call.
        SYS_FORCE_INLINE bool isConstant() const
        {
            return (uintptr_t(myData) & theConstantPageBit) != 0;
        }
        /// This is only valid to call if the type doesn't fit inline.
        SYS_FORCE_INLINE bool isConstantZero() const
        {
            return (uintptr_t(myData) == theConstantPageBit);
        }
        /// This is always valid to call, and will return true
        /// iff this is either inline and all zero or
        /// using the constant zero optimization.
        SYS_FORCE_INLINE bool isConstantAndZeroSafe() const
        {
            return (uintptr_t(myData) == theConstantPageBit)
                && (myPRefCount == nullptr);
        }
        /// This is only valid to call when DATA_T is non-void.
        SYS_FORCE_INLINE static bool typeFitsInline(exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
            if (sizeof(PageTableEntry) <= sizeof(NotVoidType))
                return false;
            return (sizeof(NotVoidType)*((TSIZE >= 0) ? TSIZE : tuplesize) < sizeof(PageTableEntry));
        }

        /// This is always valid to call, but be aware that the parameter is the
        /// number of *bytes* in a tuple, not the number of components in a
        /// tuple.
        SYS_FORCE_INLINE bool isRefd(exint tuplebytes) const
        {
            if (!isConstant())
                return true;

            // This will be checked at compile time, when possible.
            if (sizeof(PageTableEntry) > ((TSIZE >= 0 && !SYSisSame<DATA_T,void>()) ? TSIZE*sizeof(NotVoidType) : tuplebytes))
                return false;

            return !isConstantZero();
        }

        /// NOTE: This should only be called when there's an actual allocated page,
        ///       i.e. !isConstant() || (!typeFitsInline(tuplesize) && !isConstantZero()).
        SYS_FORCE_INLINE bool isShared() const
        {
            UT_ASSERT_P(myPRefCount->relaxedLoad() > 0);
            return myPRefCount->relaxedLoad() != 1;
        }

        /// NOTE: This should only be called when there's an actual allocated page to decRef,
        ///       i.e. !isConstant() || (!typeFitsInline(tuplesize) && !isConstantZero()).
        SYS_FORCE_INLINE void incRef()
        {
            myPRefCount->add(1);
        }

        /// NOTE: This should only be called when there's an actual allocated page to decRef,
        ///       i.e. !isConstant() || (!typeFitsInline(tuplesize) && !isConstantZero()).
        SYS_FORCE_INLINE void decRef()
        {
            int64 new_count = myPRefCount->add(-1);
            UT_ASSERT_P(new_count >= 0);
            if (new_count == 0)
            {
                free((NotVoidType*)(uintptr_t(myData) & ~theConstantPageBit));
                free(myPRefCount);
#if 0
                printf("Free page        %d pages total\n", int(thePageTablePageCount.add(-1)));
                fflush(stdout);
#endif
            }
        }

        /// NOTE: This should only be called when there's an actual allocated page,
        ///       i.e. !isConstant() || (!typeFitsInline(tuplesize) && !isConstantZero()).
        SYS_FORCE_INLINE exint getRefCount() const
        {
            UT_ASSERT_P(myPRefCount->relaxedLoad() > 0);
            return myPRefCount->relaxedLoad();
        }

        SYS_FORCE_INLINE void alloc(UT_PageOff nelements, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
            myData = (NotVoidType *)malloc(nelements*((TSIZE >= 0) ? TSIZE : tuplesize)*sizeof(NotVoidType));
            myPRefCount = (SYS_AtomicCounter *)malloc(sizeof(SYS_AtomicCounter));
            myPRefCount->relaxedStore(1);
#if 0
            printf("Alloc page        %d pages total\n", int(thePageTablePageCount.add(1)));
            fflush(stdout);
#endif
        }
        void realloc(UT_PageOff sizetocopy, UT_PageOff newpagecapacity, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
            UT_ASSERT_P(sizetocopy >= 0 && sizetocopy <= newpagecapacity);
            UT_ASSERT_P(!isConstant());

            UT_ASSERT_P(tuplesize >= 0);
            // This is just to try to force the compiler to constant-value-optimize.
            if (TSIZE >= 0)
                tuplesize = TSIZE;

            UT_ASSERT_P(myPRefCount->relaxedLoad() > 0);
            if (myPRefCount->relaxedLoad() > 1)
            {
                // If shared, need to make a new page.
                PageTableEntry newpage;
                newpage.alloc(newpagecapacity, tuplesize);
                NotVoidType *__restrict newdata = newpage.myData;
                const NotVoidType *__restrict olddata = myData;
                // Yes, this loop is an inline memcpy, because memcpy won't inline,
                // but the compiler will optimize it if it's like this.
                sizetocopy *= tuplesize;
                while (sizetocopy)
                {
                    *newdata = *olddata;
                    ++newdata;
                    ++olddata;
                    --sizetocopy;
                }
                decRef();
                myData = newpage.myData;
                myPRefCount = newpage.myPRefCount;
            }
            else
            {
                // If not shared, we can potentially reuse the same memory.
                myData = (NotVoidType*)::realloc(myData, newpagecapacity*(tuplesize*sizeof(NotVoidType)));
                // NOTE: Reference count doesn't need to change, since it's 1.
            }
        }

        /// Atomically decrement the reference count iff the page is shared.
        /// See the description of decRefIffSharedPage() above for
        /// why this function exists.
        SYS_FORCE_INLINE
        bool decRefIffShared(exint tuplesize = TSIZE)
        {
            if (isConstant())
            {
                UT_ASSERT_P(tuplesize >= 0);
                if (typeFitsInline(tuplesize) || isConstantZero())
                    return false;
            }
            return decCounterIffAbove1(*myPRefCount);
        }

        /// Returns the data pointer, if not a constant page
        SYS_FORCE_INLINE
        NotVoidType *getFirstPtr()
        {
            UT_ASSERT_MSG_P(!isConstant(), "getFirstPtr() is for non-constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

            return myData;
        }

        /// Returns the data pointer, if not a constant page
        SYS_FORCE_INLINE
        const NotVoidType *getFirstPtr() const
        {
            UT_ASSERT_MSG_P(!isConstant(), "getFirstPtr() is for non-constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

            return myData;
        }

        /// Returns the data pointer, if not a constant page
        SYS_FORCE_INLINE
        void *getFirstPtrVoid()
        {
            UT_ASSERT_MSG_P(!isConstant(), "getFirstPtrVoid() is for non-constant pages");

            return myData;
        }

        /// Returns the data pointer, if not a constant page
        SYS_FORCE_INLINE
        const void *getFirstPtrVoid() const
        {
            UT_ASSERT_MSG_P(!isConstant(), "getFirstPtrVoid() is for non-constant pages");

            return myData;
        }

        /// Returns the data pointer, or whatever's in its place
        /// with bit 0 set to 1 if it's something constant.
        /// If it's constant, it might not be a flagged pointer.
        SYS_FORCE_INLINE
        void *getFirstPtrVoidUnsafe()
        {
            return myData;
        }

        /// Returns the data pointer, or whatever's in its place
        /// with bit 0 set to 1 if it's something constant.
        /// If it's constant, it might not be a flagged pointer.
        SYS_FORCE_INLINE
        const void *getFirstPtrVoidUnsafe() const
        {
            return myData;
        }

        /// Returns the data pointer, if an inline constant page
        SYS_FORCE_INLINE
        NotVoidType *getInlinePtr(exint tuplesize)
        {
            UT_ASSERT_MSG_P(isConstant() && typeFitsInline(tuplesize), "getInlinePtr() is for inline constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
            return (NotVoidType *)(((int8 *)(this+1)) - (sizeof(NotVoidType)*(TSIZE >= 0 ? TSIZE : tuplesize)));
        }

        /// Returns the data pointer, if an inline constant page
        SYS_FORCE_INLINE
        const NotVoidType *getInlinePtr(exint tuplesize) const
        {
            UT_ASSERT_MSG_P(isConstant() && typeFitsInline(tuplesize), "getInlinePtr() is for inline constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));
            return (const NotVoidType *)(((const int8 *)(this+1)) - (sizeof(NotVoidType)*(TSIZE >= 0 ? TSIZE : tuplesize)));
        }

        /// Returns the data pointer, if a non-inline constant page.
        /// Returns nullptr for a zero constant page.
        SYS_FORCE_INLINE
        NotVoidType *getMaskedPtr()
        {
            UT_ASSERT_MSG_P(isConstant(), "getMaskedPtr() is for constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

            return (NotVoidType *)(uintptr_t(myData) & ~theConstantPageBit);
        }

        /// Returns the data pointer, if a non-inline constant page.
        /// Returns nullptr for a zero constant page.
        SYS_FORCE_INLINE
        const NotVoidType *getMaskedPtr() const
        {
            UT_ASSERT_MSG_P(isConstant(), "getMaskedPtr() is for constant pages");
            UT_ASSERT_P(!(SYS_IsSame<DATA_T,void>::value));

            return (const NotVoidType *)(uintptr_t(myData) & ~theConstantPageBit);
        }

        /// Returns the data pointer, if a non-inline constant page.
        /// Returns nullptr for a zero constant page.
        SYS_FORCE_INLINE
        void *getMaskedPtrVoid()
        {
            UT_ASSERT_MSG_P(isConstant(), "getMaskedPtrVoid() is for constant pages");

            return (void *)(uintptr_t(myData) & ~theConstantPageBit);
        }

        /// Returns the data pointer, if a non-inline constant page.
        /// Returns nullptr for a zero constant page.
        SYS_FORCE_INLINE
        const void *getMaskedPtrVoid() const
        {
            UT_ASSERT_MSG_P(isConstant(), "getMaskedPtrVoid() is for constant pages");

            return (const void *)(uintptr_t(myData) & ~theConstantPageBit);
        }

        /// Initializes a page to constant zero, assuming that it hasn't been
        /// initialized yet.
        SYS_FORCE_INLINE
        void initZero()
        {
            myData = (NotVoidType*)theConstantPageBit;
            myPRefCount = nullptr;
        }

        SYS_FORCE_INLINE
        void setConstantBit()
        {
            myData = (NotVoidType *)(uintptr_t(myData) | theConstantPageBit);
        }

        void tryCompressPage(UT_PageOff pagesize, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
            UT_ASSERT_P(!isConstant());

            tuplesize = (TSIZE >= 0 ? TSIZE : tuplesize);

            const NotVoidType *const first = myData;
            const NotVoidType *const end = first + (TSIZE >= 0 ? TSIZE : tuplesize)*exint(pagesize);
            for (const NotVoidType *other = first+tuplesize; other < end; other += tuplesize)
            {
                for (exint component = 0; component < (TSIZE >= 0 ? TSIZE : tuplesize); ++component)
                {
                    if (other[component] != first[component])
                        return;
                }
            }
            // We have a constant page!  Compress it!

            PageTableEntry newpage;
            newpage.initZero();
            const exint bytesize = sizeof(NotVoidType)*(TSIZE >= 0 ? TSIZE : tuplesize);
            // Value stored inline if it's strictly smaller than a pointer
            // The first check can be done at compile time, so it's only logically redundant,
            // not functionally redundant.
            if (sizeof(PageTableEntry) > sizeof(NotVoidType) && sizeof(PageTableEntry) > bytesize)
            {
                memcpy(((int8*)&newpage) + (sizeof(PageTableEntry) - bytesize),
                    first, bytesize);
            }
            else
            {
                if (!isZero(first, (TSIZE >= 0 ? TSIZE : tuplesize)))
                {
                    newpage.alloc(UT_PageOff(1), tuplesize);
                    NotVoidType *newfirst = newpage.getFirstPtr();
                    memcpy(newfirst, first, bytesize);
                    newpage.setConstantBit();
                }
            }
            decRef();
            *this = newpage;
        }

        /// NOTE: This always excludes sizeof(*this), and is only valid for
        ///       non-small pages, (okay if constant).
        SYS_FORCE_INLINE int64 getMemoryUsage(exint tuplebytes) const
        {
            if (!isConstant())
                return sizeof(SYS_AtomicCounter) + tuplebytes*thePageSize;
            if (tuplebytes < sizeof(PageTableEntry) || isConstantZero())
                return 0;
            return sizeof(SYS_AtomicCounter) + tuplebytes;
        }

        template<typename DEST_DATA_T>
        SYS_FORCE_INLINE
        const typename UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTableEntry *
        castType() const
        {
            UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
            return (const typename UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTableEntry *)this;
        }
        template<typename DEST_DATA_T>
        SYS_FORCE_INLINE
        typename UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTableEntry *
        castType()
        {
            UT_ASSERT_P((SYS_IsSame<DATA_T,void>::value || SYS_IsSame<DATA_T,DEST_DATA_T>::value));
            return (typename UT_PageArray<DEST_DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T>::PageTableEntry *)this;
        }

        /// NOTE: This just does a shallow comparison.
        SYS_FORCE_INLINE
        bool operator==(const ThisType &that) const
        {
            return (myData == that.myData) && (myPRefCount == that.myPRefCount);
        }
        /// NOTE: This just does a shallow comparison.
        SYS_FORCE_INLINE
        bool operator!=(const ThisType &that) const
        {
            return (myData != that.myData) || (myPRefCount != that.myPRefCount);
        }

    private:
        NotVoidType *myData;
        SYS_AtomicCounter *myPRefCount;
    };

    SYS_FORCE_INLINE
    static bool decCounterIffAbove1(SYS_AtomicCounter &counter)
    {
        exint cur_count = counter.relaxedLoad();

        // It should be rare for this to ever loop, because looping
        // means that something else referring to the same page has
        // done an incRef, decRef, or decRefIffShared on this same page,
        // between the relaxedLoad and the compare_swap calls, or on
        // subsequent loops, between the compare_swap calls.
        while (true)
        {
            if (cur_count == 1)
                return false; // Not shared, so don't decrement
            exint new_count = cur_count-1;
            exint prev_count = counter.compare_swap(cur_count, new_count);
            if (prev_count == cur_count)
                return true; // Successfully decremented
            cur_count = prev_count;
        }
        // Execution never gets here, because the only escapes from
        // the loops return.
    }

    SYS_FORCE_INLINE static NotVoidType getConstant(const PageTableEntry *const page,exint component,exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(component >= 0 && component < tuplesize);
        UT_ASSERT_P(page->isConstant());

        // Any types whose tuples are strictly smaller than a pointer will be stored inline.
        // If a single one is at least as large as a pointer, it's
        // rejected at compile time, so there'll be no op at runtime in that case.
        if (PageTableEntry::typeFitsInline(tuplesize))
        {
            return page->getInlinePtr(tuplesize)[component];
        }

        const NotVoidType *masked = page->getMaskedPtr();
        if (!masked)
	{
	    NotVoidType zero;
	    memset(&zero, 0, sizeof(NotVoidType));
            return zero;
	}

        // Other types have a 1-element page.
        return masked[component];
    }
    /// NOTE: It may seem like overkill to take a PageTableEntry *, but the
    ///       returned pointer may be *inside* the pointer that the caller owns,
    ///       so we need the caller's pointer, not a copy.  getConstant doesn't
    ///       have this issue, since it can use the local pointer and read the
    ///       value.
    SYS_FORCE_INLINE static const NotVoidType *getConstantPtr(const PageTableEntry *page,exint component = 0,exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(component >= 0 && component < tuplesize);
        UT_ASSERT_P(page->isConstant());

        // Any types whose tuples are strictly smaller than PageTableEntry will be stored inline.
        if (PageTableEntry::typeFitsInline(tuplesize))
        {
            return page->getInlinePtr(tuplesize) + component;
        }

        const NotVoidType *masked = page->getMaskedPtr();
        if (!masked)
            return nullptr;

        // Other types have a 1-element page.
        return masked + component;
    }
    /// NOTE: It may seem like overkill to take a PageTableEntry *, but the
    ///       returned pointer may be *inside* the pointer that the caller owns,
    ///       so we need the caller's pointer, not a copy.  getConstant doesn't
    ///       have this issue, since it can use the local pointer and read the
    ///       value.
    SYS_FORCE_INLINE static NotVoidType *getConstantPtr(PageTableEntry *page,exint component = 0,exint tuplesize = TSIZE)
    {
        return SYSconst_cast(getConstantPtr((const PageTableEntry *)page, component, tuplesize));
    }
    SYS_FORCE_INLINE static void makeConstant(PageTableEntry *page, const UT_FixedVector<NotVoidType, theSafeTupleSize> &val)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE >= 1);
        UT_ASSERT_P(!PAGESHARDENED);

        if (PAGESHARDENED)
            return;

        typedef UT_FixedVector<NotVoidType, theSafeTupleSize> Tuple;

        // Unref existing non-constant page
        bool wasconst = page->isConstant();
        if (!wasconst)
            page->decRef();

        // Easy for inline case, checked at compile-time.
        if (sizeof(PageTableEntry) > sizeof(Tuple))
        {
            page->initZero();
            NotVoidType *tuple = page->getInlinePtr(theSafeTupleSize);
            *(Tuple *)tuple = val;
            return;
        }

        bool iszero = val.isZero();

        // In other cases, we may or may not have to unref constant page
        if (wasconst)
        {
            NotVoidType *masked = page->getMaskedPtr();
            if (!masked)
            {
                // Fairly common case: already zero, making zero.
                if (iszero)
                    return;
            }
            else if (iszero || (page->isShared() && (*(const Tuple*)masked != val)))
            {
                // No longer need this old constant page
                page->decRef();
            }
            else if (!page->isShared())
            {
                // Reuse the unshared constant page
                *(Tuple*)masked = val;
                return;
            }
            else
            {
                // Already equal; nothing to do
                UT_ASSERT_P(*(const Tuple*)masked == val);
                return;
            }
        }

        if (iszero)
        {
            page->initZero();
            return;
        }

        // Need to allocate new constant page
        PageTableEntry newpage;
        newpage.alloc(UT_PageOff(1), theSafeTupleSize);
        NotVoidType *tuple = newpage.getFirstPtr();
        (*(Tuple*)tuple) = val;
        newpage.setConstantBit();
        (*page) = newpage;
    }

    static void makeConstant(PageTableEntry *page, const NotVoidType &val, exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(!PAGESHARDENED);
        UT_ASSERT_P(tuplesize >= 1);

        if (PAGESHARDENED)
            return;

        // Unref existing non-constant page
        bool wasconst = page->isConstant();
        if (!wasconst)
            page->decRef();

        // Easy for inline case, partly checked at compile-time.
        if (PageTableEntry::typeFitsInline(tuplesize))
        {
            page->initZero();
            NotVoidType *tuple = page->getInlinePtr(tuplesize);
            for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                tuple[i] = val;
            return;
        }

        bool iszero = isZero(val);

        // In other cases, we may or may not have to unref constant page
        if (wasconst)
        {
            NotVoidType *masked = page->getMaskedPtr();
            if (!masked)
            {
                // Fairly common case: already zero, making zero.
                if (iszero)
                    return;
            }
            else if (iszero)
            {
                // No longer need this old constant page
                page->decRef();
            }
            else if (page->isShared())
            {
                bool equal = true;
                for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                {
                    if (masked[i] != val)
                    {
                        equal = false;
                        break;
                    }
                }

                if (equal)
                {
                    // Already equal; nothing to do
                    return;
                }

                // No longer need this old constant page
                page->decRef();
            }
            else
            {
                // Reuse the unshared constant page
                for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                    masked[i] = val;
                return;
            }
        }

        if (iszero)
        {
            page->initZero();
            return;
        }

        // Need to allocate new constant page
        PageTableEntry newpage;
        newpage.alloc(UT_PageOff(1), tuplesize);
        NotVoidType *tuple = newpage.getFirstPtr();
        for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
            tuple[i] = val;
        newpage.setConstantBit();
        (*page) = newpage;
    }

    static void makeConstant(PageTableEntry *page, const NotVoidType *values, exint tuplesize)
    {
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(TSIZE == -1 || TSIZE == tuplesize);
        UT_ASSERT_P(!PAGESHARDENED);
        UT_ASSERT_P(tuplesize >= 1);

        if (PAGESHARDENED)
            return;

        // Unref existing non-constant page
        bool wasconst = page->isConstant();
        if (!wasconst)
            page->decRef();

        // Easy for inline case, partly checked at compile-time.
        if (PageTableEntry::typeFitsInline(tuplesize))
        {
            page->initZero();
            NotVoidType *tuple = page->getInlinePtr(tuplesize);
            for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                tuple[i] = values[i];
            return;
        }

        bool iszero = isZero(values, (TSIZE >= 0 ? TSIZE : tuplesize));

        // In other cases, we may or may not have to unref constant page
        if (wasconst)
        {
            NotVoidType *masked = page->getMaskedPtr();
            if (!masked)
            {
                // Fairly common case: already zero, making zero.
                if (iszero)
                    return;
            }
            else if (iszero)
            {
                // No longer need this old constant page
                page->decRef();
            }
            else if (page->isShared())
            {
                bool equal = true;
                for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                {
                    if (masked[i] != values[i])
                    {
                        equal = false;
                        break;
                    }
                }

                if (equal)
                {
                    // Already equal; nothing to do
                    return;
                }

                // No longer need this old constant page
                page->decRef();
            }
            else
            {
                // Reuse the unshared constant page
                for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
                    masked[i] = values[i];
                return;
            }
        }

        if (iszero)
        {
            page->initZero();
            return;
        }

        // Need to allocate new constant page
        PageTableEntry newpage;
        newpage.alloc(UT_PageOff(1), tuplesize);
        NotVoidType *tuple = newpage.getFirstPtr();
        for (exint i = 0; i < (TSIZE >= 0 ? TSIZE : tuplesize); ++i)
            tuple[i] = values[i];
        newpage.setConstantBit();
        (*page) = newpage;
    }

    /// This replaces dest with src.  It references if possible and keeps
    /// dest constant if easy.  If desttuplesize > srctuplesize, it only
    /// replaces the first srctuplesize components.
    template<typename SrcType>
    static void replacePage(PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize, UT_PageOff destpagesize, UT_PageOff destpagecapacity);

    /// This replaces part of dest with part of src.  It keeps
    /// dest constant if easy.  If desttuplesize > srctuplesize, it only
    /// replaces the first srctuplesize components.
    /// This will *not* check to see if the whole page is being replaced,
    /// so if you want the src page to be referenced, or overwrite a
    /// constant value, when writing the whole page, use replacePage.
    template<typename SrcType>
    static void copyPartialPage(PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize, UT_PageOff destoff, UT_PageOff srcoff, UT_PageOff ntuples, UT_PageOff destpagecapacity);

    /// This version of makeConstant copies a const page onto this page,
    /// converting type if necessary, and supporting a different tuple size,
    /// so long as the destination size either isn't larger or is constant.
    /// NOTE: This would be templated on the actual templates of UT_PageArray,
    ///       but somehow, Visual C++ can't deduce the type, so the template type
    ///       has to be specified by the caller, and one argument is much easier
    ///       to specify on every call than 4.
    template<typename SrcType>
    static void makeConstantFrom(PageTableEntry *dest, const typename SrcType::PageTableEntry *src, exint desttuplesize, exint srctuplesize);

    /// This is just a helper function for writing a single source tuple (or nullptr) to a block of tuple data, with conversion.
    /// NOTE: destpagedata must point to an *UNSHARED* page's data.
    template<typename SrcNotVoidType>
    static void fillNonConstWithConst(NotVoidType *destpagedata, NotVoidType *destpageend, const SrcNotVoidType *stuple, exint mintuplesize, exint desttupleextra);

    /// This is just a helper function for copying a non-constant source page to a non-constant destination page, with conversion.
    /// NOTE: destpagedata must point to an *UNSHARED* page's data.
    /// NOTE: The two data ranges must be *NON-OVERLAPPING*.
    template<typename SrcNotVoidType>
    static void copyNonConst(NotVoidType *destpagedata, const SrcNotVoidType *srcpagedata, exint desttuplesize, exint srctuplesize, UT_PageOff ntuples);

    /// This is just a helper function for checking whether two constant pages are equal, up to
    /// the smaller of the two, to see if writing is necessary.
    template<typename SrcNotVoidType>
    static bool isEqualConst(const NotVoidType *tuple, const SrcNotVoidType *stuple, exint mintuplesize);

    static void hardenConstantPage(PageTableEntry *page, UT_PageOff pagecapacity, exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(page->isConstant());

        PageTableEntry newpage;
        newpage.alloc(pagecapacity, tuplesize);
        NotVoidType *dest = newpage.getFirstPtr();
        bool isinline = PageTableEntry::typeFitsInline(tuplesize);
        if (isinline ? page->isConstantAndZeroSafe() : page->isConstantZero())
        {
            memset(dest, 0, (sizeof(NotVoidType)*(TSIZE >= 0 ? TSIZE : tuplesize))*pagecapacity);
        }
        else
        {
            const NotVoidType *src;
            if (isinline)
                src = page->getInlinePtr(tuplesize);
            else
                src = page->getMaskedPtr();

            for (UT_PageOff i = 0; i < pagecapacity; ++i)
            {
                for (exint component = 0; component < tuplesize; ++component, ++dest)
                    *dest = src[component];
            }

            if (!isinline)
                page->decRef();
        }
        (*page) = newpage;
    }

    static void hardenConstantPageNoInit(PageTableEntry *page, UT_PageOff pagecapacity, exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(page->isConstant());

        PageTableEntry newpage;
        newpage.alloc(pagecapacity, tuplesize);
        // Compile-time check, to speed up case where even one element doesn't fit inline.
        bool isinline = PageTableEntry::typeFitsInline(tuplesize);
        if (!isinline && !page->isConstantZero())
            page->decRef();
        (*page) = newpage;
    }

    static void hardenSharedPage(PageTableEntry *page, UT_PageOff pagecapacity, exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(!page->isConstant());
        // This may technically no longer be shared between checking and calling
        // due to other threads decRef'ing the page.  It's okay to harden anyway.
        //UT_ASSERT_P(page->isShared());

        PageTableEntry newpage;
        newpage.alloc(pagecapacity, tuplesize);
        NotVoidType *dest = newpage.getFirstPtr();
        const NotVoidType *src = page->getFirstPtr();
        memcpy(dest, src, (sizeof(NotVoidType)*(TSIZE >= 0 ? TSIZE : tuplesize))*pagecapacity);
        page->decRef();
        (*page) = newpage;
    }

    static void hardenSharedPageNoInit(PageTableEntry *page, UT_PageOff pagecapacity, exint tuplesize = TSIZE)
    {
        UT_ASSERT_P(TSIZE == -1 || tuplesize == TSIZE);
        UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
        UT_ASSERT_P(!page->isConstant());
        // This may technically no longer be shared between checking and calling
        // due to other threads decRef'ing the page.  It's okay to harden anyway.
        //UT_ASSERT_P(page->isShared());

        PageTableEntry newpage;
        newpage.alloc(pagecapacity, tuplesize);
        page->decRef();
        (*page) = newpage;
    }

    class PageTable
    {
    public:
        typedef PageTable ThisType;
        typedef UT_PageArray<DATA_T,TSIZE,TABLEHARDENED,PAGESHARDENED,THEPAGEBITS,IDX_T> ParentType;

        SYS_FORCE_INLINE PageTableEntry *getFirstPage()
        {
            UT_ASSERT_P(myCapacity > IDX_T(0));
            return reinterpret_cast<PageTableEntry*>(this+1);
        }
        SYS_FORCE_INLINE const PageTableEntry *getFirstPage() const
        {
            UT_ASSERT_P(myCapacity > IDX_T(0));
            return reinterpret_cast<const PageTableEntry *>(this+1);
        }
        // NOTE: Clang sometimes decides to ignore const references and pass by
        //       value, so we have to return by pointer, in case the page
        //       is constant and inlined, so that the address of the constant
        //       value is accessible through this function.
        //       Yes, many cases don't rely on this, but to reduce the risk of
        //       bugs, the only way to access page pointers in a const PageTable
        //       is to call this to get the page pointer address and explicitly
        //       dereference, if valid in the given circumstance.
        SYS_FORCE_INLINE const PageTableEntry *getPPage(UT_PageNum i) const
        {
            UT_ASSERT_P(myCapacity > IDX_T(i << THEPAGEBITS));
            return reinterpret_cast<const PageTableEntry *>(this+1) + i;
        }
        SYS_FORCE_INLINE PageTableEntry *getPPage(UT_PageNum i)
        {
            UT_ASSERT_P(myCapacity > IDX_T(i << THEPAGEBITS));
            return reinterpret_cast<PageTableEntry*>(this+1) + i;
        }
        SYS_FORCE_INLINE bool isShared() const
        {
            UT_ASSERT_P(myRefCount.relaxedLoad() > 0);
            return myRefCount.relaxedLoad() != 1;
        }
        SYS_FORCE_INLINE exint getRefCount() const
        {
            return myRefCount.relaxedLoad();
        }
        /// NOTE: This is the size of the full array, not the number of pages.
        SYS_FORCE_INLINE IDX_T size() const
        {
            return mySize;
        }
        /// NOTE: This is the capacity of the full array, not the capacity of pages.
        SYS_FORCE_INLINE IDX_T capacity() const
        {
            return myCapacity;
        }
        /// NOTE: This will *assume* the capacity is large enough!  The caller *must* check!
        /// NOTE: This is the size of the full array, not the number of pages.
        SYS_FORCE_INLINE void setSize(IDX_T newsize)
        {
            UT_ASSERT_P(newsize <= capacity());
            UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");
            if (PAGESHARDENED && newsize > mySize)
            {
                UT_ASSERT_P(TSIZE >= 0);
                // Need to ensure pages between mySize and newsize are hardened.
                // Pages containing offsets less than or equal to mySize are
                // already guaranteed to be hardened.
                UT_PageNum startpage = numPages(mySize);
                UT_PageNum endpage = numPages(newsize);
                UT_PageOff pagecapacity(thePageSize);
                if (endpage == UT_PageNum(1) && exint(capacity()) < thePageSize)
                    pagecapacity = capacity();
                PageTableEntry *page = getPPage(startpage);
                for (UT_PageNum p = startpage; p < endpage; ++p, ++page)
                {
                    if (page->isConstant())
                        hardenConstantPage(page, pagecapacity);
                    else if (page->isShared())
                        hardenSharedPage(page, pagecapacity);
                }
            }
            mySize = newsize;
        }
        /// NOTE: This will *assume* the capacity is large enough!  The caller *must* check!
        /// NOTE: This is the size of the full array, not the number of pages.
        SYS_FORCE_INLINE void setSize(IDX_T newsize, exint tuplesize)
        {
            UT_ASSERT_P(newsize <= capacity());
            UT_ASSERT_MSG_P(myRefCount.relaxedLoad() == 1, "The table must already be hardened before we modify it!");
            if (PAGESHARDENED && newsize > mySize)
            {
                // Need to ensure pages between mySize and newsize are hardened.
                // Pages containing offsets less than or equal to mySize are
                // already guaranteed to be hardened.
                UT_PageNum startpage = numPages(mySize);
                UT_PageNum endpage = numPages(newsize);
                UT_PageOff pagecapacity(thePageSize);
                if (endpage == UT_PageNum(1) && exint(capacity()) < thePageSize)
                    pagecapacity = capacity();
                PageTableEntry *page = getPPage(startpage);
                for (UT_PageNum p = startpage; p < endpage; ++p, ++page)
                {
                    if (page->isConstant())
                        hardenConstantPage(page, pagecapacity, tuplesize);
                    else if (page->isShared())
                        hardenSharedPage(page, pagecapacity, tuplesize);
                }
            }
            mySize = newsize;
        }
        void fill(IDX_T start, IDX_T end, const NotVoidType &val);
        void fill(IDX_T start, IDX_T end, NotVoidType val, exint tuplesize);
        void fill(IDX_T start, IDX_T end, const NotVoidType *values, exint tuplesize);
        void fill(IDX_T start, IDX_T end, const UT_FixedVector<NotVoidType,theSafeTupleSize> &val);

        void incRef()
        {
            myRefCount.add(1);
        }
        void decRef(exint tuplesize)
        {
            const int64 new_count = myRefCount.add(-1);
            if (new_count != 0)
            {
                UT_ASSERT_P(new_count > 0);
                return;
            }

            UT_ASSERT_P(!(SYSisSame<DATA_T, void>()));

            const size_t bytesize = sizeof(NotVoidType)*((TSIZE >= 0) ? TSIZE : tuplesize);

            // If we're deleting the table, first, decRef all pages.
            UT_PageNum npages = numPages(myCapacity);
            PageTableEntry *page = getFirstPage();
            for (UT_PageNum p = 0; p < npages; ++p, ++page)
            {
                if (page->isRefd(bytesize))
                    page->decRef();
            }
            free(this);
#if 0
            printf("Free table %d pages (case 0);        %d tables total\n",int(npages), int(thePageTableCount.add(-1)));
            fflush(stdout);
#endif
        }

        void hardenAllPages(IDX_T start, IDX_T end, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));
            UT_ASSERT_P(tuplesize >= 0);
            UT_PageNum startpage = pageNum(start);
            UT_PageNum endpage = numPages(end);
            UT_PageOff pagecapacity(thePageSize);
            if (endpage == UT_PageNum(1) && exint(capacity()) < thePageSize)
                pagecapacity = capacity();
            PageTableEntry *page = getPPage(startpage);
            for (UT_PageNum p = startpage; p < endpage; ++p, ++page)
            {
                if (page->isConstant())
                    hardenConstantPage(page, pagecapacity, tuplesize);
                else if (page->isShared())
                    hardenSharedPage(page, pagecapacity, tuplesize);
            }
        }

        void tryCompressAllPages(IDX_T start, IDX_T end, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(tuplesize >= 0);
            UT_PageNum startpage = pageNum(start);
            UT_PageNum endpage = numPages(end);
            UT_PageNum npages = numPages(mySize);
            PageTableEntry *page = getPPage(startpage);
            for (UT_PageNum p = startpage; p < endpage; ++p, ++page)
            {
                if (page->isConstant())
                    continue;

                UT_PageOff pagesize = thePageSize;
                if (p+1 >= npages)
                {
                    if (p+1 == npages)
                    {
                        UT_PageOff pagesizetemp = pageOff(mySize);
                        if (pagesizetemp != UT_PageOff(0))
                            pagesize = pagesizetemp;
                    }
                    else
                        pagesize = UT_PageOff(0);
                }
                page->tryCompressPage(pagesize, tuplesize);
            }
        }

        void tryCompressPage(UT_PageNum pagenum, exint tuplesize = TSIZE)
        {
            UT_ASSERT_P(tuplesize >= 0);
            PageTableEntry *page = getPPage(pagenum);
            if (page->isConstant())
                return;

            UT_PageOff pagesize = thePageSize;
            UT_PageNum npages = numPages(mySize);
            if (pagenum+1 >= npages)
            {
                if (pagenum+1 == npages)
                {
                    UT_PageOff pagesizetemp = pageOff(mySize);
                    if (pagesizetemp != UT_PageOff(0))
                        pagesize = pagesizetemp;
                }
                else
                    pagesize = UT_PageOff(0);
            }
            page->tryCompressPage(pagesize, tuplesize);
        }

        static ThisType *alloc(UT_PageNum npages)
        {
            UT_ASSERT_MSG_P(npages > UT_PageNum(0), "Shouldn't have a zero-page page table; nullptr should be used instead.");
            ThisType *p = (ThisType *)malloc(sizeof(ThisType) + npages*sizeof(PageTableEntry));
            //UT_ASSERT_P(_CrtIsValidHeapPointer(p));
            p->myRefCount.relaxedStore(1);
#if 0
            printf("Alloc table %d pages;        %d tables total\n",int(npages), int(thePageTableCount.add(1)));
            fflush(stdout);
#endif
            return p;
        }
        static ThisType *harden(ThisType *pages, IDX_T newcapacity, exint tuplesize=TSIZE)
        {
            UT_ASSERT_P(!(SYSisSame<DATA_T,void>()));

            // If we're setting capacity to zero, free the table.
            if (newcapacity == IDX_T(0))
            {
                if (pages)
                    pages->decRef(tuplesize);
                return nullptr;
            }

            UT_ASSERT_P(tuplesize >= 0);
            UT_ASSERT_MSG_P(newcapacity == roundUpCapacity(newcapacity), "Caller should already have adjusted capacity");

            bool wasshared = pages && pages->isShared();
            UT_PageNum npages = numPages(newcapacity);
            IDX_T oldcapacity = pages ? pages->capacity() : IDX_T(0);
            UT_PageNum oldnpages = numPages(oldcapacity);

            // Caller must force capacity to be on a page boundary if more than one page, else power of 2.
            UT_ASSERT_P((newcapacity < IDX_T(thePageSize) && SYSisPow2(exint(newcapacity))) || newcapacity == IDX_T(exint(npages)<<thePageBits));

            const bool nonconstfirst = pages && !pages->getFirstPage()->isConstant();

            if (npages == UT_PageNum(1) && oldnpages == UT_PageNum(1) && !wasshared)
            {
                // Can reuse existing page table if we're staying at one page, but changing its capacity.

                // NOTE: The "newcapacity != oldcapacity" will almost always be true here, but
                //       we need to make absolutely sure that we don't realloc a shared page when just
                //       calling hardenTable()!  If we didn't have this check, it could happen, because
                //       between the reference count check in hardenTable() could return 2, and then
                //       before the pages->isShared() call above, the reference count drops to 1,
                //       so we have an already hardened table, but we can identify that we don't need
                //       to do anything because the capacity won't be changing.
                //       The reason we can't have hardenTable() realloc a shared page is that in
                //       GA_PrimitiveList, it needs to incDataRef() on all GA_OffsetLists in a shared
                //       page before ever hardening one.
                if (nonconstfirst && newcapacity != oldcapacity)
                {
                    // Just need to realloc the page.
                    UT_PageOff newpagecapacity = UT_PageOff(exint(newcapacity));
                    UT_PageOff sizetocopy = SYSmin(UT_PageOff(exint(pages->mySize)), newpagecapacity);
                    PageTableEntry *page = pages->getFirstPage();
                    page->realloc(sizetocopy, newpagecapacity, tuplesize);
                }
                // Don't need to change anything but capacity (and possibly size)
                // if constant page, even if it's an allocated constant page.

                pages->myCapacity = newcapacity;
                if (pages->mySize > newcapacity)
                    pages->mySize = newcapacity;
                return pages;
            }

            PageTable *table = PageTable::alloc(npages);
            table->myCapacity = newcapacity;
            if (pages)
            {
                UT_ASSERT_P(pages->getRefCount() > 0 && table->getRefCount() > 0);
                UT_ASSERT_P(pages->capacity() > IDX_T(0));
                UT_ASSERT_P(oldnpages > UT_PageNum(0));
                const bool wassmallfirstpage = pages->capacity() < IDX_T(thePageSize);
                const bool willbesmallfirstpage = newcapacity < IDX_T(thePageSize);

                UT_PageNum minnpages = SYSmin(npages, oldnpages);
                table->mySize = pages->mySize;
                memcpy(table->getFirstPage(), pages->getFirstPage(), (sizeof(PageTableEntry))*minnpages);

                UT_ASSERT_P(!(SYSisSame<DATA_T, void>()));

                const size_t bytesize = sizeof(NotVoidType)*((TSIZE >= 0) ? TSIZE : tuplesize);

                if (wasshared)
                {
                    // Increment all of the reference counts of reused pages,
                    // if we're unsharing a table.
                    PageTableEntry *page = table->getFirstPage();
                    for (UT_PageNum p = 0; p < minnpages; ++p, ++page)
                    {
                        if (page->isRefd(bytesize))
                            page->incRef();
                    }
                    pages->decRef(tuplesize);
                }
                else
                {
                    // pages isn't shared, so we can just decRef() any pages no
                    // longer referenced and free pages.  This avoids calling
                    // incRef on all kept pages and decRef on all pages like the
                    // above would do if pages had its reference count go to
                    // zero.
                    PageTableEntry *page = pages->getFirstPage() + npages;
                    for (UT_PageNum p = npages; p < oldnpages; ++p, ++page)
                    {
                        if (page->isRefd(bytesize))
                            page->decRef();
                    }
                    UT_ASSERT_MSG_P(!pages->isShared(), "This should still be unshared.");
                    free(pages);
#if 0
                    printf("Free table %d pages (case 1);        %d tables total\n",int(oldnpages), int(thePageTableCount.add(-1)));
                    fflush(stdout);
#endif
                }
                // Can't refer to pages beyond this point!
                UT_IF_ASSERT_P(pages = nullptr;)

                if (table->mySize > newcapacity)
                    table->mySize = newcapacity;

                // NOTE: The "newcapacity != oldcapacity" is to make absolutely
                //       sure that we don't realloc a shared page when just
                //       calling hardenTable()!  This is because in
                //       GA_PrimitiveList, it needs to incDataRef() on all
                //       GA_OffsetLists in a shared page before ever hardening one.
                if (nonconstfirst && (wassmallfirstpage || willbesmallfirstpage) && newcapacity != oldcapacity)
                {
                    UT_PageOff newpagecapacity = SYSmin(UT_PageOff(exint(newcapacity)), UT_PageOff(thePageSize));
                    UT_PageOff sizetocopy = SYSmin(UT_PageOff(exint(table->mySize)), newpagecapacity);
                    PageTableEntry *page = table->getFirstPage();
                    page->realloc(sizetocopy, newpagecapacity, tuplesize);
                }
            }
            else
            {
                oldnpages = 0;
                table->mySize = IDX_T(0);
            }

            // Set any new pages to be constant zero.
            PageTableEntry *page = table->getFirstPage() + oldnpages;
            for (UT_PageNum p = oldnpages; p < npages; ++p, ++page)
            {
                page->initZero();
            }

            if (PAGESHARDENED && wasshared)
            {
                if (TSIZE >= 1)
                    table->hardenAllPages(IDX_T(0), table->mySize);
                else
                    table->hardenAllPages(IDX_T(0), table->mySize, tuplesize);
            }

            return table;
        }

        /// Atomically decrement the reference count iff the page is shared.
        /// See the description of decRefIffSharedPage() above for
        /// why this function exists.
        bool decRefIffShared()
        {
            return decCounterIffAbove1(myRefCount);
        }

    private:
        // The constructor/destructor should never be called, since the class
        // can only be allocated with malloc.
        PageTable() { UT_ASSERT_P(0); }
        PageTable(const PageTable &) { UT_ASSERT_P(0); }
        ~PageTable() { UT_ASSERT_P(0); }

        SYS_AtomicCounter myRefCount;
        IDX_T mySize;
        IDX_T myCapacity;
    };

    /// NOTE: These have to be below the PageTable class definition, because
    ///       for no apparent reason, even though you can have a regular local
    ///       variable of type PageTable before, you can't have a function
    ///       parameter of type PageTable before.  *sigh*
    /// @{
    static void hardenConstantPage(PageTable *pages, PageTableEntry *page)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenConstantPage(page, pagecapacity);
    }
    static void hardenConstantPageNoInit(PageTable *pages, PageTableEntry *page)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenConstantPageNoInit(page, pagecapacity);
    }
    static void hardenConstantPage(PageTable *pages, PageTableEntry *page, exint tuplesize)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenConstantPage(page, pagecapacity, tuplesize);
    }
    static void hardenConstantPageNoInit(PageTable *pages, PageTableEntry *page, exint tuplesize)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenConstantPageNoInit(page, pagecapacity, tuplesize);
    }
    static void hardenSharedPage(PageTable *pages, PageTableEntry *page)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenSharedPage(page, pagecapacity);
    }
    static void hardenSharedPageNoInit(PageTable *pages, PageTableEntry *page)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenSharedPageNoInit(page, pagecapacity);
    }
    static void hardenSharedPage(PageTable *pages, PageTableEntry *page, exint tuplesize)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenSharedPage(page, pagecapacity, tuplesize);
    }
    static void hardenSharedPageNoInit(PageTable *pages, PageTableEntry *page, exint tuplesize)
    {
        const exint arraycapacity(exint(pages->capacity()));
        UT_PageOff pagecapacity((arraycapacity < thePageSize) ? arraycapacity : thePageSize);
        hardenSharedPageNoInit(page, pagecapacity, tuplesize);
    }
    /// @}

    void forceHardenTable(IDX_T newcapacity)
    {
        if (SYSisSame<DATA_T,void>())
        {
            const UT_Storage storage = myImpl.getStorage();
            switch (storage)
            {
                case UT_Storage::INVALID:
                    UT_ASSERT_MSG_P(0, "Can't have a non-numeric type with a void DATA_T.");
                    return;
                case UT_Storage::INT8:
                    castType<int8>().forceHardenTable(newcapacity);     return;
                case UT_Storage::INT16:
                    castType<int16>().forceHardenTable(newcapacity);    return;
                case UT_Storage::INT32:
                    castType<int32>().forceHardenTable(newcapacity);    return;
                case UT_Storage::INT64:
                    castType<int64>().forceHardenTable(newcapacity);    return;
                case UT_Storage::REAL16:
                    castType<fpreal16>().forceHardenTable(newcapacity); return;
                case UT_Storage::REAL32:
                    castType<fpreal32>().forceHardenTable(newcapacity); return;
                case UT_Storage::REAL64:
                    castType<fpreal64>().forceHardenTable(newcapacity); return;
            }
            return;
        }

        PageTable *&pages = myImpl.getPages();
        pages = PageTable::harden(pages, newcapacity, myImpl.getTupleSize());
    }

protected:
    template<typename T>
    SYS_FORCE_INLINE
    static bool isZero(const T &val)
    {
        // TODO: Find a way to check for zero that will still compile, but
        //       that avoids the mess of dealing with memset for POD types.
        T zero;
        memset(&zero, 0, sizeof(T));
        return (val == zero);
    }

    template<typename T>
    SYS_FORCE_INLINE
    static bool isZero(const T *val,exint tuplesize)
    {
        UT_ASSERT_P(tuplesize >= 1);
        if (tuplesize == 0)
            return true;

        // TODO: Find a way to check for zero that will still compile, but
        //       that avoids the mess of dealing with memset for POD types.
        T zero;
        memset(&zero, 0, sizeof(T));

        bool iszero = (val[0] == zero);
        for (exint i = 1; i < tuplesize; ++i)
            iszero &= (val[i] == zero);
        return iszero;
    }

    template<typename T0,typename T1>
    SYS_FORCE_INLINE
    static bool isEqual(const T0 *a,const T1 *b,exint tuplesize)
    {
        UT_ASSERT_P(tuplesize >= 1);
        UT_ASSERT_P(a != nullptr && b != nullptr);

        for (exint i = 0; i < tuplesize; ++i)
        {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }

    template<typename DEST_TYPE,typename SRC_TYPE,exint SRC_TSIZE,bool SRC_INSTANTIATED>
    SYS_FORCE_INLINE
    static UT_FixedVector<DEST_TYPE, SRC_TSIZE> convertVectorStorage(const UT_FixedVector<SRC_TYPE,SRC_TSIZE,SRC_INSTANTIATED> &src)
    {
        if (SYSisSame<DEST_TYPE,SRC_TYPE>())
            return src;

        UT_FixedVector<DEST_TYPE, SRC_TSIZE> dest;
        for (exint i = 0; i < SRC_TSIZE; ++i)
            dest[i] = UTconvertStorage<DEST_TYPE>(src[i]);
        return dest;
    }

private:
    /// This class wraps the data of UT_PageArray, just so that if
    /// the tuple size and data type are known at compile-time, it doesn't
    /// need to store that information as an int and a UT_Storage.
    /// myPages is the same in all of them, but can't be extracted out,
    /// because the C++ standard, unfortunately, forbids 0-byte classes.
    /// @{
    struct ImplPageData
    {
        PageTable *myPages;
    };

    template<exint ITSIZE, typename IDATA_T>
    class Impl :
        private ImplPageData,
        private UT_TupleType<ITSIZE, IDATA_T>
    {
    public:
        SYS_FORCE_INLINE const PageTable *getPages() const { return ImplPageData::myPages; }
        SYS_FORCE_INLINE PageTable *&getPages() { return ImplPageData::myPages; }
        using UT_TupleType<ITSIZE, IDATA_T>::getTupleSize;
        using UT_TupleType<ITSIZE, IDATA_T>::setTupleSize;
        using UT_TupleType<ITSIZE, IDATA_T>::getStorage;
        using UT_TupleType<ITSIZE, IDATA_T>::setStorage;
    };

    Impl<TSIZE,DATA_T> myImpl;
    /// @}
};

#endif
