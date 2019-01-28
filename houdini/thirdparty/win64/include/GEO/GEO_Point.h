/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GEO_Point_H__
#define __GEO_Point_H__

#include "GEO_API.h"
#include <GA/GA_GBElement.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_Vector4.h>

class GA_Detail;
class GEO_Primitive;

SYS_DEPRECATED_PUSH_DISABLE()

/// Base class for GEO_Point.
///
class GEO_API GA_GBPoint : public GA_GBElement
{
public:
    /// Protect the constructors as this is only a base class and cannot itself
    /// be instantiated.
     GA_GBPoint(const GA_IndexMap &index_map, GA_Offset offset)
	: GA_GBElement(index_map, offset) { }
     GA_GBPoint(const GA_GBPoint &src)
	: GA_GBElement(src) { }
    GA_GBPoint()
        : GA_GBElement()
    {}

    SYS_DEPRECATED_HDK(13.0)
    GA_GBPoint	&operator=(const GA_GBPoint &src)
		{
		    GA_GBElement::operator=(src);
		    return *this;
		}

     /// We protect the destructor as nobody has any business using a reference
     /// to this class to delete an instance of a class derived from us.
    ~GA_GBPoint() {}

public:
    // --------------------------------------------------------------------
    // 
    // Methods from GEO_Point
    //
    // --------------------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    void	copyPoint(const GA_GBElement &src,
			  const GA_AttributeSet &src_dict,
			  const GA_AttributeSet &dest_dict)
		{
		    copyAttribData(src, src_dict, dest_dict, true);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyPoint(const GA_GBElement &src,
			  const GA_AttributeDict &src_dict,
			  const GA_AttributeDict &dest_dict)
		{
		    copyAttribData(src, src_dict.getSet(), dest_dict.getSet(), true);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyPoint(const GA_GBElement &src, const GA_AttributeSet &dict)
		{
		    copyAttribData(src, dict, true);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyPoint(const GA_GBElement &src, const GA_AttributeDict &dict)
		{
		    copyAttribData(src, dict.getSet(), true);
		}


    // Methods which don't have any support in GA
    // int		saveASCII(ostream &os) const;
    // int		saveBinary(ostream &os) const;
    // bool		load(UT_IStream &is);

protected:
    /// @{
    /// Get de-homogenized position (that is, without the homogeneous coordinate
    /// multiplied through).
    const UT_Vector4	getPos3D() const;
    const UT_Vector3	getPos2D() const;
    const UT_Vector2	getPos2() const;
    float		getPw() const;
    /// @}

    void		setPos3D(const UT_Vector4 &P);
    void		setPos3D(const UT_Vector3 &P);
    void		setPos3D(const UT_Vector3 &P, float w);
    void		setPos3D(float x, float y, float z, float w);

    void		setPos2D(const UT_Vector3 &P);
    void		setPos2D(const UT_Vector2 &P);
    void		setPos2D(const UT_Vector2 &P, float w);
    void		setPos2D(float x, float y, float w);

    void		setPw(float w);
};

inline const UT_Vector4
GA_GBPoint::getPos3D() const
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    return getDetail().getPos4(getMapOffset());
}
inline const UT_Vector3
GA_GBPoint::getPos2D() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    return getDetail().getPos3(getMapOffset());
}
inline const UT_Vector2
GA_GBPoint::getPos2() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    return getDetail().getPos2(getMapOffset());
}
inline float
GA_GBPoint::getPw() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    return getDetail().getPw(getMapOffset());
}
inline void
GA_GBPoint::setPos3D(const UT_Vector4 &P)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos4(getMapOffset(), P);
}
inline void
GA_GBPoint::setPos3D(const UT_Vector3 &P)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos3(getMapOffset(), P);
}
inline void
GA_GBPoint::setPos3D(const UT_Vector3 &P, float w)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    UT_Vector4		P4(P.x(), P.y(), P.z(), w);
    getDetail().setPos4(getMapOffset(), P4);
}
inline void
GA_GBPoint::setPos3D(float x, float y, float z, float w)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    UT_Vector4		P4(x, y, z, w);
    getDetail().setPos4(getMapOffset(), P4);
}
inline void
GA_GBPoint::setPos2D(const UT_Vector3 &P)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos3(getMapOffset(), P);
}
inline void
GA_GBPoint::setPos2D(const UT_Vector2 &P)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos2(getMapOffset(), P);
}
inline void
GA_GBPoint::setPos2D(const UT_Vector2 &P, float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos3(getMapOffset(), P.x(), P.y(), w);
}
inline void
GA_GBPoint::setPos2D(float x, float y, float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPos3(getMapOffset(), x, y, w);
}
inline void
GA_GBPoint::setPw(float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_POINT);
    getDetail().setPw(getMapOffset(), w);
}

/// DO NOT USE THIS CLASS!!!
class GEO_API GA_GBPointP
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP()
        : myPoint()
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP(const GA_Detail *gdp, GA_Offset offset)
        : myPoint(gdp->getPointMap(), offset)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP(const GA_Detail &gdp, GA_Offset offset)
        : myPoint(gdp.getPointMap(), offset)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP(const GA_GBPointP &that)
        : myPoint(that.myPoint)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP(const GA_GBPoint *that)
        : myPoint()
    {
        if (that)
            myPoint = GA_GBPoint(*that);
    }
    SYS_DEPRECATED_HDK(13.0)
    bool operator!() const
    {
        return !myPoint.isGAValid();
    }
    SYS_DEPRECATED_HDK(13.0)
    operator bool() const
    {
        return myPoint.isGAValid();
    }
    SYS_DEPRECATED_HDK(13.0)
    operator GA_GBPoint() const
    {
        return myPoint;
    }
    SYS_DEPRECATED_HDK(13.0)
    operator GA_Offset() const
    {
        return myPoint.getMapOffset();
    }
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP &operator=(const GA_GBPointP &that)
    {
        myPoint = GA_GBPoint(that.myPoint);
        return *this;
    }
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP &operator=(const GA_GBPoint *that)
    {
        if (that)
            myPoint = GA_GBPoint(*that);
        else
            myPoint = GA_GBPoint();
        return *this;
    }
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPoint &operator*() const
    {
        return SYSconst_cast(myPoint);
    }
    SYS_DEPRECATED_HDK(13.0)
    bool operator==(const GA_GBPointP &that)
    {
        if (!(*this))
            return !that;
        if (!that)
            return false;
        return (myPoint.getMapOffset() == that->getMapOffset()) &&
            (&myPoint.getDetail() == &that->getDetail());
    }
    SYS_DEPRECATED_HDK(13.0)
    bool operator!=(const GA_GBPointP &that)
    {
        return !((*this)==that);
    }
    SYS_DEPRECATED_HDK(13.0)
    bool operator==(const GA_GBPoint *that)
    {
        if (!(*this))
            return !that || !that->isGAValid();
        if (!that || !that->isGAValid())
            return false;
        return (myPoint.getMapOffset() == that->getMapOffset()) &&
            (&myPoint.getDetail() == &that->getDetail());
    }
    SYS_DEPRECATED_HDK(13.0)
    bool operator!=(const GA_GBPoint *that)
    {
        return !((*this)==that);
    }
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPoint *operator->() const
    {
        return SYSconst_cast(&myPoint);
    }
public:
    GA_GBPoint myPoint;
};

class GEO_API GEO_Point : public GA_GBPoint
{
public:
    /// Class constructors and destructors. The one that takes an attribute
    /// data "steals" the data from adata and sets adata's data pointer to 0.
    // @{
    SYS_DEPRECATED_HDK(13.0)
    GEO_Point()
	: GA_GBPoint() {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_Point(const GA_IndexMap &index_map, GA_Offset offset)
	: GA_GBPoint(index_map, offset) {}
    SYS_DEPRECATED_HDK(13.0)
    ~GEO_Point() {}
    // @}

    /// @{
    /// Get de-homogenized position (that is, without the homogeneous coordinate
    /// multiplied through).
    SYS_DEPRECATED_HDK(13.0)
    const UT_Vector4	getPos() const { return GA_GBPoint::getPos3D(); }
    /// @}

    /// @{
    /// Get and set the homogeneous coordinate
    SYS_DEPRECATED_HDK(13.0)
    float	getPw() const		{ return GA_GBPoint::getPw(); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPw(float w)	{ return GA_GBPoint::setPw(w); }
    /// @}

    SYS_DEPRECATED_HDK(13.0)
    void	setPos(const UT_Vector4 &P) { GA_GBPoint::setPos3D(P); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(const UT_Vector3 &P, float w=1)
					    { GA_GBPoint::setPos3D(P, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(float x, float y, float z)
					    { GA_GBPoint::setPos3D(x,y,z,1); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(float x, float y, float z, float w)
					    { GA_GBPoint::setPos3D(x,y,z,w); }

    /// New GA methods which support getting/setting positons without affecting
    /// the w (homogeneous) coordinate.
    /// @{ 
    SYS_DEPRECATED_HDK(13.0)
    const UT_Vector3	getPos3() const
			    { return GA_GBPoint::getPos2D(); }
    SYS_DEPRECATED_HDK(13.0)
    void		setPos3(const UT_Vector3 &P)
			    { GA_GBPoint::setPos2D(P); }
    /// @}

    SYS_DEPRECATED_HDK(14.0)
    GA_StringIndexType getStringHandle(const GA_ROAttributeRef &h,
				       int index = 0) const
		    {
			const GA_Attribute *attrib = h.getAttribute();
			if (!attrib) 
			    return GA_INVALID_STRING_INDEX;
			const GA_AIFSharedStringTuple *aif = 
				    attrib->getAIFSharedStringTuple();
			if (!aif) 
			    return GA_INVALID_STRING_INDEX;
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			return aif->getHandle(attrib, getMapOffset(), index);
		    }

    SYS_DEPRECATED_HDK(14.0)
    GA_StringIndexType getStringHandle(const GA_RWAttributeRef &h,
				       int index = 0) const
		    {
			const GA_Attribute *attrib = h.getAttribute();
			if (!attrib) 
			    return GA_INVALID_STRING_INDEX;
			const GA_AIFSharedStringTuple *aif = 
				    attrib->getAIFSharedStringTuple();
			if (!aif) 
			    return GA_INVALID_STRING_INDEX;
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			return aif->getHandle(attrib, getMapOffset(), index);
		    }

    typedef GA_Detail	DetailType;

};

/// DO NOT USE THIS CLASS!!!
class GEO_API GEO_PointP : public GA_GBPointP
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GEO_PointP()
        : GA_GBPointP()
    {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_PointP(const GA_Detail *gdp, GA_Offset offset)
        : GA_GBPointP(gdp, offset)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_PointP(const GA_Detail &gdp, GA_Offset offset)
        : GA_GBPointP(gdp, offset)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_PointP(const GA_GBPointP &that)
        : GA_GBPointP(that)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_PointP(const GA_GBPoint *that)
        : GA_GBPointP(that)
    {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_Point *operator->() const
    {
        return static_cast<GEO_Point*>((*this).GA_GBPointP::operator->());
    }
    SYS_DEPRECATED_HDK(13.0)
    GEO_Point &operator*() const
    {
        return static_cast<GEO_Point&>((*this).GA_GBPointP::operator*());
    }
    SYS_DEPRECATED_HDK(13.0)
    operator GEO_Point() const
    {
        return GEO_Point((*this)->getIndexMap(), (*this)->getMapOffset());
    }
};

SYS_DEPRECATED_POP_DISABLE()

#endif
