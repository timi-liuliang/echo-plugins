/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_Visualize_h__
#define __VOP_Visualize_h__

#include "VOP_API.h"
#include "VOP_ParameterBase.h"

class PI_EditScriptedParm;

class VOP_API VOP_Visualize : public VOP_ParameterBase
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		*buildTemplates(VOP_Language *language);
    virtual void		 getCode(UT_String &codestr,
                		         const VOP_CodeGenContext &context);

    virtual void		 initializeNode();

    bool forceCodeGenerationOfInputs( const VOP_CodeGenContext &ctx, 
				      bool check_shader_context) const
    {
	if (!getInput(0))
	    return false;
	return VOP_ParameterBase::forceCodeGenerationOfInputs(ctx, 
		check_shader_context);
    }

    virtual bool		 getIsParmGenerator() const
    { if (!getInput(0)) return false;
      return VOP_ParameterBase::getIsParmGenerator(); }

    // Because we switch in and out of being an op generator we
    // have to dirty the parm generator explicitly on a rewire.
    virtual void	 opChanged(OP_EventType reason, void *data=0);

    // Get the parm block information that will go in the dialog script
    // for our VOPNET.
    virtual void		 getParameterDeclaration(UT_String &parmdecl,
					    const VOP_Language *language,
					    const VOP_CodeGenContext &context);

    virtual bool		 getParmConstantString(const char *parmname,
					       const VOP_TypeInfo &type_info,
					       UT_String &str,
					       bool expand_string,
					       const VOP_Language *language=0);

    virtual const char		*inputLabel(unsigned idx) const;

    virtual void		 setPARMNAME(const UT_String &str);
    void			 setPARMLABEL(const UT_String &str);
    virtual void		 setPARMTYPEINDEX(int type);
    virtual void		 setPARMSTORAGE(int type);
    void			 setPARMSTRINGTYPE(const char *style);
    void			 setPARMOPFILTER(const char *filter);

    virtual void		 PARMNAME(UT_String &str) const;
    virtual void		 PARMPREFIX(UT_String &str) const;
    virtual void		 PARMPOSTFIX(UT_String &str) const;
    virtual void		 PARMLABEL(UT_String &str) const;
    virtual int			 PARMTYPEINDEX() const;

    virtual bool		 isParmVisible() const;
    virtual void		 copySettingsFrom(VOP_ParmGenerator *pvop);
    virtual void		 copyDefaultValuesFrom(
				    const PI_EditScriptedParm *parm);

protected:
	     VOP_Visualize(OP_Network *parent, const char *name,
		           OP_Operator *entry);
    virtual ~VOP_Visualize();

    virtual bool	 	 updateParmsFlags();
    
    // Override cook method just so we can set our warnings properly.
    virtual bool		 USEASPARMDEFINER() const;
    virtual int			 UNIFORM() const;
    virtual int			 USEBOUND() const;
    virtual int			 EXPORTPARM() const;
    virtual void		 EXPORTSHADERCONTEXTS(
					UT_StringArray &context_names) const;
    using VOP_ParameterBase::EXPORTSHADERCONTEXTS;
    virtual void		 MENUCHOICES(UT_String &str) const;
    void			 CALLBACKSTR(UT_String &str) const;
    void			 HELPTEXTSTR(UT_String &str) const;
    int				 INVISIBLE() const;
    int				 JOINNEXT() const;
    int				 SHOWLABEL() const;
    int				 IRANGE(int index) const;
    float			 FRANGE(int index) const;

    virtual bool		 needsInput(void);

    // Build the code to be output when useBound() is true but the parameter
    // is not actually bound:
    virtual void		 getUnboundCode(UT_String &codestr,
				    const VOP_CodeGenContext &context);

    virtual void	 getParmDeclarationCode(UT_String &parm_decl,
			    const VOP_CodeGenContext &codegen_ctx,
			    const VOP_Language *language=NULL,
			    bool export_parms=false);

    // Determines based on the parm index if the PARMNAME may be changed
    // when this parm is changed.
    virtual bool		 getParmCanAffectParmName(int pidx) const;

    virtual void		 getInputTypeInfoSubclass(
					VOP_TypeInfo &type_info, int idx);
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
private:
    bool			 allowMenu(int parmtype) const;

    // Store the index of the Visualize Name and Parameter Type parameters.
    static int			 theParmNameParmIndex;
    static int			 theParmTypeParmIndex;
};

#endif
