/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Node.h
 *
 * COMMENTS:
 *	Defines a transformation class, which allows transforms to be
 *	collapsed into 1 transform.
 */
#ifndef COP2_TRANSFORM_PARMS_H
#define COP2_TRANSFORM_PARMS_H

#include "COP2_API.h"
#include <UT/UT_XformOrder.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_FilterType.h>
#include <RU/RU_FilterArea.h>

class COP2_API COP2_TransformParms
{
public:
    COP2_TransformParms(float tx = 0.0f, float ty = 0.0f, float rot = 0.0f,
			float sx = 1.0f, float sy = 1.0f,
			float px = 0.0f, float py = 0.0f,
			UT_XformOrder::rstOrder order  = UT_XformOrder::TRS,
			UT_FilterType		filter = UT_FILTER_BOX,
			float			filtersize = 1.0f,
			RU_FILTER_WRAP		wrap   = RU_FILTER_BORDER)
	: myRot(rot),
	  mySX(sx),mySY(sy),
	  myTX(tx),myTY(ty),
	  myPX(px),myPY(py),
	  myFilterSizeX(filtersize),	myFilterSizeY(filtersize),
	  myFilterX(filter),		myFilterY(filter),
	  myOrder(order),
	  myWrapX(wrap),		myWrapY(wrap),
	  myCombinedTransform(0),
	  myConstant(false), myUseBlur(false), myBlur(0.0f),
	  myBias(0.0f), mySegments(8) {}

		 COP2_TransformParms(const COP2_TransformParms &parms);
		~COP2_TransformParms();

    bool	 isCompatibleWith(const COP2_TransformParms &parms)
	{ return (myFilterX == parms.myFilterX &&
		  myFilterY == parms.myFilterY &&
		  myFilterSizeX == parms.myFilterSizeX &&
		  myFilterSizeY == parms.myFilterSizeY &&
		  myWrapX == parms.myWrapX &&
		  myWrapY == parms.myWrapY &&
		  (myConstant || parms.myConstant ||
		   (myUseBlur == parms.myUseBlur &&
		    (!myUseBlur || (myBlur == parms.myBlur &&
				    myBias == parms.myBias &&
				    mySegments == parms.mySegments))))); } 

    bool	collapseTransform(const COP2_TransformParms &parms);

    void	convertToMatrix();

    // Return the matrix representing our transform parameters.
    void	getMatrix(UT_Matrix4 &) const;

    // Return the inverse of the matrix representing our transform parameters
    void	getInverseMatrix(UT_Matrix4 &mat) const;

    // public Data
    float		myRot;
    float		mySX, mySY;
    float		myTX, myTY;
    float		myPX, myPY;
    float		myFilterSizeX, myFilterSizeY;
    UT_FilterType	myFilterX, myFilterY;
    UT_XformOrder	myOrder;
    RU_FILTER_WRAP	myWrapX, myWrapY;
    UT_Matrix4	       *myCombinedTransform;

    bool		myConstant;
    bool		myUseBlur;
    float		myBlur;
    float		myBias;
    int			mySegments;
};

#endif
