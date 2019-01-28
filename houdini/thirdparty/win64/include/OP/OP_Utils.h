/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	Some utility functions for operators.
 *
 */

#ifndef __OP_Utils_h__
#define __OP_Utils_h__

#include "OP_API.h"
#include "OP_Node.h"
#include <UT/UT_Matrix2.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Ramp.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

class OP_NetworkBoxItem;
class UT_BitArray;
class UT_WorkBuffer;
class UT_StringArray;

OP_API void	 OPwarnBadRefs(UT_WorkBuffer &msg,
			       const UT_StringArray &badrefs);
OP_API bool	 OPconvertOrWarnExternalReferences(OP_Node *node,
						   bool checktoplevelnode,
						   UT_WorkBuffer &err);
OP_API bool	 OPconvertOrWarnExternalOverrides(OP_Node *node,
						  bool checktoplevelnode,
						  UT_WorkBuffer &err);

/// Looks for any nodes inside the specified operator that are defined by
/// VOP Networks outside the provided node. REtrns true if no such nodes
/// are found. Otherwise it puts an error message in the err buffer and
/// returns false.
OP_API bool	 OPcheckForVopnetDefinedNodes(OP_Node *node,
					      UT_WorkBuffer &err);
OP_API void	 OPgetVopSubnetMask(OP_Node *node,
				    UT_String &vopnetmask);

/// @{ Functions for testing whether a node parameter is used as a shader
/// parameter, when the node represents a shader. Some node parameters may
/// belong to only certain contexts (eg, surface shader), while other ones
/// may be just true spare parameters that are not shader parameters at all.
OP_API bool	 OPisPotentialShaderParm(const PRM_Template &parm);
OP_API bool	 OPisStandInShaderParm(const PRM_Template &parm);
OP_API bool	 OPmatchesShaderContextTag(const PRM_Template &parm, 
			const char *context_name);
OP_API bool	 OPisLegacyShaderParm(const PRM_Template &parm, OP_Node *owner);
OP_API bool	 OPhasShaderContextTag(const PRM_Template *list, 
			int start = 0, int end = -1);
/// @}

OP_API OP_Node *OPfindOpInput(OP_Node *op, const char *find_op_type,
			      bool include_me = true);
OP_API OP_Node *OPfindOpInput(OP_Node *op, const char **find_op_types,
			      bool include_me = true);

/// Returns coordinates specifying the smallest possible bounding box 
/// around the items in 'items'. Takes each item's height and width into 
/// consideration.
OP_API void	OPgetItemBounds(fpreal &x1, fpreal &y1, fpreal &x2, fpreal &y2, 
			        const OP_NetworkBoxItemList &items);

/// Looks for any VOP Networks inside the specified node which define operator
/// types that are instantiated outside the node. If it returns false, the
/// badvopnets array will be filled with the VOP Networks that are used
/// outside the root node.
OP_API bool	 OPcheckForVopnetsUsedOutside(OP_Node *node,
					      OP_NodeList *badvopnets);

/// Creates a sticky note about the lack of vops inside synced HDAs, since the
/// children have been removed because the subnet is using cached code.
OP_API void	 OPcreateStickyNoteAboutCachedCode(OP_Network *subnet);

/// Given a set of unique nodes, compute a topological ordering using only the
/// connectivity amongst the given nodes. If the roots are not known, then it
/// will be internally computed using OPmarkNodeRoots(only_picked=false).
/// Otherwise, the nodes will only be sorted by traversing from the given root
/// nodes.
OP_API void	 OPtopologicalSort(
			UT_IntArray &order,
			const OP_NodeList &nodes,
			const UT_BitArray *roots = nullptr);

/// Given a set of nodes return a bit array indicating the nodes which are
/// roots WITHIN THE SET. If assume_picked is true, then it's assumed that the
/// node is in the set if and only if it's in the set.
OP_API void	OPmarkRootNodes(
			UT_BitArray &roots,
			const OP_NodeList &nodes,
			bool assume_picked);

namespace OP_Utils
{
    // Boiler plate to load individual types from op.
    static inline void evalOpParm(
	int64 &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v = node->evalInt(parmname, 0, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
    }
    static inline void evalOpParm(
	bool &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v = node->evalInt(parmname, 0, time);
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
    }
    static inline void evalOpParm(
	fpreal64 &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v = node->evalFloat(parmname, 0, time);
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
    }
    static inline void evalOpParm(
	UT_Vector2D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalFloat(parmname, 0, time); 
      v.y() = node->evalFloat(parmname, 1, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
    }
    static inline void evalOpParm(
	UT_Vector3D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalFloat(parmname, 0, time); 
      v.y() = node->evalFloat(parmname, 1, time); 
      v.z() = node->evalFloat(parmname, 2, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(2));
    }
    static inline void evalOpParm(
	UT_Vector4D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalFloat(parmname, 0, time); 
      v.y() = node->evalFloat(parmname, 1, time); 
      v.z() = node->evalFloat(parmname, 2, time); 
      v.w() = node->evalFloat(parmname, 3, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(2));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(3));
    }

    static inline void evalOpParm(
	UT_Matrix2D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 2; r++)
      {
	for (int c = 0; c < 2; c++)
	{
	    v(r, c) = node->evalFloat(parmname, r * 2 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(r * 2 + c));
	}
      }
    }
    static inline void evalOpParm(
	UT_Matrix3D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 3; r++)
      {
	for (int c = 0; c < 3; c++)
	{
	    v(r, c) = node->evalFloat(parmname, r * 3 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(r * 3 + c));
	}
      }
    }
    static inline void evalOpParm(
	UT_Matrix4D &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 4; r++)
      {
	for (int c = 0; c < 4; c++)
	{
	    v(r, c) = node->evalFloat(parmname, r * 4 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(r * 4 + c));
	}
      }
    }

    static void inline evalOpParm(
	UT_Vector2I &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalInt(parmname, 0, time); 
      v.y() = node->evalInt(parmname, 1, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
    }
    static void inline evalOpParm(
	UT_Vector3I &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalInt(parmname, 0, time); 
      v.y() = node->evalInt(parmname, 1, time); 
      v.z() = node->evalInt(parmname, 2, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(2));
    }
    static void inline evalOpParm(
	UT_Vector4I &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { v.x() = node->evalInt(parmname, 0, time); 
      v.y() = node->evalInt(parmname, 1, time); 
      v.z() = node->evalInt(parmname, 2, time); 
      v.w() = node->evalInt(parmname, 3, time); 
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(1));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(2));
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(3));
    }
    static void inline evalOpParm(
	UT_StringHolder &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { UT_String result; node->evalString(result, parmname, 0, time); v = result;
      if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
    }
    static void inline evalOpParm(
	UT_SharedPtr<UT_Ramp> &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { 
        v.reset(new UT_Ramp());
        
        if (node->hasParm(parmname))
            node->updateRampFromMultiParm(time, node->getParm(parmname), *v, 0, depnode);
    }
    static void inline evalOpParm(
	PRM_DataItemHandle &v, const OP_Node *node, const char *parmname, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v = node->evalData(parmname, 0, time);
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParm(parmname).microNode(0));
    }
    
    
    static void inline evalOpParmInst(
	int64 &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v = node->evalIntInst(parmname, inst, 0, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
    }
    static void inline evalOpParmInst(
	bool &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v = node->evalIntInst(parmname, inst, 0, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
    }
    static void inline evalOpParmInst(
	fpreal64 &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v = node->evalFloatInst(parmname, inst, 0, time);
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
    }
    static void inline evalOpParmInst(
	UT_Vector2D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v.x() = node->evalFloatInst(parmname, inst, 0, time); 
	v.y() = node->evalFloatInst(parmname, inst, 1, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(1));
    }
    static void inline evalOpParmInst(
	UT_Vector3D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v.x() = node->evalFloatInst(parmname, inst, 0, time); 
	v.y() = node->evalFloatInst(parmname, inst, 1, time); 
	v.z() = node->evalFloatInst(parmname, inst, 2, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(1));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(2));
    }
    static void inline evalOpParmInst(
	UT_Vector4D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v.x() = node->evalFloatInst(parmname, inst, 0, time); 
	v.y() = node->evalFloatInst(parmname, inst, 1, time); 
	v.z() = node->evalFloatInst(parmname, inst, 2, time); 
	v.w() = node->evalFloatInst(parmname, inst, 3, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(1));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(2));
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(3));
    }

    static void inline evalOpParmInst(
	UT_Matrix2D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 2; r++)
      {
	for (int c = 0; c < 2; c++)
	{
	    v(r, c) = node->evalFloatInst(parmname, inst, r * 2 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(r * 2 + c));
	}
      }
    }
    static void inline evalOpParmInst(
	UT_Matrix3D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 3; r++)
      {
	for (int c = 0; c < 3; c++)
	{
	    v(r, c) = node->evalFloatInst(parmname, inst, r * 3 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(r * 3 + c));
	}
      }
    }
    static void inline evalOpParmInst(
	UT_Matrix4D &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { for (int r = 0; r < 4; r++)
      {
	for (int c = 0; c < 4; c++)
	{
	    v(r, c) = node->evalFloatInst(parmname, inst, r * 4 + c, time);
	    if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(r * 4 + c));
	}
      }
    }

    static void inline evalOpParmInst(
	UT_StringHolder &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	UT_String result; node->evalStringInst(parmname, inst, result, 0, time); v = result; 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
    }
    static void inline evalOpParmInst(
	UT_SharedPtr<UT_Ramp> &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
        v.reset(new UT_Ramp());
        
        const PRM_Parm  *rampparm = node->getParmList()->getParmPtrInst(parmname, inst, /*nestlevel=*/1);
        if (rampparm)
            node->updateRampFromMultiParm(time, *rampparm, *v, 0, depnode);
    }
    static void inline evalOpParmInst(
	PRM_DataItemHandle &v, const OP_Node *node, const char *parmname, 
	int *inst, fpreal time,
	DEP_MicroNode *depnode)
    { 
	v = node->evalDataInst(parmname, inst, 0, time); 
	if (depnode) depnode->addExplicitInput(SYSconst_cast(node)->getParmPtrInst(parmname, inst)->microNode(0));
    }
}

#endif
