/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_TypeDefinition.h (VOP Library, C++)
 *
 * COMMENTS:	Class describes a generic vop type definition for complex types.
 *		A vop type corresponds to the (wire) connector types on nodes.
 *		Simple types don't need a definition, but custom and complex
 *		types like structs need a way to describe their members, etc.
 */

#ifndef __VOP_TypeDefinition__
#define __VOP_TypeDefinition__

#include "VOP_API.h"
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>

class VOP_Language;
class UT_String;
class UT_Color;
class UT_JSONValue;


// ============================================================================
/// A class abstracting definition of a VOP data type.
class VOP_API VOP_TypeDefinition
{
public:
    /// Constructor and destructor.
	     VOP_TypeDefinition();
    virtual ~VOP_TypeDefinition();

    /// Returns what sort of type this is. (eg, "struct", "class", "custom").
    /// It is used in JSON files to identify the type definition implementation
    /// factory and to tell in general the kind of a type.
    virtual UT_StringHolder	getMetaType() const = 0;

    /// @{ Sets and gets the type definition unique name identifier.
    const UT_StringHolder &	getTypeName() const;
    void			setTypeName( const char *name );
    /// @}
    //
    /// @{ Sets and gets the type definition unique name identifier.
    const UT_StringHolder &	getTypeLabel() const;
    void			setTypeLabel( const char *label );
    /// @}

    /// @{ Methods for the color of the connector of this type.
    const UT_Color *		getConnectorColor() const;
    void			setSpecificConnectorColor(const UT_Color &clr);
    void			setDefaultConnectorColor();
    /// @}

    /// @{ Sets and gets the type name used in the generated code.
    void			setCodeTypeName( const char *code_type_name );
    const UT_StringHolder &	getCodeTypeName() const;
    const UT_StringHolder &	getCodeTypeNameRaw() const;
    /// @}

    /// The key in the JSONValue map dictionary that denotes the metatype
    /// of the type definition (e.g., "struct" or "custom").
    static const UT_StringHolder    METATYPE_KEY;

    /// Loads a type definition from a JSON value.
    /// The JSON value parameter is a map (of type JSON_MAP) containing
    /// entries relevant to the type.
    /// Returns true on success, and false on failure.
    virtual bool		load( const UT_JSONValue &json );

    /// Saves a type definition to a JSON value.
    /// The JSON value should be a map (of type JSON_MAP).
    /// Returns true on success, and false on failure.
    virtual bool		save( UT_JSONValue &json ) const;

   
    /// @{ Standard comparison operators.
    bool		operator==( const VOP_TypeDefinition &other ) const;
    bool		operator!=( const VOP_TypeDefinition &other ) const;
    /// @}
    
    /// Obtains the source code that defines the type in the programming
    /// (scripting) language.
    // TODO: this method should be moved to the language class (or language 
    //	    manager) because vop type definition should be language agnostic.
    //	    Any custom vop types should provide mapping between vop type
    //	    and its definition in a specific language.
    virtual void	getTypeDefinitionSourceCode( UT_String &code,
				const VOP_Language *language ) const;

protected:
    /// Returns true if the types are the same, even though their identifier 
    /// name may be different.
    virtual bool	isEqual( const VOP_TypeDefinition &other ) const = 0;
   
private:
    /// Name identifying the vop type definition.
    /// It can be an arbitrary name and does not have to be the same as the
    /// keyword that declares variables of that type.
    /// The name should be unique within a given language.
    UT_StringHolder		myTypeName;

    // Label used for human-readable interfaces. If empty, the name is used.
    UT_StringHolder		myTypeLabel;

    /// Connector color. If NULL, use the default color for this type.
    UT_SharedPtr<UT_Color>	myConnectorColor;

    /// The type name used in the generated code.
    UT_StringHolder		myCodeTypeName;
};

// ============================================================================
/// Define a handle for the type definition objects.
typedef UT_SharedPtr<const VOP_TypeDefinition>	VOP_TypeDefinitionHandle; 

// ============================================================================
/// The simplest form of a custom type implementation.
class VOP_API VOP_CustomTypeDefinition : public VOP_TypeDefinition
{
public:
    /// Constructor and destructor.
	     VOP_CustomTypeDefinition();
    virtual ~VOP_CustomTypeDefinition();

    /// @{ Saves to or loades from a JSON value associated with given key.
    static const UT_StringHolder	METATYPE_VALUE;
    virtual UT_StringHolder		getMetaType() const;
    /// @}

protected:
    /// @{ Standard overrides from the base class
    virtual bool	isEqual( const VOP_TypeDefinition &other ) const;
    /// @}
};

#endif

