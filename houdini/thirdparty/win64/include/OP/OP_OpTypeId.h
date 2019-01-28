/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	Enums for OP_Node
 *
 */

#ifndef __OP_OpTypeId_h__
#define __OP_OpTypeId_h__

enum OP_OpTypeId
{
    INVALID_OPTYPE_ID = -1,
    OBJ_OPTYPE_ID	= 0,
    SOP_OPTYPE_ID,
    POPNET_OPTYPE_ID,
    POP_OPTYPE_ID,
    CHOPNET_OPTYPE_ID,
    CHOP_OPTYPE_ID,
    ROP_OPTYPE_ID,
    SHOP_OPTYPE_ID,
    COP2_OPTYPE_ID,
    COPNET_OPTYPE_ID,
    VOP_OPTYPE_ID,
    VOPNET_OPTYPE_ID,
    DOP_OPTYPE_ID,
    TOP_OPTYPE_ID,
    LOP_OPTYPE_ID,
    MGR_OPTYPE_ID,
    DIR_OPTYPE_ID,
    NUM_MANAGERS
};

#endif
