/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_LanguageManager.h ( VOP Library, C++)
 *
 * COMMENTS:	Class for managing vop languages, their properties and types.
 */

#ifndef __VOP_LanguageManager__
#define __VOP_LanguageManager__

#include "VOP_API.h"
#include "VOP_Types.h"
#include "VOP_TypeDefinition.h"
#include <UT/UT_Notifier.h>
#include <UT/UT_StringMap.h>
class VOP_LanguageManager;
class VOP_LanguageManagerEvent;
class VOP_TypeDefinitionSource;
class VOP_TypeDefinitionFile;
class VOP_TypeDefinitionFileLoader;
class vop_DefinitionInfo;


// ============================================================================ 
/// Obtains the manager for vop languages and their types.
VOP_API extern VOP_LanguageManager  &VOPgetLanguageManager();


// ============================================================================ 
/// A class for managing the languages and custom data types in VOPs.
class VOP_API VOP_LanguageManager 
{
public:
    /// @{
    /// Default constructor and destructor
     VOP_LanguageManager();
    ~VOP_LanguageManager();
    /// @}


    /// Returns language type guessed from shader's render mask.
    VOP_LanguageType	getLanguageTypeFromRenderMask( const char *mask ) const;
    bool		isMantraVexFromRenderMask( const char *mask ) const;

    /// Returns language of a given type.
    // TODO: return const language
    VOP_Language *	getLanguage( VOP_LanguageType type ) const;

    /// Gets the list of all available type definitions.
    void		getTypeDefinitionNames(
				UT_StringArray &type_names ) const;

    /// Tests if the manager has a definition for the given name.
    bool		hasTypeDefinition( const char *type_name ) const;

    /// Gets the type definition based on the type name.
    VOP_TypeDefinitionHandle
			getTypeDefinitionHandle( const char *type_name ) const;

    /// Gets the source tha provides the full definition for the type.
    VOP_TypeDefinitionSource *
			getTypeDefinitionSource( const char *type_name ) const;



    /// Registers a new vop type whose full definition is provided by 
    /// the source upon request.
    /// If 'type_name' already exists, the new source (and new definition) 
    /// is going to be used after this registration.
    /// If 'old_type_name' is provided, the old type is renamed to the new name 
    /// whose full definition is provided by the given source; after 
    /// renaming, the old name is unregistered and becomes unavailable.
    /// Issues an event about added or renamed type.
    void		registerTypeDefinition( const char *type_name,
				VOP_TypeDefinitionSource &src,
				const char * old_type_name = NULL );

    /// Removes the type from the manager. After unregistration, 
    /// this type definition will no longer be availble from manager.
    /// Issues an event about removed type.
    void		unregisterTypeDefinition( const char *type_name );

    /// A convenience method that unregisters all the types associated
    /// with the given source. Equivalent to calling unregisterTypeDefinition()
    /// on all types registered with the given source (ie, whose definitions
    /// are provided by this source).
    /// Issues events about removed types.
    void		unregisterTypeDefinitionSource( 
				VOP_TypeDefinitionSource &src );

    /// Marks the given type definition as dirty.
    /// The next time someone requests full definition handle, the manager will
    /// ask the source for an update before returning the refreshed definition.
    /// Issues an event about type definition change.
    void		dirtyTypeDefinition( const char *type_name );


    /// Obtains a unique type name not used in the manager yet.
    void		generateUniqueTypeName( UT_String &type_name,
				bool use_guid );


    /// Returns an object that loads and saves the definitions to and from file.
    VOP_TypeDefinitionFileLoader & getFileLoader();


    /// For registering a GUI editor callback.
    typedef void (*EditCallback)(const char *type_name);
    static void		setEditCallback( VOP_LanguageManager::EditCallback cb );
    
    /// Brings up a type editor GUI for editing the given type.
    void		editType( const char *type_name );


    
    /// Gets the notifier that sends events triggered by various changes
    /// to the data structures overseen by the manager.
    UT_NotifierImpl<const VOP_LanguageManagerEvent&> &	
			getEventNotifier()
				{ return myEventNotifier; }

private:
    /// Sets the definition source for the info object, also registering
    /// a deletion interest on the source (to avoid stale pointers).
    void		setDefinitionInfoSource( 
				vop_DefinitionInfo *info,
				VOP_TypeDefinitionSource &src );

    /// @{
    /// Helper methods to find, create, and remove the type definition info 
    /// from the table.
    vop_DefinitionInfo*	    findDefinitionInfo( const char *type_name ) const;
    vop_DefinitionInfo*	    findOrCreateDefinitionInfo( const char *type_name );
    void		    removeDefinitionInfo( const char *type_name );
    /// @}

private:
    /// All defined data types (aka, wire type or loosly variable type) 
    /// for all languages.
    UT_StringMap< vop_DefinitionInfo* >			myDefinitionInfos;

    /// Notifier object for manager's events.
    UT_NotifierImpl<const VOP_LanguageManagerEvent&>	myEventNotifier;

    /// The default loader of definitions stored in files.
    VOP_TypeDefinitionFileLoader *		myFileLoader;

    // The callbak that can be invoked to edit a type in a GUI editor.
    static VOP_LanguageManager::EditCallback 		theEditCallback;
};


// ============================================================================ 
/// A class describing notification events emitted by the language manager.
class VOP_API VOP_LanguageManagerEvent
{
public:
    /// Defines the type of the events that can manager can send out.
    enum EventType
    {
	TYPE_ADDED,	    // a new vop type has been added
	TYPE_CHANGED,	    // a vop type has been updated
	TYPE_REMOVED,	    // a vop type has been removed
	TYPE_RENAMED	    // a vop type has been renamed 
    };

    /// Constructor
    VOP_LanguageManagerEvent( VOP_LanguageManagerEvent::EventType type,
			      const char *vop_type_name,
			      const char *new_vop_type_name = NULL );

    /// @{ Accessors.
    /// Note, if the event type does not involve certain data,
    /// the accessor for that data will return an empty string.
    VOP_LanguageManagerEvent::EventType	getEventType() const;
    const UT_String &			getVopTypeName() const;
    const UT_String &			getNewVopTypeName() const;
    /// @}
    
private:
    /// The type of the event.
    VOP_LanguageManagerEvent::EventType	myEventType;

    /// The soure entity (such as a type name) the event is about. 
    UT_String				myVopTypeName;
    UT_String				myNewVopTypeName;
};

#endif

