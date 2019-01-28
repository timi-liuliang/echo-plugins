/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Determines which tetrahedra should be considered part of a solid
 *	     object and which should not
 */

#ifndef __GU_Solidify_h__
#define __GU_Solidify_h__

#include <GEO/GEO_PrimPoly.h>
#include <GEO/GEO_PrimTetrahedron.h>
#include <UT/UT_Interrupt.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_SparseMatrix.h>
#include <UT/UT_Vector.h>
#include <UT/UT_MatrixSolver.h>
#include <vector>
#include "GU_TetConnectivity.h"

//#define USE_EIGEN

#ifdef USE_EIGEN
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#endif

class GU_API GU_Solidify
{
private:
#ifdef USE_EIGEN
    typedef Eigen::SparseMatrix<double>  MatrixType;
    typedef Eigen::SparseLU<MatrixType>	 SolverType;
    //typedef Eigen::ConjugateGradient<MatrixType, Eigen::Lower|Eigen::Upper>	 SolverType;
    typedef Eigen::VectorXd		 VectorType;
    typedef Eigen::Triplet<double>	 TripletType;
#else
    typedef UT_SparseMatrixCSRT<double>  MatrixType;
    typedef UT_MatrixIterSolverT<double> SolverType;
    typedef UT_VectorT<double>		 VectorType;
    typedef MatrixType::Triplet		 TripletType;
#endif

    // fills given interiorgrp with tets not on the boundary of the tetrahedralization
    void findInteriorTets(
	    const GU_TetrahedronAdjacency& adjacency,
	    const GA_PrimitiveGroup *search_grp,
	    GA_PrimitiveGroup* interiorgrp);

public:

    GU_Solidify()
	: mySrc(nullptr)
    { }
    ~GU_Solidify() { }

    enum Status
    {
	SUCCESS = 0,
	NO_INTERIOR_TETS,
	SOLVER_FAIL,
	INTERRUPTED
    };

    Status solidify(const GU_Detail *gdp, 
		    const GA_PrimitiveGroup *tet_grp,
		    const GA_PrimitiveGroup *poly_grp);

    bool   hasValidSolidity(GA_Offset tet)
    {
	if ( !mySrc ) return false;
	return myTetSolidityIndex[mySrc->primitiveIndex(tet)] != -1;
    }

    fpreal getSolidity(GA_Offset tet)
    {
	if ( !mySrc )
	    return -1.0;
	auto idx = myTetSolidityIndex[mySrc->primitiveIndex(tet)];
	if ( idx == -1 )
	    return -1.0; // not solid by default
#ifdef USE_EIGEN
	return mySolidity[idx];
#else
	return mySolidity(idx);
#endif
    }
    const GU_Detail*		getSrc() { return mySrc; }
    const GA_PrimitiveGroup*	getTetGrp() { return myTetGrp; }
    const GA_PrimitiveGroup*	getPolyGrp() { return myPolyGrp; }

private:
    const GU_Detail		*mySrc;
    const GA_PrimitiveGroup     *myTetGrp;
    const GA_PrimitiveGroup     *myPolyGrp;
    UT_ExintArray		 myTetSolidityIndex; // GA_Index -> system index map
    VectorType			 mySolidity;
};

#endif // __GU_Solidify_h__
