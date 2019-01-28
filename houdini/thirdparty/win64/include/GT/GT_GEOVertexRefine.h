/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOVertexRefine.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOVertexRefine__
#define __GT_GEOVertexRefine__

#include "GT_API.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <GA/GA_Types.h>
#include "GT_Handles.h"

class GT_GEOAttributeFilter;
class GT_GEOPrimitive;
class GEO_Primitive;
class GA_Detail;
class GA_Attribute;
class GA_AttributeDict;
class GA_AttributeRefMap;
class GEO_Curve;

/// Refine points/vertices into unique data arrays (not shared by the detail).
///
/// This class can be used to create point/vertex attributes for refined
/// primitives.  The way to use this would be to: @code
///     GT_GEOPrimitive		srcprim(initialize);
///     GT_GEOAttributeFilter	filter;
///     GT_GEOVertexRefine	vrefine(srcprim, filter, true, 3);
///     for (int seg = 0; seg < srcprim.getMotionSegments(); ++seg)
///	{
///	     const GEO_Primitive	*prim;
///	     GA_AttributeRefMap	hlist;
///	     prim = vrefine.bindAttributeHandle(seg, hlist);
///	     vertex_offsets = getRefinedVertices();
///	     for (exint v = 0; v < nvertices; ++v)
///		prim->evaluate(parms, vertex_offsets[v], hlist);
///	     vrefine.extractVertices(seg, hlist, vertex_offsets, nvertices);
///	}
/// @endcode
class GT_API GT_GEOVertexRefine
{
public:
    /// Create an attribute list for vertices (and optionally points).
    /// The @c psize specifies the tuple size for P and is only valid when
    /// adding point attributes too.
    /// If @c user_defined_N is true, the N attribute will be added if it's not
    /// found on points/vertices.  Also, the N attribute will @b not be picked
    /// up for refinement (relying on the user to set the values properly).
    ///
    /// This creates the attribute handle list.  Refinement of vertex
    /// attributes is a separate process.
    GT_GEOVertexRefine(const GT_GEOPrimitive &prim,
		const GT_GEOAttributeFilter &filter,
		bool do_point_attributes,
		int psize=3,
		bool user_defined_N=false);
    ~GT_GEOVertexRefine();

    /// Get the attribute handle
    const GT_AttributeListHandle	&getGTAttributes() const
					    { return myGTAttributes; }

    /// Bind the attributes to the attribute handle list.  The handle list
    /// should be cleared of entries before calling this.
    /// The method returns the primitive associated with the segment.
    GA_AttributeRefMap	bindAttributeHandle(int segment);

    /// Extract the attribute values from the vertices, creating GT data arrays
    /// for the attribute data.
    /// This fills out the data arrays on the attribute list handle
    void		extractVertices(int segment,
				const GA_AttributeRefMap &hlist,
				const GA_Offset *vertices,
				GA_Size nvertices);

    /// Fill attributes for a curve
    void		fillCurveVertices(int segment,
				const GEO_Curve *curve,
				const UT_FloatArray &uvals);

    /// Set the N attribute for vertices.  This should only be called if @c
    /// user_defined_N was set in the constructor.  It must be called @b after
    /// @c extractVertices.
    void		setN(int segment,
				const UT_Vector3 *N,
				GA_Size nvertices);

private:
    bool		addAttribute(GT_AttributeMap &map,
				const GA_Attribute *attrib);
    void		addAttributes(GT_AttributeMap &map,
				const GA_AttributeDict &dict,
				const GT_GEOAttributeFilter &filter);
    const GA_Attribute	*getDetailAttribute(const GA_Detail *gdp, int idx);

    const GT_GEOPrimitive		*myPrimitive;
    GT_AttributeListHandle		 myGTAttributes;
    UT_ValArray<const GA_Attribute *>	 myGAAttributes;
    UT_IntArray				 myGTOffsets;
    int					 myPSize;
    int					 myNOffset;
    bool				 myUserN;
};

#endif
