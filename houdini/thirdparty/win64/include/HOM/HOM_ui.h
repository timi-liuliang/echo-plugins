/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ui_h__
#define __HOM_ui_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Attrib.h"
#include "HOM_EnumValue.h"
#include "HOM_EnumModules.h"
#include "HOM_Color.h"
#include "HOM_Errors.h"
#include <PY/PY_Callback.h>
#include <PY/PY_Python.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Tuple.h>
#include <vector>

class HOM_ShellIO;
class HOM_Desktop;
class HOM_Pane;
class HOM_PaneTab;
class HOM_FloatingPanel;
class HOM_NodeType;
class HOM_Node;
class HOM_RadialMenu;
class HOM_RopNode;
class HOM_Dialog;
class HOM_ViewerStateMenu;
class HOM_ViewerStateTemplate;
class HOM_SopNodeType;

SWIGOUT(%rename(ui) HOM_ui;)
class HOM_API HOM_ui
{
public:
    virtual ~HOM_ui()
    {}

    virtual std::string __repr__() = 0;

    virtual HOM_ShellIO &shellIO() = 0;

    SWIGOUT(%newobject curDesktop;)
    virtual HOM_Desktop* curDesktop() = 0;
    SWIGOUT(%newobject desktop;)
    virtual HOM_Desktop* desktop(const char *name) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Desktop> > desktops() = 0;

    virtual HOM_RadialMenu* radialMenu(const std::string &name) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_RadialMenu> > radialMenus() = 0;
    virtual HOM_RadialMenu* createRadialMenu(const std::string &name,
	    const char *label) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Pane> > panes() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > paneTabs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > currentPaneTabs() = 0;

    SWIGOUT(%newobject paneTabOfType;)
    virtual HOM_PaneTab *paneTabOfType(HOM_EnumValue &type, int index=0) = 0;

    SWIGOUT(%newobject findPane;)
    virtual HOM_Pane *findPane(int pane_id) = 0;

    SWIGOUT(%newobject findPaneTab;)
    virtual HOM_PaneTab *findPaneTab(const char *name) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PaneTab> > floatingPaneTabs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_FloatingPanel> > floatingPanels() = 0;

    SWIGOUT(%newobject paneUnderCursor;)
    virtual HOM_Pane *paneUnderCursor() = 0;
    SWIGOUT(%newobject paneTabUnderCursor;)
    virtual HOM_PaneTab *paneTabUnderCursor() = 0;

    virtual HOM_EnumValue &orientationUpAxis() = 0;
    virtual HOM_EnumValue &handleOrientToNormalAxis() = 0;

    SWIGOUT(%kwargs displayConfirmation;)
    virtual bool displayConfirmation(
	    const char *text,
	    HOM_EnumValue &severity = HOM_severityType::Message,
	    const char *help = NULL,
	    const char *title = NULL,
	    const char *details = NULL,
	    const char *details_label = NULL,
	    HOM_EnumValue &suppress = HOM_confirmType::OverwriteFile) = 0;

    SWIGOUT(%kwargs displayMessage;)
    virtual int displayMessage(
	    const char *text,
	    const std::vector<std::string> &buttons =
		std::vector<std::string>(1, "OK"),
	    HOM_EnumValue &severity = HOM_severityType::Message,
	    int default_choice = 0,
	    int close_choice = -1,
	    const char *help = NULL,
	    const char *title = NULL,
	    const char *details = NULL,
	    const char *details_label = NULL,
	    bool details_expanded = false) = 0;

    SWIGOUT(%kwargs readInput;)
    virtual std::pair<int, std::string> readInput(
	    const char *message,
	    const std::vector<std::string> &buttons =
		std::vector<std::string>(1, "OK"),
	    HOM_EnumValue &severity = HOM_severityType::Message,
	    int default_choice = 0,
	    int close_choice = -1,
	    const char *help = NULL,
	    const char *title = NULL,
	    const char *initial_contents = NULL) = 0;

    SWIGOUT(%kwargs readMultiInput;)
    virtual std::pair<int, std::vector<std::string> > readMultiInput(
	    const char *message,
	    const std::vector<std::string> &input_labels,
	    const std::vector<int> &password_input_indices = std::vector<int>(),
	    const std::vector<std::string> &buttons =
		std::vector<std::string>(1, "OK"),
	    HOM_EnumValue &severity = HOM_severityType::Message,
	    int default_choice = 0,
	    int close_choice = -1,
	    const char *help = NULL,
	    const char *title = NULL,
	    const std::vector<std::string> &initial_contents =
		std::vector<std::string>() ) = 0;

    SWIGOUT(%kwargs selectFromList;)
    virtual std::vector<int> selectFromList(
	    const std::vector<std::string> &choices,
	    const std::vector<int> &default_choices = std::vector<int>(),
	    bool exclusive = false,
	    const char *message = NULL,
	    const char *title = NULL,
	    const char *column_header = "Choices",
	    int num_visible_rows = 10,
	    bool clear_on_cancel = false) = 0;

    SWIGOUT(%kwargs selectFromTree;)
    virtual std::vector<std::string> selectFromTree(
	    const std::vector<std::string> &choices,
	    const std::vector<std::string> &picked = std::vector<std::string>(),
	    bool exclusive = false,
	    const char *message = NULL,
	    const char *title = NULL,
	    bool clear_on_cancel = false) = 0;

    SWIGOUT(%kwargs selectFile;)
    virtual std::string selectFile(
	    const char *start_directory = NULL,
	    const char *title = NULL,
	    bool collapse_sequences = false,
	    HOM_EnumValue &file_type = HOM_fileType::Any,
	    const char *pattern = NULL,
	    const char *default_value = NULL,
	    bool multiple_select = false,
	    bool image_chooser = false,
	    HOM_EnumValue &chooser_mode = HOM_fileChooserMode::ReadAndWrite) = 0;

    SWIGOUT(%kwargs selectNode;)
    SWIGOUT(%newobject selectNode;)
    virtual char *selectNode(
	    HOM_Node *relative_to_node = NULL,
	    HOM_Node *initial_node = NULL,
	    HOM_EnumValue *node_type_filter = NULL) = 0;

    SWIGOUT(%kwargs selectMultipleNodes;)
    virtual std::vector<std::string> selectMultipleNodes(
	HOM_Node *relative_to_node = NULL,
	HOM_Node *initial_node = NULL,
	HOM_EnumValue *node_type_filter = NULL) = 0;

    SWIGOUT(%ignore _selectNodeDataInternal;)
    virtual std::vector<std::string> _selectNodeDataInternal(
	    const char *title = NULL,
	    const char *message = NULL,
	    int width = 0,
	    int height = 0,
	    const std::vector<std::string> &
		initial_selection = std::vector<std::string>(),
	    HOM_EnumValue *node_type_filter = nullptr,
	    bool multiple_select = false,
	    bool include_data_type_headers = true,
	    bool include_parms=true,
	    bool include_object_transforms=true,
	    bool include_geometry_bounding_boxes=true,
	    bool include_geometry_attributes=true,
	    bool expand_components=true,
	    void *custom_data_callback=nullptr,
	    const char *help_url=nullptr) = 0;

    SWIGOUT(%kwargs selectParmTag;)
    virtual std::vector<std::string> selectParmTag() = 0;

    SWIGOUT(%kwargs selectParm;)
    virtual std::vector<std::string> selectParm(
	    HOM_NodeTypeCategory *category = NULL,
	    bool bound_parms_only = false,
	    HOM_Node *relative_to_node = NULL,
	    const char *message = NULL,
	    const char *title = NULL,
	    const std::vector<std::string> &
		    initial_parms = std::vector<std::string>(),
	    bool multiple_select = true) = 0;

    SWIGOUT(%kwargs selectParmTuple;)
    virtual std::vector<std::string> selectParmTuple(
	    HOM_NodeTypeCategory *category = NULL,
	    bool bound_parms_only = false,
	    HOM_Node *relative_to_node = NULL,
	    const char *message = NULL,
	    const char *title = NULL,
	    const std::vector<std::string> &
		    initial_parm_tuples = std::vector<std::string>(),
	    bool multiple_select = true) = 0;

    SWIGOUT(%kwargs selectColor;)
    SWIGOUT(%newobject selectColor;)
    virtual HOM_Color *selectColor(HOM_Color *initial_color = NULL) = 0;

    SWIGOUT(%kwargs _openColorEditor;)
    virtual void _openColorEditor(
	    void *color_changed_callback, bool include_alpha=false, 
	    HOM_Color *initial_color=nullptr, float initial_alpha=1.0f) = 0;

    virtual std::vector<HOM_Color> loadPaletteFile(const char *file) = 0;
    virtual void savePaletteFile(const char *file,
	    const std::vector<HOM_Color> &colors) = 0;

    SWIGOUT(%ignore _openValueLadder;)
    SWIGOUT(%kwargs _openValueLadder;)
    virtual void _openValueLadder(
	    float initial_value, void *value_changed_callback,
	    HOM_EnumValue &type=HOM_valueLadderType::Generic, 
	    HOM_EnumValue &data_type=HOM_valueLadderDataType::Float) = 0;
    virtual void updateValueLadder(
	    int cursor_x, int cursor_y, bool alt_key, bool shift_key) = 0;
    virtual void closeValueLadder() = 0;

    SWIGOUT(%kwargs displayFileDependencyDialog;)
    virtual UT_Tuple<bool, 
		std::vector<std::pair<HOM_ElemPtr<HOM_Parm>, std::string> > >
        displayFileDependencyDialog(
            HOM_Node *rop_node = NULL,
            const std::vector<std::string> &uploaded_files =
                std::vector<std::string>(),
	    const std::vector<std::string> &forced_unselected_patterns =
                std::vector<std::string>(),
	    const char *project_dir_variable="HIP",
	    bool is_standalone=true) = 0;

    virtual void displayNodeHelp(HOM_NodeType &node_type) = 0;
    virtual void openTypePropertiesDialog(
	    HOM_Node &node, bool promote_spare_parms=false,
	    bool immediately_save=false) = 0;
    virtual void openTypePropertiesDialog(
	    HOM_NodeType &node_type, bool promote_spare_parms=false,
	    bool immediately_save=false) = 0;

    virtual void openRenderDialog(HOM_RopNode &rop) = 0;

    virtual void openRenameSelectedDialog(HOM_Node &network) = 0;

    virtual HOM_EnumValue &updateMode() = 0;

    virtual void setUpdateMode(HOM_EnumValue &mode) = 0;
    virtual void triggerUpdate() = 0;
    virtual void reloadViewportColorSchemes() = 0;

    virtual std::vector<int> _getTabMenuIconSize() = 0;

    SWIGOUT(%ignore addOpaqueSelectionCallback;)
    virtual void addOpaqueSelectionCallback(void *py_callback) = 0;
    SWIGOUT(%ignore removeOpaqueSelectionCallback;)
    virtual void removeOpaqueSelectionCallback(void *py_callback) = 0;
    virtual void removeAllSelectionCallbacks() = 0;
    SWIGOUT(%ignore opaqueSelectionCallbacks;)
    virtual std::vector<void *> opaqueSelectionCallbacks() = 0;

    SWIGOUT(%ignore addOpaqueEventLoopCallback;)
    virtual void addOpaqueEventLoopCallback(void *callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventLoopCallback;)
    virtual void removeOpaqueEventLoopCallback(void *callback) = 0;
    SWIGOUT(%ignore opaqueEventLoopCallbacks;)
    virtual std::vector<void *> opaqueEventLoopCallbacks() = 0;

    SWIGOUT(%newobject createDialog;)
    virtual HOM_Dialog *createDialog(const char *ui_file_name) = 0;
    SWIGOUT(%newobject findDialog;)
    virtual HOM_Dialog *findDialog(const char *ui_file_name) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Dialog> > dialogs() = 0;

    SWIGOUT(%ignore opaqueWaitUntil;)
    virtual void opaqueWaitUntil(void *condition_callback) = 0;

    SWIGOUT(%kwargs writePythonShellHistoryFile;)
    virtual void writePythonShellHistoryFile(const char *filename=NULL) = 0;

    SWIGOUT(%kwargs readPythonShellHistoryFile;)
    virtual void readPythonShellHistoryFile(const char *filename=NULL) = 0;

    SWIGOUT(%kwargs setStatusMessage;)
    virtual void setStatusMessage(
	    const char *message,
	    HOM_EnumValue &severity=HOM_severityType::Message) = 0;

    virtual void openAssetUploadDialog(
	    HOM_Node *uploading_node, const char *session_key,
	    HOM_Node *containing_node) = 0;

    virtual void openAssetDependenciesDialog(
	    const std::vector<HOM_Node *> &uploading_nodes,
	    const std::vector<HOM_Node *> &uploaded_nodes,
	    const char *session_key,
	    HOM_Node *containing_node) = 0;

    virtual bool hasDragSourceData(const char *label) = 0;
    virtual HOM_DDSourceAny getDragSourceData(const char *label, int index) = 0;

    virtual std::string resourceValueFromName(const char *name) = 0;
    virtual HOM_Color colorFromName(const char *name) = 0;

    virtual double globalScaleFactor() = 0;
    virtual int scaledSize(int size) = 0;

    virtual double inchesToPixels(double inches) = 0;
    virtual double pixelsToInches(double pixels) = 0;

    virtual void copyTextToClipboard(const char *text) = 0;
    virtual std::string getTextFromClipboard() = 0;

    virtual std::vector<std::string> hotkeys(const char *hotkey_symbol) = 0;
    virtual std::string hotkeyDescription(const char *hotkey_symbol) = 0;
    virtual bool isKeyMatch(const char *key, const char *hotkey_symbol) = 0;

    virtual void openCaptureWeightSpreadsheet(HOM_Node *node, 
					      const char *pattern = NULL) = 0;

    virtual void registerViewerState(HOM_ViewerStateTemplate& vs_templ) = 0;
    virtual void unregisterViewerState(std::string const& state_typename) = 0;
    virtual bool isRegisteredViewerState(std::string const& state_name) = 0;
    virtual void reloadViewerState(std::string const& state_typename) = 0;
    SWIGOUT(%kwargs reloadViewerStates;)
    virtual void reloadViewerStates(
	std::vector<std::string> const& state_names=std::vector<std::string>()) = 0;

#ifdef SWIG
%extend
{
    SWIGOUT(%kwargs _selectNodeData;)
    std::vector<std::string> _selectNodeData(
	    const char *title = NULL,
	    const char *message = NULL,
	    int width = 0,
	    int height = 0,
	    const std::vector<std::string> &
		initial_selection = std::vector<std::string>(),
	    HOM_EnumValue *node_type_filter = nullptr,
	    bool multiple_select = false,
	    bool include_data_type_headers = true,
	    bool include_parms=true,
	    bool include_object_transforms=true,
	    bool include_geometry_bounding_boxes=true,
	    bool include_geometry_attributes=true,
	    bool expand_components=true,
	    InterpreterObject custom_data_callback=nullptr,
	    const char *help_url=nullptr)
    {
	if (custom_data_callback && custom_data_callback == Py_None)
	    custom_data_callback = nullptr;

	return self->_selectNodeDataInternal(
	    title,
	    message,
	    width,
	    height,
	    initial_selection,
	    node_type_filter,
	    multiple_select,
	    include_data_type_headers,
	    include_parms,
	    include_object_transforms,
	    include_geometry_bounding_boxes,
	    include_geometry_attributes,
	    expand_components,
	    custom_data_callback,
	    help_url
	);
    }

    SWIGOUT(%kwargs openColorEditor;)
    void openColorEditor(
	InterpreterObject color_changed_callback, 
	bool include_alpha=false, 
	HOM_Color *initial_color=nullptr, float initial_alpha=1.0f)
    { self->_openColorEditor(
	color_changed_callback, include_alpha, initial_color, initial_alpha); }

    SWIGOUT(%kwargs openValueLadder;)
    void openValueLadder(
	float initial_value, 
	InterpreterObject value_changed_callback, 
	HOM_EnumValue &type=HOM_valueLadderType::Generic, 
	HOM_EnumValue &data_type=HOM_valueLadderDataType::Float)
    { self->_openValueLadder(
	initial_value, value_changed_callback, type, data_type); }

    void addEventLoopCallback(InterpreterObject callback)
    { self->addOpaqueEventLoopCallback(callback); }

    void removeEventLoopCallback(InterpreterObject callback)
    { self->removeOpaqueEventLoopCallback(callback); }

    // Note that we return a vector of borrowed references because swig
    // will increment the reference counts.
    std::vector<InterpreterObject> eventLoopCallbacks()
    {
	std::vector<void *> opaque_callbacks(self->opaqueEventLoopCallbacks());

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }

    void waitUntil(InterpreterObject callback)
    { self->opaqueWaitUntil(callback); }

    void addSelectionCallback(InterpreterObject callback)
    { self->addOpaqueSelectionCallback(callback); }

    void removeSelectionCallback(InterpreterObject callback)
    { self->removeOpaqueSelectionCallback(callback); }

    std::vector<InterpreterObject> selectionCallbacks()
    {
	std::vector<void *> opaque_callbacks = self->opaqueSelectionCallbacks();
	std::vector<InterpreterObject> callbacks;

	for (int i = 0, n = opaque_callbacks.size(); i < n; i++)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);

	return callbacks;
    }
}
#endif

};

#endif
