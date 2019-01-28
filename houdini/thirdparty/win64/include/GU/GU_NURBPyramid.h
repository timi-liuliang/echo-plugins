/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	NURB Pyramid classes.	
 *
 */

#ifndef __GU_NURBPyramid_h__
#define __GU_NURBPyramid_h__

#include "GU_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_Interval.h>
#include <GA/GA_Basis.h>
#include <GA/GA_PwHandle.h>
#include <GEO/GEO_Hull.h>
#include <GEO/GEO_Face.h>

template <int Dir> // u = 0, v = 1
class GU_API GU_CVMesh
{
public:
    GU_CVMesh()
	: myCV(0)
	, myHull(0)
	, myFace(0) {}
    GU_CVMesh(UT_RefMatrix<UT_Vector4> *cv)
	: myCV(cv)
	, myHull(0)
	, myFace(0) {}
    GU_CVMesh(const GEO_Hull *cv, const GA_PwHandleRO &h)
	: myHull(cv)
	, myCV(0)
	, myFace(0)
	, myHandle(h) {}
    GU_CVMesh(const GEO_Face *cv, const GA_PwHandleRO &h)
	: myFace(cv)
	, myCV(0)
	, myHull(0)
	, myHandle(h) {}

    unsigned int	 rows() const
			 {
			     return Dir ? getCols() : getRows();
			 }
    unsigned int	 cols() const
			 {
			     return Dir ? getRows() : getCols();
			 }
    const UT_Vector4	 getPos(int i, int j) const
			 { 
			     return Dir ? get(j, i) : get(i, j);
			 }
    void		 setPos(int i, int j, const UT_Vector4 &pos)
			 { 
			     if (Dir)
				 set(j, i, pos);
			     else
				 set(i, j, pos);
			 }
    void		 homogenize(int i, int j)
			 {
			     UT_Vector4	 pos = getPos(i, j);
			     pos.homogenize();
			     setPos(i, j, pos);
			 }

private:
    const UT_Vector4	 get(int i, int j) const
			 { 
			     if (myCV)
				 return (*myCV)(i, j);
			     if (myHull)
				 return myHandle.get(myHull->getPointOffset(i, j));
			     return myHandle.get(myFace->getPointOffset(j));
			 }
    void		 set(int i, int j, const UT_Vector4 &pos)
			 {
			     if (myCV)
				 (*myCV)(i, j) = pos;
			     else
				 UT_ASSERT(0 && "Read-only mesh");
			 }
    int			 getRows() const
			 {
			     return myCV ? myCV->rows() :
				 (myHull ? myHull->getNumRows() : 1);
			 }
    int			 getCols() const
			 {
			     return myCV ? myCV->cols() :
				 (myHull ? myHull->getNumCols() :
				  myFace->getVertexCount());
			 }

private:
    UT_RefMatrix<UT_Vector4>	*myCV;
    const GEO_Hull		*myHull;
    const GEO_Face		*myFace;
    GA_PwHandleRO		 myHandle;
};

template <int Dir> // u = 0, v = 1
class GU_API GU_NURBPyramid
{
public:
    GU_NURBPyramid()
    {
    }
    GU_NURBPyramid(const GA_Basis *basis,
		   UT_RefMatrix<UT_Vector4> *cv,
		   fpreal umin = 0, fpreal umax = -1)
		{
		    // cv is not modified
		    myCV = GU_CVMesh<Dir>(cv);
		    myUMin = umin;
		    myUMax = umax;
		    if (myUMin > myUMax)
			basis->getValidRange(myUMin, myUMax);
		}

    template <typename T> // GEO_Hull or GEO_Face
    GU_NURBPyramid(const GA_Basis *basis,
		   const T *cv, const GA_PwHandleRO &h,
		   fpreal umin = 0, fpreal umax = -1)
		{
		    // cv is not modified
		    myCV = GU_CVMesh<Dir>(cv, h);
		    myUMin = umin;
		    myUMax = umax;
		    if (myUMin > myUMax)
			basis->getValidRange(myUMin, myUMax);
		}

    template <typename T> // GEO_Hull or GEO_Face
    void init(const GA_Basis *basis,
	   const T *cv, const GA_PwHandleRO &h,
	   fpreal umin = 0, fpreal umax = -1)
    {
	// cv is not modified
	myCV = GU_CVMesh<Dir>(cv, h);
	myUMin = umin;
	myUMax = umax;
	if (myUMin > myUMax)
	    basis->getValidRange(myUMin, myUMax);
    }

    void	rewind(const GA_Basis *basis);
    bool	atEnd()	{ return SYSisEqual(myInterval.min, myUMax); }
    void	advance();

    // Return bezier hull data.  Vectors are homogenized, so to get
    // bounding information you will need to dehomogenize these positions.
    UT_RefMatrix<UT_Vector4>	&getResult()	{ return myResultData; }

    // Return the current parametric interval
    const UT_Interval		&getInterval()	{ return myInterval; }

private:
    void	refine(fpreal u, int R, int newIdx);

    int				 myOrd, myDeg;
    int				 myCVSize;
    GA_KnotVector		 myKnots;
    GU_CVMesh<Dir>		 myCV;
    fpreal			 myUMin, myUMax;

    GU_CVMesh<Dir>		 myPyramid;
    UT_RefMatrix<UT_Vector4>	 myPyramidData;
    GU_CVMesh<Dir>		 myResult;
    UT_RefMatrix<UT_Vector4>	 myResultData;

    // Iteration indices
    UT_Interval			 myInterval;	// Parametric interval
    int				 myK;		// Knot index
    int				 myCVIdx;	// CV index
};

#endif
