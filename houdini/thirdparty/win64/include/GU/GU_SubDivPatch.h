/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SubDivPatch.h ( GU Library, C++)
 *
 * COMMENTS:	Patch-based subdivision shared between mantra and the
 *		subdivide SOP
 */

#ifndef __GU_SubDivPatch_h__
#define __GU_SubDivPatch_h__

#include "GU_API.h"
#include "GU_SubDivPacking.h"
#include <GA/GA_ElementGroup.h>
#include <GA/GA_OffsetList.h>
#include <GA/GA_Types.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <stddef.h>

class GU_Detail;
class GU_SubDivVertex;
class GU_SubDivPatch;
class GEO_Primitive;

///
/// A subdivision edge for user-driven subdivision.  The edge stores hard
/// edge information including weights and per-vertex hard flags.  Edges
/// are destroyed when either attached vertex is destroyed.
///

static const uint8	SUBD_HARD_EDGE		= 0x01;
static const uint8	SUBD_LINEAR_CREASES	= 0x02;

class GU_API GU_SubDivEdge
{
public:
	     GU_SubDivEdge(GU_SubDivVertex *v1, GU_SubDivVertex *v2,
			   const GU_SubDivEdge *parent = 0);
	    ~GU_SubDivEdge();

    /// Calculate the new subdivision point using the catmull-clark masks.
    void		 catclarkChild();
    GU_SubDivVertex	*getChild()
			 {
			     if (!myChild)
				 catclarkChild();
			     return myChild;
			 }

    /// Return the other vertex
    GU_SubDivVertex	*getOther(const GU_SubDivVertex *vtx) const
			 {
			     return (vtx == myVertex[0]) ?
				 myVertex[1] : myVertex[0];
			 }

    void		 setFlag(uint8 f)	{ myFlag |= f; }
    void		 clearFlag(uint8 f)	{ myFlag &= ~f; }
    bool		 getFlag(uint8 f) const	{ return myFlag & f; }

    void		 setWeight(fpreal weight) { myWeight = weight; }
    fpreal		 getWeight() const	{ return myWeight; }
    bool		 isHardVertex() const	{ return myHardVertex; }
    void		 initHardVertex(int size)
			 {
			     if (!myHardVertex)
				  myHardVertex = new uint8[size];
			 }
    void		 initHardVertex(const UT_IntArray &sizes)
			 {
			     if (!myHardVertex)
			     {
				 myHardVertex = new uint8[sizes.entries()];
				 for (exint i = 0; i < sizes.entries(); i++)
				     myHardVertex[i] = sizes(i);
			     }
			 }
    void		 setHardVertex(int i)
			 { myHardVertex[i] = getHardVertexSize(i) | 0x80; }
    void		 clearHardVertex(int i)
			 { myHardVertex[i] = getHardVertexSize(i); }
    bool		 getHardVertex(int i) const
			 { return myHardVertex ? myHardVertex[i] & 0x80 : 0; }
    int			 getHardVertexSize(int i) const
			 { return myHardVertex ? myHardVertex[i] & 0x7F : 0; }
    int			 getHardVertexCount(int vcount) const
			 {
			     int	i, off;
			     for (i = 0, off = 0; off < vcount; i++)
				 off += getHardVertexSize(i);
			     return i;
			 }

private:
    GU_SubDivEdge() {}

private:
    GU_SubDivVertex	*myChild;
    GU_SubDivVertex	*myVertex[2];
    uint8		*myHardVertex;
    fpreal		 myWeight;
    uint8		 myFlag;
};

///
/// A subdivision vertex for user-driven subdivision.  Vertices hold the
/// subdivision data as well as references to edges and faces.
///

static const uint8	  SUBD_VTX_COMPLETE	= 0x01;
static const uint8	  SUBD_VTX_VTXSET	= 0x02;
static const uint8	  SUBD_VTX_MARK		= 0x04;

class GU_API GU_SubDivVertex
{
public:
    static GU_SubDivVertex	*allocVertex(int valence, int datasize,
					     int vertexsize);
    static GU_SubDivVertex	*allocVertex(const GU_SubDivVertex &src);
    static void			 freeVertex(GU_SubDivVertex *vtx);
    
    void		 incRefCount()	{ myRefCount++; }
    void		 decRefCount()
			 {
			     myRefCount--;
			     UT_ASSERT(myRefCount >= 0);
			     if (myRefCount <= 0)
				 freeVertex(this);
			 }

    void		 setNum(GA_Index num)	{ myGeoNum = num; }
    GA_Index		 getNum() const		{ return myGeoNum; }

    /// Adds vtx at the specified position in my edge table.  If idx == -1,
    /// then add vtx to the next available position.  Returns the edge.
    GU_SubDivEdge	*setEdge(GU_SubDivVertex *vtx, int idx = -1,
				 GU_SubDivEdge *parent = 0);
    void		 setEdge(GU_SubDivEdge *edge, int idx)
			 { myEdges[idx] = edge; }
    void		 clearEdge(GU_SubDivEdge *edge);

    /// Remove an edge from this vertex at the given index.  This operation
    /// may change the valence.
    void		 removeEdge(int idx);

    /// Add the face to the specified position in my face table.  If idx ==
    /// -1, then we'll analyse the edges in the face to decide where to
    /// put it.
    void		 setFace(GU_SubDivPatch *face, int idx = -1);
    void		 clearFace(GU_SubDivPatch *face);

    /// Set and get completion status.  We're complete when all our
    /// neighbouring edges and faces are defined.
    void		 setAllIncomplete();
    void		 copyIncomplete(const GU_SubDivVertex *vtx);
    void		 setComplete(int i)	{ myFaces[i] = 0; }

    /// Get subdivision data associated with this vertex
    float		*getData() const	{ return myData; }

    /// Get per-face vertex data if it exists.  You should use
    /// getFaceIndex() to retrieve the face index to be passed to this
    /// operation.
    float		*getVertexData(int fidx) const;

    /// Set vertex data.  The index is a face identifier in the current
    /// vertex.  If a vertex seam is detected, we'll allocate separate
    /// face vertex data.  The offset array is used to isolate individual
    /// vertex attributes so that we can have an independent hard edge mask
    /// for each attribute.  The offsets are in number of floats and point
    /// to the start of each attribute.
    void		 setVertexData(const float *data, int idx,
				       const UT_IntArray &offsets);

    /// Resets the vertex data at off to the default values.
    void		 defaultVertexData(int low, int high);

    int			 getSize() const	{ return myFloatCount; }
    int			 getVertexSize() const	{ return myVertexCount; }

    /// Operations used to build new data from subdivision rules
    void		 sum(const GU_SubDivVertex *vtx, fpreal scale = 1)
			 {
			     for (int i = 0; i < myFloatCount; i++)
				 myData[i] += vtx->myData[i] * scale;
			 }
    void		 sum(const GU_SubDivVertex *vtx,
			     const GU_SubDivPatch *face, fpreal scale = 1);
    void		 sumVertex(const GU_SubDivVertex *vtx,
				   const GU_SubDivPatch *face, int idx,
				   int low = 0, int high = -1,
				   fpreal scale = 1);
    void		 scale(fpreal scale)
			 {
			     for (int i = 0; i < myFloatCount; i++)
				 myData[i] *= scale;
			 }
    void		 scaleVertex(fpreal scale,
				     int low = 0, int high = -1);
    void		 copyVertex(int src, int dest, int low = 0, int high = -1);

    // Get vertex and face connectivity.
    void		 getVertices(UT_Array<GU_SubDivVertex *> &list);
    void		 getFaces(UT_Array<GU_SubDivPatch *> &list,
				  const GU_SubDivPatch *face) const;

    /// Calculate the new subdivision point using the catmull-clark masks.
    GU_SubDivVertex	*getChild()
			 {
			     if (!myChild)
				 catclarkChild(false);
			     return myChild;
			 }

    /// Calculate the limit subdivision point using the catmull-clark
    /// masks.  If you use this operation, further subdivision using this
    /// vertex will not produce the correct result.
    GU_SubDivVertex	*getLimit()
			 {
			     if (!myChild)
				 catclarkChild(true);
			     return myChild;
			 }

    int			 getEdgeIndex(const GU_SubDivVertex *vtx) const;
    GU_SubDivEdge	*getEdge(int idx) const	{ return myEdges[idx]; }
    GU_SubDivEdge	*getEdge(const GU_SubDivVertex *vtx) const
			 {
			     int idx = getEdgeIndex(vtx);
			     return idx >= 0 ?  myEdges[idx] : 0;
			 }
    GU_SubDivVertex	*getEdgeChild(const GU_SubDivVertex *vtx) const
			 { return getEdge(vtx)->getChild(); }

    int			 getFaceIndex(const GU_SubDivPatch *face) const;
    GU_SubDivPatch	*getFace(int idx) const	{ return myFaces[idx]; }
    bool		 hasFace(const GU_SubDivPatch *face) const
			 { return getFaceIndex(face) >= 0; }

    int			 getValence() const	{ return myValence; }
    bool		 hasHardEdge() const;

    /// Order the edges using the face set for this vertex
    void		 sortEdges();
    void		 setWeight(fpreal weight, bool linear,
				   const GU_SubDivPatch *face);

    void		 setFlag(uint8 f)	{ myFlag |= f; }
    void		 clearFlag(uint8 f)	{ myFlag &= ~f; }
    bool		 getFlag(uint8 f) const	{ return myFlag & f; }

    /// Dump geometry for the vertex as a draw file
    void		 dumpGeo(char c, const GU_SubDivPatch *face = 0) const;

protected:
    void		 catclarkRule(GU_SubDivVertex *dest,
				      int creaseidx[2], int hcount,
				      bool limit);
    void		 catclarkRuleVertex(GU_SubDivVertex *dest,
				      int creaseidx[2], int hcount,
				      int low, int high,
				      bool limit);

private:
     GU_SubDivVertex() {}
    ~GU_SubDivVertex() {}

    void		 catclarkChild(bool limit);

private:
    GU_SubDivVertex	 *myChild;
    GU_SubDivEdge	**myEdges;
    GU_SubDivPatch	**myFaces;
    float		 *myData;
    float		 *myVertexData;		// Optional for vtx boundary
    int			  myValence;
    int			  myRefCount;
    int			  myFloatCount;		// myData size
    int			  myVertexCount;	// myVertexData size
    GA_Index		  myGeoNum;		// GEO point number
    uint8		  myFlag;
};

///
/// A subdivision patch for user-driven subdivision.  Patches can be
/// subdivided in any order, and should be freed by decrementing the
/// reference count in the caller.  Creation automatically initializes the
/// reference count to 1.  You should not increment the reference count.
///

static const uint8	  SUBD_PATCH_LIVE	= 0x01;
static const uint8	  SUBD_PATCH_REVERSE	= 0x02;
static const uint8	  SUBD_PATCH_MARK	= 0x04;

class GU_API GU_SubDivPatch
{
public:
    static GU_SubDivPatch	*allocPatch(
	    const UT_Array<GU_SubDivVertex *> &vertices, bool islive);
    static void			 freePatch(GU_SubDivPatch *patch);

    void		 decRefCount();
    int			 getRefCount()	{ return myRefCount; }

    ///
    /// Catmull-clark subdivision
    ///
    /// Perform subdivision on this patch, creating a new set of
    /// subpatches.  The caller attains ownership of the patches.
    ///
    void		 catclark(UT_Array<GU_SubDivPatch *> &patches);

    /// Return the face centroid
    GU_SubDivVertex	*getChild()
			 {
			     if (!myChild)
				 catclarkChild();
			     return myChild;
			 }

    /// Return the number of edges and vertices bordering this patch.
    int			 getValence() const	{ return myValence; }
    /// Return the next vertex after vtx
    GU_SubDivVertex	*getNextVertex(GU_SubDivVertex *vtx) const;
    GU_SubDivVertex	*getPrevVertex(GU_SubDivVertex *vtx) const;

    /// If we're a bezier, returns the bezier hull.  The correct vertex
    /// attribute associations are returned in the vtxdata array.  If we're
    /// not a bezier, false is returned.
    bool		 getBezier(UT_Array<GU_SubDivVertex *> &vtx,
				   UT_Array<float *> &vtxdata) const;
    /// Get internal and external vertices bordering this patch
    void		 getVertices(UT_Array<GU_SubDivVertex *> &vtx) const;
    GU_SubDivVertex	*getVertex(int i) const	{ return myVertex[i]; }
    void		 getFaceRing(UT_Array<GU_SubDivPatch *> &list) const;

    /// Make an independent copy of this patch with new vertices and edges
    GU_SubDivPatch	*copy() const;

    /// Live patches are allowed to be externally referenced and keep
    /// additional references to the surrounding ring of patches so that
    /// all the data required for subdivision is available.
    void		 makeLive();
    bool		 isLive() const	{ return myFlag & SUBD_PATCH_LIVE; }

    /// Reverse the vertex order for this patch.  We'll keep a flag to
    /// determine whether it is flipped relative to the original geometry.
    void		 reverse();

    /// Dump geometry for the patch as a draw file
    void		 dumpGeo() const;

    void		 setFlag(uint8 f)	{ myFlag |= f; }
    void		 clearFlag(uint8 f)	{ myFlag &= ~f; }
    void		 toggleFlag(uint8 f)	{ myFlag ^= f; }
    bool		 getFlag(uint8 f) const	{ return myFlag & f; }

protected:
    void		 decRefCountInternal()
			 {
			     myRefCount--;
			     UT_ASSERT(myRefCount >= 0);
			     if (myRefCount == 0)
				 freePatch(this);
			 }

    /// Creates subpatches within this patch.  If a face is supplied, we'll
    /// only construct subpatches on the boundary with the face.
    void		 catclarkPatches(GU_SubDivPatch *face = 0);

private:
     GU_SubDivPatch() {}
    ~GU_SubDivPatch() {}

    void		 incRefCount()	{ myRefCount++; }
    void		 catclarkChild();

private:
    GU_SubDivPatch	**mySubPatches;
    GU_SubDivVertex	 *myChild;
    GU_SubDivVertex	**myVertex;
    int			  myValence;
    int			  myRefCount;
    uint8		  myFlag;
};

/// An interface for constructing a set of patches from geometry
class GU_API GU_SubDivPatchSet
{
public:
    /// Given a set of motion blur details, create the subdivision patch
    /// representation.
	     GU_SubDivPatchSet(const GU_Detail *detail,
			       const GU_SubDivPacking &ptattribs,
			       const GU_SubDivPacking &vtxattribs,
			       bool linearcreases = false,
                               const GA_PrimitiveGroup *primgroup=NULL);
    ~GU_SubDivPatchSet();

    void		 stealPatches(UT_Array<GU_SubDivPatch *> &patches);

    GU_SubDivPatch	*getPatch(exint i) const;
    const GEO_Primitive	*getGEOPrimitive(exint i) const;
    GA_Size              getSoupPolygon(exint i ) const;

    /// Perform subdivision on the geometry.  The @c gdp is modified so the
    /// original polygons are deleted and replaced with the Catmull-Clark
    /// subdivision representation.
    static void	catclark(GU_Detail *gdp,
			int depth,
			const GA_PrimitiveGroup *group,
			bool linear_creases,
			bool build_polysoups);

    const GU_SubDivPacking	&getPointPacking() const
				    { return myPointPacking; }
    const GU_SubDivPacking	&getVertexPacking() const
				    { return myVertexPacking; }

private:
    void	processFace(const GEO_Primitive *prim,
			const UT_Array<GU_SubDivVertex *> &vertices);
    void	processSoup(const GEO_Primitive *prim,
			const UT_Array<GU_SubDivVertex *> &vertices);
    bool	addFace(GA_Offset primoff,
			const UT_Array<GA_Offset> &gfacevtx,
			const UT_Array<GU_SubDivVertex *> &vertices,
                        GA_Size souppolygon);

    const GU_Detail		*myDetail;
    UT_Array<GU_SubDivPatch *>	 myPatches;
    GA_OffsetList		 myGEOPrims;
    GA_ListType<GA_Size,GA_Size> mySoupPolygons;
    GU_SubDivPacking		 myPointPacking;
    GU_SubDivPacking		 myVertexPacking;
};

#endif
