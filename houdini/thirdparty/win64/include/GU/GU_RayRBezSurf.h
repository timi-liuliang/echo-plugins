/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	RayIntersect objects handling surfaces.
 *
 */

#ifndef __GU_RayRBezSurf_h__
#define __GU_RayRBezSurf_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4Array.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_Matrix.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_BoundingRect.h>

class GEO_Primitive;
class GU_RayInfo;
class GU_MinInfo;
class GU_RayRBezCurve;
class GU_RayRBezSurf;
class GU_IsectCurveSet;
class GU_IsectCurve;


//
// The actual functional unit of hulls, a single bezier patch.
//
class GU_API GU_RayRBezSurf
{
public:
     GU_RayRBezSurf();
    ~GU_RayRBezSurf();
    void	init(int uOrder, int vOrder, 
		     float uStart, float uLength,
		     float vStart, float vLength, int isRational);

    void	buildBBox();

    // Finds isection of bbox with specified ray
    int		intersectBBox(const UT_Vector3 &org, const UT_Vector3 &dir,
			    float tmax, float tol, float *t);

    // Finds intersection of this surface with ray/primitive.
    int		intersect(const UT_Vector3 &org,
			  const UT_Vector3 &dir,
			  GU_RayInfo &hitInfo, int depth,
			  int noBBoxTest = 0 );
    int		intersectQR(const UT_Vector3 &org,
			  const UT_Vector3 &dir,
			  GU_RayInfo &hitInfo, int depth,
			  int noBBoxTest = 0 );
    int		intersectAP(const UT_Vector3 &org,
			  const UT_Vector3 &dir,
			  GU_RayInfo &hitInfo);
    int		intersect(GU_RayRBezSurf &surf, GU_RayInfo &hitinfo);

    // Finds minimum point of this surface with specifed point/primitive.
    int		minimum(const UT_Vector3 &p,
			  GU_MinInfo &mininfo, int depth,
			  int noBBoxTest = 0 );
    int		minimum(GU_RayRBezSurf &surf, GU_MinInfo &mininfo, 
			int depth, int noBBoxTest = 0);

    // Finds closest point on the surface to p, returning in u, v.
    // d2 is the resulting distance squared & tol is the maximum allowed.
    int		pointInvert(const UT_Vector3 &p, float &u, float &v,
			    float &d2, float tol);
    // Cut the surface between the specified points, performing operation
    // in place
    void	carveU(float ustart, float uend);
    void	carveV(float vstart, float vend);
    // Split myself, locking to ensure no double splits
    void	splitMyself(int depth);
    // Split the surface evenly into 4 quadrants.
    void	split(GU_RayRBezSurf *topLeft, 
		      GU_RayRBezSurf *bottomLeft,
		      GU_RayRBezSurf *bottomRight, 
		      GU_RayRBezSurf *topRight) const;
    void	evaluate(float u, float v, UT_Vector3 &pos) const;
    void	evaluateHomogeneous(float u, float v, UT_Vector4 &pos, 
			 UT_Vector4 &derU, UT_Vector4 &derV) const;
    void	evaluate(float u, float v, UT_Vector3 &pos, 
			 UT_Vector3 &derU, UT_Vector3 &derV,
			 UT_Vector3 &derUV,
			 UT_Vector3 &derUU, UT_Vector3 &derVV) const;
    // patches will be initialized with topleft, bottomleft, bottomright, topright
    // sortvalue should be 4 floats already having their sort values
    // sortvalue will end up with the sort value for the corresponding
    // patches[]
    void	sortSubPatches(GU_RayRBezSurf **patches, float *sortvalue) const;
    void	guessIntersection(const UT_Vector3 &org,
				  const UT_Vector3 &dir,
				  float &t, float &u, float &v) const;
    void	guessIntersection(const UT_Vector3 &org,
				  float &u, float &v) const;

    void	buildImplicit();

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCV.getMemoryUsage(false);
        mem += mySplitLock.getMemoryUsage(false);
        mem += myXCoeff.getMemoryUsage(false);
        mem += myYCoeff.getMemoryUsage(false);
        mem += myZCoeff.getMemoryUsage(false);
        mem += myWCoeff.getMemoryUsage(false);
        if (myTopLeft)
        {
            mem += myTopLeft->getMemoryUsage(true);
            mem += myBottomLeft->getMemoryUsage(true);
            mem += myTopRight->getMemoryUsage(true);
            mem += myBottomRight->getMemoryUsage(true);
        }
        return mem;
    }

private:
    void	buildPowerCoeffs(UT_MatrixF &A, int index) const;

    // The logic of all this is surf is intersected with this.  Thus,
    // in the point array the x, y are this' u,v coordinates and
    // z, w are surf's u,v coords.  Also, domain is in terms of
    // surf's space, not this.
    int		domainDecomposition(GU_IsectCurveSet &curveset,
				    GU_RayRBezSurf &surf, 
				    const UT_BoundingRect &domain, 
				    UT_Vector4Array &points,
				    float intersecttol = 1e-6F, 
				    float domaintol = 1e-2F, 
				    float steprate = 1e-2F, int dividedir=0);
    // Returns 0 if successfuly traces from start to finish.  -1 if
    // leaves source or base surface without converging, and -2 if
    // hits a cusp which prevents further travel.  Whatever portion
    // of the curve is generated can be found in isectcurve.
    // If trace fails, one should try again with opposite sign for
    // direction.
    int		traceIsectCurve(GU_IsectCurve &isectcurve, 
				GU_RayRBezSurf &surf, 
				const UT_BoundingRect &domain,
				const UT_Vector4 &start, const UT_Vector4 &end,
				float tol = 1e-6F, float domaintol = 1e-2F,
				float steprate = 1.0f/100.0f,
				int canreverse = 1, int direction = 1);
				
    // Uses inverse power iterations to reconverge back to the isect
    // curve from u, v by adjusting the v position.
    // Return -1 if converging failed (Use smaller step size!)
    int		polishInV(GU_RayRBezSurf &surf, float u, float v, 
			  float &vresult, float &invu, float &invv,
			  float tol = 1e-4F, int maxiter = 30);
    int		polishInU(GU_RayRBezSurf &surf, float u, float v, 
			  float &uresult, float &invu, float &invv,
			  float tol = 1e-4F, int maxiter = 30);
    int		polishCurve(GU_RayRBezCurve &curve, float &t, 
			  float &invu, float &invv, float tol,
			    int maxiter);
    int		polishCurveSlow(GU_RayRBezCurve &curve, float &t, float tol,
			    int maxiter);

    void	extractUIsoparm(GU_RayRBezCurve &curve, float u) const;
    void	extractVIsoparm(GU_RayRBezCurve &curve, float v) const;

    void	isectEdges(GU_RayRBezSurf &surf, 
			UT_Vector4Array &pointlist, 
			float tol = 1e-6F, float ttol = 1e-2F);
    void	isectIsoU(GU_RayRBezSurf &surf, 
			float u, float vstart, float vend,
			UT_Vector4Array &pointlist,
			float tol = 1e-6F, float ttol = 1e-2F, int useAP=1);
    void	isectIsoV(GU_RayRBezSurf &surf, 
			float v, float ustart, float uend,
			UT_Vector4Array &pointlist, 
			float tol = 1e-6F, float ttol = 1e-2F, int useAP=1);

public:
    UT_BoundingBox	 	 myBBox;
    UT_RefMatrix<UT_Vector4>	 myCV;
    float			 myUStart, myULength;
    float			 myVStart, myVLength;
    float			 sortValue;
    int				 myRational;
    GU_RayRBezSurf		*myTopLeft, *myBottomLeft;
    GU_RayRBezSurf		*myTopRight, *myBottomRight;

    UT_Lock			 mySplitLock;
    volatile bool		 myHasSplit;

    int				 myImplicitFlag;
    // Is eigenvalues recoverable?
    int				 myEigenVectorCorrect;
    // Coeff arrays are:
    // [a ba b2a ... b(2n-1)a] coeff [y       ]
    //                               [xy      ]
    //	                             [...     ]
    //                               [x(2m-1)y]
    // where a = [1 a ... a(m-1)] and
    // y = [1 y ... y(n-1)]
    // and m = udeg, n = vdeg
    // This is only true if they are still the full size matrices!
    // If they have been reduced to eliminate base points, this order
    // is changed in an effectively uninvertable manner.
    UT_MatrixF			 myXCoeff, myYCoeff, myZCoeff, myWCoeff;
};


#endif

