/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FitCubicImpl.h (C++)
 *
 * COMMENTS:	Given a group within a detail, fit curves to
 *		each polygon.  Original fitting code from "Graphic Gems":
 *		Extended to cubic curves.
 *
 *   An Algorithm for Automatically Fitting Digitized Curves
 *   by Philip J. Schneider
 *   mostly from "Graphics Gems", Academic Press, 1990
 *
 */

#ifndef __UT_FitCubicImpl__
#define __UT_FitCubicImpl__

#ifndef __UT_FitCubic_H__
#error Do not include this file directly. Include UT_FitCubic.h instead.
#endif


#include "UT_Vector2.h"
#include "UT_FitCubic.h"
#include "UT_Interrupt.h"
#include <SYS/SYS_Math.h>

#define ACUTE_ANGLE		1.6
#define MAXITERATIONS		4



/*
 * allocate a new node in the linked list
 * containing the non-redundant points of the curve
 */

template <typename T>
typename UT_FitCubicT<T>::Span *
UT_FitCubicT<T>::appendCurve(CubicCurve fcurve, int islinear)
{
    Span	*node = new Span;

    if (node)
    {
	node->point[0] = fcurve[0]; // last point always redundant
	node->point[1] = fcurve[1];
	node->point[2] = fcurve[2];
	node->isLinear = islinear;
	node->next = 0;
    }

    return (node);
}


/*
 *  FitCurve :
 *     Fit a curve to a set of digitized points 
 *
 */

template <typename T>
UT_FitCubicT<T>::UT_FitCubicT(void)
{
    myHead = 0;
    myContainsCurve = 0;
    myError2 = 1.0;
    myNSpans = 0;
    myFitType = UT_FIT_BEZIER;
    myCurveType = UT_FIT_BOTH;
}

template <typename T>
UT_FitCubicT<T>::~UT_FitCubicT()
{
    destroySolution();
}

template <typename T>
void
UT_FitCubicT<T>::destroySolution()
{
    Span	*temp;

    while(myHead)
    {
	temp = myHead;
	myHead = myHead->next;
	delete temp;
    };

    myContainsCurve = 0;
}


template <typename T>
int
UT_FitCubicT<T>::fitCurve(Vector2 *d, int npts, int closed, fpreal error2,
							bool preserveExtrema)
{
    Vector2		 tHat1, tHat2;	/* Unit tangent vectors at endpoints */
    Span		*tail=0, *headS, *tailS;
    int			 start, corner;
    int			 last = npts - 1;
    int			 nsegs;
    UT_Interrupt	*boss;
    
    
    // remove points from corner to corner and fit curve to each segment

    start = 0;

    myError2 = error2;
    myNumPoints = npts;
    myClosed = closed;

    myNSpans = 0;

    destroySolution();

    boss = UTgetInterrupt();
    
    if(boss->opStart("Fitting Curve"))
    {
	do
	{
	    corner = last;

	    if (myCurveType == UT_FIT_POLYS || myCurveType == UT_FIT_BOTH)
		findCorner(d, start, last, &corner);
	    else
		corner = last;

	    nsegs = 0;

	    if (myCurveType == UT_FIT_POLYS)
		nsegs = fitLine(d, start, corner, &headS, &tailS, 1);
	    else if (myCurveType == UT_FIT_BOTH)
		nsegs = fitLine(d, start, corner, &headS, &tailS, 0);

	    if (!nsegs && 
		((myCurveType == UT_FIT_BOTH) || (myCurveType == UT_FIT_CURVES)))
	    {
		tHat1 = computeCenterTangent(d, start);
		tHat1.negate();
		tHat2 =  computeCenterTangent(d, corner);
	    
		// use combination of both if at seam
		if (closed && (corner==last) && !isSeamCorner(d, last))
		{
		    tHat2 -= tHat1;
		    tHat2.normalize();
		    tHat1 = tHat2*(-1.0);
		}
	        nsegs = fitCubic(d, start, corner, tHat1, tHat2, 
				     &headS, &tailS, preserveExtrema, boss);
	    }

	    myNSpans += nsegs;


	    // join with existing curve
	    if(headS && tailS)
	    {
		if (myHead)
		{
		    tail->next = headS;
		    tail = tailS;
		}
		else
		{
		    myHead = headS;
		    tail = tailS;
		}
	    }
	    
	    start = corner;

	    if(boss->opInterrupt())
		break;

	} while(start < last);
    }
    boss->opEnd();

    return myNSpans;

}


/*
 * Search through the list of points
 * If there appears to be an acute angle set splitPoint to center
 *
 */

template <typename T>
void
UT_FitCubicT<T>::findCorner(Vector2 *d, int first, int last, int *splitPoint)
{
    fpreal	theta;
    Vector2	v1, v2;
    int		i;

    // prev and next must be 2 steps away from center

    for (i=first+2; i<last-2; ++i)
    {
	// if angle(prev, i, next) < 95 then corner

	v1 = d[i+2] - d[i];
	v1.normalize();

	v2 = d[i-2] - d[i];
	v2.normalize();

	theta = SYSacos(v1.dot(v2));

	if (theta < ACUTE_ANGLE)
	{
	    *splitPoint = i;
	    return;
	}

    }
}


// check if the closed shape contains a corner at point 0

template <typename T>
int
UT_FitCubicT<T>::isSeamCorner(Vector2 *d, int last)
{
    fpreal	theta;
    Vector2	v1, v2;

    // prev and next must be 2 steps away from center
    // if angle(prev, i, next) < 95 then corner

    if (last<2) return 1;	// not enought points

    v1 = d[2] - d[0];
    v1.normalize();

    v2 = d[last-1] - d[0];
    v2.normalize();

    theta = SYSacos(v1.dot(v2));

    if (theta < ACUTE_ANGLE)
	return 1;
    else
	return 0;
}




/*
 *  fitLine :
 *     Fit a line segment to a set of digitized points
 *     Return the number of segments required (0 or 1)
 */

template <typename T>
int 
UT_FitCubicT<T>::fitLine(Vector2 *d, int first, int last,  
		     Span **head, Span **tail,
		     int recurse)
{
    CubicCurve		 fcurve;	/* Control pts of fitted curve*/
    T			*u = 0;		/* Parameter values for point  */
    fpreal		 maxError2;	/* Maximum fitting error        */
    int			 npts;		/* Number of points in subset  */
    Vector2		 temp1, temp2;	/* tangents in using line approx */
    fpreal	 	 dist;
    int			 maxi;
    Span		*headL, *tailL;	/* Preceeding subsection */
    Span		*headR, *tailR;	/* Proceeding subsection */
    int			 nsegs = 0;

    npts = last - first + 1;

    /*  Use heuristic to to form line */

    dist  = 1.0/3.0;
    temp1 = (d[last]-d[first]);
    temp2 = (d[first]-d[last]);
    fcurve[0] = d[first];
    fcurve[3] = d[last];
    fcurve[1] = fcurve[0] + temp1*dist;
    fcurve[2] = fcurve[3] + temp2*dist;

    /* if fewer than 2 points use this line */

    if (npts <= 2) 
    {
	*head = *tail = appendCurve(fcurve, 1);
	return 1;
    }

    /* else check if line fits well enough */

    u = chordLengthParameterize(d, first, last);
    maxError2 = computeLineError(d, first, last, fcurve, u, &maxi);
    delete [] u;

    if (maxError2 < myError2) 
    {
	*tail = *head = appendCurve(fcurve, 1);
        return 1;
    }
    else if (recurse)
    {
	nsegs =  fitLine(d, first, maxi, &headL, &tailL, recurse);
	nsegs += fitLine(d, maxi, last, &headR, &tailR, recurse);

	if(headL && tailL && headR && tailR)
	{
	    *head = headL;
	    tailL->next = headR;
	    *tail = tailR;
	}
	else if(headL && tailL)
	{
	    *head = headL;
	    tailL->next = 0;
	    *tail = tailL;
	}
	else
	{
	    *head = 0;
	    *tail = 0;
	}
    }

    return nsegs;
}




/*
 *  FitCubic :
 *     Fit a curve to a (sub)set of digitized points
 *     Return the number of segments required
 */

#include <stdio.h>

template <typename T>
int 
UT_FitCubicT<T>::fitCubic(Vector2 *d, int first, int last, Vector2 tHat1, 
		      Vector2 tHat2, Span **head, Span **tail,
		      bool preserveExtrema, UT_Interrupt *boss)
{
    CubicCurve		 rcurve;	/* Control pts of fitted curve*/
    T			*u = 0;		/* Parameter values for point  */
    fpreal		 maxError2;	/* Maximum fitting error        */
    int			 splitPoint;	/* Point to split point set at  */
    int			 npts;		/* Number of points in subset  */
    fpreal		 iterationError;/* Error below which try iterating  */
    Vector2		 tHatCenter;	/* Unit tangent vector at splitPoint */
    int			 i;              
    Span		*headL, *tailL;	/* Preceeding subsection */
    Span		*headR, *tailR;	/* Proceeding subsection */
    int			 nsegs;		/* Number of curves required */

    /* if fewer than 2 points use heuristic*/

    npts = last - first + 1;
    if (npts <= 2) 
    {
	Vector2	tempv(d[first] - d[last]);
	fpreal	dist = tempv.length() / 3.0;
	rcurve[0] = d[first];
	rcurve[3] = d[last];
	rcurve[1] = rcurve[0] + tHat1*dist;
	rcurve[2] = rcurve[3] + tHat2*dist;
	*head = *tail = appendCurve(rcurve, 1);
	return 1;
    }

    /*  attempt to fit curve */

    if (myFitType == UT_FIT_BEZIER)
    {
	u = chordLengthParameterize(d, first, last);
	generateBezier(d, first, last, u, tHat1, tHat2, rcurve);
	maxError2 = computeBezierError(d, first, last, rcurve, u, &splitPoint);
    }
    else
    {
	simpleCurve(d, first, last, tHat1, tHat2, rcurve);
	maxError2 = computeCubicError(d, first, last, rcurve, &splitPoint,
							    preserveExtrema);
    }


    if (maxError2 < myError2) 
    {
	if (u) delete [] u;	// RB: stop memory leak
	*tail = *head = appendCurve(rcurve, 0);
	myContainsCurve = 1;
        return 1;
    }


    /*  If error not too large, try some reparameterization  */
    /*  and iteration */

    if (myFitType == UT_FIT_BEZIER)
    {
	T		*uPrime = 0;	/* Improved parameter values */

	iterationError = myError2 * myError2;
	if (maxError2 < iterationError) 
	{
	    for (i = 0; i < MAXITERATIONS; i++) 
	    {
		uPrime = reparameterize(d, first, last, u, rcurve);
		generateBezier(d, first, last, uPrime, tHat1, tHat2, rcurve);
		maxError2 = computeBezierError(d, first, last, rcurve, 
					   uPrime, &splitPoint);
		if (maxError2 < myError2) 
		{
		    delete [] u;		// RB: stop memory leaks
		    delete [] uPrime;
		    *tail = *head = appendCurve(rcurve, 0);
		    myContainsCurve = 1;
		    return 1;
		}

		delete [] u;
		u = uPrime;
	    }
	}

	if (uPrime) delete [] uPrime;
    }
    else
    {
	delete [] u;
    }


    /* Fitting failed -- split at max error point and fit recursively */
    if(!boss->opInterrupt())
    {
	tHatCenter = computeCenterTangent(d, splitPoint);

	nsegs = fitCubic(d, first, splitPoint, tHat1, tHatCenter, &headL,
						&tailL, preserveExtrema, boss);

	tHatCenter *= -1.0;

	nsegs += fitCubic(d, splitPoint, last, tHatCenter, tHat2, &headR,
						&tailR, preserveExtrema, boss);

	/* Concatenate two pieces together */

	if(headL && tailL && headR && tailR)
	{
	    *head = headL;
	    tailL->next = headR;
	    *tail = tailR;
	}
	else if(headL && tailL)
	{
	    *head = headL;
	    tailL->next = 0;
	    *tail = tailL;
	}
	else
	{
	    *head = 0;
	    *tail = 0;
	}
    }
    else
    {
	nsegs = 0;
	tail =0;
	head =0;
    }

    return nsegs;

}


/*
 *  generateBezier :
 *  Use least-squares method to find Bezier control points for region.
 *
 */


/*
 * use the Wu/Barsky heuristic 
 */

template <typename T>
void
UT_FitCubicT<T>::simpleCurve(Vector2 *d, int first, int last,
			 Vector2 tHat1, Vector2 tHat2,
			 CubicCurve rcurve)
{
    Vector2	tempv = Vector2(d[first] - d[last]);
    fpreal	dist = tempv.length() / 3.0;

    rcurve[0] = d[first];
    rcurve[3] = d[last];
    rcurve[1] = rcurve[0] + tHat1*dist;
    rcurve[2] = rcurve[3] + tHat2*dist;
    return;
}

template <typename T>
void 
UT_FitCubicT<T>::generateBezier(Vector2 *d, int first, int last, 
			   T *uPrime, Vector2 tHat1, 
			   Vector2 tHat2, CubicCurve rcurve)
{
    int		i;
    Vector2	*A0, *A1;		/* Precomputed rhs for eqn      */
    int		npts;                   /* Number of pts in sub-curve */
    T		C[2][2];		/* Matrix C             */
    T		X[2];                   /* Matrix X                     */
    fpreal	det_C0_C1,              /* Determinants of matrices     */
		det_C0_X,
		det_X_C1;
    fpreal	alpha_l, alpha_r;	/* Alpha values, left and right */
    Vector2	tmp;			/* Utility variable             */

    npts = last - first + 1;

    /* Allocate 2-columned A array */

    A0 = new Vector2[npts];
    if (!A0) return;

    A1 = new Vector2[npts];
    if (!A1)
    {
	delete [] A0;
	return;
    }

    /* Compute the A's */
    for (i=0; i<npts; i++) {
	A0[i] = tHat1*BEZ_MULT_1(uPrime[i]);
	A1[i] = tHat2*BEZ_MULT_2(uPrime[i]);
    }

    /* Create the C and X matrices     */
    C[0][0] = 0.0;
    C[0][1] = 0.0;
    C[1][0] = 0.0;
    C[1][1] = 0.0;
    X[0]    = 0.0;
    X[1]    = 0.0;

    for (i = 0; i < npts; i++) 
    {
        C[0][0] += A0[i].dot(A0[i]);
	C[0][1] += A0[i].dot(A1[i]);
	C[1][0] = C[0][1];
	C[1][1] += A1[i].dot(A1[i]);

	tmp  = d[last]*BEZ_MULT_3(uPrime[i]);
	tmp += d[last]*BEZ_MULT_2(uPrime[i]);
	tmp += d[first]*BEZ_MULT_1(uPrime[i]);
	tmp += d[first]*BEZ_MULT_0(uPrime[i]);
	tmp = d[first+i] - tmp;

	X[0] += A0[i].dot(tmp);
	X[1] += A1[i].dot(tmp);
    }

    /* Finished with A array */
    delete [] A0;
    delete [] A1;

    /* Compute the determinants of C and X     */
    det_C0_C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
    det_C0_X  = C[0][0] * X[1]    - C[0][1] * X[0];
    det_X_C1  = X[0]    * C[1][1] - X[1]    * C[0][1];

    /* Finally, derive alpha values    */
    if (SYSequalZero(det_C0_C1))
    {
	alpha_l = alpha_r = 0;
    }
    else
    {
	alpha_l = det_X_C1 / det_C0_C1;
	alpha_r = det_C0_X / det_C0_C1;
    }

    if (alpha_l < 0.0 || alpha_r < 0.0) 
    {
	simpleCurve(d, first, last, tHat1, tHat2, rcurve);
    }
    else
    {
	/*  First and last control points of the curve are */
	/*  positioned exactly at the first and last data points */
	/*  Control points 1 and 2 are positioned an alpha distance out */
	/*  on the tangent vectors, left and right, respectively */

	rcurve[0] = d[first];
	rcurve[3] = d[last];
	rcurve[1] = rcurve[0] + tHat1*alpha_l;
	rcurve[2] = rcurve[3] + tHat2*alpha_r;
    }
}


/*
 *  reparameterize:
 *     Given set of points and their parameterization, try to find
 *   a better parameterization.
 *
 */

template <typename T>
T *
UT_FitCubicT<T>::reparameterize(Vector2 *d, int first, int last, T *u, 
			    CubicCurve fcurve)
{
    int        npts = last-first+1;    
    int        i;
    T	      *uPrime;                /*  New parameter values        */

    uPrime = new T[npts];

    for (i = first; i <= last; i++)
       uPrime[i-first] = newtonRaphsonRootFind(fcurve, d[i], u[i- first]);
    
    return (uPrime);
}



/*
 *  newtonRaphsonRootFind :
 *     Use Newton-Raphson iteration to find better root.
 */

template <typename T>
fpreal 
UT_FitCubicT<T>::newtonRaphsonRootFind(CubicCurve Q, Vector2 P, fpreal u)
{
    fpreal	numerator, denominator;
    Vector2	Q1[3], Q2[2];		/*  Q' and Q''                  */
    Vector2	Q_u, Q1_u, Q2_u;	/*u evaluated at Q, Q', & Q''  */
    fpreal	uPrime;			/*  Improved u                  */
    int		i;
    
    /* Compute Q(u)    */
    if (myFitType == UT_FIT_BEZIER)
	Q_u = calcBezier3(Q, u);
    
    /* Generate control vertices for Q'        */
    for (i = 0; i <= 2; i++) 
	Q1[i] = (Q[i+1] - Q[i]) * 3.0;
    
    /* Generate control vertices for Q'' */
    for (i = 0; i <= 1; i++) 
	Q2[i] = (Q1[i+1] - Q1[i]) * 2.0;
    
    /* Compute Q'(u) and Q''(u)        */
    if (myFitType == UT_FIT_BEZIER)
	Q1_u = calcBezier2(Q1, u);

    Q2_u = Q2[0]*(1.0-u);
    Q2_u += Q2[1]*u;

    /* Compute f(u)/f'(u) */
    numerator = Q1_u.dot(Q_u - P);
    denominator = Q1_u.length2() + Q2_u.dot(Q_u - P);

    
    /* u = u - f(u)/f'(u) */

    if (!SYSequalZero(denominator))
	uPrime = u - (numerator/denominator);
    else
	uPrime = u;

    return (uPrime);
}

       
                      
/*
 *  Bezier :
 *     Evaluate a Bezier curve at a particular parameter value
 * 
 */

template <typename T>
typename UT_FitCubicT<T>::Vector2 
UT_FitCubicT<T>::calcBezier3(Vector2 *V, fpreal t)
{
    fpreal	u = 1.0 - t;
    Vector2	Vtemp[4]; /* Local copy of control points */

    /* Triangle computation    */

    Vtemp[0]  = V[0]*u;
    Vtemp[0] += V[1]*t;

    Vtemp[1]  = V[1]*u;
    Vtemp[1] += V[2]*t;

    Vtemp[2]  = V[2]*u;
    Vtemp[2] += V[3]*t;

    Vtemp[0]  = Vtemp[0]*u;
    Vtemp[0] += Vtemp[1]*t;

    Vtemp[1]  = Vtemp[1]*u;
    Vtemp[1] += Vtemp[2]*t;

    Vtemp[0]  = Vtemp[0]*u;
    Vtemp[0] += Vtemp[1]*t;

    return Vtemp[0];
}


template <typename T>
typename UT_FitCubicT<T>::Vector2 
UT_FitCubicT<T>::calcBezier2(Vector2 *V, fpreal t)
{
    fpreal	u = 1.0 - t;
    Vector2	Vtemp[2];

    /* Triangle computation    */

    Vtemp[0]  = V[0]*u;
    Vtemp[0] += V[1]*t;

    Vtemp[1]  = V[1]*u;
    Vtemp[1] += V[2]*t;

    Vtemp[0]  = Vtemp[0]*u;
    Vtemp[0] += Vtemp[1]*t;

    return Vtemp[0];
}

template <typename T>
typename UT_FitCubicT<T>::Vector2 
UT_FitCubicT<T>::calcCubic(Vector2 *V, fpreal t)
{
    fpreal	a, b, c, d;
    fpreal	y0, y1, m0, m1;
    fpreal	x, ml, mh;
    fpreal	xl, xh, yh, yl;
    Vector2	vec;

    xl = V[0].x();
    xh = V[3].x();

    yl = V[0].y();
    yh = V[3].y();

    ml = (V[1].y() - V[0].y()) / (V[1].x() - V[0].x());
    mh = (V[3].y() - V[2].y()) / (V[3].x() - V[2].x());

    //    m0 = ml * (xh-xl)/(yh-yl);
    //    m1 = mh * (xh-xl)/(yh-yl);

    m0 = ml * (xh-xl);
    m1 = mh * (xh-xl);

    y0 = yl;
    y1 = yh;

    a = m0 + m1 + 2*(y0 - y1);
    b = 3*(y1-y0) - m1 - 2*m0;
    c = m0;
    d = y1;

    x = (t - xl) / (xh - xl);

    vec.assign(t, a*x*x*x + b*x*x + c*x + d  - (yh-yl));

    return vec;
}


/*
 * computeCenterTangent :
 * Approximate unit tangents at "center" of digitized curve
 */


template <typename T>
typename UT_FitCubicT<T>::Vector2 
UT_FitCubicT<T>::computeCenterTangent(Vector2 *d, int center)
{
    Vector2	tHatCenter;
    int		i1, i2;

    i1 = center - 1;
    i2 = center + 1;

    if (i1 < 0)
	i1 = myClosed ? (myNumPoints - 1) : 0;

    if (i2 >= myNumPoints)
	i2 = myClosed ? 0 : (myNumPoints - 1);

    tHatCenter = d[i1] - d[i2];
    tHatCenter.normalize();

    return tHatCenter;
}


/*
 *  chordLengthParameterize :
 *     Assign parameter values to digitized points 
 *     using relative distances between points.
 */

template <typename T>
T *
UT_FitCubicT<T>::chordLengthParameterize(Vector2 *d, int first, int last)
{
    int		 i;      
    T		*u;	/*  Parameterization            */
    Vector2	 tempv;

    u = new T[last-first+1];

    u[0] = 0.0;

    for (i = first+1; i <= last; i++) 
    {
	tempv = d[i] - d[i-1];
	u[i-first] = u[i-first-1] + tempv.length();
    }

    for (i = first+1; i <= last; i++) 
	u[i-first] = u[i-first] / u[last-first];

    return(u);
}



/*
 *  computeError :
 *     Find the maximum squared distance of digitized points
 *     to fitted curve.
 */

template <typename T>
fpreal 
UT_FitCubicT<T>::computeBezierError(Vector2 *d, int first, int last, 
			  CubicCurve rcurve, T *u, int *splitPoint)
{
    int			 i;
    fpreal		 maxDist;	/*  Maximum error               */
    fpreal		 dist;		/*  Current error               */
    Vector2		 P;		/*  Point on curve              */
    Vector2		 v;		/*  Vector from point to curve  */

    maxDist = 0.0;
    *splitPoint = int((first+last) * 0.5);

    for (i = first + 1; i < last; i++) 
    {
	P = calcBezier3(rcurve, u[i-first]);
	v = P - d[i];
	dist = v.length2();
	if (dist >= maxDist) 
	{
	    maxDist = dist;
	    *splitPoint = i;
	}
    }

    return maxDist;
}

template <typename T>
fpreal 
UT_FitCubicT<T>::computeCubicError(Vector2 *d, int first, int last, 
			  CubicCurve rcurve, int *splitPoint,
			  bool preserveExtrema)
{
    int			 i;
    fpreal		 maxDist;	/*  Maximum error               */
    fpreal		 dist;		/*  Current error               */
    Vector2		 P;		/*  Point on curve              */

    //  Local min/max variables
    int			 iLocal = first;
    fpreal		 localVal = d[first].y();
    int			 typeLocal = d[first+1].y() > localVal ? +1 :
				    (d[first+1].y() < localVal ? -1 : 0);

    maxDist = 0.0;
    *splitPoint = int((first+last) * 0.5);

    for (i = first + 1; i < last; i++) 
    {
	P = calcCubic(rcurve, d[i].x());
	dist = SYSpow(P.y() - d[i].y(), T(2.0));
	if (dist >= maxDist) 
	{
	    maxDist = dist;
	    *splitPoint = i;
	}

	if (preserveExtrema)
	{
	    if ((d[i].y() < localVal && typeLocal == -1)
		|| (d[i].y() > localVal && typeLocal == 1)
		|| (d[i].y() == localVal && typeLocal == 0))
	    {
		localVal = d[i].y();
		iLocal = i;
	    }
	    else
	    {
		*splitPoint = iLocal;
		return myError2;
	    }
	}
    }

    return maxDist;
}


/*
 *  computeLineError :
 *     Find the maximum squared distance of digitized points
 *     to fitted line.
 */

template <typename T>
fpreal 
UT_FitCubicT<T>::computeLineError(Vector2 *d, int first, int last, 
			      CubicCurve rcurve, T *u, int *maxi)
{
    int			 i;
    fpreal		 maxDist;	/*  Maximum error              */
    fpreal		 dist;		/*  Current error              */
    Vector2		 P;		/*  Point on line              */
    Vector2		 v;		/*  Vector from point to line  */

    maxDist = 0.0;
    if (maxi)
	*maxi = first;

    for (i = first + 1; i < last; i++) 
    {
	P  = rcurve[0]*(1-u[i-first]);
	P += rcurve[3]*u[i-first];
	v = P - d[i];
	dist = v.length2();
	if (dist >= maxDist) 
	{
	    maxDist = dist;
	    if (maxi)
		*maxi = i;
	}
    }
    return (maxDist);
}

#endif // __UT_FitCubicImpl__
