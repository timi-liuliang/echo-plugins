/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonTypes.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor enumerations, defines, structures.
 */

#ifndef __UT_PerfMonTypes__
#define __UT_PerfMonTypes__

#include "UT_API.h"

/// Default file extension to use for profiles.
#define UT_PERFMON_PROFILE_FILE_EXTENSION   ".hperf"

static const int UT_PERFORMANCE_INVALID_FRAME = -1;

/// Categories.
enum UT_PerfMonCategory
{
    UT_PERFMON_INVALID_CATEGORY = -1,
    UT_PERFMON_GENERAL_CATEGORY,
    UT_PERFMON_COOK_CATEGORY,
    UT_PERFMON_SOLVE_CATEGORY,
    UT_PERFMON_DRAW_CATEGORY,
    UT_PERFMON_GPU_DRAW_CATEGORY,
    UT_PERFMON_SCRIPT_CATEGORY,
    UT_PERFMON_RENDER_CATEGORY,
    UT_PERFMON_NUM_CATEGORIES
};

/// Event types.
enum UT_PerfMonEventType
{
    UT_PERFMON_INVALID_EVENT = -1,
    UT_PERFMON_GENERAL_EVENT,
    UT_PERFMON_TIMED_EVENT,
    UT_PERFMON_MEMORY_EVENT,
    UT_PERFMON_NUM_EVENT_TYPES
};

/// Object types.
enum UT_PerfMonObjectType
{
    UT_PERFMON_INVALID_OBJECT = -1,
    UT_PERFMON_NODE_OBJECT,
    UT_PERFMON_VIEWPORT_OBJECT,
    UT_PERFMON_SCRIPT_OBJECT,
    UT_PERFMON_RENDERER_OBJECT,
    UT_PERFMON_FRAME_OBJECT,
    UT_PERFMON_OTHER_OBJECT,
    UT_PERFMON_NUM_OBJECT_TYPES
};

// Enumeration of stat types.
enum UT_PerfMonStatType
{
    UT_PERFMON_INVALID_STAT_TYPE = -1,
    UT_PERFMON_CUMULATIVE_TIME_STAT,
    UT_PERFMON_SELF_TIME_STAT,
    UT_PERFMON_EVENT_COUNT_STAT,
    UT_PERFMON_AVERAGE_TIME_STAT,
    UT_PERFMON_MIN_TIME_STAT,
    UT_PERFMON_MAX_TIME_STAT,
    UT_PERFMON_CUMULATIVE_MEMORY_STAT,
    UT_PERFMON_SELF_MEMORY_STAT,
    UT_PERFMON_NUM_STAT_TYPES
};

/// Viewport types recognized by the performance monitor.
enum UT_PerfMonViewportType
{
    UT_PERFMON_3D_VIEWPORT,
    UT_PERFMON_2D_VIEWPORT,
    UT_PERFMON_NUM_VIEWPORT_TYPES
};

/// Script types recognized by the performance monitor.
enum UT_PerfMonScriptType
{
    UT_PERFMON_HSCRIPT,
    UT_PERFMON_PYTHON,
    UT_PERFMON_NUM_SCRIPT_TYPES
};

// Enumeration of attribute types.
enum UT_PerfMonAttributeType
{
    UT_PERFMON_INVALID_ATTR_TYPE = -1,
    UT_PERFMON_STRING_ATTR_TYPE,
    UT_PERFMON_NUMBER_ATTR_TYPE,
    UT_PERFMON_MAP_ATTR_TYPE,
    UT_PERFMON_ARRAY_ATTR_TYPE,
    UT_PERFMON_NUM_ATTR_TYPES
};

// Enumeration of public profile attributes.
enum UT_PerfMonProfileAttribute
{
    UT_PERFMON_INVALID_PROFILE_ATTR = -1,
    UT_PERFMON_PROFILE_NAME_ATTR,
    UT_PERFMON_PROFILE_STATS_ATTR,
    UT_PERFMON_PROFILE_STAT_DEFS_ATTR,
    UT_PERFMON_NUM_PROFILE_ATTRS
};

// Enumeration of public stats attributes.
enum UT_PerfMonStatsAttribute
{
    UT_PERFMON_INVALID_STATS_ATTR = -1,
    UT_PERFMON_STATS_NAME_ATTR,
    UT_PERFMON_STATS_PATH_ATTR,
    UT_PERFMON_STATS_OBJ_TYPE_ATTR,
    UT_PERFMON_STATS_OBJ_ICON_ATTR,
    UT_PERFMON_STATS_GLOBAL_STATS_ATTR,
    UT_PERFMON_STATS_STATS_ATTR,
    UT_PERFMON_STATS_CHILDREN_ATTR,
    UT_PERFMON_NUM_STATS_ATTRS
};

// Enumeration of public stat definition attributes.
enum UT_PerfMonStatDefAttribute
{
    UT_PERFMON_INVALID_STAT_DEF_ATTR = -1,
    UT_PERFMON_STAT_DEF_NAME_ATTR,
    UT_PERFMON_STAT_DEF_CATEGORY_ATTR,
    UT_PERFMON_STAT_DEF_TYPE_ATTR,
    UT_PERFMON_STAT_DEF_IS_DIFF_ATTR,
    UT_PERFMON_NUM_STAT_DEF_ATTRS
};

// Time formats to use in event logs.
enum UT_PerfMonLogTimeFormat
{
    UT_PERFMON_LOG_TIME_FORMAT_ALL,
    UT_PERFMON_LOG_TIME_FORMAT_MS,
    UT_PERFMON_LOG_TIME_FORMAT_SEC
};

/// Return the (display) name of the given event type.
UT_API const char   *UTgetEventTypeName(UT_PerfMonEventType type);

/// Return the (display) name of the given category.
UT_API const char   *UTgetCategoryName(UT_PerfMonCategory category);

/// Return the (display) name of the given event object type.
UT_API const char   *UTgetObjectTypeName(UT_PerfMonObjectType object_type);

/// Return the (display) name of the given stat type.
UT_API const char   *UTgetStatTypeName(UT_PerfMonStatType stat_type);

/// Return the event type enum of the given event type name.
UT_API UT_PerfMonEventType
		     UTgetEventType(const char *type_name);

/// Return the category enum of the given category name.
UT_API UT_PerfMonCategory
		     UTgetCategory(const char *category_name);

/// Return the enum type of the given object type name.
UT_API UT_PerfMonObjectType
		     UTgetObjectType(const char *type_name);

/// Return the enum type of the given stat type name.
UT_API UT_PerfMonStatType
		     UTgetStatType(const char *type_name);

/// Return the (display) name of the given profile attribute.
UT_API const char   *UTgetProfileAttrName(UT_PerfMonProfileAttribute attr);

/// Return the type of the given profile attribute.
UT_API UT_PerfMonAttributeType	
		     UTgetProfileAttrType(UT_PerfMonProfileAttribute attr);

/// Return the (display) name of the given stats attribute.
UT_API const char   *UTgetStatsAttrName(UT_PerfMonStatsAttribute attr);

/// Return the (display) name of the given stat definition attribute.
UT_API const char   *UTgetStatDefAttrName(UT_PerfMonStatDefAttribute attr);

/// Return the profile attribute bound to the given (display) name.
/// Return UT_PERFMON_INVALID_PROFILE_ATTR if no such attribute exists.
UT_API UT_PerfMonProfileAttribute
		     UTgetProfileAttribute(const char *attr_name);

/// Return the type of the given stats attribute.
UT_API UT_PerfMonAttributeType
		     UTgetStatsAttrType(UT_PerfMonStatsAttribute attr);

/// Return the stats attribute bound to the given (display) name.
/// Return UT_PERFMON_INVALID_STATS_ATTR if no such attribute exists.
UT_API UT_PerfMonStatsAttribute
		     UTgetStatsAttribute(const char *attr_name);

/// Return the stat definition attribute bound to the given (display) name.
/// Return UT_PERFMON_INVALID_STAT_DEF_ATTR if no such attribute exists.
UT_API UT_PerfMonStatDefAttribute
		     UTgetStatDefAttribute(const char *attr_name);

/// Initialize data structures internal to this module.
/// This should only be called once from the UT_Performance constructor.
UT_API void	     UTinitPerfMonTypes();
#endif
