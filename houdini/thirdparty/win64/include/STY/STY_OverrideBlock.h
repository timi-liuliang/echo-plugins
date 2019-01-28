/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_OverrideBlock.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_OverrideBlock__
#define __STY_OverrideBlock__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_LoadContext;
class STY_OverrideCategory;
class STY_ResultsFilter;
class UT_JSONValue;

/// Contains a map of category names to override categories.
class STY_API STY_OverrideBlock :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_OverrideBlock>
{
public:
		 STY_OverrideBlock(const UT_JSONValue *json,
			const STY_LoadContext *load_context);
		 STY_OverrideBlock(const STY_OverrideCategoryMap &overrides);
		~STY_OverrideBlock();

    /// Returns true if we have no overrides.
    bool	 isEmpty() const
		 { return myOverrideCategories.empty(); }

    /// Get a list of categories allowed by the filter.
    void	 getMatchingCategories(
			const STY_ResultsFilter &filter,
			UT_StringArray &categories) const;

    /// Get the overrides for a paritcular category.
    STY_OverrideCategoryHandle
		 getOverrideCategory(const UT_StringRef &category) const;

    /// Obtains the list of data names that the override scripts
    /// are interested in for their input argument binding.
    const UT_StringArray &
		 getOverrideBindingDataNames() const;

    /// Save this override set as JSON to a style sheet.
    void	 saveStyleSheets(UT_JSONValue *values,
			STY_TargetMatchClosureHandle *closures,
			const exint num_values,
			bool evaluate_scripts,
			bool allow_shared_scripts) const;

private:
    void	 load(const UT_JSONValue *json,
			const STY_LoadContext *load_context);
    void	 buildBindingDataNames();

    STY_OverrideCategoryMap	 myOverrideCategories;
    UT_StringArray		 myOverrideBindingDataNames;
};

#endif

