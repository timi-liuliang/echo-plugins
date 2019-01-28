/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	An object that cooks its transform by running a Python script.
 *
 *	This node gets the Python code from a section in the containing
 *	digital asset.
 */

#ifndef __OBJ_Python_h__
#define __OBJ_Python_h__

#include "OBJ_API.h"
#include "OBJ_SubNet.h"
class OP_ScriptOperator;
class OP_OTLDefinition;

class OBJ_API OBJ_Python : public OBJ_SubNet
{
public:
    static OP_ScriptOperator *createScriptOperator(
	const OP_OTLDefinition &definition);

    static OP_Node *myConstructor(
	OP_Network *network, const char *name, OP_Operator *op_type);

    OBJ_Python(
	OP_Network *network, const char *name,
	OP_ScriptOperator *script_op_type);

    // These methods are used by HOMF_ObjNode::setCookTransform()
    // to set the parm transform from within the Python cook code.
    bool isCooking() const { return myIsCooking; }
    void setCookParmTransform(const UT_DMatrix4 &xform)
    { myCookParmTransform = xform; }

    virtual bool getParmTransform(OP_Context &context, UT_DMatrix4 &xform);

private:
    UT_DMatrix4 myCookParmTransform;
    bool myIsCooking;
};

#endif
