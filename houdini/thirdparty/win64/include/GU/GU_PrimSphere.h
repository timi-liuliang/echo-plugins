/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimSphere.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimSphere.h class...
 * 
 */

#ifndef __GU_PrimSphere_h__
#define __GU_PrimSphere_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GEO/GEO_SurfaceType.h>
#include <GEO/GEO_PrimSphere.h>
#include <UT/UT_Matrix4.h>

class GU_PrimMesh;
class GU_PrimNURBSurf;
class GU_PrimRBezSurf;
class GEO_ConvertParms;
class GEO_Hull;
class GEO_TriMesh;
class GA_ElementWranglerCache;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimSphere				   //
// Descr:     This class is responsible for operations     //
//	 done on sphere primitives.  As we need to access  //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       sphere with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimSphereParms
{
public:
    GU_PrimSphereParms(GU_Detail *gdp=NULL, GA_Offset ptoff=GA_INVALID_OFFSET);

    GU_Detail		*gdp;
    GA_Offset		 ptoff;		// use this point instead of yours
    UT_Matrix4		 xform;
    int			 freq;		// For polygons/polysoup
    int			 rows, cols;	// For mesh
    int			 uniquePole;	// For mesh&others
    int			 orderu;	// for surfaces
    int			 orderv;
    int			 imperfect;	// nonrational
    GEO_SurfaceType	 type;		// For hull types
    bool                 soup;          // For polygons/polysoup
};


class GU_API GU_PrimSphere : public GEO_PrimSphere
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimSphere() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimSphere(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimSphere(gdp, offset)
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

    // Conversion Methods
    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
				GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    virtual void	normal(NormalComp &output) const;


    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimSphere::<functname>
    //        i.e.      sphereptr = GU_PrimSphere::sphereBuild(params...);

    // Optional Build Method

    static GEO_Primitive	*build(const GU_PrimSphereParms &parms,
					GA_PrimitiveTypeId type=GEO_PRIMSPHERE);

    virtual int			 intersectRay(const UT_Vector3 &o, 
					const UT_Vector3 &d,
					float tmax = 1E17F, float tol = 1E-12F,
					float *distance = 0, 
					UT_Vector3 *pos = 0,
					UT_Vector3 *nml = 0,
					int accurate = 0,
					float *u = 0, float *v = 0,
					int ignoretrim = 1) const;

private:
    // Pass center attributes to converted geometry:
    void		passAttributesToPolys(const GA_Range &primrange,
					      const GA_Range &ptrange,
					      bool preserve_groups,
					      GA_ElementWranglerCache &) const;
    void		passAttributesToMesh(GEO_Primitive &dest,
					     bool preserve_groups,
					     GA_ElementWranglerCache &) const;

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
