/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Lookup.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Lookup__
#define __GAS_Lookup__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;

class GAS_API GAS_Lookup : public GAS_SubSolver
{
public:
    bool		 shouldMultiThread(const UT_VoxelArrayF *field) const 
			 { return field->numTiles() > 1; }

protected:
    explicit		 GAS_Lookup(const SIM_DataFactory *factory);
    virtual		~GAS_Lookup();

    /// Lookups scalar fields by moving along curvature or performing
    /// relaxation steps.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 doLookup(SIM_RawField *dstfield,
				const SIM_VectorField *posfield,
				const SIM_RawField *srcfield);

    THREADED_METHOD3_CONST(GAS_Lookup, shouldMultiThread(dstfield->field()), 
		    lookup,
		    SIM_RawField *, dstfield,
		    const SIM_VectorField *, posfield,
		    const SIM_RawField *, srcfield);
    void		 lookupPartial(SIM_RawField *dstfield,
			    const SIM_VectorField *posfield,
			    const SIM_RawField *srcfield, 
			    const UT_JobInfo &jobinfo) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Lookup,
			GAS_SubSolver,
			"Gas Lookup",
			getDopDescription());
};

#endif
