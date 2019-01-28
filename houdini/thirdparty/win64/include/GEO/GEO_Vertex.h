/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	The vertex class. As opposed to GEO_Points, all vertices are unique.
 *
 * WARNING: Try to avoid using the assignment operator '=' because it is
 *	    shallow in the attribute component!
 *
 */

#ifndef __GEO_Vertex_H__
#define __GEO_Vertex_H__

#include "GEO_API.h"
#include <GA/GA_GBElement.h>

class GEO_Detail;
class GEO_Primitive;

SYS_DEPRECATED_PUSH_DISABLE()

/// Base class for GEO_Vertex.
///
class GEO_API GA_GBVertex : public GA_GBElement
{
protected:    
    /// Protect the constructors as this is only a base class and cannot itself
    /// be instantiated.
     GA_GBVertex(const GA_IndexMap &index_map, GA_Offset offset)
	 : GA_GBElement(index_map, offset) { }
     GA_GBVertex()
	 : GA_GBElement() {}
     GA_GBVertex(const GA_GBVertex &src)
	 : GA_GBElement(src) { }

     /// We protect the destructor as nobody has any business using a reference
     /// to this class to delete an instance of a class derived from us.
    ~GA_GBVertex() {}

public:
    // --------------------------------------------------------------------
    // 
    // Methods from GB_Vertex
    //
    // --------------------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    GA_Offset		 getPointOffset() const;
    SYS_DEPRECATED_HDK(13.0)
    void		 setPointOffset(GA_Offset);

    SYS_DEPRECATED_HDK(13.0)
    GA_Index		 getPointIndex() const;

    /// @{
    /// Copy/Compare the vertex handle (not the vertex contents)
    SYS_DEPRECATED_HDK(13.0)
    GA_GBVertex	&operator=(const GA_GBVertex &src)
		{
		    GA_GBElement::operator=(src);
		    return *this;
		}
    SYS_DEPRECATED_HDK(13.0)
    bool	operator==(const GA_GBVertex &src) const
		{
		    return GA_GBElement::operator==(src);
		}
    /// @}

    // --------------------------------------------------------------------
    // 
    // Methods from GEO_Vertex
    //
    // --------------------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    void		 copyVertex(const GA_GBVertex &src,
				    GA_Offset offset,
				    const GA_AttributeSet &dict)
			 {
			     setPointOffset(offset);
			     copyAttribData(src, dict);
			 }
    SYS_DEPRECATED_HDK(13.0)
    void		 copyVertex(const GA_GBVertex &src,
				    GA_GBElement *pt,
				    const GA_AttributeSet &dict)
			 {
			     copyVertex(src, pt->getMapOffset(), dict);
			 }
    SYS_DEPRECATED_HDK(13.0)
    void		 copyVertex(const GA_GBVertex &src,
				    const GA_AttributeSet &dict)
			 {
			     copyVertex(src, src.getPointOffset(), dict);
			 }

protected:
    /// @{
    /// Get de-homogenized position (that is, without the homogeneous coordinate
    /// multiplied through).
    const UT_Vector4	 getPos3D() const;
    const UT_Vector3	 getPos2D() const;
    const UT_Vector2	 getPos2() const;
    float		 getPw() const;
    /// @}

    void		 setPos3D(const UT_Vector4 &P);
    void		 setPos3D(const UT_Vector3 &P);
    void		 setPos3D(const UT_Vector3 &P, float w);
    void		 setPos3D(float x, float y, float z, float w);

    void		 setPos2D(const UT_Vector3 &P);
    void		 setPos2D(const UT_Vector2 &P);
    void		 setPos2D(const UT_Vector2 &P, float w);
    void		 setPos2D(float x, float y, float w);

    void		 setPw(float w);
};

inline const UT_Vector4
GA_GBVertex::getPos3D() const
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().getPos4( getDetail().vertexPoint(getMapOffset()) );
}
inline const UT_Vector3
GA_GBVertex::getPos2D() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().getPos3( getDetail().vertexPoint(getMapOffset()) );
}
inline const UT_Vector2
GA_GBVertex::getPos2() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().getPos2( getDetail().vertexPoint(getMapOffset()) );
}
inline float
GA_GBVertex::getPw() const
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().getPw( getDetail().vertexPoint(getMapOffset()) );
}
inline void
GA_GBVertex::setPos3D(const UT_Vector4 &P)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    getDetail().setPos4( getDetail().vertexPoint(getMapOffset()), P );
}
inline void
GA_GBVertex::setPos3D(const UT_Vector3 &P)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().setPos3(getDetail().vertexPoint(getMapOffset()),P);
}
inline void
GA_GBVertex::setPos3D(const UT_Vector3 &P, float w)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    UT_Vector4		P4(P.x(), P.y(), P.z(), w);
    return getDetail().setPos4( getDetail().vertexPoint(getMapOffset()),
			    P4);
}
inline void
GA_GBVertex::setPos3D(float x, float y, float z, float w)
{
    UT_ASSERT_P(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    UT_Vector4		P4(x, y, z, w);
    return getDetail().setPos4( getDetail().vertexPoint(getMapOffset()),
			    P4);
}
inline void
GA_GBVertex::setPos2D(const UT_Vector3 &P)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    getDetail().setPos3( getDetail().vertexPoint(getMapOffset()), P);
}
inline void
GA_GBVertex::setPos2D(const UT_Vector2 &P)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().setPos2(getDetail().vertexPoint(getMapOffset()),P);
}
inline void
GA_GBVertex::setPos2D(const UT_Vector2 &P, float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().setPos3(getDetail().vertexPoint(getMapOffset()),
			    P.x(), P.y(), w);
}
inline void
GA_GBVertex::setPos2D(float x, float y, float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    return getDetail().setPos3(getDetail().vertexPoint(getMapOffset()),
			    x, y, w);
}
inline void
GA_GBVertex::setPw(float w)
{
    UT_ASSERT(getIndexMap().getOwner() == GA_ATTRIB_VERTEX);
    getDetail().setPw( getDetail().vertexPoint(getMapOffset()), w );
}

class GEO_API GEO_Vertex : public GA_GBVertex
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex(const GA_IndexMap &index, GA_Offset offset)
	: GA_GBVertex(index, offset) {}
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex()
	: GA_GBVertex() {}

    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex(const GEO_Vertex &v)
	: GA_GBVertex(v) {}

    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex	&operator=(const GEO_Vertex &src)
		 {
		    GA_GBVertex::operator=(src);
		    return *this;
		 }
    SYS_DEPRECATED_HDK(13.0)
    bool	operator==(const GEO_Vertex &src) const
		 {
		    return GA_GBVertex::operator==(src);
		 }

    /// @{
    /// Return the primitive that owns this vertex (references the vertex).
    /// This may return a NULL pointer if the topology attributes aren't built
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Primitive	*getPrimitive() const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_Primitive	*getPrimitive();
    /// @}

    /// @{
    /// Get de-homogenized position (that is, without the homogeneous coordinate
    /// multiplied through).
    SYS_DEPRECATED_HDK(13.0)
    const UT_Vector4	getPos() const { return GA_GBVertex::getPos3D(); }
    /// @}

    /// @{
    /// Get and set the homogeneous coordinate
    SYS_DEPRECATED_HDK(13.0)
    fpreal64	getPw() const		{ return GA_GBVertex::getPw(); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPw(fpreal64 w)	{ return GA_GBVertex::setPw(w); }
    /// @}

    SYS_DEPRECATED_HDK(13.0)
    void	setPos(const UT_Vector4 &P) { GA_GBVertex::setPos3D(P); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(const UT_Vector3 &P, fpreal64 w=1)
					    { GA_GBVertex::setPos3D(P, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(fpreal64 x, fpreal64 y, fpreal64 z)
					    { GA_GBVertex::setPos3D(x,y,z,1); }
    SYS_DEPRECATED_HDK(13.0)
    void	setPos(fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w)
					    { GA_GBVertex::setPos3D(x,y,z,w); }

    /// New GA methods which support getting/setting positons without affecting
    /// the w (homogeneous) coordinate.
    /// @{ 
    SYS_DEPRECATED_HDK(13.0)
    const UT_Vector3	getPos3() const
			    { return GA_GBVertex::getPos2D(); }
    SYS_DEPRECATED_HDK(13.0)
    void		setPos3(const UT_Vector3 &P)
			    { GA_GBVertex::setPos2D(P); }
    /// @}

private:
    /// @{
    /// Prevent array allocation of vertex objects
    void	*operator new[](size_t size);
#if defined(WIN32)
protected:
#endif
    void	operator delete[](void *p);
    /// @}
    // Nothing.
};

SYS_DEPRECATED_POP_DISABLE()

#endif
