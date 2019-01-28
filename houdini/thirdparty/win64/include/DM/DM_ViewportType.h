/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DM_ViewportType.h ( DM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DM_ViewportType__
#define __DM_ViewportType__

#define DM_VIEWPORT_ORTHO	0x0ff
#define DM_VIEWPORT_TOP		0x001
#define DM_VIEWPORT_FRONT	0x002
#define DM_VIEWPORT_RIGHT	0x004
#define DM_VIEWPORT_BOTTOM	0x008
#define DM_VIEWPORT_BACK	0x010
#define DM_VIEWPORT_LEFT	0x020

#define DM_VIEWPORT_PERSPECTIVE	0xf00

#define DM_VIEWPORT_ALL_3D	(DM_VIEWPORT_PERSPECTIVE|DM_VIEWPORT_ORTHO)

#define DM_VIEWPORT_UV		0x1000

#define DM_VIEWPORT_ALL		(DM_VIEWPORT_ALL_3D | DM_VIEWPORT_UV)

typedef int DM_ViewportType;

/// Enumerates the ways in which the viewport can display stereo 3d images
enum DM_S3DDisplayFormat
{
    DM_S3D_DISP_ANAGLYPH,	    // red-cyan anaglyph
    DM_S3D_DISP_H_INTERLACE,	    // horizontal line interlace 
    DM_S3D_DISP_H_INTERLACE_REV,    // as above, but with even lines for R-eye
    DM_S3D_DISP_QUAD_BUFFER,	    // OpenGL quad buffer stereo
    DM_S3D_DISP_LEFT_RIGHT,	    // Left/Right image (2 in 1, horiz aligned)
    DM_S3D_DISP_LEFT_RIGHT_REV,	    // Right/Left image (2 in 1, horiz aligned)
    DM_S3D_DISP_OVER_UNDER,	    // Over/under image (2 in 1 LR vert aligned)
    DM_S3D_DISP_OVER_UNDER_REV,	    // Over/under image (2 in 1 RL vert aligned)
   
    DM_S3D_DISP_SINGLE_EYE	    // Render a single eye only.
};

/// Different types of things that can be located in the viewport.
enum DM_LocateFlags
{
    // Handles can be combined with any other locate type.
    DM_LOCATE_HANDLES				= 0x0001,
    // Selectable objects with or without handles.
    DM_LOCATE_SELECTABLE_OBJECTS		= 0x0002,
    DM_LOCATE_SELECTABLE_OBJECTS_AND_HANDLES	= 0x0003,
    // Non-selectable objects only. Just used to differentiate ALL_OBJECTS
    // from SELECTABLE_OBJECTS.
    DM_LOCATE_NONSELECTABLE_OBJECTS		= 0x0004,
    // All objects means selectable objects and non-selectable objects.
    DM_LOCATE_ALL_OBJECTS			= 0x0006,
    DM_LOCATE_ALL_OBJECTS_AND_HANDLES		= 0x0007,
    // Component locating is mutually exclusive with objet locating.
    DM_LOCATE_COMPONENTS			= 0x0008,
    DM_LOCATE_COMPONENTS_AND_HANDLES		= 0x0009
};

#endif
