/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP Library (C++)
 *
 * COMMENTS:	Common UI definitions
 *
 */

#ifndef __CHOP_ChopShared_H__
#define __CHOP_ChopShared_H__

#include "CHOP_API.h"
#include <PRM/PRM_ChoiceList.h>
#include <PRM/PRM_Range.h>
#include <PRM/PRM_Default.h>

// menu enumerations

enum
{
    ACTIVE_OFF		= 0,
    ACTIVE_ON		= 1,
    ACTIVE_MOMENTARY	= 2,
    ACTIVE_TOGGLE	= 3
};

enum
{
    SHIFT_FORWARD	= 0,
    SHIFT_BACKWARD	= 1,
    SHIFT_OFF		= 2
};

enum
{
    TYPE_RAW		= 0,
    TYPE_SPEEDRAW	= 1,
    TYPE_POSITION	= 2,
    TYPE_SPEED		= 3,
    TYPE_SPEEDHOLD	= 4
};

enum
{
    UNITS_ABSOLUTE	= 0,
    UNITS_RELATIVE	= 1,
    UNITS_CURRENT	= 2
};

enum
{
    RECORD_OFF		= 0,
    RECORD_ON		= 1,
    RECORD_ADD		= 2,
    RECORD_AUTO_RANGE	= 3,
    RECORD_SINGLE	= 4
};

enum
{
    REMAINDER_DISCARD	= 0,
    REMAINDER_EXTEND	= 1,
    REMAINDER_ADD	= 2
};

enum PRM_Match
{
    MATCH_NUMBER	= 0,
    MATCH_NAME		= 1,
    MATCH_UNION		= 2
};

enum PRM_MatchFailure
{
    MATCH_FAILURE_ERROR	    = 0,
    MATCH_FAILURE_WARNING   = 1,
    MATCH_FAILURE_IGNORE    = 2
};

enum PRM_RangeType
{
    RANGE_FULL_ANIMATION    = 0,
    RANGE_CURRENT_FRAME	    = 1,
    RANGE_USER_ENTERED	    = 2,
    RANGE_LAST_ITEM	    = RANGE_USER_ENTERED    // sentinel
};

enum
{
    NUMTHREADS_NONE	    =  0,
    NUMTHREADS_1PERPROC	    =  1,
    NUMTHREADS_NUM_ITEMS	// sentinel
};


// Some common menu defs
CHOP_API extern PRM_ChoiceList		CHOP_AlignMenu;
CHOP_API extern PRM_ChoiceList		CHOP_BlendFunctionMenu;
CHOP_API extern PRM_ChoiceList		CHOP_BlendMethodMenu;
CHOP_API extern PRM_ChoiceList		CHOP_ExtendMenu;
CHOP_API extern PRM_ChoiceList		CHOP_ExtendFullMenu;
CHOP_API extern PRM_ChoiceList		CHOP_FilterChunkMenu;
CHOP_API extern PRM_ChoiceList		CHOP_MatchMenu;
CHOP_API extern PRM_ChoiceList		CHOP_Match2Menu;
CHOP_API extern PRM_ChoiceList		CHOP_MatchFailureMenu;
CHOP_API extern PRM_ChoiceList		CHOP_MatchFailure2Menu;
CHOP_API extern PRM_ChoiceList		CHOP_OrderMenu;
CHOP_API extern PRM_ChoiceList		CHOP_RelativeMenu;
CHOP_API extern PRM_ChoiceList		CHOP_Relative2Menu;
CHOP_API extern PRM_ChoiceList		CHOP_Relative3Menu;
CHOP_API extern PRM_ChoiceList		CHOP_Relative4Menu;
CHOP_API extern PRM_ChoiceList		CHOP_InterpolateMenu;
CHOP_API extern PRM_ChoiceList		CHOP_RemainderMenu;
CHOP_API extern PRM_ChoiceList		CHOP_RangeMenu;
CHOP_API extern PRM_ChoiceList		CHOP_IterateMenu;
CHOP_API extern PRM_ChoiceList		CHOP_NumThreadsMenu;

// Beat box menus
CHOP_API extern PRM_ChoiceList		CHOP_ActiveMenu;
CHOP_API extern PRM_ChoiceList		CHOP_KeyMenu;
CHOP_API extern PRM_ChoiceList		CHOP_TypeMenu;
CHOP_API extern PRM_ChoiceList		CHOP_LimitMenu;
CHOP_API extern PRM_ChoiceList		CHOP_RecordMenu;

// Ranges
CHOP_API extern PRM_Range		CHOP_BiasRange;
CHOP_API extern PRM_Range		CHOP_CycleLengthRange;
CHOP_API extern PRM_Range		CHOP_DefaultValueRange;
CHOP_API extern PRM_Range		CHOP_DoubleFrameRange;
CHOP_API extern PRM_Range		CHOP_FrameRange;
CHOP_API extern PRM_Range		CHOP_OverlapChunkRange;
CHOP_API extern PRM_Range		CHOP_PosFrameRange;
CHOP_API extern PRM_Range		CHOP_SampleRateRange;
CHOP_API extern PRM_Range		CHOP_StepRange;

// Defaults
CHOP_API extern PRM_Default		CHOP_BiasDefault;
CHOP_API extern PRM_Default		CHOP_BlendFunctionDefault;
CHOP_API extern PRM_Default		CHOP_ChunkDiscardDefault;
CHOP_API extern PRM_Default		CHOP_CycleLengthDefault;
CHOP_API extern PRM_Default		CHOP_EndDefault;
CHOP_API extern PRM_Default		CHOP_FilterChunkDefault;
CHOP_API extern PRM_Default		CHOP_HalfWayDefault;
CHOP_API extern PRM_Default		CHOP_OverlapChunkDefault;
CHOP_API extern PRM_Default		CHOP_SampleRateDefault;
CHOP_API extern PRM_Default		CHOP_StartDefault;
CHOP_API extern PRM_Default		CHOP_ScopeDefault;
CHOP_API extern PRM_Default		CHOP_RelativeDefault;
CHOP_API extern PRM_Default		CHOP_InterpolateDefault;

// Beatbox Defaults
CHOP_API extern PRM_Default		CHOP_ActiveDefault;
CHOP_API extern PRM_Default		CHOP_TypeDefault;
CHOP_API extern PRM_Default		CHOP_KeyDefault[];
CHOP_API extern PRM_Default		CHOP_LimitDefault;
CHOP_API extern PRM_Default		CHOP_RecordDefault;
CHOP_API extern PRM_Default		CHOP_RemainderDefault;
CHOP_API extern PRM_Default		CHOP_RotScopeDefault;
CHOP_API extern PRM_Default		CHOP_RXScopeDefault;
CHOP_API extern PRM_Default		CHOP_RYScopeDefault;
CHOP_API extern PRM_Default		CHOP_RZScopeDefault;

// Names
CHOP_API extern PRM_Name		CHOP_AlignName;
CHOP_API extern PRM_Name		CHOP_BiasName;
CHOP_API extern PRM_Name		CHOP_BlendFunctionName;
CHOP_API extern PRM_Name		CHOP_BlendMethodName;
CHOP_API extern PRM_Name		CHOP_BlendRegionName;
CHOP_API extern PRM_Name		CHOP_BlendStepScopeName;
CHOP_API extern PRM_Name		CHOP_ConstantAreaName;
CHOP_API extern PRM_Name		CHOP_CycleLengthName;
CHOP_API extern PRM_Name		CHOP_DefaultValueName;
CHOP_API extern PRM_Name		CHOP_ExtendLeftName;
CHOP_API extern PRM_Name		CHOP_ExtendRightName;
CHOP_API extern PRM_Name		CHOP_MatchName;
CHOP_API extern PRM_Name		CHOP_MatchFailureName;
CHOP_API extern PRM_Name		CHOP_OrderName;
CHOP_API extern PRM_Name		CHOP_RelativeName;
CHOP_API extern PRM_Name		CHOP_BlendStepName;
CHOP_API extern PRM_Name		CHOP_SampleRateName;
CHOP_API extern PRM_Name		CHOP_InterpolateName;
CHOP_API extern PRM_Name		CHOP_RemainderName;
CHOP_API extern PRM_Name		CHOP_RangeName;

CHOP_API extern PRM_Name		CHOP_QuaternionName;
CHOP_API extern PRM_Name		CHOP_RXScopeName;
CHOP_API extern PRM_Name		CHOP_RYScopeName;
CHOP_API extern PRM_Name		CHOP_RZScopeName;
CHOP_API extern PRM_Name		CHOP_ShortRotName;
CHOP_API extern PRM_Name		CHOP_RotScopeName;

CHOP_API extern PRM_Name		CHOP_StartName;
CHOP_API extern PRM_Name		CHOP_EndName;
CHOP_API extern PRM_Name		CHOP_SingleFrameName;


// Beatbox Names
CHOP_API extern PRM_Name		CHOP_ActiveName;
CHOP_API extern PRM_Name		CHOP_RecordName;
CHOP_API extern PRM_Name		CHOP_KeyName;
CHOP_API extern PRM_Name		CHOP_TypeName;
CHOP_API extern PRM_Name		CHOP_LimitName;


// Conversion functions
CHOP_API int 			convertExtendFullMenu(int index);
CHOP_API int			convertKeyMenu(int index);

#endif
