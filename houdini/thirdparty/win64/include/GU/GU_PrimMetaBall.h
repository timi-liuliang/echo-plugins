/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimMetaBall.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimMetaBall.h class...
 * 
 */

#ifndef __GU_PrimMetaBall_h__
#define __GU_PrimMetaBall_h__

#include "GU_API.h"
#include <GEO/GEO_PrimMetaBall.h>
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
// Class: 	GU_PrimMetaBall				   //
// Descr:     This class is responsible for operations     //
//	 done on sphere primitives.  As we need to access  //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       sphere with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimMetaBallParms
{
public:
    GU_PrimMetaBallParms(GU_Detail *gdp=NULL, GA_Offset ptoff=GA_INVALID_OFFSET,
			fpreal weight=2);

    GU_Detail	*gdp;
    GA_Offset	 ptoff; // use this point instead of yours
    UT_Matrix4	 xform;
    float	 weight;
};

class GU_API GU_PrimMetaBall : public GEO_PrimMetaBall
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimMetaBall()
    {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimMetaBall(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimMetaBall(gdp, offset)
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
    GEO_Primitive		*convert(GEO_ConvertParms &parms,
					 GA_PointGroup *usedpts = 0);
    GEO_Primitive		*convertNew(GEO_ConvertParms &parms);

    virtual void		normal(NormalComp &output) const {}

    // Optional Build Method 
    static GU_PrimMetaBall	*build(const GU_PrimMetaBallParms &parms,
				       const char *type = "wyvill");

    // Other representations. The rotation vector must be specified in a
    // RIGHT-HANDED system.

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
