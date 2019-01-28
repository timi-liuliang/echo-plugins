/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	Calculates the volume inside or on the surface of a 2 or 3D object.
 *
 */

#ifndef __GU_Volume_H__
#define __GU_Volume_H__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>

class GU_Detail;

class GU_API GU_Volume
{
public:
    enum
    {
	HOLLOW_OBJECT,
	SOLID_OBJECT
    };

    		 GU_Volume(GU_Detail *geo, int divs,
			   int type = SOLID_OBJECT);
    virtual	~GU_Volume();

    float	 getVolume();
    UT_Vector3	 getCenterOfMass();
    float	 getMomentOfInertia(const UT_Vector3 &about,
				    const UT_Vector3 &axis,
				    float mass = 1.0F);

    UT_Vector3	 getResistanceTorque(const UT_Vector3 &rotvel,
				     float resistance, int newtonmodel,
				     float scale);
    
    void	 DEBUGgetPoints(float *&pnts, int &num);

    UT_BoundingBox &getBoundingBox() { return myBounds; } 
private:
    void	 voxelize();
    void	 voxelize2D();
    void	 voxelize3D();

    void	 determineEdges();

    void	 scanlineIntersect(const UT_Vector3 &o, const UT_Vector3 &d,
				   int &numsections, float *pos, int *orient);
    
private:
    GU_Detail	  *myGeometry;
    UT_BoundingBox myBounds;
    int		   myDivisions;
    int		   myObjectType;
    int		   myIsVoxelized;
    int		   myEdgesDetermined;
    unsigned char *myVoxels;

    // cached stuff...
    int		   myIsVolumeCalc;
    float	   myVolume;
    int		   myIsCOMCalc;
    UT_Vector3	   myCOM;	   
};

#endif
