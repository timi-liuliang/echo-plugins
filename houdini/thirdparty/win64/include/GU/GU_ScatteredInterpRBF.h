/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	GU_ScatteredInterpRBF.h (GU Library, C++)
*
* COMMENTS: Each column is an example, one value for each DOF.
*	    Each row is a DOF, one value for each example.
*/

#ifndef __GU_SCATTEREDINTERPRBF_H__
#define __GU_SCATTEREDINTERPRBF_H__

#include "GU_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Functor.h>

#include "GU_LeastSquares.h"
#include "GU_ScatteredInterp.h"

class GU_API GU_ScatteredInterpRBF
{
public:
    GU_ScatteredInterpRBF()
	: myNumDOF(-1),
	myNumExamples(-1),
	myNumInputDimensions(-1)
    {
    }

    ~GU_ScatteredInterpRBF()
    {
    }

    struct Parms
    {
	bool			theClampInput;
	GU_ScatteredInterpType	theFormulationType;

	// rbf kernel params
	GU_RadialBasisKernelType    theKernelType;
	bool			    theHyperplane;	    //for rbf
	bool			    theMaximizeHyperplane;  //for rbf
	int			    theExp;		    //for thinplate rbf
	fpreal			    theFalloff;		    //for gaussian, multiquadric, inverse-multiquadric, and exponential bump
	bool			    thePositiveWeights;	    //for interpolate examples

	// LeastSquares solver parms
	GU_LeastSquaresSolverType   theSolverType;
	fpreal			    theDamping;
	int			    theMaxIterations;

	Parms()
	    : theClampInput(false),
	    theFormulationType(GU_SCATTERED_INTERP_AUTO),
	    theHyperplane(false),
	    theMaximizeHyperplane(false),
	    theKernelType(GU_KERNEL_GAUSSIAN),
	    theExp(0),
	    theFalloff(0.0),
	    thePositiveWeights(false),
	    theSolverType(GU_FAST_LEAST_SQUARES),
	    theDamping(0.0),
	    theMaxIterations(0)
	{
	}
    };

    bool solveForCoefficients(
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source, bool source_changed,
	const Parms &params);

    bool solveForCoefficients(
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source, bool source_changed,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &target, bool target_changed,
	const Parms &params);

    template <typename U>
    typename std::enable_if<GU_Matrix<U>::has_row, void>::type
	computeOutput(
	    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &input,
	    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source,
	    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &target,
	    U *output_data);

private:
    typedef UT_Functor1<fpreal, fpreal>		EvalFunctor1;

    bool hasChanged(
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &source,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &target,
	const Parms &parms) const;

    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic>   myRadialCoefficients;
    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic>   myLinearCoefficients;
    Eigen::Matrix<fpreal, Eigen::Dynamic, 1>		    mySourceMin;
    Eigen::Matrix<fpreal, Eigen::Dynamic, 1>		    mySourceMax;
    EvalFunctor1    myRadialKernel;

    // Cached params
    int	    myNumDOF;
    int	    myNumExamples;
    int	    myNumInputDimensions;
    Parms   myParms;
};

#include "GU_ScatteredInterpRBFImpl.h"

#endif // __GU_SCATTEREDINTERPRBF_H__
