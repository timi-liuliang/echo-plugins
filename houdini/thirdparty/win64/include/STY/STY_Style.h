/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Style.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Style__
#define __STY_Style__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_Target.h"
#include "STY_SourceLocation.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringArray.h>

class STY_LoadContext; 
class STY_Results;
class STY_ResultsFilter;
class UT_JSONValueMap;

/// Represents a style sheet entry that contains a target and overrides.
class STY_API STY_Style :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_Style>
{
public:
		 STY_Style(const UT_JSONValue *json,
			const STY_LoadContext *load_context,
			STY_StyleSheetLoadMode load_mode,
			const char* file_name);
		 STY_Style(const UT_StringHolder &label,
			const STY_TargetHandle &target,
			const UT_StringArray &import_overrides,
			const STY_OverrideBlockHandle &local_overrides,
			STY_StyleFlags flags);
		 STY_Style(const STY_Style &src,
			const STY_OverrideBlockHandle &local_overrides);
		~STY_Style();

    /// Returns the label given to this style entry.
    const UT_StringHolder &getLabel() const
		 { return myLabel; }

    /// Returns the node path for this style entry
    const UT_StringHolder &getPath() const
		 { return myPath; }

    /// Returns the target the style sheet entry is intended for.
    const STY_TargetHandle &getTarget() const
		 { return myTargetHandle; }

    /// Returns the overrides referenced by the style sheet entry, and any
    /// defined within the style entry (in the map with an empty string key).
    const STY_OverrideBlockMap &getOverrideSets() const
		 { return myOverrideSets; }
    /// Return the names of referenced overrides in the order specified in
    /// the JSON array. This will include the names of sets that could not
    /// actually be found in the load context.
    const UT_StringArray &getOverrideSetsNames() const
		 { return myOverrideSetsNames; }

    /// Obtains the list of data names that the entry's override scripts
    /// are interested in for their input argument binding.
    const UT_StringArray &getOverrideBindingDataNames() const;

    /// Gets the flags for this style entry.
    STY_StyleFlags	getFlags() const
		 { return myFlags; }

    STY_SourceLocation getSourceLocation() const
		 { return mySourceLocation; }

    /// Create a JSON map for saving to a style sheet. Not reference counted.
    void	 saveStyleSheets(UT_JSONValue *values,
			STY_TargetMatchClosureHandle *closures,
			const exint num_values,
			const STY_TargetHandle &target,
			bool evaluate_scripts_if_possible,
			bool save_verbose,
			bool save_path) const;

    /// Calculate override values from this style for an array of closures.
    void	 getResults(STY_Results **results,
			STY_TargetMatchClosureHandle *closures,
			bool all_closures_identical,
			const exint num_values,
			const STY_ResultsFilter &filter) const;

    /// Gather just the categories and names of the overrides that would
    /// be set by this style if it were to match.
    void	 getOverrideNames(STY_OverrideNames &override_names) const;

    /// Returns a style entry identical to this one but without any overrides
    /// that match the passed in overrides parameter. Returns nullptr if there
    /// are no overrides left after removing these. Returns this if no
    /// overrides are removed.
    const STY_Style *cloneWithOverridesRemoved(
			const STY_OverrideNames &override_names) const;

private:
    void	 loadStyle(const UT_JSONValue *json,
			const STY_LoadContext *load_context,
			STY_StyleSheetLoadMode load_mode);
    void	 buildOverrideBindingDataNames();

    UT_StringHolder		 myLabel;
    UT_StringHolder		 myPath;
    STY_TargetHandle		 myTargetHandle;
    STY_OverrideBlockMap	 myOverrideSets;
    UT_StringArray		 myOverrideSetsNames;
    UT_StringArray		 myOverrideBindingDataNames;
    STY_StyleFlags		 myFlags;
    STY_SourceLocation		 mySourceLocation;
};

#endif

