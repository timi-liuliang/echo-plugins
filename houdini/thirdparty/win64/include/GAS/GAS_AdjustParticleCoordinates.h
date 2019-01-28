/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_AdjustElasticity.h ( GAS Library, C++)
 *
 * COMMENTS:	This node is used to adjust and reinitialize
 *		a coordinate system attached to the particles
 *		in a particle fluid simulation.
 *
 */

#ifndef __GAS_AdjustParticleCoordinates__
#define __GAS_AdjustParticleCoordinates__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <GA/GA_Handle.h>
#include <UT/UT_BoundingBox.h>

class GU_Detail;

class GAS_API GAS_AdjustParticleCoordinates : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY,	GeometryName);

protected:
    explicit		 GAS_AdjustParticleCoordinates(const SIM_DataFactory *factory);
    virtual		~GAS_AdjustParticleCoordinates();

    /// Performs any necessary adjustments to the coordinate
    /// system attached to the fluid particles.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

private:
    /// Populates the provided attribute handles, and returns
    /// false if any attribute does not exist.
    bool		 getAttributes(GU_Detail *gdp,
				    GA_ROHandleV3 &bboxminh,
				    GA_ROHandleV3 &bboxmaxh,
				    GA_ROHandleV3 &coordinatescaleh,
				    GA_RWHandleF &transitionstateh,
				    GA_ROHandleF &periodh,
				    GA_ROHandleF &transitionlengthh,
				    GA_RWHandleV3 &coordinate1attrib,
				    GA_RWHandleV3 &coordinate2attrib,
				    GA_RWHandleR &lastadjusth);

    /// Initializes a new coordinate system for the fluid.
    void		 initNewCoordinates(GA_Detail *gdp,
				    const UT_BoundingBox &bbox,
				    const UT_Vector3 &coordinateScales,
				    const GA_RWHandleV3 &coordinateGah);

    /// Swaps the coordinate systems.
    void		 swapCoordinates(GA_Detail *gdp,
				    const GA_RWHandleV3 &coordinate1Gah,
				    const GA_ROHandleV3 &coordinate2Gah);

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_AdjustParticleCoordinates,
			GAS_SubSolver,
			"Gas Adjust Particle Coordinates",
			getDopDescription());
};

#endif
