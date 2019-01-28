/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Selection (C++)
 *
 * COMMENTS:
 *	Base class of a gdp selection. This clas hides the actual type
 *	of selection (point, edge, primitive, etc)
 *
 * 
 */

#ifndef __GU_SelectType_h__
#define __GU_SelectType_h__

#include <UT/UT_SharedPtr.h>
#include <SYS/SYS_Deprecated.h>

class GU_Selection;

/// Shared pointer to a selection. Used to pass them around the system since
/// we have to be careful about ownership passing around.
typedef UT_SharedPtr<GU_Selection> GU_SelectionHandle;


// Types of known selections:
enum SYS_DEPRECATED(15.0) GU_SelectionType 
{
    GU_SInvalid = -1,
    GU_SPoint = 0,
    GU_SVertex = 1,
    GU_SEdge = 2,
    GU_SBreakpoint = 3,
    GU_SPrimitive = 4,
};

// Different ways to combine selections.
enum GU_SelectionRule
{
    GU_InvalidSelect = -1,
    GU_AddSelect = 0,
    GU_ToggleSelect = 1,
    GU_RemoveSelect = 2,
    GU_ReplaceSelect = 3,
    GU_IntersectSelect = 4,
    GU_NumSelectionRules = 5
};

// Possible results of a select operation.
enum GU_SelectionResult
{
    GU_SelectionAdd,
    GU_SelectionRemove,
    GU_SelectionNone
};

enum GU_ModifyType
{
    GU_ModifyAdd,
    GU_ModifyToggle,
    GU_ModifyRemove
};

enum GU_Connectivity
{
    GU_CONNECTIVITY_NONE = -1,
    GU_CONNECTIVITY_3D = 0,
    GU_CONNECTIVITY_UV = 1,
    GU_CONNECTIVITY_CUT = 2
};

#endif
