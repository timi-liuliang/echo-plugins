/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: CaptureWeightVisualize creates Cd values for capture weights.
 *
 */

#ifndef __SOP_CaptureWeightVisualize_h__
#define __SOP_CaptureWeightVisualize_h__

#include "SOP_API.h"
#include "SOP_CaptureLayerData.h"

#include <GU/GU_Detail.h>
#include <UT/UT_Color.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Array.h>
#include <OP/OP_Node.h>

class SOP_API SOP_CaptureWeightVisualize
{
public:
    static void	 getSingleBoneVisualization(const GU_Detail *gdp, int ptnum,
					    int idx, bool visnormalized,
					    const UT_ColorRamp &ramp,
					    UT_Vector3 *vis,
					    GEO_Detail::CaptureType t = 
						      GEO_Detail::CAPTURE_BONE);

    static void	 getMultiBoneVisualization(const GU_Detail *gdp, int ptnum,
			bool visnormalized,
			const UT_Array<const UT_Vector3 *> *colours,
			UT_Vector3 *vis,
			GEO_Detail::CaptureType t = GEO_Detail::CAPTURE_BONE);

    static void	 doSingleBoneVisualize(GU_Detail *gdp, int idx,
				       bool visnormalized,
				       const UT_ColorRamp &ramp,
				       GEO_Detail::CaptureType t = 
						      GEO_Detail::CAPTURE_BONE);

    static void  doMultiBoneVisualize(OP_Node *node, GU_Detail *gdp,
				      OP_Context &context, bool visnormalized,
				      GEO_Detail::CaptureType t = 
						      GEO_Detail::CAPTURE_BONE);

    static void	 getSingleBoneLayerVisualization(SOP_CaptureLayerData *layer,
				 const UT_IntArray &cregion_mapping,
				 int ptnum, int idx, bool visnormalized,
				 const UT_ColorRamp &ramp, UT_Vector3 *vis,
				 GEO_Detail::CaptureType t = 
						      GEO_Detail::CAPTURE_BONE);

    static void	 getMultiBoneLayerVisualization(SOP_CaptureLayerData *layer,
				const UT_IntArray &cregion_mapping,
				int ptnum, bool visnormalized,
				const UT_Array<const UT_Vector3 *> *colours,
				UT_Vector3 *vis,
				GEO_Detail::CaptureType t = 
						      GEO_Detail::CAPTURE_BONE);
};

#endif
