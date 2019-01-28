/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ScriptOperator.h ( VOP Library, C++)
 *
 * COMMENTS:	OP_Operator for VOP classes.
 */

#ifndef __VOP_ScriptOperator__
#define __VOP_ScriptOperator__

#include "VOP_API.h"
#include <UT/UT_String.h>
#include <UT/UT_Array.h>
#include <PI/PI_ScriptOperator.h>
#include "VOP_OperatorInfo.h"

#define DEFAULT_INPUTSET_NAME		"default"
#define DEFAULT_INPUTSET_LABEL		"Default Inputs"
#define VOP_MATERIAL_OP_SUBTYPE_NAME	"material"

class VOP_CodeGenContext;


class VOP_API VOP_ScriptOperator : public PI_ScriptOperator
{
public:
    VOP_ScriptOperator(const char	*path,
			const char	*name,
			const char	*label,
			int minInputs, int maxInputs,
			const char	*vopnetMask,
			OP_Constructor	 construct,
			const char	   *child_table_name,
			PRM_Template	*multiInputSetTemplates,
			bool		is_subnet);
    virtual ~VOP_ScriptOperator();

    static bool			 isMaterialOperator(const OP_OTLDefinition &);
    static OP_ScriptOperator	*createScriptOperator(const OP_OTLDefinition &);

    virtual OP_SpecificData		*getOpSpecificData()
					 { return &myOperatorInfo; }
    virtual const OP_SpecificData	*getOpSpecificData() const
					 { return &myOperatorInfo; }
    VOP_OperatorInfo			*getVopOperatorInfo()
					 { return &myOperatorInfo; }
    const VOP_OperatorInfo		*getVopOperatorInfo() const
					 { return &myOperatorInfo; }

    virtual int			 updateParmTemplates();
    virtual unsigned		 maxOutputs();
    unsigned			 maxScriptOutputs();
    const UT_StringHolder	&getOutputName(int idx);
    const UT_StringHolder	&getOutputLabel(int idx);
    const VOP_TypeInfo		&getOutputTypeInfo(int idx, int inputset);

    const UT_StringHolder	&getInputName(int idx);
    const UT_StringHolder	&getInputLabel(int idx);
    const VOP_TypeInfo		&getInputTypeInfo(int idx, int inputset);
    bool			 getIsInputVisible(int idx); 

    // True if op wants to triger code generation in a given context (
    // eg surface, displacement, etc).
    bool			 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &ctx,
					bool check_shader_context) const;

    const UT_String		&getOuterCode() const;
    const UT_String		&getCode() const;

    virtual bool		 wantsParametersAnimatable() const;
    virtual bool		 wantsInputEditor() const;
    virtual bool		 wantsParentInputs() const;

    /// @{
    /// Methods for querying the input sets (ie, operator signatures).
    /// A VOP HDA may support inputs of different types (ints, floats),
    /// and it can do it by defining a new input set (type signature) for
    /// each such input type. On HDA it's done in Input/Output tab.
    int				 getNumInputSets() const;
    const char			*getInputSetName(int index) const;
    const char			*getInputSetLabel(int index) const;
    int				 getInputSetIndex(const char *name) const;
    /// @}

    virtual const PRM_Template	*getBaseParmTemplates();

    void getAllowedInputTypes(unsigned idx, VOP_VopTypeArray &voptypes);
    void getAllowedInputTypeInfos(unsigned idx, VOP_VopTypeInfoArray &voptypes);

protected:
    virtual const char		*getAddedDSIncludePath();
    virtual int			 handleUnknownToken(DS_Stream &ds,
					    UT_String &token);
    bool			 addOutput(const UT_String &outtype,
					   const UT_String &name,
					   const UT_String &label);
    bool			 addInput(const UT_String &intype,
					  const UT_String &name,
					  const UT_String &label);
    virtual PRM_Template	*loadParmTemplates(UT_IStream &is,
					    const char *filename,
					    UT_String &scriptName,
					    PRM_ScriptImports *&imports);

private:
    // Just a convenience function that does something we need to do a lot.
    // It checks if our .ds file is loaded yet, and loads it if required.
    void			 ensureParmsAreLoaded();

    VOP_OperatorInfo		 myOperatorInfo;
    PRM_Template		*myMultiInputSetBase;
    bool			 myIsSubnet;
};

#endif
