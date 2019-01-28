/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_AttributeHandleList.h ( GEO Library, C++)
 *
 * COMMENTS:	A list of handles to perform operations on multiple attributes
 */

#ifndef __GEO_AttributeHandleList__
#define __GEO_AttributeHandleList__

#include "GEO_API.h"
#include "GEO_Detail.h"
#include "GEO_WorkVertexBuffer.h"

#include <GA/GA_AttributeRefMap.h>
#include <GA/GA_VertexPool.h>
#include <UT/UT_Matrix.h>
#include <UT/UT_StackBuffer.h>
#include <UT/UT_SymbolTable.h>


class GA_AttributeFilter;
class GEO_AttributeHandle;

//
// Whether to include "P" in the attribute handle list defined by float
// attributes.  In some cases, it's more useful to skip the "P" attribute since
// it's computed by other means.
//
#define GEO_AHANDLE_LIST_BYPASS_P	0
#define GEO_AHANDLE_LIST_INCLUDE_P	1

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_AttributeHandleList
{
public:
    SYS_DEPRECATED_HDK(13.0)
	     GEO_AttributeHandleList();
    SYS_DEPRECATED_HDK(13.0)
    virtual ~GEO_AttributeHandleList();

    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandleList	*clone() const;

    /// Binding a const detail pointer will create read-only handles, while
    /// binding a non-const detail pointer will create read-write handles.
    /// @note Binding to a populated list will clear the list if the binding
    /// differs from the previous binding, even if only const-ness.
    SYS_DEPRECATED_HDK(13.0)
    void		bindDetail(const GEO_Detail *gdp);
    SYS_DEPRECATED_HDK(13.0)
    void		bindDetail(GEO_Detail *gdp);

    /// Binding a mutable copy of a const detail creates an empty writable
    /// detail, that is automatically populated with copies of attributes
    /// from the original detail as this handle list is populated.
    ///
    /// In addition, the source mapping will be automatically built from
    /// the original detail.  Consequently users of this handle list must
    /// be careful to distinguish between destination and source elements
    /// for an operation, exactly as if the handle list was explicitly
    /// bound to a mutable detail with the source mapping explicitly
    /// built from another detail.  In fact, this method is simply a
    /// conveniently wrapped shorthand for that very sequence of actions.
    /// @note Binding to a populated list will clear the list if the binding
    /// differs from the previous binding, even if only in const-ness.
    SYS_DEPRECATED_HDK(13.0)
    void		bindMutableTemp(const GEO_Detail *gdp);

    /// Access to the vertex pool.  Under no circumstances are you allowed
    /// to change the detail binding.  Also note that the vertex pool may
    /// be unbound in the case of a list of read-only handles.
    SYS_DEPRECATED_HDK(13.0)
    GA_VertexPool	&getVertexPool()	{ return myVertexPool; }
    /// @{
    /// Map from a GA_VertexPoolIndex to a GA_Offset
    SYS_DEPRECATED_HDK(13.0)
    GA_Offset		getTempVertex(GA_VertexPoolIndex index) const
			    { return myVertexPool.getVertex(index); }
    SYS_DEPRECATED_HDK(13.0)
    GA_Offset		getTempPoint(GA_VertexPoolIndex index) const
			    { return myVertexPool.getPoint(index); }
    /// @}
    /// @{
    /// Allocate a temporary vertex/point
    SYS_DEPRECATED_HDK(13.0)
    GA_VertexPoolIndex	 appendTempVertex(GA_Offset pt=GA_INVALID_OFFSET)
			    { return myVertexPool.appendVertex(pt); }
    SYS_DEPRECATED_HDK(13.0)
    GA_VertexPoolIndex	 appendTempPoint()
			    { return myVertexPool.appendPoint(); }
    /// @}
    /// @{
    /// Free a temporary vertex/point
    SYS_DEPRECATED_HDK(13.0)
    void		 freeTempVertex(GA_VertexPoolIndex v)
			    { myVertexPool.freeVertex(v); }
    SYS_DEPRECATED_HDK(13.0)
    void		 freeTempPoint(GA_VertexPoolIndex p)
			    { myVertexPool.freePoint(p); }
    /// @}


    /// Query whether the specified detail is the "destination" detail of this
    /// handle list.
    /// @warning The handle list may consist of read-only handles if bound to
    /// a const pointer to that detail.
    /// @sa isCommonSourceMapDetail()
    SYS_DEPRECATED_HDK(13.0)
    bool		isBoundDetail(const GEO_Detail *gdp) const
			    { return (getBoundDetail() == gdp); }

    /// Get a pointer to the currently bound mutable detail, if any.  Note
    /// that this may be NULL for a list bound to a const detail.
    SYS_DEPRECATED_HDK(13.0)
    GEO_Detail		*getBoundMutableDetail() { return myMutableDetail; }

    SYS_DEPRECATED_HDK(13.0)
    void		invalidate();

    SYS_DEPRECATED_HDK(13.0)
    int			entries() const	{ return myAttribs.entries(); }
    SYS_DEPRECATED_HDK(13.0)
    bool		isEmpty() const	{ return entries() == 0; }

    SYS_DEPRECATED_HDK(13.0)
    bool		hasVectorAttributes(GA_AttributeOwner owner) const;

    //
    // Attribute creation
    //
    // The following methods add attributes to the attribute list based on
    // particular filters.
    //
    SYS_DEPRECATED_HDK(13.0)
    void		clearAllAttributes();

    SYS_DEPRECATED_HDK(13.0)
    int		appendAttributes(GA_AttributeOwner, 
				 const GA_AttributeFilter &filter,
				 GA_AttributeScope scope = GA_SCOPE_INVALID);
    SYS_DEPRECATED_HDK(13.0)
    int		appendAllAttributes(GA_AttributeOwner owner);
    SYS_DEPRECATED_HDK(13.0)
    int		appendFloatAttributes(GA_AttributeOwner owner,
				int includeP = GEO_AHANDLE_LIST_INCLUDE_P);
    SYS_DEPRECATED_HDK(13.0)
    int		appendAttributesByType(const char *type, GA_AttributeOwner o);
    SYS_DEPRECATED_HDK(13.0)
    int		appendAttributesByTypeInfo(GA_TypeInfo i, GA_AttributeOwner o);
    SYS_DEPRECATED_HDK(13.0)
    bool	appendAttributeByName(const char *name, GA_AttributeOwner o);
    SYS_DEPRECATED_HDK(13.0)
    int		appendAttributesByPattern(const char *pattern,
					GA_AttributeOwner o,
					GA_AttributeScope s = GA_SCOPE_INVALID);
    SYS_DEPRECATED_HDK(13.0)
    bool	 appendAttribute(const GA_Attribute *attrib);
    SYS_DEPRECATED_HDK(13.0)
    bool	 appendAttribute(GA_Attribute *attrib);
    SYS_DEPRECATED_HDK(13.0)
    bool	 appendAttribute(const GA_Attribute &attrib);
    SYS_DEPRECATED_HDK(13.0)
    bool	 appendAttribute(GA_Attribute &attrib);

    // Remove any attributes from the handle list which don't exist in this
    // "other" detail.
    SYS_DEPRECATED_HDK(13.0)
    int		removeAttributes(const GA_AttributeFilter &filter);
    SYS_DEPRECATED_HDK(13.0)
    int		removeMissingAttributes(const GEO_Detail *other);
    SYS_DEPRECATED_HDK(13.0)
    bool	removeAttributeByName(const char *name);

    SYS_DEPRECATED_HDK(13.0)
    void	clearSourceMap();
    SYS_DEPRECATED_HDK(13.0)
    void	appendSourceMap(const GEO_Detail *gdp, GA_AttributeOwner o);
    SYS_DEPRECATED_HDK(13.0)
    void	appendSourceMap(const GEO_AttributeHandleList &hlist);

    /// Call this method whenever a child handle's source mapping is changed
    /// outside of this list.
    SYS_DEPRECATED_HDK(13.0)
    void	childSourceMapChanged(); 

    ///	Query whether the specified detail is the common origin of all the
    /// source map attributes (trivially true if no source attributes).
    /// @note It IS possible to be bound to multiple source details, though
    /// one needs to be EXTREMELY careful and this is not recommended.
    SYS_DEPRECATED_HDK(13.0)
    bool	isCommonSourceMapDetail(const GEO_Detail *gdp) const;

    /// Returns a pointer to an equivalent handle list for performing all
    /// operations using the bound mutable detail as both the destination
    /// and source.
    ///
    /// More specifically, the returned handle list is bound to the same
    /// mutable detail as this list, populated with the same attributes,
    /// with a default source map from that same mutable detail.  A NULL
    /// will be returned from an unbound list.
    ///
    /// @note The returned handle list, if any, is owned by this list, and
    /// the pointer should not be used across any changes to the original
    /// handle list (attribute addition/removal, binding/unbinding, etc)
    /// without obtaining it from this method again.
    ///
    /// @warning First call is not thread-safe.
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandleList *getBoundClosure();

    /// Assignment with a handle list bound to a mutable copy of a const
    /// detail will bind to that mutable copy of the detail, so h cannot
    /// be destroyed or rebound before this handle list is destroyed or
    /// rebound.
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandleList	&operator=(const GEO_AttributeHandleList &h);

    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle		*operator[](int i)
	{ return static_cast<GEO_AttributeHandle *>(getEntry(i)); }
    SYS_DEPRECATED_HDK(13.0)
    const GEO_AttributeHandle	*operator[](int i) const
	{ return static_cast<const GEO_AttributeHandle *>(getEntry(i)); }
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle		*operator[](const char *name)
	{ return static_cast<GEO_AttributeHandle *>(getEntry(name)); }
    SYS_DEPRECATED_HDK(13.0)
    const GEO_AttributeHandle	*operator[](const char *name) const
	{ return static_cast<const GEO_AttributeHandle *>(getEntry(name)); }

    /// Return the total number of floating point attributes in the list.
    SYS_DEPRECATED_HDK(13.0)
    int		getFloatCount(bool includePw=true) const;
    /// Move all the floating point data into a matrix column
    SYS_DEPRECATED_HDK(13.0)
    void	floatsToMatrix(UT_MatrixF &matrix, int row,
				bool includePw=true) const;
    /// Extract the floats from the given row of the matrix
    SYS_DEPRECATED_HDK(13.0)
    void	matrixToFloats(const UT_MatrixF &matrix, int row,
				bool includePw=true);

    //
    // Preferred methods for working on data.  This allows all handles to be
    // operated on properly.
    //
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *prim, GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *prim, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *prim, const GEO_Vertex &v);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *prim, const GEO_Vertex &v);

    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Primitive *p, const GEO_Vertex *v)
		    { addScale(p, v, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *p, const GEO_Vertex *v, fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *p, const GEO_Vertex *v);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p, const GEO_Vertex *v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Vertex *v0,
		     const GEO_Primitive *p1, const GEO_Vertex *v1,
		     fpreal weight);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *p, const GEO_Vertex *v);

    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Primitive *p, const GEO_Vertex &v)
		    { addScale(p, v, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *p, const GEO_Vertex &v, fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	addHScale(const GEO_Primitive *p, const GEO_Vertex &v, fpreal s);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *p, const GEO_Vertex &v);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p, const GEO_Vertex &v, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Vertex &v0,
		     const GEO_Primitive *p1, const GEO_Vertex &v1,
		     fpreal weight);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *p, const GEO_Vertex &v);

    // Unary operations on attribute data
    SYS_DEPRECATED_HDK(13.0)
    void	setValue(fpreal value);	// Usually used to zero data
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(fpreal scale);	// Multiply all data by value
 
    // Alternative operations.  In some cases, it's not possible to get the
    // primitive/vertex, and thus, you need to work on the native element.
    // Only appropriate attribute handles will be operated upon.
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Vertex *vtx);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Vertex *vtx);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Vertex vtx);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Primitive *prim);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Primitive *prim);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(GEO_Detail *gdp);
    SYS_DEPRECATED_HDK(13.0)
    void	setElement(const GEO_Detail *gdp);

    SYS_DEPRECATED_HDK(13.0)
    void	setPoint(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setVertex(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setPrimitive(GA_Offset offset);
    SYS_DEPRECATED_HDK(13.0)
    void	setGlobal();

    // Applied to vertex AND point attributes
    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Vertex *ppt)	{ addScale(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Vertex *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex *ppt, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex *p0, const GEO_Vertex *p1, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Vertex *ppt);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Vertex *ppt);

    SYS_DEPRECATED_HDK(13.0)
    void	addDest(const GEO_Vertex *ppt)	{ addScaleDest(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Vertex *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Vertex *ppt);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Vertex *ppt);

    // Applied to vertex AND point attributes
    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Vertex &ppt)
				{ add(&ppt); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Vertex &ppt, fpreal scale)
				{ addScale(&ppt, scale); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex &ppt, fpreal w)
				{ lerp(&ppt, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Vertex &p0, const GEO_Vertex &p1, fpreal w)
				{ lerp(&p0, &p1, w); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Vertex &ppt)
				{ multiply(&ppt); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Vertex &ppt)
				{ copyDataFrom(&ppt); }

    SYS_DEPRECATED_HDK(13.0)
    void	addDest(const GEO_Vertex &ppt)
				{ addDest(&ppt); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Vertex &ppt, fpreal scale)
				{ addScaleDest(&ppt, scale); }
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Vertex &ppt)
				{ multiplyDest(&ppt); }
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Vertex &ppt)
				{ copyDestData(&ppt); }

    // Applied to primitive AND detail attributes
    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Primitive *ppt)	{ addScale(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Primitive *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *ppt, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Primitive *p0, const GEO_Primitive *p1,fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Primitive *ppt);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Primitive *ppt);

    SYS_DEPRECATED_HDK(13.0)
    void	addDest(const GEO_Primitive *ppt) { addScaleDest(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Primitive *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Primitive *ppt);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDestData(const GEO_Primitive *ppt);

    // Applied to detail attributes only
    SYS_DEPRECATED_HDK(13.0)
    void	add(const GEO_Detail *ppt)	{ addScale(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScale(const GEO_Detail *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	lerp(const GEO_Detail *ppt, fpreal w);
    //void	lerp(const GEO_Detail *gdp0, const GEO_Detail *gdp1, fpreal w);
    SYS_DEPRECATED_HDK(13.0)
    void	multiply(const GEO_Detail *ppt);
    SYS_DEPRECATED_HDK(13.0)
    void	copyDataFrom(const GEO_Detail *ppt);

    SYS_DEPRECATED_HDK(13.0)
    void	addDest(const GEO_Detail *ppt)	{ addScaleDest(ppt, 1.0); }
    SYS_DEPRECATED_HDK(13.0)
    void	addScaleDest(const GEO_Detail *ppt, fpreal scale);
    SYS_DEPRECATED_HDK(13.0)
    void	multiplyDest(const GEO_Detail *ppt);
    //void	copyDestData(const GEO_Detail *ppt);

    SYS_DEPRECATED_HDK(13.0)
    void	dump(const char *msg="") const;		// For debugging

    // Add special position attribute "P"
    SYS_DEPRECATED_HDK(13.0)
    int		appendPointP();

    /// Fill a GA_AttributeRefMap.  This method can be used to adapt code
    /// that used to work with a GEO_AttributeHandleList to use a
    /// GA_AttributeRefMap instead.
    SYS_DEPRECATED_HDK(13.0)
    void		fillAttributeRefMap(GA_AttributeRefMap &map) const;

    /// Fill a GA_AttributeRefMap, but only with source attributes that match
    /// the @c source_owner.  For example, consider the code: @code
    ///	   GEO_AttributeHandleList  alist;
    ///    alist.appendAllAttributes(GA_ATTRIB_VERTEX);
    ///    alist.appendFloatAttributes(GA_ATTRIB_POINT);
    ///    alist.appendFloatAttributes(GA_ATTRIB_PRIMITIVE);
    ///    alist.appendAllAttributes(GA_ATTRIB_DETAIL);
    ///    GA_AttributeRefMap	rmap;
    ///    alist.fillAttributeRefMap(rmap, GA_ATTRIB_POINT);
    ///    alist.fillAttributeRefMap(rmap, GA_ATTRIB_VERTEX);
    /// @endcode
    /// With the code above, the rmap will only contain point/vertex attributes.
    SYS_DEPRECATED_HDK(13.0)
    void		fillAttributeRefMap(GA_AttributeRefMap &map,
				GA_AttributeOwner source_owner) const;

    /// Adapter to convert algorithms that used to operate on
    /// GEO_WorkVertexArray to GA_Offset*
    class VertexArrayAdapter
    {
    public:
        SYS_DEPRECATED_HDK(13.0)
        VertexArrayAdapter(GEO_AttributeHandleList &hlist,
		       GEO_WorkVertexArray &verts)
	    : myOffsets(verts.entries())
	{
	    hlist.fillAttributeRefMap(myMap);
	    for (int i = 0; i < verts.entries(); i++)
		myOffsets[i] = verts.getVertexElements()[i]->getMapOffset();
	}

        SYS_DEPRECATED_HDK(13.0)
        GA_AttributeRefMap	&getMap()
				 { return myMap; }
        SYS_DEPRECATED_HDK(13.0)
        GA_Offset		*getOffsets()
				 { return (GA_Offset *)myOffsets; }

    private:
	GA_AttributeRefMap		myMap;
	UT_StackBuffer<GA_Offset>	myOffsets;
    };

protected:
    void			 setDetail(const GEO_Detail *gdp);
    void			 setDetail(GEO_Detail *gdp);
    void			 setTempDetail(const GEO_Detail *gdp);

    const GEO_Detail *
    getBoundDetail() const
	{ return (myMutableDetail ? myMutableDetail : myConstDetail); } 

    void			 invalidateCachedLists();

    // Add an attribute to the attribute list & name map
    bool		 appendAttribute(GEO_AttributeHandle *h);
    bool		 removeAttribute(int i);
    bool		 removeAttribute(const char *name);

    GEO_AttributeHandle	*getEntry(int i) const	{ return myAttribs(i); }
    GEO_AttributeHandle	*getEntry(const char *name) const;

    /// Removes the attributes
    void		clearAttributes();

private:
    // When myMutableDetail is set, myConstDetail will also be set, to either
    // the same detail, if we were bound to a mutable detail, or to the const
    // detail from which we were bound to a mutable copy.  In the latter case
    // we also have myMutableDetail == &myVertexPool.getDetail().
    //
    // When bound to a const detail, only myConstDetail will be set.
    const GEO_Detail			*myConstDetail;
    GEO_Detail				*myMutableDetail;

    UT_Array<GEO_AttributeHandle *>	 myAttribs;
    UT_SymbolMap<GEO_AttributeHandle *>  myNameMap;
    GA_VertexPool			 myVertexPool;
    GEO_AttributeHandleList		*myBoundClosureList;
    bool				 myReadOnly;
    bool				 myIsDefaultSourceMap;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
