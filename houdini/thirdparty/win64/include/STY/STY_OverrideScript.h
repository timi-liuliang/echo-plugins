/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_OverrideScript.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_OverrideScript__
#define __STY_OverrideScript__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_LoadContext;
class STY_ScriptCVEX;
class STY_TargetMatchClosure;
class UT_JSONValue;
class UT_JSONValueMap;
class UT_StringArray;

//  =========================================================================== 
/// Defines a helper interface for resolving CVEX command references.
/// Simply creating one of these objects registers it with the STY library
/// and it will subsequently be given an opportunity to resolve any override
/// script commands.
class STY_API STY_ScriptNodeResolver
{
public:
			 STY_ScriptNodeResolver();
    virtual		~STY_ScriptNodeResolver();

    virtual bool	 resolveScriptNode(const char *node_path,
			    UT_StringHolder &command) const = 0;
};

//  =========================================================================== 
/// Defines a script that computes an override value.
class STY_API STY_OverrideScript :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_OverrideScript>
{
public:
    enum CodeSourceType
    {
	CODE_INVALID,
	CODE_SHARED,
	CODE_FILE,
	CODE_NODE,
	CODE_BINDING,
	CODE_INLINE
    };

    /// Creates an override script object and loads it from the given json.
    /// If load context is not NULL, the script can resolve any references the
    /// script may have (if it has no references, the context is not needed).
			 STY_OverrideScript(const UT_JSONValue *json,
			    const STY_LoadContext *load_context);
			 STY_OverrideScript(
			    const CodeSourceType code_source,
			    const UT_StringHolder &code,
			    const UT_StringHolder &return_variable,
			    const UT_Array<STY_BindingHandle> *bindings);
			~STY_OverrideScript();

    /// Obtains override values for multiple closures in one call. The data
    /// can come back as either STY_Constant or UT_JSONValues.
    bool		 createOverrideValues(
			    UT_Array<STY_ConstantHandle> *opt_values,
			    UT_JSONValue *json_values,
			    const STY_TargetMatchClosureHandle *closures,
			    int num_closures) const;

    /// Appends data names that this script is interested in for its input 
    /// argument binding.
    void		appendBindingDataNames(UT_StringArray &names) const;

    /// Save this override set as JSON to a style sheet.
    void		saveStyleSheet(UT_JSONValueMap *map,
			    bool allow_shared_scripts) const;

    /// Access the attributes that define the script.
    CodeSourceType	 getCodeSource(bool from_import) const;
    const UT_StringHolder &getCode(bool from_import) const;
    const UT_StringHolder &getReturnVariable(bool from_import) const;
    const UT_Array<STY_BindingHandle> *getBindings(bool from_import) const;

    /// Convert CodeSourceType to/from a string equivalent.
    static const UT_StringHolder&codeSourceToStr(CodeSourceType code_source);
    static CodeSourceType	 codeSourceFromStr(const UT_StringHolder &str);

private:
    /// Suffix for script function name to make sure it is unique.
    const char		*getMangleSuffix() const;

    // Helper methods for running the script.
    void		 declareInputBindings(STY_ScriptCVEX &cvex,
			    const STY_TargetMatchClosureHandle *closure) const;
    void		 bindInputData(STY_ScriptCVEX &cvex,
			    const STY_TargetMatchClosureHandle *closure) const;

    void		 saveStyleSheet(UT_JSONValueMap *map,
			    const UT_Array<STY_BindingHandle> *bindings,
			    const CodeSourceType code_source,
			    const UT_StringHolder &code,
			    const UT_StringHolder &return_variable,
			    bool allow_shared_scripts) const;

    CodeSourceType				 myCodeSource;
    UT_StringHolder				 myCode;
    UT_StringHolder				 myReturnVariable;
    UT_UniquePtr<UT_Array<STY_BindingHandle> >	 myBindings;
    UT_UniquePtr<UT_Array<STY_BindingHandle> >	 myAutomaticBindings;
    STY_OverrideScriptHandle			 myImportedScript;
    UT_StringHolder				 myMangleSuffix;
};

#endif

