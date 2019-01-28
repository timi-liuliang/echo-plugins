/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimMesh.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimMesh.h class...
 * 
 */

#ifndef __GU_PrimMesh_h__
#define __GU_PrimMesh_h__

#include "GU_API.h"
#include <GEO/GEO_PrimMesh.h>

class GA_Detail;
class GA_EdgeGroup;
class GEO_ConvertParms;
class GEO_Detail;
class GEO_PrimPoly;
class GEO_PolyCounts;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_PrimMesh				   //
// Descr:     This class is responsible for operations     //
//	 done on mesh primitives.  As we need to access    //
//       the GEO_Detail, the pointer is passed in the      //
//       constructor.  The second constructor creates a    //
//       mesh with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimMesh : public GEO_PrimMesh
{
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimMesh() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimMesh(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimMesh(gdp, offset)
    {}

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    // Conversion Method 

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
				GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    // Cut a wedge of the primitive given a domain range
    // ind1 and ind2 are indices to the refined values
    // They are updated if negative on input, else used as is.
    // If keep is zero the curve is only refined and the indices
    // updated.

    GU_PrimMesh		*cutU(float u1, float u2, 
			      int &ind1, int &ind2, int keep);
    GU_PrimMesh		*cutV(float v1, float v2, 
			      int &ind1, int &ind2, int keep);

    // Open the primitive at the given domain value
    void		 openAtU(float u);
    void		 openAtV(float v);

     // remove a single curve in the domain of the hull
    GEO_PrimPoly	*extractU(float unitu)
    { return extractU(unitu, getParent()); }
    GEO_PrimPoly	*extractU(float unitu, GEO_Detail *parent) const;
    GEO_PrimPoly	*extractV(float unitv)
    { return extractV(unitv, getParent()); }
    GEO_PrimPoly	*extractV(float unitv, GEO_Detail *parent) const;

    // This is more powerful than convertNew. It always returns a NEW
    // object, so free it when you're done with it. It may return a mesh,
    // a NURB surface or a Bezier surface depending on the type.
    GEO_Hull		*reconfigure(unsigned type, int orderu, int orderv,
				     bool openu, bool openv) const;

    virtual int        intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				     float tmax = 1E17F, float tol = 1E-12F,
				     float *distance = 0, UT_Vector3 *pos = 0,
				     UT_Vector3 *nml = 0, int accurate = 0,
				     float *u = 0, float *v = 0,
				     int ignoretrim = 1) const;


    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimMesh::<functname>
    //        i.e.        meshptr = GU_PrimMesh::build(params...);

    // Optional Build Method 

    static GU_PrimMesh	*build(GEO_Detail *gudp,
				int rows, int cols,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				int wrapu = 0, int wrapv = 0,
				int appendPoints=1);

    // Remove as many of the given edges of possible from this poly.
    // If edge #a is removed, bit a in the bit array is set to a & 1.
    // returns 0 if successful, -1 if the poly becomes degenerate, and
    // -2 if the poly should be removed.
    int          removeEdges(const GA_EdgeGroup &edges,
			     UT_BitArray *remove_edges=0);

    /// For each vertex of polygons representing the mesh with the
    /// specified properties, this function fills in what the
    /// vertex number in the corresponding mesh would be.
    /// This is useful for avoiding creating an intermediate GU_PrimMesh
    /// that would just be converted to polygons or a polygon soup.
    static void fillMeshVertArray(GEO_SurfaceType surface, GA_Size nrows,
                                  GA_Size ncols, bool wrappedu, bool wrappedv,
                                  UT_IntArray &relvertnums,
                                  GEO_PolyCounts &polysizes,
                                  bool parallel = true);

protected:
    virtual GEO_Hull	*getSubHull(int top, int left, int bottom, int right,
				    GEO_SubHullFlags break_flags);

private:
    // Recursively break the mesh using the edge-deletion matricies.
    // used by removeEdges

    int 		 create(int rows, int cols,
				GEO_SurfaceType type = GEO_PATCH_QUADS, 
				int wrapu = 0, int wrapv = 0,
				int appendPoints = 1);

    void		 computeVertices(GEO_Hull &hull, int, int) const;

    GEO_Primitive	*doConversion(GEO_ConvertParms &parms, int reusePoints);

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
