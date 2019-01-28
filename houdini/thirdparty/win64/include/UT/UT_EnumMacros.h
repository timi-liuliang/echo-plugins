 /*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Macros for generating base enum code.
 */

#ifndef UT_EnumMacros_H
#define UT_EnumMacros_H

#define UT_TO_STRING(name) #name
#define UT_STRING(name) UT_TO_STRING(name)
#define UT_CAT2(a, b) a ## b
#define UT_CAT(a,b) UT_CAT2(a,b)

// These macros generates enum base code
// MACRO should take the following parameters:
// enum_module: enum module name 
// enum_elem: enum element name 
// ienum_elem: internal enum element 
// ienum_elem_name: internal enum element name
// enum_value: enum value

#define UT_UIEVENT_REASON_CODE_GENERATOR(MACRO)\
MACRO(uiEventReason, NoReason,	    UI_NO_REASON,	    "", 0)\
MACRO(uiEventReason, Changed,	    UI_VALUE_CHANGED,	    "", 1)\
MACRO(uiEventReason, Active,	    UI_VALUE_ACTIVE,	    "", 2)\
MACRO(uiEventReason, Start,	    UI_VALUE_START,	    "", 3)\
MACRO(uiEventReason, New,	    UI_VALUE_NEW,	    "", 4)\
MACRO(uiEventReason, RangeChanged,  UI_VALUE_RANGE_CHANGED, "", 5)\
MACRO(uiEventReason, Picked,	    UI_VALUE_PICKED,	    "", 6)\
MACRO(uiEventReason, Located,	    UI_VALUE_LOCATED,	    "", 7)\
MACRO(uiEventReason, ItemsChanged,  UI_VALUE_ITEMS_CHANGED, "", 8)

#define UT_UIEVENT_VALUE_CODE_GENERATOR(MACRO)\
MACRO(uiEventValueType, Float,	    UI_VALUE_FLOAT,	    "", 0)\
MACRO(uiEventValueType, Integer,    UI_VALUE_INTEGER,	    "", 1)\
MACRO(uiEventValueType, String,	    UI_VALUE_STRING,	    "", 2)\
MACRO(uiEventValueType, FloatArray, UI_VALUE_FLOAT_ARRAY,   "", 3)\
MACRO(uiEventValueType, IntArray,   UI_VALUE_INT_ARRAY,	    "", 4)\
MACRO(uiEventValueType, StringArray,UI_VALUE_STRING_ARRAY,  "", 5)\
MACRO(uiEventValueType, NoType,	    UI_VALUE_POINTER,	    "", 6)

#define UT_DRAWABLE_PRIMITIVE_CODE_GENERATOR(MACRO)\
MACRO(drawablePrimitive, Sphere, DM_DrawablePrimitive::Sphere, "", 0)

#endif // UT_EnumMacros_H
