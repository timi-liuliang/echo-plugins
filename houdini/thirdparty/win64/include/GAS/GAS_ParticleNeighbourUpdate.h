/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleNeighbourUpdate.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleNeighbourUpdate__
#define __GAS_ParticleNeighbourUpdate__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_RawField;

class GAS_API GAS_ParticleNeighbourUpdate : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S("neighbourdataname", NeighbourName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

protected:
    explicit		 GAS_ParticleNeighbourUpdate(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleNeighbourUpdate();

    /// Rebuild the GU_NeighbourList of our particles.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleNeighbourUpdate,
			GAS_SubSolver,
			"Gas Particle Neighbour Update",
			getDopDescription());
};

#endif

