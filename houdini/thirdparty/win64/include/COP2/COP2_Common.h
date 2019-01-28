/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Node.h
 *
 * COMMENTS:
 *	Common defines for COP Nodes.
 */
#ifndef _COP2_COMMON_H_
#define _COP2_COMMON_H_

#include <OP/OP_DataTypes.h>

#define COP_MASK_PAGE_NAME	"Mask"
#define COP_GEN_MASK_COUNT	4
#define COP_GEN_IMAGE_PAGE	"Image"
#define COP_GEN_IMAGE_COUNT	16
#define COP_GEN_SEQUENCE_PAGE	"Sequence"
#define COP_GEN_SEQUENCE_COUNT	8
#define COP_MULTI_MERGE_PAGE	"Merge"
#define COP_MULTI_MERGE_COUNT	5
#define COP_FRAME_PAGE_NAME	"Frame Scope"

#define TOOL_PARM	PRM_Template::PRM_EXPORT_TBX
#define POPUP_PARM	PRM_Template::PRM_EXPORT_MAX

#define COP_EMPTY_SWITCHER(name)				\
	static PRM_Default switcher[] = {			\
		PRM_Default(0,name), }	

#define COP_SWITCHER(count, name)				\
	static PRM_Default switcher[] = {			\
		PRM_Default(count, name) }	
	
#define COP_SWITCHER2(count1, name1, count2, name2)		\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2) }	

#define COP_SWITCHER3(count1, name1, count2, name2, count3, name3)\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(count3, name3) }	

#define COP_SWITCHER4(count1,name1, count2,name2, count3,name3, count4,name4)\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(count3, name3),			\
		PRM_Default(count4, name4) }

#define COP_SWITCHER5(count1,name1, count2,name2, count3,name3, count4,name4, count5, name5)\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(count3, name3),			\
		PRM_Default(count4, name4),			\
		PRM_Default(count5, name5) }

// Generator switchers...

#define COP_GENERATOR_SWITCHER(count, name)			\
	static PRM_Default switcher[] = {			\
		PRM_Default(count, name),			\
		PRM_Default(COP_GEN_MASK_COUNT, COP_MASK_PAGE_NAME), \
		PRM_Default(COP_GEN_IMAGE_COUNT, COP_GEN_IMAGE_PAGE), \
		PRM_Default(COP_GEN_SEQUENCE_COUNT, COP_GEN_SEQUENCE_PAGE) }

#define COP_GENERATOR_SWITCHER2(count1, name1, count2, name2)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(COP_GEN_MASK_COUNT, COP_MASK_PAGE_NAME), \
		PRM_Default(COP_GEN_IMAGE_COUNT, COP_GEN_IMAGE_PAGE), \
		PRM_Default(COP_GEN_SEQUENCE_COUNT, COP_GEN_SEQUENCE_PAGE) }

#define COP_GENERATOR_SWITCHER3(count1,name1, count2,name2, count3,name3) \
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(count3, name3),			\
		PRM_Default(COP_GEN_MASK_COUNT, COP_MASK_PAGE_NAME), \
		PRM_Default(COP_GEN_IMAGE_COUNT, COP_GEN_IMAGE_PAGE), \
		PRM_Default(COP_GEN_SEQUENCE_COUNT, COP_GEN_SEQUENCE_PAGE) }

// Multi-Input Switchers
#define COP_MULTI_SWITCHER(count, name)				\
	static PRM_Default switcher[] = {			\
		PRM_Default(count, name),			\
		PRM_Default(COP_MULTI_MERGE_COUNT, COP_MULTI_MERGE_PAGE) }
	
#define COP_MULTI_SWITCHER2(count1, name1, count2, name2)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(COP_MULTI_MERGE_COUNT, COP_MULTI_MERGE_PAGE) }


#define COP_MULTI_SWITCHER3(count1, name1, count2, name2, count3, name3)\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2, name2),			\
		PRM_Default(count3, name3),			\
		PRM_Default(COP_MULTI_MERGE_COUNT, COP_MULTI_MERGE_PAGE) }

// Pixel-based Switcher (classes derived directly from COP2_PixelBase)
#define COP_PIXEL_EXTRAS	11
#define COP_PIXEL_FIRST_EXTRAS	 2
#define COP_PIXEL_FRAME_EXTRAS	 9

#define COP_PIXEL_SWITCHER(count, name)				\
	static PRM_Default switcher[] = {			\
		PRM_Default(count+COP_PIXEL_FIRST_EXTRAS, name),\
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

#define COP_PIXEL_SWITCHER2(count1, name1, count2, name2)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2+COP_PIXEL_FIRST_EXTRAS, name2),\
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

// Mask Op-based Switcher (classes derived directly from COP2_MaskOp)
#define COP_MASK_EXTRAS	(COP_PIXEL_FIRST_EXTRAS+5)

#define COP_MASK_SWITCHER(count, name)				\
	static PRM_Default switcher[] = {			\
		PRM_Default(count, name),	\
	        PRM_Default(COP_MASK_EXTRAS, COP_MASK_PAGE_NAME),   \
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

#define COP_MASK_SWITCHER2(count1, name1, count2, name2)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),	\
		PRM_Default(count2, name2),	\
	        PRM_Default(COP_MASK_EXTRAS, COP_MASK_PAGE_NAME),   \
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

#define COP_MASK_SWITCHER3(count1, name1, count2, name2, count3, name3)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),	\
		PRM_Default(count2, name2),	\
		PRM_Default(count3, name3),	\
	        PRM_Default(COP_MASK_EXTRAS, COP_MASK_PAGE_NAME),   \
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

// Pixel Op-based Switcher (classes derived directly from COP2_PixelOp)
#define COP_PIXEL_OP_EXTRAS 2

#define COP_PIXEL_OP_SWITCHER(count, name)			\
	static PRM_Default switcher[] = {			\
		PRM_Default(count+COP_PIXEL_OP_EXTRAS, name),	\
	        PRM_Default(COP_MASK_EXTRAS, COP_MASK_PAGE_NAME),   \
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

#define COP_PIXEL_OP_SWITCHER2(count1, name1, count2, name2)	\
	static PRM_Default switcher[] = {			\
		PRM_Default(count1, name1),			\
		PRM_Default(count2+COP_PIXEL_OP_EXTRAS, name2),	\
	        PRM_Default(COP_MASK_EXTRAS, COP_MASK_PAGE_NAME),   \
		PRM_Default(COP_PIXEL_FRAME_EXTRAS, COP_FRAME_PAGE_NAME) }

#define DEFAULT_COLOR_NAME	"C"
#define DEFAULT_RED_NAME	"R"
#define DEFAULT_GREEN_NAME	"G"
#define DEFAULT_BLUE_NAME	"B"

#define DEFAULT_ALPHA_NAME	"A"
#define DEFAULT_LUMINANCE_NAME	"Y"
#define DEFAULT_DEPTH_NAME	"Z"
#define DEFAULT_STENCIL_NAME	"S"

// Image units
#define IMAGE_UNIT_UV			0
#define IMAGE_UNIT_PIXEL		1

// Plane merging options
#define PLANE_MERGE_ALL			0
#define PLANE_MERGE_COMMON		1
#define PLANE_MERGE_FIRST		2

// Depth matching options
#define PLANE_DEPTH_PROMOTE		0
#define PLANE_DEPTH_DEMOTE		1
#define PLANE_DEPTH_FIRST		2
#define PLANE_DEPTH_MISMATCH_ERROR	3

// Range matching options
#define SEQUENCE_RANGE_TRIM_FIRST	0
#define SEQUENCE_RANGE_SHIFT_FIRST	1
#define SEQUENCE_RANGE_EXTEND_MAX	2
#define SEQUENCE_RANGE_TRIM_MIN		3

// Rate selection options
#define SEQUENCE_RATE_FIRST		0
#define SEQUENCE_RATE_HIGHEST		1
#define SEQUENCE_RATE_LOWEST		2
#define SEQUENCE_RATE_MISMATCH_ERROR	3

// Frame matching options
#define SEQUENCE_FRAME_NEAREST		0
#define SEQUENCE_FRAME_PREVIOUS		1
#define SEQUENCE_FRAME_NEXT		2

// Icon Preview options
#define ICON_PREVIEW_RGB		0
#define ICON_PREVIEW_ALPHA		1
#define ICON_PREVIEW_DEPTH		2
#define ICON_PREVIEW_LUM		3

// COP Thread options
#define COP_MULTITHREADED		0
#define COP_SINGLE_THREAD		1
#define COP_EXCLUSIVE_THREAD		2

// Parm Groups
#define COP2_PARM_NONVOLITILE		0
#define COP2_PARM_IMAGE			1
#define COP2_PARM_TIMING		2
#define COP2_PARM_ALL_VOLITILE		3

// Shift options
#define SHIFT_ALIGN_START_START		0
#define SHIFT_ALIGN_END_END		1
#define SHIFT_ALIGN_END_START		2
#define SHIFT_ALIGN_START_END		3

#define SHIFT_REL_TO_START		0
#define SHIFT_ABS_START			1
#define SHIFT_ABS_END			2

// Trim options
#define TRIM_REL		0
#define TRIM_ABS		1
#define TRIM_SINGLE		2
#define TRIM_SINGLE_START	3
#define TRIM_SINGLE_END		4

// COP coloring.
enum COP2_OpColorType {
    COP2_COLOR_GENERATOR,
    COP2_COLOR_TIMING,
    COP2_COLOR_PIXELOP,
    COP2_COLOR_VEX
};

enum COP2_ThreadPref
{
    COP2_THREAD_NO_PREF = 0,
    COP2_THREAD_SINGLE  = 0x1,
    COP2_THREAD_MULTI   = 0x2
};

enum COP2_ColorComponent {
    COP2_RED,
    COP2_GREEN,
    COP2_BLUE,
    COP2_ALPHA,
    COP2_HUE,
    COP2_SATURATION,
    COP2_VALUE,
    COP2_LUMINANCE,
    COP2_NUM_COLOR_COMPONENTS
};

enum COP2_UIChangeType 
{
    COP2_UICHANGE_DISK_CACHE	= OP_UICHANGE_OPTYPE_COP2
};


// defined at the end of COP2_Node.C
struct copLockInfo
{
    copLockInfo(const char *name, int num);
    ~copLockInfo();

    static int hash(const char *name);
    
    char	*myLockedPlane;
    int		 myLockedFrame;
    int		 myHash;
};

#endif
