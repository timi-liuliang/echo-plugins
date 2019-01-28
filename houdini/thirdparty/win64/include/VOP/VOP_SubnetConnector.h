/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_SubnetConnector_h__
#define __VOP_SubnetConnector_h__

#include "VOP_API.h"
#include "VOP_ParameterBase.h"

class PI_EditScriptedParm;

class VOP_API VOP_SubnetConnector : public VOP_ParameterBase
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		*buildTemplates(VOP_Language *language);

    virtual bool		 getParmConstantString(const char *parmname,
					       const VOP_TypeInfo &type_info,
					       UT_String &str,
					       bool expand_string,
					       const VOP_Language *language=0);

    virtual void	    getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		    getInputFromNameSubclass(const UT_String &in) const;
    virtual const char	   *inputLabel(unsigned idx) const;
    virtual bool	    forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &ctx, 
					bool check_shader_context) const;

    virtual void		 setPARMNAME(const UT_String &str);
    void			 setPARMLABEL(const UT_String &str);
    virtual void		 setPARMTYPEINDEX(int type);
    virtual void		 setPARMSTORAGE(int type);
    void			 setPARMSTRINGTYPE(const char *style);

    virtual void		 PARMSCOPE(UT_String &str) const;
    virtual void		 PARMNAME(UT_String &str) const;
    virtual void		 PARMLABEL(UT_String &str) const;
    virtual int			 PARMTYPEINDEX() const;

    virtual bool		 isParmVisible() const;
    virtual void		 copySettingsFrom(VOP_ParmGenerator *pvop);

protected:
	     VOP_SubnetConnector(OP_Network *parent, const char *name,
		           OP_Operator *entry);
    virtual ~VOP_SubnetConnector();

    virtual bool	 	 updateParmsFlags();
    
    // Override cook method just so we can set our warnings properly.
    virtual bool		 USEASPARMDEFINER() const;
    virtual int			 UNIFORM() const;
    virtual int			 EXPORTPARM() const;

    virtual bool		 needsInput(void);

    // Determines based on the parm index if the PARMNAME may be changed
    // when this parm is changed.
    virtual bool		 getParmCanAffectParmName(int pidx) const;

    virtual void		 getInputTypeInfoSubclass(
					VOP_TypeInfo &type_info, int idx);
    virtual void		 getAllowedInputTypesSubclass(unsigned idx,
					VOP_VopTypeArray &voptypes);

private:
    // Store the index of the Bind Name and Parameter Type parameters.
    static int			 theParmNameParmIndex;
    static int			 theParmTypeParmIndex;
};

#endif
