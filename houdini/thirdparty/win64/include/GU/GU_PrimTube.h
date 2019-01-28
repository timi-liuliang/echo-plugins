/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimTube.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimTube.h class...
 * 
 */

#ifndef __GU_PrimTube_h__
#define __GU_PrimTube_h__

#include "GU_API.h"
#include <GEO/GEO_PrimTube.h>
#include <GEO/GEO_SurfaceType.h>
#include "GU_Cap.h"

class GEO_ConvertParms;
class GEO_Hull;
class GEO_Detail;
class GEO_TriMesh;
class GU_PrimMesh;
class GU_PrimNURBSurf;
class GU_PrimRBezSurf;
class GU_CapOptions;
class GA_ElementWranglerCache;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimTube				   //
// Descr:     This class is responsible for operations     //
//	done on tube primitives.  As we need to access     //
//       the GEO_Detail, the pointer is passed in the      //
//       constructor.  The second constructor creates a    //
//       tube with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimTubeParms
{
public:
    GU_PrimTubeParms(GEO_Detail *gdp=NULL, GA_Offset ptoff=GA_INVALID_OFFSET);

//
//  Build a transform matrix for a tube given two points in space.  The
//	transform will create a tube connecting the two points.
    void		 buildXform(const UT_Vector3 &p0, const UT_Vector3 &p1,
					float radius);

    GEO_Detail		*gdp;
    GA_Offset		 ptoff;
    UT_Matrix4		 xform;
    int			 rows, cols;
    int			 orderu;
    int			 orderv;
    int			 imperfect;
    float		 taper;
    GEO_SurfaceType	 type;
};

class GU_API GU_PrimTube : public GEO_PrimTube
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimTube() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimTube(GEO_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimTube(gdp, offset)
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

    virtual void	 normal(NormalComp &ouput) const;
    virtual int		 intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				    float tmax = 1E17F, float tol = 1E-12F,
				    float *distance = 0, UT_Vector3 *pos = 0,
				    UT_Vector3 *nml = 0, int accurate = 0,
				    float *u = 0, float *v = 0,
				    int ignoretrim = 1) const;

    static GEO_Primitive	*build(const GU_PrimTubeParms &parms,
				       const GU_CapOptions &caps,
				       GA_PrimitiveTypeId type = GEO_PRIMTUBE);

private:
    // Pass base attributes to converted geometry:
    void		passAttributesToFaces(const GA_Range &primrange,
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
