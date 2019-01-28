/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_SpareProperty.h ( PI Library, C++)
 *
 * COMMENTS:	Spare properties provide a way to specify UI for well defined
 *		spare parameters.  The spare properties load in dialog script
 *		files, defining parameters.  There's a hierarchy to the
 *		parameters.
 *
 *		There are different "classes" of properties (i.e.  properties
 *		for mantra9.0 or prman12.5).  The class is defined by the
 *		name/label of the dialog script.
 *
 *		Each parameter also has a "category".  The parameters are
 *		bundled into their categories.  When the spare property is
 *		added to an operator, the property will automatically be
 *		shuffled into a folder with the given category label.
 *
 *		Categories are specified by a parmtag "category" in the dialog
 *		script.
 *
 *		If the category is an empty string (""), then the parameter
 *		will not be considered to be a property, but rather just a
 *		place holder.  Place holders can be inserted between groups of
 *		parameters to avoid the automatic aglomeration of groups
 *		performed by dialog script parsing.  They can also just be
 *		placeholders.
 *
 *		Sometimes, parameters come in "bundles".  Parameters which
 *		belong together.  Groups in the dialog script are used for this
 *		purpose.  The parmtag specifying the category should be put on
 *		the group.  All parameters in the group will be added
 *		simultaneously when the user adds the group.
 *
 *		The help-tag stored on parameters is accessible.
 */

#ifndef __PI_SpareProperty__
#define __PI_SpareProperty__

#include "PI_API.h"

class UT_WorkBuffer;
class UT_StringArray;
class UT_String;
class PRM_ScriptParm;
class OP_Node;
class PI_EditScriptedParms;

typedef enum {
    PI_SPARE_PROPERTY_SKIPFILTER,
    PI_SPARE_PROPERTY_USEFILTER,
} PI_SPARE_PROPERTY_FILTER;

typedef enum {
    PI_SPARE_PROPERTY_ASIS,		// Put spare parms at end of folder
    PI_SPARE_PROPERTY_REPACKAGE,	// Re-package original parameters
} PI_SPARE_PROPERTY_PACKAGE;

/// @brief Specifies collections of pre-defined spare parameter definitions
///
/// This class manages all the spare property lists.  These property lists are
/// used by Houdini to quickly add pre-defined parameters to parameter dialogs.
///
/// Parameters are collected into "classes".  These class collections store
/// parameters that have a common purpose.  These classes correspond to the
/// parameter definitions in $HH/soho/parameters.
///
/// Each class has a set of "categories".  Each category corresponds to a
/// folder of properties.  For example, "Shading".
///
/// So, the hierarchy is:  class.category.property
///
class PI_API PI_SpareProperty {
public:
    /// If the class_pattern is nil, the list of classes will be returned.
    /// If the category is nil, the list of categories for matching classes
    /// will be returned.
    ///
    /// All parameters which are contained within the class pattern and
    /// category pattern will be returned.  If the quiet flag is false, the
    /// associated category of each parameter will also be printed, otherwise,
    /// just the parameter name will be listed.
    static void		 list(UT_String &errors, UT_WorkBuffer &wbuf,
					const char *class_pattern = 0,
					const char *category = 0,
					bool quiet = false);

    // Create a new spare property (or properties) for the node
    // Top level folder is the folder to put the parameter (and folder) in.  If
    // the top_level_folder is null, the folder will be added to the top level
    // switcherr (or one will be created).
    static bool		 addProperty(OP_Node *node,
					const char *top_level_folder,
					const char *class_pattern,
					const char *parameter,
					UT_String &errors,
					PI_SPARE_PROPERTY_FILTER ignore_filter,
					PI_SPARE_PROPERTY_PACKAGE repackage,
					const char *category_override=nullptr);
    static bool		 addProperty(PI_EditScriptedParms &editparms,
					const char *top_level_folder,
					const char *class_pattern,
					const char *parameter,
					UT_String &errors,
					OP_Node *node,
					PI_SPARE_PROPERTY_FILTER ignore_filter,
					PI_SPARE_PROPERTY_PACKAGE repackage,
					const char *category_override=nullptr);

    // Initialize the spare properties by loading in all the dialog scripts.
    // Calling initialize multiple times is ok.  Each call will re-load the
    // dialogs (i.e. dsreload)
    static bool		initialize(UT_String &errors);

    // Adding a class file will add a path to the list of files scanned for
    // properties.  Thus, it's possible to add a dialog script after
    // initialization.  This method will automatically call initialize() and
    // return errors from the initialization.
    static bool		addClassFile(UT_String &errors,
					const char *path, int pathvar=-1);

    /// Get a list of the class files
    static int		getClassFiles(UT_StringArray &list);

    /// Get a list of all the classes defined by the files
    /// Each class has a list of tags associated with it (defined in the .ds
    /// file).  You can filter the class selection based on the tag expression.
    static int		getClasses(UT_StringArray &list,
				    const char *filter_tags="*");

    /// Each parameter class as properties.
    /// This method returns the "label" for the class.
    static const char	*getClassLabel(const char *name);

    // Checks whether or not the given name is a class
    static bool		isClass(const char *name);

    // Get a list of all the categories for a given class pattern
    static int		getCategories(UT_StringArray &list,
					const char *class_pattern);

    // Get a list of all the parameters for a given a class and category
    // pattern.
    static int		getParameters(UT_StringArray &list,
					const char *class_pattern,
					const char *category);

    /// @{
    /// Populate `parms` with a set of parameters representing a class, or a
    /// category, or the properties in a category.
    static void		getClassAsParameters(
			    PI_EditScriptedParms &parms, 
			    const char *class_name);
    static void		getCategoryAsParameters(
			    PI_EditScriptedParms &parms,
			    const char *class_name,
			    const char *category_name);
    static void		getCategoryPropertiesAsParameters(
			    PI_EditScriptedParms &parms, 
			    const char *class_name,
			    const char *category_name);
    /// @}
    
    // Get a pointer to the PRM_ScriptParm that defines a property
    static const PRM_ScriptParm	*getParmDefinition(const char *classname,
					const char *parameter);

    // Get the name of the class that owns the specified property
    static bool		getClassName(const PRM_ScriptParm *property,
					UT_String &classname);
};

#endif
