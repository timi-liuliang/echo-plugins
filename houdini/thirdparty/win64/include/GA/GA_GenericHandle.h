/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GenericHandle.h ( GA Library, C++)
 *
 * COMMENTS:	A set of classes for writing generic algorithms that operate
 *		either on all attributes (using GA_Offset and
 *		GA_AttributeRefMap) or on a floating point tuple (using
 *		float, UT_Vector3, UT_Vector4, UT_Matrix3, UT_Matrix4).
 *		Algorithms must be written using the set of arithmetic
 *		operations defined in GA_GenericMath, which must also match
 *		those in GA_AttributeRefMapDestHandle.
 *
 *		The classes are:
 *		- GA_GenericHandle
 *		- GA_GenericMath
 *		- GA_GenericTemp
 */

#ifndef __GA_GenericHandle__
#define __GA_GenericHandle__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeRefMapDestHandle.h"
#include "GA_Detail.h"
#include "GA_Handle.h"
#include "GA_PwHandle.h"
#include "GA_Types.h"
#include "GA_WeightedSum.h"
#include "GA_WorkVertexBuffer.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Vector4.h>

#include <SYS/SYS_Types.h>


class GA_AttributeRefMap;


/// Homogenize/dehomogenize that only operate on UT_Vector4
namespace GA_Homogenize
{
    template <typename T> static inline void homogenize(T &val) {}
    template <> inline void homogenize<UT_Vector4>(UT_Vector4 &val)
    { val.homogenize(); }

    template <typename T> static inline void dehomogenize(T &val) {}
    template <> inline void dehomogenize<UT_Vector4>(UT_Vector4 &val)
    { val.dehomogenize(); }
}

/// Read-only generic handle.
///
/// The handle provides read-only access to attributedata.
/// The @c T template parameter can be specialized with
///  - float
///  - UT_Vector2
///  - UT_Vector3
///  - UT_Vector4 (see specializations below)
///  - UT_Matrix3
///  - UT_Matrix4
///  - GA_Offset (see specializations below)
/// The @c T_OWNER parameter can be specialized with
///  - GA_ATTRIB_POINT @n
///	Point offsets will be passeed to math.getValue().  Only point
///	attributes will be modified for GA_Offset specializations.
///  - GA_ATTRIB_VERTEX
///	Vertex offsets must be passed to math.getValue().  Both point and
///	vertex attributes will be modified with the GA_Offset specialization.
///
/// The offsets passed to @c getValue() must match the @c T_OWNER template
/// parameter.
template <typename T, GA_AttributeOwner T_OWNER>
class GA_ROGenericHandle
{
public:
    GA_ROGenericHandle(const GA_Attribute *attr, int component=0, bool forceisp=false)
	: myHandle(SYSconst_cast(attr))
	, myComponent(component)
	, myConst(true)
        , myIsRational(attr->getTypeInfo() == GA_TYPE_HPOINT ||
            ((attr == attr->getDetail().getP() || forceisp)
             && attr->getDetail().getPwAttribute()))
        , myIsForcedP(forceisp)
    {}

    const GA_Attribute	*getAttribute() const
			 { return myHandle.getAttribute(); }
    bool		 isRational() const
			 { return myIsRational; }

    T	 getValue(GA_Offset off) const
	 {
             UT_ASSERT_P(myHandle.isValid());
	     const GA_Attribute *attr = myHandle.getAttribute();
	     T	val;
	     if (T_OWNER == GA_ATTRIB_POINT)
	     {
		 UT_ASSERT_P(attr->getOwner() == GA_ATTRIB_POINT);
		 val = myHandle.get(off, myComponent);
	     }
	     else if (attr->getOwner() == GA_ATTRIB_POINT)
	     {
		 val = myHandle.get(attr->getDetail().vertexPoint(off),
			 myComponent);
	     }
	     else
	     {
		 val = myHandle.get(off, myComponent);
	     }
	     return val;
	 }
    T	 getHomogeneous(GA_Offset off) const
	 {
             // Only specialization for UT_Vector4 needs homogenization.
	     return getValue(off);
	 }
        bool isForcedP() const
        { return myIsForcedP; }

protected:
    GA_RWHandleT<T>	 myHandle;
    int			 myComponent;
    bool		 myConst;
    bool                 myIsRational;
    bool                 myIsForcedP;
};

/// Specialization of GA_ROGenericHandle for GA_ATTRIB_POINT offsets
template <typename T>
class GA_ROGenericHandlePoint
    : public GA_ROGenericHandle<T, GA_ATTRIB_POINT>
{
public:
    GA_ROGenericHandlePoint(const GA_Attribute *attr, int component=0)
	: GA_ROGenericHandle<T, GA_ATTRIB_POINT>(attr, component)
    {
    }
};

/// Specialization of GA_ROGenericHandle for GA_ATTRIB_VERTEX offsets
template <typename T>
class GA_ROGenericHandleVertex
    : public GA_ROGenericHandle<T, GA_ATTRIB_VERTEX>
{
public:
    GA_ROGenericHandleVertex(const GA_Attribute *attr, int component=0)
	: GA_ROGenericHandle<T, GA_ATTRIB_VERTEX>(attr, component)
    {
    }
    GA_ROGenericHandleVertex(const GA_Attribute *attr, bool forceisp)
	: GA_ROGenericHandle<T, GA_ATTRIB_VERTEX>(attr, 0, forceisp)
    {
    }
};

/// Read-Write access to generic attribute data
///
/// Like GA_ROGenericHandle, but also provides a method to write to the
/// attribute.  The offsets passed to set value must match the @c T_OWNER
/// template parameter.
template <typename T, GA_AttributeOwner T_OWNER>
class GA_RWGenericHandle
    : public GA_ROGenericHandle<T, T_OWNER>
{
public:
    typedef GA_ROGenericHandle<T, T_OWNER>	Base;

    GA_RWGenericHandle(GA_Attribute *attr, int component=0)
	: GA_ROGenericHandle<T, T_OWNER>(attr, component)
    {
	Base::myConst = false;
    }

    void setValue(GA_Offset off, const T &val) const
	 {
	     UT_ASSERT(!Base::myConst);
	     const GA_Attribute	*attr = Base::myHandle.getAttribute();
	     if (T_OWNER == GA_ATTRIB_POINT)
	     {
		 UT_ASSERT_P(attr->getOwner() == GA_ATTRIB_POINT);
		 Base::myHandle.set(off, Base::myComponent, val);
	     }
	     else if (attr->getOwner() == GA_ATTRIB_POINT)
	     {
		 GA_Offset	ptoff = attr->getDetail().vertexPoint(off);
		 Base::myHandle.set(ptoff, Base::myComponent, val);
	     }
	     else
	     {
		 Base::myHandle.set(off, Base::myComponent, val);
	     }
	 }
};

/// Specialization of GA_RWGenericHandle for GA_ATTRIB_POINT offsets
template <typename T>
class GA_RWGenericHandlePoint
    : public GA_RWGenericHandle<T, GA_ATTRIB_POINT>
{
public:
    GA_RWGenericHandlePoint(GA_Attribute *attr, int component=0)
	: GA_RWGenericHandle<T, GA_ATTRIB_POINT>(attr, component)
    {
    }
};

/// Specialization of GA_RWGenericHandle for GA_ATTRIB_VERTEX offsets
template <typename T>
class GA_RWGenericHandleVertex
    : public GA_RWGenericHandle<T, GA_ATTRIB_VERTEX>
{
public:
    GA_RWGenericHandleVertex(GA_Attribute *attr, int component=0)
	: GA_RWGenericHandle<T, GA_ATTRIB_VERTEX>(attr, component)
    {
    }
};

/// Template math for operating on generic handles
///
/// This template is specialized to work on @c float, @c UT_Vector3, @c
/// UT_Vector4, and other POD data types.
///
/// The offsets passed in to methods must match the @c T_OWNER template
/// parameter.
template <typename T, GA_AttributeOwner T_OWNER>
class GA_GenericMath
{
public:
    GA_GenericMath(const GA_ROGenericHandle<T, T_OWNER> &h)
	: myHandle(h) {}

    bool	setElement(T &val)
		{ myDest = &val; return true; }

    // Source/destination operations
    void	copy(GA_Offset source_index) const
		{ *myDest = myHandle.getValue(source_index); }
    void	add(GA_WeightedSum &sum,
			GA_Offset source_index,
			fpreal w) const
		{
		    sum.advance(w);
		    *myDest += myHandle.getValue(source_index)*w;
		}
    void	addH(GA_WeightedSum &sum,
			GA_Offset source_index,
			fpreal w) const
		{
		    sum.advance(w);
		    *myDest += myHandle.getHomogeneous(source_index)*w;
		}

    // Weighted sum
    void	startSum(const GA_WeightedSum &sum) const
		{ *myDest = 0; }
    void	startHSum(const GA_WeightedSum &sum) const
		{ *myDest = 0; }
    void	finishSum(const GA_WeightedSum &sum,
			fpreal normalization=1) const
		{
		    *myDest *= normalization;
		}
    void	finishHSum(const GA_WeightedSum &sum,
			fpreal normalization=1) const
		{
		    *myDest *= normalization;
		}

    // Comparsion
    bool	isEqual(const T &val) const
		    { return *myDest == val; }
    bool	isAlmostEqual(const T &val) const
		    { return *myDest == val; }

    // Destination operations
    void	zero() const { *myDest = 0; }

    void	multiply(fpreal scale) const
		    { *myDest *= scale; }

    void	copyDest(T val) const { *myDest = val; }
    void	addDest(T val) const { *myDest += val; }

    void	subDest(T val) const { *myDest -= val; }
    void	lerpDest(T s0, T s1, fpreal t) const
		{ *myDest = s0 + (s1-s0)*t; }

    void	addDest(GA_WeightedSum &sum, T val, fpreal w) const
		{
		    sum.advance(w);
		    *myDest += val*w;
		}
    void	addHDest(GA_WeightedSum &sum, T val, fpreal w) const
		{
		    sum.advance(w);
		    *myDest += GA_Homogenize::homogenize(val)*w;
		}
    void	dehomogenizeData(T *data, int size)
		{
		    if (myHandle.isRational())
		    {
			for (int i = 0; i < size; i++)
			    GA_Homogenize::dehomogenize(data[i]);
		    }
		}

private:
    const GA_ROGenericHandle<T, T_OWNER>	&myHandle;
    T						*myDest;
};

/// Specialization of GA_GenericMath for GA_ATTRIB_POINT offsets
template <typename T>
class GA_GenericMathPoint
    : public GA_GenericMath<T, GA_ATTRIB_POINT>
{
public:
    GA_GenericMathPoint(const GA_ROGenericHandlePoint<T> &h)
	: GA_GenericMath<T, GA_ATTRIB_POINT>(h)
    {
    }
};

/// Specialization of GA_GenericMath for GA_ATTRIB_VERTEX offsets
template <typename T>
class GA_GenericMathVertex
    : public GA_GenericMath<T, GA_ATTRIB_VERTEX>
{
public:
    GA_GenericMathVertex(const GA_ROGenericHandleVertex<T> &h)
	: GA_GenericMath<T, GA_ATTRIB_VERTEX>(h)
    {
    }
};

/// Template class to construct temporary objects
///
/// The default does no initialization on objects and is suitable for POD data.
template <typename T, GA_AttributeOwner T_OWNER>
class GA_GenericTemp
{
public:
    GA_GenericTemp(const GA_ROGenericHandle<T, T_OWNER> &) {}
    void	appendVertices(T *, int) {}
    void	clear() {}
};

/// Specialization of GA_GenericTemp for GA_ATTRIB_POINT types
template <typename T>
class GA_GenericTempPoint
    : public GA_GenericTemp<T, GA_ATTRIB_POINT>
{
public:
    GA_GenericTempPoint(const GA_ROGenericHandlePoint<T> &h)
	: GA_GenericTemp<T, GA_ATTRIB_POINT>(h)
    {}
};

/// Specialization of GA_GenericTemp for GA_ATTRIB_POINT types
template <typename T>
class GA_GenericTempVertex
    : public GA_GenericTemp<T, GA_ATTRIB_VERTEX>
{
public:
    GA_GenericTempVertex(const GA_ROGenericHandleVertex<T> &h)
	: GA_GenericTemp<T, GA_ATTRIB_VERTEX>(h)
    {}
};

// ----------------------------------------------------------
// Specializations for GA_Offset
// ----------------------------------------------------------

/// Specialization of GA_ROGenericHandle for GA_Offset
template <GA_AttributeOwner T_OWNER>
class GA_ROGenericHandle<GA_Offset, T_OWNER>
{
public:
    GA_ROGenericHandle(GA_AttributeRefMap &map, int component)
	: myMap(map)
    {
	 UT_ASSERT(component == 0 &&
		    "component operations not supported on GA_Offset handles");
    }

    void		 setValue(GA_Offset, const GA_Offset &) const;
    const GA_Attribute	*getAttribute() const	{ return 0; }
    bool		 isRational() const	{ return false; }
    bool                 isForcedP() const { return false; }
    GA_AttributeRefMap	&getMap() const	{ return myMap; }

private:
    GA_AttributeRefMap	&myMap;
};

/// Specialization of GA_ROGenericHandlePoint for GA_Offset
template <>
class GA_ROGenericHandlePoint<GA_Offset>
    : public GA_ROGenericHandle<GA_Offset, GA_ATTRIB_POINT>
{
public:
    GA_ROGenericHandlePoint(GA_AttributeRefMap &map, int component=0)
	: GA_ROGenericHandle<GA_Offset, GA_ATTRIB_POINT>(map, component)
    { }
};

/// Specialization of GA_ROGenericHandleVertex for GA_Offset
template <>
class GA_ROGenericHandleVertex<GA_Offset>
    : public GA_ROGenericHandle<GA_Offset, GA_ATTRIB_VERTEX>
{
public:
    GA_ROGenericHandleVertex(GA_AttributeRefMap &map, int component=0)
	: GA_ROGenericHandle<GA_Offset, GA_ATTRIB_VERTEX>(map, component)
    { }
};

/// Specialization of GA_RWGenericHandle for GA_Offset
template <GA_AttributeOwner T_OWNER>
class GA_RWGenericHandle<GA_Offset, T_OWNER>
    : public GA_ROGenericHandle<GA_Offset, T_OWNER>
{
public:
    typedef GA_RWGenericHandle<GA_Offset, T_OWNER>	Base;

    GA_RWGenericHandle(GA_AttributeRefMap &map, int component=0)
	: GA_ROGenericHandle<GA_Offset, T_OWNER>(map, component)
    {
    }

    void	 setValue(GA_Offset dest, const GA_Offset &src) const
		 {
		     if (T_OWNER == GA_ATTRIB_POINT)
		     {
			 Base::getMap().copyValue(GA_ATTRIB_POINT, dest,
				 GA_ATTRIB_POINT, src);
		     }
		     else
		     {
			 Base::getMap().copyValue(GA_ATTRIB_VERTEX, dest,
				    GA_ATTRIB_VERTEX, src);
		     }
		 }
private:
};

/// Specialization of GA_GenericMath for GA_Offset
template <GA_AttributeOwner T_OWNER>
class GA_GenericMath<GA_Offset, T_OWNER>
    : public GA_AttributeRefMapDestHandle<T_OWNER>
{
public:
    typedef GA_AttributeRefMapDestHandle<T_OWNER>	Base;

    GA_GenericMath(const GA_ROGenericHandle<GA_Offset, T_OWNER> &h)
	: GA_AttributeRefMapDestHandle<T_OWNER>(h.getMap()) {}

    void	dehomogenizeData(GA_Offset *data, int size)
		{
		    for (int i = 0; i < size; i++)
		    {
			Base::setElement(data[i]);
			Base::dehomogenize();
		    }
		}
};

/// Specialization of GA_GenericMathPoint for GA_Offset
template <>
class GA_GenericMathPoint<GA_Offset>
    : public GA_GenericMath<GA_Offset, GA_ATTRIB_POINT>
{
public:
    GA_GenericMathPoint(const GA_ROGenericHandlePoint<GA_Offset> &h)
	: GA_GenericMath<GA_Offset, GA_ATTRIB_POINT>(h)
    {
    }
};

/// Specialization of GA_GenericMathVertex for GA_Offset
template <>
class GA_GenericMathVertex<GA_Offset>
    : public GA_GenericMath<GA_Offset, GA_ATTRIB_VERTEX>
{
public:
    GA_GenericMathVertex(const GA_ROGenericHandleVertex<GA_Offset> &h)
	: GA_GenericMath<GA_Offset, GA_ATTRIB_VERTEX>(h)
    {
    }
};


/// Specialization of GA_GenericTemp for GA_Offset
template <GA_AttributeOwner T_OWNER>
class GA_GenericTemp<GA_Offset, T_OWNER>
{
public:
    GA_GenericTemp(const GA_ROGenericHandle<GA_Offset, T_OWNER> &gah)
	: myBuffer(gah.getMap().getVertexPool()) {}

    void	appendVertices(GA_Offset *data, int size)
		{
		    if (T_OWNER == GA_ATTRIB_POINT)
			myBuffer.appendPoints(data, size);
		    else
			myBuffer.appendVertices(data, size);
		}
    void	clear()
		    { myBuffer.clear(); }

private:
    GA_WorkVertexBuffer	myBuffer;
};

/// Specialization of GA_GenericTempPoint for GA_Offset
template <>
class GA_GenericTempPoint<GA_Offset>
    : public GA_GenericTemp<GA_Offset, GA_ATTRIB_POINT>
{
public:
    GA_GenericTempPoint(const GA_ROGenericHandlePoint<GA_Offset> &h)
	: GA_GenericTemp<GA_Offset, GA_ATTRIB_POINT>(h)
    {}
};

/// Specialization of GA_GenericTempVertex for GA_Offset
template <>
class GA_GenericTempVertex<GA_Offset>
    : public GA_GenericTemp<GA_Offset, GA_ATTRIB_VERTEX>
{
public:
    GA_GenericTempVertex(const GA_ROGenericHandleVertex<GA_Offset> &h)
	: GA_GenericTemp<GA_Offset, GA_ATTRIB_VERTEX>(h)
    {}
};

/// Specialization for UT_Vector4, so that (P,Pw) attribute pair will work.
template <GA_AttributeOwner T_OWNER>
class GA_ROGenericHandle<UT_Vector4, T_OWNER>
{
public:
    GA_ROGenericHandle(const GA_Attribute *attr, int component=0, bool forceisp=false)
        : myHandle(SYSconst_cast(attr))
        , myConst(true)
        , myIsRational(attr->getTypeInfo() == GA_TYPE_HPOINT ||
            ((attr == attr->getDetail().getP() || forceisp)
             && attr->getDetail().getPwAttribute()))
        , myIsForcedP(forceisp)
    {
        UT_ASSERT_P(component == 0);
        if (!myHandle.isValid() && attr && (attr == attr->getDetail().getP() || forceisp))
            myPwHandle = GA_PwHandleRW(SYSconst_cast(attr), SYSconst_cast(attr->getDetail().getPwAttribute()));
    }

    const GA_Attribute *getAttribute() const
    { return myHandle.isValid() ? myHandle.getAttribute() : myPwHandle.getAttribute(); }
    bool isRational() const
    { return myIsRational; }
    bool isValid() const
    { return getAttribute() != NULL; }

    UT_Vector4 getValue(GA_Offset off) const
    {
        UT_ASSERT_P(myHandle.isValid() != myPwHandle.isValid());
        const GA_Attribute *attr = getAttribute();
        UT_Vector4 val;
        if (T_OWNER == GA_ATTRIB_POINT)
        {
            UT_ASSERT_P(attr->getOwner() == GA_ATTRIB_POINT);
            val = myHandle.isValid() ? myHandle.get(off)
                                     : myPwHandle.get(off);
        }
        else if (attr->getOwner() == GA_ATTRIB_POINT)
        {
            GA_Offset ptoff = attr->getDetail().vertexPoint(off);
            val = myHandle.isValid()
                ? myHandle.get(ptoff)
                : myPwHandle.get(ptoff);
        }
        else
        {
            val = myHandle.isValid() ? myHandle.get(off)
                                     : myPwHandle.get(off);
        }
        return val;
    }
    UT_Vector4 getHomogeneous(GA_Offset off) const
    {
        UT_Vector4 val = getValue(off);
        if (isRational())
            val.homogenize();
        return val;
    }
    bool isForcedP() const
    { return myIsForcedP; }
protected:
    GA_RWHandleT<UT_Vector4> myHandle;
    bool                     myConst;
    bool                     myIsRational;
    bool                     myIsForcedP;
    GA_PwHandleRW            myPwHandle;
};

template <GA_AttributeOwner T_OWNER>
class GA_RWGenericHandle<UT_Vector4, T_OWNER>
    : public GA_ROGenericHandle<UT_Vector4, T_OWNER>
{
public:
    typedef GA_ROGenericHandle<UT_Vector4, T_OWNER> Base;
protected:
    using Base::myHandle;
    using Base::myPwHandle;
public:

    GA_RWGenericHandle(GA_Attribute *attr, int component=0)
        : Base(attr, component)
    {
        Base::myConst = false;
    }

    void setValue(GA_Offset off, const UT_Vector4 &val) const
    {
        UT_ASSERT_P(myHandle.isValid() != myPwHandle.isValid());
        UT_ASSERT(!Base::myConst);
        const GA_Attribute *attr = Base::getAttribute();
        if (T_OWNER == GA_ATTRIB_POINT)
        {
            UT_ASSERT_P(attr->getOwner() == GA_ATTRIB_POINT);
            if (myHandle.isValid())
                myHandle.set(off, val);
            else
                myPwHandle.set(off, val);
        }
        else if (attr->getOwner() == GA_ATTRIB_POINT)
        {
            GA_Offset ptoff = attr->getDetail().vertexPoint(off);
            if (myHandle.isValid())
                myHandle.set(ptoff, val);
            else
                myPwHandle.set(ptoff, val);
        }
        else
        {
            if (myHandle.isValid())
                myHandle.set(off, val);
            else
                myPwHandle.set(off, val);
        }
    }
};

#endif
