/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Operator.h (C++)
 *
 * COMMENTS:
 *	An OP_Operator instance defines an operator which is registered
 *	with an OP_Network and subsequently allows nodes of this type to
 *	be created in networks of the corresponding type. (This is the
 *	new equivalent of the old *_TableEntry class).
 *
 */

#ifndef __OP_Operator_h__
#define __OP_Operator_h__

#include "OP_API.h"
#include <UT/UT_String.h>
#include <UT/UT_DeepString.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>
#include <FS/FS_IndexFile.h>
#include <VEX/VEX_VexTypes.h>
#include <VEX/VEX_RslTypes.h>
#include <PRM/DS_Stream.h>
#include <PRM/PRM_ScriptPage.h>
#include <iosfwd>
#include "OP_OTLDefinition.h"
#include "OP_OTLLicenseType.h"
#include "OP_Value.h"
#include "OP_Node.h"

// The VOPNET parameter name that defines the english name of
// the operator defined by that VOPNET and a flag that indicates
// whether the defined operator should be in the tab menu.
#define ENGLISH_NAME		    "englishname"
#define VOPNET_TAB_MENU_FLAG_NAME   "tabmenuflag"

class UT_FileUtil;
class UT_StringArray;
class UT_InfoTree;
class UT_WorkBuffer;
class UT_Options;
class PY_EvaluationContext;
class PY_CompiledCode;
class FS_IndexFile;
class CH_LocalVariable;
class PRM_Template;
class PRM_SimpleParm;
class PRM_PresetInfo;
class PRM_ScriptImports;
class OP_ExtraInfoBuffer;
class OP_Network;
class OP_Node;
class OP_OperatorTable;
class OP_Operator;
class OP_OTLLibrary;
class OP_SpareParmCache;
class OP_SpareParms;
class OP_UpdateTemplatesSink;
class OP_EventScriptPathCache;

/// Flags passed to the contructor:
#define OP_FLAG_NETWORK			0x01
#define OP_FLAG_GENERATOR		0x02 /// Generates new data
#define OP_FLAG_UNORDERED		0x04 /// Unordered, variable inputs
#define OP_FLAG_SCRIPTDEF		0x08 /// Defined by a dialog script
#define OP_FLAG_MANAGER			0x10 /// This is a management node.
#define OP_FLAG_PRIMARYSUBNET		0x20 /// The primary "subnet" operator
#define OP_FLAG_OUTPUT			0x40 /// Output type. Only one per net
#define OP_FLAG_EDITABLE_INPUT_DATA	0x80 /// Enable editable input data

// Maximum number of input labels that can be specified for non-HDAs:
#define OP_MAX_INPUT_LABELS	4

// Default max value used for multi-inputs like Merge nodes
#define OP_MULTI_INPUT_MAX	9999

// Maximum number of distinct indirect inputs shown until none are shown
// (all inputs are assumed to apply to the subnet).
#define OP_MAX_INDIRECT_INPUTS  500

// Maximum number of outputs we allow for nodes that allow multiple outputs 
#define OP_MULTI_OUTPUT_MAX	99999

typedef UT_ValArray<OP_Operator *>	 OP_OperatorList;
typedef	OP_Node	*(*OP_Constructor)(OP_Network *, const char *, OP_Operator *);

// Event Script types
typedef enum {
    OP_EVENTSCRIPT_GLOBAL,
    OP_EVENTSCRIPT_OPTYPE,
    OP_EVENTSCRIPT_NODE
} OP_EventScriptType;

class OP_API OP_SpecificData
{
public:
			 OP_SpecificData() { }
    virtual		~OP_SpecificData() { }

    virtual void	 saveToDialogScript(std::ostream &os) const = 0;
    virtual void	 updateExtraInfoBuffer(OP_ExtraInfoBuffer &b) const {}
};

class OP_API OP_TemplatePair
{
public:
			 OP_TemplatePair(PRM_Template	 *templates,
					 OP_TemplatePair *parent_pair = 0)
					 {
					    myTemplate	       = templates;
					    myBaseTemplatePair = parent_pair;
					 }
    PRM_Template	*myTemplate;		// Could be zero
    OP_TemplatePair	*myBaseTemplatePair;	// Could be zero
};

class OP_API OP_VariablePair
{
public:
			 OP_VariablePair(CH_LocalVariable *variables,
					 OP_VariablePair  *parent_pair = 0)
					 {
					    myVariables = variables;
					    myBaseVariablePair = parent_pair;
					 }
    CH_LocalVariable	*myVariables;		// Could be zero
    OP_VariablePair	*myBaseVariablePair;	// Could be zero
};

class OP_API OP_InputDataEditorConfig
{
public:
    UT_StringHolder	 myDataKey;
    UT_StringHolder	 myDataHeader;
    UT_StringHolder	 myDataDefault;
};

class OP_API OP_Operator
{
public:
    /// Note: The 'inputlabels' array is expected to be null-terminated.
    OP_Operator(const char	 *name,
		const char	 *english,
		OP_Constructor    construct,
		PRM_Template	 *templates,
		unsigned	  min_sources,
		unsigned	  max_sources = 9999,
		CH_LocalVariable *variables = 0,
		unsigned	  flags = 0,
		const char	**inputlabels = 0,
		int 	          maxoutputs = 1,
		const char       *tab_submenu_path = 0);

    OP_Operator(const char	 *name,
		const char	 *english,
		OP_Constructor    construct,
		OP_TemplatePair	 *template_pair,
		unsigned	  min_sources,
		unsigned	  max_sources = 9999,
		OP_VariablePair	 *variable_pair = 0,
		unsigned	  flags = 0,
		const char	**inputlabels = 0,
		int		  maxoutputs = 1,
		const char       *tab_submenu_path = 0);

    OP_Operator(const char	 *name,
		const char	 *english,
		OP_Constructor    construct,
		PRM_Template	 *templates,
		const char	   *child_table_name,
		unsigned	  min_sources,
		unsigned	  max_sources = 9999,
		CH_LocalVariable *variables = 0,
		unsigned	  flags = 0,
		const char	**inputlabels = 0,
		int 	          maxoutputs = 1,
		const char       *tab_submenu_path = 0);

    OP_Operator(const char	 *name,
		const char	 *english,
		OP_Constructor    construct,
		OP_TemplatePair	 *template_pair,
		const char	   *child_table_name,
		unsigned	  min_sources,
		unsigned	  max_sources = 9999,
		OP_VariablePair	 *variable_pair = 0,
		unsigned	  flags = 0,
		const char	**inputlabels = 0,
		int		  maxoutputs = 1,
		const char       *tab_submenu_path = 0);

    virtual ~OP_Operator();

    OP_Node		*constructNode(OP_Network *net, const char *name);
    int			 getNumActiveNodes() const;
    OP_Node		*getActiveNode(int i) const;
    void		 nodeDestroyed(OP_Node *node);

    // Because of sub-classing of this class, we require the ability to set
    // "specific" data for different network types.  For example, SHOPs are
    // required to know the "rendermask" and "shader type" for each OP.
    virtual OP_SpecificData		*getOpSpecificData()
					 { return 0; }
    virtual const OP_SpecificData	*getOpSpecificData() const
					 { return 0; }

    const UT_StringHolder &getName() const
			 { return myOTLDefinition.getName(); }

    /// Get the op table name and the op name concatenated together to get a
    /// unique name.
    /// Returns full name with table, including namespace and version tags
    /// if there are any, eg "artistA::table/op::2.0"
    void		 getTableAndName(UT_WorkBuffer &name) const
			 { myOTLDefinition.getTableAndName(name); }
    void		 getTableAndName(UT_String &name) const
			 { myOTLDefinition.getTableAndName(name); }

    const UT_StringHolder &getEnglish() const
			 { return myOTLDefinition.getLabel(); }
    UT_Color		 getDefaultColor() const;
    const UT_StringHolder &getDefaultShape() const;

    virtual bool	 getHDKHelp(UT_String & /*str*/) const
			 { return false; }

    virtual bool	 getOpHelpURL(UT_String &url);
    virtual bool         getOpTabSubMenuPath(UT_String &tab_submenu_path);
    void		 setOpTabSubMenuPath(const char *tab_submenu_path);

    bool		 getOTLHelp(UT_String &str);
    void		 getEditableSubNodes(UT_String &pattern);
    void		 getMessageSubNodes(UT_String &pattern);
    void		 getDiveTarget(UT_String &divetarget);
    void		 getDescriptiveParmName(UT_String &descrparm);
    void		 getDefaultState(UT_String &statename);
    void		 getFunctionName(UT_String &function_name);
    void		 getSecondaryFunctionsNames( 
			    UT_StringMap<UT_StringHolder> &functions );

    void		 getComment(UT_String &comment);

    /// For HDAs, getVersion will return the version number defined by its HDA
    /// definition, or the empty string if it does not exist. For non-HDA's
    /// (ie. native or HDK node types), it will return the Houdini version
    /// X.Y.Z.W string that we were compiled with. Subclasses may override this
    /// to provide different numbering. One of the uses of this function to
    /// determine if loading a node's parameters needs to be upgraded.
    virtual bool	 getVersion(UT_String &version);
				 
    void		 getHelpFileDirAndName(UT_String & script_dir, 
					UT_String & name);

    void		 getDefinitionSource(UT_String &defsource) const;

    static void		 clearOperatorHelpCache()
			 { ourLastHelpOp = 0; }
    OP_Node		*getDefiningNetwork() const;
    
    // These functions are used by scripted ops.
    // IndexPath is something like "subnet".
    // IndexFile is something ike "SUBsop".
    const UT_StringHolder &getIndexPath() const
			 { return myOTLDefinition.getIndexPath(); }
    const UT_StringHolder &getIndexFile() const
			 { return myOTLDefinition.getIndexFile(); }
    bool		 getScriptIsSubnet() const
			 { return myOTLDefinition.getIsSubnet(); }
    bool		 getScriptIsVex() const
			 { return (myVexContextType != VEX_INVALID_CONTEXT); }
    bool		 getScriptIsRsl() const
			 { return (myRslContextType != RSL_INVALID_CONTEXT); }
    VEX_ContextType	 getVexContextType() const
			 { return myVexContextType; }
    RSL_ContextType	 getRslContextType() const
			 { return myRslContextType; }
    int			 getModTime() const
			 { return myOTLDefinition.getModTime(); }
    bool		 getIsDummyDefinition() const
			 { return myOTLDefinition.getIsDummyDefinition(); }

    // Does this node type cook by running python code?
    bool		 getScriptIsPython() const
	    		 { return myOTLDefinition.getIsPython(); }
    void		 setScriptIsPython(bool is_python)
			{ myOTLDefinition.setIsPython(is_python); }

    // Returns the extra info string from our OTL definition
    const UT_StringHolder &getExtraInfo() const
			 { return myOTLDefinition.getExtraInfo(); }

    // Get and set all our OTL Definition information.
    void		 setOTLDefinition(const OP_OTLDefinition &definition);
    const OP_OTLDefinition& getOTLDefinition() const
			 { return myOTLDefinition; }

    // Get and set our OP_OTLLibrary.
    void		 setOTLLibrary(OP_OTLLibrary *library)
			 { myOTLLibrary = library; }
    OP_OTLLibrary	*getOTLLibrary()
			 { return myOTLLibrary; }

    bool		 hasContentsSection();

    // Get reference to our FS_IndexFile from myOTLLibrary.
    FS_IndexFile	*getOTLIndexFile()
			 { return getOTLIndexFilePtr().get(); }

    // Get underlying shared ptr of getOTLIndexFile().
    // This not const because it updates myOTLIndexFile.
    FS_IndexFileHandle	 getOTLIndexFilePtr();


    // Force the OTL-stored information for this operator to be cleared
    // out and reloaded. We do this when the OTL has changed or we are
    // changing the OTL that defines this operator.
    virtual void	 clearOTLIndexFile();

    // Create a new FS_IndexFile that contains all the info we need about
    // this operator. It is the responsibility of the caller to delete
    // the return pointer.
    FS_IndexFile	*createDefaultOTLIndexFile() const;
    FS_IndexFile	*createOTLIndexFile(bool reservedsectionsonly,
					    UT_StringArray *externalfiles);

    /// Returns an official name of the library section that contains 
    /// the HDA definition eg "Object/hda".
    void		 getDefinitionSectionName(UT_String &sectname) const;
    
    /// @{
    /// Returns a path that refers to an index file section within
    /// an OTL library or an HDA definition (depending on opdef_or_oplib prefix,
    /// which should be either OTL_LIBRARY_PREFIX or OTL_DEFINITION_PREFIX).
    /// The method version that takes a string array instead of a string
    /// is intended for nested sections where the sections towards the end of 
    /// the array are innermost sections within sections earlier in the array 
    /// (that are themselves index files).  For example, the path may be
    /// "opdef:/Object/hda?section" or
    /// "opdef:/artistA::Object/hda::2.0?section, or
    /// "opdef:/Sop/hda?outer_section.idx?inner_section, etc
    void		 constructIndexFileSectionPath(UT_String &path,
					 const char *opdef_or_oplib,
					 const char *section_name) const;
    void		 constructIndexFileSectionPath(UT_String &path,
					 const char *opdef_or_oplib,
					 const UT_StringArray &section_names) 
									const;
    /// @}
			
    /// List all operators found in the contents section
    void		 listContainedOperators(UT_StringSet &contents_list);

    // The icon name is used to determine what the icon displayed is.
    // Typically, this is the operator name preceded by the op type.
    const char		*getIconName() const;
    void		 setIconName(const char *name);
    void		 setDefaultIconName();

    unsigned		 minInputs() const
			 { return myOTLDefinition.getMinInputs(); }
    unsigned		 maxInputs() const
			 { return myOTLDefinition.getMaxInputs(); }
	
    /// @{ Sets an array that maps an old input index to a new input name.
    /// It is used for handling legacy H11 hip files, where nodes were saved
    /// with indices only. If an operator definition is changed (ie, inputs 
    /// get inserted or reordered), it is necessary to provide this mapping,
    /// to be able to load legacy hip files. If op definition does not change,
    /// there is no need to define this mapping, since the default assignment
    /// of input index will still work. 
    /// Also it is not needed for handling newer hip files that save the input 
    /// name information and can handle input order change in new op definition.
    /// The map is given as a sequence of input names eg "name1 name2".
    void		  setLegacyInputsMap(const char *map);
    void		  getLegacyInputsMap(UT_String &map) const;
    const UT_StringArray *getLegacyInputsIndexMap() const;
    /// @}

    // Management operators can't have outputs.  Otherwise the general
    // procedure (except for VOPs) is one output.
    virtual unsigned	 maxOutputs()
			 {
			     return isManagementOp()
				 ? 0
				 : myOTLDefinition.getMaxOutputs();
			 }

    OP_SpareParms	*loadSpareParms(UT_IStream &is);

    virtual bool	  hasLoadedParmTemplates() const;
    virtual int		  updateParmTemplates();

    // This function forces a reload of the operator's parm templates.
    virtual void	  forceUpdateParmTemplates();

    /// Returns the operator's parameter templates with base parameters
    /// appearing first (if they exist for script/HDA operators) followed by
    /// non-base parameters.
    ///
    /// If getLayoutParmTemplates() returns nullptr, then getParmTemplates()
    /// can be used for the parameter layout.
    PRM_Template 	*getParmTemplates();
    PRM_ScriptImports	*getParmTemplateImports();

    /// Returns the operator's parameter templates in their final parameter
    /// layout.  This can return nullptr if the operator is not a script/HDA /
    /// operator and if the operator's parameter layout is not overridden by a
    /// custom dialog script.
    PRM_Template 	*getLayoutParmTemplates();
    PRM_ScriptImports	*getLayoutParmTemplateImports();

    /// Exists only for script/HDA operators.
    /// Returns the parm templates that are base to the HDA parameters.
    virtual const PRM_Template	*getBaseParmTemplates() { return myBase; }

    /// Returns the parm templates that are base to the final parameter layout
    /// if the layout is overridden by a custom dialog script.  Returns
    /// nullptr if the parameter layout is not overridden.
    PRM_Template	*getBaseOverrideLayoutParmTemplates()
			    { return myBaseOverrideLayout; }

    virtual PRM_Template *getObsoleteTemplates()
			  { return myObsoleteTemplates; }
    void		  setObsoleteTemplates(PRM_Template *tmpl)
			  { myObsoleteTemplates = tmpl; }

    /// Pass back the file path of the .ds file that was loaded
    /// to override the operator's parameter templates.  Return true if such a
    /// file was found and loaded.  Return false if no .ds override file was
    /// found.
    bool		  getOverrideLayoutParmTemplateFile(
			    UT_String &ds_path) const;

    CH_LocalVariable	 *getVariable(int i) const;
    CH_LocalVariable	 *getVariable(const char *name) const;
    CH_LocalVariable	 *getVariables()     const { return myVariables; }
    int			  getVariableCount() const { return myNvariables; }

    void		  resetVariables(OP_VariablePair *variable_pair);

    unsigned		 isNetwork() const
			 {
			     return (myFlags & OP_FLAG_NETWORK) != 0;
			 }
    unsigned		 isGenerator() const
			 {
			     return (myFlags & OP_FLAG_GENERATOR) != 0;
			 }
    unsigned		 isScriptedOp() const
			 {
			     return (myFlags & OP_FLAG_SCRIPTDEF) != 0;
			 }
    bool		 isCustomDSO() const;
    unsigned		 unorderedInputs() const
			 {
			     return (myFlags & OP_FLAG_UNORDERED) != 0;
			 }
    unsigned		 isManagementOp() const
			 {
			     return (myFlags & OP_FLAG_MANAGER) != 0;
			 }
    unsigned		 isOutputOp() const
			 {
			     return (myFlags & OP_FLAG_OUTPUT) != 0;
			 }
    bool		 hasEditableInputData() const
			 {
			     return (myFlags&OP_FLAG_EDITABLE_INPUT_DATA) != 0;
			 }
    /// Test to see if this is the primary "subnet" operator for the table.
    /// This is the operator type which is used as a default container for
    /// nodes.  There should only be one of these defined.
    bool		 getIsPrimarySubnetType() const
			 {
			    return (myFlags & OP_FLAG_PRIMARYSUBNET) != 0;
			 }
    bool		 isCreatingNode() const
			 {
			     return myCreatingNode;
			 }
    bool		 isChangingParmTemplates() const
			 {
			     return myChangingParmTemplates;
			 }
    bool		 hasNodeLoadingContentsSection() const
			 {
			     return myLoadingContentsSectionCount > 0;
			 }
    void		 beginLoadingContentsSectionForNode()
			 {
			     myLoadingContentsSectionCount++;
			 }
    void		 endLoadingContentsSectionForNode()
			 {
			     myLoadingContentsSectionCount--;
			 }
			

    /// Returns true if the operator should be preserved by op director,
    /// when elimiating unused HDAs during a library refresh.
    bool		 shouldPreserve() const
			 { 
			     return getNumActiveNodes() > 0 || myRefreshingLib;
			 }
    
    enum OP_OperatorCompare {
	    OP_COMPARE_GEN_ENGLISH	= 0,	// Generators, then english
	    OP_COMPARE_GEN_NAME		= 1,	// Generators first, then name
	    OP_COMPARE_ENGLISH		= 2,	// By english solely
	    OP_COMPARE_NAME		= 3	// By name solely
    };

    // The method passed in is typically one of the enums.  However,
    // sub-classes of OP_Operator can add new comparison methods.
    virtual int		 compareOperator(const OP_Operator *other,
					 int method) const;

    // Be very very carefull calling the change parm template functions...
    void		 changeParmTemplate(PRM_Template *tp);

    /// @{ Accessing operator inputs name, label, and reference flag.
    const UT_StringHolder	 &getInputName(int idx) const;
    const UT_StringArray &getInputNames() const
			    { return myInputNames; }
    const UT_StringHolder	 &getInputLabel(int idx) const;
    const UT_StringArray &getInputLabels() const
			    { return myInputLabels; }
    bool		  getInputIsReference(int idx) const;
    const UT_Array<bool> &getInputAreReferences() const
			    { return myInputReferences; }
    virtual bool	  allowsInputNameEditing() const;
    /// @}
    /// @{ Accessing operator outputs label.
    const UT_StringHolder	 &getOutputLabel(int idx) const;
    const UT_StringArray &getOutputLabels() const
			    { return myOutputLabels; }
    /// @}

    unsigned		 getPermissionMask() const;

    // When our parm template changes, refresh all persistent handles to
    // make sure they aren't attached to a non-existant parameter.
    virtual void	 getRefreshPICommands(OP_Node *, std::ostream &) { }

    // This virtual function determines if we want animatable parameters.
    // Used when creating parameters from a dialog script.
    virtual bool	 wantsParametersAnimatable() const;
    // This function determines if we want an input editor gadget in our
    // parameter dialog if we have more than 4 inputs.
    virtual bool	 wantsInputEditor() const;
    // This function determines if we want the VOPNET parameter order
    // editor in our parameter dialog. Obviously defaults to false.
    virtual bool	 wantsParmOrderEditor() const;
    // This function determines if we want the VOP subnet output name
    // editor in our parameter dialog. Obviously defaults to false.
    virtual bool	 wantsOutputNameEditor() const;
    // Returns true if the network inside a node should have indirect inputs 
    // corresponding to that parent node's input connectors.
    virtual bool	 wantsParentInputs() const;
    // Returns a structure describing how the input data editor in the parm
    // dialog should be configured.
    const OP_InputDataEditorConfig &getInputDataEditorConfig() const
			 { return myInputDataEditorConfig; }
    void		 setInputDataEditorConfig(
				const OP_InputDataEditorConfig &config)
			 { myInputDataEditorConfig = config; }

    // Appends a short help string to the provided buffer. Generally,
    // This help string will be hard-coded.
    virtual const char	*getOperatorShortHelpString();

    // Get Info Text that is specific to this operator type. All
    // implementations should call their base class version.
    // Text should be appended to the provided buffer.
    virtual void	 getOperatorSpecificInfoText(int verbose,
						     UT_WorkBuffer &text);
    
    /// Fill in 'tree' with details that are common to all operator types. Then 
    /// call fillInfoTreeOperatorSpecific() to get specific details about this 
    /// specific operator
    void	 	 fillInfoTree(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);
    
    /// Triggered by 'fillInfoTree()', this virtual function adds all operator-
    /// specific info to 'tree'. All child classes must create their own 
    /// branches under 'tree' to place their info
    virtual void         fillInfoTreeOperatorSpecific(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);
    
    // Load operators from .optype files...
    static int		 loadFromOpType(const char *fname, const char *savedir);
    static int		 uninstallOpType(const char *path,
			    const char *tablename, const char *opname,
			    const char *indexpath, const char *indexfile);
    // Remove override files, wherever they might be hiding...
    static void		 removeHelpCardOverrides(UT_FileUtil *fu,
			    const char *path, const char *tablename,
			    const char *opname, int flags);
    static void 	 removeRenameOverrides(UT_FileUtil *fu, 
			    const char *path, const char *tablename,
			    const char *opname, int flags);
    static void 	 removeIconOverrides(UT_FileUtil *fu,
			    const char *path, const char *tablename,
			    const char *opname, int flags);
    static void		 removeCmdOverrides(UT_FileUtil *fu, 
			    const char *path, const char *tablename,
			    const char *opname, int flags);
    // Constructs the default icon name for a given op and op table.
    static void		 getDefaultIconName(const char *tablename,
					    const char *opname,
					    UT_String &iconname);

    // This returns a pointer to the table name.
    // This is one of the *_TABLE_NAME defined in OP_Operator, from
    // which any other prefix can be calculated.
    const char		*getTableName() const;

    // This returns a pointer to our actual operator table.
    OP_OperatorTable	*getTable() const;

    // Get the table name where this operator's children will go
    const UT_StringHolder       &getChildTableName() const;
    OP_OperatorTable	*getChildOperatorTable() const;

    // Query the license type associated with this operator.
    OP_OTLLicenseType	 getLicense();

    // Sets the file where this operator is defined.
    // If the definition starts with "op:", we alert the operator
    // referred to by the rest of the path that we are now attached
    // to it.
    void			 setDefinition(const char *def);
    // Change the English name of the operator.
    void			 setEnglish(const char *english);

    // the tab menu flag changed, so inform the sinks of template update
    // so that they can adjust their information about this op.
    void			 informOfTabMenuFlagChange();

    // Change the name of this operator. We have to remove and re-add
    // ourselves to the operator table to update our data properly.
    virtual void		 setName(const char *name);

    // Set the minimum and maximum number of sources. Overrides the
    // values that were passed into the constructor. These functions
    // should only be called when reloading the parm template, so
    // that changeParmTemplate will get called on OPs of this type.
    // changeParmTemplate now deals with chaning numbers of inputs
    // and outputs.
    //
    // If you want to set both, you should call setMin first as setMax
    // will verify min <= max and decrease min if required.
    void			 setMinInputs(int inputs);
    void			 setMaxInputs(int inputs);
    void			 setIsGenerator(bool isgen);

    // Functions to dirty and get the preset info for this operator.
    // The get function creates the preset info if required.
    void			 updatePresetInfo();
    PRM_PresetInfo		*getPresetInfo();

    /// Returns a file name corresponding to the operator name. 
    /// In most cases, the file name is the same as the operator name, but
    /// since it has to be acceptable by all file systems, some invalid
    /// characters may be converted to dashes 
    /// (eg, the namespace semicolons which cause problems on Windows).
    /// @{
    void			 getOpBasedFileName(UT_String &filename);
    void			 getOpBasedFileName(UT_WorkBuffer &filename);
    /// @}

    // This function runs one of the embedded scripts. It returns
    // true if the script was found, even if it was empty. The
    // nodedeletedbyscript parameter lets the caller know if the node
    // passed in as nodearg was deleted while running the script.
    bool			 runEventScript(
				    const char *event,
				    OP_Node *node_arg = NULL,
				    bool full_path = false,
				    UT_Options *extra_args = NULL,
				    bool *node_deleted_by_script = NULL);

    // Digital assets can have a pair of Python dictionaries that store
    // the contents of the asset's Python "module".  For non-DA's, these
    // dictionaries are empty.
    PY_EvaluationContext	*getPythonModuleDicts()
				{ return myPythonModuleDicts; }
    bool			 hasLoadedPythonModuleSection() const
				{ return myHasLoadedPythonModuleSection; }
    void			 loadPythonModuleSection(bool force=false);

    // Clear all paths to event scripts cached for this operator.
    void			 clearEventScriptPathCache();

    // Query all cached event script paths.
    void			 getCachedEventScriptPaths(
						UT_StringArray &events,
						UT_StringArray &paths,
						bool only_nonempty) const;

    /// Is this operator type thread-safe for cooking? OP_Node::cook() calls
    /// this to determine if serialization is needed.
    /// DEFAULT: false
    bool			 isThreadSafe() const
				{ return myIsThreadSafe; }

    /// This function is used to specify whether a particular operator type
    /// can be cooked in a multi-threaded manner.
    void			 setIsThreadSafe(bool is_safe)
				{ myIsThreadSafe = is_safe; }

    static const char		*getCurrentPythonModuleKey();
    static const OP_Operator	*mapSecurityKeyToOperator(const char *key);
    static void			 registerPythonModuleKeyClient(void *client);
    static bool			 verifyPythonModuleKeyClient(
				    const OP_Operator *op, void *client);

    static PRM_Template		*getEmptyTemplateList();

    /// Returns true if this operator comes from a compiled OTL,
    /// false otherwise.
    bool                         isCompiled();

    /// Returns true if this operator is black boxed, and false otherwise.
    bool			 isBlackBoxed();

    static int operatorNameCompare(const OP_Operator *a, const OP_Operator *b)
			{
			    return (a ? (b ? (a->getName() < b->getName())
					   : false)
				      : true);
			}

protected:
    // This function should be called from updateParmTemplates whenever
    // that function would return a 1. Also called by setName and setEnglish.
    void			 notifyUpdateTemplatesSinksOfUpdate();
    // Called when the operator is deleted.
    void			 notifyUpdateTemplatesSinksOfDelete();

    // Notify active nodes that the parameter templates have changed.
    void			 notifyActiveNodesOfTemplatesUpdate();

    // For script operators - we need to make these methods protected.
    static PRM_Template		*addTemplates(OP_TemplatePair *);
    static CH_LocalVariable	*addVariables(OP_VariablePair *, int &);

    // This is currently only used temporarily to transfer from
    // a DS file to a HDA.
    void		 setDescriptiveParmName(const UT_StringHolder &parm)
			 { myDescriptiveParmName = parm; }

    void		 dirtyParmTemplatesLoaded();

    virtual bool	 loadParmTemplatesSubclass();

    /// Load parameter templates from the specified file stream.
    bool		 loadAndCreateLayoutParmTemplates(
			    PRM_ScriptPage *page,
			    DS_Stream &ds_is, 
			    PRM_Template *&base_tplates,
			    PRM_Template *&layout_tplates, 
			    PRM_ScriptImports *&imports,
			    PRM_Template *&tplates,
			    int base_controls_switcher_index,
			    int base_controls_insert_index,
			    int base_controls_switcher_page_num,
			    PRM_Default *&base_controls_switcher_defaults,
			    bool hide_default_parms);

    PRM_Template		*myParmTemplates;
    PRM_ScriptImports		*myImports;
    PRM_Template		*myLayoutParmTemplates;
    PRM_ScriptImports		*myLayoutParmImports;
    PRM_Template		*myBaseOverrideLayout;
    PRM_Template		*myBase;
    bool			 myParmTemplatesLoaded;
    PRM_Template		*myObsoleteTemplates;
    UT_StringArray		 myInputNames;
    UT_StringArray		 myInputLabels;
    UT_StringArray		 myOutputLabels;
    UT_DeepString		 myTabSubmenuPath;
    UT_Array<bool>		 myInputReferences;
    CH_LocalVariable		*myVariables;
    int				 myNvariables;
    UT_StringHolder		 myChildTableName;

    OP_OTLDefinition		 myOTLDefinition;
    OP_InputDataEditorConfig	 myInputDataEditorConfig;
    VEX_ContextType		 myVexContextType;
    RSL_ContextType		 myRslContextType;
    unsigned			 myFlags;
    bool			 myHasContentsSection;

    // The script page used for storing the parsed override parameters
    // if they exist.  Though this class is the only owner of the page object
    // we use a shared pointer as a way of controlling when the object
    // actually gets deleted.  For example, some methods will create a local
    // shared pointer to the same object as a way of keeping it alive for the
    // method scope.
    UT_SharedPtr<PRM_ScriptPage> myOverrideLayoutParmScriptPage;

private:
    unsigned		 operator<(const OP_Operator &);
    void		 uniqueName(OP_Network *);
    void		 addUpdateTemplatesSink(OP_UpdateTemplatesSink *);
    void		 removeUpdateTemplatesSink(OP_UpdateTemplatesSink *);

    void		 getContentSectionTypeNames(UT_IStream &is,
						    UT_StringSet &contents_list) const;

    // When the VOPNET that defines us chages, we want to know about it...
    static void		 handleDefiningNetworkChange(OP_Node *caller,
						     void *callee,
						     OP_EventType type,
						     void *data);

    CH_ScriptLanguage eventScriptLanguage(
			    FS_IndexFile &def_file, const char *event);
    bool		 runEventScript(
			    const char *event,
			    OP_Node *node_arg,
			    bool full_path,
			    UT_Options *extra_args,
			    bool &node_deleted_by_script,
			    OP_EventScriptType eventtype);
    void		 runHscriptEventScript(
			    const char *script,
			    const char *script_path,
			    OP_Node *node,
			    bool full_path,
			    const char *extra_args);
    void		 runPythonEventScript(
			    const char *script,
			    const char *script_path,
			    PY_EvaluationContext &evaluation_context,
			    const char *kwargs_dict, OP_Node *node);

    // Get the script path cache for this operator, allocating it if
    // necessary.
    OP_EventScriptPathCache	*getEventScriptPathCache();

    /// Attempt to load the custom override parameter layout
    /// if it exists on disk.
    void		 loadOverrideLayoutParmTemplates();

    void		 ensureParmTemplatesLoaded();

    OP_Constructor	 myConstructor;

    OP_NodeList		 myActiveNodes;

    OP_OTLLibrary	*myOTLLibrary;
    FS_IndexFileHandle	 myOTLIndexFile;
    OP_OperatorTable	*myOperatorTable;
    OP_Node		*myDefiningNetwork;
    PRM_PresetInfo	*myPresetInfo;
    OP_SpareParmCache	*mySpareParmCache;
    OP_EventScriptPathCache	 *myEventScriptPathCache;
    bool		 myCreatingNode;
    bool		 myChangingParmTemplates;
    bool		 myRefreshingLib;
    bool		 myDestroyingSelf;
    int			 myLoadingContentsSectionCount;

    UT_ValArray<OP_UpdateTemplatesSink *>	myUpdateTemplatesSinks;

    PY_EvaluationContext *myPythonModuleDicts;
    bool		 myHasLoadedPythonModuleSection;

    bool		 myIsThreadSafe;
    int			 myIsCompiled;

    UT_StringArray	 myLegacyInputs;

    // We cannot assign descriptive parms when parsing dialog scripts
    // as they are stored in .otl sections, but we need to hold onto
    // it until buildotl finishes...
    UT_StringHolder	 myDescriptiveParmName;

    friend class	 OP_OperatorTable;
    friend class	 SHOP_ScriptOperator;
    friend class	 OP_UpdateTemplatesSink;

    static const OP_Operator	*ourLastHelpOp;
    static int			 ourLastHelpLevel;
    static UT_String	 	 ourHelp;
    static bool			 ourHelpIsHtml;
};

class OP_API OP_UpdateTemplatesSink
{
public:
		 OP_UpdateTemplatesSink()	{ }
    virtual 	~OP_UpdateTemplatesSink()
		 { removeAllTemplatesSinks(); }

    virtual void templatesUpdated(OP_Operator *op) = 0;
    virtual void operatorDeleted(OP_Operator *op)
		 { removeUpdateTemplatesSink(op); }

protected:
    void	 addUpdateTemplatesSink(OP_Operator *op)
		 {
		     if( !op ) return;
		     op->addUpdateTemplatesSink(this);
		     myOperators.append(op, 1);
		 }
    bool	 hasTemplatesSink(OP_Operator *op)
		 {
		     return myOperators.find(op) >= 0;
		 }
    void	 removeUpdateTemplatesSink(OP_Operator *op)
		 {
		     if( !op ) return;
		     op->removeUpdateTemplatesSink(this);
		     myOperators.findAndRemove(op);
		 }
    void	 removeAllTemplatesSinks()
		 {
		     for( int i = myOperators.entries(); i --> 0; )
			 removeUpdateTemplatesSink(myOperators(i));
		 }

private:
    OP_OperatorList	 myOperators;
};

#endif
