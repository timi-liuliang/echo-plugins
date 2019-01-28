/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimCircle.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimCircle.h class...
 * 
 */

#ifndef __GU_PrimCircle_h__
#define __GU_PrimCircle_h__

#include "GU_API.h"
#include <GEO/GEO_PrimCircle.h>
#include "GU_Detail.h"

class GA_ElementWranglerCache;
class GEO_ConvertParms;
class GEO_Face;
class GU_PrimPoly;
class GU_PrimNURBCurve;
class GU_PrimRBezCurve;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimCircle				   //
// Descr:     This class is responsible for operations     //
//	 done on circle primitives.  As we need to access  //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       circle with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimCircleParms
{
public:
    GU_PrimCircleParms(GU_Detail *gdp=NULL, GA_Offset ptoff=GA_INVALID_OFFSET);

    GU_Detail		*gdp;
    GA_Offset		 ptoff;		// use this point instead of your own
    UT_Matrix4		 xform;
    int			 divisions;
    float		 beginAngle;	// Radians
    float		 endAngle;	// Radians
    GU_CircleType	 type;		// Type of circle
    int			 order;
    int			 imperfect;	// nonrational
};

class GU_API GU_PrimCircle : public GEO_PrimCircle
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimCircle() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimCircle(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimCircle(gdp, offset)
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

    // Conversion Methods: to polygon, nurb curve, and bezier curve
    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
				 GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    virtual void	 normal(NormalComp &output) const;
    virtual int		 intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0, 
				int ignoretrim = 1) const;

    static GEO_Primitive	*build(const GU_PrimCircleParms &parms,
					GA_PrimitiveTypeId type=GEO_PRIMCIRCLE);

protected:
private:
    void		 passAttributesTo(GEO_Primitive &dest,
					  GA_ElementWranglerCache &wranglers,
					  int preserve_groups) const;

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
