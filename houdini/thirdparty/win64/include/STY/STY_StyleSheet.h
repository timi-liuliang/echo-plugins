/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_StyleSheet.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_StyleSheet__
#define __STY_StyleSheet__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_Array.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Lock.h>

class STY_Material;
class STY_OverrideScript;
class UT_JSONWriter;
class UT_JSONValue;
class UT_IStream;

/// Style sheet definition that contains style entries (an entry specifies the
/// target styling subject and overrides that need to be applied to the
/// properties of that subject. It also contains extra data needed for 
/// successful overrides, such as material definitions.
class STY_API STY_StyleSheet :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_StyleSheet>
{
public:
    /// Constructor and destructor.
    explicit		 STY_StyleSheet(const UT_StringHolder &source_location,
				STY_StyleSheetLoadMode load_mode);
    explicit		 STY_StyleSheet(const char *json_str,
				const UT_StringHolder &source_location,
				STY_StyleSheetLoadMode load_mode);
    explicit		 STY_StyleSheet(const UT_JSONValue &json_value,
				const UT_StringHolder &source_location,
				STY_StyleSheetLoadMode load_mode);
    explicit		 STY_StyleSheet(
				const UT_StringArray &import_files,
				const UT_Array<STY_StyleHandle> &styles,
				const STY_MaterialMap &materials,
				const STY_OverrideScriptMap &scripts,
				const STY_OverrideBlockMap &override_sets,
				STY_StyleSheetFlags flags);
			~STY_StyleSheet();

    /// Utility function to track internally defined style sheets, to allow
    /// one internal style sheet to reference another, and to treat internally
    /// defined style sheet on par with external files.
    static void		 setRegisteredStyleSheetMap(
				const UT_StringMap<UT_StringHolder> *
				stylesheet_map);

    /// Primitive/detail string attribute name that can define a style sheet.
    static const UT_StringHolder &
			 getDefAttributeName();
    /// Object parameter name that can define a style sheet.
    static const UT_StringHolder &
			 getDefParmName();

    /// Object parameter label in the data tree
    static const UT_StringHolder &
			 getDefParmLabel();

    /// Get any errors encounterd during the parsing of the stylesheet.
    const UT_WorkBuffer &getErrors() const
			 { return myErrors; }

    /// Get the style entries defined by this style sheet.
    const UT_Array<STY_StyleHandle> &
			 getStyles() const
			 { return myStyles; }

    /// Gets all material definitions provided by the style sheet.
    const STY_MaterialMap &
			 getMaterialDefinitions() const
			 { return myMaterials; }

    /// Gets all shared script definitions provided by the style sheet.
    const STY_OverrideScriptMap &
			 getScripts() const
			 { return myScripts; }

    /// Gets all material definitions provided by the style sheet.
    const STY_OverrideBlockMap &
			 getOverrideSets() const
			 { return myOverrideSets; }

    /// Gets a list of imported style sheet files.
    const UT_StringArray &
			 getImportFiles() const
			 { return myImportFiles; }

    /// Gets the flags for this style sheet.
    STY_StyleSheetFlags	 getFlags() const
			 { return myFlags; }

    /// Saves the style sheet as text in JSON format.
    void		 saveStyleSheet(UT_WorkBuffer &stylesheet) const;

private:
    /// Parses the JSON definition of the style sheet and builds internal data.
    void		 loadStyleSheetString(const char *json_str,
				const UT_StringHolder &source_location,
				STY_StyleSheetLoadMode load_mode);
    void		 loadStyleSheetFile(const UT_StringHolder &file_name,
				STY_StyleSheetLoadMode load_mode);
    void		 loadStyleSheet(UT_IStream &is,
				const UT_StringHolder &file_name,
				STY_StyleSheetLoadMode load_mode);
    void		 loadStyleSheet(const UT_JSONValue &json_value,
				const UT_StringHolder &file_name,
				STY_StyleSheetLoadMode load_mode);

    /// A list of imported files (assuming we dont parse them on load).
    UT_StringArray				myImportFiles;
    STY_StyleSheetLoadMode			myLoadMode;

    /// A style contains the target specification and value overrides.
    UT_Array<STY_StyleHandle>			myStyles;

    /// Style sheet can define materials used in value overrides.
    STY_MaterialMap				myMaterials;

    /// Style sheet can define scripts referenced in override scripts.
    STY_OverrideScriptMap			myScripts;

    /// Style sheet can define override sets referenced in styles.
    STY_OverrideBlockMap			myOverrideSets;

    /// Holds errors generated during constuction/parsing.
    UT_WorkBuffer				myErrors;

    /// A set of all file names imported while loading this style sheet.
    UT_StringSet				myStyleSheetFiles;

    /// Flags that determine styler behavior.
    STY_StyleSheetFlags				myFlags;
    bool					myFlagsSet;
};

#endif

