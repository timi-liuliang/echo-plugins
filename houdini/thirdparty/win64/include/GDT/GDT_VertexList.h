/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	GDT_VertexList contains tokens for vertex attributes.
 *		All these tokens are not sparse.
 *
 */
#ifndef __GDT_VertexList_h__
#define __GDT_VertexList_h__

#include "GDT_API.h"
#include "GDT_TokenList.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_ValArray.h>
#include <GEO/GEO_Detail.h>

class GDT_API GDT_VertexList : public GDT_TokenList
{
public:
    GDT_VertexList();
    virtual ~GDT_VertexList() {}

    // Implement the pure virtual applyDeltas() from GDT_TokenList.
    virtual void	 applyDeltas(GEO_Detail &gdp, bool inverse=false) const;

    // Merge vertex deltas from another GDT_VertexList
    void		 mergeVertexDeltas(const GDT_VertexList &other,
					   bool inverse = false);

    // Apply a scaled delta...
    void		 applyScaledDelta(GEO_Detail &gdp, GA_Offset vtxoff,
                                          float scale) const;

    // Overload hiding base class method which requires a GA_AIFEditDeltaMap.
    void		 endAttributeChange(const GEO_Detail &gdp);
    
    // Compute the vertex offsets array.
    // This array must always be in sync with the gdp,
    // otherwise endVertexAttributeChange calls will fail.
    void		 updateVertexOffsets(const GEO_Detail &gdp);
	
    virtual const GA_AttributeDict   &getAttributeDict(
						const GEO_Detail &gdp) const
						{ return gdp.vertexAttribs(); }

    bool			saveOffsets(UT_JSONWriter &w) const;
    bool			loadOffsets(UT_JSONParser &p);

    virtual bool		legacyLoadAttribs(UT_IStream &is, uint size);

    // Destroy the vertex deltas
    virtual void		destroy();

private:
    // myOffsets is used as a way to flatten the vertex indices.  A cell at
    // index i of myOffsets contains the total number of vertices up to, 
    // but not including the primitive i.  For example, myOffsets[0] is always
    // 0.  If primitive 0 had 4 vertices and primitive 1 had 2 then myOffsets[1]
    // would be 4 and myOffsets[2] would be 6, and so on.  That way it is
    // very efficient to index a vertex based on a vertex number and primitive
    // number, it's index would be myOffsets[primnum] + vertexnum.
    // By its nature the offsets array is sorted, so to compute the inverse,
    // that is to find the primitive and vertex index given and index into
    // the vertex array, you just have to do a binary search.  The last
    // entry in the array contains the total number of vertices in the gdp.
    // 
    // Note that this array should be treated as though it were part of the
    // delta.  Since we don't keep around a gdp, this array might not coincide
    // with the gdp that we have to apply the delta to.  This should be treated
    // the same way we treat topology changes in the other lists.
    UT_ValArray<GA_Index>	myOffsets;
};
#endif
