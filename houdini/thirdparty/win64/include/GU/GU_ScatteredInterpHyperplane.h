/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	GU_ScatteredInterpHyperplane.h (GU Library, C++)
*
* COMMENTS: Each column is an example, one value for each DOF.
*	    Each row is a DOF, one value for each example.
*/

#ifndef __GU_SCATTEREDINTERPHYPERPLANE_H__
#define __GU_SCATTEREDINTERPHYPERPLANE_H__

#include "GU_API.h"
#include <SYS/SYS_Types.h>

#include "GU_LeastSquares.h"
#include "GU_ScatteredInterp.h"

class GU_API GU_ScatteredInterpHyperplane
{
public:
    GU_ScatteredInterpHyperplane()
	: myNumDOF(-1),
	myNumExamples(-1),
	myNumInputDimensions(-1)

    {
    }

    ~GU_ScatteredInterpHyperplane()
    {
    }

    struct Parms
    {
	bool			theClampInput;
	GU_ScatteredInterpType	theFormulationType;

	// LeastSquares solver parms
	GU_LeastSquaresSolverType   theSolverType;
	fpreal			    theDamping;
	int			    theMaxIterations;

	Parms()
	    : theClampInput(false),
	    theFormulationType(GU_SCATTERED_INTERP_AUTO),
	    theSolverType(GU_FAST_LEAST_SQUARES),
	    theDamping(0.0),
	    theMaxIterations(0)
	{
	}
    };

    bool solveForCoefficients(
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source, bool source_changed,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &target, bool target_changed,
	const Parms &params);

    template <typename U>
    typename std::enable_if<GU_Matrix<U>::has_row, void>::type
	computeOutput(
	    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &input,
	    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source,
	    U *output_data);

private:
    bool hasChanged(
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &target,
	const Parms &parms) const;

    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic>   myLinearCoefficients;
    Eigen::Matrix<fpreal, Eigen::Dynamic, 1>		    mySourceMin;
    Eigen::Matrix<fpreal, Eigen::Dynamic, 1>		    mySourceMax;

    // Cached params
    int	    myNumDOF;
    int	    myNumExamples;
    int	    myNumInputDimensions;
    Parms   myParms;
};

#include "GU_ScatteredInterpHyperplaneImpl.h"

#endif // __GU_SCATTEREDINTERPHYPERPLANE_H__
