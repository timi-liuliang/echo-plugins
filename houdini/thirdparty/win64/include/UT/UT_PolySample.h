/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PolySample.h ( UT Library, C++)
 *
 * COMMENTS: This code comes from RAY_Quad2d.h, and provies
 * 	     methods to sample two dimensional polygons.  Ie, given
 *	     a point, find the "u v" coordinates of that point in
 *	     the polygon.
 *	     It currently only handles quads and tris.  Tris give
 *	     barycentric coordinates, quads give bilinear interpolated
 *	     coordinates.
 */

#ifndef __UT_PolySample__
#define __UT_PolySample__

#include "UT_API.h"
#include "UT_Vector2.h"

class UT_SampleQuad2D;

typedef int	(UT_SampleQuad2D::*UT_QuadSampleFunc)(double x, double y,
					      float &u, float &v);

class UT_API UT_SampleSimpleTri2D {
public:
    float	u0, u1;
    float	v0, v1;
    float	Nxi;

    inline void	init(float x0[2], float x1[2], float x2[2])
		{
		    u0 = x1[0] - x0[0]; v0 = x1[1] - x0[1];
		    u1 = x2[0] - x0[0]; v1 = x2[1] - x0[1];
		    Nxi = 1.0F/(u0*v1 - u1*v0);
		}
    inline void	setVertices(fpreal x0, fpreal y0, fpreal x1, fpreal y1,
			    fpreal x2, fpreal y2)
		{
		    u0 = x1 - x0; v0 = y1 - y0;
		    u1 = x2 - x0; v1 = y2 - y0;
		    Nxi = 1.0F / (u0*v1 - u1*v0);
		}

    inline void initD0(float x0[2], float x2[2], fpreal dx, fpreal dy)
		{
		    u0 = dx; v0 = dy;
		    u1 = x2[0] - x0[0]; v1 = x2[1] - x0[1];
		    Nxi = 1/(u0*v1 - u1*v0);
		}

    inline int	sample(fpreal sx, fpreal sy, fpreal x0, fpreal y0, float &u, float &v, fpreal tol=0.0)
		{
		    sx -= x0; sy -= y0;
		    fpreal tu = (sx*v1 - sy*u1) * Nxi;
		    if (tu >= -tol && tu <= 1+tol) {
			fpreal tv = (sy*u0 - sx*v0) * Nxi;
			if (tv >= -tol && tu+tv <= 1+tol) {
			    u = tu;
			    v = tv;
			    return 1;
			}
		    }
		    return 0;
		 }
};

class UT_API UT_SampleTri2D {
public:
    int		isBackface() const	{ return myBackface; }
    void	setVertices(fpreal x0, fpreal y0, fpreal x1, fpreal y1,
			    fpreal x2, fpreal y2)
		{
		    fpreal	dx, dy, z;
		    myX[0] = x0;
		    myX[1] = y0;
		    myTri.setVertices(x0, y0, x1, y1, x2, y2);
		    dx = myTri.v1; dy = -myTri.u1;
		    z = -(dx*x0 + dy*y0);
		    myBackface = dx*x1 + dy*y1 + z > 0;
		}
    int		sample(fpreal x, fpreal y, float &u, float &v, fpreal tol=0.0)
		{
		    if (myTri.sample(x, y, myX[0], myX[1], u, v, tol))
		    {
			return 1;
		    }
		    return 0;
		}

private:
    UT_SampleSimpleTri2D	 myTri;
    float		 myX[2];
    int			 myBackface;
};

class UT_API UT_SampleQuad2D {
public:
    // Static init can be called once before sampling.  Please set up the point
    // positions before this is done...
    // If the polygon is large (in screen space), please set accurate to 1
    int		setVertices(fpreal x0, fpreal y0, fpreal x1, fpreal y1,
			    fpreal x2, fpreal y2, fpreal x3, fpreal y3)
		{
		    myXY[0][0] = x0; myXY[0][1] = y0;
		    myXY[1][0] = x1; myXY[1][1] = y1;
		    myXY[2][0] = x2; myXY[2][1] = y2;
		    myXY[3][0] = x3; myXY[3][1] = y3;
		    return init();
		}

    int		 init();

    int		 sample(double x, double y, float &u, float &v)
		 {
		     return (this->*mySampler)(x, y, u, v);
		 }

    // Call the sampler after the init.  See above for calling syntax
    int		 isBackface() { return myBackface; }

private:
    // Trapezoidal sampling
    int		 sampleTrap1(double x, double y, float &u, float &v);
    int		 sampleTrap2(double x, double y, float &u, float &v);

    // Convex quadrilateral
    int		 sampleQuad(double x, double y, float &u, float &v);
    // Concave quadrilateral
    int		 sampleConcave(double x, double y, float &u, float &v);
    // Bowtie quad
    int		 sampleBowtie(double x, double y, float &u, float &v);
    int		 sampleGeneral(double x, double y, float &u, float &v);

    // Otherwise, only the following is needed
    float		 myXY[4][2];

    UT_QuadSampleFunc	 mySampler;
    UT_SampleSimpleTri2D myFirst;
    UT_SampleSimpleTri2D mySecond;
    UT_Vector2		 myAB, myBC, myCD, myAD, myAE;
    float		 myU, myV;
    int			 myBackface;
};

// This class actually performs bilinear sampling, which UT_SampleQuad2D
// does not seem to do.
class UT_API UT_BilinearSampleQuad2D {
public:
    // Static init can be called once before sampling.  Please set up the point
    // positions before this is done...
    void	setVertices(fpreal x0, fpreal y0, fpreal x1, fpreal y1,
			    fpreal x2, fpreal y2, fpreal x3, fpreal y3);

    // NOTE: The u & v here are returned using the Mantra style winding
    // of the polygon.  This is opposite of Houdini's winding, so if
    // this is to be used for evaluateInteriorPoint, one should
    // reverse the u & v values (or swap x1 & x2)
    int		sample(fpreal x, fpreal y, float &u, float &v);


private:
    int			 myBackface;
    int			 myFlipped;
    int			 myGeneral;
    float		 mySBx, mySDx;

    float		 myAx, myAy, myBx, myBy, myCx, myCy, myDx, myDy;
    float		 myK, myIK, myL, myM;
};


#endif

