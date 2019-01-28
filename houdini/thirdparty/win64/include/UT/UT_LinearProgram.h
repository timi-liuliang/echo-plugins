/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_LinearProgram_h__
#define __UT_LinearProgram_h__

#include "UT_API.h"
typedef enum {
    UT_LPERROR,
    UT_LPINFEASIBLE,
    UT_LPUNBOUNDED,
    UT_LPOPTIMAL
} UT_LPSolutionType;

#include "UT_Vector.h"
#include "UT_Matrix.h"

// This class implements a linear program solver using the simplex method.
// The solver algorithm is from "Numerical Recipes in C, Second Edition"
// by W.H. Press, S.A. Teukolsky, W.T. Vetterling, and B.P. Flannery.

class UT_API UT_LinearProgram {
public:
			 UT_LinearProgram();
    virtual		~UT_LinearProgram();

    // Set up the linear programming problem. Input number of constraints
    // or <= type, >= type, and = type, and the number of values in the
    // result. Input matrix has resultSize+1 columns and total num rules + 1
    // rows. First column is equation constants, subsequent columns are
    // coefficients in constraint equations. The first row is the objective
    // function, followed by rows that express the <= constraints, then >=
    // constraints, then = constraints. Input matrix should start at (1,1).
    // Returns 0 if there was some problem with the input (matrix wrong size),
    // otherwise returns 1.
    int			 setupProblem( int leRules, int geRules,
				       int equalRules, int resultSize,
				       UT_MatrixF *input, fpreal tolerance );

    // Solve the problem set up in setupProblem. Returns UT_LPERROR if no
    // problem has been set up, or if any of the constants (first column)
    // in the matrix are negative. Modifies the input matrix.
    UT_LPSolutionType	 minimize();
    UT_LPSolutionType	 maximize();

    // Fills result with the solution vector. Returns 0 if there has been
    // no problem set up, or the problem hasn't been solved, or did not
    // produce an optimal solution (output would be meaningless). Otherwise
    // it returns 1. This function is required because after calling solve(),
    // the input matrix requires some small massaging to get the output vector.
    int			 getSolution( UT_Vector *result );

private:
    // Performs the solution for both minimize and maximize
    UT_LPSolutionType	 solve();
    
    void		 destroyWorkingVectors();
    void		 createWorkingVectors();
    void		 findPivotCol( float *factors, int *indices,
				       int maxIndex, int abs, int findMax,
				       int &pivotCol, float &factor );
    void		 findPivotRow( int &pivotRow, int pivotCol );
    void		 doPivot( int doAux, int row, int col );

    int			 myMaximize;
    int			 myNumLERules;		// Number of rules of each
    int			 myNumGERules;		// type. Less than, greater
    int			 myNumEqualRules;	// than, and equality.
    int			 myNumRules;		// Total number of rules
    int			 myResultSize;		// Size of result vector
    int			 mySolved;		// Set to 1 when solve() called
    float		 myTolerance;		// Tolerance value
    UT_MatrixF		*myInput;		// Input array
    float		*myAuxObj;		// Auxiliary objective function
    int			*myL1, *myL3;		// Temp vectors
    int			*myRowIndices;		// Row number of each variable
    int			*myColumnIndices;	// Column number of each var
};

#endif
