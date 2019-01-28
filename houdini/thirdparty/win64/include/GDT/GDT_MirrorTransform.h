/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GDT_MirrorTransform_h__
#define __GDT_MirrorTransform_h__

#include "GDT_API.h"
#include "GDT_SymmetryTransform.h"

#include <GEO/GEO_PointTree.h>
#include <GU/GU_Detail.h>

class GDT_API GDT_MirrorTransform : public GDT_SymmetryTransform
{
public:
    GDT_MirrorTransform();
    void rebuild(const GU_Detail *gdp);
    void setMirrorProperties(UT_Vector3 &axis, UT_Vector3 &origin, float distance, float threshold);
    virtual GA_Index findSymmetry(GA_Index input_delta_index,
				  const GA_AIFEditDeltaMap &map,
				  UT_Matrix3R &xform);
    virtual const GEO_Detail *getDetail() const { return myDetail; }

    void clear();

private:
    const GU_Detail	*myDetail;
    UT_Vector3		 myAxis;
    UT_Vector3		 myOrigin;
    UT_Matrix3R		 myReflMatrix;
    float		 myDistance;
    float		 myThreshold;
    GEO_PointTreeGAOffset myPointTree;
};

#endif
