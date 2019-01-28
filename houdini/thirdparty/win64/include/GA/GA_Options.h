/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Options.h ( GA Library, C++)
 *
 * COMMENTS:	Common option names for attributes.  These are the keys used to
 *		lookup values in the UT_Options for each attribute.
 *
 *		Keys should only use [a-z], [A-Z], [0-9] and '_', though
 *		there's no particular rule per se.
 */

#ifndef __GA_Options__
#define __GA_Options__

#include "GA_API.h"
//---------------------------------------------------------------------
//
//		Attribute Creation Options
//
// These options are used when creating attributes.
//---------------------------------------------------------------------

// Specify the "default" values for the attribute.  The value of this default
// changes based on different attribute types.
#define GA_OPTION_DEFAULTS	"defaults"

// The "ordered_group" option can be passed into creation of groups to
// make the group keep order information.
//	bool	"ordered_group"	true|false
//
#define GA_OPTION_ORDERED_GROUP	"ordered_group"

// The "interleaved_defaults" option can be passed into creation
// of indexpair attributes to indicate that the supplied defaults interleave
// indices and values.  Primarily used for loading old .geo/.bgeo files.
//	bool	"interleaved_defaults"	true|false
//
#define GA_OPTION_INTERLEAVED_DEFAULTS	"interleaved_defaults"

// The "tuple_size" option can be specified during creation of certain
// attributes to control the number of components.
//	int	"tuple_size"	3
//
#define GA_OPTION_TUPLE_SIZE	"tuple_size"

// The "storage" option can be specified during creation of certain
// attributes to control the type of storage used for the values.
//	int	"storage"	GA_STORE_REAL32|GA_STORE_INT16|...
//
#define GA_OPTION_STORAGE	"storage"

// The "entry_count" option can be specified during creation of certain
// (e.g. indexpair) attributes to control the number of entries.  Some
// attribute values can consist of "entry_count" tuples, each tuple with
// "tuple_size" components.
//	int	"entry_count"	3
//
#define GA_OPTION_ENTRY_COUNT	"entry_count"

// The "file_load_flag" option can be passed during attribute creation as
// a hint that a GA_AIFFile::load() call will follow.  Attributes can use
// this hint as an opportunity to avoid any initializations that would be
// overridden during the load.
//	bool	"file_load_flag"	true|false
//
#define GA_OPTION_FILE_LOAD_FLAG	"file_load_flag"

//---------------------------------------------------------------------
//
//		Per-Attribute Options
//
// Although any option can be set on a GA_Attribute, these are some common
// names and their meanings.
//---------------------------------------------------------------------

// The "export_on_save" option controls whether an attribute can be saved
// when saving a .geo/.bgeo file.  The default value depends on the scope
// of the attribute: GA_SCOPE_PUBLIC=true, GA_SCOPE_PRIVATE=false
//	bool	"export_on_save"	true|false
//
#define GA_OPTION_EXPORT_ON_SAVE	"export_on_save"

// The "export_on_merge" option controls whether an attribute will be merged.
// The default value depends on the scope of the attribute:
// GA_SCOPE_PUBLIC=true, GA_SCOPE_PRIVATE=false
//	bool	"export_on_merge"	true|false
//
#define GA_OPTION_EXPORT_ON_MERGE	"export_on_merge"

// The "export_to_infoblock" option controls whether a detail attribute
// is saved to the meta-data of a .geo/.bgeo files.  This allows it
// to be retrieved without loading the entire file.  The default is
// false.
#define GA_OPTION_EXPORT_TO_INFOBLOCK	"export_to_infoblock"

// Flags the attribute as non-transforming despite being of point type
#define GA_OPTION_NON_TRANSFORMING 	"attribute:non_transforming"

// Flags the string attribute to store a set rather than unique
#define GA_OPTION_STRINGSET		"blob:stringset"

// It's possible to store options on attributes to help with exporting the data
// to RIB.  The "rsl_type" allows you to store the type information for the RIB
// variable.  The "rsl_offset" and "rsl_length" allow you to extract a
// sub-component of tuples.
// Example:  Pass an attribute as a color to RIB
//	string	"rsl_type"	"uniform color"
//	int	"rsl_offset"	0
//	int	"rsl_length"	3	# Assume RiColor is 3-tuple
//
// Example:  Extract the 'v' component from a 3-tuple as a scalar value
//	string	"rsl_type"	"vertex float"
//	int	"rsl_offset"	1	# Extract 2nd component 
//	int	"rsl_length"	1	# Force length to 1
//
#define GA_OPTION_RSL_TYPE	"rsl_type"
#define GA_OPTION_RSL_OFFSET	"rsl_offset"
#define GA_OPTION_RSL_LENGTH	"rsl_length"

#endif
