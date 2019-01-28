/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	Paste DAG class.
 * 
 */

#ifndef __GEO_PasteSurfDAG_h__
#define __GEO_PasteSurfDAG_h__

#include "GEO_API.h"
#include <GP/GP_DomainDAG.h>
#include <GA/GA_Handle.h>
#include "GEO_PasteSurf.h"

class GA_AttributeRefMap;
class GA_PointGroup;
class GA_SaveMap;
class GA_LoadMap;
class GA_Defragment;
class GEO_Vertex;
class GEO_AttributeHandleList;
class UT_IStream;

class GEO_API GEO_PasteSurfDAG : public GP_DomainDAG
{
public:
    // C-tor with 0 nodes. Can't paste on it.
    GEO_PasteSurfDAG(void);

    // The root this c-tor assigns to its root cannot be already pasted
    // somewhere else.
    GEO_PasteSurfDAG(GEO_PasteSurf *root);

    // Shallow copy c-tor: same nodes, but different lists.
    GEO_PasteSurfDAG(const GEO_PasteSurfDAG &srcdag);
    GEO_PasteSurfDAG(const GP_DomainDAG     &srcdag);

    // D-tor that destroys all the nodes (thanks to its base class).
    virtual		~GEO_PasteSurfDAG(void);

    // Clear the graph and destroy the nodes as well. If "total" is
    // 1, even the GEO_TPSurfs will be deleted; otherwise the GEO_TPSurfs
    // will be disconnected by removing their pasted pointer, and will
    // survive.
    virtual void	 clearAndDestroy(int total = 1);

    // Evaluate one point (when du=dv=0), or the du-th dv-th derivative.
    // Return 0 if successful, and -1 otherwise. origu and origv must lie
    // in the original domain of the root surface. The root is assumed present.
    bool		evaluate(fpreal u, fpreal v, GEO_Vertex result,
				GEO_AttributeHandleList &hlist,
				unsigned du=0, unsigned dv=0) const;
    bool		evaluate(fpreal u, fpreal v, GA_Offset result,
				GA_AttributeRefMap &map, unsigned du=0,
				unsigned dv=0) const;
    int 		evaluate(float origu, float origv, UT_Vector4 &pos, 
				 unsigned du = 0, unsigned dv = 0) const;

    // Evaluate the unit normal at (origu,origv). origu and origv must lie
    // in the original domain of the root surface. The root is assumed present.
    // Return 0 if successful, else -1.
    int			evaluateNormal(float origu,float origv,
				       UT_Vector3 &nml) const;

    // Evaluate the surface  between breakpoints. 'lod' is
    // the number of points between two breakpoints in u or v. The methods
    // compute the number of spans in v if vspans is -1. The number of isoparms
    // computed in v is returned if OK; else -1. Pass in the bkpoint index of
    // the startidx knot if you know it.
    int			fillBreakMesh(int ustartidx, int ustopidx,
				    int vstartidx, int vstopidx,
				    int ulod, int vlod,
				    UT_Vector4 *pos, int &vspans /* = -1 */,
				    int vbreakidx = -1,
				    unsigned du = 0, unsigned dv = 0,
				    UT_FloatArray *uval = 0,
				    UT_FloatArray *vval = 0) const;
    int			fillBreakMesh(int ustartidx, int ustopidx,
				    int vstartidx, int vstopidx,
				    int ulod, int vlod,
				    GEO_Vertex **result,
				    GEO_AttributeHandleList &hlist,
				    int &vspans, int vbreakidx = -1,
				    unsigned du = 0, unsigned dv = 0) const;

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    // Return -1 if child isn't ours, else if all OK, 0.
    int			raiseOrderU(GEO_PasteSurf &child, int neworder,
					GA_AttributeRefMap &map);
    int			raiseOrderU	  (GEO_PasteSurf &child, int neworder);
    int			raiseOrderV(GEO_PasteSurf &child, int neworder,
					GA_AttributeRefMap &map);
    int			raiseOrderV	  (GEO_PasteSurf &child, int neworder);

    // U and V knot insertion methods. Return inserted index or -1.
    int			refineU(GEO_PasteSurf &c, float k,
				GA_AttributeRefMap &hlist, int r=1);
    int			refineU		(GEO_PasteSurf &c, float k,  int r=1);

    int			refineV(GEO_PasteSurf &c, float k,
				GA_AttributeRefMap &hlist, int r=1);
    int			refineV		(GEO_PasteSurf &c, float k,  int r=1);

    // Refine an area around the boundary of the surface, called the "belt".
    // Return 0 if OK, else -1.
    int			refineBelt(GEO_PasteSurf &child,
				   float ubwidth = 0.4F, float vbwidth = 0.4F,
				   int   ubdivs  = 2,   int   vbdivs  = 2);

    // Remove rows and or columns based on a curvature tolerance. Return 1 of
    // something was removed, 0 if nothing removed, and -1 if c is not a 
    // child.
    int			unrefineU(GEO_PasteSurf &c, int kidx,
					GA_AttributeRefMap &hlist,
					int mult=0, float tol=1e-4F,
					GA_PointGroup *delgroup = 0);
    int			unrefineU	(GEO_PasteSurf &c, int kidx,
					 int mult=0, float tol=1e-4F,
					 GA_PointGroup *delgroup = 0);

    int			unrefineV(GEO_PasteSurf &c, int kidx,
					GA_AttributeRefMap &hlist,
					int mult=0, float tol=1e-4F,
					GA_PointGroup *delgroup = 0);
    int			unrefineV	(GEO_PasteSurf &c, int kidx,
					 int mult=0, float tol=1e-4F,
					 GA_PointGroup *delgroup = 0);

    // Reverse rows (V) or columns (U). Reversing in either direction will
    // flip the normal. Return -1 if child isn't ours, else 0.
    int 		reverseU(GEO_PasteSurf &child);
    int 		reverseV(GEO_PasteSurf &child);

    // Reparameterize the surface by changing its basis. This type of 
    // reparameterization is generally NOT shape preserving.
    // Return -1 if child isn't ours, else 0.
    int	 		reparameterizeU(GEO_PasteSurf &child,
				GA_ParameterizationType p);
    int 		reparameterizeV(GEO_PasteSurf &child,
				GA_ParameterizationType p);

    // Unpaste a surface alone or with all its contained children. If you know
    // what the contained set is, pass it in for speed. Don't try to unpaste
    // the root surface unless it has no kids. If preserveshape is 1, 
    // preservexform is ignored. If preserveshape is 0, we have the option to
    // keep the domain xform as it is or reset it to identity.
    GEO_PasteSurf	*unpasteAt  (GEO_PasteSurf &child, int preserveshape=0,
							   int preservexform=0);
    GEO_PasteSurfDAG	*unpasteFrom(GEO_PasteSurf &child, GP_NodeTree *ctree,
							   int preserveshape=0,
							   int preservexform=0);

    // Change the height of the unpasted vertices by the given amount either
    // for a single surface or for it and all of its descendents. Return 0
    // if surf is pasted on us, else -1.
    int			 offsetAt  (GEO_PasteSurf &surf, float height);
    int			 offsetFrom(GEO_PasteSurf &surf, float height);
    int			 height(float newheight)
			 {
			     return (rootSurface())
				 ? offsetAt(*rootSurface(),newheight) : -1;
			 }

    // Reverse the pasting direction of the given surface and make sure 
    // everybody pasted on it partially or fully is refreshed.
    // Return 0 if child is pasted on us, else -1.
    int			 flipAt  (GEO_PasteSurf &child);
    int			 flipFrom(GEO_PasteSurf &child);
    int			 flip    (void)
			 {
			     return (rootSurface())?flipFrom(*rootSurface()):-1;
			 }

    // Make sure the xforms, the displacements and the world image are up
    // to date:
    void		 updateHierarchy(void);

    // Recompute the vertices and the domain of the root surface such that
    // the domain fits the root tightly, and the CVs are displaced from
    // the Grevilles as tightly as possible.
    void		 optimizeDisplacements(int update = 1);

    // Copy from the source. Shallow in the nodes. All out nodes are 
    // destroyed first. Be careful.
    GEO_PasteSurfDAG	&operator=(const GEO_PasteSurfDAG &src);
    GEO_PasteSurfDAG	&operator=(const GP_DomainDAG     &src);

    // Assuming "child" is the root of a newly pasted DAG, link it to its
    // parent domain(s) (top containers) in our DAG. The methods make sure 
    // everything is (re)pasted and updated accordingly. The bases are
    // already set at this point by findParents().
    virtual void	 link(GP_Domain &child, GP_NodeList &parents,
			      int keepshape = 0);
    virtual void	 link(GP_Domain &child, GP_Domain   &parent,
			      int withcorrection = 1);

    // Assuming child is pasted on us, update the pasted image of all of
    // surf's descendents in pasted order. surf is not updated.
    // Return 0 upon success, else -1.
    virtual int		 updateDescendents(GP_Domain &child,
					   int including_child = 0);

    // Some vertices (or all) have changed, so reverse engineer the
    // displacements from the CVs of the spline surface. In other words,
    // given the local frames and the GEO_Points, build the displacements for
    // the child node, then update its descendents. Return -1 if not our child.
    virtual int		 updateDisplacements(GP_Domain &child, 
					     int keepframes = 1);

    // Use this method after loading to make sure all the proper links are
    // set. ONLY after loading. Return 0 is OK and -1 otherwise.
    int			  pasteInPlace(GEO_PasteSurf &ps, GEO_TPSurf &tpsurf);

    /// Compute the texture coordinates either uniformly of chord length. If
    /// the attribute is invalid, the methods find it and create the attribute
    /// if necessary. If ptattrib is true we deal with points, else with vertices.
    /// Returns false iff problems.
    bool		rowColTexture (const GA_RWHandleV3 &txth, bool ptattrib);
    bool		uniformTexture (const GA_RWHandleV3 &txth, bool ptattrib);
    bool		grevilleTexture(const GA_RWHandleV3 &txth, bool ptattrib);
    bool		chordLenTexture(const GA_RWHandleV3 &txth, bool ptattrib);

    // Traverse the paated surfaces in the order in which they were pasted.
    // Stop the traversal if the applied function returns zero.
    int			 traverse        (int (*f)(const GEO_PasteSurf*,void*),
					  void *d) const;
    int			 traverseBackward(int (*f)(const GEO_PasteSurf*,void*),
					  void *d) const;

    // The public can only look at the root but not change it because things
    // are tricky in the hierarchy:
    const GEO_PasteSurf	*rootSurface() const
			 {
			     return (const GEO_PasteSurf*) root();
			 }

    // I/O functions returning 0 if OK and -1 otherwise.
    // In inFromH9() all the nodes and the frames are loaded in, but they are
    // not connected to each other in any way. The pointers to TPSurfs are 
    // not set either, but we have loaded the primitive numbers. The reason we
    // cannot resolve them now is that we're not sure the primitives have been 
    // loaded. Once the primitives are loaded, call resolveLinks() in the
    // derived class.
    bool		 outToH9 (std::ostream &os, int binary = 0) const;
    bool		 inFromH9(UT_IStream &is);

    bool		 saveHierarchy(UT_JSONWriter &w,
				       const GA_SaveMap &map) const;
    bool		 loadHierarchy(UT_JSONParser &p,
				       const GA_LoadMap &map);

protected:
    // Let us know when something has changed in the root domain.
    virtual void	 changedRoot(void);

    // Derived classes get a full look at the root with a non-const handle:
    GEO_PasteSurf	*rootSurface()	{ return (GEO_PasteSurf*)root(); }

    // Virtual (shallow) c-tor that can be used by classes derived
    // from us to instantiate themselves at this level:
    virtual GEO_PasteSurfDAG	*newDAGSpecies(void) const;
    int			fillVCurve(float u,int uidx, int vstartidx,int vstopidx,
				   int density, UT_Vector4 *pos, int pvoffset,
				   int rbound, unsigned du, unsigned dv) const;
    int			fillVCurve(float u, int uidx, int vstartidx,
				   int vstopidx, int density,
				   GEO_Vertex **result,
				   GEO_AttributeHandleList &hlist,
				   int pvoffset, int rbound,
				   unsigned du, unsigned dv) const;

};

#endif
