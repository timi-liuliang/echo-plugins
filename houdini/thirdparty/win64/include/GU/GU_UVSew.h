/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	This class contains the methods to perform the UV Sew 
 *		operation.
 *
 */



#ifndef __GU_UVSew_h__
#define __GU_UVSew_h__

#include "GU_API.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_VectorTypes.h>
#include <GA/GA_Handle.h>
#include <GEO/GEO_PrimClassifier.h>

class UT_BitArray;
class GA_EdgeGroup;
class GA_Primitive;
class GA_VertexGroup;
class GEO_Delta;
class GEO_Primitive;
class GEO_PrimPoly;
class GEO_PrimConnectorUV;
class GU_Detail;
class gu_SuperVertex;
class gu_VertexTransform;
class gu_ClassEdges;
template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

class GU_API GU_UVSew
{
public:

    GU_UVSew(GU_Detail &gdp, GEO_Delta *geodelta, const GA_RWHandleV3 &uvattrib);
    ~GU_UVSew();

    // The sew() method is the workhorse of this class.  Given an edge group,
    // this method will perform the work necessary to sew vertex UV's sharing
    // the same points.
    void		  sew(const GA_EdgeGroup *edge_grp);

protected:
    // Helper method that sews an edge, this time the from and to polygon have
    // already been resolved.  They should both share the given edge.
    void		  sew(const GEO_PrimPoly &from_poly,
			      const GEO_PrimPoly &to_poly,
			      GA_Edge &edge);

    // Fills the supervertex array of the given primitive.  After this call
    // the primitive should have a super vertex assigned to each of its
    // vertices.
    void		  fillSuperVertexArray(const GEO_Primitive &prim);
    
    // This method is used to process all the vertices connected 
    // (uv-connectivity) to the super vertex svtx_idx.
    void		  processConnectedVertices(int svtx_idx,
						 const UT_Vector2 &translate,
						 float rotation,
						 const UT_Vector2 &scales,
						 const UT_Vector2 &pivot,
						 const UT_Vector3 &p0,
						 const UT_Vector3 &p1);

    // Returns the index of the supervertex containing vtx on prim.
    int			  getSuperVertexIndex(const GA_Primitive &prim, 
					      int vtx);

    // Helper method that adds prim/vtx to the super vertex indexed by svtx_idx.
    void		  insertVertexToSuperVertex(int svtx_idx, 
						    const GEO_Primitive &prim,
						    int vtx);
    

    // Convenience methods for retrieving UV coordinates from vertices and
    // supervertices.
    void		  getVertexUV(const GEO_Primitive &prim,
				      int vtx, UT_Vector3 &uv);
    bool		  getSuperVertexUV(int svtx_idx, UT_Vector3 &uv);


    // Adds the vertices in a super vertex to the given group.
    void		  addSuperVertexToGroup(int svtx_idx,
						GA_VertexGroup &group);


    // Apply all the accumulated transforms to each super vertex.
    void		  applyAndRemoveSuperTransforms();


    // Convenience methods for appending transforms to supervertices.
    void		  transformSuperVertex(int svtx_idx, 
					       const UT_Vector2 &translate,
					       float rotation,
					       const UT_Vector2 &scales,
					       const UT_Vector2 &pivots,
					       bool  locked,
					       float weight);
    void		  translateSuperVertex(int svtx_idx,
					       const UT_Vector2 &translate,
					       bool locked,
					       float weight);

    // Returns the list of super vertices that are connected to the given
    // super vertex.
    void		  getConnectedSuperVertices(int svtx_idx,
						    UT_IntArray &connected);

    // From the given edge group, this method sorts and partitions the edges
    // into classes (e.g. patches) and gets them ready to be processed in a
    // specific order.
    void		  buildMainClasses(const GA_EdgeGroup *edge_grp);

    // This is a helper method for builMainClasses, it goes through the list
    // of edges in the given edge group and determines populates the 
    // myMainClassEdges array with the corresponding main edges.
    void		  resolveToEdges(const GA_EdgeGroup *edge_grp);

    // Clears the super transforms array.
    void		  clearSuperTransforms();

    // Returns the main polygon that we should be moving the other polygons to.
    GEO_PrimPoly	 *getMainPoly(const GA_Edge &edge,
				      const GEO_Primitive *edge_prim,
				      const UT_IntArray &polys);

    // Rebuilds our connectivity information -- use sparingly.
    void		  resetConnectivityStructures();

private:
    GU_Detail			      &myGdp;
    GEO_Delta			      *myDelta;
    GA_RWHandleV3                      myVertexUVAttrib;
    GEO_PrimClassifier		       myUVClassifier;
    GEO_PrimConnectorUV		      *myUVConnector;
    UT_ValArray<gu_SuperVertex *>      mySuperVertices;
    UT_ValArray<gu_VertexTransform *>  mySuperTransforms;
    UT_Map<uint64, int>		       myVertexToSuperVertexTable;
    UT_ValArray<gu_ClassEdges *>       myMainClassEdges;
};

#endif
