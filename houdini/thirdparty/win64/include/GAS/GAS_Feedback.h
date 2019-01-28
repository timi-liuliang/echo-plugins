/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Feedback.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Feedback__
#define __GAS_Feedback__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_RawField;
class SIM_ScalarField;
class SIM_IndexField;

class GAS_API GAS_Feedback : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_PRESSURE, PressureName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_INDEX, IndexName);

    GET_DATA_FUNC_F("feedbackscale", FeedbackScale);
    
protected:
    explicit		 GAS_Feedback(const SIM_DataFactory *factory);
    virtual		~GAS_Feedback();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Calculates the appropriate feedback on the objects given
    /// the calculated pressure field.  This will add the appropraite
    /// SIM_FEEDBACK impacts.
    void		 addPressureFeedback(SIM_Engine &engine,
					const SIM_ScalarField *collisionmask,
					SIM_Object *fluidobj,
					const SIM_IndexField *collideindex,
					const SIM_RawField *pressure,
					UT_DMatrix4 toworld,
					const SIM_Time &time) const;

    /// Calculates the appropriate feedback on the objects given
    /// the calculated pressure field and collision weights.  This will add the
    /// appropraite SIM_FEEDBACK impacts.
    void		 addPressureFeedbackWeighted(SIM_Engine &engine,
					const SIM_VectorField *colweights,
					int axis,
					SIM_Object *fluidobj,
					const SIM_IndexField *collideindex,
					const SIM_RawField *pressure,
					UT_DMatrix4 toworld,
					const SIM_Time &time) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Feedback,
			GAS_SubSolver,
			"Gas Feedback",
			getDopDescription());
};

#endif

