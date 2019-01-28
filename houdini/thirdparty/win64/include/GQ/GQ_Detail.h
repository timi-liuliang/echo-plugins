/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Quad Edge Library (C++)
 *
 */

#ifndef _GQ_Detail_h_
#define _GQ_Detail_h_

#include "GQ_API.h"
#include "GQ_Error.h"
#include "GQ_Edge.h"

#include <GU/GU_PrimPoly.h>
#include <GU/GU_RayIntersect.h>
#include <GU/GU_Types.h>
#include <GEO/GEO_Primitive.h>
#include <GEO/GEO_WorkVertexBuffer.h>
#include <GA/GA_AttributeRefMap.h>

#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Plane.h>
#include <UT/UT_Array.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>

class GU_Detail;
class GQ_Face;
class GQ_Point;
class GEO_PrimPoly;
class GA_PointWrangler;
class GA_PrimitiveGroup;
class GA_EdgeGroup;
class GA_WorkVertexBuffer;
class GQ_StitchParms;
class GQ_SubdivideParms;
class gq_SubdivideAttribHandler;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

enum GQ_BooleanOpType
{
    GQ_BOOLEAN_INTERSECT        = 0,
    GQ_BOOLEAN_UNION            = 1,
    GQ_BOOLEAN_A_MINUS_B        = 2,
    GQ_BOOLEAN_B_MINUS_A        = 3,
    GQ_BOOLEAN_A_EDGE           = 4,
    GQ_BOOLEAN_B_EDGE           = 5 
};  

/// Quad Edge data structure for topological manipulation of polygonal data.
///
/// The Quad Edge data structure was invented by Guibas & Stolfi in their
/// 1985 paper, "Primitives for the Manipulation of General Subdivisions and
/// the Computation of Voronoi Diagrams", ACM Transactions on Graphics,
/// 4(2):74-123, April 1985.
///
/// That paper has a good description of the capabilities of the data
/// structure, and the definition of its basic operators, makeedge and splice().
class GQ_API GQ_Detail : UT_NonCopyable
{
public:
    /// The super_point_tolerance should probably be set to -1 which flags
    /// for no super points to be generated.  Super points often cause
    /// confusing (for the caller) results.
				 GQ_Detail(GU_Detail *gdp,
					   GA_PrimitiveGroup * = 0,
					   fpreal super_point_tolerance = 1E-6);
				~GQ_Detail();

    void			 clearAndDestroy(void);
    /// Cusp polygons which have adjacent edges larger than angle (in degrees)
    void                         cusp(fpreal angle, bool no_cut = false);

    /// Cusp edges in a group
    void			 cusp(const GA_EdgeGroup &edges,
				      bool do_cut = false);


    /// Both clip() and crease() will invalidate this GQ_Detail. If you wish
    /// to do another GQ_Detail operation, create a new GQ_Detail.
    /// {
    void			 clip(UT_Vector3 &norm, fpreal distance,
				      int normalize);
    int				 crease(UT_Vector3 &norm, fpreal distance,
					int normalize, int outputGroups,
                                        GA_PrimitiveGroup *primitive_above=0, 
                                        GA_PrimitiveGroup *primitive_below=0);
    /// }
    
    void                         bricker(fpreal sizex = 1.0, fpreal sizey = 1.0,
                                         fpreal sizez = 1.0, fpreal offx = 0.0,
                                         fpreal offy = 0.0, fpreal offz = 0.0,
                                         fpreal anglex = 0.0, fpreal angley = 0.0,
                                         fpreal anglez = 0.0,
					 bool fixsharededges = false);
    void			 smooth(int divisions, fpreal relativeSize,
					fpreal weight);

    // Adds edge weights to the GQ_Detail. (If not already present)
    void			 createEdgeWeights();

    // This is used to set up the edge weights:
    // To read edgeweights, use getEdgeWeight
    // The following method assumes that poly is not in the GQ_Detail.  It can
    // be, but it is not a requirement.
    void			 setCreaseWeight(const GEO_PrimPoly &poly,
						fpreal weight);
    // With the following method the poly can be in the GQ_Detail or in another
    // gdp.  It requires the creaseweight attribute on the polygon or on the
    // vertices of the polygon.  If either the vtxoff or primoff is >= 0, that
    // attribute will be used...
    void			 setCreaseWeight(const GEO_PrimPoly &poly,
                                                GA_ROHandleF vtxattrib,
                                                GA_ROHandleF primattrib);

    // Given a GA_Edge, we can map this to a GQ_Edge and set the corresponding
    // weight.  A GU_Detail is required to look up point indices from the edge
    // since the GA_Edge may not be from the GQ_Detail.
    void			 setCreaseWeight(const GA_Edge &edge,
    						const GU_Detail &edge_gdp,
    						fpreal weight);

    // This is similar to the setCreaseWeight method for GEO_PrimPoly,
    // but the edge supplied is the only one affected.  This edge must
    // belong to a poly (so that the attribute can be grabbed).
    void			 setCreaseWeight(const GA_Edge &gedge,
					const GA_Primitive &prim,
                                        GA_ROHandleF vtxattrib,
                                        GA_ROHandleF primattrib);
    

    fpreal 			 getEdgeWeight(const GA_Edge &edge);
    fpreal 			 getEdgeWeight(exint edge_index) const;
    bool			 setEdgeWeight(exint edge_index,
					       fpreal weight);
    void			 subdivide(const GQ_SubdivideParms &parms,
				    GA_PrimitiveGroup *nonsubdivprims);
    void			 dual(const char *attribs_to_swap=NULL);
    void			 stitch(const GQ_StitchParms &parms);
    int 			 stitchEdges(GEO_PrimPoly &polya,
				    const UT_Array<GEO_PrimPoly *> &facea,
				    GEO_PrimPoly &polyb,
				    const UT_Array<GEO_PrimPoly *> &faceb,
				    GA_PrimitiveGroup &changedpolys,
				    fpreal tol, int clamp, int consolidate);
    void			 makeWire(fpreal radius, int doSpheres,
					  int doCaps);
    void			 buildGeometry(GA_PrimitiveGroup *output = NULL);
    void			 buildCreases(GA_PrimitiveGroup *group = 0);
    void			 copyEdgeWeightToVertex();
    void			 unHole(int maintain);
    void			 boundary();
    void			 createCuspGroup(GA_Group *group, 
						 bool doMinAngle, fpreal minAngle, 
						 bool doMaxAngle, fpreal maxAngle);
    void			 createBoundaryGroup(GA_Group *grp);
    void			 createBoundaryGroup(GA_PointGroup &grp,
						     UT_IntArray *arr);
    void			 createBoundaryGroup(GA_Offset ppt,
						     GA_PointGroup &pointgroup);
    void			 createBoundaryGroup(const GA_Edge  &edge,
						     GA_PointGroup  &grp,
						     GEO_PrimPoly  *&poly);
    /// For each boundary loop a separate UT_IntArray is added to the ptlist.
    /// These intarrays will contain the point numbers, in order, of the
    /// points that belong to those boundaries.
    /// This is preferable to the group based approach becausae it can
    /// handle duplicate points that show up on the boundary of certain
    /// degenerate geometry..
    void			 createBoundaryList(UT_Array<UT_IntArray> &ptlist);
					    

//  Creates a point group of those points which are adjacent by the certain
//  given number of edges.
    void                         groupEdgePoints(GA_Offset ptoff,
						 int depth,
                                                 GA_PointGroup &point_group);

    int				 isClose();
				 
    // deleteAllShareEdges is only for testing deleteShareEdge
    // which will be used for polygon reduction.
    // Don't use it for deleting all shared edges (ie. want only the boundary)
    void			 deleteAllShareEdges();


    GQ_Point			*appendPoint(const GQ_Point *src=0);
    GQ_Edge			*appendEdge()
    {
        GQ_Edge *Q = new GQ_Edge[4];
        int idx = myEdges.append(Q);
        for (int i = 0; i < 4; i++)
            Q[i].init(i + idx * 4);
        return Q;
    }
    GQ_Face			*appendFace(GEO_PrimPoly *poly);
    void			 removePoint(GQ_Point *p);
    void			 removePoint(int i);
    void			 removeEdge(GQ_Edge *e);
    void			 removeEdge(int i);

    // removeFace(GQ_Face* f) ensures that the face f is not in the detail
    // after the call. It returns whether the face was present before the call.
    bool			 removeFace(GQ_Face *f);

    void			 removeFace(int i);

    void			 collapseEdgeList() { myEdges.collapse(); }
    void			 collapseFaceList() { myFaces.collapse(); }
    UT_Array<GQ_Point *>        &getPointList()	    { return myPoints; }
    const UT_Array<GQ_Point *>  &getPointList() const { return myPoints; }
    UT_ValArray<GQ_Edge *>      &getEdgeList()	    { return myEdges; }
    const UT_ValArray<GQ_Edge *>&getEdgeList() const{ return myEdges; }
    UT_DoubleArray		&getEdgeWeights()   { return myEdgeWeights; }
    const UT_DoubleArray	&getEdgeWeights() const { return myEdgeWeights; }
    UT_Array<GQ_Point *>	&getEdgePoints()    { return myEdgePoints; }
    const UT_Array<GQ_Point*>   &getEdgePoints() const { return myEdgePoints; }

    UT_ValArray<GQ_Face *>      &getFaceList()	    { return myFaces; }
    const UT_ValArray<GQ_Face *>&getFaceList() const{ return myFaces; }
    GU_Detail			*getGdp()	    { return myGdp; }
    const GU_Detail		*getGdp() const	    { return myGdp; }
    GU_RayIntersect		*getRay()	    { return myRay; }
    void			 buildRay()
				 {
				    if (myRay) delete myRay;
				    myRay = new GU_RayIntersect(myGdp, myGroup);
				 }
    UT_Array<GA_RWAttributeRef> &getOffsets()       { return myOffsets; }
    

    /// Split edge will return 0 if t is too close to 0 or 1.
    GQ_Point			*splitEdge(GQ_Edge *edge, fpreal t);
    GQ_Edge			*splitEdge(GQ_Edge *, GQ_Point &pt);
    void			 deleteShareEdge(GQ_Edge *edge, 
					 GA_PrimitiveGroup *deletePrimGroup=0);

    void			 wireEdge(GQ_Edge *edge, fpreal radius,
					  int doCaps);
    void			 pointSphere(GQ_Point *pt, fpreal radius);

    int                  	 nEdges(void) const { return myEdges.entries();}
    int	               	         nFaces(void) const { return myFaces.entries();}
    int                  	 nPoints(void) const{return myPoints.entries();}
    GU_Detail           	*getDetail(void) const { return myGdp; }
    // Only test a point of face
    int				 aboveOrBelow(GQ_Face *face);

    // Methods used by decimation
    void			 simpleDecimate(int targetPolys);
    void			 decCollapse(GQ_Edge *e, GA_PrimitiveGroup *);
    void			 decSplit(GQ_Edge *e);
    void			 decSwap(GQ_Edge *e);
    // End of the decimation methods

    void			 save(std::ostream &os) const;
    friend std::ostream		&operator<<(std::ostream &os, const GQ_Detail &d)
				 { d.save(os); return os; }

    /// Given a GA_Edge, return the equivalent GQ_Edge. Relatively
    /// efficient: O(degree(org)), not O(nedges)
    GQ_Edge			*findEdge(const GA_Edge *edge,
					  const GU_Detail *edge_gdp);
    GQ_Edge			*findEdge(const GA_Edge *edge)
					    { return findEdge(edge, myGdp); }
    GQ_Edge			*findEdge(const GQ_Point *org, const GQ_Point *dest);
    GA_Offset			 stepForwardOffset(const GA_Edge *edge);

    //Returns the first point encountered connected to the org, otherwise
    //GA_INVALID_OFFSET
    GA_Offset			 pickArbitraryConnectedPointOffset(GA_Offset org);
    
    //Rotates the edge around a specified origin and resturns the new
    //destination point
    GA_Offset			 rotateAboutOriginOffset(GA_Edge &edge, int dir);

    // Returns the face that is adjacent to 'face' on the edge indexed by 'edge'
    GEO_PrimPoly		*adjacentFace(const GEO_Face &face, int edge);

private:
    void			 snapToPlane(UT_Vector3 &norm, fpreal dist);
    void			 buildSuperPoints(fpreal distance,
                                                const GA_PointGroup *usedpts);
    /// NOTE: sp and spd1 must both be superpoints!
    GQ_Edge			*findEdgeToShare(GQ_Point *sp, GQ_Point *spd1);
    GA_PrimitiveGroup 		*convertToQuadEdge(GA_PrimitiveGroup *,
						   fpreal sp_tol=0.001F);
    void			 cleanEdgeToPt(GQ_Point *p);
    int 			 buildFace(GQ_Face *face);
    GA_Size                  	 getVertexNum(const GQ_Face *face,
					      GA_Offset prevPt,
					      GA_Offset pt) const;
    void			 creaseFace(GQ_Face *face, UT_Vector3 &norm,
					    fpreal dist, 
					    int outputGroups,
					    GA_PrimitiveGroup *prim_above = 0,
					    GA_PrimitiveGroup *prim_below = 0);
    void 			 createCreaseFace(GQ_Edge *edge,
					  UT_Vector3 &norm, fpreal dist,
					  int outputGroups,
					  GA_PrimitiveGroup *prim_above = 0,
					  GA_PrimitiveGroup *prim_below = 0);
    void			 addIntersectEdges(GQ_Face *face,
						   UT_Vector3 &normal);

    /// Follows e->lnext() checking if e->org() has flag set, returns true
    /// if it does.  The nonBridge component means when it hits a bridge
    /// it doesn't follow it, but runs e->oprev() instead.
    bool			 nonBridgeLoopHasFlag(GQ_Edge *e, int flag);

    /// Removes all the bridges on the face that are no longer necessary
    /// if we connect together points that are GQ_SELECTED.
    /// The bridges array is filled with any bridge edges that have
    /// GQ_INTERSECT flag and weren't deleted.  (It is hard to get
    /// this afterwards since you'll no longer have a single face)
    void 			 removeAllPossibleBridges(GQ_Face *face,
					UT_Array<GQ_Edge *> &bridges);
    GQ_Edge *			 removePossibleBridge(GQ_Edge *bridgeEdge,
					bool hasisect,
					int depth,
					UT_Array<GQ_Edge *> &bridges);
    void			 reBridge(UT_Array<GQ_Edge *> &bridges);
    void			 reBridgeForCrease(UT_Array<GQ_Edge *>
							    &intersectBridges);
    UT_Array<GQ_Face *> 	*split(UT_Vector3 &normal, fpreal distance,
				       int outputGroups = 0,
				       GA_PrimitiveGroup *prim_above = 0,
				       GA_PrimitiveGroup *prim_below = 0);
    void			 expandPoint(GQ_Point *pt);
    void			 plugGap(GQ_Edge *edge);
    void			 shrinkFace(GQ_Face *face, fpreal a, fpreal b);
    void			 unHole(GQ_Face *face, int maintain);
    GQ_Edge *			 unBridge(GQ_Edge *bridgeEdge, int maintain);
    void			 changeGeoPoint(GQ_Face *face, GA_Offset opt, 
						GA_Offset npt, bool all);
    void			 uniqueEdge(GQ_Edge *edge);
    void			 uniquePoint(GQ_Point *point);
    void			 cleanFace(GQ_Face *face);
    int				 twoFacesShareAllEdges(GQ_Edge *edge, 
					   GA_PrimitiveGroup *deletePrimGroup);

    // Semi-smooth Catmull-Clark subdivision:
    void		 calcFacePoints(int numfaces);
    void		 calcEdgePoints(int numedges, 
					GA_Offset *oldEdgeAttribs,
					bool smoothvertex);
    void		 calcVertexPoints(int numpoints,
					GA_Offset *oldEdgeAttribs,
					GA_Offset *newVertex,
					bool smoothvertex);

    // This has one vertex attribute per point which will be copied in
    // to all the edge attributes comeing out of that point.
    void		 copyVertexAttributeByPoint(int numpoints,
					GA_Offset *newVertex,
					gq_SubdivideAttribHandler &ahandler);

    void		 copyEdgeVertexAttributes(int numedge);
    // This is used after the edge attributes are calculated.  It will
    // then set the GQ_VTXBOUNDARY appropriately for all points.
    void		 flagVertexBoundaries();
    void		 subdivideEdges(int numedges, bool linear);
    void		 subdivideFaces(int numfaces,
					gq_SubdivideAttribHandler &ahandler);

    // This copies the myEdgeAttribs into the geometrie's polygons:
    void		 copyVertexAttributes(int numedges,
					      gq_SubdivideAttribHandler &);

    void		 splitPolysWithMultipleHoles(GA_PrimitiveGroup *,
                            UT_Array<GQ_Point *> &,
			    gq_SubdivideAttribHandler &);
    GEO_PrimPoly	*findNonSubDivPolyWithEdge(GA_Offset,
			    GA_Offset, GA_PrimitiveGroup *, int &,
			    int &);
    GEO_PrimPoly	*findNonSubDivPolyWithEdge(GA_Offset,
			    GA_Offset, GA_PrimitiveGroup *);
    GQ_Edge		*findBoundEdge(const GQ_Point *, const GQ_Point *,
			    UT_Vector4 &, UT_Vector4 &, int &);
    int			 addSubDivBoundaryPoints(GQ_Edge *, const GQ_Point *,
			    GA_PointGroup *);
    void		 createVirtualEdgePoints(const UT_Vector4 &,
			    const UT_Vector4 &, GA_PointGroup *,
			    UT_Vector4Array &);
    void		 createActualEdgePoints(const UT_Vector4 &,
			    const UT_Vector4 &, GA_PointGroup *,
			    GA_PointGroup *, gq_SubdivideAttribHandler &);
    void		 triangulateNonSubDivPoly(GEO_PrimPoly *,
			    GA_PointGroup *, GA_PrimitiveGroup *,
			    GA_Offset, GA_Offset, GA_Offset, GA_Offset,
			    int, gq_SubdivideAttribHandler &);
    GEO_PrimPoly	*dividePolygonContainingEdge(GA_PointGroup *,
			    GA_Offset, GA_Offset,
			    GA_PrimitiveGroup *, int,
			    GA_Offset &, GA_Offset &,
			    gq_SubdivideAttribHandler &);
    void		 pullHolesUsingBias(GA_PointGroup *,
			    UT_Vector4Array &, fpreal);
    void		 stitchHoles(GA_PointGroup *, GA_PointGroup *,
			    GEO_PrimPoly *, int, gq_SubdivideAttribHandler &);
    void		 pullToEndPoints(GA_PointGroup *, GA_Offset, GA_Offset);
    void		 closeHolesAfterSubdivide(UT_Array<GQ_Point *> &,
			    GA_PrimitiveGroup *, const GQ_SubdivideParms &,
			    gq_SubdivideAttribHandler &);

    // Goes through the edges of p and modifies vertex attributes of edges
    // with a GQ_CUSP flag to make them cusp'd
    void cuspPoint(GQ_Point *p, const GA_RWHandleV3 &normal_attrib, int i);

    // Create a point wrangler just-in-time
    GA_PointWrangler	&pointWrangler();

    // Create a vertex wrangler just-in-time
    GA_VertexWrangler	&vertexWrangler();

private:
    
    UT_ValArray<GQ_Edge *>	 myEdges;
    UT_DoubleArray		 myEdgeWeights;
    UT_Array<GQ_Point *>	 myEdgePoints;
    UT_ValArray<GQ_Face *>	 myFaces;
    UT_Array<GQ_Point *>	 myPoints;
    UT_Array<GA_RWAttributeRef>	 myOffsets;
    GA_PrimitiveGroup		*myGroup; // Primitive group
    GU_Detail			*myGdp;
    GU_RayIntersect		*myRay;

    GA_PointWrangler		*myPointWrangler;
    GA_VertexWrangler		*myVertexWrangler;

    // This data is used by subdivide:
    // For most of subdivide, myEdgeAttribs maps edge indices into vertex data,
    // but in the edge-closing situation it is reinterpreted to map
    // point numbers into vertex data.
    GA_AttributeRefMap		 myHandles;
    GA_WorkVertexBuffer		*myEdgeVertices;
    GA_WorkVertexBuffer		*myFaceVertices;
};

#endif
