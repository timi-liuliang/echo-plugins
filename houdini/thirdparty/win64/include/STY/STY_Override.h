/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Override.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Override__
#define __STY_Override__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_UniquePtr.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class UT_JSONValue;
class STY_LoadContext;

/// Describes the way in which a single property override value is calculated.
class STY_API STY_Override :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_Override>
{
public:
		 STY_Override(const UT_JSONValue *json,
			const STY_LoadContext *load_context);
		 STY_Override(const STY_ConstantHandle &value,
			const STY_OverrideScriptHandle &script);
		~STY_Override();

    /// Appends data names that the script is interested in for its input 
    /// argument binding.
    void	 appendBindingDataNames(UT_StringArray &names) const;

    /// Save this override set as JSON to a style sheet.
    void	 saveStyleSheets(UT_JSONValue *values,
			STY_TargetMatchClosureHandle *closures,
			const exint num_values,
			bool evaluate_scripts,
			bool allow_shared_scripts) const;
    /// Creates an array of values from an array of closures.
    bool	 createOverrideValues(UT_Array<STY_ConstantHandle> &values,
			STY_TargetMatchClosureHandle *closures,
			const exint num_values) const;

    /// Direct access to the static override value.
    const STY_ConstantHandle		&getConstant() const
		 { return myConstant; }
    /// Direct access to the override script.
    const STY_OverrideScriptHandle	&getScript() const
		 { return myScript; }

private:
    void	 loadDefinition(const UT_JSONValue *json,
			const STY_LoadContext *load_context);
    void	 loadMapDefinition(const UT_JSONValue *json,
			const STY_LoadContext *load_context);

    STY_ConstantHandle		myConstant;
    STY_OverrideScriptHandle	myScript;
};

#endif

