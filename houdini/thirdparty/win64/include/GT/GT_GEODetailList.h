/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEODetailList.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEODetailList__
#define __GT_GEODetailList__

#include "GT_API.h"
#include <UT/UT_SharedPtr.h>
#include "GT_Handles.h"
#include "GT_Types.h"
#include "GT_FaceSetMap.h"
#include <GA/GA_Types.h>
#include <GU/GU_DetailHandle.h>
#include <GEO/GEO_Normal.h>

class GEO_Primitive;
class GT_GEOOffsetList;
class GT_GEOAttributeFilter;

/// @brief Keeps a list of GU_Detail pointers
class GT_API GT_GEODetailList
{
public:
    /// Default constructor
    GT_GEODetailList()
	: myList(myBuffer)
	, mySize(0)
	, myPrimitiveId(GA_INVALID_OFFSET)
	, myVertexId(GA_INVALID_OFFSET)
	, myPointId(GA_INVALID_OFFSET)
    {}
    /// Useful constructor
    GT_GEODetailList(const GU_ConstDetailHandle *list, int size)
	: myList(NULL)
	, mySize(0)
	, myPrimitiveId(GA_INVALID_OFFSET)
	, myVertexId(GA_INVALID_OFFSET)
	, myPointId(GA_INVALID_OFFSET)
    {
	copyFrom(list, size);
    }
#if 0
    GT_GEODetailList(const GU_ConstDetailHandle *detail_list,
			const GEO_Primitive *const* list,
			int size)
	: myList(NULL)
	, mySize(0)
	, myPrimitiveId(GA_INVALID_OFFSET)
	, myVertexId(GA_INVALID_OFFSET)
	, myPointId(GA_INVALID_OFFSET)
    {
	copyFrom(detail_list, list, size);
    }
#endif
    GT_GEODetailList(const GU_ConstDetailHandle &gdp)
	: myList(NULL)
	, mySize(0)
	, myPrimitiveId(GA_INVALID_OFFSET)
	, myVertexId(GA_INVALID_OFFSET)
	, myPointId(GA_INVALID_OFFSET)
    {
	copyFrom(&gdp, 1);
    }
    /// Copy constructor
    GT_GEODetailList(const GT_GEODetailList &src)
	: myList(NULL)
	, mySize(0)
	, myPrimitiveId(src.myPrimitiveId)
	, myVertexId(src.myVertexId)
	, myPointId(src.myPointId)
    {
	copyFrom(src.myList, src.mySize);
    }
    ~GT_GEODetailList()
    {
	clear();
    }

    /// Assignment operator
    GT_GEODetailList	&operator=(const GT_GEODetailList &src)
			{
			    if (this != &src)
			    {
				clear();
				copyFrom(src.myList, src.mySize);
				myPrimitiveId = src.myPrimitiveId;
				myVertexId = src.myVertexId;
				myPointId = src.myPointId;
			    }
			    return *this;
			}

    /// Return number of details in the list
    int			entries() const
			    { return mySize; }

    /// Return number of details in the list
    int			getMotionSegments() const
			    { return mySize; }

    /// @{
    /// Access to primitive/vertex/point offset overrides
    GA_Offset	primitiveId() const	{ return myPrimitiveId; }
    void	setPrimitiveId(GA_Offset offset)
		    { myPrimitiveId = offset; }
    GA_Offset	vertexId() const	{ return myVertexId; }
    void	setVertexId(GA_Offset offset)
		    { myVertexId = offset; }
    GA_Offset	pointId() const	{ return myPointId; }
    void	setPointId(GA_Offset offset)
		    { myPointId = offset; }
    /// @}

    /// Return a list of all the GU_ConstDetailHandles associated with this list
    GU_ConstDetailHandle	*detailHandles() const { return myList; }

    /// Return a specific element
    const GU_ConstDetailHandle	&getGeometry(int segment) const
				{ return myList[SYSmin(segment, mySize-1)]; }

    /// () operator
    const GU_ConstDetailHandle	&operator()(int segment) const
				    { return getGeometry(segment); }

    /// Fill the primitive list for the given offset.  Returns false if there
    /// are mis-matched primitives:
    ///	- The primitives have different factories
    /// - The detail has different 
    bool		fillPrimitiveSegments(const GEO_Primitive **list,
				    GA_Offset prim_offset) const;

    /// Create a data array containing the value of primitive intrinsic
    /// attributes.  The code handles the case where the attribute name is
    /// prefixed with "intrinsic:".  The method uses the __primitive_id to
    /// perform the value lookup.
    static GT_DataArrayHandle	fillPrimitiveIntrinsic(
					const GU_Detail &gdp,
					const char *intrinsic_name,
					const GT_Primitive &prim);

    /// Check intrinsic information for a given primtiive.  Like
    /// fillPrimitiveIntrinsic(), this uses the __primtiive_id to perform the
    /// checks.
    static bool			findPrimitiveIntrinsic(
					const GU_Detail &gdp,
					const char *intrinsic_name,
					const GT_Primitive &prim,
					GT_Storage &storage,
					int &tuple_size);

    /// Class to traverse over the geometry for each segment
    class GT_API iterator
    {
    public:
	iterator() : myList(0), myCurr(0), mySize(0) {}

	iterator	&operator=(const iterator &src)
			 {
			     myList = src.myList;
			     myCurr = src.myCurr;
			     mySize = src.mySize;
			     return *this;
			 }

	void	rewind()	{ myCurr = 0; }
	void	advance()	{ myCurr++; }
	bool	atEnd() const	{ return myCurr >= mySize; }
	iterator	&operator++()	 { advance(); return *this; }
	// No post increment as it is dangerous.

	int			getSegment() const
				    { return myCurr; }
	const GU_ConstDetailHandle	&getGeometry() const
				    { return myList->getGeometry(myCurr); }
    private:
	iterator(const GT_GEODetailList *list)
	    : myList(list), myCurr(0), mySize(myList->entries())
	{
	}
	const GT_GEODetailList	*myList;
	int			 myCurr;
	int			 mySize;
	friend class GT_GEODetailList;
    };
    iterator	begin()	const	{ return iterator(this); }

    /// Whether to include point attributes when creating vertex attribute
    /// lists
    enum GT_GEOIncludePoint
    {
	GEO_SKIP_POINT		= false,
	GEO_INCLUDE_POINT	= true,
    };
    /// Whether to include primitive attributes when creating vertex attribute
    /// lists
    enum GT_GEOIncludePrimitive
    {
	GEO_SKIP_PRIMITIVE	= false,
	GEO_INCLUDE_PRIMITIVE	= true,
    };
    /// Whether to include detail attributes when creating vertex/primitive
    /// attribute lists
    enum GT_GEOIncludeDetail
    {
	GEO_SKIP_DETAIL		= false,
	GEO_INCLUDE_DETAIL	= true,
    };

    /// Get an attribute handle list for point attributes
    GT_AttributeListHandle	getPointAttributes(
				    const GT_GEOAttributeFilter &filter,
				    const GT_GEOOffsetList *pt_offsets=NULL,
				    bool add_point_id_attribute=false,
                                    bool add_point_normals_if_missing=false
				) const;

    /// Get an attribute handle list for vertex attributes
    GT_AttributeListHandle	getVertexAttributes(
				    const GT_GEOAttributeFilter &filter,
				    const GT_GEOOffsetList *vtx_offsets=NULL,
				    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
                                    GA_AttributeOwner add_normals_to_if_missing=GA_ATTRIB_INVALID,
                                    float cusp_angle=GEO_DEFAULT_ADJUSTED_CUSP_ANGLE
				) const;
    /// Get attribute handle for the primitive attributes
    GT_AttributeListHandle	getPrimitiveAttributes(
				    const GT_GEOAttributeFilter &filter,
				    const GT_GEOOffsetList *prim_offsets=NULL,
				    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
				    bool add_topology_data_id=true,
				    GT_DataArrayHandle mat_id = NULL,
				    GT_DataArrayHandle mat_remap = NULL
				) const;
    /// Return the attribute list for primitives that have a single vertex.
    GT_AttributeListHandle	getPrimitiveVertexAttributes(
				    const GT_GEOAttributeFilter &filter,
				    const GT_GEOOffsetList &prim_offsets,
				    const GT_GEOOffsetList &vertex_offsets,
				    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
				    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
				    GT_DataArrayHandle mat_id = NULL,
				    GT_DataArrayHandle mat_remap = NULL
				) const;


    /// Get the attribute list handle for the detail attributes
    GT_AttributeListHandle	getDetailAttributes(
					const GT_GEOAttributeFilter &filter,
					bool add_topology_data_id=true,
					GT_DataArrayHandle mat_id = NULL,
					GT_DataArrayHandle mat_remap = NULL
				) const;

    /// Build face sets.  Please see GT_RefineParms for the values for
    /// faceset_mode
    GT_FaceSetMapPtr		buildFaceSets(
				    const GT_GEOOffsetList &prim_offsets,
				    int faceset_mode
				) const;

    /// Create vertex attributes for a list of primitives for *this* geometry
    GT_AttributeListHandle	createVertex(const GEO_Primitive *const*list,
				    int nsegments,
				    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
				    const GT_GEOAttributeFilter *f=NULL
				) const
    {
	return createVertexAttributes(myList, list, nsegments, pt, f);
    }
    /// Create primitive & vertex attributes for a list of primitives of *this*
    /// geometry.
    GT_AttributeListHandle	createPrimitiveVertex(
				    const GEO_Primitive *const* list,
				    int nsegments,
				    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
				    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
				    const GT_GEOAttributeFilter *f=NULL
				) const
    {
	return createPrimitiveVertexAttributes(myList, list,
		nsegments, pt, det, f);
    }
    /// Create primitive attributes for a list of primitives for *this*
    /// geometry, optionally including detail attributes for the list.
    GT_AttributeListHandle	createPrimitive(
				    const GEO_Primitive *const* list,
				    int nsegments,
				    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
				    bool add_topology_data_id=true,
				    const GT_GEOAttributeFilter *f=NULL
				) const
    {
	return createPrimitiveAttributes(myList, list, nsegments, det,
				    add_topology_data_id, f);
    }
    /// Create detail attributes for *this* geometry
    GT_AttributeListHandle	createDetail(
				    const GEO_Primitive *const* list,
				    int nsegments,
				    bool add_topology_data_id=true,
				    const GT_GEOAttributeFilter *f=NULL
				) const
    {
	return createDetailAttributes(myList, list, nsegments,
				    add_topology_data_id, f);
    }


    /// Create vertex attributes for a list of primitives.  This will verify
    /// that all primitives have the same number and offsets of vertices.
    /// If this is not the case, the method will return an empty handle.
    /// If the user requests to @c include_point_attributes, these attributes
    /// will be included in the list of returned attributes.
    static GT_AttributeListHandle	createVertexAttributes(
					    const GU_ConstDetailHandle *dlist,
					    const GEO_Primitive *const* list,
					    int nsegments,
					    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
					    const GT_GEOAttributeFilter *f=NULL
					);
    /// Create primitive & vertex attributes for a list of primitives.  Each
    /// primitive should have a single vertex.  If the user requests to @c
    /// include_point_attributes, these attributes will be included in the list
    /// of returned attributes.
    static GT_AttributeListHandle	createPrimitiveVertexAttributes(
					    const GU_ConstDetailHandle *dlist,
					    const GEO_Primitive *const* list,
					    int nsegments,
					    GT_GEOIncludePoint pt=GEO_SKIP_POINT,
					    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
					    const GT_GEOAttributeFilter *f=NULL,
					    GT_DataArrayHandle mat_id = NULL
					);
    /// Create primitive attributes for a list of primitives, optionally
    /// including detail attributes for the list.
    static GT_AttributeListHandle	createPrimitiveAttributes(
					    const GU_ConstDetailHandle *dlist,
					    const GEO_Primitive *const* list,
					    int nsegments,
					    GT_GEOIncludeDetail det=GEO_SKIP_DETAIL,
					    bool add_topology_data_id=true,
					    const GT_GEOAttributeFilter *f=NULL
					);
    /// Create a detail attribute list for a list of primitives.
    static GT_AttributeListHandle	createDetailAttributes(
					    const GU_ConstDetailHandle *dlist,
					    const GEO_Primitive *const* list,
					    int nsegments,
					    bool add_topology_data_id=true,
					    const GT_GEOAttributeFilter *f=NULL
					);
private:
    void		clear();
    void		copyFrom(const GU_ConstDetailHandle *list, int size);
#if 0
    void		copyFrom(const GU_ConstDetailhandle *detail_list,
				    const GEO_Primitive *const*list, int size);
#endif

    GU_ConstDetailHandle	 *myList;
    GU_ConstDetailHandle	  myBuffer[4];
    int				  mySize;
    GA_Offset			  myPrimitiveId;
    GA_Offset			  myPointId;
    GA_Offset			  myVertexId;
};

typedef UT_SharedPtr<GT_GEODetailList>	GT_GEODetailListHandle;

#endif
