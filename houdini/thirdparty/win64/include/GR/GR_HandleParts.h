/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO rendering library (C++)
 *
 * COMMENTS:
 *	Colors and materials for all kinds of handles.
 *
 */

#ifndef __GR_HandleParts_H__
#define __GR_HandleParts_H__

#include "GR_API.h"
#include <RE/RE_Material.h>

#include "GR_DisplayColors.h"

enum GR_HandleMaterial
{
    GR_MATERIAL_ACTIVE,
    GR_MATERIAL_PICKED,
    GR_MATERIAL_INACTIVE,
    GR_MATERIAL_SELECTED,
    GR_MATERIAL_GUIDE,
    GR_MATERIAL_BOX,
    GR_MATERIAL_AXES,
    GR_MATERIAL_X_AXIS,
    GR_MATERIAL_Y_AXIS,
    GR_MATERIAL_Z_AXIS,
    GR_MATERIAL_PIVOT,
    GR_MATERIAL_CPLANE_BORDER,
    GR_MATERIAL_CPLANE_XLATE,
    GR_MATERIAL_CPLANE_ROTATE,
    GR_MATERIAL_SLIDER_MAX_KNOB,
    GR_MATERIAL_TUMBLE_SPHERE,

    GR_NUM_HANDLE_MATERIALS
};

class UT_Color;
class GR_CommonDispOption;

class GR_API GR_HandleParts
{
public:
     GR_HandleParts(const GR_CommonDispOption &dopt);
    ~GR_HandleParts();

    void		 reset();

    RE_MaterialPtr	 getMaterial(GR_HandleMaterial mat, bool ghost = false);
    const UT_Color	&getColor(GR_DisplayColor color, bool ghost = false);

private:
    const UT_Color	&initColor(int ghost, UT_Color *&color,
				UT_Color *&ghostcolor,
				GR_DisplayColor type);

    RE_MaterialPtr	 myMaterials[GR_NUM_HANDLE_MATERIALS*2];

    UT_Color		*myActiveColor;
    UT_Color		*myPickedColor;
    UT_Color		*myInactiveColor;
    UT_Color		*mySelectedColor;
    UT_Color		*myGuideColor;

    UT_Color		*myAxisColor;
    UT_Color		*myXAxisColor;
    UT_Color		*myYAxisColor;
    UT_Color		*myZAxisColor;
    UT_Color		*myPivotColor;
    UT_Color		*myPlaneAxisColor;
    UT_Color		*myViewAxisRotateColor;
    UT_Color		*myTumbleSphereColor;
    UT_Color		*myTracerColor;
    UT_Color		*myDeltaTextColor;
    UT_Color		*myDeltaTextShadowColor;
    
    UT_Color		*myGhostActiveColor;
    UT_Color		*myGhostPickedColor;
    UT_Color		*myGhostInactiveColor;
    UT_Color		*myGhostSelectedColor;
    UT_Color		*myGhostGuideColor;

    UT_Color		*myGhostAxisColor;
    UT_Color		*myGhostXAxisColor;
    UT_Color		*myGhostYAxisColor;
    UT_Color		*myGhostZAxisColor;
    UT_Color		*myGhostPivotColor;
    UT_Color		*myGhostPlaneAxisColor;
    UT_Color		*myGhostViewAxisRotateColor;
    UT_Color		*myGhostTumbleSphereColor;
    UT_Color		*myGhostTracerColor;
    UT_Color		*myGhostDeltaTextColor;
    UT_Color		*myGhostDeltaTextShadowColor;

    UT_Color		*myCplaneHandleXlateColor;
    UT_Color		*myCplaneHandleRotateColor;
    UT_Color		*myCplaneHandleBorderColor;

    UT_Color		*myGhostCplaneHandleXlateColor;
    UT_Color		*myGhostCplaneHandleRotateColor;
    UT_Color		*myGhostCplaneHandleBorderColor;

    UT_Color		*mySliderKnobMaxColor;
    UT_Color		*myGhostSliderKnobMaxColor;

    const GR_CommonDispOption	&myDisplayOpt; // our color source
};

#endif
