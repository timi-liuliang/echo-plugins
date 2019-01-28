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

#ifndef __VOP_OutputInfoManager_h__
#define __VOP_OutputInfoManager_h__

#include "VOP_API.h"
#include "VOP_OutputInfo.h"
#include "VOP_Types.h"
#include <iosfwd>

#define VOP_OUTPUT_VALUE_NAME	    "value"

class OP_Context;
class VOP_Node;
class UT_InfoTree;
class UT_IStream;

class VOP_API VOP_OutputInfoManager
{
public:
    VOP_OutputInfoManager();
    ~VOP_OutputInfoManager();

    void setParentNode(VOP_Node* parent_node);
    void clear(void);

    /// To avoid hundreds of errors and warnings (a few per VOP node) in older
    /// versions of Houdini, we only save the override info if it is being
    /// used. This function returns true if the infos need to be saved,
    /// false otherwise.
    bool getNeedToSave(void);

    void setNormalOutputMode(const char* output_name, EOutputModeType mode);
    void setBypassedOutputMode(const char* output_name, EOutputModeType mode);
    UT_Options* getDebugValuePtr(const char* output_name);
    UT_Options* getBypassValuePtr(const char* output_name);
    EOutputModeType getDebugOutputMode(const char* output_name);
    EOutputModeType getBypassOutputMode(const char* output_name);
    int findCorrespondingInputIndex(const char* output_name, 
	    bool is_in_debug_mode, bool is_in_bypass_mode);
    void findCorrespondingInputName(const char* output_name,
	    bool is_in_debug_mode, bool is_in_bypass_mode, UT_String& str_out);
    const char* getDebugCorrespondingInputName(const char* output_name);
    const char* getBypassCorrespondingInputName(const char* output_name);
    void setDebugCorrespondingInputName(const char* output_name, const char* input_name);
    void setBypassCorrespondingInputName(const char* output_name, const char* input_name);
    
    void setUseDefaultValue(const char* output_name, bool val);
    bool getUseDefaultValue(const char* output_name);

    void appendOutputOverrideCode(UT_String& str_code);
    bool areAllOverridden(void);
    bool getIsOverriden(const char* output_name);

    static VOP_Type getConnectorType(VOP_Node* node, int connector_idx, bool is_output = true);
    static VOP_Type getConnectorType(VOP_Node* node, const char* connector_name, bool is_output = true);

    void update(void);

    bool save(std::ostream &os, bool is_binary);
    bool load(UT_IStream &is);

    void appendInfoText(bool verbose, UT_WorkBuffer &text);
    void appendInfoTree(bool verbose, UT_InfoTree &tree);
    bool doesInfoExist(const char* output_name);

private:
    /// Private because the manager is responsible for setting everything.
    VOP_OutputInfo* getOutputInfo(const char* output_name, bool do_allocate);
    void getZeroValueString(VOP_OutputInfo* output_info, UT_String& str_out);

    VOP_Node* myParentNode;
    TOutputInfosMap myOutputInfos;
};

#endif
