/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PageHandle.h ( GA Library, C++)
 *
 * COMMENTS:	A heavyweight probe to deal with sequential access of
 *		a specific attribute of specific type, where one can
 *		either get a marshalled or raw pointer to underlying data.
 */

#ifndef __GA_PAGEHANDLE_H_INCLUDED__
#define __GA_PAGEHANDLE_H_INCLUDED__

#include "GA_API.h"
#include "GA_AIFTuple.h"
#include "GA_ATINumeric.h"
#include "GA_Attribute.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

#include <SYS/SYS_Types.h>


class GA_Detail;


template <typename T, typename TB, bool READ, bool WRITE, typename EXTERNAL_ATTRIB, typename INTERNAL_ATTRIB, typename DETAIL>
class GA_PageHandleT
{
public:
    typedef T		    value_type;

			    GA_PageHandleT()
			    {
				clear();
			    }
			    GA_PageHandleT(
				    EXTERNAL_ATTRIB *attrib,
				    int component = 0)
			    {
				clear();
				bind(attrib, component);
			    }
			    GA_PageHandleT(
				    DETAIL *gdp,
				    GA_AttributeOwner owner,
				    const char *name,
				    int component=0);

			    ~GA_PageHandleT()
			    {
				if (WRITE)
				    flushIfNeeded();
			    }

    void		    bind(EXTERNAL_ATTRIB *attrib, int component = 0);
    /// Bind unsafe does no checking of the attribute, assuming it
    /// is already a GA_ATINumeric.  
    void		    bindUnsafe(EXTERNAL_ATTRIB *attrib, int component = 0);
    void		    bind(DETAIL *gdp, GA_AttributeOwner owner, const char *name, int component = 0);

    /// Somewhat expensive as it must flush and rebind!
    /// It is preferred to have several page handles, one for each
    /// component.
    void		    setComponent(int comp)  { bind(myAttrib, comp); }

    EXTERNAL_ATTRIB	*   getAttribute() const    { return myAttrib; }
    EXTERNAL_ATTRIB 	*   operator->() const
    {
        return myAttrib;
    }

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


    void		    clear()
			    {
				myAttrib = 0;
				myDataPtr = 0;
				myBaseOffset = GA_INVALID_OFFSET;
				myIsConstant = false;
			    }

    /// Sets the default value
    /// If the attribute is unbound, this is the value that
    /// will be read.
    void		    setDefaultValue(T value)
			    {
				for (GA_PageOff i(0); i < GA_PAGE_SIZE; i++)
				    myMarshalledData[i] = value;
				myDataPtr = myMarshalledData;
				myIsConstant = true;
				// We now are a valid offset (but
				// we will still want to update for
				// setpage)
				myBaseOffset = GA_Offset(0);
			    }

    /// This returns if the page was constant when it was fetched.
    /// It may no longer be constant if someone wrote to it.
    SYS_FORCE_INLINE bool   isCurrentPageConstant() const
			    { return myIsConstant; }

    void		    makeConstant(T cval)
    {
	myAttrib->getAIFTuple()->makeConstant(myAttrib, (TB *)&cval, sizeof(T)/sizeof(TB), myComponent);
    }

    /// Sets all components of all elements of the specified page to
    /// the given values.
    /// NOTE: The length of values must be equal to the tuple size.
    void setPageConstant(GA_PageNum pagenum, T values)
    {
	myAttrib->getData().setPageConstant(pagenum, values.data());
	myIsConstant = true;
    }

    SYS_FORCE_INLINE bool   isValid() const	    { return myAttrib; }
    SYS_FORCE_INLINE bool   isInvalid() const	    { return !myAttrib; }

    void		    setPage(GA_Offset pagebase);

    inline T		    get(GA_Offset off) const
			    {
				return getRelative(GA_PageOff(off-myBaseOffset));
			    }
    inline T		    getRelative(GA_PageOff rel) const
			    {
				UT_ASSERT_P(rel >= 0 && rel < GA_PAGE_SIZE);
				return myDataPtr[rel];
			    }

    inline void		    set(GA_Offset off, T value)
			    {
				setRelative(GA_PageOff(off-myBaseOffset), value);
			    }
    inline void		    setRelative(GA_PageOff rel, T value)
			    {
				UT_ASSERT_P(rel >= 0 && rel < GA_PAGE_SIZE);
				myDataPtr[rel] = value;
			    }

    // Returns a reference so you can do things like:
    // page.value(offset) += 5
    inline T &		    value(GA_Offset off)
			    {
				return valueRelative(GA_PageOff(off-myBaseOffset));
			    }
    inline T &		    valueRelative(GA_PageOff rel)
			    {
				UT_ASSERT_P(rel >= 0 && rel < GA_PAGE_SIZE);
				return myDataPtr[rel];
			    }

    // Writes backout the page table if it was copied in.
    // Normally end users do not have to do this as it will be
    // done automatically when switching pages or destructing.
    void		    flushIfNeeded();

protected:
    T			    myMarshalledData[GA_PAGE_SIZE];
    T *			    myDataPtr;
    INTERNAL_ATTRIB *	    myAttrib;
    GA_Offset		    myBaseOffset;
    int			    myComponent;
    bool		    myIsConstant;
};

template <typename T>
class GA_PageHandleScalar
{
public:
    typedef T						value_type;

    typedef GA_PageHandleT<T, T, true, false, const GA_Attribute, const GA_ATINumeric, const GA_Detail> ROType;
    typedef GA_PageHandleT<T, T, true, true, GA_Attribute, GA_ATINumeric, GA_Detail>              RWType;
};

template <typename T>
class GA_PageHandleV
{
public:
    typedef T						value_type;

    typedef GA_PageHandleT<T, typename T::value_type, true, false, const GA_Attribute, const GA_ATINumeric, const GA_Detail> ROType;
    typedef GA_PageHandleT<T, typename T::value_type, true, true, GA_Attribute, GA_ATINumeric, GA_Detail>              RWType;
};

typedef GA_PageHandleScalar<int32>::ROType		GA_ROPageHandleI;
typedef GA_PageHandleScalar<int32>::RWType		GA_RWPageHandleI;
typedef GA_PageHandleScalar<int64>::ROType		GA_ROPageHandleID;
typedef GA_PageHandleScalar<int64>::RWType		GA_RWPageHandleID;

typedef GA_PageHandleScalar<fpreal>::ROType		GA_ROPageHandleR;
typedef GA_PageHandleScalar<fpreal>::RWType		GA_RWPageHandleR;

typedef GA_PageHandleScalar<fpreal16>::ROType		GA_ROPageHandleH;
typedef GA_PageHandleScalar<fpreal16>::RWType		GA_RWPageHandleH;

typedef GA_PageHandleScalar<fpreal32>::ROType		GA_ROPageHandleF;
typedef GA_PageHandleScalar<fpreal32>::RWType		GA_RWPageHandleF;

typedef GA_PageHandleScalar<fpreal64>::ROType		GA_ROPageHandleD;
typedef GA_PageHandleScalar<fpreal64>::RWType		GA_RWPageHandleD;

typedef GA_PageHandleV<UT_Vector2F>::ROType		GA_ROPageHandleV2;
typedef GA_PageHandleV<UT_Vector2F>::RWType		GA_RWPageHandleV2;
typedef GA_PageHandleV<UT_Vector2D>::ROType		GA_ROPageHandleV2D;
typedef GA_PageHandleV<UT_Vector2D>::RWType		GA_RWPageHandleV2D;

typedef GA_PageHandleV<UT_Vector3F>::ROType		GA_ROPageHandleV3;
typedef GA_PageHandleV<UT_Vector3F>::RWType		GA_RWPageHandleV3;
typedef GA_PageHandleV<UT_Vector3D>::ROType		GA_ROPageHandleV3D;
typedef GA_PageHandleV<UT_Vector3D>::RWType		GA_RWPageHandleV3D;

typedef GA_PageHandleV<UT_Vector4F>::ROType		GA_ROPageHandleV4;
typedef GA_PageHandleV<UT_Vector4F>::RWType		GA_RWPageHandleV4;
typedef GA_PageHandleV<UT_Vector4D>::ROType		GA_ROPageHandleV4D;
typedef GA_PageHandleV<UT_Vector4D>::RWType		GA_RWPageHandleV4D;

typedef GA_PageHandleV<UT_QuaternionF>::ROType		GA_ROPageHandleQ;
typedef GA_PageHandleV<UT_QuaternionF>::RWType		GA_RWPageHandleQ;
typedef GA_PageHandleV<UT_QuaternionD>::ROType		GA_ROPageHandleQD;
typedef GA_PageHandleV<UT_QuaternionD>::RWType		GA_RWPageHandleQD;

typedef GA_PageHandleV<UT_Matrix2F>::ROType		GA_ROPageHandleM2;
typedef GA_PageHandleV<UT_Matrix2F>::RWType		GA_RWPageHandleM2;
typedef GA_PageHandleV<UT_Matrix2D>::ROType		GA_ROPageHandleM2D;
typedef GA_PageHandleV<UT_Matrix2D>::RWType		GA_RWPageHandleM2D;

typedef GA_PageHandleV<UT_Matrix3F>::ROType		GA_ROPageHandleM3;
typedef GA_PageHandleV<UT_Matrix3F>::RWType		GA_RWPageHandleM3;
typedef GA_PageHandleV<UT_Matrix3D>::ROType		GA_ROPageHandleM3D;
typedef GA_PageHandleV<UT_Matrix3D>::RWType		GA_RWPageHandleM3D;

typedef GA_PageHandleV<UT_Matrix4F>::ROType		GA_ROPageHandleM4;
typedef GA_PageHandleV<UT_Matrix4F>::RWType		GA_RWPageHandleM4;
typedef GA_PageHandleV<UT_Matrix4D>::ROType		GA_ROPageHandleM4D;
typedef GA_PageHandleV<UT_Matrix4D>::RWType		GA_RWPageHandleM4D;


///////////////////////////////////////////////////////////////////////////////
// 
// GA_PageHandleT implementation
//

template <typename T, typename TB, bool READ, bool WRITE, typename EXTERNAL_ATTRIB, typename INTERNAL_ATTRIB, typename DETAIL>
inline void
GA_PageHandleT<T, TB, READ, WRITE, EXTERNAL_ATTRIB, INTERNAL_ATTRIB, DETAIL>::bind(
	EXTERNAL_ATTRIB *attrib,
	int component)
{
    if (WRITE)
	flushIfNeeded();
    clear();
    myAttrib = INTERNAL_ATTRIB::cast(attrib);
    if (!myAttrib)
	return;

    if (myAttrib->getTupleSize() < ((1+component) * sizeof(T)/sizeof(TB)))
    {
	myAttrib = 0;
	return;
    }
    myComponent = component;
}

template <typename T, typename TB, bool READ, bool WRITE, typename EXTERNAL_ATTRIB, typename INTERNAL_ATTRIB, typename DETAIL>
inline void
GA_PageHandleT<T, TB, READ, WRITE, EXTERNAL_ATTRIB, INTERNAL_ATTRIB, DETAIL>::bindUnsafe(
	EXTERNAL_ATTRIB *attrib,
	int component)
{
    if (WRITE)
	flushIfNeeded();
    clear();

    myAttrib = (INTERNAL_ATTRIB *)(attrib);
    myComponent = component;
}

template <typename T, typename TB, bool READ, bool WRITE, typename EXTERNAL_ATTRIB, typename INTERNAL_ATTRIB, typename DETAIL>
inline void
GA_PageHandleT<T, TB, READ, WRITE, EXTERNAL_ATTRIB, INTERNAL_ATTRIB, DETAIL>::setPage(GA_Offset pagebase)
{
    // Check if this is the same page as before.
    if (myDataPtr
	&& pagebase >= myBaseOffset
	&& (pagebase < myBaseOffset + GA_PAGE_SIZE))
    {
	return;
    }

    if (!isValid())
    {
	// We still want to set our page offset with invalid pages
	// so default values will work
	pagebase -= GAgetPageOff(pagebase);
	myBaseOffset = pagebase;
	return;
    }

    if (WRITE)
	flushIfNeeded();

    // Set our new base.
    pagebase -= GAgetPageOff(pagebase);
    myBaseOffset = pagebase;
    
    // Read in our data.
    GA_PageNum pagenum = GAgetPageNum(pagebase);
    auto &src = myAttrib->getData();
    if (myComponent == 0 && UT_StorageNum<TB>::theStorage == src.GA_ATINumeric::DataType::Base::getStorage()
        && (sizeof(T) / sizeof(TB)) == src.getTupleSize())
    {
        if (WRITE)
        {
            GA_PageArray<TB,(sizeof(T) / sizeof(TB))> &matchsrc = SYSconst_cast(src).template castTupleSize<sizeof(T)/sizeof(TB)>().template castType<TB>();

            matchsrc.hardenPage(pagenum);
            myDataPtr = reinterpret_cast<T*>(SYSconst_cast(matchsrc.getPageData(pagenum)));
            myIsConstant = false;
        }
        else
        {
            const GA_PageArray<TB,(sizeof(T) / sizeof(TB))> &matchsrc = src.template castTupleSize<sizeof(T)/sizeof(TB)>().template castType<TB>();

            if (matchsrc.isPageConstant(pagenum))
            {
                T v;
                const T *data = reinterpret_cast<const T*>(matchsrc.getPageData(pagenum));

                // Can skip if already marshalled same value last time
                bool equal = myIsConstant;
                if (equal)
                {
                    if (!data)
                    {
                        memset(&v,0,sizeof(T));
                        data = &v;
                    }
                    equal = (myMarshalledData[0] == *data);
                }
                UT_ASSERT_P(!equal || myDataPtr == myMarshalledData);
                if (!equal)
                {
                    GA_Size count = ((pagebase + GA_PAGE_SIZE) <= matchsrc.size()) ? GA_PAGE_SIZE : (matchsrc.size() & GA_PAGE_MASK);
                    if (!data)
                    {
                        memset(myMarshalledData, 0, count*sizeof(myMarshalledData[0]));
                    }
                    else
                    {
                        for (GA_Size off = 0; off < count; ++off)
                            myMarshalledData[off] = *data;
                    }
                    myDataPtr = myMarshalledData;
                    myIsConstant = true;
                }
            }
            else
            {
                myDataPtr = reinterpret_cast<T*>(SYSconst_cast(matchsrc.getPageData(pagenum)));
                myIsConstant = false;
            }
        }
    }
    else
    {
        GA_Size count = ((pagebase + GA_PAGE_SIZE) <= src.size()) ? GA_PAGE_SIZE : (src.size() & GA_PAGE_MASK);
        if (myComponent <= 0)
        {
            for (GA_Size off = 0; off < count; ++off)
            {
                typename UT_FixedVectorTraits<T>::FixedVectorType v = src.template getVector<TB, sizeof(T) / sizeof(TB)>(pagebase + off);
                myMarshalledData[off] = *reinterpret_cast<const T*>(&v);
            }
        }
        else
        {
            UT_ASSERT_MSG_P((SYSisSame<T,TB>()), "Don't currently support vector types when myComponent > 0.");
            for (GA_Size off = 0; off < count; ++off)
            {
                TB v = src.template get<TB>(pagebase + off, myComponent);
                myMarshalledData[off] = *reinterpret_cast<const T*>(&v);
            }
        }
        myDataPtr = myMarshalledData;
        myIsConstant = src.isPageConstant(pagenum);
    }
}

#endif // __GA_PAGEHANDLE_H_INCLUDED__
