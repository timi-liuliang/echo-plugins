/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Utility Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GU_Sweep_h__
#define __GU_Sweep_h__

#include "GU_API.h"
#include "GU_Error.h"
#include "GU_Types.h"
#include <GA/GA_AttributeInstanceMatrix.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Matrix3.h>

typedef UT_Array<UT_Matrix3> utMatrixArray;

class UT_Interrupt;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GEO_PrimPoly;
class GU_Detail;

enum GU_SweepFixType
{
    GU_SWEEP_FIX_NONE	= 0,
    GU_SWEEP_FIX_ANGLE	= 1,
    GU_SWEEP_FIX_NOFLIP	= 2,
    GU_SWEEP_FIX_BOTH	= 3
};

enum GU_SweepSkinType
{
    GU_SWEEP_SKIN_OFF			= 0,
    GU_SWEEP_SKIN_ON			= 1,
    GU_SWEEP_SKIN_AUTO			= 2,
    GU_SWEEP_SKIN_PRESERVE_SHAPE_ON	= 3,
    GU_SWEEP_SKIN_PRESERVE_SHAPE_AUTO	= 4
};

typedef float	(*GU_SweepCallBack)(void *data, GA_Index pathindex,
			int vertexNumber, int numberOfVertices);

class GU_API GU_SweepParms
{
public:
    GU_SweepParms();
   ~GU_SweepParms() {}

    void			 reset();
    GU_ERROR			 doIt( GU_Detail *gdp );

    GU_Detail			*templateGdp;
    const GU_Detail		*paths;		// the backbone rails
    const GU_Detail		*refpaths;	// the orientation reference paths
    const GU_Detail		*xSection;      // the cross-section
    GU_CrossSectionType 	 cycleType;     // cycleType
    GU_SweepFixType		 fixType;	// fix type
    int				 skipCoin;	// skip coincident points
    int  			 aimAtRef;	// Aim at points reference path.
    int 			 useVertex;	// use vertex to attach to
    int				 vertexNumber;  // vertex to attach to 
    float 			 scale;         // scale
    float 			 twist;         // twist
    float 			 roll;          // roll
    int				 outputGroups;	// output sweep groups switch 
    GU_SweepSkinType		 skinType;	// convert output to meshes
    int				 outputPolys;	// Convert output mesh to poly
    char                   	*sweepGrpName;  // sweep groups prefix name
    const GA_PrimitiveGroup	*pathGroup;     // optional rail group
    const GA_PrimitiveGroup	*refGroup;      // optional ref. group
    const GA_PrimitiveGroup 	*xGroup;        // optional xsec group

    /// Set useInstanceMatrix to true and initialize instanceMatrix
    /// in order to use the backbone's
    /// N, up, trans, pscale, scale, rot, orient, pivot to determine
    /// how to transform cross-sections, instead of the scale, twist,
    /// and roll values above.
    /// @{
    bool                         useInstanceMatrix;
    GA_AttributeInstanceMatrix   instanceMatrix;
    /// @}
    GU_SweepCallBack		 getTwist;	// way of computing twist
    GU_SweepCallBack		 getScale;	//  and scale 
    GU_SweepCallBack		 getRoll;	//  and roll  during sweep
    void			*callbackData;

    /// Pattern for selecting which attributes to transfer from path
    /// to cross-sections.
    const char *myAttribPattern;

private:

    GU_ERROR			 sweepPaths(GU_Detail* dest,
				    const GA_PrimitiveGroup *tgroup,
				    bool use_template,
				    GA_Index &template_startpt,
				    UT_Interrupt *boss, int &stop);

    GU_ERROR			 sweep(const GEO_PrimPoly &path,
				    const GEO_PrimPoly *refpath,
				    const GA_PrimitiveGroup *tGroup,
				    GU_Detail &dest,
				    bool use_template, GA_Index &template_startpt,
				    UT_Interrupt *boss, int &stop);

    bool			 calculateRotations(
				    const GEO_PrimPoly &path,
				    const GEO_PrimPoly *refPoly,
				    GA_Size numPathVertices,
				    UT_Matrix3 &rotOrigin,
				    UT_Vector3 &tEnd,
				    float *&refRollArray,
				    bool *&sweepOk);

    utMatrixArray		 dihedralArray;
    utMatrixArray		 stretchArray;
    utMatrixArray		 refTiltArray;

    int				 angleFlag;
    int				 cycleAll;
    int				 cycleOne;
    int				 noFlip;
    int				 sweepGroupCount;

    GA_PrimitiveGroup 		*srcGroup;
    GA_PrimitiveGroup		*onePrimGroup;
    GA_PrimitiveGroup		*dest_skingroup;
    GA_PrimitiveGroup		*dest_delgroup;
};

#endif
