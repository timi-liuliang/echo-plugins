/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLSectionNames.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_OTLSECTIONAMES_H_INCLUDED__
#define __OP_OTLSECTIONAMES_H_INCLUDED__

#include "OP_API.h"
#include <UT/UT_StringHolder.h>

#define OTL_DS_SECTION			"DialogScript"_UTsh
#define OTL_CREATESCRIPT_SECTION	"CreateScript"_UTsh
#define OTL_CONTENTS_SECTION		"Contents"_UTsh
#define OTL_CONTENTS_GZ_SECTION		"Contents.gz"_UTsh
#define OTL_CONTENTS_SC_SECTION		"Contents.sc"_UTsh
#define OTL_CONTENTS_MIME_SECTION	"Contents.mime"_UTsh
#define OTL_EDITABLE_SUBNODES_SECTION	"EditableNodes"_UTsh
#define OTL_MESSAGE_SUBNODES_SECTION	"MessageNodes"_UTsh
#define OTL_DIVETARGET_SECTION		"DiveTarget"_UTsh
#define OTL_DESCRIPTIVEPARMNAME_SECTION	"DescriptiveParmName"_UTsh
#define OTL_DEFAULTSTATE_SECTION	"DefaultState"_UTsh
#define OTL_SELECTORS_SECTION		"Selectors"_UTsh
#define OTL_SHELFTOOLS_SECTION		"Tools.shelf"_UTsh
#define OTL_VEX_SECTION			"VexCode"_UTsh
#define OTL_PYTHON_COOK_SECTION		"PythonCook"_UTsh
#define OTL_VFL_SECTION			"VflCode"_UTsh
#define OTL_ENCAPSULATED_VEX_SECTION	"EncVexCode"_UTsh
#define OTL_FUNCTION_NAME_SECTION	"FunctionName"_UTsh
#define OTL_HELP_SECTION		"Help"_UTsh
#define OTL_HELPURL_SECTION		"HelpUrl"_UTsh
#define OTL_ICON_SECTION		"Icon"_UTsh
#define OTL_ICONIMAGE_SECTION		"IconImage"_UTsh
#define OTL_ICONSVG_SECTION		"IconSVG"_UTsh
#define OTL_PRESETS_SECTION		"Presets"_UTsh
#define OTL_COMMENT_SECTION		"Comment"_UTsh
#define OTL_VERSION_SECTION		"Version"_UTsh
#define OTL_EXPRESSIONS_SECTION		"Expressions"_UTsh
#define OTL_PYTHONMODULE_SECTION	"PythonModule"_UTsh
#define OTL_PREFIRSTCREATE_SECTION	"PreFirstCreate"_UTsh
#define OTL_ONCREATED_SECTION		"OnCreated"_UTsh
#define OTL_ONLOADED_SECTION		"OnLoaded"_UTsh
#define OTL_ONUPDATED_SECTION		"OnUpdated"_UTsh
#define OTL_ONDELETED_SECTION		"OnDeleted"_UTsh
#define OTL_POSTLASTDELETE_SECTION	"PostLastDelete"_UTsh
#define OTL_ONINPUTCHANGED_SECTION	"OnInputChanged"_UTsh
#define OTL_ONNAMECHANGED_SECTION	"OnNameChanged"_UTsh
#define OTL_SYNCNODEVERSION_SECTION	"SyncNodeVersion"_UTsh
#define OTL_TYPEPROPOPTIONS_SECTION	"TypePropertiesOptions"_UTsh
#define OTL_EXTRAFILEOPTIONS_SECTION	"ExtraFileOptions"_UTsh
#define OTL_SDLAYOUT_SECTION		"SDLayout"_UTsh
#define OTL_CPLAYOUT_SECTION		"CPLayout"_UTsh
#define OTL_CPIMAGE_SECTION		"CPImage"_UTsh
#define OTL_GALLERY_SECTION		"Gallery"_UTsh
#define OTL_ONINSTALL_SECTION		"OnInstall"_UTsh
#define OTL_ONUNINSTALL_SECTION		"OnUninstall"_UTsh
// obsolete; use v150
// @{
#define OTL_GLSL_VERTEX_SECTION		"GlslVertex"_UTsh
#define OTL_GLSL_FRAGMENT_SECTION	"GlslFragment"_UTsh
// @}
#define OTL_GLSL150_VERTEX_SECTION	"Glsl150Vertex"_UTsh
#define OTL_GLSL150_GEOMETRY_SECTION	"Glsl150Geometry"_UTsh
#define OTL_GLSL150_FRAGMENT_SECTION	"Glsl150Fragment"_UTsh
#define OTL_SCRIPT_MAT_INFO_SECTION	"ScriptMaterialInfo"_UTsh
#define OTL_INTERNALFILEOPTIONS_SECTION	"InternalFileOptions"_UTsh
#define OTL_VOP_TYPE_DEFINITIONS	"VopTypeDefinitions"_UTsh
#define OTL_LIGHTBANK_SECTION		"LightBank"_UTsh

#define OTL_VEX_SECTION_SUFFIX		OTL_VEX_SECTION
#define OTL_VFL_SECTION_SUFFIX		OTL_VFL_SECTION
#define OTL_ENC_VEX_SECTION_SUFFIX	OTL_ENCAPSULATED_VEX_SECTION

#define OTL_MODULE_VEX_SECTION		"Module"_UTsh OTL_VEX_SECTION_SUFFIX
#define OTL_MODULE_VFL_SECTION		"Module"_UTsh OTL_VFL_SECTION_SUFFIX

// standard location when expanding/processing contents section
#define OTL_CONTENTS_DIRECTORY		"Contents.dir"_UTsh

#endif // __OP_OTLSECTIONAMES_H_INCLUDED__
