/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	VOP Library (C++)
*
* COMMENTS:	
*
*/

#ifndef __VOP_OutputInfo_h__
#define __VOP_OutputInfo_h__

#include <UT/UT_Options.h>
#include "VOP_API.h"
#include <map>
#include <string>
#include <iosfwd>


#define VOP_AUTO_CORR_INPUT_TOKEN	"___auto___"
#define VOP_DEFAULT_INPUT_TOKEN		"__default__"

class VOP_Node;
/******************************************************************************/
// Ascii tokens
static const char	VOP_NUM_INFOS_TOKEN[]		= "NumInfos";
static const char	VOP_BEGIN_INFO_RECORD_TOKEN[]	= "BeginInfo";
static const char	VOP_NORMAL_MODE_TOKEN[]		= "NormalMode";
static const char	VOP_END_INFO_RECORD_TOKEN[]	= "EndInfo";
static const char	VOP_OUTPUT_NAME_TOKEN[]		= "OutputName";
static const char	VOP_BYPASS_MODE_TOKEN[]		= "BypassMode";
static const char	VOP_ARE_VALUES_BINARY_TOKEN[]	= "ValuesBinary";
static const char	VOP_DEBUG_CORR_INPUT_NAME_TOKEN[]	= "DebugCorrInputName";
static const char	VOP_BYPASS_CORR_INPUT_NAME_TOKEN[]	= "BypassCorrInputName";
static const char	VOP_USE_DEFAULT_VALUE_TOKEN[]		= "UseDefaultValue";

static const char	VOP_COMMENT_CHAR = '#';

// Binary tokens
enum
{
    VOP_NUM_INFOS		= 100, // give some room for future growth
    VOP_ARE_VALUES_BINARY,

    VOP_BEGIN_INFO_RECORD,
    VOP_OUTPUT_NAME,
    VOP_NORMAL_MODE,
    VOP_BYPASS_MODE,
    VOP_DEBUG_CORR_INPUT_NAME,
    VOP_BYPASS_CORR_INPUT_NAME,
    VOP_USE_DEFAULT_VALUE,

    VOP_END_INFO_RECORD,
};
/******************************************************************************/
// Note: the order of these must correspond to the FUSE_VopTile.ui menu items.
enum EOutputModeType
{
    VOP_OUTPUT_MODE_EXECUTE = 0,
    VOP_OUTPUT_MODE_PASSTHROUGH,
    VOP_OUTPUT_MODE_CONSTANT,

    VOP_OUTPUT_MODE_NONE
};
/******************************************************************************/
class VOP_API VOP_OutputInfo
{
public:

    VOP_OutputInfo(VOP_Node* parent_node, const char* output_name);
    ~VOP_OutputInfo();

    /// Sets the behaviour type for this output in the debug mode.
    void setDebugMode(EOutputModeType mode);
    /// Sets the behaviour type for this output in the bypass mode.
    void setBypassedMode(EOutputModeType mode);

    EOutputModeType getDebugMode(void);
    EOutputModeType getBypassMode(void);

    /// Retrieves the constant value pointer for the debug mode.
    UT_Options* getDebugValuePtr(void);
    /// Retrieves the constant value pointer for the release mode.
    UT_Options* getBypassValuePtr(void);

    void appendOutputOverrideCode(UT_String& str_code, bool is_in_debug_mode, bool is_in_bypass_mode);

    /// Returns the index of the output this info applies to.
    int getIndex(void);

    /// Returns the variable name that should be used in the override code.
    /// The variable may or may not be prefixed with '$', depending if
    /// the output variable name is fixed or not. In majority of cases it is,
    /// but for vops such as globals or exporting parm vop, it is not.
    void	getOutputVariableCode(UT_String &var_code);

    bool save(std::ostream &os, bool is_binary);
    bool load(UT_IStream &is);

    /// Returns the name of the output this info applies to.
    const char* getOutputName(void);
    const char* getDebugCorrespondingInputName(void);
    const char* getBypassCorrespondingInputName(void);
    void setDebugCorrespondingInputName(const char* input_name);
    void setBypassCorrespondingInputName(const char* input_name);
    
    void    setUseDefaultValue(bool val) { myUseDefaultVal = val; }
    bool    getUseDefaultValue()	 { return myUseDefaultVal; }

    /// Attempts to find an input which (logically) corresponds to this
    /// output, and returns its index if found or -1 otherwise. Note that
    /// this looks at any explicit corresponding input names that may
    /// be specified, and returns that, if found, first.
    int findCorrespondingInput(bool is_in_debug_mode, bool is_in_bypass_mode);

    /// Attempts to find an input which best corresponds to the output with the
    /// name specified on the given node.
    static int autoFindCorrespondingInput(VOP_Node* node, const char* output_name);

private:

    VOP_Node* myParentNode;
    std::string myOutputName;

    // The mode which dictates what to output in normal state.
    EOutputModeType myDebugMode;

    // The mode which dictates what to output in bypassed state.
    EOutputModeType myBypassedMode;

    UT_Options myDebugValue, myBypassValue;

    std::string myDebugCorrespondingInputName, myBypassCorrespondingInputName;
    bool    myUseDefaultVal;
};
/******************************************************************************/
typedef std::map < std::string, VOP_OutputInfo* > TOutputInfosMap;
/******************************************************************************/
#endif
