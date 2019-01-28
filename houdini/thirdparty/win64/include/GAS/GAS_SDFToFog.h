/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SDFToFog.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SDFToFog_h__
#define __GAS_SDFToFog_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <SIM/SIM_Names.h>


class GAS_API GAS_SDFToFog : public GAS_SubSolver
{
public:
    /// These macros are used to create the accessors
    /// getFieldDstName and getFieldSrcName functions we'll use
    /// to access our data options.
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, Velocity);
    GET_DATA_FUNC_I("sampling", Sampling);
    GET_DATA_FUNC_I("numsupersamples", NumSuperSamples);
    GET_DATA_FUNC_B(SIM_NAME_INVERT, Invert);

protected:
    explicit		 GAS_SDFToFog(const SIM_DataFactory *factory);
    virtual		    ~GAS_SDFToFog();

    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    virtual void initializeSubclass();

    void doFractionsSampled(SIM_RawField *dst,
                            const SIM_RawField *sdf) const;

    void doFractionsVolumeFraction(SIM_RawField *dst,
				   const SIM_RawField *sdf) const;

    void doFractionsFace(SIM_RawField *dst,
                         const SIM_RawField *sdf,
                         int axis) const;

private:
    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription	*getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SDFToFog,
			GAS_SubSolver,
			"Gas SDF To Fog",
			getDopDescription());

};


#endif

