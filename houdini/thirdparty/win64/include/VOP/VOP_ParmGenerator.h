/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 * COMMENTS:	ParmGenerator VOP operator.
 *
 */

#ifndef __VOP_ParmGenerator_h__
#define __VOP_ParmGenerator_h__

#include "VOP_API.h"
#include "VOP_Node.h"
#include <CH/CH_ExprLanguage.h>
#include <UT/UT_StringHolder.h>

class PI_EditScriptedParm;
class VOP_CodeVarMapperContext;
class VOP_ExportedParmsManager;

class VOP_API VOP_ParmGenerator : public VOP_Node
{
public:
    // Get the "isbound" code, if it's needed.
    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual unsigned		 getNumVisibleInputs() const;
    virtual bool		 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &ctx, 
					bool check_shader_context) const;

    virtual const char		*outputLabel(unsigned idx) const;
    virtual bool		 areOutputVariablesFixed(
					const VOP_CodeGenContext &context);
    using VOP_Node::areOutputVariablesFixed;
    virtual void		 getFixedOutputVariable(UT_String &var,int idx);

    virtual bool		 getIsParmGenerator() const;
    virtual const VOP_ParmGenerator *castToParmGenerator() const { return this;}
    virtual VOP_ParmGenerator *castToParmGenerator() { return this; }

    virtual void		 initializeNode();
    virtual VOP_Node		*getRealDefinition();

    virtual void		 opChanged(OP_EventType reason, void *data=0);

    /// @{ Obtains the scope for this parameter.
    static const char* const	 SCOPE_SHADER;	    // shader parm (on SHOP)
    static const char* const	 SCOPE_CLASS;	    // class member variable
    static const char* const	 SCOPE_METHOD;	    // method argument
    static const char* const	 SCOPE_SUBNET; // subnet input (and output)

    bool	isSubnetParameterDefiner(VOP_Node *subnet = nullptr) const;
    bool	isSubnetInputOrOutputDefiner(VOP_Node *subnet = nullptr) const;
    bool	isSubnetInputDefiner( VOP_Node *subnet = nullptr,
			bool allow_search = false) const;
    bool	isSubnetOutputDefiner( VOP_Node *subnet = nullptr,
			bool allow_search = false) const;
    bool	isSubnetInputParm() const;
    bool	isSubnetOutputParm() const;
    /// @}

    /// @{ Menu options for this parameter.
    static const char* const	 NO_MENU;
    static const char* const	 MENU_ITEMS;
    static const char* const	 MENU_SCRIPT;
    /// @}
   
    /// Returns true if parm node is used for shader argument.
    bool			 isShaderParm() const;
    bool			 isClassMemberParm() const;

    /// Gets the access level (public, private, etc) for this variable.
    void			 getParmAccess( UT_String &access );

    /// Checks if this node is inside a shader of the given type.
    bool			 isInsideShaderOfType( VOP_Type shader_type );

    /// Returns true if this node represents a subnet input.
    virtual bool		 isSubnetInput() const;

    /// Returns the type of a subnet input represented by this node.
    virtual VOP_TypeInfo	 getSubnetInputTypeInfo();

    /// Obtains a list of shader context types that this node is interested in
    /// supporting. 
    virtual void		 getContextsForCodeGeneration(
					UT_StringArray &context_names ) const;

    // Does a search through the network to find the node that defines
    // the parameter for node. This function updates the local symbol
    // table with the appropriate value.
    void			 recalculateParmDefiner(bool sendchangeevents);

    // Calls PARMNAME() to make sure that our parm name cache value is
    // up to date.
    void			 cacheParmName();
    // Returns ths cached evaluation of the parameter name parm. This
    // is useful because this parameter is evaluated extremely often.
    const UT_StringHolder	&getParmNameCache() const
				    { return myParmNameCache; }

    /// Get the variable name for this parameter, as it is used in code.
    void			 getParmCodeName( UT_String &name ) const;

    /// Returns parm label.
    void			 parmLabel(UT_String &str);

    /// Returns true if it's an exported parameter.
    bool			 isExportParm() const
				 { return exportParm(); }

    /// Returns the real parameter definer for this node.
    VOP_ParmGenerator		*getParmDefiner() const;
    VOP_ParmGenerator		*getParmDefiner(const char *parm_name,
						bool for_shader) const;

    // Fills the supplied array with a list of all channels that will be
    // created for the parameter defined by this node.
    void			 getParmChannels(UT_StringArray &channels);

    // Parm generators are special - they may or may not have an input,
    // but even when it is being display, we may run into cases where
    // the my inputs array is actually empty (even though we're showing
    // an input).
    virtual unsigned		 nInputs() const;

    // These methods, which must be implemented by the derived class, should
    // not worry about whether the parameter id defined elsewhere:
    virtual void		 PARMSCOPE(UT_String &str) const;
    virtual void		 setPARMSCOPE(const char *str);
    virtual void		 PARMACCESS(UT_String &str) const;
    virtual void		 PARMNAME(UT_String &str) const	= 0;
    virtual void		 setPARMNAME(const UT_String &str);
    virtual void		 PARMPREFIX(UT_String &str) const;
    virtual void		 PARMPOSTFIX(UT_String &str) const;
    virtual void		 PARMLABEL(UT_String &str) const = 0;
    virtual int			 PARMTYPEINDEX() const	= 0;
    virtual void		 setPARMTYPEINDEX(int type);
    virtual void		 PARMTYPENAME(UT_String &type_name) const;
    virtual void		 setPARMSTORAGE(int type);
    virtual void		 PARMMENUOPTION(UT_String &str) const;
    virtual void		 setPARMMENUOPTION(const UT_String &menu_option);

    // Convenience method to set a parameter type
    void			 setParmType( VOP_Type type );

    /// Return true if the promoted parameter is visible
    /// on the VOPNET creator node.
    virtual bool		 isParmVisible() const = 0;

    /// Copy settings from a source parameter generator
    virtual void		 copySettingsFrom(VOP_ParmGenerator *pvop);

    /// Copy the default values from the given parameter and use them
    /// as the default values for this node's generated parameter.
    virtual void		 copyDefaultValuesFrom(
				    const PI_EditScriptedParm *parm);

    /// Get the type of the parameter (possibly filtered for a given language)
    VOP_Type		 getParameterType(const VOP_Language *l=NULL);
    VOP_TypeInfo	 getParameterTypeInfo(const VOP_Language *l=NULL);

    /// Get the name of the parameter that holds the current default value.
    const char *	 getParameterDefaultValueParmName();

    /// Get the value of the parameter as an integer.  This checks the owner
    /// (i.e. network) for a parameter, then will evaluate the parameter as an
    /// integer.
    virtual bool	evalIntegerValue(int &value);

    /// Return the exported parameters manager that is keeping track of this
    /// parameter VOP.
    VOP_ExportedParmsManager *ownerExportedParmsManager() const;

    /// Parm nodes may override definers notion of export context.
    /// These are methods to determine if that's the case.
    bool	canForceExportInContext(const VOP_CodeGenContext &ctx) const;
    bool	shouldForceExportInContext(const VOP_CodeGenContext &ctx) const;
    bool	shouldCheckSecondaryDefinerInput(
			const VOP_CodeGenContext &ctx) const;

    /// Returns a list of all parm generatos that define parm of a given name.
    static void		 findAllShaderParmDefiners( 
				    UT_Array<VOP_ParmGenerator*> &parm_gens,
				    OP_Network *root,
				    const UT_StringRef &parmname);
protected:
	     VOP_ParmGenerator(OP_Network *parent, const char *name,
			       OP_Operator *entry);
    virtual ~VOP_ParmGenerator();

    /// @{ Scope menu helpers for subclasses.
    static PRM_ChoiceList	 theParmscopeMenu;
    static PRM_Default		 theParmscopeDefault;
    /// @}

    VOP_ParmGenerator *	    findParmDefinerInTable(
				    const UT_StringRef &parm_name,
				    const VOP_ParmGeneratorMap *table ) const;
    const VOP_ParmGeneratorMap *getShaderParmTable() const;
    const VOP_ParmGeneratorMap *getSubnetParmTable() const;
    void		 recalculateShaderParmDefiner(bool sendchangeevents);
    void		 recalculateSubnetParmDefiner(bool sendchangeevents);
    const VOP_ParmGenerator *getSubnetConnectorParmDefiner(bool search) const;

    virtual void	 getDescriptiveName(UT_String &name) const;
    virtual void	 postOpChanged(OP_EventType reason, void *data);

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
    virtual void	 getOutputVariableName(UT_String &var, int idx) const;

    // Helpers for dealing with variable name different than output name.
    void		 getTmpParmCodeName( UT_String &name ) const;
    bool		 usesTempOutputVar() const;

    /// Factored out helper that gets the parameter declaration.
    bool		 getParameterDeclarationHelper(UT_String &parmdecl,
					    const VOP_Language *language,
					    const VOP_CodeGenContext &context);

    /// Pass back the declaration code for the parameters
    /// defined by this VOP node.
    virtual void	 getParmDeclarationCode(UT_String &parm_decl,
			    const VOP_CodeGenContext &codegen_ctx,
			    const VOP_Language *language=NULL,
			    bool export_parms=false);

    // These methods, which must be implemented by the derived class, should
    // not worry about whether the parameter id defined elsewhere:
    virtual bool		 USEASPARMDEFINER() const	= 0;
    virtual bool		 USEOWNEXPORTCONTEXT() const;
    virtual bool		 SHOULDHAVECORRESPONDINGCONNECTOR() const;
    virtual int			 UNIFORM() const= 0;
    virtual int			 USEBOUND() const;
    virtual int			 EXPORTPARM() const;
    virtual void		 EXPORTSHADERCONTEXTS(
				    UT_StringArray &context_names) const;
    virtual void		 MENUCHOICES(UT_String &str) const;
    virtual void		 MENUSCRIPT(UT_String &str) const;
    virtual CH_ScriptLanguage	 MENUSCRIPTLANGUAGE() const;

    virtual bool		 needsInput(void) = 0;

    // Build the code to be output when useBound() is true but the parameter
    // is not actually bound:
    virtual void		 getUnboundCode(UT_String &codestr,
				    const VOP_CodeGenContext &context);

    // Handles the case of having our parameter name changed. This
    // implementation does nothing.
    virtual void		 handleParmNameChange(
					const UT_StringRef &oldparmname,
					const UT_StringRef &newparmname);

    // Determines based on the parm index if the PARMNAME may be changed
    // when this parm is changed.
    virtual bool		 getParmCanAffectParmName(int pidx) const = 0;

    virtual void		 getNodeSpecificInfoText(
					OP_Context &context,
					OP_NodeInfoParms &iparms);
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    // These methods check if the parameter is defined elsewhere before calling
    // the upper-case methods:
    int				 useBound() const;
    bool			 shouldHaveCorrespondingConnector() const;
    int				 exportParm(bool check_connected = true) const;
    void			 exportShaderContexts(
					UT_StringArray &context_names) const;
    int				 getParmTypeIndex();
    void			 getParmTypeName(UT_String &type_name);
    void			 getMenuOption(UT_String &menu_option);
    void			 menuChoices(UT_String &str);

    VOP_Type			 getUniformType(int pidx,
					bool conditioned = true,
					const VOP_Language *language=0);

    /// Copy a single parameter's value from the source parameter
    /// This is a convenience method for the copySettingsFrom() method
    void		copyParameterValue(const VOP_ParmGenerator *src,
				    const char *name,
				    bool just_value = true);

    virtual void	 preDelete();

private:
    // Sends change events to al parm nodes that have the changednode as
    // their parm definer.
    static void			 sendChangesForAllParmsDefinedBy(
						 OP_Node *changednode,
						 OP_Node *node);

    // Returns a pointer to the VOP_ParmGenerator within a given network
    // that defines the parameter with the specified name.
    static VOP_ParmGenerator	*findParmDefiner(OP_Network *root,
						 const UT_StringRef &parmname,
						 bool subnet_parm);
    // REcursive version of the above function.
    static VOP_ParmGenerator	*findParmDefiner(OP_Network *root,
						 const UT_StringRef &parmname,
						 bool &preferred,
						 bool subnet_parm);

    UT_StringHolder		 myParmNameCache;
    mutable OP_VERSION		 myCachedIsShaderParmVersion;
    mutable bool		 myCachedIsShaderParm;
    bool			 myIsAlwaysUsedAsShaderParm;
};

#endif
