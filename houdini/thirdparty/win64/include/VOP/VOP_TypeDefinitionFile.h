/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_TypeDefinitionFile.h ( VOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_TypeDefinitionFile__
#define __VOP_TypeDefinitionFile__

#include "VOP_API.h"
#include "VOP_TypeDefinitionSource.h"
#include <OP/OP_OperatorTable.h>    // for base classes
#include <UT/UT_ValArray.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Functor.h>
class UT_StringArray;

// ============================================================================ 
/// Function that creates an instance of a type definition subclass,
/// and a dictionary that maps a definition metatype names to such functions.
typedef UT_Functor<VOP_TypeDefinition *>	VOP_TypeDefinitionFactory;
typedef UT_StringMap<VOP_TypeDefinitionFactory>	VOP_TypeDefinitionFactoryMap;

// ============================================================================ 
/// Represents a source of vop type definitions saved in files or inside HDAs.
class VOP_API VOP_TypeDefinitionFile : public VOP_TypeDefinitionSource
{
public:
    /// Constructor. 
    /// Note, still need to invoke load() before the object can provide info 
    ///about the types defined in that file.
			VOP_TypeDefinitionFile( const char *file_path );

    /// Gets the definition source file path.
    virtual void	getSourcePath( UT_String &path ) const;
    const UT_String &	getFilePath() const;

    /// Loads the necessary data from the file before this class provide
    /// information about type definitions from that file.
    bool	    load( const VOP_TypeDefinitionFactoryMap &factory_map,
			    UT_String &errors );

    /// Saves the current data into the file.
    bool	    save( UT_String &errors ) const;

    /// Returns a type definition object found in the file at load time.
    virtual VOP_TypeDefinitionHandle
		    getTypeDefinition( const char *type_name );


    /// Obtains the list of type definition names provided by this source.
    void	    getTypeDefinitionNames( UT_StringArray &type_names ) const;

    /// Add a new definition to this source.
    void	    addDefinition( VOP_TypeDefinitionHandle type_handle );

    /// Update an old or add a new definition to this source.
    void	    updateOrAddDefinition( const char *type_name,
				VOP_TypeDefinitionHandle type_handle );

    /// Delete the definition from the source.
    bool	    removeDefinition( const char *type_name );

    /// @{ Tests some characteristics of the disk file this source represents.
    bool	    exists() const;
    bool	    canRead() const;
    bool	    canWrite() const;
    /// @}

    /// @{ Allows editing of types it provides in the default editor.
    void	    setAllowsEditingOfTypes( bool flag );
    bool	    allowsEditingOfTypes() const;
    /// @}

private:
    bool	    loadJSON( const VOP_TypeDefinitionFactoryMap &factory_map,
			      UT_ValArray< VOP_TypeDefinition *> &typedefs,
			      UT_String &errors );
    bool	    saveJSON( const UT_Array<VOP_TypeDefinitionHandle> &
				typedefs ) const;

    /// Find a handle pointer within the definition handle list.
    VOP_TypeDefinitionHandle *	findTypeHandle( const char *type_name );
    int				findTypeHandleIndex( const char *type_name );

private:
    /// The file path.
    UT_String			myFilePath;

    /// Allows editor to modify the types and their source.
    bool			myAllowEditingOfTypes;

    /// The list of type definition this source provides.
    UT_Array<VOP_TypeDefinitionHandle>	myTypeDefinitions;
};


// ============================================================================ 
/// A class for handling files and HDAs as sources of vop type definitions. 
class VOP_API VOP_TypeDefinitionFileLoader : public OP_UpdateTableSink,
					     public OP_UpdateTemplatesSink 
{
public:
	     VOP_TypeDefinitionFileLoader( VOP_LanguageManager &mgr );
    virtual ~VOP_TypeDefinitionFileLoader();

    /// Gets the file path where the vop types are saved by default.
    const char *	getDefaultSourceFile() const;
    
    /// Looks for the definition files in the search path and loads them.
    void		loadDefinitionFilesInSearchPath();

    /// Loads the definitions from the specified file.
    bool		loadFile( const char *file_path, UT_String &errors );

    /// Sets a new type definition for a given type name.
    /// @param	old_type_name	The original name of the type to redefine.
    ///		    This should be NULL, if it's a brand new type.
    ///		    If the new definition has a new type name, this
    ///		    parameter contains the original (old) type name
    ///		    that is being renamed.
    /// @param	type_handle	The handle to the type definition object.
    ///		    Manager will use it as the new definition for the type.
    ///		    The definition must have a valid non-empty type name string.
    /// @param	source_path	The file where to save the type definition.
    ///		    If the old definition resided in another file, it is moved
    ///		    to this new file.
    ///		    If NULL, a default file will be used.
    /// @param	errors		An outgoing string parameter that will contain 
    ///		    error messages if manger encounters any problems.
    void		createOrUpdateType( const char *old_type_name, 
				VOP_TypeDefinitionHandle type_handle,
				const char *source_path,
				UT_String &errors );

    /// Deletes the definition from the source.
    bool		deleteType( const char *type_name );

    /// @{ Registers a functor that creates a data type definition object
    /// associated with the given meta type name, when such meta type
    /// is encountered when reading and parsing a JSON file.
    /// After that the created object is given a chance to load itself from 
    /// the definition JSON value.
    void		registerFactory( const UT_StringHolder  &metatype_name,
				const VOP_TypeDefinitionFactory &factory);
    void		unregisterFactory(const UT_StringHolder &metatype_name);
    /// @}

    /// Adds default factories to the map (eg, for "struct" metatype) 
    void		registerDefaultFactories();

    /// Registers interests in tables and ops to monitor HDAs for their
    /// section that may define vop structs.
    void		registerOperatorInterests();

    /// @{ Starts and ends a block of code in which the op table updates
    /// should not be handled.  All the pending actions are performed only
    /// at the time the last endDeferHandlingUpdates() is called.
    void		beginDeferUpdates();
    void		endDeferUpdates();
    /// @}

protected:
    /// @{ Virtual overrides from the base class. Respectively: monitoring 
    /// for any new HDAs that appear in a table, keeping an eye on existing HDAs
    /// for anu updates, and making sure we handle deleted HDA operator types.
    virtual void	tableUpdated(OP_OperatorTable *table);
    virtual void	templatesUpdated( OP_Operator *op );
    virtual void	operatorDeleted( OP_Operator *op );
    /// @}

private:
    /// Helper method that handles an update or removal of operator.
    void		updateDefinitionsAndSourcesFromOp( const char *op_name,
				VOP_TypeDefinitionFile *old_src,
				VOP_TypeDefinitionFile *new_src );
	    

    /// Scans the search path and loads all the definition files found there.
    void		loadDefinitionFiles( const char *file_ext );

    /// Create info objects for the definitions provided by the loaded source.
    void		registerLoadedDefinitions(
					VOP_TypeDefinitionFile *source,
					bool takes_precedence );

    /// @{
    /// Helper methods to find and create a vop type definition srouce 
    /// corresponding to a disk file. 
    /// When takes_precedence is true, any newly loaded type definitions
    /// that conflict with existing types will take precedence; otherwise,
    /// the existing definitions will remain the ones that define the type.
    bool		hasSource( VOP_TypeDefinitionFile *src ) const;
    VOP_TypeDefinitionFile * findSource( const char *path );
    VOP_TypeDefinitionFile * createAndLoadSource(
					const char *path,
					bool takes_precedence,
					UT_String *errors = NULL );
    /// @}

private:
    /// The manager in which we register types defined in files.
    VOP_LanguageManager &	myOwnerMgr;

    /// The default source file path.
    UT_String			myDefaultFileSource;

    /// The known sources of type definitions.
    UT_ValArray< VOP_TypeDefinitionFile * >	myFileSources;

    /// Dictionary that maps an op to a definitions source managed by loader.
    UT_StringMap< VOP_TypeDefinitionFile *>	myOpSourcesMap; 

    /// The map from a definition metatype name (eg "struct") to a function
    /// callback that creates an instance of VOP_TypeDefinition subclass
    /// implementing that type.
    VOP_TypeDefinitionFactoryMap		myTypeDefinitionFactoryMap;

    /// List of deferred op tables with pending update.
    UT_Set<OP_OperatorTable*>	myDeferredUpdateTables;

    /// Nesting level of deferred update handling.
    int				myDeferUpdateCounter;
};


// ============================================================================ 
/// A class for to postpone reloading of VOP type definitions.
/// Sometimes (eg, on startup) there is a slew of HDA load events, each may
/// try to reload and update definitions, but reloading can be safely 
/// performed only once, on the last update, after all HDAs are available. 
/// An instance of this class postpones updates until it goes out of scope.
/// When the last instance goes out of scope, all pending updates are performed.
class VOP_API VOP_AutoDeferTypeUpdates 
{
public:
     VOP_AutoDeferTypeUpdates();
    ~VOP_AutoDeferTypeUpdates();
};

#endif

