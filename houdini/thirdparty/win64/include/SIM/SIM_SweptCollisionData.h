/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SweptCollisionData_h__
#define __SIM_SweptCollisionData_h__

#include "SIM_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_StringArray.h>
#include <vector>

// Spatial coordinates for point (e.g., position or velocity)
struct SIM_API SIM_SpatialPoint
{
    SIM_SpatialPoint() 
    {
	myV = UT_Vector3D(0, 0, 0);
    }

    explicit SIM_SpatialPoint(const UT_Vector3D& v)
    {
        myV = v;
    }

    SIM_SpatialPoint(const SIM_SpatialPoint& s)
    {
        myV = s.myV;
    }

    SIM_SpatialPoint& operator=(const SIM_SpatialPoint& s)
    {
        myV = s.myV;
        return *this;
    }

    UT_Vector3D myV;
};

typedef SIM_SpatialPoint SIM_PositionPoint;
typedef std::vector<SIM_PositionPoint> SIM_PositionPoints;

typedef SIM_SpatialPoint SIM_VelocityPoint;
typedef std::vector<SIM_VelocityPoint> SIM_VelocityPoints;

// Triangle represented as a triple of point indices
struct SIM_API SIM_Triangle
{
    SIM_Triangle()
    {
        myPS[0] = -1;
        myPS[1] = -1;
        myPS[2] = -1;
    }

    int myPS[3];
};

// Connectivity structure consisting of triangles
struct SIM_API SIM_TriangleConnectivity
{
    std::vector<SIM_Triangle> myTriangles;    

    // GA_Index of the source primitive.  Note that there is often more
    // than one triangle per primitive!
    UT_IntArray			myPrimIdx;
    // Geometry id of source data
    UT_IntArray			myGeoId;

    // myGeoId can index into this list of paths.
    UT_StringArray		myGeoPath;
};

#endif

