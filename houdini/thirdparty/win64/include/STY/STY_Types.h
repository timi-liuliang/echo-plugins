/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Types.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Types__
#define __STY_Types__

#include "STY_API.h"
#include <UT/UT_Set.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Array.h>

/// Style sheet categories.
STY_API extern const UT_StringHolder	 STY_OVERRIDE_MAT;
STY_API extern const UT_StringHolder	 STY_OVERRIDE_MAT_PARMS;
STY_API extern const UT_StringHolder	 STY_OVERRIDE_MAT_PROPERTIES;
STY_API extern const UT_StringHolder	 STY_OVERRIDE_SURFACE_PARMS;
STY_API extern const UT_StringHolder	 STY_OVERRIDE_DISPLACE_PARMS;
STY_API extern const UT_StringHolder	 STY_OVERRIDE_PROPERTIES;

/// Common Target requirements.
/// Name: the name of the target (eg, prim ID or its 'name' attrib value).
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_NAME;
/// Path: for brevity it describes the target hierarchy in terms
/// of a single string (similar to file paths).
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_PATH;
/// Object name: the name of the geometry object.
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_OBJNAME;
/// Object tag: a tag expression tested against the geometry object tags.
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_OBJCATEGORY;
/// Object bundle: a bundle that tests the geometry object for membership.
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_OBJBUNDLE;
/// Prim Group: ad-hoc primitive group the target needs to belong to).
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_PRIMGROUP;
/// Point Group: ad-hoc point group the target needs to belong to).
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_POINTGROUP;
/// Vertex Group: eg, ad-hoc vertex group the target needs to belong to).
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_VERTEXGROUP;
/// Shape: shape, eg, the name of the agent's shape.
STY_API extern const UT_StringHolder	 STY_REQUIREMENT_SHAPE;

/// Class to push and pop information used to generate error messages.
/// The string data is not copied, so must remain valid for the lifetime
/// of the scope object. The information is kept for each thread.
class STY_API STY_ErrorScope
{
public:
    STY_ErrorScope(const char *source_location, const char *source);
    ~STY_ErrorScope();
};

/// We want to load style sheets differently depending on whether we are
/// loading them to construct a STY_Styler and do actual styling, or if
/// we are loading them to edit their contents in the UI.
enum STY_StyleSheetLoadMode
{
    STY_LOAD_FOR_STYLING,
    STY_LOAD_FOR_EDITING
};

/// Flags that may be set on stylesheets.
enum STY_StyleSheetFlags
{
    STY_STYLESHEET_NOFLAGS	= 0x00,
    STY_STYLESHEET_SOLO		= 0x01,
    STY_STYLESHEET_MUTE		= 0x02
};

/// Flags that may be set on styles.
enum STY_StyleFlags
{
    STY_STYLEENTRY_NOFLAGS	= 0x00,
    STY_STYLEENTRY_SOLO		= 0x01,
    STY_STYLEENTRY_MUTE		= 0x02,
    STY_STYLEENTRY_TARGETONLY	= 0x04,
};


// Represents a value object used in STY library.
class UT_OptionEntry;
using STY_Constant		= UT_OptionEntry;
using STY_ConstantHandle	= UT_SharedPtr<const STY_Constant>;
using STY_ConstantMap		= UT_SortedStringMap<STY_ConstantHandle>;

/// Represents a script that calculates an override value.
class STY_OverrideScript;
using STY_OverrideScriptHandle	= UT_IntrusivePtr<const STY_OverrideScript>;
using STY_OverrideScriptMap	= UT_SortedStringMap<STY_OverrideScriptHandle>;

/// Represents an override definition contained in a style.
class STY_Override;
using STY_OverrideHandle	= UT_IntrusivePtr<const STY_Override>;
using STY_OverrideMap		= UT_SortedStringMap<STY_OverrideHandle>;

/// A set of related overrides, eg, overrides for material parameters.
class STY_OverrideCategory;
using STY_OverrideCategoryHandle = UT_IntrusivePtr<const STY_OverrideCategory>;
using STY_OverrideCategoryMap = UT_SortedStringMap<STY_OverrideCategoryHandle>;

/// Targets an entity to be overriden by a style.
class STY_Target;
using STY_TargetHandle		= UT_IntrusivePtr<const STY_Target>;

/// Defines a target entity and a collection of override sets.
class STY_Style;
using STY_StyleHandle		= UT_IntrusivePtr<const STY_Style>;

/// Represents an entity whose properties to override, if it matches the target.
class STY_Subject;
using STY_SubjectHandle	= UT_IntrusivePtr<STY_Subject>;

/// Represents a set of override categories grouped together to define a look.
class STY_OverrideBlock;
using STY_OverrideBlockHandle	= UT_IntrusivePtr<const STY_OverrideBlock>;
using STY_OverrideBlockMap	= UT_SortedStringMap<STY_OverrideBlockHandle>;

/// Map of override categories to lists of override names.
using STY_OverrideNames		= UT_StringMap< UT_Set<UT_StringHolder> >;

/// Represents binding between a script parameter and an arg value data source.
class STY_Binding;
using STY_BindingHandle		= UT_IntrusivePtr<const STY_Binding>;

/// Represents a data source from which arg values are taken.
class STY_BindingDataSource;
using STY_BindingDataSourceHandle =
	UT_IntrusivePtr<const STY_BindingDataSource>;

/// Represents a target match closure, ie a subject that matched a target.
class STY_TargetMatchClosure;
using STY_TargetMatchClosureHandle = 
	UT_IntrusivePtr<const STY_TargetMatchClosure>;

/// Represents a styler match.
class STY_StylerMatch;
using STY_StylerMatchHandle	= UT_IntrusivePtr<const STY_StylerMatch>;

/// Represents a group matche.
class STY_StylerGroupMatch;
using STY_StylerGroupMatchHandle = UT_IntrusivePtr<STY_StylerGroupMatch>;

/// Represents a material definition, eg, a shader name and initial arguments.
class STY_Material;
using STY_MaterialHandle	= UT_IntrusivePtr<const STY_Material>;
using STY_MaterialMap		= UT_SortedStringMap<STY_MaterialHandle>;

/// Represents a style sheet that packages all the above elements.
class STY_StyleSheet;
using STY_StyleSheetHandle	= UT_IntrusivePtr<const STY_StyleSheet>;


#endif

