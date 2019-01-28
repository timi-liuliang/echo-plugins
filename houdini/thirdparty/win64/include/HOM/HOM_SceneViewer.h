/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SceneViewer_h__
#define __HOM_SceneViewer_h__

#include "HOM_PathBasedPaneTab.h"
#include "HOM_EnumModules.h"
#include "HOM_BoundingBox.h"
#include "HOM_Vector3.h"
#include "HOM_Vector2.h"
#include "HOM_Matrix3.h"
#include <vector>

class HOM_ConstructionPlane;
class HOM_ReferencePlane;
class HOM_DopData;
class HOM_FlipbookSettings;
class HOM_GeometryViewport;
class HOM_GeometrySelection;
class HOM_Selection;
class HOM_ParmTemplateGroup;

SWIGOUT(%rename(SceneViewer) HOM_SceneViewer;)

class HOM_API HOM_SceneViewer : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_SceneViewer()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_SceneViewer(const HOM_SceneViewer &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_SceneViewer()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    // returns a stateViewerType enum, either Scene or SceneGraph.
    virtual HOM_EnumValue &viewerType() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_GeometryViewport> > viewports() = 0;
    SWIGOUT(%newobject findViewport;)
    virtual HOM_GeometryViewport *findViewport(const char *name) = 0;
    virtual HOM_GeometryViewport *curViewport() = 0;

    virtual std::string currentState() = 0;
    virtual void enterViewState(bool wait_for_exit = false) = 0;
    virtual void enterCurrentNodeState(bool wait_for_exit = false) = 0;
    virtual void enterTranslateToolState(bool wait_for_exit = false) = 0;
    virtual void enterRotateToolState(bool wait_for_exit = false) = 0;
    virtual void enterScaleToolState(bool wait_for_exit = false) = 0;
    SWIGOUT(%kwargs setCurrentState;)
    virtual void setCurrentState(const char *state, bool wait_for_exit = false,
				 const HOM_EnumValue &generate =
						HOM_stateGenerateMode::Insert,
				 bool request_new_on_generate = false) = 0;

    virtual std::vector<std::string> _getCurrentStateParmNames() = 0;
    virtual std::string _evalCurrentStateParm(const char *name) = 0;
    virtual bool _setCurrentStateParm(const char *name, const char* value) =0;
    virtual bool _pressCurrentStateButton(const char *name)=0;

    virtual bool isCreateInContext() = 0;

    virtual HOM_EnumValue& viewportLayout() = 0;
    virtual void setViewportLayout(HOM_EnumValue &layout, int single = -1) = 0;

    SWIGOUT(%kwargs selectObjects;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > selectObjects(
	    const char *prompt = "Select objects",
	    int sel_index = 0,
	    bool allow_drag = false,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    bool allow_multisel = true,
	    const std::vector<std::string> &allowed_types =
		std::vector<std::string>(1, "*"),
	    const char *icon = NULL,
	    const char *label = NULL,
	    const std::vector<std::string> &prior_selection_paths =
		std::vector<std::string>(),
	    const std::vector<std::string> &prior_selection_ids =
		std::vector<std::string>(),
	    const std::vector<HOM_Selection *> &prior_selections =
		std::vector<HOM_Selection *>(),
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr,
	    bool confirm_existing = false
	    ) = 0;

    SWIGOUT(%newobject selectGeometry;)
    SWIGOUT(%kwargs selectGeometry;)
    virtual HOM_GeometrySelection *selectGeometry(
	    const char *prompt = "Select geometry",
	    int sel_index = 0,
	    bool allow_drag = false,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    const char *initial_selection = NULL,
	    HOM_EnumValue *initial_selection_type = NULL,
	    bool ordered = false,
	    const std::vector<HOM_EnumValue *> geometry_types =
		std::vector<HOM_EnumValue *>(),
	    const std::vector<HOM_EnumValue *> primitive_types =
		std::vector<HOM_EnumValue *>(),
	    bool allow_obj_sel = true,
	    const char *icon = NULL,
	    const char *label = NULL,
	    const std::vector<std::string> &prior_selection_paths =
		std::vector<std::string>(),
	    const std::vector<std::string> &prior_selection_ids =
		std::vector<std::string>(),
	    const std::vector<HOM_Selection *> &prior_selections =
		std::vector<HOM_Selection *>(),
	    bool allow_other_sops = true,
	    bool consume_selections = true,
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr,
	    bool confirm_existing = false,
	    bool pick_at_obj_level = false
	    ) = 0;

    SWIGOUT(%kwargs selectDynamics;)
    virtual std::vector<HOM_ElemPtr<HOM_DopData> > selectDynamics(
	    const char *prompt = "Select dynamics objects",
	    int sel_index = 0,
	    bool allow_objects = true,
	    bool allow_modifiers = false,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    bool allow_multisel = true,
	    const char *icon = NULL,
	    const char *label = NULL,
	    const std::vector<std::string> &prior_selection_paths =
		std::vector<std::string>(),
	    const std::vector<std::string> &prior_selection_ids =
		std::vector<std::string>(),
	    const std::vector<HOM_Selection *> &prior_selections =
		std::vector<HOM_Selection *>(),
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr
		) = 0;

    SWIGOUT(%kwargs selectDynamicsPoints;)
    virtual std::vector<std::pair<HOM_ElemPtr<HOM_DopData>, HOM_ElemPtr<HOM_GeometrySelection> > > selectDynamicsPoints(
	    const char *prompt = "Select dynamics points",
	    int sel_index = 0,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    bool allow_multisel = true,
	    bool only_select_points = true,
	    bool object_based_point_selection = false,
	    bool use_last_selected_object = false,
	    const char *icon = NULL,
	    const char *label = NULL,
	    const std::vector<std::string> &prior_selection_paths =
		std::vector<std::string>(),
	    const std::vector<std::string> &prior_selection_ids =
		std::vector<std::string>(),
	    const std::vector<HOM_Selection *> &prior_selections =
		std::vector<HOM_Selection *>(),
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr
		)= 0;

    SWIGOUT(%kwargs selectDynamicsPolygons;)
    virtual std::vector<std::pair<HOM_ElemPtr<HOM_DopData>, HOM_ElemPtr<HOM_GeometrySelection> > > selectDynamicsPolygons(
	    const char *prompt = "Select dynamics polygons",
	    int sel_index = 0,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    bool object_based_point_selection = false,
	    bool use_last_selected_object = false,
	    const char *icon = NULL,
	    const char *label = NULL,
	    const std::vector<std::string> &prior_selection_paths =
		std::vector<std::string>(),
	    const std::vector<std::string> &prior_selection_ids =
		std::vector<std::string>(),
	    const std::vector<HOM_Selection *> &prior_selections =
		std::vector<HOM_Selection *>(),
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr
		) = 0;

    SWIGOUT(%kwargs selectSceneGraph;)
    virtual std::vector<std::string> selectSceneGraph(
	    const char *prompt = "Select scene graph items",
	    const std::vector<std::string> &preselection =
		  std::vector<std::string>(),
	    const HOM_EnumValue &prim_mask= HOM_scenePrimMask::ViewerSetting,
	    bool quick_select = false,
	    bool use_existing_selection = true,
	    bool confirm_existing = false,
	    bool allow_multisel = true,
	    bool allow_drag = true,
	    const char *path_prefix_mask = nullptr,
	    const char *prim_kind = nullptr) = 0;

	
    SWIGOUT(%newobject selectPositions;)
    SWIGOUT(%kwargs selectPositions;)
    virtual std::vector<HOM_ElemPtr<HOM_Vector3> > selectPositions(
	    const char *prompt = "Click to specify a position",
	    int number_of_positions = 1,
	    bool connect_positions = true,
	    bool show_coordinates = true,
	    const HOM_BoundingBox &bbox = HOM_BoundingBox(),
	    HOM_EnumValue &position_type = HOM_positionType::WorldSpace,
	    const char *icon = NULL,
	    const char *label = NULL,
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr
	    ) = 0;

    SWIGOUT(%kwargs selectOrientedPositions;)
    virtual std::vector<std::pair<HOM_ElemPtr<HOM_Vector3>, HOM_ElemPtr<HOM_Matrix3> > > selectOrientedPositions(
	    const char *prompt = "Click to specify a position",
	    int number_of_positions = 1,
	    bool connect_positions = true,
	    bool show_coordinates = true,
	    const HOM_Matrix3 &base_orientation = HOM_Matrix3(1.0),
	    const HOM_BoundingBox &bbox = HOM_BoundingBox(),
	    const char *icon = NULL,
	    const char *label = NULL,
	    HOM_ParmTemplateGroup *toolbox_templategroup = nullptr,
	    HOM_ParmTemplateGroup *toolbox1_templategroup = nullptr
	    ) = 0;

    SWIGOUT(%newobject currentGeometrySelection;)
    virtual HOM_GeometrySelection *currentGeometrySelection() = 0;

    virtual void setCurrentGeometrySelection(
	    HOM_EnumValue &geometry_type,
	    const std::vector<HOM_Node *> &nodes,
	    const std::vector<HOM_Selection *> &selections) = 0;

    virtual std::vector<std::string> currentSceneGraphSelection() = 0;
    virtual void setCurrentSceneGraphSelection(
	    const std::vector<std::string> &selection) = 0;

    // Snapping control
    virtual HOM_EnumValue &snappingMode() = 0;
    virtual void setSnappingMode(HOM_EnumValue &snapping_mode) = 0;

    virtual bool isSnappingToTemplates() = 0;
    virtual void setSnapToTemplates(bool on) = 0;

    virtual bool isSnappingToOtherObjects() = 0;
    virtual void setSnapToOtherObjects(bool on) = 0;

    virtual bool isDepthSnapping() = 0;
    virtual void setDepthSnapping(bool on) = 0;

    virtual bool isOrientingOnSnap() = 0;
    virtual void setOrientOnSnap(bool on) = 0;

    // Selection control
    virtual bool isPickingVisibleGeometry() = 0;
    virtual void setPickingVisibleGeometry(bool on) = 0;

    virtual bool isPickingContainedGeometry() = 0;
    virtual void setPickingContainedGeometry(bool on) = 0;
    
    virtual bool isGroupPicking() = 0;
    virtual void setGroupPicking(bool on) = 0;

    virtual bool isWholeGeometryPicking() = 0;
    virtual void setWholeGeometryPicking(bool on) = 0;

    virtual bool isSecureSelection() = 0;
    virtual void setSecureSelection(bool on) = 0;

    virtual bool isPickingCurrentNode() = 0;
    virtual void setPickingCurrentNode(bool on) = 0;

    virtual HOM_EnumValue &pickGeometryType() = 0;
    virtual void setPickGeometryType(HOM_EnumValue &geometry_type) = 0;

    virtual HOM_EnumValue &selectionMode() = 0;
    virtual void setSelectionMode(HOM_EnumValue &style) = 0;
    
    virtual HOM_EnumValue &pickStyle() = 0;
    virtual void setPickStyle(HOM_EnumValue &style) = 0;

    virtual HOM_EnumValue &pickModifier() = 0;
    virtual void setPickModifier(HOM_EnumValue &modifier) = 0;

    virtual HOM_EnumValue &defaultPickModifier() = 0;
    virtual void setDefaultPickModifier(HOM_EnumValue &modifier) = 0;

    virtual HOM_EnumValue &pickFacing() = 0;
    virtual void setPickFacing(HOM_EnumValue &facing) = 0;

    // Group list control
    virtual bool isGroupListVisible() = 0;
    virtual void setGroupListVisible(bool on) = 0;

    virtual bool isGroupListColoringGeometry() = 0;
    virtual void setGroupListColoringGeometry(bool on) = 0;

    virtual bool isGroupListShowingEmptyGroups() = 0;
    virtual void setGroupListShowingEmptyGroups(bool on) = 0;

    virtual bool isGroupListShowingOnlyPreSelectedGroups() = 0;
    virtual void setGroupListShowingOnlyPreSelectedGroups(bool on) = 0;

    virtual bool isGroupListCondensingPathHierarchies() = 0;
    virtual void setGroupListCondensingPathHierarchies(bool on) = 0;

    virtual HOM_Vector2 *groupListSize() = 0;
    virtual void setGroupListSize(double width, double height) = 0;

    virtual HOM_EnumValue &groupListType() = 0;
    virtual void setGroupListType(HOM_EnumValue &group_list_type) = 0;

    virtual std::string groupListMask() = 0;
    virtual void setGroupListMask(const char *mask) = 0;

    // Construction plane access
    SWIGOUT(%newobject constructionPlane;)
    virtual HOM_ConstructionPlane *constructionPlane() = 0;

    // Reference plane access
    SWIGOUT(%newobject referencePlane;)
    virtual HOM_ReferencePlane *referencePlane() = 0;

    SWIGOUT(%newobject flipbookSettings;)
    virtual HOM_FlipbookSettings *flipbookSettings() = 0;

    SWIGOUT(%kwargs flipbook;)
    virtual void flipbook(HOM_GeometryViewport *viewport = NULL,
			  HOM_FlipbookSettings *settings = NULL,
			  bool open_dialog = false) = 0;

    virtual void runShelfTool(const char *tool_name) = 0;

    virtual void displayRadialMenu(const std::string &name) = 0;

    virtual void beginStateUndo(const char *label) = 0;
    virtual void endStateUndo() = 0;

    virtual void setPromptMessage(
	std::string const & msg,
	HOM_EnumValue const & msg_type = HOM_promptMessageType::Prompt) = 0;
    virtual void clearPromptMessage() = 0;

    virtual void showHandle( std::string const& name, bool value ) = 0;
    // Scene graph-specific
#ifdef SWIG
%extend
{
    SWIGOUT(%kwargs stage;)
    InterpreterObject stage()
    { return HOMincRef((InterpreterObject)self->opaqueStage()); }
}
#endif
    SWIGOUT(%ignore opaqueStage;)
    virtual void *opaqueStage() = 0;

    virtual int	  stageSerial() = 0;
    
    virtual void setShowGeometry(bool show) = 0;
    virtual bool showGeometry() = 0;
    virtual void setShowCameras(bool show) = 0;
    virtual bool showCameras() = 0;
    virtual void setShowLights(bool show) = 0;
    virtual bool showLights() = 0;
    virtual void setShowSelection(bool show) = 0;
    virtual bool showSelection() = 0;
    virtual void setDenoise(bool show) = 0;
    virtual bool denoise() = 0;

    virtual std::vector<std::string> hydraRenderers() = 0;
    virtual void setHydraRenderer(const std::string &ren_name) = 0;
    virtual std::string currentHydraRenderer() = 0;
};

#endif
