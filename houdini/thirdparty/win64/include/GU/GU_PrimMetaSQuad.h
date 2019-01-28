/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimMetaSQuad.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimMetaSQuad.h class...
 * 
 */

#ifndef __GU_PrimMetaSQuad_h__
#define __GU_PrimMetaSQuad_h__

#include "GU_API.h"
#include <GEO/GEO_PrimMetaSQuad.h>
#include "GU_Detail.h"

class GEO_ConvertParms;
class GEO_Hull;
class GU_PrimPoly;
class GU_PrimMesh;
class GU_PrimNURBSurf;
class GU_PrimRBezSurf;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimMetaSQuad			   //
// Descr:     This class is responsible for operations     //
//	 done on sphere primitives.  As we need to access  //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       sphere with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimMetaSQuadParms
{
public:
    GU_PrimMetaSQuadParms();

    GU_Detail	*gdp;
    GA_Offset	 ptoff;
    UT_Matrix4	 xform;
    float	 weight;
    float	 xyExp;
    float	 zExp;
};

class GU_API GU_PrimMetaSQuad : public GEO_PrimMetaSQuad
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimMetaSQuad()
    {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimMetaSQuad(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimMetaSQuad(gdp, offset)
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

    virtual void	normal(NormalComp &output) const {}

    // Optional Build Method 
    static GU_PrimMetaSQuad	*build(const GU_PrimMetaSQuadParms &parms,
				       const char *kernel="wyvill");

    virtual int		intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0, 
				int ignoretrim = 1) const;

private:
    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;

// Don't warn about overriding deprecated virtual methods
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
