/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_hipFile_h__
#define __HOM_hipFile_h__

#include "HOM_API.h"
#include "HOM_Color.h"
#include "HOM_Defines.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include <utility>
#include <vector>

SWIGOUT(%rename(hipFile) HOM_hipFile;)
class HOM_API HOM_hipFile
{
public:
    virtual ~HOM_hipFile()
    {}

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs save;)
    virtual void save(
	    const char *file_name=NULL, bool save_to_recent_files=true) = 0;

    virtual void saveAndIncrementFileName() = 0;

    virtual void saveAsBackup() = 0;

    virtual std::string basename() = 0;

    virtual std::string name() = 0;

    virtual std::string path() = 0;

    virtual void setName(const char *file_name) = 0;

    virtual HOM_EnumValue &saveMode() = 0;
    virtual void setSaveMode(HOM_EnumValue &savemode) = 0;

    SWIGOUT(%kwargs clear;)
    virtual void clear(bool suppress_save_prompt=false) = 0;

    SWIGOUT(%kwargs load;)
    virtual void load(const char *file_name, 
	    bool suppress_save_prompt=false,
	    bool ignore_load_warnings=false) = 0;

    SWIGOUT(%kwargs merge;)
    virtual void merge(const char *file_name, 
	    const char *node_pattern="*",
	    bool overwrite_on_conflict=false,
	    bool ignore_load_warnings=false) = 0;

    SWIGOUT(%kwargs collisionNodesIfMerged;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > collisionNodesIfMerged(
	    const char *file_name, const char *node_pattern="*") = 0;

    virtual bool isLoadingHipFile() = 0;

    virtual bool isShuttingDown() = 0;
    
    virtual bool hasUnsavedChanges() = 0;


    virtual std::map<std::string, HOM_Color> groupColorTable() = 0;
    virtual void setGroupColorTable(
	const std::map<std::string, HOM_Color> &color_table) = 0;

    SWIGOUT(%kwargs importFBX;)
    virtual std::pair<HOM_ElemPtr<HOM_Node>, std::string>
	importFBX(
	    const char *file_name, 
	    bool suppress_save_prompt=false,
	    bool merge_into_scene=true,
	    bool import_cameras=true,
	    bool import_joints_and_skin=true,
	    bool import_geometry=true,
	    bool import_lights=true,
	    bool import_animation=true,
	    bool import_materials=true,
	    bool resample_animation=false,
	    double resample_interval=1.0,
	    bool override_framerate=false,
	    int framerate=-1,
	    bool hide_joints_attached_to_skin=true,
	    bool convert_joints_to_zyx_rotation_order=false,
	    HOM_EnumValue &material_mode=HOM_fbxMaterialMode::FBXShaderNodes,
	    HOM_EnumValue &compatibility_mode=HOM_fbxCompatibilityMode::Maya,
	    bool single_precision_vertex_caches=false,
	    bool triangulate_nurbs=false,
	    bool triangulate_patches=false,
	    bool import_global_ambient_light=false,
	    bool import_blend_deformers_as_blend_sops=false,
	    bool segment_scale_already_baked_in=true,
	    bool convert_file_paths_to_relative=true,
	    bool unlock_geometry=false,
	    bool unlock_deformations=false,
	    bool import_nulls_as_subnets=false,
	    bool import_into_object_subnet=true,
	    bool convert_into_y_up_coordinate_system=false) = 0;

    SWIGOUT(%ignore addOpaqueEventCallback;)
    virtual void addOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventCallback;)
    virtual void removeOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore opaqueEventCallbacks;)
    virtual std::vector<void *> opaqueEventCallbacks() = 0;
    SWIGOUT(%ignore clearOpaqueEventCallbacks;)
    virtual void clearOpaqueEventCallbacks() = 0;

#ifdef SWIG
%extend
{
    void addEventCallback(InterpreterObject callback)
    { self->addOpaqueEventCallback(callback); }

    void removeEventCallback(InterpreterObject callback)
    { self->removeOpaqueEventCallback(callback); }

    void clearEventCallbacks()
    { self->clearOpaqueEventCallbacks(); }

    // Note that we return a vector of borrowed references because swig
    // will increment the reference counts.
    std::vector<InterpreterObject> eventCallbacks()
    {
	std::vector<void *> opaque_callbacks(self->opaqueEventCallbacks());

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }
}
#endif

};

#endif
