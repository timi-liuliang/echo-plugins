/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	Paste surface class.
 * 
 */

#ifndef __GEO_PasteSurf_h__
#define __GEO_PasteSurf_h__

#include "GEO_API.h"
#include <UT/UT_RefMatrix.h>
#include <UT/UT_Vector2.h>
#include <GP/GP_Domain.h>
#include <GA/GA_Handle.h>
#include "GEO_PasteVertex.h"

class GA_AttributeRefMap;
class GA_LoadMap;
class GA_SaveMap;
class GP_NodeList;
class GEO_Vertex;
class GEO_AttributeHandleList;
class GEO_TPSurf;
class GEO_PasteSurfDAG;


class GEO_API GEO_PasteSurf : public GP_Domain
{
    friend class GEO_PasteSurfDAG;

public:
    // Class c-tors and d-tor
		 GEO_PasteSurf(void);
		 GEO_PasteSurf(GEO_TPSurf &tpsurf, const GP_XformHandle &xform,
			      int pasteup = 1, float height = 0);
		 GEO_PasteSurf(const GEO_PasteSurf &d);
    virtual	~GEO_PasteSurf(void);

    // Produce a brand new copy of ourselves. Must free it yourself.
    // Does not copy the spline surface, just its primitive index!
    virtual GP_Node	*copy(void) const;

    // Create a a brand new object of the same type as us just using the
    // default c-tor.
    virtual GP_Node	*newSpecies(void) const;

    // I/O functions returning 0 if OK and -1 otherwise.
    virtual int		 save(std::ostream &os, int binary = 0) const;
    virtual bool	 load(UT_IStream &is);

    virtual bool	 saveJSONFields(UT_JSONWriter &w,
					const GA_SaveMap &map) const;
    virtual void	 initJSONLoad();
    virtual bool	 loadJSONField(UT_JSONParser &p, const GA_LoadMap &map,
				       const char *field_token);

    // Return a decreasing-order list of would-be parents listed in decreasing
    // order in a stack of nodes.
    virtual void	 findParents(GP_NodeList &stack, GP_NodeList &parents);
    virtual void	 findParent (GP_Domain &parent);

    // Spawn a child and link it to us, assign its frames accordingly, update
    // its vertices, set its parent and base, etc. Return that domain if
    // everything is successful, else 0. The child in the domain area defined
    // by brect, and the ub/vb stuff are belt reinforcements.
    virtual GP_Domain	*spawn(const UT_BoundingRect &brect,
			       float ubwidth = 0, float vbwidth = 0,
			       int   ubdivs  = 2, int   vbdivs  = 2,
			       int   newkey  = 0);

    // Evaluate at a world domain point and return 0 if successful, else -1:
    bool		 evaluate(fpreal worldu, fpreal worldv,
				GEO_Vertex result,
				GEO_AttributeHandleList &hlist,
				unsigned du = 0, unsigned dv = 0) const;
    bool		 evaluate(fpreal worldu, fpreal worldv,
				GA_Offset result,
				GA_AttributeRefMap &map,
				unsigned du = 0, unsigned dv = 0) const;

    int 		 evaluate(float worldu, float worldv, UT_Vector4 &pos,
				  unsigned du = 0, unsigned dv = 0) const;

    // Make this surface's CVs independent of any base surfaces and reset the
    // base surface pointers to point at myself:
    void		 independentVertices(int preserve_shape = 1);

    // Translate all the CVs by an equal amount in world space:
    void		 translateVertices(const UT_Vector3 &delta);

    // Update the frame origins by translating them because the CVs of the
    // spline surface have been translated by this amount too.
    void		 updateTranslation(const UT_Vector3 &delta);

    // Build the local frames using the current base surfaces, then apply
    // the displacements to compute the world image. This is likely to
    // change the contents of the GEO_Points.
    void		 updateVertices(void);

    // Some vertices (or all) have changed, so reverse engineer our
    // displacements from the CVs of the spline surface. In other words,
    // given the local frames and the GEO_Points, build our displacements.
    void		 updateDisplacements(int keepframes = 1);

    // Indicate that a CV has changed.
    // These methods don't touch mySurf in any way, so make sure you do it
    // yourself before calling us. Also, we don't flag the surface cache, and
    // we don't update the hierarchy pasted on us. Better change all you have
    // to change, then updateDescendents() from the DAG class.
    // Another WARNING: we don't check if r and c are within bounds!
    void		 updateDisplacement(int r, int c);

    // Recompute the vertices and the domain of the root surface such that
    // the domain fits the root tightly, and the CVs are displaced from
    // the Grevilles as tightly as possible.
    void		 optimizeDisplacements(int update = 1);

    // Set the bases from a single parent. Be careful with this method.
    void		 setBases(GEO_PasteSurf *base = 0);

    // A much cheaper version than modifyBasis(). To be called when the
    // original basis has been remapped _affinely_ to another range, e.g.
    // using GEO_TPSurf::mapU/VBasis(). The result should leave everything
    // unchanged, including displacements, world image, base surfaces, etc.
    // Don't even think of using it when reparameterizing the surface or
    // refining it.
    void		 affineBasis(void);

    /// Compute the texture coordinates either uniformly of chord length.
    /// If ptattrib is true we deal with points, else with vertices.
    /// Return the texture offset or -1 if problems.
    bool                rowColTexture  (const GA_RWHandleV3 &txth, bool ptattrib = true);
    bool                uniformTexture (const GA_RWHandleV3 &txth, bool ptattrib = true);
    bool                grevilleTexture(const GA_RWHandleV3 &txth, bool ptattrib = true);
    bool                chordLenTexture(const GA_RWHandleV3 &txth, bool ptattrib = true);

    // Trim the underlying base(s):
    void		trimUnder(float scale = 1.0f, int keepoutside = 1);

    // Query or set the height, which is just an offset of the CV displacement:
    float		 height(void) const	{ return myHeight;	   }
    void		 height(float h);

    // Reset the spline surface pointer. If mutual, then we reset the 
    // surface's pointer to us as well. Use with care.
    // Pardon the choice of names :-)
    void		 sterilize(int mutual = 1);
    void		 pregnant (GEO_TPSurf &s, int mutual=1, int rebuild=1);

    // Query or set the paste direction flag, and then possibly update the
    // world image. Normally you want the image to update:
    int			 pastedUp (void) const	{ return myPasteUp;	   }
    void		 pasteUp  (int updt=1)	{ flipPaste(1, updt);	   }
    void		 pasteDown(int updt=1)	{ flipPaste(0, updt);	   }

    // Cautious handles to our frame and surface:
    const GP_Frame	&frame  (void) const	{ return frames()->root(); }
    const GEO_TPSurf	&surface(void) const	{ return *mySurf;          }

    // Determine if we are associated with a surface:
    int			 hasSurface(void) const { return (mySurf) ? 1 : 0; }

    GEO_PasteSurfDAG	*hierarchy(void) const;	//  return dag(); 

    // Index of spline primitive we should use as mySurf. To be used ONLY
    // for loading purposes. myNum is never updated afterwards.
    int			 primNumber(void) const	{ return myNum;		   }


protected:
    // Copy the in nodes from a pool of already built nodes:
    // Return 0 if OK and -1 otherwise.
    virtual int 	 copyInNodes(const GP_Node &src,
				     const GP_NodeTree &destpool);


private:
    GEO_TPSurf				*mySurf;
    UT_RefMatrix<GEO_PasteVertex>	 myCVs;
    GP_PointMatrix			 myGrev;
    float				 myHeight;
    int					 myPasteUp;
    int					 myNum; // used ONLY to resolve loading

    // Special c-tor using for spawning.
    GEO_PasteSurf(GEO_TPSurf &tpsurf, GEO_PasteSurf &parent, int spawnkey);

    // Assign us the the surface and vice versa, and update the GP_Domain's
    // internals. Used during load.
    void		 surface(GEO_TPSurf &tpsurf, int buildvertices = 1);

    // Compute the Greville matrix from the original basis information:
    void		 buildGrevilles(void);

    // Set up the diffuse CV matrix by mapping the Greville space into 3D
    // and computing CV displacements. Build the Grevilles first.
    void		 buildVertices(int originalgrev = 1);

    // Change the local frame to point to the world greville image with an
    // identity matrix. This is almost equivalent to pasting onto a plane.
    // This method does not update the GEO_Points, so call updateWorldImage()
    // if you need to.
    void		 resetVertices(void);

    // Anything goes in terms of spline surface changes, especially if it
    // involves changes of both knots and CVs, as in refinement, unrefinement,
    // degree elevation, or cutting. If "standalone" is 1, we temporarily go
    // back to our unpasted shape before calling (*apply)(). Return 0 if OK,
    // else -1. This method resets the base surfaces to self, and does not
    // update the GEO_Points if standalone is 1, so call updateWorldImage()
    // if you need to.
    virtual int		 modifyBasis(int (*apply)(void *tpsurf, void *d),
				     void *data = 0, int standalone = 1);

    // Assign myself to all the base surfaces:
    void		 initializeBases(void);

    // Flip the pasting direction:
    void		 flipPaste(int up, int updateimage = 1);

    // Update the texture coordinates of the surface by mapping the existing
    // ones to the transformed domain (using the frame xform). txtoff must be
    // >= 0 for anything to happen.
    void		updateTextureCoordinates(const GA_RWHandleV3 &txth,
				bool ptattrib = true);

    // Assuming that everything is clean, load the world values of the 
    // transformed CVs into the GEO_Points.
    void		 updateWorldImage(void);

    // A truly internal method, originalImage() loads the CVs from the original
    // grevilles and displacements and ignores the current frames and world
    // xform. Nothing else gets changed, so this is meant to be temporary.
    // We use this to apply spline algorithms on the original surface (eg.
    // refine it), and then go back to the pasted image.
    void		 buildOriginalImage(void);
};

#endif
