/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OpUtils.h ( UT Library, C++)
 *
 * COMMENTS:	Utility functions for basic manipulation of operator type names.
 */

#ifndef __UT_OpUtils__
#define __UT_OpUtils__

#include "UT_API.h"

class UT_String;
class UT_StringArray;
class UT_WorkBuffer;

class UT_API UT_OpUtils
{
public:
    /// Utility function to combine an index file path and a section name into 
    /// a section path according to the standard convention (using 
    /// UT_SECTION_SEPARATOR_CHAR, '?', as a separator). 
    /// E.g.,
    /// "/path/to/index_file" and "section_name" -->
    /// "/path/to/index_file?section_name".
    /// Note, some non-standard index file file protocols may use '?' for 
    /// other purposses than separating a section name (eg "http:/foo?bar=baz"),
    /// and this function should not be used for such paths.
    static void		 combineStandardIndexFileSectionPath( 
					UT_String &source_section_path,
					const char *index_file_path,
					const char *section_name);

    /// Utility function to split the source path into an index file path
    /// and a section name, if the source indeed refers to an index file section
    /// (according to the standard convention of using 
    /// UT_SECTION_SEPARATOR_CHAR, '?', as a separator).
    /// If source_section_path refers to a section, then index_file_path is set
    /// to the index file and section_name to the name. 
    /// If source_section_path does not refer to any section, then
    /// index_file_path is set to the original path and section_name to an empty
    /// string.
    /// Note, some non-standard index file file protocols may use '?' for 
    /// other purposses than separating a section name (eg "http:/foo?bar=baz"),
    /// and this function should not be used for such paths.
    /// Returns true if the original path indeed contains a section name.
    static bool		 splitStandardIndexFileSectionPath( 
					const char *source_section_path,
					UT_String &index_file_path,
					UT_String &section_name);

    /// @{
    /// Utility functions to check if the path starts with standard prefixes.
    static bool		 startsWithOpDefPrefix(const char *path);
    static bool		 startsWithOpLibPrefix(const char *path);
    /// @}

    /// @{
    /// Returns a path that refers to an index file section within an
    /// OTL library or within an HDA definition depending on the path prefix.
    /// For example, the path may be "opdef:/Object/hda?section" or
    /// "opdef:/artistA::Object/hda::2.0?section, etc, and for 
    /// nested sections with section array containing ["outer.idx", "inner"],
    /// the path may be "opdef:/Object/hda?outer.idx?inner". 
    /// @param[out]	path	Will contain the path pointing to the section.
    /// @param[in]	prefix	The prefix specifying the type of the main
    ///			    index file (OTL or HDA), and should be either
    ///			    UT_OTL_LIBRARY_PREFIX (ie, "oplib:") or
    ///			    UT_HDA_DEFINITION_PREFIX (ie, "opdef:"). 
    /// @param[in]	table_and_opname    The operator type name including
    ///			    the table name. This specifies the operator
    ///			    type OTL library or HDA definition that contains 
    ///			    the section. This argument value can be obtained 
    ///			    from combineTableAndOpName() method.
    /// @param[in]	section_name	A string that specifies the section 
    ///			    name that the final path is supposed to refer to. 
    ///			    This is the section directly inside the OTL library
    ///			    or HDA definition index file.
    /// @param[in]	section_names	A string array that specifies a sequence
    ///			    of sections, if the final innermost section 
    ///			    is nested within other sections (that themselves 
    ///			    are index files). For such nested sections, 
    ///			    the outer sections (the ones that are index
    ///			    files themselves) are earlier in the array, and 
    ///			    the innermost section is at the end of the array.
    static void		 combineOpIndexFileSectionPath(UT_String &path,
					const char *prefix,
					const char *table_and_opname,
					const char *section_name );
    static void		 combineOpIndexFileSectionPath(UT_String &path,
					const char *prefix,
					const char *table_and_opname,
					const UT_StringArray &section_names );
    /// @}

    /// @{
    /// A fuction to split the original opdef or oplib source path into the 
    /// the prefix, table-and-opname, and section name components.
    /// The path should start with either UT_OTL_LIBRARY_PREFIX (ie, "oplib:"),
    /// or UT_HDA_DEFINITION_PREFIX (ie, "opdef:").
    /// Eg,
    /// "oplib:/Object/hda?Object/hda" -> "oplib:", "Object/hda", "Object/hda"
    /// "opdef:/Object/hda?section" -> "opdef:", "Object/hda", "section"
    /// "opdef:.?section" -> "opdef:", ".", "section"
    /// "opdef:/Sop/hda?outer?inner" -> "opdef:", "Sop/hda", ["outer", "inner"]
    ///
    /// @param[in]  path  The original source path to split.
    /// @param[out] prefix The prefix specifying the type of the main
    ///			index file (OTL or HDA), and will be either
    ///			UT_OTL_LIBRARY_PREFIX (ie, "oplib:") or
    ///			UT_HDA_DEFINITION_PREFIX (ie, "opdef:"). 
    /// @param[out]  table_and_opname    The operator type name including
    ///			the table name. This will specifie the operator
    ///			type OTL library or HDA definition that contains 
    ///			the section. This argument can be further  
    ///			parsed with splitTableAndOpName() method.
    /// @param[out] section_name    If the original source path is expected
    ///			to contain only one section (ie, no nested section)
    ///			then this string parameter will contain the name of 
    ///			that section. It is an error to use it for paths with
    ///			nested sections, and the string array argument should
    ///			be used instead.
    /// @param[out] section_names    If the original source path refers to 
    ///			a section that is nested within other
    ///			sections (that themselves are index files), then
    ///			this parameter is set to these section names, with 
    ///			the outermost section at the beginning of the array
    ///			and the innermost section (contained within the outer
    ///			sections) at the end of the array. If there is just
    ///			a single section, the array will contain a single
    ///			entry (the single section name)
    static void		 splitOpIndexFileSectionPath(const char *path,
					UT_String *prefix,
					UT_String *table_and_opname,
					UT_String *section);
    static void		 splitOpIndexFileSectionPath(const char *path,
					UT_String *prefix,
					UT_String *table_and_opname,
					UT_StringArray &section_names );
    static void		 splitOpIndexFileSectionPath(const char *path,
					UT_WorkBuffer *prefix,
					UT_WorkBuffer *table_and_opname,
					UT_StringArray *section_names );
    /// @}


    /// Returns the official name of the OTL library section that contains 
    /// the HDA definition. Eg "Object/hda", or "artistA::Object/hda::2.0"
    static void		 getOpTypeDefSectionName(UT_String &sectname,
					     const char *table_name,
					     const char *op_name);

    /// @{
    /// Obtains a full name with a table from a table name and op name (which 
    /// may include namespaces and version tags, but not a table name).
    /// Eg, "Object" and "artistA::hda::2.0" -> "artistA::Object/hda::2.0"
    static void		 combineTableAndOpName(UT_WorkBuffer & table_and_opname,
					 const char *table_name,
					 const char *op_name);
    static void		 combineTableAndOpName(UT_String & table_and_opname,
					 const char *table_name,
					 const char *op_name);
    /// @}

    /// @{
    /// Obtains a table name and an operator name (which may include namespaces
    /// and version tags, but not a table name) from a string that contains
    /// a full opname with table name included in it.
    /// If the table-and-opname has no slash, the table_name will be empty.
    /// Eg, "artistA::Object/hda::2.0" -> "Object" and "artistA::hda::2.0" 
    static void		 splitTableAndOpName(const char *table_and_opname,
					    UT_String &table_name,
					    UT_String &op_name);
    static void		 splitTableAndOpName(const char *table_and_opname,
					    UT_WorkBuffer &table_name,
					    UT_WorkBuffer &op_name);
    /// @}

    /// @{
    /// Build a full operator type name string that includes a scope operator, 
    /// namespace, op name, and version tags, out of the (separate) components 
    /// given by the arguments.
    /// The caller can pass NULL for the components that don't apply.
    /// E.g.,
    /// NULL, "artistA", "my_op", and "2.0" -> "artistA::my_op::2.0" and
    /// NULL, "artistA", "Object/my_op", and "2.0" -> 
    ///	    "artistA::Object/my_op::2.0" 
    /// "Object/light", NULL, "Sop/mybulb", NULL -> "Object/light::Sop/mybulb" 
    /// "vA::Object/outer::1.0", "vB", "Sop/inner", and "2.0" ->
    ///	    "vA::Object/outer::1.0::vB:Sop/inner::2.0" 
    static void		 getFullNameFromComponents(UT_WorkBuffer & full_name,
					    const char *scope_opname,
					    const char *namespace_name,
					    const char *op_name,
					    const char *version);
    static void		 getFullNameFromComponents(UT_String & full_name,
					    const char *scope_opname,
					    const char *namespace_name,
					    const char *op_name,
					    const char *version);
    /// @}

    /// @{
    /// Extract the components from a full operator type name. 
    /// The full operator name may include a scope operator (ie, a subnet 
    /// operator only in which 'op_name' operator can be created), 
    /// namespaces and version tag.
    /// The caller can pass NULL for the components that it's not interested in.
    /// E.g.,
    /// "artistA::my_op::2.0" -> NULL, "artistA", "my_op", and "2.0" 
    /// "artistA::Object/my_op::2.0" -> NULL, "artistA", "Object/my_op", "2.0"
    /// "Object/light::Sop/mybulb" -> "Object/light", NULL, "Sop/mybulb", NULL
    /// "aA::Object/outer::1.0::aB:Sop/inner::2.0" -> "aA::Object/outer::1.0",
    ///	    "aB", "Sop/inner", and "2.0".
    static void		 getComponentsFromFullName(const char *full_name, 
					    UT_WorkBuffer *scope_opname,
					    UT_WorkBuffer *namespace_name,
					    UT_WorkBuffer *op_name,
					    UT_WorkBuffer *version);
    static void		 getComponentsFromFullName(const char *full_name, 
					    UT_String *scope_opname,
					    UT_String *namespace_name,
					    UT_String *op_name,
					    UT_String *version);
    /// @}

    /// @{
    /// Makes sure the filename is valid on all platforms by converting
    /// any invalid characters such as slashes and colons to dashes.
    /// Eg, "ns::Object/hda::1.0" -> "ns-Object-hda-1.0".
    /// If the file name is already valid, it remains unchanged.
    static void		 ensureValidOpBasedFileName(UT_String &filename);
    static void		 ensureValidOpBasedFileName(UT_WorkBuffer &filename);
    /// @}
};

#endif

