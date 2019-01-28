/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_ImportUtils.h ( PI Library, C++)
 *
 * COMMENTS:
 *
 *      This class encapsulates some useful utility methods for dealing
 *      with import sources in a PI_EditScriptedParm.
 */

#ifndef __PI_ImportUtils_h__
#define __PI_ImportUtils_h__

#include "PI_API.h"

class UT_String;
template<typename T>
class UT_SymbolMap;
class UT_WorkBuffer;
class OP_Node;
class PRM_ScriptPage;
class PRM_Parm;
class PRM_ScriptParm;
class PRM_Template;

#define PI_IMPORTBLOCK_TOKEN	"importblock"

enum PI_ImportType
{
    PI_IMPORT_TYPE_UNKNOWN,
    PI_IMPORT_TYPE_OP,
    PI_IMPORT_TYPE_FILE
};

class PI_API PI_ImportUtils
{
public:

    static PI_ImportType	queryImportType(const char *source);

    static void			buildFolderToken(UT_WorkBuffer &buffer,
						 const char *switcher,
						 const char *folder);

    // Parse the given source string and extract the src node path.  The
    // caller is responsible for hardening the string.  The node referenced
    // in the source string, if found, is returned.
    static OP_Node	*parseOpSource(const char *source,
				       const OP_Node *relative_to,
				       UT_String &src_node_path);

    // Build the appropriate import source string given the arguments.
    static void		 buildOpSource(UT_WorkBuffer &buffer,
				       const char *rel_node_path);

    // This method parses the given source string, and returns whether it
    // refers to the specified relative node path and the given source token
    // belongs to parm_table (which maps parameter names to PRM_Parm pointers).
    // Folders are rejected as valid sources if they're flagged as not being
    // available for import.  Regular parameters not available for import
    // should not be in the parm_table.  Finally, this method will also reject
    // any parameters that have changed either into or from a multiparm, an
    // operation unsupported by a simple update.
    static bool		 validateOpSource(const char *source, const char *token,
					  bool is_folder, bool is_multiparm,
					  const char *src_node_rel_path,
					  const UT_SymbolMap<PRM_Parm *> &parm_table);

    // This method checks flags on the supplied parm to determine if we can
    // import it.  The subindex is used only for switcher parms to query a
    // specific folder.
    static bool		 isAvailableForImport(const PRM_Template *templ,
					      int subindex = -1);
    static bool		 isAvailableForImport(const PRM_Parm *parm,
					      int subindex = -1);

    // Parse the given source string and extract the src file path.  The
    // caller is responsible for hardening the returned string.
    static bool		 parseFileSource(const char *source, UT_String &file);

    // Build the appropriate import source string given the arguments.
    static void		 buildFileSource(UT_WorkBuffer &buffer,
					 const char *file_path);

    static PRM_ScriptPage	*parseImportFile(const char *file);

    // This method parses the given source string, and returns whether it
    // refers to the specified file and the given source token belongs to
    // parm_table (which maps parameter names to PRM_ScriptParm pointers).
    // Finally, this method will also reject any parameters that have
    // changed either into or from a multiparm, an operation unsupported by
    // a simple update.
    static bool		 validateFileSource(const char *source,
					    const char *token, bool is_folder,
					    bool is_multiparm, const char *file,
					    const UT_SymbolMap<PRM_ScriptParm *> &parm_table);
};

#endif
