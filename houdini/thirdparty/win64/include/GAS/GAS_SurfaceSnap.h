/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SurfaceSnap.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SurfaceSnap__
#define __GAS_SurfaceSnap__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <UT/UT_ThreadedAlgorithm.h>

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_SurfaceSnap : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    
    GETSET_DATA_FUNCS_F("snapdist", SnapDist);
    GETSET_DATA_FUNCS_F("snapangle", SnapAngle);

protected:
    explicit		 GAS_SurfaceSnap(const SIM_DataFactory *factory);
    virtual		~GAS_SurfaceSnap();

    /// Applies a force to the velocity field proportional
    /// to the vorticle values.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 shouldMultiThread(const UT_VoxelArrayF *field) const 
			 { return field->numTiles() > 1; }

    void		 applySurfaceSnap(SIM_ScalarField *surface,
				    const SIM_ScalarField *collision);

    THREADED_METHOD4_CONST(GAS_SurfaceSnap,  
			    shouldMultiThread(surface->field()),
			    doSurfaceSnap,
			    SIM_RawField *, surface,
			    const SIM_RawField *, collision,
			    float, snapdist,
			    float, snapangle);
    void		 doSurfaceSnapPartial(SIM_RawField *surface,
			    const SIM_RawField *collision,
			    float snapdist,
			    float snapangle,
			    const UT_JobInfo &info) const;
			
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SurfaceSnap,
			GAS_SubSolver,
			"Gas Surface Snap",
			getDopDescription());
};

#endif

