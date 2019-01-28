/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	GU_ScatteredInterpWeightedRBF.h (GU Library, C++)
*
* COMMENTS: Each column is an example, one value for each DOF.
*	    Each row is a DOF, one value for each example.
*/

#ifndef __GU_SCATTEREDINTERPWEIGHTEDRBF_H__
#define __GU_SCATTEREDINTERPWEIGHTEDRBF_H__

#include "GU_API.h"
#include <SYS/SYS_Types.h>

#include <UT/UT_Array.h>
#include <UT/UT_Functor.h>
#include <UT/UT_VectorTypes.h>

#include "GU_LeastSquares.h"
#include "GU_ScatteredInterp.h"

class GA_Attribute;
class GA_AIFIndexPair;

class GU_API GU_ScatteredInterpWeightedRBF
{
public:
    GU_ScatteredInterpWeightedRBF()
	: myNumDOF(-1),
	myNumExamples(-1),
	myNumInputDimensions(-1)
    {
    }

    ~GU_ScatteredInterpWeightedRBF()
    {
    }

    struct Parms
    {
	int			theTupleSize;

	// rbf kernel params
	GU_RadialBasisKernelType    theKernelType;
	bool			    theHyperplane;	    //for rbf
	bool			    theMaximizeHyperplane;  //for rbf
	int			    theExp;		    //for thinplate rbf
	fpreal			    theFalloff;		    //for gaussian, multiquadric, inverse-multiquadric, and exponential bump
	bool			    thePositiveWeights;

	// LeastSquares solver parms
	GU_LeastSquaresSolverType   theSolverType;
	fpreal			    theDamping;
	int			    theMaxIterations;

	Parms()
	    : theTupleSize(0),
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
	const Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &sparse_vertex_region,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &sparse_vertex_weight,
	const UT_Vector2IArray &region_to_dofs_map,
	const UT_IntArray &dof_to_region_map,
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
	const Parms &parms) const;

    void computeVertexDOFWeights(
	const Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> &sparse_vertex_region,
	const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &sparse_vertex_weight,
	const UT_Vector2IArray &region_to_dofs_map,
	const UT_IntArray &dof_to_region_map);

    UT_Array<Eigen::Matrix<fpreal, Eigen::Dynamic, 1>>		    myVertexDOFWeights;
    UT_Array<Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic>> myRadialCoefficients;
    UT_Array<Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic>> myLinearCoefficients;
    UT_Array<EvalFunctor1>					    myRadialKernel;

    // Cached params
    int	    myNumDOF;
    int	    myNumExamples;
    int	    myNumVertices;
    int	    myNumInputDimensions;
    Parms   myParms;
};

#include "GU_ScatteredInterpWeightedRBFImpl.h"

#endif // __GU_SCATTEREDINTERPWEIGHTEDRBF_H__
