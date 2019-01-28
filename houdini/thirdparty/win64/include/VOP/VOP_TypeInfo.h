/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_TypeInfo.h (  Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_TypeInfo__
#define __VOP_TypeInfo__

#include "VOP_API.h"
#include "VOP_Types.h"
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>

class VOP_Language;
class VOP_TypeDefinition;
typedef UT_SharedPtr<const VOP_TypeDefinition>	VOP_TypeDefinitionHandle; 


// ============================================================================
// In general, this is a type meta-name that accepts any struct name.
// An empty string does not collide with custom struct type names and
// also works well with printf() which is used for connector type name tokens.
// The exact meaning is imposed by the use of it:
// - auto-detect and adhoc-unpack (etc) vops specify it as input types,
//   indicating that any struct type is accepted
// - some vops test the incoming wire type, and interpret it as a struct type 
//   yet-unresolved to anything concrete 
#define VOP_STRUCT_TYPE_NAME_ANY		""

// ============================================================================
/// Class abstracting VOP variable and connector types. 
/// Unlike the basic VOP_Type enum, this class can represent and refer to any
/// type, including structs and custom types.
class VOP_API VOP_TypeInfo
{
public:
    /// @{
    /// A constructor and a destructor.
    // NB: explicit keyword is useful during sweeps to convert code from 
    //     VOP_Type to VOP_TypeInfo; after that we can remove the keyword
    explicit	 VOP_TypeInfo( VOP_Type type = VOP_TYPE_UNDEF,
			       const char * type_name = NULL );
		~VOP_TypeInfo();
    /// @}

    /// Utility function that converts VOP_Type into a VOP_TypeInfo.
    /// It is used during transition from VOP_Type to VOP_TypeInfo to mark
    /// the places that should abandon VOP_Type in favour of VOP_TypeInfo, 
    /// but whose conversion is quite involved and likely requires lengthy 
    /// sweeps, and thus is postponed untill later phases.
    // TODO: convert all places that call this function to use VOP_TypeInfo
    //       directly, instead of going through VOP_Type.
    void		setTypeInfoFromType( VOP_Type type )
			{ setType( type ); }

    /// @{
    /// Sets a new type and additional data for the type info.
    void		setType( VOP_Type type,
				 VOP_Type raw_type = VOP_TYPE_UNDEF,
				 const char *type_name = NULL );
    void		setType( VOP_Type type, 
				 const char *type_name );
    /// @}

    /// @{ Returns the  type enumeration value.
    VOP_Type		getType() const
			    { return myType; }
    VOP_Type		getRawType() const
			    { return myRawType; }
    VOP_Type		getBaseType() const
			    { return VOP_BASE_TYPE( myType ); }
    /// @}

    /// Returns the official type name.
    const UT_StringHolder &getTypeName() const
			    { return myTypeName; }

    /// Returns the actual type definition for the type represented by object.
    VOP_TypeDefinitionHandle	getTypeDefinitionHandle() const;

    /// @{ Sets and gets the array length.
    enum		    { UNKNOWN_ARRAY_LENGTH = -1 };
    void		setArrayLength(int length)
			    { myArrayLength = length; }
    int			getArrayLength() const
			    { return myArrayLength; }
    bool		isFixedArrayLength() const
			    { return myArrayLength != UNKNOWN_ARRAY_LENGTH; } 
    /// @}

    /// Returns a type info that represents an undefined type.
    static const VOP_TypeInfo &	    getUndefinedTypeInfo();


    /// @{ Comparison operators.
    bool		operator==( const VOP_TypeInfo &other ) const;
    bool		operator!=( const VOP_TypeInfo &other ) const;
    /// @}

    /// Language will condition the type to an equivalent base type,
    /// which is useful for comparing types to test assignment compatibility
    /// (eg, color and vector). Makes the type and raw type equal to the
    /// type conditioned by the language.
    void		conditionType( const VOP_Language &language );

    /// Returns true if a variable of this type can be directly assigned 
    /// a value of the src_type type, without any explicit conversion.
    bool		canDirectlyAssign( const VOP_TypeInfo &src_type,
			    const VOP_Language *language = nullptr) const;

    /// Returns true if the type is valid.
    bool		isValid() const;

    /// Returns true if the type is an array.
    bool		isArray() const;

    /// Returns the type of the array element, if this type is an array.
    VOP_TypeInfo	getArrayElementTypeInfo() const;

    /// Returns true for sturct and class custom type.
    bool		isStructLike() const;

    /// Returns a type description used for labeling node inputs, outputs, 
    /// and other places with a legible name. Eg, "int", "uniform float", 
    /// "vector array", and "mystruct" (for 'struct mystruct' type).
    void		getDescriptionString( UT_String &description ) const;

    /// Returns a string that encodes the type information into a single-worded
    /// string. Eg, "int", "ufloat", "vectora", "struct_mystruct" (for 'struct
    /// mystruct' type).
    void		getTokenString( UT_String &token ) const;

    /// Sets the type info from the string representation of that type.
    /// The string representation should be compatible with the strings
    /// returned from getDescriptionString() or getTokenString() above.
    /// Note, the struct types can be only deduced from strings compatible
    /// with getTokenString(), or if str is "struct" (unnamed struct).
    void		setFromTokenString( const char *token_or_description );

private:
    /// The basic type of a variable or a connector.
    VOP_Type		myType;

    // The raw (unconditioned) type of the variable or connector. The
    // raw type is not used for VOP_TypeInfo comparison, but is simply a way of
    // doing an exact equivalence check when selecting the most appropriate
    // signature of a scripted VOP.
    VOP_Type		myRawType;

    /// The name of the type.
    /// For simple types it is an empty string (tough it could contain the
    /// official vop type name for simple types too). 
    /// For structs and custom types, it contains the official type name
    /// recognized by the language & type manager, VOP_LanguageManager.
    UT_StringHolder	myTypeName;

    /// The length of the array.
    /// Some languages like OSL require statically determined lengths.
    int			myArrayLength;
};

#endif

