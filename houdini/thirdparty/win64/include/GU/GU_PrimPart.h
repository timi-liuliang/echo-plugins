/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimParticle.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimParticle.h class...
 * 
 */

#ifndef __GU_PrimParticle_h__
#define __GU_PrimParticle_h__

#include "GU_API.h"
#include <GEO/GEO_PrimPart.h>
#include "GU_Detail.h"

class UT_MemoryCounter;
class GEO_ConvertParms;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimParticle				   //
// Descr:     This class is responsible for operations     //
//	 done on particle primitives.  As we need to       //
//       access the GU_Detail, the pointer is passed in    //
//       the constructor.  				   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimParticle : public GEO_PrimParticle
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimParticle() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimParticle(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimParticle(gdp, offset)
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

    virtual void	 normal(NormalComp &output) const;

    void		 restrictedNormal(NormalComp &output,
					  const UT_BitArray &skip_pts) const;

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimParticle::<functname>
    //        i.e.        partptr = GU_PrimParticle::build(params...);

    // Optional Build Method 

    static GU_PrimParticle	*build(GU_Detail *gdp, GA_Size numparts,
					bool appendPts = true);

    virtual void	clip(UT_Vector3 nml, float d = 0,
				GA_PrimitiveGroup *delgroup = 0);

private:
     void 		create(GA_Size numparts);

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
