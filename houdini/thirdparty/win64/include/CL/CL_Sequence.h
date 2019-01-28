/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Sequence.h ( Clip Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CL_Sequence__
#define __CL_Sequence__

#include "CL_API.h"
#include "CL_Track.h"
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>

enum CL_BlendFunction { 
    BLEND_LINEAR=0,
    BLEND_EASE_IN,
    BLEND_EASE_OUT,
    BLEND_COSINE,
    BLEND_CUBIC, 
    BLEND_ADD 
};

#define BLEND_PRESERVE		0
#define BLEND_OVERLAP		1
#define BLEND_INSERT		2

class UT_XformOrder;

class CL_API CL_Sequence 
{

public:

     CL_Sequence();
    ~CL_Sequence();

    static int		usesBias(CL_BlendFunction func);

    static int		sequence(fpreal *&result, CL_TrackListC &source, 
				 int blend_region, fpreal blend_bias,
				 fpreal step_factor, int mirror,
				 int blend_type, int iscycle,
				 fpreal cycle, CL_BlendFunction func,
				 fpreal partial_first = 1.0,
				 fpreal partial_last  = 1.0,
				 int steady_segment = 0,
				 int *steady_index = 0);

    static int		quatSequence(fpreal *&rx,fpreal *&ry,fpreal *&rz,
				     CL_TrackListC &sourcex,
				     CL_TrackListC &sourcey,
				     CL_TrackListC &sourcez,
				     int blend_region,fpreal blend_bias,
				     int blend_type,
				     fpreal stepx,fpreal stepy,fpreal stepz,
				     UT_XformOrder order,
				     CL_BlendFunction blend_function);
    
    static int		transSequence(fpreal *&tx,fpreal *&ty, fpreal *&tz,
				      CL_TrackListC &sourcex,
				      CL_TrackListC &sourcey,
				      CL_TrackListC &sourcez,
				      int blend_region,
				      int blend_type);
				      

    static void		getBlendValues(int nsamples,fpreal *data,fpreal bias,
				       CL_BlendFunction func);

    static void		getCubicParms(fpreal off1,fpreal val1,fpreal slope1,
				      fpreal off2,fpreal val2,fpreal slope2, 
				      UT_Vector4R &result);

    static void		getCubic(int nsamples, fpreal *data, 
	    			 fpreal off1,fpreal val1, fpreal slope1,
				 fpreal off2,fpreal val2, fpreal slope2);
    
    static void		blendQuaternion(fpreal *rx,fpreal *ry,fpreal *rz,
					fpreal rx1,fpreal ry1,fpreal rz1,
					fpreal rx2,fpreal ry2,fpreal rz2,
					fpreal ox, fpreal oy, fpreal oz,
					int size, fpreal *blend, fpreal *base,
					const UT_XformOrder &order,
					const UT_XformOrder &order2);

    static fpreal	blendTranslates(fpreal *trans,
					fpreal tp1,fpreal tp2, fpreal tp3,
					fpreal tn1,fpreal tn2, fpreal tn3,
					int size);

};

#endif

