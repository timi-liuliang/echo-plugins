/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Muscle.h
 *
 * COMMENTS:	
 */

#ifndef __GU_Muscle_h__
#define __GU_Muscle_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GA/GA_Handle.h>

class OP_Context;
class GEO_PrimPoly;
class GU_PrimMetaball;
class GU_PrimSphere;

// Default values for group names
#define DEFAULT_CENTER_GROUP "center"

typedef fpreal gu_Cubic[4];

enum GU_MuscleType
{
    GU_MUSCLE_META = 0,
    GU_MUSCLE_SPHERE
};

//-----------------------------------------------------------------------------

class GU_API GU_MuscleParms
{
public:

    GU_Detail                   *gdp; 
    
    GU_Detail                   *myExtraGdp;

    // Parameters directly corresponding to those appearing in the UI:
    
    // Muscle Name
    UT_String                    name;

    // Muscle scale
    fpreal                       fullscalex, fullscaley, fullscalez;

    // Control positions
    fpreal                       pos2, pos3, pos4;

    // Control scales
    fpreal                       sx1, sx2, sx3, sx4, sx5;
    fpreal                       sy1, sy2, sy3, sy4, sy5;
    fpreal                       sz1, sz2, sz3, sz4, sz5;

    // Primitives per segment
    int                          primsPerSegment;
    
    // Muscle Type - Metaball Capture, Spheres Only
    GU_MuscleType                type;

    // Center Line parameters
    int                 	 basis;
    int				 sampledivs;
    int				 firstcv;
    int				 lastcv;
    float			 cvtension;
};

//-----------------------------------------------------------------------------

class GU_API GU_Muscle
{
public:

    GU_Muscle();
    ~GU_Muscle() {}

    bool                        build(GU_MuscleParms &parms, 
                                      const UT_Array<UT_Matrix4> 
                                      &inputXforms);

    GA_PointGroup               *getCenterPoints() { return myCenterPoints; }
    GA_PrimitiveGroup           *getPrimitiveGroup() { return myPrimitives; }
    int                          getNumPrimitives() { return myNumPrimitives; }
    
    bool                         getTranslates(
                                    UT_Array<UT_Matrix4> &translates);

    bool                         getRotates(UT_Array<UT_Matrix4> &rotates);

    const GEO_PrimPoly          *getCenterCurve();
    
private:

    // Computes the a set of cubics for the left hand side of the
    // profile curve.
    static void                 calcLeftCurve(gu_Cubic &curve1, 
                                              gu_Cubic &curve2,
	   		                      fpreal y1, fpreal y2, fpreal y3,
			                      fpreal x2, fpreal x3);

    // Computes the set of cubics for the right hand side of the
    // profile curve.
    static void                 calcRightCurve(gu_Cubic &curve1, 
                                               gu_Cubic &curve2,
                                               fpreal y3, fpreal y4, fpreal y5,
                                               fpreal x3, fpreal x4);

    // Evaluates a profile curve using the given cubics and division information.
    static inline fpreal        evalProfile(fpreal t,
					    const gu_Cubic &curve1,
					    const gu_Cubic &curve2,
					    const gu_Cubic &curve3,
					    const gu_Cubic &curve4,
					    fpreal x2, fpreal x3, fpreal x4);

    // This function takes the existing centre line and uses it to
    // construct a new centre line with points at the given positions
    // along the curve. It is needed to make point distribution uniform.
    GEO_PrimPoly	        *resampleCenterLine(const UT_FloatArray 
                                                   &positions);

    // Adds primitives depending on the muscle type
    void                         addPrimitives(int numPrimitives);

    // Sets up scale transforms using an array of scale values
    void			 setupScaleXforms(const UT_Vector3Array 
                                                  &scales);

    // Returns a list of muscle primitive positions (ie. u values 0 <= u <= 1)
    // along the center line as well as three dimensional scale values for
    // each muscle primitive.
    bool                         getPrimitiveInfo( UT_FloatArray &positions, 
                                                   UT_Vector3Array &scales,
                                                   bool controlPointsOnly=false);
    

    // Transforms each muscle primitive to place it along the
    // center line and orient it appropriately
    void                         transformPrimitives(GEO_PrimPoly *guide);

    // Builds the center line for this muscle.
    bool                         buildCenterLine(const UT_Array<UT_Matrix4> 
                                                 &inputXforms);

    void	                 CENTERGROUP(UT_String &str)
			         {
			             str = DEFAULT_CENTER_GROUP;
			         }            

    // A function to be called by subdivide point groups, used to create
    // polygons interpolating between our anchor points.
    void                         addPoly(GA_PointGroup *group);

    // Adds metaball primitives (and a group to store them) if necessary.
    void                         addMetaPrimitives(int numMeta);

    //Adds sphere primitives (and a group to store them) if necessary.
    void                         addSpherePrimitives(int numSpheres);

    void                         getPrimitiveTranslates(GEO_PrimPoly *guide,
                                     int numPrimitives,
                                     UT_Array<UT_Matrix4> &translates);

private:

    GU_MuscleParms              *myParms;

    // Stores the centre curve for this muscle
    GEO_PrimPoly	        *myCenterCurve;

    // Stores the centre curve points for this
    // muscle
    GA_PointGroup               *myCenterPoints;

    // Stores the primitives (spheres or metaballs)
    // currently used by this muscle
    GA_PrimitiveGroup	        *myPrimitives;

    // Stores the number of primitives we are currently
    // storing
    int			         myNumPrimitives;

    // Since we never duplicate any input sources,
    // we should be able to just store the normal
    // offset
    GA_RWHandleV3                myNormalAttrib;

    // Array used to cache the scale transforms for our metaballs
    // so that we don't have to evaluate them unless the profile
    // channels have actually changed
    UT_Array<UT_Matrix4>      myCachedScaleXforms;
   
    // Transforms that can be used by muscle handles
    // to position/scale/orient themselves
    UT_Array<UT_Matrix4>      mySavedRotates;
    UT_Array<UT_Matrix4>      mySavedTranslates;
};



#endif
