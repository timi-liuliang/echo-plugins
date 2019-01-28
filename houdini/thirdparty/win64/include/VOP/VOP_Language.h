/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_Language.h ( VOP Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __VOP_Language__
#define __VOP_Language__

#include "VOP_API.h"
#include <UT/UT_StringHolder.h>
#include "VOP_Types.h"

class UT_String;
class UT_StringArray;
class UT_StringSet;
class VOP_NodeParmManager;
class VOP_TypeInfo;
class VOP_VarInfo;


/// Reprsents a language for which VOPs can generate source code.
class VOP_API VOP_Language {
public:
    /// Constructor and destructor.
				 VOP_Language();
    virtual			~VOP_Language();

    /// Convenience function to get the default language (VEX).
    static VOP_Language		*getVex();

    /// Returns the name of the language.
    virtual const char		*getName() const = 0;

    /// Returns the enumerated type of this language.
    virtual VOP_LanguageType	 getLanguageType() const = 0;
    bool			 isVex() const;

    /// Gets source code file extension used for code written in this language.
    virtual const char		*getFileExtension() const = 0;

    /// Gets the list of words reserved for special language meaning.
    /// These words cannot be used as variable or function names, etc.
    virtual const UT_StringSet &getReservedWords() const = 0;

    /// Returns the keyword used for shader export (output) parameters.
    virtual const char	*getExportKeyword() const = 0;

    /// Returns the separator character between shader arguments in
    /// the shader function definition.
    virtual const char	*getShaderParmSeparator() const;

    /// Returns the name of formated print function, or NULL if none.
    virtual const char	*getSprintf() const = 0;

    /// Returns the name of the isbound() function, or NULL if none.
    virtual const char	*getIsBoundFunction() const = 0;

    /// Gets the source code snippet that represents a default constant value 
    /// for the variable of a given type.
    virtual void	getEmptyConstantCode(UT_String &code,
				const VOP_TypeInfo &type_info) const = 0;

    /// Get the source code for a default value of a struct given in type_info.
    virtual void	getCustomTypeEmptyConstantCode(UT_String &code,
				const VOP_TypeInfo &type_info) const;

    /// Gets the source code snippet that represents a constant for a variable 
    /// of the given type, whose value is given in the 'value' argument.
    virtual void	getConstantCode(UT_String &code,
				const VOP_TypeInfo &type_info, bool unquoted,
				const UT_StringArray &value) const = 0;
    void		getConstantCode(UT_StringHolder &code,
				const VOP_TypeInfo &type_info, bool unquoted,
				const UT_StringArray &value) const
			{ 
			    UT_String tmp; 
			    getConstantCode(tmp, type_info, unquoted, value);
			    code.adoptFromString(tmp);
			}

    /// @{ Gets type source code keyword for declaring variables of that type.
    virtual void	getTypeCode(UT_String &code,
				const VOP_TypeInfo &type_info) const = 0;
    virtual void	getVaryingTypeCode(UT_String &code,
				const VOP_TypeInfo &type_info) const;
    /// @}

    /// Get the shader contexts available in that language.
    virtual void	getSupportedShaderTypes(VOP_ShaderTypeList &list) const;

    /// Get the source code keyword for the given shader type.
    virtual void	getShaderTypeCode(UT_String &code,
				VOP_ContextType context) const;

    /// Get the source code that declares a variable of the given struct type.
    virtual void	getCustomTypeCode(UT_String &code,
				const VOP_TypeInfo &type_info) const;

    /// Get the source code that defines the given type (eg, struct definition).
    virtual void	getTypeDefinitionCode(UT_String &type_definition,
				const VOP_TypeInfo &type_info) const;

    /// Get the variable member access operator for structs/classes.
    virtual void	getMemberVarAccessOpCode(UT_String &access_op) const;

    /// Returns true if structs and class definitions should include
    /// the default values for their members.
    virtual bool	shouldSpecifyMemberDefaults() const;

    /// Returns the terminator that follows the struct definition (usually
    /// a semicolon or NULL, if terminator is not required.
    virtual void	getStructDefinitionTerminator(UT_String &term) const;

    /// @{ Gets array declaration brackets string for a given type.
    /// If the type is indeed a string, returns "[]", otherwise returns
    /// an empty string "".
    virtual void	getArrayBracketsCode( UT_String &code,
				const VOP_TypeInfo &type_info) const;
    /// @}

    /// Returns true if function parameters are const references by default
    /// and outgoing parameters need an explicit "export" keyword.
    /// Otherwise, returns false (eg for VEX whose function paramerers
    /// are non-const references by default).
    virtual bool	needsFunctionParmExportKeyword() const;

    /// Returns true if it's possible to use shader calls to call a shader 
    /// function for this context.
    virtual bool 	canUseShaderCalls(VOP_ContextType context) const = 0;

    /// Writes out a function call to the buffer.
    virtual void	getFunctionCall( UT_String &str,
				const UT_String &func_name,
				const UT_StringArray &args) const = 0;

    /// Writes out a co-shader call to the buffer.
    virtual void	getShaderCall( UT_String &buffer,
				const UT_String &shader_name,
				const UT_StringArray &arg_names,
				const UT_StringArray &arg_vals) const = 0;

    /// Obtains the list of global variables available in the given context.
    virtual void	getGlobalVariables( UT_Array<VOP_VarInfo> &global_vars,
				VOP_ContextType context) const;

    /// Returns a manager that maps VOP types to PRM node parameters.
    const VOP_NodeParmManager	    &getParmManager() const
				     { return *myParmManager; }

    /// Returns the name that encodes the given language and context.
    /// This name is then targetted by various VOP nodes with a vopnet mask,
    /// to see if these VOPs can show up in that vopnet or not.
    virtual const char *getVopnetMaskTarget(VOP_ContextType context_type) const;

    /// Returns a color name for the connectors of the given type.
    virtual const char *getTypeUIColor(VOP_Type type) const;

    /// @{ Returns true, if the language supports uniform varying variables.
    virtual bool	supportsUniformVarying() const;
    virtual bool	isTypeUniformVarying(VOP_Type type) const;
    /// @}

    /// @{ Take an arbitrary type and return the type supported by the language.
    virtual VOP_Type	conditionType(VOP_Type type) const = 0;
    int			isTypeSupported(VOP_Type type) const
			{ return conditionType(type) == type; }
    /// @}
    
    /// Returns ture if the second type can be assigned directly to first.
    virtual bool	canDirectlyAssign( const VOP_TypeInfo &dst_type,
				const VOP_TypeInfo &src_type ) const;

protected:
    /// Returns an element at a given array index, quoting it if requested.
    void		getStringValue(UT_String &value, 
				const UT_StringArray &values, int index,
				bool unquoted ) const;
private:
    /// Mapper between VOP types and the PRM node parameters.
    VOP_NodeParmManager *	myParmManager;
};

#endif
