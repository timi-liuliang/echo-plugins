/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	trim fillet (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_TrimFillet_h__
#define __GU_TrimFillet_h__

#include "GU_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_FloatArray.h>

class GD_Face;
class GD_TrimLoop;
class GEO_Face;

class GU_API GU_TrimFilletParms
{
public:
    GU_TrimFilletParms(int curves);
    ~GU_TrimFilletParms();

    int 	 order;
    int		 numcurves;
    int 	 usecurvature;
    int		 circularfillet;
    int		 usegordon;

    UT_ValArray<GD_TrimLoop *>		loop;
    UT_ValArray<GD_Face *>		face;
    UT_ValArray<GEO_Face *>		gface;

    UT_FloatArray			width1, width2;

    // Scale factors for tangents:
    UT_FloatArray			 scaletangents;
    UT_IntArray				 normalizetangents;
    UT_FloatArray			 scalecurvature;
    UT_IntArray				 normalizecurvature;
    UT_IntArray				 fliptangents;
    UT_IntArray				 usefrenet;
    UT_FloatArray			 theta;

    int		 divisions;
    int		 extractdiv;
    int		 keepsharp;
    float	 fittol;
};
    

#endif
