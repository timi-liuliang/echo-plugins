/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_AttributeHandle.h ( GEO Library, C++)
 *
 * COMMENTS:	A handle class to perform operations on an attribute
 */

#ifndef __GEO_AttributeHandle__
#define __GEO_AttributeHandle__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_Vertex.h"

#include <GA/GA_Types.h>

#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

#include <SYS/SYS_Types.h>


class UT_StringArray;
class GEO_Detail;
class GEO_Primitive;
class GEO_Vertex;
class GA_GBElement;
class GEO_Point;

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_AttributeHandle
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle();
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle(const GEO_AttributeHandle &gah);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle(const GA_Attribute *attrib);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle(GA_Attribute *attrib);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle(const GA_ROAttributeRef &ref);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle(const GA_RWAttributeRef &ref);

    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	*clone() const;
    SYS_DEPRECATED_HDK(13.0)
    const char		*getName() const;

    SYS_DEPRECATED_HDK(13.0)
    ~GEO_AttributeHandle();

    SYS_DEPRECATED_HDK(13.0)
    const GA_Attribute	*getAttribute() const;
    SYS_DEPRECATED_HDK(13.0)
    const GA_Attribute	*getSourceAttribute() const;

    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	&operator=(const GEO_AttributeHandle &gah);

    SYS_DEPRECATED_HDK(13.0)
    void	invalidate();

    SYS_DEPRECATED_HDK(13.0)
    bool	isAttributeValid() const;
    SYS_DEPRECATED_HDK(13.0)
    bool	isSourceAttributeValid() const;
    SYS_DEPRECATED_HDK(13.0)
    void	clearElement() { setRawElement(GA_INVALID_OFFSET); }

    // Upgrade a read-only handle to a read-write handle, returning true on
    // success.
    SYS_DEPRECATED_HDK(13.0)
    bool	upgradeToReadWrite(GA_AttributeSet &set);
    // Downgrade a read-write handle to a read-only handle, returning true on
    // success.
    SYS_DEPRECATED_HDK(13.0)
    bool	downgradeToReadOnly();

    /// @{
    /// Test to see whether the attribute is a string attribute.  This tests to
    /// see whether the attribute has an shared string tuple interface (@see
    /// GA_AIFSharedStringTuple).
    /// Please use isSharedStringAttribute() rather than isIndexAttribute()
    SYS_DEPRECATED(12.0) bool		isIndexAttribute() const
				    { return isSharedStringAttribute(); }
    SYS_DEPRECATED_HDK(13.0)
    bool			isSharedStringAttribute() const;
    /// @}

    /// Returns true if our attribute handle is bound to a valid attribute of
    /// a floating point tuple type.
    SYS_DEPRECATED_HDK(13.0)
    bool		isFloatOrVectorAttribute() const;

    /// Returns true if the attribute handle has strings defined.
    /// It's possible that the array contains 0 strings.
    SYS_DEPRECATED_HDK(13.0)
    bool		getDefinedStrings(UT_StringArray &list,
						bool harden=true) const;

    /// Return the string assocated with the given index.  This is only valid
    /// for shared string attributes.
    SYS_DEPRECATED(12.0) const char		*getDefinedString(int idx) const
			    { return getSharedString(idx); }
    SYS_DEPRECATED_HDK(13.0)
    const char		*getSharedString(int index) const;

    /// The myOffset refers to elements which are bound to the handle (i.e
    /// through setElementRaw().  When copying data from other details, the
    /// attribute data doesn't always come from the same location (or even the
    /// same element).  The source offset is used to define the offset from the
    /// source detail (i.e. the detail *not* bound to this handle).
    /// By default, myOffset == mySourceOffset
    SYS_DEPRECATED_HDK(13.0)
    void	clearSourceMap()
		{
		    GA_ROAttributeRef invalid;
		    setSourceMap(invalid);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	setSourceMap(GA_AttributeOwner owner,
			     const GA_ROAttributeRef &src);
    SYS_DEPRECATED_HDK(13.0)
    void	setSourceMap(const GA_AttributeDict &dict,
			     GA_AttributeOwner owner);
    SYS_DEPRECATED_HDK(13.0)
    void	setSourceMap(const GEO_AttributeHandle &h);

    /// Attribute data is now stored externally to the element, so we can no
    /// longer use the constness of the element argument to determine whether
    /// we can write to the attribute.
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Vertex *v)
		    { setElement(const_cast<const GEO_Vertex *>(v)); }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *prim)
		    { setElement(const_cast<const GEO_Primitive *> (prim)); }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Detail *gdp)
		    { setElement(const_cast<const GEO_Detail *>(gdp)); }

    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Vertex *v)
		    {
			setVertex(v ? v->getMapOffset():GA_INVALID_OFFSET);
		    }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *prim)
		    {
			if (myDictType == GA_ATTRIB_GLOBAL)
			    setElement(prim ? prim->getParent() : 0);
			else
			    setPrimitive(prim ? prim->getMapOffset()
					      : GA_INVALID_OFFSET);
		    }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Detail *gdp)
		    {
			if (gdp)
			    setGlobal();
			else
			    setRawElement(GA_INVALID_OFFSET);
		    }

    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *p, GEO_Vertex *v)
		    { setElement(const_cast<const GEO_Primitive *>(p),
				 const_cast<const GEO_Vertex *>(v)); }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *p, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *p, const GEO_Vertex &v)
		    { setElement(const_cast<const GEO_Primitive *>(p), v); }
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *p, const GEO_Vertex &v);

    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GA_AttributeOwner owner, GA_Offset offset);

    SYS_DEPRECATED_HDK(13.0)
    void	setPoint(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setVertex(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setPrimitive(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setGlobal();

    SYS_DEPRECATED_HDK(13.0)
    GA_AttributeOwner	getDictionary() const	{ return myDictType; }
    SYS_DEPRECATED_HDK(13.0)
    bool		isP() const		{ return myIsPointP; }
    SYS_DEPRECATED_HDK(13.0)
    GA_AttributeOwner	getSourceDictionary() const
						{ return mySourceDictType; }

    SYS_DEPRECATED(12.5) int	entries() const	// Return the tuple size
			    { return tupleSize(); }
    SYS_DEPRECATED_HDK(13.0)
    int			tupleSize() const;
    SYS_DEPRECATED_HDK(13.0)
    int			getFloatCount(bool includePw) const
			{
			    if (myIsPointP)
				return 3;
			    return isFloatOrVectorAttribute() ? tupleSize() : 0;
			}

    /// Generic Data Accessors
    SYS_DEPRECATED_HDK(13.0)
    void		setF(fpreal val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    fpreal		getF(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setI(int val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    int			getI(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setV2(const UT_Vector2 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Vector2		getV2(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setV3(const UT_Vector3 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Vector3		getV3(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setV4(const UT_Vector4 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Vector4		getV4(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setM2(const UT_Matrix2 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Matrix2		getM2(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setM3(const UT_Matrix3 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Matrix3		getM3(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setM4(const UT_Matrix4 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Matrix4		getM4(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    void		setQ(const UT_Quaternion &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Quaternion	getQ(int index=0) const;

    SYS_DEPRECATED_HDK(13.0)
    bool		importVector(fpreal32 *val, int size) const;
    SYS_DEPRECATED_HDK(13.0)
    bool		importVector(fpreal64 *val, int size) const;
    SYS_DEPRECATED_HDK(13.0)
    bool		importVector(int32 *val, int size) const;
    SYS_DEPRECATED_HDK(13.0)
    bool		importVector(int64 *val, int size) const;

    SYS_DEPRECATED_HDK(13.0)
    bool		import(fpreal32 &val) const
			    { return importVector(&val, 1); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(fpreal64 &val) const
			    { return importVector(&val, 1); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(int32 &val) const
			    { return importVector(&val, 1); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(int64 &val) const
			    { return importVector(&val, 1); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Vector2 &v)
			    { return importVector(v.data(), 2); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Vector3 &v)
			    { return importVector(v.data(), 3); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Vector4 &v)
			    { return importVector(v.data(), 4); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Matrix2 &v)
			    { return importVector(v.data(), 4); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Matrix3 &v)
			    { return importVector(v.data(), 9); }
    SYS_DEPRECATED_HDK(13.0)
    bool		import(UT_Matrix4 &v)
			    { return importVector(v.data(), 16); }

    /// p4 will return a 4th component of 1 if it does not exist
    SYS_DEPRECATED_HDK(13.0)
    void		setP4(const UT_Vector4 &val, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    UT_Vector4		getP4(int index=0) const;

    // The {get,set}String methods return true if the attribute is a valid
    // string attribute and false otherwise.
    SYS_DEPRECATED_HDK(13.0)
    bool		setString(const UT_String &str, int index=0);
    SYS_DEPRECATED_HDK(13.0)
    bool		getString(UT_String &str, int index=0,
					    bool harden=true) const;

    // Operations on data
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(fpreal w);

    SYS_DEPRECATED_HDK(13.0)
    void	setValue(fpreal val);

    /// Performs:  this = this + gah * w
    SYS_DEPRECATED_HDK(13.0)
    void		addScale(const GEO_AttributeHandle &gah, fpreal w);

    /// Performs:  this = this * gah
    SYS_DEPRECATED_HDK(13.0)
    void		multiply(const GEO_AttributeHandle &gah);
    /// Performs the operation this = this*(1-w) + gah*w
    SYS_DEPRECATED_HDK(13.0)
    void		lerp(const GEO_AttributeHandle &gah, fpreal w);

    /// Copies the data from the src attribute to this attribute
    SYS_DEPRECATED_HDK(13.0)
    void		copyDataFrom(const GEO_AttributeHandle &src);


    // Operations combining other attributes.  The provided objects (points,
    // vertices, primitives) must be from the same detail as the source map
    // attribute.  This also applies to the detail versions, and so it makes
    // no sense to have a lerp(const GEO_Detail *, const GEO_Detail *, fpreal).
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *p, const GEO_Vertex *v, fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p, const GEO_Vertex *v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Vertex *v0,
			const GEO_Primitive *p1, const GEO_Vertex *v1,
			fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *p, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *p, const GEO_Vertex *v);

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Point *elem, fpreal w)
		    { addScale((const GA_GBElement*)elem, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Point *elem, fpreal w)
		    { lerp((const GA_GBElement*)elem, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Point *p0, const GEO_Point *p1, fpreal w)
		    { lerp((const GA_GBElement*)p0, (const GA_GBElement*)p1, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Point *elem)
		    { multiply((const GA_GBElement*)elem, GA_ATTRIB_POINT); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Point *elem)
		    { copyDataFrom((const GA_GBElement*)elem, GA_ATTRIB_POINT); }

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *p, const GEO_Vertex &v, fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p, const GEO_Vertex &v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Vertex &v0,
			const GEO_Primitive *p1, const GEO_Vertex &v1,
			fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *p, const GEO_Vertex &v);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *p, const GEO_Vertex &v);

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Vertex *elem, fpreal w)
		    { addScale(elem, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex *elem, fpreal w)
		    { lerp(elem, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex *p0, const GEO_Vertex *p1, fpreal w)
		    { lerp(p0, p1, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Vertex *elem)
		    { multiply(elem, GA_ATTRIB_VERTEX); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Vertex *elem)
		    { copyDataFrom(elem, GA_ATTRIB_VERTEX); }

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Vertex &elem, fpreal w)
		    { addScale(&elem, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex &elem, fpreal w)
		    { lerp(&elem, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex &p0, const GEO_Vertex &p1, fpreal w)
		    { lerp(&p0, &p1, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Vertex &elem)
		    { multiply(&elem); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Vertex &elem)
		    { copyDataFrom(&elem); }

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *elem, fpreal w)
		    { addScaleRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *elem, fpreal w)
		    { lerpRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Primitive *p1, fpreal w)
		    {
                        lerpRaw(p0 ? p0->getMapOffset() : GA_INVALID_OFFSET,
                                p1 ? p1->getMapOffset() : GA_INVALID_OFFSET,
                                GA_ATTRIB_PRIMITIVE, w);
                    }
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *elem)
		    { multiplyRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *elem)
		    { copyDataFromRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE); }

    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Detail *elem, fpreal w)
		{
		    addScaleRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL, w);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Detail *elem, fpreal w)
		{
		    lerpRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL, w);
		}
    //void	lerp(const GEO_Detail *gdp0, const GEO_Detail *gdp1, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Detail *elem)
		{
		    multiplyRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Detail *elem)
		{
		    copyDataFromRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL);
		}

    // Operations combining attribute values from other elements.  The provided
    // objects (points, vertices, primitives) must be from the same detail as
    // the bound attribute.  This also applies to the detail versions, and so
    // it makes no sense to have 
    //   lepDest(const GEO_Detail *, fpreal)
    //   lerpDest(const GEO_Detail *, const GEO_Detail *, fpreal)
    //   copyDestData(const GEO_Detail *)
    // 
    // These operations read from and write to the destination attribute, and
    // so do not require a source map.
    // @{
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Primitive *p, const GEO_Vertex *v,
			     fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Primitive *p, const GEO_Vertex &v,
			     fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Point *elem, fpreal w)
		    { addScaleDest((const GA_GBElement*)elem, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Vertex *elem, fpreal w)
		    { addScaleDest(elem, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Vertex &elem, fpreal w)
		    { addScaleDest(&elem, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Primitive *elem, fpreal w)
		    { addScaleDestRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Detail *elem, fpreal w)
		    { addScaleDestRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL, w); }

    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *p, const GEO_Vertex *v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *p, const GEO_Vertex &v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Point *elem, fpreal w)
		    { lerpDest((const GA_GBElement*)elem, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Vertex *elem, fpreal w)
		    { lerpDest(elem, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Vertex &elem, fpreal w)
		    { lerpDest(&elem, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *elem, fpreal w)
		    { lerpDestRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE, w); }

    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *p0, const GEO_Vertex *v0,
			 const GEO_Primitive *p1, const GEO_Vertex *v1,
			 fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *p0, const GEO_Vertex &v0,
			 const GEO_Primitive *p1, const GEO_Vertex &v1,
			 fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Point *p0, const GEO_Point *p1, fpreal w)
		    { lerpDest((const GA_GBElement*)p0, (const GA_GBElement*)p1, GA_ATTRIB_POINT, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Vertex *p0, const GEO_Vertex *p1, fpreal w)
		    { lerpDest(p0, p1, GA_ATTRIB_VERTEX, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Vertex &p0, const GEO_Vertex &p1, fpreal w)
		    { lerpDest(&p0, &p1, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerpDest(const GEO_Primitive *p0, const GEO_Primitive *p1,
			 fpreal w)
		    { lerpDestRaw(p0 ? p0->getMapOffset() : GA_INVALID_OFFSET, p1 ? p1->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE, w); }

    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Primitive *p, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Primitive *p, const GEO_Vertex &v);
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Point *elem)
		    { multiplyDest((const GA_GBElement*)elem, GA_ATTRIB_POINT); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Vertex *elem)
		    { multiplyDest(elem, GA_ATTRIB_VERTEX); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Vertex &elem)
		    { multiplyDest(&elem); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Primitive *elem)
		    { multiplyDestRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Detail *elem)
		    { multiplyDestRaw(elem ? GA_Offset(0) : GA_INVALID_OFFSET,
				GA_ATTRIB_GLOBAL); }

    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Primitive *p, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Primitive *p, const GEO_Vertex &v);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Point *elem)
		    { copyDestData((const GA_GBElement*)elem, GA_ATTRIB_POINT); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Vertex *elem)
		    { copyDestData(elem, GA_ATTRIB_VERTEX); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Vertex &elem)
		    { copyDestData(&elem); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Primitive *elem)
		    { copyDestDataRaw(elem ? elem->getMapOffset() : GA_INVALID_OFFSET, GA_ATTRIB_PRIMITIVE); }
    // @}


    SYS_DEPRECATED_HDK(13.0)
    bool	match(const GA_AttributeFilter &filter) const;

    SYS_DEPRECATED_HDK(13.0)
    void	dump(const char *msg="") const;		// For debugging

protected:
    bool	isValid() const
		    { return isAttributeValid() && GAisValid(myBoundElement); }
    bool	isSourceValid() const
		    { return isSourceAttributeValid() && GAisValid(myBoundElement); }
    void	setRawElement(GA_Offset offset);

    // Intermediaries between the GEO_Point, GEO_Vertex, GEO_Primitive versions
    // and the GA_Offset "Raw" methods.
    void	addScale(const GA_GBElement *elem, GA_AttributeOwner owner,
			 fpreal w);
    void	lerp(const GA_GBElement *elem, GA_AttributeOwner owner,
		     fpreal w);
    void	lerpH(const GA_GBElement *elem, GA_AttributeOwner owner,
		     fpreal w);
    void	lerp(const GA_GBElement *e0, const GA_GBElement *e1,
		     GA_AttributeOwner owner, fpreal w);
    void	multiply(const GA_GBElement *elem, GA_AttributeOwner owner);
    void	copyDataFrom(const GA_GBElement *elem, GA_AttributeOwner owner);

    // The "Raw" methods that work directly with the GA_Offset of the source
    // element.
    void	addScaleRaw(GA_Offset elem, GA_AttributeOwner owner, fpreal w);
    void	lerpRaw(GA_Offset elem, GA_AttributeOwner owner, fpreal w);
    void	lerpHRaw(GA_Offset elem, GA_AttributeOwner owner, fpreal w);
    void	lerpRaw(GA_Offset e0, GA_Offset e1, GA_AttributeOwner owner,
			fpreal w);
    void	multiplyRaw(GA_Offset elem, GA_AttributeOwner owner);
    void	copyDataFromRaw(GA_Offset elem, GA_AttributeOwner owner);

    // Intermediaries between the GEO_Point, GEO_Vertex, GEO_Primitive versions
    // and the GA_Offset "Raw" methods.
    void	addScaleDest(const GA_GBElement *elem, GA_AttributeOwner owner,
			     fpreal w);
    void	lerpDest(const GA_GBElement *elem, GA_AttributeOwner owner,
			 fpreal w);
    void	lerpDest(const GA_GBElement *e0, const GA_GBElement *e1,
			 GA_AttributeOwner owner, fpreal w);
    void	multiplyDest(const GA_GBElement *elem, GA_AttributeOwner owner);
    void	copyDestData(const GA_GBElement *elem, GA_AttributeOwner owner);

    // The "Raw" methods that work directly with the GA_Offset of the dest
    // element.
    void	addScaleDestRaw(GA_Offset elem, GA_AttributeOwner owner,
				fpreal w);
    void	lerpDestRaw(GA_Offset elem, GA_AttributeOwner owner, fpreal w);
    void	lerpDestRaw(GA_Offset e0, GA_Offset e1, GA_AttributeOwner owner,
			    fpreal w);
    void	multiplyDestRaw(GA_Offset elem, GA_AttributeOwner owner);
    void	copyDestDataRaw(GA_Offset elem, GA_AttributeOwner owner);

private:
    // These initializer methods assume that myAttributeProxy, myReadOnly, and
    // myBoundElement are set externally.
    void		 initCommon(const GA_Attribute &attrib);
    void		 initCommon();

    const GA_Attribute	*getConstAttribute() const;
    GA_Attribute	*getNonConstAttribute();

    const GA_Detail	*getDetail() const;

    template <typename T> inline bool
	importVectorT(T *val, int size) const;

    GA_AttributeOwner	myDictType;
    GA_AttributeOwner	mySourceDictType;

#if 0
    union {
	const GA_AttributeProxy	*myConst;
	GA_AttributeProxy	*myNonConst;
    } myAttributeProxy;
#endif
    GA_AttributeProxyHandle		myDestProxy;
    GA_TypeInfo				myDestTypeInfo;
    GA_ConstAttributeProxyHandle	mySourceProxy;

    // Cached AIFs to avoid virtual query for every element.
    const GA_AIFMath	*myMath;
    const GA_AIFInterp	*myInterp;
    const GA_AIFTuple	*myTuple;
    const GA_AIFCopyData *myCopyData;

    GA_Offset		 myBoundElement;

    // This flag marks whether this is a read-only or read-write attribute
    // handle, and thus, which member of the myAttributeProxy union to access.
    bool		 myReadOnly;

    // Special flag for "P" attrib on points.
    bool		 myIsPointP;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
