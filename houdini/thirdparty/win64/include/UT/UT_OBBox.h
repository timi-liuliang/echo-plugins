/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OBBox.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_OBBox_h__
#define __UT_OBBox_h__

#include "UT_API.h"
#include "UT_Matrix3.h"
#include "UT_Vector3.h"
#include "UT_BoundingBox.h"
#include "UT_QuickHull.h"

template<typename T>
class UT_Array;

/// Arbitrarily Oriented Bounding (OBB)
template <typename T>
class UT_API UT_OBBoxT
{
public:
    UT_OBBoxT();
    UT_OBBoxT(const UT_Array<UT_Vector3T<T> > &points,
              int refinement = 6)
    {
        init = false;
        myCenter = 0;
        myRotation = 0;
        myRadii = 0;
        myBasis.identity();
        calcOBB(points, refinement);
    }

    /// Calculate the OBB for the set of points
    void    calcOBB(const UT_Array<UT_Vector3T<T> > &points,
		    int refinement = 6);
    /// Return the basis that defines the orientation of the OBB
    UT_Matrix3T<T>  getBasis() const {return myBasis;}
    /// Return the half radii(side length) of the OBB
    UT_Vector3T<T>  getRadii() const {return myRadii;}
    /// Return the rotation i radians
    UT_Vector3T<T>  getRotation() const {return myRotation;}
    /// Return the center of the OBB
    UT_Vector3T<T>  getCenter() const {return myCenter;}
    /// Return the tranform matrix of the OBB
    UT_Matrix4T<T>  getTransform() const;
    /// Return the non-oriented bounding box.
    UT_BoundingBoxT<T> getBBox() const {return myBB;}
    /// Return the minimum axis direction
    UT_Vector3T<T> getMinAxis() const
    {
        int imin = SYSargmin(myRadii.x(), myRadii.y(), myRadii.z());
        UT_Vector3T<T> dir = UT_Vector3T<T>(
            myBasis(0,imin),
            myBasis(1,imin),
            myBasis(2,imin)
        );
        return dir;
    }

protected:
    UT_BoundingBoxT<T>  myBB;
    UT_Matrix3T<T>	myBasis;
    UT_Vector3T<T>	myCenter;
    UT_Vector3T<T>	myRotation;
    UT_Vector3T<T>	myRadii;
    bool		init;
};

typedef UT_OBBoxT<fpreal>	UT_OBBoxR;
typedef UT_OBBoxT<fpreal32>	UT_OBBoxF;
typedef UT_OBBoxT<fpreal64>	UT_OBBoxD;
typedef UT_OBBoxT<float>	UT_OBBox;	// deprecated

#endif
