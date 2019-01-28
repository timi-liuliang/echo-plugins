/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_OperatorInfo.h ( VOP Library, C++)
 *
 * COMMENTS:	This is additional information available via every VOP
 *		operator.
 */

#ifndef __VOP_OperatorInfo__
#define __VOP_OperatorInfo__

#include "VOP_API.h"
#include "VOP_Types.h"
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>
#include <OP/OP_Operator.h>
#include <iosfwd>

class VOP_LanguageContextTypeList;
class VOP_TypeInfo;
typedef UT_Array<VOP_Type>	 VOP_VopTypeArray;
typedef UT_Array<VOP_TypeInfo>	 VOP_VopTypeInfoArray;

enum VOP_ConnectorFlagType
{
    VOP_CONNECTOR_FLAG_HIDDEN		= 0x0001,   // initially hidden
    VOP_CONNECTOR_FLAG_PARMGEN_CHILD	= 0x0002    // defined by Parm VOP child
};


/// Class containing tags for an input or an output.
class VOP_API VOP_InOutTagMap
{
public:
    const VOP_InOutTagMap&	operator=(const VOP_InOutTagMap &other);

    void		setTag(const char* name, const char* value);
    UT_StringHolder	getTag(const char* name) const;
    bool		hasTag(const char *name) const;

    void		getTagNames(UT_StringArray &names) const;

private:
    using MapType = UT_StringMap<UT_StringHolder>;
    UT_UniquePtr<MapType>		myTags;
};

/// Class containing information about node's input or output connector port.
class VOP_API VOP_InOutInfo
{
public:
    VOP_InOutInfo(const UT_StringHolder &name, const UT_StringHolder &label,
		  const VOP_TypeInfo &type_info);
    VOP_InOutInfo(const VOP_InOutInfo &other);
    ~VOP_InOutInfo();

    const UT_StringHolder &getName() const
			 { return myName; }
    void		 setName(const UT_StringHolder &name)
			 { myName = name; }
    const UT_StringHolder &getLabel() const
			 { return myLabel; }
    void		 setLabel(const UT_StringHolder &label)
			 { myLabel = label; }

    void		 addTypeInfo(const VOP_TypeInfo &type_info);
    void		 insertTypeInfo(const VOP_TypeInfo &newtype, int i);
    void		 setTypeInfo(int i, const VOP_TypeInfo &newtype);
    void		 removeTypeInfo(int i);

    const VOP_TypeInfo	&getTypeInfo(int i) const;
    VOP_Type		 getType(int i) const;
    
    void		 addInputName(const char* name)
			 { myDefaultInputNames.append(name); }
    void		 insertInputName(const char *name, int i)
			 { myDefaultInputNames.insert(name, i); }
    void		 removeInputName(int i)
			 { myDefaultInputNames.remove(i); }

    const UT_StringHolder	&getInputName(int i) const
			 { return myDefaultInputNames(i); }
    void		 setInputName(int i, const char *new_name)
			 { myDefaultInputNames(i) = new_name; }

    void		 addValueOption(const VOP_TypeInfo &type_info);
    void		 insertValueOption(
			    const VOP_TypeInfo &type_info, int i);
    void		 removeValueOption(int i);
    UT_Options		&getValueOption(int i);
    void		 changeValueOption(int i, const VOP_TypeInfo &type_info,
					   const char *new_val = NULL);

    int			 getNumTypes() const
			 { return myTypeInfos.entries(); }
    int			 getNumNames() const
			 { return myDefaultInputNames.entries();}
    int			 getNumOptions() const
			 { return myValueOptions.entries();}

    void		 setTag(const char* name, const char* value)
			 { myTags.setTag(name, value); }
    UT_StringHolder	 getTag(const char* name) const
			 { return myTags.getTag(name); }
    bool		 hasTag(const char *name) const
			 { return myTags.hasTag(name); }
    const VOP_InOutTagMap &getTagsMap() const
			 { return myTags; }
    void		 copyTagsFrom(VOP_InOutInfo& src_info)
			 { myTags = src_info.myTags; }

    void		 setFlag(VOP_ConnectorFlagType flag_type, bool value)
    {
	if(value)
	    myFlags |= flag_type;
	else
	    myFlags &= ~flag_type;
    }

    bool		hasFlag(VOP_ConnectorFlagType flag_type)
    {
	return ((myFlags & flag_type) != 0);
    }

    void copyFlagsFrom(VOP_InOutInfo& src_info)
    {
	myFlags = src_info.myFlags;
    }

    void setAllFlags(int flags)
    {
	myFlags = flags;
    }

    int getAllFlags(void)
    {
	return myFlags;
    }

private:
    UT_StringHolder		myName;
    UT_StringHolder		myLabel;
    UT_ValArray<VOP_TypeInfo*>	myTypeInfos;
    UT_StringArray		myDefaultInputNames;
    UT_Array<UT_Options>	myValueOptions;	    	
    VOP_InOutTagMap		myTags;

    int				myFlags;
};

class VOP_API VOP_OperatorInfo : public OP_SpecificData
{
public:
	     VOP_OperatorInfo();
    virtual ~VOP_OperatorInfo();

    const VOP_OperatorInfo &	operator=(const VOP_OperatorInfo &other);

    UT_String				&getCode()
					 { return myCode; }
    const UT_String			&getCode() const
					 { return myCode; }
    UT_String				&getOuterCode()
					 { return myOuterCode; }
    const UT_String			&getOuterCode() const
					 { return myOuterCode; }
    void				 setVopnetMask(const char *mask);
    const UT_String			&getVopnetMask() const
					 { return myVopnetMask; }
    void				 setRenderMask(const char *mask);
    const UT_String			&getRenderMask() const
					 { return myRenderMask; }
    UT_ValArray<VOP_InOutInfo *>	&getOutputInfos()
					 { return myOutputInfos; }
    const UT_ValArray<VOP_InOutInfo *>	&getOutputInfos() const
					 { return myOutputInfos; }
    UT_ValArray<VOP_InOutInfo *>	&getInputInfos()
					 { return myInputInfos; }
    const UT_ValArray<VOP_InOutInfo *>	&getInputInfos() const
					 { return myInputInfos; }
    UT_StringArray			&getInputSetNames()
					 { return myInputSetNames; }
    const UT_StringArray		&getInputSetNames() const
					 { return myInputSetNames; }
    UT_StringArray			&getInputSetLabels()
					 { return myInputSetLabels; }
    const UT_StringArray		&getInputSetLabels() const
					 { return myInputSetLabels; }
    bool				&getForceCodeGeneration()
					 { return myForceCodeGeneration; }
    const bool				&getForceCodeGeneration() const
					 { return myForceCodeGeneration; }
    bool				&getExternalShader()
					 { return myExternalShader; }
    const bool				&getExternalShader() const
					 { return myExternalShader; }
    VOP_Type				 getShaderType() const
					 { return myShaderType; }
    void				 setShaderType(VOP_Type t)
					 { myShaderType = t; }

    void		 clearInOutInfo();
    virtual void	 saveToDialogScript(std::ostream &os) const;
    virtual void	 updateExtraInfoBuffer(OP_ExtraInfoBuffer &b) const;

    /// Obtains a name of a node parameter that corresponds to a given 
    /// input name when the node uses the given signature.
    /// @{
    void		 getParmNameFromInput(UT_String &parm_name, 
			    const char *input_name, int input_set_index) const;
    void		 getParmNameFromInput(UT_WorkBuffer &parm_name, 
			    const char *input_name, int input_set_index) const;
    /// @}

    /// Obtains a name of the input given parameter name, considering
    /// the available signatures in this class.
    void		 getInputNameFromParm(UT_String &input_name, 
			    const char *parm_name) const;

    /// Returns a new language context object based on this operator info.
    /// Caller is responsible for deleting the returned object.
    VOP_LanguageContextTypeList *newLanguageContextTypeList() const;

protected:
    static void		 saveInOutInfoToDialogScript(std::ostream &os,
				const UT_ValArray<VOP_InOutInfo *> &infoarray,
				const char *infotype);
    static void saveInputFlags(std::ostream &os, 
                               const UT_ValArray<VOP_InOutInfo *> &inoutarray,
			       const char *token, bool only_nonzero_flags);

private:
    UT_ValArray<VOP_InOutInfo *>	 myOutputInfos;
    UT_ValArray<VOP_InOutInfo *>	 myInputInfos;
    UT_StringArray			 myInputSetNames;
    UT_StringArray			 myInputSetLabels;
    UT_String				 myVopnetMask;
    UT_String				 myRenderMask;
    UT_String				 myOuterCode;
    UT_String				 myCode;
    VOP_Type				 myShaderType;
    bool				 myForceCodeGeneration;
    bool				 myExternalShader;
};

#endif

