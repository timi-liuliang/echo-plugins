/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FitCubic (C++)
 *
 * COMMENTS:	Given a group within a detail, fit curves to
 *		each polygon.
 *
 */

#ifndef __UT_FitCubic_H__
#define __UT_FitCubic_H__


#include "UT_API.h"
#include "UT_Vector2.h"

class UT_Interrupt;

enum utFitType
{
    UT_FIT_BEZIER	= 0,
    UT_FIT_CUBIC	= 1

};

enum utCurveType
{
    UT_FIT_CURVES	= 0,
    UT_FIT_POLYS	= 1,
    UT_FIT_BOTH		= 2
};

template <typename T>
class UT_FitCubicT
{
public:

     UT_FitCubicT();
    ~UT_FitCubicT();

    typedef UT_Vector2T<T>  Vector2;
    typedef Vector2	    CubicCurve[4];

    // segment, last point will always
    // match beginning of next segment
    struct Span
    {
	Vector2	point[3];
	int	isLinear;
	Span *	next;
    };


    int	 	 fitCurve(Vector2 *d, int nPts, int closed, fpreal error2,
							bool preserveExtrema);

    const Span	*getFirstSpan(void) const
			 { return myHead; }

    int		 numSpans(void) const
		 { return myNSpans; }

    int		 containsCurve(void)
		 { return myContainsCurve; }

    void	 setCurveType(utCurveType type)
		 { myCurveType = type; }

    void	 setType(utFitType type)
		 { myFitType = type; }

    static Vector2	 calcCubic(Vector2 *V, fpreal t);

private:

    int		 myContainsCurve;
    fpreal	 myError2;
    Span	*myHead;

    void	 destroySolution(void);


    Span	*appendCurve(CubicCurve fcurve, int islinear);


    int		 fitLine(Vector2 *d, int first, int last, 
			 Span **head, Span **tail,
			 int recurse);

    int		 fitCubic(Vector2 *d, int f, int l,Vector2 tHat1,
			  Vector2 tHat2, 
			  Span **head, Span **tail,
			  bool preserveExtrema, UT_Interrupt *boss);

    Vector2	 computeCenterTangent(Vector2 *d, int center);

    T		*reparameterize(Vector2 *d, int first, int last, 
				T *u, CubicCurve fcurve);

    fpreal	 newtonRaphsonRootFind(CubicCurve Q, Vector2 P, fpreal u);

    Vector2	 calcBezier2(Vector2 *V, fpreal t);
    Vector2	 calcBezier3(Vector2 *V, fpreal t);


    fpreal	 computeBezierError(Vector2 *d, int first, int last, 
			CubicCurve fcurve, T *u, int *splitPoint);

    fpreal	 computeCubicError(Vector2 *d, int first, int last, 
				   CubicCurve fcurve, int *splitPoint,
				   bool preserveExtrema);

    fpreal	 computeLineError(Vector2 *d, int first, int last, 
			CubicCurve fcurve, T *u, int *splitPoint);

    T		*chordLengthParameterize(Vector2 *d,int f,int l);

    void	 generateBezier(Vector2 *d, int first, int last, 
			        T *uPrime, Vector2 tHat1, 
			        Vector2 tHat2, CubicCurve fcurve);

    void	 simpleCurve(Vector2 *d, int first, int last,
			     Vector2 tHat1, Vector2 tHat2,
			     CubicCurve rcurve);

    void	 findCorner(Vector2 *d, int first, int last, 
			    int *splitPoint); 

    int		 isSeamCorner(Vector2 *d, int last);


    // inline multipliers

    fpreal	 BEZ_MULT_0(fpreal u)
		 {
		     fpreal tmp = 1.0F - u;
		     return (tmp * tmp * tmp);
		 }

    fpreal	 BEZ_MULT_1(fpreal u)
		 {
		     fpreal tmp = 1.0F - u;
		     return (3.0F * u * (tmp * tmp));
		 }

    fpreal	 BEZ_MULT_2(fpreal u)
		 {
		     fpreal tmp = 1.0F - u;
		     return (3.0F * u * u * tmp);
		 }

    fpreal	 BEZ_MULT_3(fpreal u)
    		 {
		     return (u * u * u);

		 }

    int		 myNumPoints;
    int		 myClosed;
    int		 myNSpans;

    utFitType	 myFitType;
    utCurveType	 myCurveType;
};

#include "UT_FitCubicImpl.h"

typedef UT_FitCubicT<fpreal>	UT_FitCubicR;
typedef UT_FitCubicT<fpreal32>	UT_FitCubicF;
typedef UT_FitCubicT<fpreal64>	UT_FitCubicD;
typedef UT_FitCubicT<float>	UT_FitCubic;    // deprecated

#endif
