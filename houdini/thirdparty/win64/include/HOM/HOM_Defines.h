/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Defines_h__
#define __HOM_Defines_h__

#include <UT/UT_Assert.h>

// Unfortunately, we can't compare against UT_ASSERT_LEVEL_PARANOID here,
// otherwise the comparison sometimes succeeds under swig when it shouldn't.
// So, we instead compare against the numeric value itself.
#if UT_ASSERT_LEVEL >= 2
#define HOM_DEBUG
#endif

#ifdef SWIG
#define SWIGOUT(x) x
#else
#define SWIGOUT(x)
#endif

#if defined(SWIG) && defined(SWIGPYTHON)
#define SWIGPYTHONOUT(x) x
#else
#define SWIGPYTHONOUT(x)
#endif

// Define a synonym for hboost::any to customize a typemap for arguments that
// are to hold values to be inserted into a UT_Options object.  SWIG allows
// typemaps to be defined for each typename independently.
#include <hboost/any.hpp>
typedef hboost::any HOM_UTOptionAny;

// Define a synonym for hboost::any to customize a typemap for arguments that
// are to return drag-n-drop source data.  SWIG allows typemaps to be defined
// for each typename independently.
typedef hboost::any HOM_DDSourceAny;

#ifdef SWIG

//----------------------------------------------------------------------------

// Since there's no way for a C++ function to have varying return types,
// we need to add special methods and functions that are only known
// to swig.  For python, there's a typemap for PyObject* so that when
// swig encounters a function returning that type it won't try to wrap
// the return value, and will instead send it directly to the interpreter.
// We create a typemap for this type so our code can use it instead of
// PyObject* directly so it's easier to use more than just python in the
// future.

// The swig code we output directly to the swig wrapper below (using %{%})
// uses InterpreterObject, so we need to say what it is.
%{
#ifdef SWIGPYTHON
typedef PyObject *InterpreterObject;

PyObject *HOMincRef(PyObject *object)
{
    if (!object)
	object = Py_None;
    Py_INCREF(object);
    return object;
}
#endif
%}

// We need to tell swig what InterpreterObject is so it applies the PyObject*
// typemap properly.
typedef PyObject *InterpreterObject;

//----------------------------------------------------------------------------

%{

#include <UT/UT_StdUtil.h>

// These helper functions convert from C++ objects to corresponding
// interpreter objects (PyObject*'s in python).  They're roughly the same
// as swig::from(), but swig::from() does a slow lookup on the name of the
// type.  These functions know the type at compile-time so they're faster.
// Unfortunately, we need to explicitly specialize all the types supported by
// the functions.  Specifically, since HOM_IterableList calls these functions,
// objects that can be inside HOM_IterableList need to be specialized below.

// The general case is left unimplemented so we get compiler errors if
// we call the function with an unspecialized type.
template <typename T> InterpreterObject
HOMconvertValueForInterpreter(const T &value, int own);

// These functions specialize the native data types.
template <> InterpreterObject
HOMconvertValueForInterpreter<int>(const int& value, int /*own*/)
{ return SWIG_From_int(value); }

template <> InterpreterObject
HOMconvertValueForInterpreter<float>(const float& value, int /*own*/)
{ return SWIG_From_float(value); }

template <> InterpreterObject
HOMconvertValueForInterpreter<double>(const double& value, int /*own*/)
{ return SWIG_From_double(value); }

template <> InterpreterObject
HOMconvertValueForInterpreter<std::string>(
    const std::string& value, int /*own*/)
{ return SWIG_From_std_string(value); }

// These functions specialize base classes to return interpreter instances
// of the proper subclass.
template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_Node*>(HOM_Node *const &node, int own)
{
    PY_InterpreterAutoLock py_lock;
    OP_OpTypeId		 optypeid;

    if (!node)
	return SWIG_NewPointerObj(node, SWIGTYPE_p_HOM_Node, own);

    optypeid = (OP_OpTypeId)node->opTypeIdAsInt();
    switch (optypeid)
    {
    case OBJ_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_ObjNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
	return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_ObjNode* const>(node),
		SWIGTYPE_p_HOM_ObjNode, own);

    case SOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_SopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_SopNode* const>(node),
	    SWIGTYPE_p_HOM_SopNode, own);

    case CHOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_ChopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ChopNode* const>(node),
	    SWIGTYPE_p_HOM_ChopNode, own);

    case COP2_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_CopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_CopNode* const>(node),
	    SWIGTYPE_p_HOM_CopNode, own);

    case DOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_DopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_DopNode* const>(node),
	    SWIGTYPE_p_HOM_DopNode, own);

    case POP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_PopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_PopNode* const>(node),
	    SWIGTYPE_p_HOM_PopNode, own);

    case POPNET_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_PopNetNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_PopNetNode* const>(node),
	    SWIGTYPE_p_HOM_PopNetNode, own);

    case SHOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_ShopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ShopNode* const>(node),
	    SWIGTYPE_p_HOM_ShopNode, own);

    case VOPNET_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_VopNetNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_VopNetNode* const>(node),
	    SWIGTYPE_p_HOM_VopNetNode, own);

    case ROP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_RopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_RopNode* const>(node),
	    SWIGTYPE_p_HOM_RopNode, own);

    case VOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_VopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
	return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_VopNode* const>(node),
	    SWIGTYPE_p_HOM_VopNode, own);

    case TOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_TopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_TopNode* const>(node),
	    SWIGTYPE_p_HOM_TopNode, own);

    case LOP_OPTYPE_ID:
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_LopNode so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_LopNode* const>(node),
	    SWIGTYPE_p_HOM_LopNode, own);

    case MGR_OPTYPE_ID:
	// Special case for the /stage manager node, which is actually a
	// LOP_Network, and therefore a LOP_Node.
	if (HOM().lopNodeTypeCategory() == node->childTypeCategory())
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_LopNode* const>(node),
		SWIGTYPE_p_HOM_LopNode, own);

    default:
        break;
    };

    return SWIG_NewPointerObj(node, SWIGTYPE_p_HOM_Node, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_NodeType*>(
    HOM_NodeType *const &nodetype, int own)
{
    PY_InterpreterAutoLock py_lock;
    OP_OpTypeId		 optypeid;
	
    if (!nodetype)
	return SWIG_NewPointerObj(nodetype, SWIGTYPE_p_HOM_NodeType, own);

    if( !nodetype->managerFlag() )
    {
	optypeid = (OP_OpTypeId)nodetype->opTypeIdAsInt();
	switch (optypeid)
	{
	case SOP_OPTYPE_ID:
	    // Because of multiple inheritance, it's extremely important that
	    // we explicitly cast the pointer to a HOM_SopNodeType so we
	    // pass the right address to SWIG_NewPointerObject, since it takes
	    // a void*.
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_SopNodeType* const>(nodetype),
		SWIGTYPE_p_HOM_SopNodeType, own);

	case SHOP_OPTYPE_ID:
	    // Because of multiple inheritance, it's extremely important that
	    // we explicitly cast the pointer to a HOM_ShopNodeType so we
	    // pass the right address to SWIG_NewPointerObject, since it takes
	    // a void*.
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_ShopNodeType* const>(nodetype),
		SWIGTYPE_p_HOM_ShopNodeType, own);

	case VOP_OPTYPE_ID:
	    // Because of multiple inheritance, it's extremely important that
	    // we explicitly cast the pointer to a HOM_VopNodeType so we
	    // pass the right address to SWIG_NewPointerObject, since it takes
	    // a void*.
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_VopNodeType* const>(nodetype),
		SWIGTYPE_p_HOM_VopNodeType, own);
	default:
	    break;
	}
    }

    return SWIG_NewPointerObj(nodetype, SWIGTYPE_p_HOM_NodeType, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_BaseKeyframe*>(
    HOM_BaseKeyframe *const &keyframe, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!keyframe)
	return SWIG_NewPointerObj(keyframe, SWIGTYPE_p_HOM_BaseKeyframe, own);

    if (keyframe->evaluatedType() == HOM_parmData::Float)
    {
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_Keyframe so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Keyframe* const>(keyframe),
	    SWIGTYPE_p_HOM_Keyframe, own);
    }
    else if (keyframe->evaluatedType() == HOM_parmData::String)
    {
	// Because of multiple inheritance, it's extremely important that
	// we explicitly cast the pointer to a HOM_Keyframe so we
	// pass the right address to SWIG_NewPointerObject, since it takes
	// a void*.
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_StringKeyframe* const>(keyframe),
	    SWIGTYPE_p_HOM_StringKeyframe, own);
    }

    return SWIG_NewPointerObj(keyframe, SWIGTYPE_p_HOM_BaseKeyframe, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_PackedPrim*>(HOM_PackedPrim *const &prim, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!prim)
	return SWIG_NewPointerObj(prim, SWIGTYPE_p_HOM_PackedPrim, own);

    // Because of multiple inheritance, it's extremely important that
    // we explicitly cast the pointer to the right HOM type so we
    // pass the right address to SWIG_NewPointerObject, since it takes
    // a void*.

    switch (prim->type().id())
    {
    case HOM_primType::Agent_Id:
	return SWIG_NewPointerObj(
	    (void *)dynamic_cast<const HOM_Agent *const>(prim),
	    SWIGTYPE_p_HOM_Agent, own);

    case HOM_primType::PackedFragment_Id:
	return SWIG_NewPointerObj(
	    (void *)dynamic_cast<const HOM_PackedFragment *const>(prim),
	    SWIGTYPE_p_HOM_PackedFragment, own);

    case HOM_primType::PackedGeometry_Id:
	return SWIG_NewPointerObj(
	    (void *)dynamic_cast<const HOM_PackedGeometry *const>(prim),
	    SWIGTYPE_p_HOM_PackedGeometry, own);
    }

    return SWIG_NewPointerObj(
	(void *)dynamic_cast<const HOM_PackedPrim *const>(prim),
	SWIGTYPE_p_HOM_PackedPrim, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_Prim*>(HOM_Prim *const &prim, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!prim)
	return SWIG_NewPointerObj(prim, SWIGTYPE_p_HOM_Prim, own);

    // Because of multiple inheritance, it's extremely important that
    // we explicitly cast the pointer to the right HOM type so we
    // pass the right address to SWIG_NewPointerObject, since it takes
    // a void*.
    HOM_PackedPrim * packed = dynamic_cast<HOM_PackedPrim *>(prim);
    if(packed)
	return HOMconvertValueForInterpreter(packed, own);

    switch (prim->type().id())
    {
    case HOM_primType::Polygon_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Polygon* const>(prim),
	    SWIGTYPE_p_HOM_Polygon, own);

    case HOM_primType::NURBSCurve_Id:
    case HOM_primType::BezierCurve_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Face* const>(prim),
	    SWIGTYPE_p_HOM_Face, own);

    case HOM_primType::Mesh_Id:
    case HOM_primType::NURBSSurface_Id:
    case HOM_primType::BezierSurface_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Surface* const>(prim),
	    SWIGTYPE_p_HOM_Surface, own);

    case HOM_primType::Circle_Id:
    case HOM_primType::Sphere_Id:
    case HOM_primType::Tube_Id:
	return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Quadric* const>(prim),
	    SWIGTYPE_p_HOM_Quadric, own);

    case HOM_primType::Volume_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_Volume* const>(prim),
	    SWIGTYPE_p_HOM_Volume, own);

    case HOM_primType::VDB_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_VDB* const>(prim),
	    SWIGTYPE_p_HOM_VDB, own);

    // TODO: Support more primitive types.
    }

    return SWIG_NewPointerObj(prim, SWIGTYPE_p_HOM_Prim, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_ParmTemplate*>(
    HOM_ParmTemplate *const &parm_template, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!parm_template)
	return SWIG_NewPointerObj(
	    parm_template, SWIGTYPE_p_HOM_ParmTemplate, own);

    // Because of multiple inheritance, it's extremely important that we
    // explicitly cast to a pointer to the subclass so we pass the right
    // address to SWIG_NewPointerObject, since it takes a void*.
    switch (parm_template->type().id())
    {
    case HOM_parmTemplateType::Int_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_IntParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_IntParmTemplate, own);

    case HOM_parmTemplateType::Float_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_FloatParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_FloatParmTemplate, own);

    case HOM_parmTemplateType::String_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_StringParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_StringParmTemplate, own);

    case HOM_parmTemplateType::Data_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_DataParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_DataParmTemplate, own);

    case HOM_parmTemplateType::Toggle_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ToggleParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_ToggleParmTemplate, own);

    case HOM_parmTemplateType::Menu_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_MenuParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_MenuParmTemplate, own);

    case HOM_parmTemplateType::Button_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ButtonParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_ButtonParmTemplate, own);

    case HOM_parmTemplateType::Label_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_LabelParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_LabelParmTemplate, own);

    case HOM_parmTemplateType::Separator_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_SeparatorParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_SeparatorParmTemplate, own);

    case HOM_parmTemplateType::FolderSet_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_FolderSetParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_FolderSetParmTemplate, own);

    case HOM_parmTemplateType::Folder_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_FolderParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_FolderParmTemplate, own);

    case HOM_parmTemplateType::Ramp_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_RampParmTemplate* const>(
		parm_template),
	    SWIGTYPE_p_HOM_RampParmTemplate, own);
    };

    UT_ASSERT(!"Unknown parm template type");
    return SWIG_NewPointerObj(parm_template, SWIGTYPE_p_HOM_ParmTemplate, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_PaneTab*>(
    HOM_PaneTab *const &pane_tab, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!pane_tab)
	return SWIG_NewPointerObj(pane_tab, SWIGTYPE_p_HOM_PaneTab, own);

    // Because of multiple inheritance, it's extremely important that
    // we explicitly cast the pointer to the HOM type so we pass the right
    // address to SWIG_NewPointerObject, since it takes a void*.

    switch (pane_tab->type().id())
    {
    case HOM_paneTabType::ContextViewer_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ContextViewer* const>(pane_tab),
	    SWIGTYPE_p_HOM_ContextViewer, own);

    case HOM_paneTabType::SceneViewer_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_SceneViewer* const>(pane_tab),
	    SWIGTYPE_p_HOM_SceneViewer, own);

    case HOM_paneTabType::CompositorViewer_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_CompositorViewer* const>(pane_tab),
	    SWIGTYPE_p_HOM_CompositorViewer, own);

    case HOM_paneTabType::NetworkEditor_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_NetworkEditor* const>(pane_tab),
	    SWIGTYPE_p_HOM_NetworkEditor, own);
	    
    case HOM_paneTabType::HelpBrowser_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_HelpBrowser* const>(pane_tab),
	    SWIGTYPE_p_HOM_HelpBrowser, own);

    case HOM_paneTabType::PythonPanel_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_PythonPanel* const>(pane_tab),
	    SWIGTYPE_p_HOM_PythonPanel, own);

    case HOM_paneTabType::IPRViewer_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_IPRViewer* const>(pane_tab),
	    SWIGTYPE_p_HOM_IPRViewer, own);

    case HOM_paneTabType::AssetBrowser_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_AssetBrowser* const>(pane_tab),
	    SWIGTYPE_p_HOM_AssetBrowser, own);

    case HOM_paneTabType::PerformanceMonitor_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_PerformanceMonitor* const>(pane_tab),
	    SWIGTYPE_p_HOM_PerformanceMonitor, own);

    case HOM_paneTabType::ChannelEditor_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_ChannelEditorPane* const>(pane_tab),
	    SWIGTYPE_p_HOM_ChannelEditorPane, own);

    case HOM_paneTabType::DataTree_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_DataTree* const>(pane_tab),
	    SWIGTYPE_p_HOM_DataTree, own);

    case HOM_paneTabType::SceneGraphTree_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_SceneGraphTree* const>(pane_tab),
	    SWIGTYPE_p_HOM_SceneGraphTree, own);

    case HOM_paneTabType::ChannelViewer_Id:
    case HOM_paneTabType::OutputViewer_Id:
    case HOM_paneTabType::ShaderViewer_Id:
    case HOM_paneTabType::Parm_Id:
    case HOM_paneTabType::DetailsView_Id:
    case HOM_paneTabType::TreeView_Id:
    case HOM_paneTabType::TaskGraphTable_Id:
        return SWIG_NewPointerObj(
	    (void*)dynamic_cast<const HOM_PathBasedPaneTab* const>(pane_tab),
	    SWIGTYPE_p_HOM_PathBasedPaneTab, own);

    case HOM_paneTabType::ChannelList_Id:
    case HOM_paneTabType::Textport_Id:
    case HOM_paneTabType::PythonShell_Id:
    case HOM_paneTabType::HandleList_Id:
    case HOM_paneTabType::BundleList_Id:
    case HOM_paneTabType::TakeList_Id:
    case HOM_paneTabType::ParmSpreadsheet_Id:
    case HOM_paneTabType::LightLinker_Id:
    case HOM_paneTabType::MaterialPalette_Id:
	return SWIG_NewPointerObj(pane_tab, SWIGTYPE_p_HOM_PaneTab, own);
    };

    UT_ASSERT(!"Unknown pane tab type");
    return SWIG_NewPointerObj(pane_tab, SWIGTYPE_p_HOM_PaneTab, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_SopVerb*>(
    HOM_SopVerb *const &verb, int own)
{
    PY_InterpreterAutoLock py_lock;

    return SWIG_NewPointerObj(verb, SWIGTYPE_p_HOM_SopVerb, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_Geometry*>(
    HOM_Geometry *const &geo, int own)
{
    PY_InterpreterAutoLock py_lock;

    return SWIG_NewPointerObj(geo, SWIGTYPE_p_HOM_Geometry, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_NetworkItem*>(
    HOM_NetworkItem *const &item, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (item)
    {
	switch (item->networkItemType().id())
	{
	case HOM_networkItemType::Node_Id:
	    // We know its a node, but we need to convert to the specific
	    // node type referred to by this item.
	    return HOMconvertValueForInterpreter(
		    dynamic_cast<HOM_Node* const>(item), own);

	case HOM_networkItemType::NetworkBox_Id:
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_NetworkBox* const>(item),
		SWIGTYPE_p_HOM_NetworkBox, own);

	case HOM_networkItemType::StickyNote_Id:
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_StickyNote* const>(item),
		SWIGTYPE_p_HOM_StickyNote, own);

	case HOM_networkItemType::SubnetIndirectInput_Id:
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_SubnetIndirectInput* const>(item),
		SWIGTYPE_p_HOM_SubnetIndirectInput, own);

	case HOM_networkItemType::Connection_Id:
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_NodeConnection* const>(item),
		SWIGTYPE_p_HOM_NodeConnection, own);

	case HOM_networkItemType::NetworkDot_Id:
	    return SWIG_NewPointerObj(
		(void*)dynamic_cast<const HOM_NetworkDot* const>(item),
		SWIGTYPE_p_HOM_NetworkDot, own);

	default:
	    break;
	};
    }

    return SWIG_NewPointerObj(item, SWIGTYPE_p_HOM_NetworkItem, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_NetworkMovableItem*>(
    HOM_NetworkMovableItem *const &item, int own)
{
    PY_InterpreterAutoLock py_lock;

    if (!item)
	return SWIG_NewPointerObj(item, SWIGTYPE_p_HOM_NetworkMovableItem, own);

    return HOMconvertValueForInterpreter<HOM_NetworkItem *>(item, own);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<
    std::vector<HOM_ElemPtr<HOM_NodeConnection> >*>(
	std::vector<HOM_ElemPtr<HOM_NodeConnection> > *const &list,
	int own)
{
    return swig::from(*list);
}

template <> InterpreterObject
HOMconvertValueForInterpreter<HOM_DopData*>(HOM_DopData *const &data, int own)
{
    PY_InterpreterAutoLock py_lock;

    const HOM_DopObject *const dop_object =
	dynamic_cast<const HOM_DopObject *const>(data);
    if (dop_object)
        return SWIG_NewPointerObj(
	    (void *)dop_object, SWIGTYPE_p_HOM_DopObject, own);

    const HOM_DopRelationship *const dop_relationship =
	dynamic_cast<const HOM_DopRelationship *const>(data);
    if (dop_relationship)
        return SWIG_NewPointerObj(
	    (void *)dop_relationship, SWIGTYPE_p_HOM_DopRelationship, own);

    return SWIG_NewPointerObj(data, SWIGTYPE_p_HOM_DopData, own);
}

// These functions specialize pointers to other classes.  If a class is
// used inside HOM_IterableList or HOM_ElemPtr, it must be listed here.
#define HOM_PROVIDE_SWIG_LOOKUP(type) \
    template <> InterpreterObject HOMconvertValueForInterpreter<type*>( \
	type* const& value, int own) \
    { \
	PY_InterpreterAutoLock py_lock; \
	return SWIG_NewPointerObj(SWIG_as_voidptr(value), \
            SWIGTYPE_p_ ## type, own); \
    }

HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentClip)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentDefinition)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentLayer)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentShape)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentShapeBinding)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentShapeDeformer)
HOM_PROVIDE_SWIG_LOOKUP(HOM_AgentTransformGroup)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Attrib)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Desktop)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Dialog)
HOM_PROVIDE_SWIG_LOOKUP(HOM_DopRecord)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Drawable)
HOM_PROVIDE_SWIG_LOOKUP(HOM_FloatingPanel)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Gallery)
HOM_PROVIDE_SWIG_LOOKUP(HOM_GalleryEntry)
HOM_PROVIDE_SWIG_LOOKUP(HOM_GeometrySelection)
HOM_PROVIDE_SWIG_LOOKUP(HOM_GeometryViewport)
HOM_PROVIDE_SWIG_LOOKUP(HOM_HDADefinition)
HOM_PROVIDE_SWIG_LOOKUP(HOM_HDASection)
HOM_PROVIDE_SWIG_LOOKUP(HOM_IndexPairPropertyTable)
HOM_PROVIDE_SWIG_LOOKUP(HOM_LopSelectionRule)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Matrix2)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Matrix3)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Matrix4)
HOM_PROVIDE_SWIG_LOOKUP(HOM_NetworkBox)
HOM_PROVIDE_SWIG_LOOKUP(HOM_NetworkDot)
HOM_PROVIDE_SWIG_LOOKUP(HOM_NodeBundle)
HOM_PROVIDE_SWIG_LOOKUP(HOM_NodeConnection)
HOM_PROVIDE_SWIG_LOOKUP(HOM_NodeGroup)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Pane)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Parm)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ParmTuple)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Point)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Polygon)
HOM_PROVIDE_SWIG_LOOKUP(HOM_PythonPanelInterface)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Edge)
HOM_PROVIDE_SWIG_LOOKUP(HOM_PointGroup)
HOM_PROVIDE_SWIG_LOOKUP(HOM_PrimGroup)
HOM_PROVIDE_SWIG_LOOKUP(HOM_EdgeGroup)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Quaternion)
HOM_PROVIDE_SWIG_LOOKUP(HOM_RadialItem)
HOM_PROVIDE_SWIG_LOOKUP(HOM_RadialSubmenu)
HOM_PROVIDE_SWIG_LOOKUP(HOM_RadialMenu)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Ramp)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Selection)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Selector)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Shelf)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ShelfSet)
HOM_PROVIDE_SWIG_LOOKUP(HOM_StickyNote)
HOM_PROVIDE_SWIG_LOOKUP(HOM_SubnetIndirectInput)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Take)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Tool)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Track)
HOM_PROVIDE_SWIG_LOOKUP(HOM_UIEvent)
HOM_PROVIDE_SWIG_LOOKUP(HOM_UIEventDevice)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Vector2)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Vector3)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Vector4)
HOM_PROVIDE_SWIG_LOOKUP(HOM_Vertex)
HOM_PROVIDE_SWIG_LOOKUP(HOM_VexContext)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ViewerState)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ViewerStateMenu)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ViewerStateTemplate)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ViewportVisualizer)
HOM_PROVIDE_SWIG_LOOKUP(HOM_ViewportVisualizerType)

%}

//----------------------------------------------------------------------------

// We create typemaps for base classes that appear as return types to make
// sure swig instances the proper subclasses in the interpreter.
%typemap(out) HOM_Prim* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_PackedPrim* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_Node* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_NodeType* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_BaseKeyframe* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_ParmTemplate* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_Pane* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_PaneTab* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_DopData* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_NetworkMovableItem* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) HOM_NetworkItem* {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

%typemap(out) std::vector<HOM_ElemPtr<HOM_NodeConnection> > * {
    $result = HOMconvertValueForInterpreter($1, $owner);
}

//----------------------------------------------------------------------------

// These typemaps convert HOM exceptions into corresponding python standard
// exceptions.
%typemap(throws) HOM_TypeError %{
    SWIG_exception_fail(SWIG_TypeError, $1.instanceMessage().c_str());
%}

%typemap(throws) HOM_ValueError %{
    SWIG_exception_fail(SWIG_ValueError, $1.instanceMessage().c_str());
%}

#ifdef SWIGPYTHON
%typemap(throws) HOM_SystemExit %{
    PyErr_SetObject(PyExc_SystemExit, SWIG_From_int($1.code()));
    SWIG_fail;
%}
#endif

//----------------------------------------------------------------------------

// This typemap lets you declare a hboost::any parameter and swig will convert
// the interpreter (Python) object to a hboost::any that wraps the C++ data
// type.
%typemap(in) hboost::any {
    // Note that if we can't convert to a hboost::any, we leave the hboost::any
    // as empty (i.e. its empty() method returns true).
    HOMinterpreterObjectToBoostAny($input, $1);
}

// This typemap lets you declare a HOM_UTOptionAny (hboost::any) parameter and
// swig will convert the interpreter (Python) object into the C++ data type
// for interpreter objects that correspond to a UT_OptionType.
%typemap(in) HOM_UTOptionAny {
    // Note that if we can't convert to a hboost::any, we leave the hboost::any
    // as empty (i.e. its empty() method returns true).
    HOMinterpreterObjectToUTOptionAny($input, $1);
}

// This typemap lets you return hboost::any objects.
%typemap(out) hboost::any {
    $result = HOMboostAnyToInterpreterObject($1);
}

%typemap(out) HOM_DDSourceAny {
    $result = HOMDDSourceAnyToInterpreterObject($1);
}

//----------------------------------------------------------------------------

#ifdef SWIGPYTHON
%typemap(out) PY_OpaqueObject {
    $result = HOMincRef((InterpreterObject)$1.opaqueObject());
}
#endif

// For whatever reason, adding the typemap does not customize swig::from(),
// so we do that manually.
%{
#ifdef SWIGPYTHON
namespace swig {
    template <>
    struct traits_from<PY_OpaqueObject>
    {
        static PyObject *from(const PY_OpaqueObject &val)
        {
            return HOMincRef((InterpreterObject)val.opaqueObject());
        }
    };
}
#endif

//----------------------------------------------------------------------------

// This helper function takes an interpreter (Python) object and converts it
// into a hboost::any object.  It returns whether or not the conversion was
// possible, and leaves the hboost::any unchanged if a conversion wasn't
// possible.
bool
HOMinterpreterObjectToBoostAny(InterpreterObject input, hboost::any &result)
{
#ifdef SWIGPYTHON
    if (input == Py_None)
    {
	result = (void *)NULL;
	return true;
    }

    // SWIG_AsVal_bool is too permissive, and treats thinks that an integer
    // is a bool.  So, we explicitly check against True and False.
    if (input == Py_True)
    {
	result = true;
	return true;
    }

    if (input == Py_False)
    {
	result = false;
	return true;
    }
#endif

    int int_result;
    if (SWIG_IsOK(SWIG_AsVal_int(input, &int_result)))
    {
        result = int_result;
	return true;
    }

    double double_result;
    if (SWIG_IsOK(SWIG_AsVal_double(input, &double_result)))
    {
        result = double_result;
	return true;
    }

    std::string string_result;
    if (SWIG_IsOK(SWIG_AsVal_std_string(input, &string_result)))
    {
        result = string_result;
	return true;
    }

    void *geometry_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &geometry_result,
				  SWIGTYPE_p_HOM_Geometry, 0)) && geometry_result)
    {
	result = reinterpret_cast<HOM_Geometry *>(geometry_result);
	return true;
    }

    void *ramp_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &ramp_result,
				  SWIGTYPE_p_HOM_Ramp, 0)) && ramp_result)
    {
	result = reinterpret_cast<HOM_Ramp *>(ramp_result);
	return true;
    }

    std::vector<int> *int_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<int> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &int_vector_ptr)))
    {
        result = *int_vector_ptr;
        delete int_vector_ptr;
	return true;
    }

    std::vector<double> *double_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<double> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &double_vector_ptr)))
    {
        result = *double_vector_ptr;
        delete double_vector_ptr;
	return true;
    }

    std::vector<std::string> *string_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<std::string> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &string_vector_ptr)))
    {
        result = *string_vector_ptr;
        delete string_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector2> *vector2_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector2> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector2_vector_ptr)))
    {
	result = *vector2_vector_ptr;
	delete vector2_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector3> *vector3_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector3> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector3_vector_ptr)))
    {
	result = *vector3_vector_ptr;
	delete vector3_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector4> *vector4_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector4> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector4_vector_ptr)))
    {
	result = *vector4_vector_ptr;
	delete vector4_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix2> *matrix2_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix2> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix2_vector_ptr)))
    {
	result = *matrix2_vector_ptr;
	delete matrix2_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix3> *matrix3_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix3> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix3_vector_ptr)))
    {
	result = *matrix3_vector_ptr;
	delete matrix3_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix4> *matrix4_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix4> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix4_vector_ptr)))
    {
	result = *matrix4_vector_ptr;
	delete matrix4_vector_ptr;
	return true;
    }

    UT_ASSERT(result.empty());
    return false;
}

// This helper function takes an interpreter (Python) object and converts it
// into a HOM_UTOptionAny (hboost::any) object.  It returns whether or not the
// conversion was possible, and leaves the HOM_UTOptionAny unchanged if a
// conversion wasn't possible.
bool
HOMinterpreterObjectToUTOptionAny(InterpreterObject input,
				  HOM_UTOptionAny &result)
{
#ifdef SWIGPYTHON
    if (input == Py_None)
    {
	result = (void *)NULL;
	return true;
    }

    // SWIG_AsVal_bool is too permissive, and treats thinks that an integer
    // is a bool.  So, we explicitly check against True and False.
    if (input == Py_True)
    {
	result = true;
	return true;
    }

    if (input == Py_False)
    {
	result = false;
	return true;
    }
#endif

    int int_result;
    if (SWIG_IsOK(SWIG_AsVal_int(input, &int_result)))
    {
        result = int_result;
	return true;
    }

    double double_result;
    if (SWIG_IsOK(SWIG_AsVal_double(input, &double_result)))
    {
        result = double_result;
	return true;
    }

    std::string string_result;
    if (SWIG_IsOK(SWIG_AsVal_std_string(input, &string_result)))
    {
        result = string_result;
	return true;
    }

    void *vector2_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &vector2_result,
				  SWIGTYPE_p_HOM_Vector2, 0)) && vector2_result)
    {
	result = reinterpret_cast<HOM_Vector2 *>(vector2_result);
	return true;
    }

    void *vector3_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &vector3_result,
				  SWIGTYPE_p_HOM_Vector3, 0)) && vector3_result)
    {
	result = reinterpret_cast<HOM_Vector3 *>(vector3_result);
	return true;
    }

    void *vector4_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &vector4_result,
				  SWIGTYPE_p_HOM_Vector4, 0)) && vector4_result)
    {
	result = reinterpret_cast<HOM_Vector4 *>(vector4_result);
	return true;
    }

    void *quat_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &quat_result,
				  SWIGTYPE_p_HOM_Quaternion, 0)) && quat_result)
    {
	result = reinterpret_cast<HOM_Quaternion *>(quat_result);
	return true;
    }

    void *matrix2_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &matrix2_result,
				  SWIGTYPE_p_HOM_Matrix2, 0)) && matrix2_result)
    {
	result = reinterpret_cast<HOM_Matrix2 *>(matrix2_result);
	return true;
    }

    void *matrix3_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &matrix3_result,
				  SWIGTYPE_p_HOM_Matrix3, 0)) && matrix3_result)
    {
	result = reinterpret_cast<HOM_Matrix3 *>(matrix3_result);
	return true;
    }

    void *matrix4_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &matrix4_result,
				  SWIGTYPE_p_HOM_Matrix4, 0)) && matrix4_result)
    {
	result = reinterpret_cast<HOM_Matrix4 *>(matrix4_result);
	return true;
    }

    void *geometry_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &geometry_result,
				  SWIGTYPE_p_HOM_Geometry, 0)) && geometry_result)
    {
	result = reinterpret_cast<HOM_Geometry *>(geometry_result);
	return true;
    }

    void *ramp_result = NULL;
    if (SWIG_IsOK(SWIG_ConvertPtr(input, &ramp_result,
				  SWIGTYPE_p_HOM_Ramp, 0)) && ramp_result)
    {
	result = reinterpret_cast<HOM_Ramp *>(ramp_result);
	return true;
    }

    std::vector<int> *int_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<int> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &int_vector_ptr)))
    {
        result = *int_vector_ptr;
        delete int_vector_ptr;
	return true;
    }

    std::vector<double> *double_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<double> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &double_vector_ptr)))
    {
        result = *double_vector_ptr;
        delete double_vector_ptr;
	return true;
    }

    std::vector<std::string> *string_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<std::string> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &string_vector_ptr)))
    {
        result = *string_vector_ptr;
        delete string_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector2> *vector2_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector2> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector2_vector_ptr)))
    {
	result = *vector2_vector_ptr;
	delete vector2_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector3> *vector3_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector3> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector3_vector_ptr)))
    {
	result = *vector3_vector_ptr;
	delete vector3_vector_ptr;
	return true;
    }

    std::vector<HOM_Vector4> *vector4_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Vector4> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &vector4_vector_ptr)))
    {
	result = *vector4_vector_ptr;
	delete vector4_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix2> *matrix2_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix2> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix2_vector_ptr)))
    {
	result = *matrix2_vector_ptr;
	delete matrix2_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix3> *matrix3_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix3> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix3_vector_ptr)))
    {
	result = *matrix3_vector_ptr;
	delete matrix3_vector_ptr;
	return true;
    }

    std::vector<HOM_Matrix4> *matrix4_vector_ptr = NULL;
    if (SWIG_IsOK(swig::asptr(input, (std::vector<HOM_Matrix4> **)NULL)) &&
	SWIG_IsOK(swig::asptr(input, &matrix4_vector_ptr)))
    {
	result = *matrix4_vector_ptr;
	delete matrix4_vector_ptr;
	return true;
    }

    UT_ASSERT(result.empty());
    return false;
}

// Creating an input typemap isn't sufficient to handle input parameters with
// std::vector's or std::map's of hboost::any's, so we need to specialize
// swig::traits_asptr<hboost::any>.
namespace swig {
    template <>
    struct traits_asptr<hboost::any>
    {
	static int asptr(InterpreterObject obj, hboost::any **val)
	{
	    // TODO: Will the hboost::any be properly deleted?
	    hboost::any *p = new hboost::any();
	    HOMinterpreterObjectToBoostAny(obj, *p);
	    if (val)
		*val = p;
	    return SWIG_NEWOBJ;
	}
    };
}

// This function is not instantiated until the first use of HOM_BinaryString,
// but we need to use it in HOMboostAnyToInterpreterObject. So just forward
// declare it here.
static PyObject * SWIG_From_HOM_BinaryString(const HOM_BinaryString& s);

// This helper function takes a variable wrapped inside a hboost::any object
// and creates a new instance of the corresponding InterpreterObject.
static InterpreterObject
HOMboostAnyToInterpreterObject(const hboost::any &result)
{
    if (result.empty())
	return SWIG_Py_Void();

    if (result.type() == typeid(int))
	return SWIG_From_int(hboost::any_cast<int>(result));

    if (result.type() == typeid(long))
	return SWIG_From_long(hboost::any_cast<long>(result));

    if (result.type() == typeid(int64))
	return swig::from(hboost::any_cast<int64>(result));

    if (result.type() == typeid(float))
	return SWIG_From_float(hboost::any_cast<float>(result));

    if (result.type() == typeid(double))
	return SWIG_From_double(hboost::any_cast<double>(result));

    if (result.type() == typeid(std::string))
	return SWIG_From_std_string(hboost::any_cast<std::string>(result));

    if (result.type() == typeid(HOM_BinaryString))
	return SWIG_From_HOM_BinaryString(hboost::any_cast<HOM_BinaryString>(result));

    if (result.type() == typeid(std::vector<int>))
	return swig::from(hboost::any_cast<std::vector<int> >(result));

    if (result.type() == typeid(std::vector<int64>))
	return swig::from(hboost::any_cast<std::vector<int64> >(result));

    if (result.type() == typeid(std::vector<float>))
	return swig::from(hboost::any_cast<std::vector<float> >(result));

    if (result.type() == typeid(std::vector<std::string>))
	return swig::from(hboost::any_cast<std::vector<std::string> >(result));

    if (result.type() == typeid(std::vector<double>))
	return swig::from(hboost::any_cast<std::vector<double> >(result));

    if (result.type() == typeid(std::vector<std::vector<float> >))
	return swig::from(
	    hboost::any_cast<std::vector<std::vector<float> > >(result));

    if (result.type() == typeid(std::vector<std::vector<double> >))
	return swig::from(
	    hboost::any_cast<std::vector<std::vector<double> > >(result));

    // If a HOM function returns a pointer to a HOM_Vector*/HOM_Matrix*, it
    // will transfer the ownership of that object to swig.
    if (result.type() == typeid(HOM_Vector2 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Vector2 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Vector3 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Vector3 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Vector4 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Vector4 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Matrix2 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Matrix2 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Matrix3 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Matrix3 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Matrix4 *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Matrix4 *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_NodeType *))
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_NodeType *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Ramp *))
	return HOMconvertValueForInterpreter(
	    hboost::any_cast<HOM_Ramp *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(HOM_Geometry *))
	return HOMconvertValueForInterpreter(
	    hboost::any_cast<HOM_Geometry *>(result), SWIG_POINTER_OWN);

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Vector2> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Vector2> > >(result));

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Vector3> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Vector3> > >(result));

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Vector4> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Vector4> > >(result));

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Matrix2> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Matrix2> > >(result));

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Matrix3> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Matrix3> > >(result));

    if (result.type() == typeid(std::vector<HOM_ElemPtr<HOM_Matrix4> >))
	return swig::from(
	    hboost::any_cast<std::vector<HOM_ElemPtr<HOM_Matrix4> > >(result));

#if UT_ASSERT_LEVEL > 0
    std::cout << "Unknown data type: "
	 << UTunmangleClassNameFromTypeIdName(result.type().name()) << "\n";
#endif
    UT_ASSERT(!"Unknown data type");
    return SWIG_Py_Void();
}

// This helper function takes a variable wrapped inside a hboost::any object
// and creates a new instance of the corresponding InterpreterObject.
static InterpreterObject
HOMDDSourceAnyToInterpreterObject(const HOM_DDSourceAny &result)
{
    if (result.type() == typeid(HOM_Node *))
    {
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Node *>(result), SWIG_POINTER_OWN);
    }
    if (result.type() == typeid(HOM_Parm *))
    {
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_Parm *>(result), SWIG_POINTER_OWN);
    }
    if (result.type() == typeid(HOM_GalleryEntry *))
    {
	return HOMconvertValueForInterpreter(
		hboost::any_cast<HOM_GalleryEntry *>(result), SWIG_POINTER_OWN);
    }

    return HOMboostAnyToInterpreterObject(result);
}

namespace swig {
    // Adding the typemap does not customize swig::from() so we do that
    // manually.
    template <>
    struct traits_from<hboost::any>
    {
        static PyObject *from(const hboost::any &val)
        { return HOMboostAnyToInterpreterObject(val); }
    };
}

static InterpreterObject
HOMoptionEntryToInterpreterObject(const UT_OptionEntry &option_entry)
{
    InterpreterObject result = NULL;

    switch (option_entry.getType())
    {
	case UT_OPTION_INT:
	    return SWIG_From_int(
		((const UT_OptionInt &)option_entry).getValue());

	case UT_OPTION_BOOL:
	    return SWIG_From_bool(
		((const UT_OptionBool &)option_entry).getValue());

	case UT_OPTION_FPREAL:
	    return SWIG_From_double(
		((const UT_OptionFpreal &)option_entry).getValue());

	case UT_OPTION_STRING:
	case UT_OPTION_STRINGRAW:
	{
	    return SWIG_From_std_string(
		 ((const UT_OptionString &)option_entry).getValue().toStdString());
	}

	case UT_OPTION_VECTOR2:
	    return HOMconvertValueForInterpreter(
		 new HOM_Vector2(
		     ((const UT_OptionVector2 &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_VECTOR3:
	    return HOMconvertValueForInterpreter(
		 new HOM_Vector3(
		    ((const UT_OptionVector3 &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_VECTOR4:
	    return HOMconvertValueForInterpreter(
		 new HOM_Vector4(
		    ((const UT_OptionVector4 &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_QUATERNION:
	    return HOMconvertValueForInterpreter(
		 new HOM_Quaternion(
		     ((const UT_OptionQuaternion &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_MATRIX2:
	    return HOMconvertValueForInterpreter(
		 new HOM_Matrix2(UT_DMatrix2(
		    ((const UT_OptionMatrix2 &)option_entry).getValue())),
		 SWIG_POINTER_OWN);

	case UT_OPTION_MATRIX3:
	    return HOMconvertValueForInterpreter(
		 new HOM_Matrix3(UT_DMatrix3(
		    ((const UT_OptionMatrix3 &)option_entry).getValue())),
		 SWIG_POINTER_OWN);

	case UT_OPTION_MATRIX4:
	    return HOMconvertValueForInterpreter(
		 new HOM_Matrix4(UT_DMatrix4(
		    ((const UT_OptionMatrix4 &)option_entry).getValue())),
		 SWIG_POINTER_OWN);

	case UT_OPTION_UV:
	    return HOMconvertValueForInterpreter(
		 new HOM_Vector2(
		    ((const UT_OptionUV &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_UVW:
	    return HOMconvertValueForInterpreter(
		 new HOM_Vector3(
		     ((const UT_OptionUVW &)option_entry).getValue()),
		 SWIG_POINTER_OWN);

	case UT_OPTION_INTARRAY:
	    {
		std::vector<int64> int_vector;
		UTarrayToStdVector(
		    static_cast<const UT_OptionInt64Array &>(option_entry)
			.getValue(), int_vector);
		return swig::from(int_vector);
	    }

	case UT_OPTION_FPREALARRAY:
	    {
		std::vector<double> double_vector;
		UTarrayToStdVector(
		    static_cast<const UT_OptionFpreal64Array &>(option_entry)
			.getValue(), double_vector);
		return swig::from(double_vector);
	    }

	case UT_OPTION_STRINGARRAY:
	    {
		std::vector<std::string> str_vector;
		UTarrayToStdVectorOfStrings(
		    static_cast<const UT_OptionStringArray &>(option_entry)
			.getValue(), str_vector);
		return swig::from(str_vector);
	    }

	case UT_OPTION_INVALID:
	case UT_OPTION_NUM_TYPES:
	    // Just exit the switch with no result.
	    break;
    }

    return result ? result : SWIG_Py_Void();
}

#ifdef SWIGPYTHON
static InterpreterObject
HOMoptionsToInterpreterObject(const UT_Options &options)
{
    InterpreterObject result = PyDict_New();

    for (UT_Options::iterator it = options.begin(); !it.atEnd(); ++it)
    {
	// The Python dictionary object will increment the reference count
	// on the value, so we decrement it since we just allocated it.
	InterpreterObject value=HOMoptionEntryToInterpreterObject(*it.entry());
	PyDict_SetItemString(result, it.name(), value);
        Py_DECREF(value);
    }

    return result;
}
#endif

//----------------------------------------------------------------------------

#ifdef SWIGPYTHON
// This helper class takes a Python buffer object and provides access to
// the underlying C buffer.  Note that the class is only used inside the
// swig file, so it doesn't need to be exported from the current library.
class HOM_PyBuffer
{
public:
    HOM_PyBuffer(InterpreterObject py_object)
    {
	myData = NULL;
	myLength = 0;

	// Python added a new buffer interface starting with Python 2.6.
#if PY_VERSION_HEX >= 0x02060000
	myUseNewAPI = false;
	if (PyObject_CheckBuffer(py_object))
	{
	    if (PyObject_GetBuffer(py_object, &myPyBuffer, PyBUF_SIMPLE) < 0)
		throw HOM_TypeError("failed to get readable buffer");

	    myUseNewAPI = true;
	    myData = myPyBuffer.buf;
	    myLength = myPyBuffer.len;
	    return;
	}
#endif
	// Either the new API isn't supported in this Python version or the
	// Python object doesn't support it.  Try the old API.
	if (!PyObject_CheckReadBuffer(py_object))
	    throw HOM_TypeError("expected a readable buffer");

	if (PyObject_AsReadBuffer(py_object, &myData, &myLength) < 0)
	    throw HOM_TypeError("failed to get readable buffer");
    }

    ~HOM_PyBuffer()
    {
#if PY_VERSION_HEX >= 0x02060000
	if (myUseNewAPI)
	    PyBuffer_Release(&myPyBuffer);
#endif
    }

#if PY_VERSION_HEX >= 0x02060000
    bool myUseNewAPI;
    Py_buffer myPyBuffer;
#endif
    const void *myData;
    Py_ssize_t myLength;
};
#endif

//----------------------------------------------------------------------------

// These helper functions are used to implement the attrib() methods of
// Points, Prims, and Vertices.  We need to do extra work for these methods
// because the return type can vary.
template <typename T>
InterpreterObject
HOMattribValue(T &geo_element, HOM_Attrib &hom_attrib)
{
    InterpreterObject result = NULL;
    switch (hom_attrib.dataType().id())
    {
    case HOM_attribData::Int_Id:
	if (hom_attrib.size() == 1)
	    result = swig::from(
		hboost::any_cast<int64>(geo_element.intAttribValue(hom_attrib)));
	else
	    result = swig::from(
		hboost::any_cast<std::vector<int64> >(
		    geo_element.intListAttribValue(hom_attrib)));
	break;

    case HOM_attribData::Float_Id:
	if (hom_attrib.size() == 1)
	    result = SWIG_From_double(geo_element.floatAttribValue(hom_attrib));
	else
	    result = swig::from(geo_element.floatListAttribValue(hom_attrib));
	break;

    case HOM_attribData::String_Id:
	if (hom_attrib.size() == 1)
	    result = SWIG_From_std_string(
		geo_element.stringAttribValue(hom_attrib));
	else
	    result = swig::from(geo_element.stringListAttribValue(hom_attrib));
	break;
    }
    UT_ASSERT(result);
    return result;
}


// Note that the caller is responsible for deleting the HOM_Attrib object
// that's returned.
template <typename T> HOM_Attrib*
HOMlookUpAttrib(T &geo_element, const char *name);

template <> HOM_Attrib*
HOMlookUpAttrib(HOM_Point &point, const char *name)
{ return HOMdel(point.geometry())->findPointAttrib(name); }

template <> HOM_Attrib*
HOMlookUpAttrib(HOM_Prim &prim, const char *name)
{ return HOMdel(prim.geometry())->findPrimAttrib(name); }

template <> HOM_Attrib*
HOMlookUpAttrib(HOM_Vertex &vertex, const char *name)
{ return HOMdel(vertex.geometry())->findVertexAttrib(name); }

template <> HOM_Attrib*
HOMlookUpAttrib(HOM_Geometry &geometry, const char *name)
{ return geometry.findGlobalAttrib(name); }

template <typename T>
InterpreterObject
HOMattribValue(T &geo_element, const char *name)
{
    HOM_Attrib *hom_attrib = HOMlookUpAttrib(geo_element, name);
    if (!hom_attrib)
	throw HOM_OperationFailed();

    InterpreterObject result = HOMattribValue(geo_element, *hom_attrib);
    delete hom_attrib;
    return result;
}
%}

//----------------------------------------------------------------------------

%{
// These helper functions are used to implement parm evaluation.
static InterpreterObject
HOMevalParm(HOM_Parm &parm)
{
    PY_InterpreterAutoLock py_lock;

    switch (parm.parmDataTypeEnumId())
    {
    case HOM_parmData::Int_Id:
	return SWIG_From_int(parm.evalAsInt());
    case HOM_parmData::Float_Id:
	return SWIG_From_double(parm.evalAsFloat());
    case HOM_parmData::String_Id:
	return SWIG_From_std_string(parm.evalAsString());
    case HOM_parmData::Ramp_Id:
	return SWIG_NewPointerObj(
	    (void*)parm.evalAsRamp(), SWIGTYPE_p_HOM_Ramp, SWIG_POINTER_OWN);
    case HOM_parmData::Data_Id:
	if (parm.dataParmTypeEnumId() == HOM_dataParmType::KeyValueDictionary_Id)
	    return swig::from(parm.evalAsJSONMap());

	return SWIG_NewPointerObj(
	    (void*)parm.evalAsGeometry(), SWIGTYPE_p_HOM_Geometry, SWIG_POINTER_OWN);
    }

    UT_ASSERT(!"Unknown parm data type");
    return SWIG_Py_Void();
}

static InterpreterObject
HOMevalParmAtFrame(HOM_Parm &parm, double frame)
{
    PY_InterpreterAutoLock py_lock;

    switch (parm.parmDataTypeEnumId())
    {
    case HOM_parmData::Int_Id:
	return SWIG_From_int(parm.evalAsIntAtFrame(frame));
    case HOM_parmData::Float_Id:
	return SWIG_From_double(parm.evalAsFloatAtFrame(frame));
    case HOM_parmData::String_Id:
	return SWIG_From_std_string(parm.evalAsStringAtFrame(frame));
    case HOM_parmData::Ramp_Id:
        return SWIG_NewPointerObj(
	    (void*)parm.evalAsRampAtFrame(frame),
	    SWIGTYPE_p_HOM_Ramp, SWIG_POINTER_OWN);
    case HOM_parmData::Data_Id:
	if (parm.dataParmTypeEnumId() == HOM_dataParmType::KeyValueDictionary_Id)
	    return swig::from(parm.evalAsJSONMapAtFrame(frame));

        return SWIG_NewPointerObj(
	    (void*)parm.evalAsGeometryAtFrame(frame),
	    SWIGTYPE_p_HOM_Geometry, SWIG_POINTER_OWN);
    }

    UT_ASSERT(!"Unknown parm data type");
    return SWIG_Py_Void();
}

static InterpreterObject
HOMevalParmTuple(HOM_ParmTuple &parm_tuple)
{
    switch (parm_tuple.parmDataTypeEnumId())
    {
    case HOM_parmData::Int_Id:
	return swig::from(parm_tuple.evalAsInts());
    case HOM_parmData::Float_Id:
	return swig::from(parm_tuple.evalAsFloats());
    case HOM_parmData::String_Id:
	return swig::from(parm_tuple.evalAsStrings());
    case HOM_parmData::Ramp_Id:
	return swig::from(parm_tuple.evalAsRamps());
    case HOM_parmData::Data_Id:
	return swig::from(parm_tuple.evalAsGeometries());
    }

    UT_ASSERT(!"Unknown parm data type");
    return SWIG_Py_Void();
}

static InterpreterObject
HOMevalParmTupleAtFrame(HOM_ParmTuple &parm_tuple, double frame)
{
    switch (parm_tuple.parmDataTypeEnumId())
    {
    case HOM_parmData::Int_Id:
	return swig::from(parm_tuple.evalAsIntsAtFrame(frame));
    case HOM_parmData::Float_Id:
	return swig::from(parm_tuple.evalAsFloatsAtFrame(frame));
    case HOM_parmData::String_Id:
	return swig::from(parm_tuple.evalAsStringsAtFrame(frame));
    case HOM_parmData::Ramp_Id:
	return swig::from(parm_tuple.evalAsRampsAtFrame(frame));
    case HOM_parmData::Data_Id:
	return swig::from(parm_tuple.evalAsGeometriesAtFrame(frame));
    }

    UT_ASSERT(!"Unknown parm data type");
    return SWIG_Py_Void();
}

static InterpreterObject
HOMevalViewportVisualizerParm(HOM_ViewportVisualizer &visualizer, const char *parm_name)
{
    PY_InterpreterAutoLock py_lock;

    switch (visualizer.parmDataTypeEnumId(parm_name))
    {
    case HOM_parmData::Int_Id:
	return SWIG_From_int(visualizer.evalParmAsInt(parm_name));
    case HOM_parmData::Float_Id:
	return SWIG_From_double(visualizer.evalParmAsFloat(parm_name));
    case HOM_parmData::String_Id:
	return SWIG_From_std_string(visualizer.evalParmAsString(parm_name));
    case HOM_parmData::Ramp_Id:
	return SWIG_NewPointerObj(
	    (void*)visualizer.evalParmAsRamp(parm_name), SWIGTYPE_p_HOM_Ramp, 
	    SWIG_POINTER_OWN);
    }

    UT_ASSERT(!"Unknown parm data type");
    return SWIG_Py_Void();
}


//----------------------------------------------------------------------------

%}
#endif

#endif
