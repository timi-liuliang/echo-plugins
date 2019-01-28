/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Handle.h ( GA Library, C++)
 *
 * COMMENTS:	A lightweight probe to deal with specific attributes of
 *		a specific type.
 */

#pragma once

#ifndef __GA_Handle__
#define __GA_Handle__

#include "GA_API.h"

#include "GA_ATINumeric.h"
#include "GA_PageArray.h"

#include "GA_AIFNumericArray.h"
#include "GA_AIFSharedStringArray.h"
#include "GA_AIFSharedStringTuple.h"
#include "GA_ATIString.h"
#include "GA_OffsetList.h"
#include "GA_Types.h"

#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID
#include "GA_IndexMap.h"
#endif

#include <UT/UT_Assert.h>
#include <UT/UT_FixedVector.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Storage.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>


class GA_Attribute;
class GA_Detail;
class GA_Range;
class UT_StringArray;


namespace {

class ga_OffsetToIndexTrivial
{
public:
    ga_OffsetToIndexTrivial(GA_Offset start)
	: myStartOffset(start)
    {
    }

    GA_Index		operator()(GA_Offset offset) const
    {
	return GA_Index((GA_Size)(offset - myStartOffset));
    }

protected:
    const GA_Offset	myStartOffset;
    
};

class ga_OffsetToIndex
{
public:
    ga_OffsetToIndex(GA_Offset startoff, const GA_ListType<GA_Offset, GA_Index> &offtoindex)
	: myStartIndex(offtoindex(startoff))
	, myOffsetToIndex(offtoindex)
    {
    }

    GA_Index		operator()(GA_Offset offset) const
    {
	return myOffsetToIndex(offset) - myStartIndex;
    }

protected:
    const GA_Index	myStartIndex;
    const GA_ListType<GA_Offset, GA_Index> &myOffsetToIndex;
};

}

/// Read-only handle for vector attribute data.
///
/// @tparam T	Return vector type for get methods. Requires T::value_type to
///		describe the underlying element type of the vector.
///
/// @tparam ENABLE  Use SYS_EnableIf to specialize arithmetic template
///		implementations (i.e. see the SCALAR implementation)
///
template <typename T, typename ENABLE = void>
class GA_ROHandleT
{
public:
    typedef void (*GAprobeAttribGetter)(T &result, const GA_Attribute *atr, GA_Offset);
    typedef typename T::value_type BASETYPE;
    static const exint theTupleSize = UT_FixedVectorTraits<T>::TupleSize;

    GA_ROHandleT()
    {
	clear();
    }
    GA_ROHandleT(const GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_ROHandleT(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(const GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    SYS_FORCE_INLINE
    void bind(const GA_Attribute *attrib, int minsize=1)
    {
	bind(GA_ATINumeric::cast(attrib), minsize);
    }

    void bind(const GA_ATINumeric *attrib, int minsize=1)
    {
	myExactMatch = nullptr;
	myAlmostMatch = nullptr;
        myAttrib = SYSconst_cast(attrib);
        if (!myAttrib)
            return;

        int actual_tuple_size = myAttrib->getTupleSize();
        if (actual_tuple_size < minsize * theTupleSize)
        {
            myAttrib = nullptr;
            return;
        }

        if (actual_tuple_size == theTupleSize)
        {
            UT_Storage actual_storage = myAttrib->getData().Base::getStorage();
            if (actual_storage == UT_StorageNum<BASETYPE>::theStorage)
                myExactMatch = &myAttrib->getData().castType<BASETYPE>().template castTupleSize<theTupleSize>();
            else if (actual_storage == UT_StorageNum<typename UT_StorageNum<BASETYPE>::SecondGuess>::theStorage)
                myAlmostMatch = &myAttrib->getData().castType<typename UT_StorageNum<BASETYPE>::SecondGuess>().template castTupleSize<theTupleSize>();
        }
    }

    const GA_ATINumeric *getAttribute() const { return myAttrib; }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }


    // Check whether the attribute is GA_TYPE_HPOINT
    bool isRational() const
    { return myAttrib->getTypeInfo() == GA_TYPE_HPOINT; }

    void clear()
    {
        myAttrib = nullptr;
        myExactMatch = nullptr;
        myAlmostMatch = nullptr;
    }

    SYS_FORCE_INLINE bool isValid() const
    {
	return myAttrib;
    }
    SYS_FORCE_INLINE bool isInvalid() const
    {
	return !myAttrib;
    }

    SYS_FORCE_INLINE T get(GA_Offset off, int comp=0) const
    {
	UT_ASSERT_P(comp == 0 &&
	    "Component offset not currently supported for vector/matrix types");
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));

        if (myExactMatch)
            return T(myExactMatch->template getVector<BASETYPE,theTupleSize>(off));
        return getNonInlinePart(off);
    }
private:
    T getNonInlinePart(GA_Offset off) const
    {
        if (myAlmostMatch)
            return T(myAlmostMatch->template getVector<BASETYPE,theTupleSize>(off));
        return T(myAttrib->getData().getVector<BASETYPE,theTupleSize>(off));
    }
public:

    /// Copies attribute values from a contiguous block of offsets
    /// into the dest array.
    /// NOTE: The block of offsets is assumed to have no holes, else
    ///       data could be read from offsets that don't have
    ///       corresponding elements.
    void getBlock(GA_Offset startoff, GA_Size nelements, T *dest, int deststride = 1, int component = 0) const;

    /// Copies attribute values from a contiguous block of indices
    /// into the dest array.
    /// This allows there to be holes in the geometry.
    void getBlockFromIndices(GA_Index startidx, GA_Size nelements, T *dest, int deststride = 1, int component = 0) const;

    SYS_FORCE_INLINE T operator()(GA_Offset off) const
    {
	return get(off);
    }
    SYS_FORCE_INLINE T operator[](GA_Offset off) const
    {
        return get(off);
    }
    /// Returns true iff the specified page is constant-compressed,
    /// where a single value is stored to represent GA_PAGE_SIZE
    /// values in the page, (or possibly fewer if only one page).
    SYS_FORCE_INLINE bool isPageConstant(GA_PageNum pagenum) const
    {
        return myAttrib->isPageConstant(pagenum);
    }
    bool operator==(const GA_ROHandleT<T> &handle) const
    {
	return myAttrib == handle.myAttrib;
    }
    bool operator!=(const GA_ROHandleT<T> &handle) const
    {
	return myAttrib != handle.myAttrib;
    }
    const GA_ATINumeric *operator->() const
    {
        return myAttrib;
    }
protected:
    GA_ATINumeric		*myAttrib;
    GA_PageArray<BASETYPE,theTupleSize> *myExactMatch;
    GA_PageArray<typename UT_StorageNum<BASETYPE>::SecondGuess,theTupleSize> *myAlmostMatch;
};

/// Read-write handle for vector attribute data.
///
/// @tparam T	Return vector type for get methods. Requires T::value_type to
///		describe the underlying element type of the vector.
///
template <typename T, typename ENABLE = void>
class GA_RWHandleT : public GA_ROHandleT<T, ENABLE>
{
public:
    typedef void (*GAprobeAttribSetter)(GA_Attribute *atr, GA_Offset, const T &);
    typedef GA_ROHandleT<T> Base;
    typedef typename T::value_type BASETYPE;
private:
    using Base::myAttrib;
    using Base::myExactMatch;
    using Base::myAlmostMatch;
public:

    GA_RWHandleT()
    {
        clear();
    }
    GA_RWHandleT(GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_RWHandleT(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    SYS_FORCE_INLINE
    void bind(GA_Attribute *attrib, int minsize=1)
    {
        bind(GA_ATINumeric::cast(attrib), minsize);
    }
    SYS_FORCE_INLINE
    void bind(GA_ATINumeric *attrib, int minsize=1)
    {
	Base::bind(attrib, minsize);
    }

    using Base::clear;

    GA_ATINumeric *getAttribute() const { return myAttrib; }

    void bumpDataId() const
    {
	if (myAttrib)
	    myAttrib->bumpDataId();
    }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }

    SYS_FORCE_INLINE void set(GA_Offset off, const T &val) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else if (myAlmostMatch)
            myAlmostMatch->setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else
            myAttrib->getData().setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
    }

    SYS_FORCE_INLINE void set(GA_Offset off, int comp, const T &val) const
    {
	UT_ASSERT(comp == 0 && "Component sets on vector types unsupported");
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else if (myAlmostMatch)
            myAlmostMatch->setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else
            myAttrib->getData().setVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
    }

    SYS_FORCE_INLINE void add(GA_Offset off, const T &val) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->addVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else if (myAlmostMatch)
            myAlmostMatch->addVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
        else
            myAttrib->getData().addVector(off, reinterpret_cast<const typename UT_FixedVectorTraits<T>::FixedVectorType &>(val));
    }

    /// Copies attribute values from the source array into a contiguous
    /// block of offsets.
    /// NOTE: The block of offsets is assumed to have no holes, else
    ///       data could be written to offsets that don't have
    ///       corresponding elements.
    void setBlock(GA_Offset startoff, GA_Size nelements, const T *source, int sourcestride = 1, int component = 0) const;

    void setBlockFromIndices(GA_Index startidx, GA_Size nelements, const T *source, int sourcestride = 1, int component = 0) const;

    void makeConstant(const T &cval) const
    {
        UT_ASSERT_P(myAttrib);
	myAttrib->getAIFTuple()->makeConstant(myAttrib, &cval.data()[0], sizeof(T)/sizeof(BASETYPE), 0);
    }

    /// Sets all components of all elements of the specified page to
    /// the given values.
    /// NOTE: The length of values must be equal to the tuple size.
    void setPageConstant(GA_PageNum pagenum, const BASETYPE *values) const
    {
        if (myExactMatch)
            myExactMatch->setPageConstant(pagenum, values);
        else if (myAlmostMatch)
            myAlmostMatch->setPageConstant(pagenum, values);
        else
            myAttrib->getData().setPageConstant(pagenum, values);
    }

    GA_ATINumeric *operator->() const
    {
        return myAttrib;
    }

};

/// Simplify checking for scalar types
#define SCALAR(T) SYS_EnableIf< SYS_IsArithmetic<T>::value >::type

/// Read-only handle for scalar attribute data.
///
/// @tparam T	Return type for get methods
///
template <typename T>
class GA_ROHandleT<T, typename SCALAR(T) >
{
public:
    typedef T (*GAprobeAttribGetter)(const GA_Attribute *atr, GA_Offset, int);
    typedef T BASETYPE;

    GA_ROHandleT()
    {
	clear();
    }
    GA_ROHandleT(const GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_ROHandleT(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(const GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    SYS_FORCE_INLINE
    void bind(const GA_Attribute *attrib, int minsize=1)
    {
        bind(GA_ATINumeric::cast(attrib), minsize);
    }
    void bind(const GA_ATINumeric *attrib, int minsize=1)
    {
	myExactMatch = nullptr;
	myAlmostMatch = nullptr;
	myAttrib = SYSconst_cast(attrib);
	if (!myAttrib)
	    return;

        int actual_tuple_size = myAttrib->getTupleSize();
	if (actual_tuple_size < minsize)
	{
	    myAttrib = nullptr;
	    return;
	}

        UT_Storage actual_storage = attrib->getData().Base::getStorage();
        if (actual_storage == UT_StorageNum<BASETYPE>::theStorage)
        {
            if (actual_tuple_size == 1)
                myExactMatch = &myAttrib->getData().castType<BASETYPE>().template castTupleSize<1>();
            else
                myAlmostMatch = &myAttrib->getData().castType<BASETYPE>();
        }
    }

    const GA_ATINumeric *getAttribute() const { return myAttrib; }
    int			 getTupleSize() const { return myAttrib->getTupleSize(); }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }

    // Check whether the attribute is GA_TYPE_HPOINT
    bool isRational() const
	    { return myAttrib->getTypeInfo() == GA_TYPE_HPOINT; }

    void clear()
    {
	myAttrib = nullptr;
        myExactMatch = nullptr;
        myAlmostMatch = nullptr;
    }

    SYS_FORCE_INLINE bool isValid() const
    {
	return myAttrib;
    }
    SYS_FORCE_INLINE bool isInvalid() const
    {
	return !myAttrib;
    }

    SYS_FORCE_INLINE T get(GA_Offset off, int comp = 0) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        UT_ASSERT_P(comp >= 0 && comp < myAttrib->getTupleSize());
        if (myExactMatch)
        {
            UT_ASSERT_P(comp == 0);
            return myExactMatch->template get<T>(off);
        }
        return getNotInline(off, comp);
    }
private:
    T getNotInline(GA_Offset off, int comp) const
    {
        if (myAlmostMatch)
            return myAlmostMatch->template get<T>(off, comp);
        return myAttrib->getData().get<T>(off, comp);
    }
public:
    SYS_FORCE_INLINE void getV(GA_Offset off, T *data, int size) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(size >= 0 && size <= myAttrib->getTupleSize());
        if (myExactMatch)
        {
            UT_ASSERT_P(size<=1);
            if (size==1)
                data[0] = myExactMatch->template get<T>(off);
        }
        else if (myAlmostMatch)
        {
            for (int comp = 0; comp < size; ++comp)
                data[comp] = myAlmostMatch->template get<T>(off, comp);
        }
        else
        {
            auto &src = myAttrib->getData();
            for (int comp = 0; comp < size; ++comp)
                data[comp] = src.get<T>(off, comp);
        }
    }

    /// Copies attribute values from a contiguous block of offsets
    /// into the dest array.
    /// NOTE: The block of offsets is assumed to have no holes, else
    ///       data could be read from offsets that don't have
    ///       corresponding elements.
    void getBlock(GA_Offset startoff, GA_Size nelements, T *dest, int deststride = 1, int component = 0) const;

    /// Copies attribute values from a contiguous block of indices
    /// into the dest array.
    /// This allows there to be holes in the geometry.
    void getBlockFromIndices(GA_Index startidx, GA_Size nelements, T *dest, int deststride = 1, int component = 0) const;

    SYS_FORCE_INLINE T operator()(GA_Offset off, int comp = 0) const
    {
        return get(off, comp);
    }
    SYS_FORCE_INLINE T operator[](GA_Offset off) const
    {
        return get(off);
    }
    /// Returns true iff the specified page is constant-compressed,
    /// where a single value is stored to represent GA_PAGE_SIZE
    /// values in the page, (or possibly fewer if only one page).
    SYS_FORCE_INLINE bool isPageConstant(GA_PageNum pagenum) const
    {
        return myAttrib->isPageConstant(pagenum);
    }
    bool operator==(const GA_ROHandleT<T> &handle) const
    {
	return myAttrib == handle.myAttrib;
    }
    bool operator!=(const GA_ROHandleT<T> &handle) const
    {
	return myAttrib != handle.myAttrib;
    }
    const GA_ATINumeric *operator->() const
    {
        return myAttrib;
    }

protected:
    GA_ATINumeric		*myAttrib;
    GA_PageArray<BASETYPE,1> *myExactMatch;
    GA_PageArray<BASETYPE> *myAlmostMatch;
};

/// Read-write handle for scalar attribute data.
///
/// @tparam T	Return type for get methods
///
template <typename T>
class GA_RWHandleT<T, typename SCALAR(T) > : public GA_ROHandleT<T, typename SCALAR(T) >
{
public:
    typedef void (*GAprobeAttribSetter)(GA_Attribute *atr, GA_Offset, int, T);
    typedef GA_ROHandleT<T> Base;
    typedef T BASETYPE;
private:
    using Base::myAttrib;
    using Base::myExactMatch;
    using Base::myAlmostMatch;
public:

    GA_RWHandleT()
    {
	clear();
    }
    GA_RWHandleT(GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_RWHandleT(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    SYS_FORCE_INLINE
    void bind(GA_Attribute *attrib, int minsize=1)
    {
        bind(GA_ATINumeric::cast(attrib), minsize);
    }
    void bind(GA_ATINumeric *attrib, int minsize=1)
    {
	Base::bind(attrib, minsize);
    }

    using Base::clear;

    GA_ATINumeric *getAttribute() const { return myAttrib; }

    void bumpDataId() const
    {
	if (myAttrib)
	    myAttrib->bumpDataId();
    }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }

    SYS_FORCE_INLINE void set(GA_Offset off, T val) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->set(off, val);
        else if (myAlmostMatch)
            myAlmostMatch->set(off, val);
        else
            myAttrib->getData().set(off, val);
    }

    SYS_FORCE_INLINE void add(GA_Offset off, T val) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->add(off, val);
        else if (myAlmostMatch)
            myAlmostMatch->add(off, val);
        else
            myAttrib->getData().add(off, val);
    }

    SYS_FORCE_INLINE void set(GA_Offset off, int comp, T val) const
    {
        UT_ASSERT_P(myAttrib != nullptr);
        UT_ASSERT_P(comp >= 0 && comp < myAttrib->getTupleSize());
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->set(off, comp, val);
        else if (myAlmostMatch)
            myAlmostMatch->set(off, comp, val);
        else
            myAttrib->getData().set(off, comp, val);
    }
    SYS_FORCE_INLINE void setV(GA_Offset off, const T *data, int size) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(size >= 0 && size <= myAttrib->getTupleSize());
        if (myExactMatch)
        {
            UT_ASSERT_P(size<=1);
            if (size==1)
                myExactMatch->set(off, data[0]);
        }
        else if (myAlmostMatch)
        {
            for (int comp = 0; comp < size; ++comp)
                myAlmostMatch->set(off, comp, data[comp]);
        }
        else
        {
            auto &src = myAttrib->getData();
            for (int comp = 0; comp < size; ++comp)
                src.set(off, comp, data[comp]);
        }
    }

    SYS_FORCE_INLINE void add(GA_Offset off, int comp, T val) const
    {
        UT_ASSERT_P(myAttrib != nullptr);
        UT_ASSERT_P(comp >= 0 && comp < myAttrib->getTupleSize());
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
        if (myExactMatch)
            myExactMatch->add(off, comp, val);
        else if (myAlmostMatch)
            myAlmostMatch->add(off, comp, val);
        else
            myAttrib->getData().add(off, comp, val);
    }

    /// Copies attribute values from the source array into a contiguous
    /// block of offsets.
    /// NOTE: The block of offsets is assumed to have no holes, else
    ///       data could be written to offsets that don't have
    ///       corresponding elements.
    void setBlock(GA_Offset startoff, GA_Size nelements, const T *source, int sourcestride=1, int component=0) const;

    void setBlockFromIndices(GA_Index startidx, GA_Size nelements, const T *source, int sourcestride = 1, int component = 0) const;

    // -1 comp means to set all components to cval.
    void makeConstant(T cval, int comp = -1) const
    {
        UT_ASSERT_P(myAttrib);
	myAttrib->getAIFTuple()->makeConstant(myAttrib, cval, comp);
    }
    void makeConstantV(const T *cval, int size) const
    {
        UT_ASSERT_P(myAttrib);
	myAttrib->getAIFTuple()->makeConstant(myAttrib, cval, size, 0);
    }
    /// Sets all components of all elements of the specified page to
    /// the given values.
    /// NOTE: The length of values must be equal to the tuple size.
    void setPageConstant(GA_PageNum pagenum, const T *values) const
    {
        if (myExactMatch)
            myExactMatch->setPageConstant(pagenum, values);
        else if (myAlmostMatch)
            myAlmostMatch->setPageConstant(pagenum, values);
        else
            myAttrib->getData().setPageConstant(pagenum, values);
    }

    GA_ATINumeric *operator->() const
    {
        return myAttrib;
    }

};

#undef SCALAR

/// Read-only handle for string attribute data.
class GA_API GA_ROHandleS
{
public:
    GA_ROHandleS()
    {
	clear();
    }
    GA_ROHandleS(const GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_ROHandleS(const GA_Detail *gdp, GA_AttributeOwner owner,
		const UT_StringRef &name, int minsize=1);
    void operator=(const GA_Attribute *attrib)
    {
	bind(attrib);
    }
    void bind(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    /// Bind the handle to the given attribute
    void	bind(const GA_Attribute *attrib, int minsize=1)
    {
        myAttrib = GA_ATIString::cast(attrib);
	if (myAttrib && myAttrib->getTupleSize() < minsize)
	{
	    myAttrib = 0;
	}
    }

    /// Access to attribute
    const GA_Attribute	*getAttribute() const { return myAttrib; }
    /// Tuple size of the attribute
    int			 getTupleSize() const { return myAttrib->getTupleSize(); }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }


    /// Clear the handle
    void 			clear()			{ myAttrib = 0; }

    /// @{
    /// Validity checks
    SYS_FORCE_INLINE bool	isValid() const		{ return myAttrib; }
    SYS_FORCE_INLINE bool	isInvalid() const	{ return !myAttrib; }
    /// @}

    /// Get the string at the given offset
    SYS_FORCE_INLINE const UT_StringHolder &get(GA_Offset off, int comp = 0) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	return myAttrib->getString(off, comp);
    }
    /// Get the string index at the given offset
    SYS_FORCE_INLINE GA_StringIndexType getIndex(GA_Offset off, int comp = 0) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	return myAttrib->getStringIndex(off, comp);
    }
    const GA_ATIString *operator->() const
    {
        return myAttrib;
    }

protected:
    const GA_ATIString *myAttrib;
};

/// Read-Write string handle
class GA_API GA_RWHandleS : public GA_ROHandleS
{
public:
    GA_RWHandleS()
	: GA_ROHandleS()
    {
    }
    GA_RWHandleS(GA_Attribute *attrib)
	: GA_ROHandleS(attrib)
    {
    }
    GA_RWHandleS(const GA_Detail *gdp, GA_AttributeOwner owner,
		const UT_StringRef &name, int minsize=1)
	: GA_ROHandleS(gdp, owner, name, minsize)
    {
    }
    void operator=(GA_Attribute *attrib)
    {
	bind(attrib);
    }

    GA_Attribute *getAttribute() const { return rwAttrib(); }

    void bumpDataId() const
    {
	if (rwAttrib())
	    rwAttrib()->bumpDataId();
    }

    GA_DataId getDataId() const
    {
	if (rwAttrib())
	    return rwAttrib()->getDataId();
	return GA_INVALID_DATAID;
    }

    /// Store the @c str at the given offset
    SYS_FORCE_INLINE void set(GA_Offset off, const UT_StringHolder &str) const
    {
	set(off, 0, str);
    }
    SYS_FORCE_INLINE void set(GA_Offset off, int comp, const UT_StringHolder &str) const
    {
        UT_ASSERT_P(rwAttrib() != nullptr);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(rwAttrib()->getIndexMap().isOffsetActive(off) || rwAttrib()->getIndexMap().isOffsetTransient(off));
	rwAttrib()->setString(off, str, comp);
    }
    /// Store the string indexed by the @c idx at the given offset
    SYS_FORCE_INLINE void set(GA_Offset off, GA_StringIndexType idx) const
    {
	set(off, 0, idx);
    }
    SYS_FORCE_INLINE void set(GA_Offset off, int comp, GA_StringIndexType idx) const
    {
        UT_ASSERT_P(rwAttrib() != nullptr);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(rwAttrib()->getIndexMap().isOffsetActive(off) || rwAttrib()->getIndexMap().isOffsetTransient(off));
	rwAttrib()->setStringIndex(off, idx, comp);
    }
    GA_ATIString *operator->() const
    {
        return rwAttrib();
    }
protected:
    GA_ATIString	*rwAttrib() const
			    { return SYSconst_cast(myAttrib); }
};

/// Read-Write string handle
/// Batches all reference counts until a flush() or its destructor.
/// Because it tracks this locally, you MUST create a separate one
/// per thread!  (Even if this were made thread safe, that would
/// defeat the purpose of this, which is to use thread-local reference
/// counts to avoid write contention)
/// NOTE: This bumps the data ID of any attribute it's bound to
///       upon destruction.
class GA_API GA_RWBatchHandleS
{
public:
    GA_RWBatchHandleS()
    {
	clear();
    }
    GA_RWBatchHandleS(GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_RWBatchHandleS(GA_Detail *gdp, GA_AttributeOwner owner,
		const UT_StringRef &name, int minsize=1);
    void operator=(GA_Attribute *attrib)
    {
	bind(attrib);
    }
    void bind(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    /// Bind the handle to the given attribute
    void	bind(GA_Attribute *attrib, int minsize=1)
    {
        myAttrib = GA_ATIString::cast(attrib);
	if (myAttrib && myAttrib->getTupleSize() < minsize)
	{
	    myAttrib = 0;
	}
	myWriter.bind(myAttrib);
    }

    /// Access to attribute
    GA_Attribute	*getAttribute() const { return myAttrib; }
    /// Tuple size of the attribute
    int			 getTupleSize() const { return myAttrib->getTupleSize(); }
    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }


    /// Clear the handle
    void 			clear()			
    { 
	myAttrib = 0; 
	myWriter.bind(myAttrib);	// Also flushes.
    }

    /// @{
    /// Validity checks
    SYS_FORCE_INLINE bool	isValid() const		{ return myAttrib; }
    SYS_FORCE_INLINE bool	isInvalid() const	{ return !myAttrib; }
    /// @}

    /// Get the string at the given offset
    SYS_FORCE_INLINE const char	*get(GA_Offset off, int comp = 0) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	return myAttrib->getString(off, comp);
    }
    /// Get the string index at the given offset
    SYS_FORCE_INLINE GA_StringIndexType	getIndex(GA_Offset off, int comp = 0) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	return myAttrib->getStringIndex(off, comp);
    }

    /// Store the @c str at the given offset
    SYS_FORCE_INLINE void	set(GA_Offset off, const UT_StringHolder &str)
    {
	set(off, 0, str);
    }
    SYS_FORCE_INLINE void	set(const GA_Range &range, const UT_StringHolder &str)
    {
	set(range, 0, str);
    }
    SYS_FORCE_INLINE void	set(GA_Offset off, int comp, const UT_StringHolder &str)
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	myWriter.setString(off, str, comp);
    }
    SYS_FORCE_INLINE void	set(const GA_Range &range, int comp, const UT_StringHolder &str)
    {
	myWriter.setString(range, str, comp);
    }
    /// Store the string indexed by the @c idx at the given offset
    SYS_FORCE_INLINE void	set(GA_Offset off, GA_StringIndexType idx)
    {
	set(off, 0, idx);
    }
    SYS_FORCE_INLINE void	set(const GA_Range &range, GA_StringIndexType idx)
    {
	set(range, 0, idx);
    }
    SYS_FORCE_INLINE void	set(GA_Offset off, int comp, GA_StringIndexType idx)
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	myWriter.setStringIndex(off, idx, comp);
    }
    SYS_FORCE_INLINE void	set(const GA_Range &range, int comp, GA_StringIndexType idx)
    {
	myWriter.setStringIndex(range, idx, comp);
    }

    SYS_FORCE_INLINE bool hasChanges() const
    {
        return myWriter.hasChanges();
    }
protected:
    GA_ATIString		*myAttrib;
    GA_ATIStringDelayedWriter	 myWriter;
};

namespace GA_HandleDetail
{
// Helper for determining AIF type given the array type T
template <typename T> struct ArrayAIF;
template <> struct ArrayAIF<UT_Int32Array> { typedef GA_AIFNumericArray type; };
template <> struct ArrayAIF<UT_Int64Array> { typedef GA_AIFNumericArray type; };
template <> struct ArrayAIF<UT_Fpreal32Array> { typedef GA_AIFNumericArray type; };
template <> struct ArrayAIF<UT_Fpreal64Array> { typedef GA_AIFNumericArray type; };
template <> struct ArrayAIF<UT_StringArray> { typedef GA_AIFSharedStringArray type; };
// Helper for determining if we have an acceptable array type
template <typename T> struct IsArray         { static const bool value = false; };
template <> struct IsArray<UT_Int32Array>    { static const bool value = true; };
template <> struct IsArray<UT_Int64Array>    { static const bool value = true; };
template <> struct IsArray<UT_Fpreal32Array> { static const bool value = true; };
template <> struct IsArray<UT_Fpreal64Array> { static const bool value = true; };
template <> struct IsArray<UT_StringArray>   { static const bool value = true; };
}

#define ENABLE_ARRAY(T) SYS_EnableIf< GA_HandleDetail::IsArray<T>::value >::type

/// Read-only handle for array attribute data 
template <typename T>
class GA_ROHandleT<T, typename ENABLE_ARRAY(T)>
{
public:
    typedef typename GA_HandleDetail::ArrayAIF<T>::type AIFType;

    GA_ROHandleT()
    {
	clear();
    }
    GA_ROHandleT(const GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_ROHandleT(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(const GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(const GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);
    void bind(const GA_Attribute *attrib, int minsize=1)
    {
	myAttrib = const_cast<GA_Attribute *>(attrib);
	if (!myAttrib)
	    return;
	myAIF = myAttrib->getAIF<AIFType>();
	if (!myAIF)
	    myAttrib = 0;
    }

    const GA_Attribute *getAttribute() const	{ return myAttrib; }

    void clear()
    {
	myAttrib = 0;
	myAIF = 0;
    }

    SYS_FORCE_INLINE bool isValid() const	{ return myAttrib; }
    SYS_FORCE_INLINE bool isInvalid() const	{ return !myAttrib; }

    SYS_FORCE_INLINE void get(GA_Offset off, T &value) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	myAIF->get(myAttrib, off, value);
    }

    bool operator==(const GA_ROHandleT<T> &handle) const
    {
	UT_ASSERT_P(myAIF == handle.myAIF);
	return myAttrib == handle.myAttrib;
    }
    bool operator!=(const GA_ROHandleT<T> &handle) const
    {
	UT_ASSERT_P(myAIF == handle.myAIF);
	return myAttrib != handle.myAttrib;
    }
    const GA_Attribute *operator->() const
    {
        return myAttrib;
    }

protected:
    GA_Attribute*   myAttrib;
    const AIFType*  myAIF;
};

/// Read-Write handle for array attribute data 
template <typename T>
class GA_RWHandleT<T, typename ENABLE_ARRAY(T)>
		: public GA_ROHandleT<T, typename ENABLE_ARRAY(T)>
{
public:
    typedef GA_ROHandleT<T> Base;

private:
    using Base::myAttrib;
public:

    GA_RWHandleT()
    {
	this->clear();
    }
    GA_RWHandleT(GA_Attribute *attrib)
    {
	bind(attrib);
    }
    GA_RWHandleT(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);

    void operator=(GA_Attribute *attrib)
    {
	bind(attrib);
    }

    void bind(GA_Detail *gdp, GA_AttributeOwner owner, const UT_StringRef &name, int minsize=1);
    void bind(GA_Attribute *attrib, int minsize=1)
    {
	Base::bind(attrib, minsize);
    }

    GA_Attribute *getAttribute() const { return Base::myAttrib; }

    void bumpDataId() const
    {
	if (myAttrib)
	    myAttrib->bumpDataId();
    }

    GA_DataId getDataId() const
    {
	if (myAttrib)
	    return myAttrib->getDataId();
	return GA_INVALID_DATAID;
    }

    SYS_FORCE_INLINE void set(GA_Offset off, const T &value) const
    {
        UT_ASSERT_P(myAttrib);
        UT_ASSERT_P(GAisValid(off));
        UT_ASSERT_P(myAttrib->getIndexMap().isOffsetActive(off) || myAttrib->getIndexMap().isOffsetTransient(off));
	Base::myAIF->set(myAttrib, off, value);
    }
    GA_Attribute *operator->() const
    {
        return myAttrib;
    }

private:
};

#undef ENABLE_ARRAY

template <typename T>
class GA_HandleT
{
public:
    typedef GA_ROHandleT<T>		ROType;
    typedef GA_RWHandleT<T>		RWType;
};

// Hacky workaround for now
template <>
class GA_HandleT<const char*>
{
public:
    typedef GA_ROHandleS		ROType;
    typedef GA_RWHandleS		RWType;
};

typedef GA_ROHandleT<fpreal>		GA_ROHandleR;
typedef GA_RWHandleT<fpreal>		GA_RWHandleR;

typedef GA_ROHandleT<fpreal16>		GA_ROHandleH;
typedef GA_RWHandleT<fpreal16>		GA_RWHandleH;

typedef GA_ROHandleT<fpreal32>		GA_ROHandleF;
typedef GA_RWHandleT<fpreal32>		GA_RWHandleF;

typedef GA_ROHandleT<fpreal64>		GA_ROHandleD;
typedef GA_RWHandleT<fpreal64>		GA_RWHandleD;

typedef GA_ROHandleT<int8>		GA_ROHandleC;
typedef GA_RWHandleT<int8>		GA_RWHandleC;

typedef GA_ROHandleT<int32>		GA_ROHandleI;
typedef GA_RWHandleT<int32>		GA_RWHandleI;

typedef GA_ROHandleT<UT_Matrix3F>	GA_ROHandleM3;
typedef GA_RWHandleT<UT_Matrix3F>	GA_RWHandleM3;

typedef GA_ROHandleT<UT_Matrix4F>	GA_ROHandleM4;
typedef GA_RWHandleT<UT_Matrix4F>	GA_RWHandleM4;

typedef GA_ROHandleT<UT_QuaternionF>	GA_ROHandleQ;
typedef GA_RWHandleT<UT_QuaternionF>	GA_RWHandleQ;

typedef GA_ROHandleT<UT_Vector2F>	GA_ROHandleV2;
typedef GA_RWHandleT<UT_Vector2F>	GA_RWHandleV2;

typedef GA_ROHandleT<UT_Vector3F>	GA_ROHandleV3;
typedef GA_RWHandleT<UT_Vector3F>	GA_RWHandleV3;

typedef GA_ROHandleT<UT_Vector4F>	GA_ROHandleV4;
typedef GA_RWHandleT<UT_Vector4F>	GA_RWHandleV4;

typedef GA_ROHandleT<int64>		GA_ROHandleID;
typedef GA_RWHandleT<int64>		GA_RWHandleID;

typedef GA_ROHandleT<UT_Matrix3D>	GA_ROHandleM3D;
typedef GA_RWHandleT<UT_Matrix3D>	GA_RWHandleM3D;

typedef GA_ROHandleT<UT_Matrix4D>	GA_ROHandleM4D;
typedef GA_RWHandleT<UT_Matrix4D>	GA_RWHandleM4D;

typedef GA_ROHandleT<UT_QuaternionD>	GA_ROHandleQD;
typedef GA_RWHandleT<UT_QuaternionD>	GA_RWHandleQD;

typedef GA_ROHandleT<UT_Vector2D>	GA_ROHandleV2D;
typedef GA_RWHandleT<UT_Vector2D>	GA_RWHandleV2D;

typedef GA_ROHandleT<UT_Vector3D>	GA_ROHandleV3D;
typedef GA_RWHandleT<UT_Vector3D>	GA_RWHandleV3D;

typedef GA_ROHandleT<UT_Vector4D>	GA_ROHandleV4D;
typedef GA_RWHandleT<UT_Vector4D>	GA_RWHandleV4D;

typedef GA_ROHandleT<UT_FprealArray>	GA_ROHandleRA;
typedef GA_RWHandleT<UT_FprealArray>	GA_RWHandleRA;

typedef GA_ROHandleT<UT_Int32Array> GA_ROHandleIA;
typedef GA_RWHandleT<UT_Int32Array> GA_RWHandleIA;

typedef GA_ROHandleT<UT_Int64Array> GA_ROHandleIDA;
typedef GA_RWHandleT<UT_Int64Array> GA_RWHandleIDA;

typedef GA_ROHandleT<UT_Fpreal32Array>  GA_ROHandleFA;
typedef GA_RWHandleT<UT_Fpreal32Array>  GA_RWHandleFA;

typedef GA_ROHandleT<UT_Fpreal64Array>  GA_ROHandleDA;
typedef GA_RWHandleT<UT_Fpreal64Array>  GA_RWHandleDA;

typedef GA_ROHandleT<UT_StringArray>	GA_ROHandleSA;
typedef GA_RWHandleT<UT_StringArray>	GA_RWHandleSA;

#endif
