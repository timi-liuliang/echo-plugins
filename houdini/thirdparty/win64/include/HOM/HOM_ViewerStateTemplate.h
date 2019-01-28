/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_ViewerStateTemplate_h
#define HOM_ViewerStateTemplate_h

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_Defines.h"
#include "HOM_ViewerStateMenu.h"
#include "HOM_NodeTypeCategory.h"

#include <string>
#include <vector>
#include <utility>

SWIGOUT(%rename(ViewerStateTemplate) HOM_ViewerStateTemplate;)
SWIGOUT(%feature("notabstract") HOM_ViewerStateTemplate;)

class HOM_API HOM_ViewerStateTemplate
{
public:
    typedef std::vector< std::pair<std::string, std::string> > Bindings;
#ifdef SWIG
%extend
{   
    HOM_ViewerStateTemplate(
	std::string const &		type_name, 
	std::string const &		label,
	HOM_NodeTypeCategory const &    category)
    { 
	return HOM().newViewerStateTemplate(type_name, label, category); 
    }
}
#else
    HOM_ViewerStateTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ViewerStateTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::string typeName() const = 0;

    virtual std::string categoryName() const = 0;

    SWIGOUT(%ignore bindFactory_internal;)
    virtual void bindFactory_internal(void *callback) = 0;

    SWIGOUT(%kwargs bindHandle;)
    virtual void bindHandle(
	std::string const&  handle_type, 
	std::string const&  name, 
	std::string const&  settings=std::string(),
	bool		    cache_previous_parms=false) = 0;

    SWIGOUT(%kwargs bindHandleStatic;)
    virtual void bindHandleStatic(
	std::string const&  handle_type, 
	std::string const&  name, 
	Bindings const &    bindings,
	std::string const&  settings=std::string() ) = 0;

    SWIGOUT(%kwargs bindGeometrySelector;)
    virtual void bindGeometrySelector(
	std::string const&		    prompt,
	bool				    allow_drag=true,
	bool				    quick_select=true,
	bool				    auto_start=true,
	bool				    use_existing_selection=true,
	std::string const&		    initial_selection=std::string(),
	HOM_EnumValue *			    initial_selection_type=nullptr,
	bool				    ordered=false,
	std::vector<HOM_EnumValue *> const& geometry_types=std::vector<HOM_EnumValue *>(),
	std::vector<HOM_EnumValue *> const& primitive_types=std::vector<HOM_EnumValue *>(),
	bool				    allow_other_sops=true,
	std::string const&		    hotkey=std::string()) = 0;

    SWIGOUT(%kwargs bindSceneGraphSelector;)
    virtual void bindSceneGraphSelector(
	std::string const&		prompt,
	bool				allow_drag = true,
	bool				quick_select = true,
	bool				auto_start=true,
	bool				use_existing_selection = true,
	bool				allow_multisel = true,
	std::vector<std::string> const&	prior_selection_paths = std::vector<std::string>(),
	HOM_EnumValue const&		prim_mask= HOM_scenePrimMask::ViewerSetting,
	std::string const&		path_prefix_mask = std::string(),
	std::string const&		prim_kind = std::string(),
	std::string const&		hotkey=std::string()) = 0;

    SWIGOUT(%kwargs bindSelector;)
    virtual void bindSelector(
	std::string const& name,
	std::string const& selector_type,
	std::string const& prompt,
	std::vector<HOM_EnumValue *> primitive_types = std::vector<HOM_EnumValue *>(),
	std::string const& group_parm_name = std::string(),
	int input_index = 0,
	bool input_required = true,
	bool allow_dragging = true ) = 0;

    virtual void bindMenu( HOM_ViewerStateMenu const & menu ) = 0;

 #ifdef SWIG
%extend
{
    virtual void bindFactory(InterpreterObject callback)
    {
	self->bindFactory_internal(callback);
    }
}
#endif // SWIG
};

#endif // HOM_ViewerStateTemplate_h

