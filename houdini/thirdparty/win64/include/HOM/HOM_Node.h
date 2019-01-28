/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Node_h__
#define __HOM_Node_h__

#include "HOM_API.h"
#include "HOM_NetworkMovableItem.h"
#include "HOM_BinaryString.h"
#include "HOM_EnumModules.h"
#include "HOM_IterableList.h"
#include "HOM_PtrOrNull.h"
#include <PY/PY_OpaqueObject.h>
#include <string>
#include <vector>

class HOM_StickyNote;
class HOM_NodeConnection;
class HOM_NodeGroup;
class HOM_NodeType;
class HOM_NodeTypeCategory;
class HOM_Parm;
class HOM_ParmTuple;
class HOM_IndirectInput;
class HOM_SubnetIndirectInput;
class HOM_HDAModule;
class HOM_ParmTemplate;
class HOM_DopSimulation;
class HOM_DopObject;
class HOM_ParmTemplateGroup;
class HOM_NodeInfoTree;
class CL_Clip;

SWIGOUT(%rename(Node) HOM_Node;)

class HOM_API HOM_Node : virtual public HOM_NetworkMovableItem
{
public:
    HOM_Node()
	: HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Node(const HOM_Node &node)
	: HOM_NetworkMovableItem(node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Node()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::vector<std::string> _getArgumentAutoComplete(
        const char *method_name, const char *arguments,
	bool meta_info_only = false) = 0;

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_Node> node) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Node> node) = 0;

    virtual std::string __str__() = 0;

    SWIGOUT(%kwargs infoTree;)
    virtual HOM_NodeInfoTree infoTree(
	bool verbose = false, bool debug = false) = 0;
    virtual bool isNetwork() = 0;

    SWIGOUT(%newobject type;)
    virtual HOM_NodeType *type() = 0;

    virtual HOM_NodeTypeCategory *childTypeCategory() = 0;

    virtual bool isFlagReadable(HOM_EnumValue &flag) = 0;
    virtual bool isFlagWritable(HOM_EnumValue &flag) = 0;
    virtual bool isGenericFlagSet(HOM_EnumValue &flag) = 0;
    virtual void setGenericFlag(HOM_EnumValue &flag, bool value) = 0;

    virtual bool isDisplayDescriptiveNameFlagSet() = 0;
    virtual void setDisplayDescriptiveNameFlag(bool on) = 0;

    // We need a quick way to get the OP_OpTypeId from C++ code so we know
    // which subclass of HOM_Node we should cast to, so that's why
    // opTypeIdAsInt() exists.
    SWIGOUT(%ignore opTypeIdAsInt;)
    virtual int opTypeIdAsInt() = 0;
    // Further, we need a quick way to get the OBJ_OBJECT_TYPE from C++ code
    // so we know which subclass of HOM_ObjNode we should cast to.
    // That's why  objTypeAsInt() exists.
    SWIGOUT(%ignore objTypeAsInt;)
    virtual int objTypeAsInt() = 0;

    SWIGOUT(%newobject node;)
    SWIGOUT(%kwargs node;)
    virtual HOM_Node* node(const char* node_path) = 0;

    SWIGOUT(%kwargs nodes;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes(
	const std::vector<std::string>& node_paths) = 0;

    SWIGOUT(%newobject item;)
    SWIGOUT(%kwargs item;)
    virtual HOM_NetworkMovableItem* item(const char* item_path) = 0;

    SWIGOUT(%kwargs items;)
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > items(
	const std::vector<std::string>& item_paths) = 0;

    SWIGOUT(%kwargs glob;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > glob(
	const char *pattern, bool ignore_case=false) = 0;

    SWIGOUT(%kwargs recursiveGlob;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > recursiveGlob(
	const char *pattern,
	HOM_EnumValue &filter = HOM_nodeTypeFilter::NoFilter) = 0;

    SWIGOUT(%newobject createNode;)
    SWIGOUT(%kwargs createNode;)
    virtual HOM_Node* createNode(
	const char* node_type_name,
	const char* node_name=NULL,
	bool run_init_scripts=true,
	bool load_contents=true,
	bool exact_type_name=false) = 0;
    virtual void runInitScripts() = 0;
    virtual void createOrMoveVisualizer(int output_index) = 0;

    virtual int inputIndex(const char *input_name) = 0;
    virtual int outputIndex(const char *output_name) = 0;

    virtual void setInput(
	int input_index,
	HOM_NetworkMovableItem *item_to_become_input,
	int output_index = 0) = 0;

    virtual void setNamedInput(
	const char *input_name,
	HOM_NetworkMovableItem *item_to_become_input,
	const char *output_name) = 0;

    virtual void setNamedInput(
	const char *input_name,
	HOM_NetworkMovableItem *item_to_become_input,
	int output_index) = 0;

    virtual void setFirstInput(
	HOM_NetworkMovableItem *item_to_become_input,
	int output_index = 0) = 0;

    virtual void setNextInput(
	HOM_NetworkMovableItem *item_to_become_input,
	int output_index = 0,
	bool unordered_only = false) = 0;

    virtual void insertInput(
	int input_index,
	HOM_NetworkMovableItem *item_to_become_input,
	int output_index = 0) = 0;

    virtual int numOrderedInputs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > inputs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> > inputConnections() = 0;

    SWIGOUT(%newobject inputConnectors;)
    virtual HOM_IterableList<std::vector<HOM_ElemPtr<HOM_NodeConnection> > > *
	inputConnectors() = 0;

    SWIGOUT(%kwargs inputAncestors;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > inputAncestors(
        bool include_ref_inputs = true,
        bool follow_subnets = false) = 0;

    virtual std::vector<std::string> inputNames() = 0;
    virtual std::vector<std::string> inputLabels() = 0;

    virtual std::string editableInputString(int input_index,
	    const char *key) = 0;
    virtual void setEditableInputString(int input_index,
	    const char *key, const char *value) = 0;

    virtual std::vector<std::string> outputNames() = 0;
    virtual std::vector<std::string> outputLabels() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > outputs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> >
	outputConnections() = 0;

    SWIGOUT(%newobject outputConnectors;)
    virtual HOM_IterableList<std::vector<HOM_ElemPtr<HOM_NodeConnection> > > *
	outputConnectors() = 0;

    // Dependency information.
    SWIGOUT(%kwargs references;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> >
	references(bool include_children=true) = 0;
    SWIGOUT(%kwargs dependents;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> >
	dependents(bool include_children=true) = 0;

    // Access things contained in this node (node must be a network).
    virtual std::vector<HOM_ElemPtr<HOM_Node> > children() = 0;
    SWIGOUT(%kwargs allSubChildren;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > allSubChildren(
	bool top_down = true, bool recurse_in_locked_nodes = true) = 0;
    SWIGOUT(%kwargs selectedChildren;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > selectedChildren(
	bool include_hidden = false,
	bool include_hidden_support_nodes = false) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > allItems() = 0;
    SWIGOUT(%kwargs selectedItems;)
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > selectedItems(
	bool include_hidden = false,
	bool include_hidden_support_nodes = false) = 0;
    SWIGOUT(%kwargs numItems;)
    virtual int numItems(HOM_EnumValue *item_type = NULL,
	bool selected_only = false,
	bool include_hidden = false) = 0;

    // Picked child item copy/paste/delete methods.
    virtual void deleteItems(
	const std::vector<HOM_NetworkMovableItem *> &items =
	    std::vector<HOM_NetworkMovableItem *>()) = 0;
    virtual void copyItemsToClipboard(
	const std::vector<HOM_NetworkMovableItem *> &items =
	    std::vector<HOM_NetworkMovableItem *>()) = 0;
    virtual void pasteItemsFromClipboard(HOM_Vector2 *position = NULL) = 0;
    SWIGOUT(%kwargs copyItems;)
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > copyItems(
	const std::vector<HOM_NetworkMovableItem *> &items,
	bool channel_reference_originals=false,
	bool relative_references=true,
	bool connect_outputs_to_multi_inputs=false) = 0;

    SWIGOUT(%newobject creator;)
    virtual HOM_Node *creator() = 0;
    virtual bool isInsideLockedHDA() = 0;
    virtual bool isEditableInsideLockedHDA() = 0;
    virtual bool isEditable() = 0;
    virtual bool isLockedHDA() = 0;
    virtual bool isCompiled() = 0;

    SWIGOUT(%newobject hdaModule;)
    virtual HOM_HDAModule *hdaModule() = 0;

    // hm is an alias for hdaModule.
    SWIGOUT(%newobject hm;)
    virtual HOM_HDAModule *hm()
    { return hdaModule(); }

    SWIGOUT(%newobject moveToGoodPosition;)
    SWIGOUT(%kwargs moveToGoodPosition;)
    virtual HOM_Vector2 *moveToGoodPosition(bool relative_to_inputs = true,
	bool move_inputs = true,
	bool move_outputs = true,
	bool move_unconnected = true) = 0;

    // TODO: Have an argument to specify sorting orders once bug 25998 is fixed.
    //	     Have an argument equivalent to oplayout -t once bug 26018 is fixed.
    SWIGOUT(%kwargs layoutChildren);
    virtual void layoutChildren
	(const std::vector<HOM_NetworkMovableItem *> &items =
	    std::vector<HOM_NetworkMovableItem *>(), 
	 double horizontal_spacing=-1, double vertical_spacing=-1) = 0;

    virtual bool isCurrent() = 0;
    SWIGOUT(%kwargs setCurrent;)
    virtual void setCurrent(bool on, bool clear_all_selected = false) = 0;

    virtual bool isHidden() = 0;
    virtual void hide(bool on) = 0;

    virtual std::string comment() = 0;
    virtual void setComment(const char *comment) = 0;
    virtual void appendComment(const char *comment) = 0;

    virtual std::string creatorState() = 0;
    virtual void setCreatorState(const char *state) = 0;

    virtual bool isBuiltExplicitly() = 0;
    virtual void setBuiltExplicitly(bool built_explicitly) = 0;

    virtual HOM_EnumValue *expressionLanguage() = 0;
    virtual void setExpressionLanguage(HOM_EnumValue &language) = 0;

    SWIGOUT(%newobject parm;)
    virtual HOM_Parm *parm(const char* parm_path) = 0;

    SWIGOUT(%newobject parmTuple;)
    virtual HOM_ParmTuple *parmTuple(const char* parm_path) = 0;

    SWIGOUT(%kwargs globParms;)
    virtual std::vector<HOM_ElemPtr<HOM_Parm> > globParms(
	const char *pattern, bool ignore_case=false, 
    bool search_label=false,
    bool single_pattern=false) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > parms() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > parmsReferencingThis() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTuple> > parmTuples() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > spareParms() = 0;
    virtual void removeSpareParms() = 0;

    virtual void syncNodeVersionIfNeeded(const std::string &from_version) = 0;

    SWIGOUT(%kwargs setParmTemplateGroup;)
    virtual void setParmTemplateGroup(
	HOM_ParmTemplateGroup &parm_template_group,
	bool rename_conflicting_parms=false) = 0;


    virtual std::vector<HOM_ElemPtr<HOM_ParmTuple> > parmTuplesInFolder(
	const std::vector<std::string> &folder_names) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > parmsInFolder(
	const std::vector<std::string> &folder_names) = 0;

    SWIGOUT(%newobject parmTemplateGroup;)
    virtual HOM_ParmTemplateGroup *parmTemplateGroup() = 0;

    virtual std::vector<std::string> localVariables() = 0;

    virtual bool isTimeDependent() = 0;

    virtual void destroy() = 0;

    // If propagate==true, unlock will be propagated to ancestors
    SWIGOUT(%kwargs allowEditingOfContents;)
    virtual void allowEditingOfContents(bool propagate=false) = 0;
    virtual void matchCurrentDefinition() = 0;
    virtual bool matchesCurrentDefinition() = 0;

    SWIGOUT(%kwargs parmAliases;)
    virtual std::map<HOM_ElemPtr<HOM_Parm>, std::string> 
	parmAliases(bool recurse=false) = 0;

    virtual void clearParmAliases() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NetworkBox> > networkBoxes() = 0;

    SWIGOUT(%newobject iterNetworkBoxes;)
    virtual HOM_IterableList<HOM_NetworkBox> *iterNetworkBoxes() = 0;

    SWIGOUT(%newobject createNetworkBox;)
    virtual HOM_NetworkBox *createNetworkBox(const char *name=NULL) = 0;

    SWIGOUT(%newobject copyNetworkBox;)
    virtual HOM_NetworkBox *copyNetworkBox(HOM_NetworkBox &network_box_to_copy,
    	const char *new_name=NULL, bool channel_reference_original=false) = 0;

    SWIGOUT(%newobject findNetworkBox;)
    virtual HOM_NetworkBox *findNetworkBox(const char *name) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NetworkBox> > 
	findNetworkBoxes(const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NetworkDot> > networkDots() = 0;

    SWIGOUT(%newobject createNetworkDot;)
    virtual HOM_NetworkDot *createNetworkDot() = 0;

    SWIGOUT(%kwargs changeNodeType);
    SWIGOUT(%newobject changeNodeType;)
    virtual HOM_Node *changeNodeType(const char *new_node_type,
	bool keep_name=true, bool keep_parms=true, 
	bool keep_network_contents=true,
	bool force_change_on_node_type_match=false) = 0;

    virtual bool canCreateDigitalAsset() = 0;
    SWIGOUT(%kwargs createDigitalAsset);
    SWIGOUT(%newobject createDigitalAsset;)
    virtual HOM_Node *createDigitalAsset(const char *name=NULL, 
	const char *hda_file_name=NULL, const char *description=NULL, 
	int min_num_inputs=0, int max_num_inputs=0, 
	bool compress_contents=false, const char *comment=NULL, 
	const char *version=NULL, bool save_as_embedded=false, 
	bool ignore_external_references=false,
	bool compile_asset = false,
	bool change_node_type = true,
	bool create_backup = true) = 0;

    SWIGOUT(%kwargs createCompiledDigitalAsset);
    virtual void createCompiledDigitalAsset(const char *name=NULL, 
	const char *hda_file_name=NULL, const char *description=NULL) = 0;

    // Sub-Network methods.
    SWIGOUT(%kwargs collapseIntoSubnet;)
    SWIGOUT(%newobject collapseIntoSubnet;)
    virtual HOM_Node *collapseIntoSubnet(
        const std::vector<HOM_NetworkMovableItem*> &child_items,
	const char *subnet_name=NULL, const char *subnet_type=NULL) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> >
	extractAndDelete() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_SubnetIndirectInput> >
	indirectInputs() = 0;

    // get the nodes inside a subnet that feed its outputs
    virtual std::vector<HOM_ElemPtr<HOM_Node> >
	subnetOutputs() = 0;

    virtual bool isSubNetwork() = 0;

    // User data methods:
    virtual void setUserData(const char *name, const std::string &value) = 0;
    SWIGOUT(%ignore boostAnyUserData;)
    virtual hboost::any boostAnyUserData(const char *name) = 0;
    virtual void destroyUserData(const char *name) = 0;
    virtual std::map<std::string, HOM_BinaryString> userDataDict() = 0;

#ifdef SWIG
%extend
{
    InterpreterObject userData(const char *name)
    { return HOMboostAnyToInterpreterObject(self->boostAnyUserData(name)); }
}
#endif


    // Cached user data methods:
    SWIGOUT(%ignore setOpaqueCachedUserData;)
    virtual void setOpaqueCachedUserData(
	const char *name, void *opaque_py_object) = 0;
    SWIGOUT(%ignore opaqueCachedUserData;)
    virtual void *opaqueCachedUserData(const char *name) = 0;
    virtual void destroyCachedUserData(const char *name) = 0;
    virtual std::map<std::string, PY_OpaqueObject> cachedUserDataDict() = 0;

#ifdef SWIG
%extend
{
    void setCachedUserData(const char *name, InterpreterObject value)
    { return self->setOpaqueCachedUserData(name, value); }

    InterpreterObject cachedUserData(const char *name)
    { return HOMincRef((InterpreterObject)self->opaqueCachedUserData(name)); }
}
#endif

    // Data Block methods (like user data, but bigger blobs with associated
    // data types).
    virtual std::vector<std::string> dataBlockKeys(
	const char *blocktype = nullptr) = 0;
    virtual std::string dataBlockType(const char *key) = 0;
    virtual HOM_BinaryString dataBlock(const char *key) = 0;
    virtual void setDataBlock(const char *key, const char *data,
	const char *blocktype) = 0;

    // asCode methods:
    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(bool brief=false,
	bool recurse=false,
	bool save_channels_only=false,
	bool save_creation_commands=true,
	bool save_keys_in_frames=false,
	bool save_outgoing_wires=false,
	bool save_parm_values_only=false,
	bool save_spare_parms=true,
	const char *function_name=NULL) = 0;

    virtual bool canGenerateCookCode(bool check_parent=false) = 0;
    SWIGOUT(%newobject cookCodeGeneratorNode;)
    virtual HOM_Node *cookCodeGeneratorNode() = 0;
    virtual std::string cookCodeLanguage() = 0;
    virtual bool supportsMultiCookCodeContexts() = 0;
   

    SWIGOUT(%kwargs saveCookCodeToFile;)
    virtual void saveCookCodeToFile(
	const char *file_name, bool skip_header=false,
	const char *context_name = NULL) = 0;

    // This method is deprecated.
    void saveCodeToFile(const char *file_name, bool skip_header=false,
			const char *context_name = NULL)
    { saveCookCodeToFile(file_name, skip_header, context_name); }


    virtual void saveCompiledCookCodeToFile(const char *file_name, 
	const char *context_name = NULL) = 0;

    // This method is deprecated.
    void saveToCompiledVexFile(const char *file_name, 
			      const char *context_name = NULL)
    { saveCompiledCookCodeToFile(file_name, context_name); }


    SWIGOUT(%kwargs cook;)
    virtual void cook(
	bool force = false,
	const std::vector<double> frame_range = std::vector<double>()) = 0;

    /// Determine if this node to cook at the current evaluation time
    virtual bool needsToCook() = 0;
    /// Determine if this node to cook at the given time
    virtual bool needsToCook(double time) = 0;

    virtual int cookCount() = 0;

    virtual void updateParmStates() = 0;

    virtual std::string deleteScript() = 0;
    virtual void setDeleteScript(const std::string &script_text,
	HOM_EnumValue &language = HOM_scriptLanguage::Python) = 0;

    // Sticky methods
    SWIGOUT(%newobject createStickyNote;)
    virtual HOM_StickyNote *createStickyNote(const char *name=NULL) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_StickyNote> > stickyNotes() = 0;

    SWIGOUT(%newobject iterStickyNotes;)
    virtual HOM_IterableList<HOM_StickyNote> *iterStickyNotes() = 0;

    SWIGOUT(%newobject copyStickyNote;)
    virtual HOM_StickyNote *copyStickyNote(HOM_StickyNote &sticky_note_to_copy,
    	const char *new_name=NULL) = 0;

    SWIGOUT(%newobject findStickyNote;)
    virtual HOM_StickyNote *findStickyNote(const char *name) = 0;

    SWIGOUT(%newobject findStickyNotes;)
    virtual std::vector<HOM_ElemPtr<HOM_StickyNote> > findStickyNotes(
        const char *pattern) = 0;


    SWIGOUT(%newobject addNodeGroup;)
    virtual HOM_NodeGroup *addNodeGroup(const char *name = NULL) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NodeGroup> > nodeGroups() = 0;

    SWIGOUT(%newobject nodeGroup;)
    virtual HOM_NodeGroup *nodeGroup(const char *name) = 0;

    virtual std::vector<std::string> errors() = 0;
    virtual std::vector<std::string> warnings() = 0;
    virtual std::vector<std::string> messages() = 0;

    virtual std::string motionEffectsNetworkPath() = 0;
    SWIGOUT(%newobject findOrCreateMotionEffectsNetwork;)
    virtual HOM_Node *findOrCreateMotionEffectsNetwork(bool create=true) = 0;

    SWIGOUT(%kwargs saveItemsToFile;)
    virtual void saveItemsToFile(
	const std::vector<HOM_NetworkMovableItem *> &items, 
	const char *file_name,
	bool save_hda_fallbacks = false) = 0;
    SWIGOUT(%kwargs loadItemsFromFile;)
    virtual void loadItemsFromFile(
	const char *file_name, bool ignore_load_warnings=false) = 0;

    SWIGOUT(%ignore addOpaqueEventCallback;)
    virtual void addOpaqueEventCallback(
	const std::vector<HOM_EnumValue *> &event_types,
	void *py_callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventCallback;)
    virtual void removeOpaqueEventCallback(
	const std::vector<HOM_EnumValue *> &event_types,
	void *py_callback) = 0;
    virtual void removeAllEventCallbacks() = 0;
    SWIGOUT(%ignore opaqueEventCallbacks;)
    virtual std::vector<std::pair<std::vector<HOM_EnumValue *>, void *> > 
	opaqueEventCallbacks() = 0;

    virtual double stampValue(const char *name, double default_value) = 0;
    virtual std::string stampValue(
	const char *name, const char *default_value) = 0;

    virtual void *_asVoidPointer() = 0;

    SWIGOUT(%newobject simulation;)
    virtual HOM_DopSimulation *simulation() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> >
	findNodesThatProcessedObject(HOM_DopObject &dop_object) = 0;

    virtual HOM_BinaryString _parmClipData(double start,
					   double end,
					   bool binary = true,
					   bool use_blosc_compression = true,
					   double sample_rate = 0,
					   bool scoped_only = false) = 0;

    SWIGOUT(%kwargs setParmClipData;)
    virtual void setParmClipData(HOM_BinaryString data,
				 bool binary = true,
				 bool blosc_compressed = true,
				 double sample_rate = 0,
				 double start = 1) = 0;

    virtual void _saveParmClip(std::string file_name,
			       double start,
			       double end,
			       double sample_rate = 0,
			       bool scoped_only = false) = 0;

    SWIGOUT(%kwargs loadParmClip;)
    virtual void loadParmClip(std::string file_name,
			      double sample_rate = 0,
			      double start = 1) = 0;

    virtual int __creationTime() = 0;
    virtual int __modificationTime() = 0;

#ifdef SWIG
%extend
{
    // evalParm() and evalParmTuple() need to be special methods because
    // their return types can vary.
    InterpreterObject evalParm(const char *parm_path)
    {
	HOM_Parm *parm = self->parm(parm_path);

	if (!parm)
	    throw HOM_OperationFailed("Invalid parm name");

	InterpreterObject result = HOMevalParm(*parm);
	delete parm;
	return result;
    }

    InterpreterObject evalParmTuple(const char *parm_path)
    {
	HOM_ParmTuple *parm_tuple = self->parmTuple(parm_path);

	if (!parm_tuple)
	    throw HOM_OperationFailed("Invalid parm tuple name");

	InterpreterObject result = HOMevalParmTuple(*parm_tuple);
	delete parm_tuple;
	return result;
    }

    void addEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types,
	    InterpreterObject callback)
    { self->addOpaqueEventCallback(event_types, callback); }

    void removeEventCallback(
	    const std::vector<HOM_EnumValue *> &event_types,
	    InterpreterObject callback)
    { self->removeOpaqueEventCallback(event_types, callback); }


    std::vector<std::pair<std::vector<HOM_EnumValue *>, InterpreterObject> >
	eventCallbacks()
    {
	std::vector<std::pair<std::vector<HOM_EnumValue *>, void *> > 
	    opaque_callbacks(self->opaqueEventCallbacks());

	std::vector<std::pair<std::vector<HOM_EnumValue *>, InterpreterObject> >
	    callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back(std::make_pair(
		opaque_callbacks[i].first, 
		(InterpreterObject)opaque_callbacks[i].second));
	return callbacks;
    }
}
#endif
};

#endif
