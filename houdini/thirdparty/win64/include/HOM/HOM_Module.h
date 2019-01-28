/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *     This class is not directly wrapped by swig.  However, when creating
 *     the swig wrapper modules we wrap each of the methods of this class
 *     in functions.  This way, the methods of this class appear as the
 *     top-level parts of the hou module.
 */

#ifndef __HOM_Module_h__
#define __HOM_Module_h__

#include "HOM_API.h"
#include "HOM_BinaryString.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include "HOM_EnumModules.h"
#include <PY/PY_InterpreterAutoLock.h>
#include <UT/UT_Lock.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Notifier.h>
#include <UT/UT_TaskLock.h>
#include <UT/UT_Thread.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <hboost/any.hpp>

class UT_OptionEntry;
class OP_Node;
class STORUI_OrboltURLHandler;
class HOM_AgentClip;
class HOM_AgentDefinition;
class HOM_AgentLayer;
class HOM_AgentRig;
class HOM_AgentShape;
class HOM_AgentShapeBinding;
class HOM_AgentShapeDeformer;
class HOM_AgentShapeLibrary;
class HOM_AgentTransformGroup;
class HOM_anonstats;
class HOM_audio;
class HOM_ButtonParmTemplate;
class HOM_ChopNode;
class HOM_Color;
class HOM_crowds;
class HOM_DataParmTemplate;
class HOM_Drawable;
class HOM_dop;
class HOM_Edge;
class HOM_EnumValue;
class HOM_EvaluationCache;
class HOM_FloatParmTemplate;
class HOM_FolderParmTemplate;
class HOM_FolderSetParmTemplate;
class HOM_galleries;
class HOM_Geometry;
class HOM_GeometrySelection;
class HOM_hda;
class HOM_HDADefinition;
class HOM_HDAModule;
class HOM_HDAOptions;
class HOM_hipFile;
class HOM_hmath;
class HOM_InterruptableOperation;
class HOM_IntParmTemplate;
class HOM_Keyframe;
class HOM_LabelParmTemplate;
class HOM_LopSelectionRule;
class HOM_MenuParmTemplate;
class HOM_NetworkBox;
class HOM_NetworkDot;
class HOM_NetworkMovableItem;
class HOM_Node;
class HOM_NodeBundle;
class HOM_NodeConnection;
class HOM_NodeType;
class HOM_NodeTypeCategory;
class HOM_OrboltURLHandler;
class HOM_Parm;
class HOM_ParmTemplate;
class HOM_ParmTemplateGroup;
class HOM_ParmTuple;
class HOM_perfMon;
class HOM_PerfMonRecordOptions;
class HOM_pypanel;
class HOM_playbar;
class HOM_Point;
class HOM_Prim;
class HOM_properties;
class HOM_qt;
class HOM_RampParmTemplate;
class HOM_RedrawBlock;
#ifndef SWIG
class HOM_RedrawResumeBlock;
#endif
class HOM_SceneViewer;
class HOM_ScriptEvalContext;
class HOM_Selection;
class HOM_SeparatorParmTemplate;
class HOM_shelves;
class HOM_StickyNote;
class HOM_StringKeyframe;
class HOM_StringParmTemplate;
class HOM_SubnetIndirectInput;
class HOM_styles;
class HOM_StyleSheet;
class HOM_takes;
class HOM_ToggleParmTemplate;
class HOM_ui;
class HOM_hotkeys;
class HOM_UIEvent;
class HOM_UIEventDevice;
class HOM_undos;
class HOM_Vertex;
class HOM_VexContext;
class HOM_ViewerEvent;
class HOM_viewportVisualizers;
class HOM_ViewerStateMenu;
class HOM_ViewerStateTemplate;

#define HOM_SESSION_MODULE_NAME		"hou.session"
#define HOM_SESSION_MODULE_CPIO_NAME	"." HOM_SESSION_MODULE_NAME

// Use this function to access the HOM module from C++ in Houdini.  This
// is equivalent to writing "hou" in python.  It will only throw an exception
// if the python module could not be imported, which should not happen.
class HOM_Module;
HOM_API HOM_Module& HOM();

class HOM_API HOM_Module
{
public:
    static void setHOMCreator(void (*homCreator)());

    virtual ~HOM_Module() {}

    virtual std::vector<std::string> _getArgumentAutoComplete(
	    const char *method_name, const char *arguments,
	    bool meta_info_only = false) = 0;

    // This method is called by the expression editor to force autocompletion
    // to be relative to the node containing the expression.  It is not
    // wrapped by swig.
    virtual void setRelativeNodeForAutoComplete(OP_Node *node) = 0;

    virtual HOM_ui& ui() = 0;
    virtual HOM_qt& qt() = 0;
    virtual HOM_hotkeys& hotkeys() = 0;
    virtual HOM_audio& audio() = 0;
    virtual bool isUIAvailable() = 0;
    virtual bool _isCurrentThreadHoldingHOMLock() = 0;

    virtual HOM_galleries& galleries() = 0;
    virtual HOM_hda& hda() = 0;
    virtual HOM_hipFile& hipFile() = 0;
    virtual HOM_perfMon& perfMon() = 0;
    virtual HOM_playbar& playbar() = 0;
    virtual HOM_pypanel& pypanel() = 0;
    virtual HOM_hmath& hmath() = 0;
    virtual HOM_dop& dop() = 0;
    virtual HOM_properties& properties() = 0;
    virtual HOM_shelves& shelves() = 0;
    virtual HOM_styles& styles() = 0;
    virtual HOM_takes& takes() = 0;
    virtual HOM_undos& undos() = 0;
    virtual HOM_viewportVisualizers& viewportVisualizers() = 0;
    virtual HOM_anonstats& anonstats() = 0;
    virtual HOM_crowds& crowds() = 0;

    virtual void exit(int exit_code=0, bool suppress_save_prompt=false) = 0;

    // isExiting() will return true if the program is actually Houdini (not
    // mplay) and the UI queue is closed.  This method is wrapped by swig, but
    // only so houdiniInterpreter can call it.
    virtual bool isExiting() = 0;

    virtual void addPreloadIcon(const char *name, int w, int h) = 0;
    virtual void rasterizePreloadIcons() = 0;

    virtual std::string applicationName() = 0;
    virtual std::vector<int> applicationVersion(bool include_patch=false) = 0;
    virtual std::string applicationVersionString() = 0;
    virtual std::string applicationCompilationDate() = 0;
    virtual std::string applicationPlatformInfo() = 0;
    virtual HOM_EnumValue &licenseCategory() = 0;
    virtual bool isApprentice() = 0;
    virtual void releaseLicense() = 0;

    // Threading controls
    virtual int maxThreads() = 0;
    virtual void setMaxThreads(int max_threads) = 0;

    // These methods return a new HOM_Node object.
    virtual HOM_Node* root() = 0;
    virtual HOM_Node* pwd() = 0;
    virtual HOM_Node* parent() = 0;

    virtual void setPwd(HOM_Node &newpwd) = 0;
    virtual void cd(const char *path) = 0;

    // Calling phm() is a shortcut for calling pwd().hdaModule().
    virtual HOM_HDAModule* phm() = 0;

    // This method returns a new HOM_DopNetNode object or None.
    virtual HOM_Node* currentDopNet() = 0;
    virtual void setCurrentDopNet(HOM_Node &dopnet) = 0;

    // Gets or sets the simulation enabled state.
    virtual bool simulationEnabled() = 0;
    virtual void setSimulationEnabled(bool enabled) = 0;

    // These methods return a new HOM_Node object or null if the node couldn't
    // be found.
    virtual HOM_Node *node(const char *path) = 0;
    virtual HOM_NetworkMovableItem* item(const char *path) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes(const std::vector<std::string> &paths) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > items(const std::vector<std::string> &paths) = 0;

    virtual HOM_Node *nodeBySessionId(int session_id) = 0;
    virtual HOM_NetworkBox *networkBoxBySessionId(int session_id) = 0;
    virtual HOM_StickyNote *stickyNoteBySessionId(int session_id) = 0;
    virtual HOM_SubnetIndirectInput *subnetIndirectInputBySessionId(
	    int64 session_id) = 0;
    virtual HOM_NetworkDot *networkDotBySessionId(int session_id) = 0;
    virtual HOM_NetworkMovableItem *itemBySessionId(
	    HOM_EnumValue &item_type, int64 session_id) = 0;
    virtual HOM_NodeConnection *nodeConnectionBySessionId(
	    int node_session_id, int input_index) = 0;

    // Functions for configuring default colors on network elements.
    virtual HOM_Color defaultColor(HOM_EnumValue &color_item) = 0;
    virtual void setDefaultColor(HOM_EnumValue &color_item, HOM_Color *color) = 0;

    // Return HOM_Nodes for all selected nodes in the session.
    virtual std::vector<HOM_ElemPtr<HOM_Node> > selectedNodes(
	    bool include_hidden = false) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> > selectedItems(
	    bool include_hidden = false) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> > selectedConnections() = 0;
    virtual void clearAllSelected() = 0;

    // Copy nodes to and paste from clipboard.
    virtual void copyNodesToClipboard(const std::vector<HOM_Node *> &nodes) = 0;
    virtual void pasteNodesFromClipboard(HOM_Node &destination_node) = 0;

    // These methods return new HOM_Parm/HOM_ParmTuple objects or null if the
    // parm couldn't be found.
    virtual HOM_Parm* parm(const char *path) = 0;
    virtual HOM_ParmTuple* parmTuple(const char *path) = 0;
    virtual HOM_Parm *evaluatingParm() = 0;

    virtual HOM_ScriptEvalContext * newScriptEvalContext(HOM_Node &node) = 0;
    virtual HOM_ScriptEvalContext * newScriptEvalContext(HOM_Parm &parm) = 0;

    // Access to the contents of the parm clipboard
	virtual std::vector< std::map<std::string, std::string> >
			    parmClipboardContents() = 0;

    // These methods return a new HOM_NodeType object or null if it couldn't
    // be found.
    virtual HOM_NodeType *nodeType(
	    HOM_NodeTypeCategory &category, const char *name) = 0;
    virtual HOM_NodeType *nodeType(const char *name) = 0;

    virtual HOM_NodeType *preferredNodeType(
	    const char *name, HOM_Node *parentNode = NULL) = 0;

    virtual HOM_NodeTypeCategory &objNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &sopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &popNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &dopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &cop2NodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &shopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &vopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &ropNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &chopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &topNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &lopNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &popNetNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &cop2NetNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &vopNetNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &chopNetNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &managerNodeTypeCategory() = 0;
    virtual HOM_NodeTypeCategory &rootNodeTypeCategory() = 0;
    virtual std::map<std::string, HOM_NodeTypeCategory *>
	    nodeTypeCategories() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > copyNodesTo(
	    const std::vector<HOM_Node *> &nodes, 
	    HOM_Node &destination_node) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Node> > moveNodesTo(
	    const std::vector<HOM_Node *> &nodes, 
	    HOM_Node &destination_node) = 0;

    virtual HOM_NodeBundle *addNodeBundle(const char *name = NULL) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NodeBundle> > nodeBundles() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NodeBundle> > selectedNodeBundles() = 0;
    virtual HOM_NodeBundle *nodeBundle(const char *name) = 0;

    virtual HOM_HDADefinition *hdaDefinition(
	    HOM_NodeTypeCategory &category, const char *name,
	    const char *file_path) = 0;

    // Note that getTimeWithoutAddingDependency is not wrapped by swig.
    virtual fpreal getTimeWithoutAddingDependency() = 0;
    virtual double time() = 0;
    virtual void setTime(double time) = 0;
    virtual double frame() = 0;
    virtual int intFrame() = 0;
    virtual void setFrame(double frame) = 0;
    virtual double fps() = 0;
    virtual void setFps(double fps) = 0;
    virtual double timeToFrame(double time) = 0;
    virtual double frameToTime(double frame) = 0;

    // Computes a scale factor given units
    virtual fpreal scaleFromMKS(const char *units) = 0;
    virtual fpreal scaleToMKS(const char *units) = 0;

    virtual double bezier() = 0;
    virtual double constant() = 0;
    virtual double cubic() = 0;
    virtual double cycle(double start_frame, double end_frame) = 0;
    virtual double cyclet(double start_time, double end_time) = 0;
    virtual double cycleoffset(double start_frame, double end_frame) = 0;
    virtual double cycleoffsett(double start_time, double end_time) = 0;
    virtual double ease() = 0;
    virtual double easein() = 0;
    virtual double easeinp(double ease_speed) = 0;
    virtual double easeout() = 0;
    virtual double easeoutp(double ease_speed) = 0;
    virtual double easep(double ease_bias) = 0;
    virtual double linear() = 0;
    virtual double match() = 0;
    virtual double matchin() = 0;
    virtual double matchout() = 0;
    virtual double qlinear() = 0;
    virtual double quintic() = 0;
    virtual double repeat(double start_frame, double end_frame) = 0;
    virtual double repeatt(double start_time, double end_time) = 0;
    virtual double spline() = 0;
    virtual double vmatch() = 0;
    virtual double vmatchin() = 0;
    virtual double vmatchout() = 0;

    virtual std::string chopExportConflictResolutionPattern() = 0;
    virtual void setChopExportConflictResolutionPattern( const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_VexContext> >
	vexContexts() = 0;

    virtual HOM_VexContext* vexContextForShaderType(HOM_EnumValue &shader_type) = 0;

    virtual HOM_VexContext* vexContextForNodeTypeCategory(
	    HOM_NodeTypeCategory &node_type_category) = 0;

    virtual std::string helpServerUrl() = 0;

    virtual std::string hscriptCommandHelp(const char *command_name) = 0;

    // This method returns a pair of strings: one for stdout and one for
    // stderr.
    virtual std::vector<std::string> hscript(const char *command) = 0;

    // Compare if two numbers are almost equal.
    virtual bool almostEqual(double x, double y) = 0;

    virtual std::vector<std::pair<HOM_ElemPtr<HOM_Parm>, std::string> >
	fileReferences(
        const char *project_dir_variable="HIP", bool include_all_refs=true) = 0;

    // Find files in $HOUDINI_PATH.
    virtual std::string findFile(const char *file_name) = 0;
    virtual std::vector<std::string> findFiles(
	const char *file_name) = 0;
    virtual std::vector<std::string> findFilesWithExtension(
	const char *file_extension, const char *subdirectory = NULL) = 0;

    // Find directories in $HOUDINI_PATH.
    virtual std::string findDirectory(const char *directory_name) = 0;
    virtual std::vector<std::string> findDirectories(
	const char *directory_name) = 0;

    virtual std::vector<std::string> houdiniPath(const char *pathvar = nullptr) = 0;

    virtual std::string homeHoudiniDirectory() = 0;

    virtual HOM_BinaryString readFile(const char *file_path) = 0;

    virtual std::map<std::string, HOM_BinaryString> loadIndexDataFromString(
	    const char *data, int length) = 0;

    virtual std::map<std::string, HOM_BinaryString> loadIndexDataFromFile(
	    const char *file_path) = 0;

    virtual HOM_BinaryString saveIndexDataToString(
	    const std::map<std::string, std::string> &index_data) = 0;

    virtual void saveIndexDataToFile(
	    const char *file_path,
	    const std::map<std::string, std::string> &index_data) = 0;

    // Note that, when wrapped by swig, this function takes a single string
    // parameter.
    virtual std::vector<std::pair<std::string, HOM_BinaryString> >
            loadCPIODataFromString(const char *data, int length) = 0;

    virtual HOM_BinaryString saveCPIODataToString(
	    const std::vector<std::pair<std::string, std::string> > &data) = 0;

    // These 3 methods are not wrapped by swig.  They are used to notify
    // interested parties that a change to the module source code was made and
    // that they should forcefully update to match the change.  The third
    // is used to load the session module source from a hip file, even though
    // that source might generate an exception.
    virtual UT_NotifierImpl<int> &getSessionModuleSourceForceUpdateNotifier()=0;
    virtual void forceUpdateFromSessionModuleSource() = 0;
    virtual void forceSetSessionModuleSource(const char *source) = 0;

    virtual void setSessionModuleSource(const char *source) = 0;
    virtual void appendSessionModuleSource(const char *source) = 0;
    virtual std::string sessionModuleSource() = 0;
    // This method is not wrapped by swig:
    virtual int sessionModuleSourceChangeCount() = 0;

    // This static method lets you know if the HOM implementation is set,
    // so you can check if it's safe to call the HOM() function.  This method
    // should only be called in rare cases.
    static bool isImplementationSet()
    { return theImplementation != NULL; }

    // Evaluates a parameter as if it was an OPpath parameter
    virtual std::string chsop(const char *path) = 0;

    // Evaluates a parameter as if it was an OPlist parameter
    virtual std::string chsoplist(const char *path) = 0;

    // Sorts a list of node paths using topological order from the graph.
    virtual std::vector<std::string>
	sortedNodePaths(const std::vector<std::string> &paths) = 0;

    // Sorts a list of HOM_Node using topological order from the graph.
    virtual std::vector<HOM_ElemPtr<HOM_Node> >
	sortedNodes(const std::vector<HOM_Node*> &nodes) = 0;

    virtual int patternMatch(
	    const char *pattern, const char *str,
	    bool ignore_case = false,
	    bool path_match = false) = 0;

    // hscriptExpression() will return different types, according to
    // the return value of the expression.
    virtual hboost::any hscriptExpression(const char *expression) = 0;
    virtual double hscriptFloatExpression(const char *expression) = 0;
    virtual std::string hscriptStringExpression(const char *expression) = 0;
    virtual std::vector<double>hscriptVectorExpression(
	    const char *expression) = 0;
    virtual std::vector<std::vector<double> > hscriptMatrixExpression(
	    const char *expression) = 0;

    virtual std::string expandString(const char *str) = 0;
    virtual std::string expandStringAtFrame(const char *str, double frame) = 0;

    virtual double lvar(const char *name) = 0;

    virtual char *getenv(const char *name, const char *default_value=NULL) = 0;

    virtual void putenv(const char *name, const char *value) = 0;

    virtual void unsetenv(const char *name) = 0;

    virtual void allowEnvironmentToOverwriteVariable(
	    const char *name, bool onoff) = 0;

    virtual void *expressionGlobals() = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // This method is used to interact with the global interrupt object.
    virtual bool updateProgressAndCheckForInterrupt(int percentage=-1) = 0;

    virtual std::map<std::string, hboost::any> runVex(
	    const char *vex_file,
	    const std::map<std::string, hboost::any> &inputs,
	    const char *precision = "32") = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    virtual void saveImageDataToFile(
	    const std::vector<float> &color_and_alpha_data,
	    int width, int height,
	    const char *file_name) = 0;

    virtual void saveImageDataToFile(
	    const std::string &color_and_alpha_data,
	    int width, int height,
	    const char *file_name) = 0;

    virtual HOM_BinaryString loadImageDataFromFile(
	    const char *image_file_name,
	    HOM_EnumValue &image_depth = HOM_imageDepth::Int8) = 0;

    virtual std::vector<int> imageResolution(const char *image_file_name) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    virtual HOM_EnumValue &updateModeSetting() = 0;
    virtual void setUpdateMode(HOM_EnumValue &mode) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    virtual HOM_OrboltURLHandler *orboltURLHandler() = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    virtual void *opaqueRunCallbackAndCatchCrashes(void *opaque_callback) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // These methods are used to grant functions defined in an asset's
    // PythonModule enhanced privileges.  They are not wrapped by swig.
    virtual void pushAccessPrivilege(const char *key) = 0;
    virtual void popAccessPrivilege(const char *key) = 0;

    virtual std::string currentPythonModuleKey() = 0;

    virtual void registerAccessPrivilegePyClient(void *opaque_python_object) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // These methods are used to instantiate HOMF objects from the HOM
    // library level.  They are not wrapped by swig.
    virtual HOM_FloatParmTemplate *newFloatParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<double> &default_value,
	    double min, double max,
	    bool min_is_strict, bool max_is_strict,
	    HOM_EnumValue &look,
	    HOM_EnumValue &naming_scheme,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::vector<std::string> &default_expression,
	    const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

    virtual HOM_IntParmTemplate *newIntParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<int> &default_value,
	    int min, int max,
	    bool min_is_strict, bool max_is_strict,
	    HOM_EnumValue &naming_scheme,
	    const std::vector<std::string> &menu_items,
	    const std::vector<std::string> &menu_labels,
	    const std::vector<std::string> &icon_names,
	    const std::string &item_generator_script,
	    HOM_EnumValue *item_generator_script_language,
	    HOM_EnumValue &menu_type,
	    bool menu_use_token,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::vector<std::string> &default_expression,
	    const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

    virtual HOM_StringParmTemplate *newStringParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    const std::vector<std::string> &default_value,
	    HOM_EnumValue &naming_scheme,
	    HOM_EnumValue &string_type,
	    HOM_EnumValue &file_type,
	    const std::vector<std::string> &menu_items,
	    const std::vector<std::string> &menu_labels,
	    const std::vector<std::string> &icon_names,
	    const std::string &item_generator_script,
	    HOM_EnumValue *item_generator_script_language,
	    HOM_EnumValue &menu_type,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::vector<std::string> &default_expression,
	    const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

    virtual HOM_DataParmTemplate *newDataParmTemplate(
	    const char *name,
	    const char *label,
	    int num_components,
	    HOM_EnumValue &look,
	    HOM_EnumValue &naming_scheme,
	    HOM_EnumValue &data_parm_type,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::vector<std::string> &default_expression,
	    const std::vector<HOM_EnumValue *> &default_expression_language) = 0;

    virtual HOM_ToggleParmTemplate *newToggleParmTemplate(
	    const char *name,
	    const char *label,
	    bool default_value,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::string &default_expression,
	    const HOM_EnumValue &default_expression_language) = 0;

    virtual HOM_MenuParmTemplate *newMenuParmTemplate(
	    const char *name,
	    const char *label,
	    const std::vector<std::string> &menu_items,
	    const std::vector<std::string> &menu_labels,
	    int default_value,
	    const std::vector<std::string> &icon_names,
	    const std::string &item_generator_script,
	    HOM_EnumValue *item_generator_script_language,
	    HOM_EnumValue &menu_type,
	    bool menu_use_token,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::string &default_expression,
	    const HOM_EnumValue &default_expression_language,
	    bool store_default_value_as_string) = 0;

    virtual HOM_ButtonParmTemplate *newButtonParmTemplate(
	    const char *name,
	    const char *label,
	    const char *disable_when,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals ) = 0;

    virtual HOM_LabelParmTemplate *newLabelParmTemplate(
	    const char *name,
	    const char *label,
	    const std::vector<std::string> &column_labels,
	    bool is_hidden,
	    bool is_label_hidden,
	    bool join_with_next,
	    const char *help,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals) = 0;

    virtual HOM_SeparatorParmTemplate *newSeparatorParmTemplate(
	    const char *name,
	    bool is_hidden,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals) = 0;

    virtual HOM_FolderSetParmTemplate *newFolderSetParmTemplate(
	    const char *name,
	    const std::vector<std::string> &folder_names,
	    HOM_EnumValue &folder_type,
	    const std::map<std::string, std::string> &tags) = 0;

    virtual HOM_FolderParmTemplate *newFolderParmTemplate(
	    const char *name,
	    const char *label,
            const std::vector<HOM_ParmTemplate *> &parm_templates,
	    HOM_EnumValue &folder_type,
            int default_value,
	    bool is_hidden,
	    bool ends_tab_group,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals) = 0;

    virtual HOM_RampParmTemplate *newRampParmTemplate(
	    const char *name,
	    const char *label,
	    HOM_EnumValue &ramp_parm_type,
	    int default_value,
	    HOM_EnumValue *default_basis,
	    bool show_controls,
	    HOM_EnumValue *color_type,
	    const char *disable_when,
	    bool is_hidden,
	    const char *help,
	    const char *script_callback,
	    HOM_EnumValue &script_callback_language,
	    const std::map<std::string, std::string> &tags,
	    const std::map<HOM_EnumValue, std::string> &conditionals,
	    const std::string &default_expression,
	    const HOM_EnumValue &default_expression_language) = 0;

    virtual HOM_ParmTemplateGroup *newParmTemplateGroup(
            const std::vector<HOM_ParmTemplate *> &parm_templates) = 0;

    virtual HOM_PerfMonRecordOptions *newPerfMonRecordOptions(
	    bool cook_stats, bool solve_stats, bool draw_stats,
	    bool gpu_draw_stats, bool viewport_stats, bool script_stats, 
	    bool render_stats, bool thread_stats, bool frame_stats,
	    bool memory_stats, bool errors) = 0;


    virtual HOM_Keyframe *newKeyframe() = 0;
    virtual HOM_Keyframe *newKeyframe(double value) = 0;
    virtual HOM_Keyframe *newKeyframe(double value, double time) = 0;
    virtual HOM_Keyframe *newKeyframe(const HOM_Keyframe &keyframe) = 0;

    virtual HOM_StringKeyframe *newStringKeyframe() = 0;
    virtual HOM_StringKeyframe *newStringKeyframe(
	const std::string &value, HOM_EnumValue &language) = 0;
    virtual HOM_StringKeyframe *newStringKeyframe(
	const std::string &value, double time, HOM_EnumValue &language) = 0;
    virtual HOM_StringKeyframe *newStringKeyframe(
	const HOM_StringKeyframe &string_keyframe) = 0;

    virtual HOM_HDAOptions *newHDAOptions() = 0;
    virtual HOM_LopSelectionRule *newLopSelectionRule() = 0;

    virtual HOM_InterruptableOperation *newInterruptableOperation(
	const char *operation_name, const char *long_operation_name,
	bool open_interrupt_dialog) = 0;

    virtual HOM_RedrawBlock *newRedrawBlock() = 0;
#ifndef SWIG
    virtual HOM_RedrawResumeBlock *newRedrawResumeBlock() = 0;
#endif

    virtual HOM_StyleSheet *newStyleSheet() = 0;
    virtual HOM_StyleSheet *newStyleSheet(const char *json_text) = 0;

    virtual HOM_Selection *newSelection(HOM_EnumValue &selection_type,
	HOM_Geometry *geo, const char *selection_string) = 0;
    virtual HOM_Selection *newSelection(
	const std::vector<HOM_EnumValue *> &selection_types,
	HOM_Geometry *geo, const char *selection_string) = 0;
    virtual HOM_Selection *newSelection(
	const std::vector<HOM_Prim *> &prims) = 0;
    virtual HOM_Selection *newSelection(
	const std::vector<HOM_Point *> &points) = 0;
    virtual HOM_Selection *newSelection(
	const std::vector<HOM_Vertex *> &vertices) = 0;
    virtual HOM_Selection *newSelection(
	const std::vector<HOM_Edge *> &edges) = 0;

    // This method returns a new HOM_Geometry object.
    virtual HOM_Geometry *newGeometry() = 0;
    virtual HOM_Geometry *newGeometry(HOM_Geometry &) = 0;

    virtual HOM_AgentDefinition *newAgentDefinition(
        const HOM_AgentRig &rig, const HOM_AgentShapeLibrary &shapelib) = 0;

    virtual HOM_AgentClip *newAgentClip(
        const char *name, const char *path, const HOM_AgentRig &rig,
        bool keep_external_ref) = 0;

    virtual HOM_AgentClip *newAgentClip(
        const char *name, HOM_ChopNode &chop, const HOM_AgentRig &rig,
        double frame) = 0;

    virtual HOM_AgentClip *newAgentClip(
        const HOM_AgentClip &clip, const HOM_AgentRig &rig,
        const char *name) = 0;

    virtual HOM_AgentRig *newAgentRig(const char *path, bool keep_external_ref) = 0;

    virtual HOM_AgentRig *newAgentRig(
        const char *name, const std::vector<std::string> &transform_names,
        const std::vector<std::vector<int>> &hierarchy) = 0;

    virtual HOM_AgentTransformGroup *newAgentTransformGroup(
        const char *path, const HOM_AgentRig &rig,
        bool keep_external_ref = true) = 0;

    virtual HOM_AgentTransformGroup *newAgentTransformGroup(
        const char *name, const std::vector<int> &transforms,
        const HOM_AgentRig &rig, const std::vector<double> &weights) = 0;

    virtual HOM_AgentShapeLibrary *newAgentShapeLibrary() = 0;
    virtual HOM_AgentShapeLibrary *newAgentShapeLibrary(
        const char *path, bool keep_external_ref) = 0;

    virtual HOM_AgentShapeBinding *
    newAgentShapeBinding(int transform, const HOM_AgentShape &shape,
                         bool deforming, double bounds_scale) = 0;

    virtual HOM_AgentShapeBinding *
    newAgentShapeBinding(int transform, const HOM_AgentShape &shape,
                         const HOM_AgentShapeDeformer &deformer,
                         double bounds_scale) = 0;

    virtual HOM_AgentLayer *newAgentLayer(
        const char *name, const HOM_AgentRig &rig,
        const HOM_AgentShapeLibrary &shapelib,
        const std::vector<HOM_AgentShapeBinding *> &shape_bindings,
        const HOM_AgentLayer *source_layer = 0) = 0;

    virtual HOM_AgentLayer *
    newAgentLayer(const char *path, const HOM_AgentRig &rig,
                  const HOM_AgentShapeLibrary &shapelib,
                  bool keep_external_ref = true) = 0;

    virtual bool compareNode(const char *path, const char *dest = NULL) = 0;
    virtual bool compareNodeBackup(const char *path, int index,
	const char *dest = NULL) = 0;
    virtual bool compareHDA(const char *path_left, const char *path_right,
	const char *dest = NULL) = 0;
    virtual bool compareHDABackup(const char *path, int index,
	const char *dest = NULL) = 0;
    virtual bool compareHip(const char *dest = NULL) = 0;
    virtual bool compareHipBackup(int index, const char *dest = NULL) = 0;

    virtual bool hasContextOption(const char *opt) = 0;
    virtual UT_OptionEntry *contextOption(const char *opt) = 0;
    virtual std::vector<std::string> contextOptionNames() = 0;
    virtual void setDefaultContextOption(const char *opt,
	    const char *value, const char *ui_config = nullptr) =0;
    virtual void setDefaultContextOption(const char *opt,
	    double value, const char *ui_config = nullptr) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // This method is called when orbolt_url runs.  It is not wrapped by swig.
    virtual void setOrboltURLHandler(
	STORUI_OrboltURLHandler &orbolt_url_handler) = 0;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // These methods are used to help debug HOM to make sure no objects
    // get allocated that don't get deallocated.
    virtual void constructObject(void *pointer, const std::type_info &type) = 0;
    virtual void destructObject(void *pointer, const std::type_info &type) = 0;
    virtual std::map<std::string, int> objectCounts() = 0;

    // Preferences
    virtual std::vector<std::string> getPreferenceNames() = 0;
    virtual std::string getPreference(const char *name) = 0;
    virtual bool setPreference(const char *name, const char* value) = 0;

    virtual HOM_ViewerStateTemplate * newViewerStateTemplate(
	std::string const &		type_name, 
	std::string const &		label,
	HOM_NodeTypeCategory const &	category) = 0;

    virtual HOM_ViewerStateMenu * newViewerStateMenu(std::string const & handle, std::string const & label) = 0;

    virtual HOM_Drawable * newDrawable( HOM_SceneViewer& scene_viewer, HOM_EnumValue const &prim_type, std::string const &name ) = 0;
    virtual HOM_Drawable * newDrawable( HOM_SceneViewer& scene_viewer, HOM_Geometry const &geometry, std::string const &name ) = 0;

    virtual HOM_UIEvent * newUIEvent( int64 val ) = 0;
    virtual HOM_ViewerEvent * newViewerEvent( HOM_SceneViewer& sceneViewer, int64 val ) = 0;
    virtual HOM_UIEventDevice * newUIEventDevice(int64 val, int64 val2) = 0;

    virtual void startHoudiniEngineDebugger(unsigned short port) = 0;
    virtual void startHoudiniEngineDebugger(const char *pipe_name) = 0;

    virtual HOM_GeometrySelection * newGeometrySelection() = 0;
    virtual HOM_GeometrySelection * newGeometrySelection(int64 val) = 0;

    virtual HOM_BinaryString convertClipData(HOM_BinaryString data,
					     bool from_binary,
					     bool from_blosc_compression,
					     bool to_binary,
					     bool to_blosc_compression) = 0;

    virtual HOM_BinaryString convertKeyframesToClipData(
        const std::map<std::string, std::vector<HOM_Keyframe *>> &keyframe_map,
	bool binary = true, bool use_blosc_compression = true) = 0;

    virtual std::string _clipInfo(HOM_BinaryString data, bool binary = true,
				  bool blosc_compressed = true) = 0;
private:
    // HOMF_Module::createHOMModule() uses this to create the HOM_Module.
    static void (*theHOMCreator)();

    // A subclass of HOM_Module in the HOMF library will define the
    // implementation of HOM.  Importing the python module will set the
    // implementation, so this function should only be called by HOMF_Module::
    // createHOMModule().
    friend class HOMF_Module;
    static void setImplementation(HOM_Module* implementation);

    // We could add a static method to access the implementation, but we
    // want to make it easy to access the HOM module from C++.  So, the
    // global function HOM() needs access to the implementation.
    friend HOM_API HOM_Module& HOM();
    static HOM_Module *theImplementation;
};


// Use this function to automatically delete objects for functions that
// return new objects.  This lets you, for example, write code like
//     HOMdel(HOM().findNode("/obj/geo1"))->setName("geo2")
template <typename T>
UT_UniquePtr<T> HOMdel(T *hom_object)
{ return UT_UniquePtr<T>(hom_object); }


// This lock is used to synchronize HOM scripts running in a separate
// thread and Houdini.  Houdini's UI queue will give up the lock when it's
// waiting for events and reacquire it when it handles an event.
//
// Note that this lock allows multiple threads in the same thread group to hold
// it at the same time.  By default, a thread started from Houdini is added to
// the group containing the main thread.  This way, since Houdini started the
// thread, we can be confident that it will use other locks to synchronize
// access to Houdini.  This approach allows, for example, a volume SOP to
// cook using multiple threads, evaluating Python in each thread while the
// main thread holds the HOM lock, without deadlocking and without allowing
// non-Houdini threads to acquire the HOM lock.  (Note that the GIL will
// synchronize access to Python's internals.)
HOM_API extern UT_TaskRootLock *HOM_theHOMLock;
inline UT_TaskRootLock &HOMgetLock()
{
    // This is not thread-safe but should be ok because HOMgetLock() is called
    // pretty early on during startup when there is only one thread executing.
    if (!HOM_theHOMLock)
	HOM_theHOMLock = new UT_TaskRootLock();

    UT_ASSERT(HOM_theHOMLock);
    return *HOM_theHOMLock;
}

// The HOM implementation uses the HOM_AutoLock to make sure Houdini isn't busy
// when they make calls into the Houdini libraries.  Note that constructing
// a HOM_AutoLock will release the python global interpreter lock (GIL) if
// it was held before acquiring the HOM lock.  This way, other python threads
// can run if it needs to block waiting for the HOM lock.
class HOM_API HOM_AutoLock : UT_NonCopyable
{
public:
    HOM_AutoLock()
    {
	if (!HOMgetLock().hasLock())
	{
	    HOMgetLock().lock();
	    myLocked = true;
	}
	else
	    myLocked = false;
    }

    ~HOM_AutoLock()
    {
	if (myLocked)
	    HOMgetLock().unlock();
    }

private:

    // It is important that we release the python global interpreter lock
    // (GIL) for the duration of waiting for, acquiring and holding onto
    // the HOM lock.  This gives other threads that are either blocked
    // on the GIL or that will try to acquire the GIL a chance to continue
    // execution.
    //
    // If the current thread needs to access the Python API, 
    // then it should explicitly acquire the GIL by creating a
    // PY_InterpreterAutoLock object.  The PY_InterpreterAutoLock must be
    // constructed *after* the HOM_AutoLock or it will have no effect.
    //
    // This prevents deadlocks that occur when the current thread
    // spawns other threads which access the Python API and try to
    // acquire the GIL.
    //
    // Note that we will reacquire the GIL when the HOM auto lock is destructed.
    PY_InterpreterAutoUnlock	myPyInterpAutoUnlock;

    // Boolean to determine if the auto lock actually locked the HOM lock.
    bool myLocked;
};

// The HOM implementation uses HOM_AutoUnlock to allow the main thread to
// run while waiting for something to happen.  Constructing a HOM_AutoUnlock
// instance will release the HOM lock but not the Python GIL.  When it's
// destructed it will reacquire the HOM lock.  Since it releases the HOM lock,
// it's important that a HOM_AutoLock was already created in a containing
// scope.
//
// Use HOM_AutoUnlock in conjunction with PY_InterpreterAutoUnlock to
// also release the Python GIL.  Be sure to create the HOM_AutoUnlock *after*
// the PY_InterpreterAutoUnlock, so that when the auto locks are destructed
// the HOM lock is reacquired before the Python GIL.  Otherwise, it will grab
// the GIL and then try to grab the HOM lock, and if another thread with the
// HOM lock tries to run Python we'll end up with deadlock.
class HOM_API HOM_AutoUnlock : UT_NonCopyable
{
public:
    HOM_AutoUnlock()
    { HOMgetLock().unlock(); }

    ~HOM_AutoUnlock()
    { HOMgetLock().lock(); }
};


// These macros are used to help detect memory leaks when debugging is enabled.
#ifdef HOM_DEBUG
#define HOM_CONSTRUCT_OBJECT(pointer) \
    HOM().constructObject(pointer, typeid(*pointer));
#define HOM_DESTRUCT_OBJECT(pointer) \
    HOM().destructObject(pointer, typeid(*pointer));
#else
#define HOM_CONSTRUCT_OBJECT(pointer)
#define HOM_DESTRUCT_OBJECT(pointer)
#endif

// gcc gets a parse error if we try to give a default map value in the
// signature, and the typedef avoids the error.
typedef std::map<std::string, std::string> HOM_StdMapStringString;
typedef std::map<HOM_EnumValue, std::string> HOM_StdMapEnumString;


// Houdini will call HOMextendLibrary() in HDK dso's so the user can install
// C++ HOM extensions.
extern "C" {
   SYS_VISIBILITY_EXPORT extern void HOMextendLibrary();
}

#endif
