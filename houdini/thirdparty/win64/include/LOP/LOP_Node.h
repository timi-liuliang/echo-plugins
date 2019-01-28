/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP Library (C++)
 *
 * COMMENTS:	The base class for all Lighting Operators
 *
 */

#ifndef __LOP_Node_h__
#define __LOP_Node_h__

#include "LOP_API.h"
#include <OP/OP_Network.h>
#include <HUSD/HUSD_DataHandle.h>
#include <HUSD/HUSD_TimeCode.h>

class LOP_Network;
class HUSD_PythonConverter;
class HUSD_EditCustomData;
class HUSD_FindPrims;
class PRM_Template;
class PRM_Name;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void newLopOperator(OP_OperatorTable *table);
};

#define LOP_DEBUG_FLAG		'D'

enum LOP_EditableObject {
    LOP_STAGE,
    LOP_LAYER
};

enum LOP_InputNameVariableId {
    LOP_INPUTNAME_VAR_IIDX,
    LOP_INPUTNAME_MAX_VARIABLES
};

enum LOP_BypassState {
    LOP_BYPASS_ABSENT,
    LOP_BYPASS_OFF,
    LOP_BYPASS_ON
};

class LOP_API LOP_Node : public OP_Network
{
public:
    // Standard OP_Network stuff:
    static const char	*theChildTableName;
    virtual const char	*getChildType() const override;
    virtual const char	*getOpType() const override;

    virtual OP_OpTypeId  getChildTypeID() const override;
    virtual OP_OpTypeId  getOpTypeID() const override;
    virtual OP_DataType	 getCookedDataType() const override;

    static void		 buildOperatorTable(OP_OperatorTable &table);
    static OP_Operator  *getManagementOperator();
    static void		 initializeExpressions();

    virtual void	 opChanged(OP_EventType reason, void *data=0) override;

    virtual fpreal       getW() const override;
    virtual fpreal       getH() const override;

    // Gets/sets the value of the debug flag on this node.
    virtual int		 getDebug() const override;
    virtual int		 setDebug(int state) override;
    virtual int		 setFlag(char flag, int onoff) override;
    bool		 getNestedDebug() const;

    // Override the function that determines whether this node should be
    // drawn in the network editor with a "bypass" look. LOPs have an optional
    // "bypass" spare parameter that makes the node act like it is bypassed,
    // so it should also be drawn like it's bypassed.
    virtual bool	 getUseBypassLook(
				const OP_EditorContext &ctx) const override;

    // Cook this node and return a data handle for the resulting stage.
    const HUSD_DataHandle &getCookedDataHandle(OP_Context &context,
				int outidx = 0);

    // Override this for nodes that want to allow python code to edit their
    // stage or active layer directly.
    virtual HUSD_PythonConverter *getEditableObject(LOP_EditableObject obj);

    // Default name for an adhoc collection multiparm collapsing container.
    static PRM_Name	&getCollectionCollapserParmName();
    // Default name for an adhoc collection multiparm.
    static PRM_Name	&getCollectionParmName();
    // Create a parm template for an adhoc collection multiparm container.
    static PRM_Template	 getCollectionCollapserParmTemplate(
				PRM_Name &collapser_parm_name);
    // Create a parm template for an adhoc collection multiparm.
    static PRM_Template	 getCollectionParmTemplate(
				PRM_Name &collection_parm_name,
				const char *multiparm_prefix = nullptr);
    // Evaluate an adhoc collection multiparm and use it to fill in an
    // HUSD_FindParms object.
    static bool		 getCollection(OP_Node *node,
				const PRM_Template &collection_parm_template,
				fpreal t, HUSD_FindPrims &findprims);
    // Evaluate a simplified adhoc collection parm and use it to fill in an
    // HUSD_FindParms object.
    static bool		 getSimplifiedCollection(OP_Node *node,
				const UT_StringRef &pattern,
				HUSD_FindPrims &findprims);
    // Builds a menu containing all the "global" collections on the stage
    // of the supplied node. The node can be null.
    static void		 buildCollectionsMenu(LOP_Node *node,
				PRM_Name *names,
				int maxnames,
				bool add_separator = false);

    // Get a parm template that defines a multiparm for setting custom data
    // on a layer or primitive.
    static const PRM_Template &getCustomDataTemplate();
    // Called on a node with the custom data template defined above, this
    // method evaluates the multiparm and calls into the edit_data object to
    // set the custom data on the prims specified in find_prims, or on the
    // active layer if find_prims is null.
    void		 setCustomDataFromParms(fpreal t,
				const HUSD_EditCustomData &edit_data,
				const HUSD_FindPrims *find_prims);

    // Local variables that can be used by nodes which allow user-specifiable
    // names on inputs (such as merge into and add variant).
    static CH_LocalVariable theInputNameLocalVariables[];

    const UT_StringArray &lastModifiedPrims() const;
    void		 setLastModifiedPrims(const UT_StringRef &primpath);
    void		 setLastModifiedPrims(const UT_StringArray &primpaths);
    void		 setLastModifiedPrims(const HUSD_FindPrims &findprims);
    void		 clearLastModifiedPrims();

    // Code for tracking convex hulls around context option "blocks" in the
    // network editor.
    virtual bool	 showConvexHull() const
			 { return false; }
    virtual int		 contextOptionsStackEffect(int input) const
			 { return 0; }
    virtual void	 findEnclosedItems(OP_EnclosedItems &items) const;

    // Evaluate local variables common to multiple LOP node types.
    virtual bool	 evalVariableValue(fpreal &val, int var_id, 
				int thread) override;
    virtual bool	 evalVariableValue(UT_String &val, int var_id, 
				int thread) override;

    LOP_BypassState	 lastCookBypassState() const
			 { return myLastCookBypassState; }
    HUSD_TimeCode	 getTimeCode(const OP_Context &context,
				bool timedep) const;

protected:
	     LOP_Node(OP_Network *parent, const char *name,
		       OP_Operator *entry);
    virtual ~LOP_Node();

    // Common cook method for all LOP nodes. Calls cookMyLop.
    virtual OP_ERROR	 cookMe(OP_Context &context) override;
    // Specific cook method for each LOP node, must be overridden.
    virtual OP_ERROR	 cookMyLop(OP_Context &context) = 0;
    // Makes our own data handle reference or copy the data handle of another
    // node. Should be used to reference nodes that are not inputs to this
    // node.
    OP_ERROR		 cookReferenceNode(OP_Context &context,
				LOP_Node *source_lop,
				bool copy_data_handle = false);
    // Makes our own data handle reference or copy the data handle of one
    // of our input nodes.
    OP_ERROR		 cookReferenceInput(OP_Context &context,
				int which_input = 0,
				bool require_input = false,
				bool copy_data_handle = false);
    // Wrapper to call copyReferenceInput with the copy_data_handle
    // parameter set to true.
    OP_ERROR		 cookModifyInput(OP_Context &context,
				int which_input = 0,
				bool require_input = false);
    // Access the data handle of one of our inputs, that must have been
    // locked already to ensure the data is cooked for the current context.
    const HUSD_DataHandle &lockedInputData(OP_Context &context,
				int which_input);
    // Access our own data handle for modification. Should only be called
    // from within cookMyLop.
    HUSD_DataHandle	&editableDataHandle();

    // For subnet nodes, this method will return the contained node that
    // corresponds to specified output connector index. For all other nodes
    // with will return nullptr.
    LOP_Node		*getNodeForSubnetOutput(int outidx) const;

    // For nodes with editable input names, this function evaluates all the
    // input names, assigning any dependencies to the specified parameter.
    OP_ERROR		 getInputStrings(const UT_StringRef &key_parm_name,
				UT_StringArray &input_names);

    virtual OP_ERROR	 bypassMe(OP_Context &context,
				int &copied_input) override;
    virtual void	 deleteCookedData() override;
    virtual int		 saveCookedData(const char *, OP_Context &) override;
    virtual int		 saveCookedData(std::ostream &os, OP_Context &,
				int binary = 0) override;
    virtual const char	*getFileExtension(int bin) const override;

    // Sets a USD attribute or relationship on a USD primitive based on the
    // current value of a node parameter.
    void		 setAttributeFromParameter(
				HUSD_AutoWriteLock &writelock,
				const UT_String &primPath,
				const PRM_Template *tplate,
				const UT_String &propName,
				const UT_String &propType,
				const UT_String &valueType,
				const UT_String &interpolation,
				fpreal frame);

    // Helps with convex hull generation by looking at the context option
    // stack information implemented by the virtual contextOptionsStackEffect.
    void		 encloseInputs(OP_EnclosedItems &items,
				int stackdepth) const;

    void		 addSystemError(const char *msg = 0)
			 { getLockedErrorManager()->systemError(msg); }
    void		 addError(int code, const char *msg = 0)
			 { getLockedErrorManager()->
			     addError(LOP_OPTYPE_NAME, code, msg); }
    void		 addWarning(int code, const char *msg = 0)
			 { getLockedErrorManager()->
			     addWarning(LOP_OPTYPE_NAME, code, msg); }
    void		 addMessage(int code, const char *msg = 0)
			 { getLockedErrorManager()->
			     addMessage(LOP_OPTYPE_NAME, code, msg); }

private:
    void		 setDebugDirtyRecursive(
				void *change_data, bool top_level);
    void		 resetDataHandle();

    HUSD_DataHandle	 myDataHandle;
    LOP_BypassState	 myLastCookBypassState;
    int			 myEvaluatingInputNameIndex;
};

#endif
