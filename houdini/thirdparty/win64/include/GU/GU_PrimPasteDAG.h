/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimPasteDAG.h (C++)
 *
 * COMMENTS:	Pasted surface class.
 * 
 */

#ifndef __GU_PrimPasteDAG_h__
#define __GU_PrimPasteDAG_h__

#include "GU_API.h"
#include <GEO/GEO_PrimPasteDAG.h>

class GEO_ConvertMethod;
class GEO_ConvertParms;
class UT_MemoryCounter;

class GU_API GU_PasteParms
{
public:
    GU_PasteParms()
    {
	method	   = 0;
	up	   = 0;
	trim	   = 0; trimscale = 1;
	height	   = 0;
	ubeltwidth = vbeltwidth = 0;
	keepshape  = 0;
    }

    int			method;	   // 0 = min distance projection
				   // 1 = vector projection
				   // 2 = parametric mapping
				   // 3 = as is

    int			up;	   // paste up or down
    int			trim;	   // trim underlying base
    float		trimscale; // scale amount for trim curve
    float		height;	   // paste at this elevation
    UT_Vector3		axis;	   // proj. axis if projecting along vector	
    float		rtol;	   // ray intersection tolerance
    int			keepshape; // don't deform feature when pasting it

    float		umin;	   // 0 <= umin <= 1
    float		umax;	   // 0 <= umax <= 1
    float		vmin;	   // 0 <= vmin <= 1
    float		vmax;	   // 0 <= vmax <= 1

    float		ubeltwidth;
    float		vbeltwidth;
    int			ubeltdivs;
    int			vbeltdivs;
};

class GU_API GU_PrimPasteDAG : public GEO_PrimPasteDAG
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimPasteDAG() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimPasteDAG(GA_Detail *d, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimPasteDAG(d, offset)
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

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimPasteDAG::<functname>
    //        i.e.        pdagptr = GU_PrimPasteDAG::build(params...);

    // Optional Build Method. Returns 0 if it fails. 
    static GU_PrimPasteDAG	*build(GU_Detail *gudp, GEO_TPSurf &root,
				       int chkpaste = 0);
    static GU_PrimPasteDAG	*build(GU_Detail *gudp, GEO_PasteSurf &root,
				       int chkpaste = 0);

    // Conversion methods
    virtual GEO_Primitive	*convert(GEO_ConvertParms &parms,
					 GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive	*convertNew(GEO_ConvertParms &parms);

    virtual void	 normal(NormalComp &output) const;

    // Intersect us with a ray and return 1 if we have a hit, else 0:
    virtual int		 intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0,
				int ignoretrim = 1) const;

    // Find minimum distance to a point and return 1 if found. 0 otherwise:
    int			 minimum(const UT_Vector3 &p, float distmax = 1E17F,
				 float *distance = 0, float *u = 0,
				 float *v = 0, int accurate = 1) const;

    // Find minimum distance between two primitives and return 1 if found,
    // 0 otherwise. (u1,v1) is on us, (u2,v2) is on prim.
    int			 minimum(const GEO_Primitive &prim, float distmax = 1E17F,
				 float *distance = 0, float *u1 = 0,
				 float *v1 = 0, float *u2 = 0, float *v2 = 0,
				 int accurate = 1) const;

    // Various pasting methods:
    GEO_PasteSurf	*paste  (GU_PrimPasteDAG &dag, const GU_PasteParms &);
    GEO_PasteSurf	*overlay(GEO_PasteSurf &child, GU_PrimPasteDAG &dag,
				 const GU_PasteParms &parms, int at = 1);
    GEO_PasteSurf	*replace(GEO_PasteSurf &child, GU_PrimPasteDAG &dag,
				 int at = 1, int up = 1, float height = 0.0f);
    GEO_PasteSurf	*spawn  (GEO_PasteSurf &child, const GU_PasteParms &);

				 
protected:
    // Virtual (shallow) c-tor that can be used by classes derived
    // from us to instantiate themselves at this level:
    virtual GEO_PasteSurfDAG	*newDAGSpecies(void) const;

private:
    // Return 0 if OK, else -1.
    int			 evaluateSurface(GEO_ConvertMethod &method,
					 int featurecoord,
					 UT_Vector4Array &pos,
					 int &nu, int &nv) const;
    // Return 0 if OK, else -1.
    int			 getUVInterval (const GEO_PasteSurf &surf,
					float &umin, float &vmin,
					float &umax, float &vmax) const;

    // Return 0 if OK, else -1.
    int			 getProjectionUV(const UT_Vector3 &axis, float tol,
					 int vecproj, GU_PrimPasteDAG &dag,
					 GP_Point &b1, GP_Point &b2,
					 GP_Point &b3, GP_Point &b4);

    // b1..b4 are in the local frame the surface, but we need
    // them in world space:
    void		 toWorld(const GEO_PasteSurf &surf,
				 GP_Point &b1, GP_Point &b2,
				 GP_Point &b3, GP_Point &b4) const;

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
