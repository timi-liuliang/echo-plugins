/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_RewireHelper.C ( OPUI Library, C++)
 *
 * COMMENTS: This is a utility class to help maintain proper wirings in a VOP
 *           network when a VOP's inputs and outputs are moved around.
 */

#ifndef __VOP_RewireHelper__
#define __VOP_RewireHelper__

#include "VOP_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>

class VOP_ScriptOperator;
class VOP_OperatorInfo;
class VOP_Node;
class VOP_SubnetOutput;


class VOP_API VOP_RewireHelper
{
public:
     VOP_RewireHelper();
    ~VOP_RewireHelper();

    /// @{ Records and rewires all nodes of the given operator type.
    void recordWires(VOP_ScriptOperator *op);
    void rewire(VOP_ScriptOperator *op);
    /// @}

    /// Rewire the node to reflect new inputs and outputs given in op info.
    void updateAndRewireNode( VOP_Node *node, const VOP_OperatorInfo *op_info );

private:
    void    recordOldNodeConnectorNamesAndWires( VOP_Node *node );
    void    doRewire(VOP_ScriptOperator *op);

    void    recordInputWires(VOP_Node *op, VOP_Node *source);
    void    recordOutputWires(VOP_Node *op);
    bool    constructMappings(VOP_ScriptOperator *op);

    void    disconnectVisitedHDAsAndSubOutputs();
    void    markSubOutputForWiring(VOP_SubnetOutput *sub_output);
    void    unmarkAllSubOutputsForWiring();

private:
    /// Forward declaration of nested helper classes.
    class RewireInfo;
    class Wire;

    VOP_RewireHelper::RewireInfo *	myRewireInfo;
    UT_Array<VOP_RewireHelper::Wire *>	myWires;
    UT_ValArray<VOP_Node *>		myVisitedHDAs;
    UT_IntArray				myVisitedSubOutputIDs;
};

#endif
