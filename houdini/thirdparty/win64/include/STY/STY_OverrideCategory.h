/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_OverrideCategory.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_OverrideCategory__
#define __STY_OverrideCategory__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_StringMap.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_LoadContext;
class STY_TargetMatchClosure;
class UT_JSONValue;

/// Contains a map of property names to the STY_Override objects that specify
/// the property values.
class STY_API STY_OverrideCategory :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_OverrideCategory>
{
public:
		 STY_OverrideCategory(const UT_JSONValue *json,
			const STY_LoadContext *load_context);
		 STY_OverrideCategory(const STY_OverrideMap &overrides);
		~STY_OverrideCategory();

    /// Return a reference to our overrides map.
    const STY_OverrideMap &getOverrides() const
		 { return myOverrides; }

    /// Gets data names that the overrde scripts are interested in 
    /// for their input argument binding.
    void	 getBindingDataNames(UT_StringArray &names) const;

    /// Save this override set as JSON to a style sheet.
    void	 saveStyleSheets(UT_JSONValue *values,
			const UT_StringHolder &category,
			STY_TargetMatchClosureHandle *closures,
			const exint num_values,
			bool evaluate_scripts,
			bool allow_shared_scripts) const;

private:
    /// Parses JSON data and fills out the overrides data structure.
    void	 loadDefinitions( const UT_JSONValue *json,
			const STY_LoadContext *load_context );

    STY_OverrideMap	myOverrides;
};

#endif

