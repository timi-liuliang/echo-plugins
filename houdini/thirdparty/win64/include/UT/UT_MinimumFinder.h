/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Minimum finding (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _UT_MinimumFinder_h_
#define _UT_MinimumFinder_h_

#include "UT_API.h"
#include "UT_Vector.h"

class UT_API UT_MinimumFinder {
public:
    // 1D minimum finders:
    //

    // Returns 1 on success.
    // Assumed a minimum is bracketted by (a,c).  Derivitive required.
    // Values at a/c need not be defined, as they are assume = INFINITY.
    int brent(void (*func)(float x, float &val, float &der, void *data),
	      void *data,
	      float ax, float bx, float cx,
	      float tol, float &result, float &val, int maxIter=200) const;

    // Multi-dimensional minimum finders:
    //


    //
    // Gradient is needed. Specify initial guess in result.
    // [x1,x2] specify the boundary that the func is defined within.
    // Note func:R^n->R.
    int frprPCG(void (*func)(const UT_Vector &x, float &val, UT_Vector &grad,
			   void *data), void *data,
	       const UT_Vector &x1, const UT_Vector &x2,
	       float tol,
	       UT_Vector &result, int maxIter=200);

private:
    // Performs line minimization along line defined by dir, starting
    // at org.  Brackets search by (x1, x2).  Before being called,
    // myOrg, myDir, and myScratch must be allocated to the proper
    // dimmensions.
    int lineMin(void (*func)(const UT_Vector &x, float &val, UT_Vector &grad,
			     void *data), void *data,
		const UT_Vector &org, const UT_Vector &dir,
		const UT_Vector &x1, const UT_Vector &x2,
		float tol, UT_Vector &result, UT_Vector &displacement,
		float &val, int maxIter=200);
		

    // Variables required to tell evaluateLine how to translate to/from
    // n-space
    void (*myFunc)(const UT_Vector &, float &, UT_Vector &, void *);
    UT_Vector *myOrg, *myDir, *myScratch, *myGrad;
    void *myData;

    // Used internally by lineMin to translate request in terms of 1d
    // line into terms of n-space.
    friend void evaluateLine(float x, float &val, float &der, void *data);
};

#endif
