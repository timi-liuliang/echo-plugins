/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimTriFan.h ( GU Library, C++)
 *
 * COMMENTS: TriFan utilitiy functions.
 */

#ifndef __GU_PrimTriFan__
#define __GU_PrimTriFan__

#include "GU_API.h"
#include <GEO/GEO_PrimTriFan.h>
#include "GU_Detail.h"

class UT_MemoryCounter;
class GEO_ConvertParms;

class GU_API GU_PrimTriFan : public GEO_PrimTriFan
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimTriFan() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimTriFan(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimTriFan(gdp, offset)
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
    virtual GEO_Primitive	*convert(GEO_ConvertParms &parms,
					 GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive	*convertNew(GEO_ConvertParms &parms);

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimTriFan::<functname>
    //        i.e.        partptr = GU_PrimTriFan::build(params...);

    // Optional Build Method 

    static GU_PrimTriFan	*build(GU_Detail *gdp, GA_Size numpts,
					bool appendpts = true);

    virtual void	normal(NormalComp &output) const;

protected:
private:
    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif

