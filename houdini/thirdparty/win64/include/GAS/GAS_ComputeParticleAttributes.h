/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ComputeParticleAttributes.h
 *		This DOP is used to recompute desired attributes
 *		on the provided particle fluid field.
 *
 * COMMENTS:
 */

#ifndef __GAS_ComputeParticleAttributes__
#define __GAS_ComputeParticleAttributes__

#include "GAS_API.h"

#include <GU/GU_Detail.h>

#include <UT/UT_ThreadedAlgorithm.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_SPH;

class GAS_API GAS_ComputeParticleAttributes : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_B("dodensitygrad", DoDensityGrad);
    GET_DATA_FUNC_B("doneighbourvel", DoNeighbourVel);

protected:
    explicit		 GAS_ComputeParticleAttributes(
				    const SIM_DataFactory *factory);
    virtual		~GAS_ComputeParticleAttributes();

    /// Recomputes the values of specified attributes
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

    bool		 shouldMultiThread() { return true; }

protected:
    // A structure used to list which attributes we want
    // to store.
    struct Attribs {
	bool		 myStoreDensityGradient;
	bool		 myStoreNeighbourVelocity;
    };

    /// Multithreaded attribute calculation method
    THREADED_METHOD3(GAS_ComputeParticleAttributes, shouldMultiThread(),
				    computeAttributes,
				    GU_Detail *, gdp,
				    const GAS_SPH &, sph,
				    const Attribs &, attribs)
    void		 computeAttributesPartial(GU_Detail *gdp,
				    const GAS_SPH &sph,
				    const Attribs &attribs,
				    const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ComputeParticleAttributes,
			GAS_SubSolver,
			"Gas Compute Particle Attributes",
			getDopDescription());
};

#endif
