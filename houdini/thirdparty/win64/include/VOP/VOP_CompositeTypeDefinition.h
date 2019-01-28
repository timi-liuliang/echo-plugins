/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_TypeDefinition.h (VOP Library, C++)
 *
 * COMMENTS:	Class implementing a composite vop connector types, 
 *		like a struct or a class.
 */

#ifndef __VOP_CompositeTypeDefinition__
#define __VOP_CompositeTypeDefinition__

#include "VOP_API.h"
#include "VOP_TypeDefinition.h"
#include "VOP_TypeInfo.h"
#include <UT/UT_StringArray.h>


// ============================================================================
/// Class that contains information about a struct/class member variable.
class VOP_API VOP_MemberVarInfo
{
public:
    /// Constructor and destructor.
	     VOP_MemberVarInfo();
	     VOP_MemberVarInfo( const VOP_TypeInfo &type_info,
				const char *name, 
				const char *label = NULL,
				const char *old_name = NULL );
	    ~VOP_MemberVarInfo();

    /// Comparison operator.
    bool		    operator==(const VOP_MemberVarInfo &other) const;

    /// @{ Member getters and setters.
    const VOP_TypeInfo &    getTypeInfo() const;
    void		    setTypeInfo( const VOP_TypeInfo &type_info );
    const UT_StringHolder & getName() const;
    void		    setName( const char *name );
    const UT_StringHolder & getLabel() const;
    void		    setLabel( const char *label );
    void		    setOldName( const char *old_name );
    const UT_StringHolder & getOldName() const;
    /// @}

private:
    VOP_TypeInfo	    myTypeInfo;	    // member type
    UT_StringHolder	    myName;	    // member name
    UT_StringHolder	    myLabel;	    // member label

    // The name by which this member was known in the past, in case 
    // some node still refers to it like that.
    UT_StringHolder	    myOldName;
};

// ============================================================================
/// Class that contains information about a struct/class method.
class VOP_API VOP_MemberMethodInfo
{
public:
    /// Constructor and destructor.
	     VOP_MemberMethodInfo();
	    ~VOP_MemberMethodInfo();

    /// Comparison operator.
    bool		    operator==(const VOP_MemberMethodInfo &other) const;

    /// @{ Method getters and setters.
    const UT_StringHolder & getName() const;
    void		    setName( const char *name );
    const VOP_TypeInfo &    getReturnTypeInfo() const;
    void		    setReturnTypeInfo( const VOP_TypeInfo &type_info );
    bool		    isVariadic() const;
    void		    setIsVariadic( bool flag );

    int			    getArgCount() const;
    const UT_StringHolder & getArgName( int idx ) const;
    const VOP_TypeInfo &    getArgTypeInfo( int idx ) const;
    bool		    isArgOut( int idx ) const;
    void		    appendArg( const char *name, 
				    const VOP_TypeInfo &type_info,
				    bool is_out );
    /// @}

private:
    VOP_TypeInfo	    myReturnTypeInfo;	// method return type
    UT_StringHolder	    myName;		// method name
    UT_StringArray	    myArgNames;		// method argument names
    UT_Array<VOP_TypeInfo>  myArgTypes;		// method argument types
    UT_Array<bool>	    myArgOut;		// is output argument 
    bool		    myIsVariadic;	// accepts arbitrary extra args
};

// ============================================================================
/// Class that defines a type composed by assembling other types as members.
class VOP_API VOP_CompositeTypeDefinition : public VOP_TypeDefinition
{
public:
    /// Constructor and destructor.
	     VOP_CompositeTypeDefinition();
    virtual ~VOP_CompositeTypeDefinition();

    /// Returns a keyword to be used in the source code for defining
    /// this composite type (eg, "struct", "class", etc).
    virtual UT_StringHolder	getCodeTypeKeyword() const = 0;
 
    /// @{ Saves to or loades from a JSON value.
    virtual bool		load( const UT_JSONValue &json );
    virtual bool		save( UT_JSONValue &json ) const;
    /// @}

    /// Obtains the source code that defines the type in the programming
    /// (scripting) language.
    virtual void		getTypeDefinitionSourceCode( UT_String &code,
				    const VOP_Language *language ) const;

    /// @{ Getters and setters for member variable information. 
    /// The first three are the core functions, while the remaining ones
    /// are convenience utility function to get at particular piece of info.
    int				    getMemberVarCount() const;
    const VOP_MemberVarInfo &	    getMemberVarInfo( int index ) const;
    const VOP_MemberVarInfo *	    findMemberVarInfo( const char *member_name,
					    bool check_old_names = false) const;
    int				    findMemberVarInfoIndex( 
					    const char *member_name,
					    bool check_old_names = false) const;
    void			    appendMemberVarInfo( 
					    const VOP_MemberVarInfo &info );
    /// @}

    /// @{ Getters and setter for member methods.
    int				    getMethodCount() const;
    const VOP_MemberMethodInfo &    getMethodInfo( int index ) const;
    const VOP_MemberMethodInfo *    findMethodInfo( 
					    const char *method_name ) const;
    void			    appendMethodInfo( 
					    const VOP_MemberMethodInfo &info);
    /// @}
  
    /// @{ True if the type is ad-hoc (eg, defined by a Struct VOP node).
    void		setIsAdHoc( bool flag );
    bool		isAdHoc() const;
    /// @}

    /// @{ Sets and gets the source code that should be used in
    /// the shader definision source code.
    /// If explicit code is NULL, then whenever an actual definition source 
    /// code is needed, this class will auto-generate the it from the info 
    /// about its members. This is the case for majority of vop structs/classes.
    /// This override method is useful for struct definitions that are
    /// outside of vops (eg, in a header file, in which case this override
    /// would likely just specify #include directive).
    void		setExplicitDefinitionCode( const char *code );
    const char *	getExplicitDefinitionCode() const;
    /// @}
   
protected:
    /// @{ Standard overrides from the base class
    virtual bool	isEqual( const VOP_TypeDefinition &other ) const;
    /// @}
    
    /// Generates a type definition for the source code from the member info.
    void		generateTypeDefinition( UT_String &code,
				    const char *meta_type_keyword,
				    const VOP_Language *language ) const; 

private:
    /// True if the type is ad-hoc (eg, defined by a Struct VOP node).
    bool				myIsAdHoc;
    
    /// Composite type member information (their types, names, and labels, etc).
    UT_Array< VOP_MemberVarInfo >	myMemberInfos;

    /// Composite type method information.
    UT_Array< VOP_MemberMethodInfo >	myMethodInfos;

    /// Code that should be used in the shader source code to define this type.
    /// It can contain full type definition or an #include directive.
    /// If it is NULL and the definition is required, the definition 
    /// is generated based on the member variable infos.
    UT_StringHolder 			myExplicitDefinitionCode;
    bool				myUseExplicitDefinitionCode;
};

// ============================================================================
/// Class that defines a struct vop type.
class VOP_API VOP_StructTypeDefinition : public VOP_CompositeTypeDefinition
{
public:
    /// @{ Standard overrides from the base class
	     VOP_StructTypeDefinition();
    virtual ~VOP_StructTypeDefinition();
    /// @}
   
    /// @{ Override the meta-type string for structs
    static const UT_StringHolder	METATYPE_VALUE;
    virtual UT_StringHolder		getMetaType() const;
    virtual UT_StringHolder		getCodeTypeKeyword() const;
    /// @}
};

// ============================================================================
/// Class that defines a class vop type:
/// - in RSL it represents co-shader objects (ie, 'shader' type in source code).
class VOP_API VOP_ClassTypeDefinition : public VOP_CompositeTypeDefinition
{
public:
    /// @{ Standard overrides from the base class
	     VOP_ClassTypeDefinition();
    virtual ~VOP_ClassTypeDefinition();
    /// @}
  
    /// @{ Override the meta-type string for classes.
    static const UT_StringHolder	METATYPE_VALUE;
    virtual UT_StringHolder		getMetaType() const;
    virtual UT_StringHolder		getCodeTypeKeyword() const;
    /// @}
};

#endif

