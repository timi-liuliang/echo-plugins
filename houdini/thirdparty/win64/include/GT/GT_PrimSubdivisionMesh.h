/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimSubdivisionMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimSubdivisionMesh__
#define __GT_PrimSubdivisionMesh__

#include "GT_API.h"
#include "GT_PrimPolygonMesh.h"
#include <UT/UT_Array.h>

/// A subdivision mesh primitive
///
/// Subdivision meshes can have a list of arbitrary tags.  Each tag can have
/// integer, real or string values.  Tags that are used in refinement:
/// - int crease[]@n
///	A list of pairs of vertices defining edges
/// - real crease[]@n
///	The crease weights for each edge
/// - int corner[]@n
///	A list of corner vertices
/// - real corner[]@n
///	The crease weights for corner
/// - int interpolateboundary@n
///	Deprecated: Determines how point attributes are interpolated
/// - int facevaryinginterpolateboundary@n
///	Deprecated: Determines how vertex attributes are interpolated
/// - int osd_vtxboundaryinterpolation @n
///	Determines how point attributes are interpolated (values match the enum
///	in OpenSubdiv.
/// - int osd_fvarlinearinterpolation @n
///	Determines how vertex attributes are interpolated (values match the enum
///	in OpenSubdiv.
/// - int osd_creasingmethod @n
///	Determines the creasing rule (values match the enum in OpenSubdiv)
/// - int osd_trianglesubdivision @n
///	Determines triangle subdivision smoothing for Catmull-Clark subdivision
///	(values match the enum in OpenSubdiv)
class GT_API GT_PrimSubdivisionMesh : public GT_PrimPolygonMesh
{
public:
    GT_PrimSubdivisionMesh()
	: GT_PrimPolygonMesh()
	, myTags()
	, myScheme(GT_CATMULL_CLARK)
    {
    }

    GT_PrimSubdivisionMesh(const GT_DataArrayHandle &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			GT_IndexingMode indexing=GT_INDEXING_QUICK,
			GT_Size min_vertex_count=0,
			GT_Size max_vertex_count=0)
	: GT_PrimPolygonMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail, indexing,
		min_vertex_count, max_vertex_count)
	, myTags()
	, myScheme(GT_CATMULL_CLARK)
    {
    }
    GT_PrimSubdivisionMesh(const GT_CountArray &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail)
	: GT_PrimPolygonMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail)
	, myTags()
	, myScheme(GT_CATMULL_CLARK)
    {
    }

    GT_PrimSubdivisionMesh(const GT_PrimSubdivisionMesh &mesh,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail)
	: GT_PrimPolygonMesh(mesh, shared, vertex, uniform, detail)
	, myTags(mesh.myTags)
	, myScheme(mesh.myScheme)
    {
    }
    GT_PrimSubdivisionMesh(const GT_PrimSubdivisionMesh &mesh,
			const GT_DataArrayHandle &vtx_index,
			const GT_AttributeListHandle &shared)
	: GT_PrimPolygonMesh(mesh, vtx_index, shared)
	, myTags(mesh.myTags)
	, myScheme(mesh.myScheme)
    {
    }
    GT_PrimSubdivisionMesh(const GT_PrimSubdivisionMesh &mesh)
	: GT_PrimPolygonMesh(mesh)
	, myTags(mesh.myTags)
	, myScheme(mesh.myScheme)
    {
    }
    /// Construct from a GT_PrimPolygonMesh
    GT_PrimSubdivisionMesh(const GT_PrimPolygonMesh &mesh, GT_Scheme scheme)
	: GT_PrimPolygonMesh(mesh)
	, myTags()
	, myScheme(scheme)
    {
    }

    /// Given a data array of either a string or integer value, look-up the
    /// corresponding scheme.  This can be used to interpret the "osd_scheme"
    /// attribute.
    static GT_Scheme	lookupScheme(const GT_DataArrayHandle &scheme,
				GT_Scheme defscheme = GT_CATMULL_CLARK);

    virtual const char	*className() const { return "GT_PrimSubdivisionMesh"; }
    virtual bool	 save(UT_JSONWriter &w) const;
    virtual int		 getPrimitiveType() const;
    virtual bool	 refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;
    virtual int64	 getMemoryUsage() const;

    void		 overrideAttributes(
				const GT_AttributeListHandle &pt_attribs,
				const GT_AttributeListHandle &vtx_attribs);
    void		 clearOverrideAttributes();

    GT_PrimSubdivisionMesh *createPointNormalsIfMissing(int segment = 0,
				const UT_StringRef &P = GA_Names::P,
				bool normalize = true) const;

    /// Subdivision tag
    class GT_API Tag
    {
    public:
	Tag()
	    : myName()
	    , myInt()
	    , myReal()
	    , myString()
	{
	}
	Tag(const char *name)
	    : myName(name, true)
	    , myInt()
	    , myReal()
	    , myString()
	{
	}
	Tag(const Tag &src)
	    : myName(src.myName, true)
	    , myInt(src.myInt)
	    , myReal(src.myReal)
	    , myString(src.myString)
	{
	}
	Tag	&operator=(const Tag &src)
		{
		    if (this != &src)
		    {
			myName.harden(src.myName);
			myInt = src.myInt;
			myReal = src.myReal;
			myString = src.myString;
		    }
		    return *this;
		}

	bool	match(const char *name) const	{ return myName == name; }
	bool	valid() const { return myName.isstring(); }
	int64	getMemoryUsage() const
		{
		    int64	mem = sizeof(*this) + myName.getMemoryUsage();
		    for (exint i = 0; i < myInt.entries(); ++i)
			mem += myInt(i)->getMemoryUsage();
		    for (exint i = 0; i < myReal.entries(); ++i)
			mem += myReal(i)->getMemoryUsage();
		    for (exint i = 0; i < myString.entries(); ++i)
			mem += myString(i)->getMemoryUsage();
		    return mem;
		}

	void	clear()
		{
		    myInt.setCapacity(0);
		    myReal.setCapacity(0);
		    myString.setCapacity(0);
		}
	const char	*name() const		{ return myName; }
	exint		 intCount() const	{ return myInt.entries(); }
	exint		 realCount() const	{ return myReal.entries(); }
	exint		 stringCount() const	{ return myString.entries(); }

	const GT_DataArrayHandle	&intArray(exint i=0) const
					    { return myInt(i); }
	const GT_DataArrayHandle	&realArray(exint i=0) const
					    { return myReal(i); }
	const GT_DataArrayHandle	&stringArray(exint i=0) const
					    { return myString(i); }

	void	harden();

	void	appendInt(const GT_DataArrayHandle &data)
		    { myInt.append(data); }
	void	appendReal(const GT_DataArrayHandle &data)
		    { myReal.append(data); }
	void	appendString(const GT_DataArrayHandle &data)
		    { myString.append(data); }

	const UT_Array<GT_DataArrayHandle>
		&getAllInt() const { return myInt; }
	const UT_Array<GT_DataArrayHandle>
		&getAllReal() const { return myReal; }
	const UT_Array<GT_DataArrayHandle>
		&getAllString() const { return myString; }

    private:
	UT_String			myName;
	UT_Array<GT_DataArrayHandle>	myInt;		// Integer arrays
	UT_Array<GT_DataArrayHandle>	myReal;	// Float arrays
	UT_Array<GT_DataArrayHandle>	myString;	// String arrays
    };

    class GT_API tag_iterator
    {
    public:
	tag_iterator()
	    : myCurr(0)
	    , myTags(NULL)
	{
	}
	tag_iterator(const tag_iterator &s)
	    : myCurr(s.myCurr)
	    , myTags(s.myTags)
	{
	}
	bool	operator==(const tag_iterator &tag) const
		{
		    if (atEnd() && tag.atEnd())
			return true;
		    if (atEnd() || tag.atEnd())
			return false;
		    return myTags == tag.myTags && myCurr == tag.myCurr;
		}
	tag_iterator	&operator++()	{ advance(); return *this; }
	tag_iterator	&operator=(const tag_iterator &src)
			 {
			     myCurr = src.myCurr;
			     myTags = src.myTags;
			     return *this;
			 }
	const GT_PrimSubdivisionMesh::Tag	&tag() const
						{ return (*myTags)(myCurr); }
	const GT_PrimSubdivisionMesh::Tag	&operator*() const
						{ return tag(); }
	int	index() const	{ return myCurr; }

	bool	atEnd() const
		{
		    return !myTags || myCurr >= myTags->entries();
		}
	void	advance()		{ myCurr++; }
	void	rewind()		{ myCurr = 0; }
    private:
	tag_iterator(const UT_Array<GT_PrimSubdivisionMesh::Tag> &tags)
	    : myTags(&tags)
	    , myCurr(0)
	{
	}
	const UT_Array<GT_PrimSubdivisionMesh::Tag>	*myTags;
	int						 myCurr;
	friend class GT_PrimSubdivisionMesh;
    };

    /// Return a polygon mesh of the subdivision hull
    GT_PrimitiveHandle	refineToHull() const;

    /// @{
    /// get/set the subdivision scheme
    GT_Scheme	scheme() const		{ return myScheme; }
    void	setScheme(GT_Scheme s)	{ myScheme = s; }
    /// @}

    /// @{
    /// Add tags.  If the @c replace tag is true, any existing tag will be
    /// removed before the tag is added.
    void	appendTag(const Tag &src);
    void	appendIntTag(const char *name, const GT_DataArrayHandle &v,
			    bool replace=false);
    void	appendRealTag(const char *name, const GT_DataArrayHandle &v,
			    bool replace=false);
    void	appendStringTag(const char *name, const GT_DataArrayHandle &v,
			    bool replace=false);
    /// @}
    /// Find a tag by name
    const Tag	*findTag(const char *name) const
		{
		    for (int i = 0; i < myTags.entries(); ++i)
		    {
			if (myTags(i).match(name))
			    return &myTags(i);
		    }
		    return NULL;
		}
    /// Delete a tag
    bool	delTag(const char *name)
		{
		    for (int i = 0; i < myTags.entries(); ++i)
		    {
			if (myTags(i).match(name))
			{
			    myTags.removeIndex(i);
			    return true;
			}
		    }
		    return false;
		}
    /// Clear all tags
    void	clearTags()
		{
		    myTags.setCapacity(0);
		}

    /// @{
    /// Tag iteration
    tag_iterator	beginTags() const	{ return tag_iterator(myTags); }
    tag_iterator	endTag() const		{ return tag_iterator(); }
    /// @}

    /// Triangulate (for Loop subdivision)
    GT_PrimitiveHandle	triangulate() const;


    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				{ return new GT_PrimSubdivisionMesh(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;
protected:
    GT_PrimSubdivisionMesh	*copySubd(GT_PrimSubdivisionMesh *smesh) const
    {
	smesh->myTags = myTags;
	smesh->myScheme = myScheme;
	return smesh;
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_DataArrayHandle &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			GT_IndexingMode indexing=GT_INDEXING_QUICK,
			GT_Size min_vertex_count=0,
			GT_Size max_vertex_count=0) const
    {
	auto smesh = new GT_PrimSubdivisionMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail, indexing,
		min_vertex_count, max_vertex_count);
	return copySubd(smesh);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_CountArray &vtx_counts,
			const GT_DataArrayHandle &vtx_indices,
			const GT_AttributeListHandle &shared,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail) const
    {
	auto smesh = new GT_PrimSubdivisionMesh(vtx_counts, vtx_indices,
		shared, vertex, uniform, detail);
	return copySubd(smesh);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_AttributeListHandle &shared,
		       const GT_AttributeListHandle &vertex,
		       const GT_AttributeListHandle &uniform,
		       const GT_AttributeListHandle &detail) const
    {
	return new GT_PrimSubdivisionMesh(*this, shared, vertex, uniform, detail);
    }
    virtual GT_PrimPolygonMesh	*clone(const GT_DataArrayHandle &vtx_indices,
		       const GT_AttributeListHandle &shared) const
    {
	return new GT_PrimSubdivisionMesh(*this, vtx_indices, shared);
    }

private:
    void	hardenTags();
    Tag		*findOrCreateTag(const char *name, bool replace)
		{
		    for (int i = 0; i < myTags.entries(); ++i)
		    {
			if (myTags(i).match(name))
			{
			    if (replace)
				myTags(i).clear();
			    return &myTags(i);
			}
		    }
		    myTags.append(Tag(name));
		    return &myTags(myTags.entries()-1);
		}

    UT_Array<Tag>		 myTags;
    GT_Scheme			 myScheme;
    GT_AttributeListHandle	 myStashedPoint;
    GT_AttributeListHandle	 myStashedVertex;
};

#endif

