/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ParmTemplateGroup_h__
#define __HOM_ParmTemplateGroup_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Module.h"
#include <vector>

class HOM_ParmTemplate;

SWIGOUT(%feature("notabstract") HOM_ParmTemplateGroup;)
SWIGOUT(%rename(ParmTemplateGroup) HOM_ParmTemplateGroup;)

class HOM_API HOM_ParmTemplateGroup
{
public:
#ifdef SWIG
%extend {
    HOM_ParmTemplateGroup(
            const std::vector<HOM_ParmTemplate *> &parm_templates =
                std::vector<HOM_ParmTemplate *>())
    {
	return HOM().newParmTemplateGroup(parm_templates);
    }
};
#else
    HOM_ParmTemplateGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ParmTemplateGroup(const HOM_ParmTemplateGroup &face)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ParmTemplateGroup()
    { HOM_DESTRUCT_OBJECT(this) }
#endif

    virtual std::string __repr__() = 0;

    virtual bool operator==(
	    HOM_PtrOrNull<HOM_ParmTemplateGroup> parm_template_group) = 0;
    virtual bool operator!=(
	    HOM_PtrOrNull<HOM_ParmTemplateGroup> parm_template_group) = 0;

    SWIGOUT(%newobject find;)
    virtual HOM_ParmTemplate *find(const char *name) = 0;

    virtual std::vector<int> findIndices(const char *name) = 0;

    virtual std::vector<int> findIndices(HOM_ParmTemplate &parm_template) = 0;

    SWIGOUT(%newobject findFolder;)
    virtual HOM_ParmTemplate *findFolder(const char *label) = 0;

    virtual HOM_ParmTemplate *findFolder(
	    const std::vector<std::string> &labels) = 0;

    virtual std::vector<int> findIndicesForFolder(const char *label) = 0;

    virtual std::vector<int> findIndicesForFolder(
	    const std::vector<std::string> &labels) = 0;

    SWIGOUT(%newobject entryAtIndices;)
    virtual HOM_ParmTemplate *entryAtIndices(const std::vector<int> &indices) = 0;

    virtual std::vector<int> containingFolderIndices(const char *name) = 0;

    virtual std::vector<int> containingFolderIndices(
	    HOM_ParmTemplate &parm_template) = 0;

    virtual std::vector<int> containingFolderIndices(
            const std::vector<int> &indices) = 0;

    SWIGOUT(%newobject containingFolder;)
    virtual HOM_ParmTemplate *containingFolder(const char *name) = 0;

    SWIGOUT(%newobject containingFolder;)
    virtual HOM_ParmTemplate *containingFolder(HOM_ParmTemplate &parm_template) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTemplate> > entries() = 0;

    // This method is an alias for entries that makes it easier to search
    // down through a parm template group into items nested in folders.
    std::vector<HOM_ElemPtr<HOM_ParmTemplate> > parmTemplates()
    { return entries(); }

    virtual std::vector<HOM_ElemPtr<HOM_ParmTemplate> > entriesWithoutFolders() = 0;

    virtual void replace(
	    const char *name, HOM_ParmTemplate &parm_template) = 0;

    virtual void replace(
	    HOM_ParmTemplate &parm_template_to_replace,
	    HOM_ParmTemplate &parm_template) = 0;

    virtual void replace(
	    const std::vector<int> &indices, HOM_ParmTemplate &parm_template) = 0;

    virtual void insertBefore(
	    const char *name, HOM_ParmTemplate &parm_template) = 0;

    virtual void insertBefore(
	    const std::vector<int> &indices, HOM_ParmTemplate &parm_template) = 0;

    virtual void insertBefore(
	    HOM_ParmTemplate &before_parm_template,
	    HOM_ParmTemplate &parm_template) = 0;

    virtual void insertAfter(
	    const char *name, HOM_ParmTemplate &parm_template) = 0;

    virtual void insertAfter(
	    const std::vector<int> &indices, HOM_ParmTemplate &parm_template) = 0;

    virtual void insertAfter(
	    HOM_ParmTemplate &after_parm_template,
	    HOM_ParmTemplate &parm_template) = 0;

    virtual void append(HOM_ParmTemplate &parm_template) = 0;

    virtual void appendToFolder(
	    const std::vector<int> &indices, HOM_ParmTemplate &parm_template) = 0;

    virtual void appendToFolder(
	    const std::vector<std::string> &labels,
	    HOM_ParmTemplate &parm_template) = 0;

    virtual void appendToFolder(
	    const char *label, HOM_ParmTemplate &parm_template) = 0;

    virtual void appendToFolder(
	    HOM_ParmTemplate &folder_parm_template,
	    HOM_ParmTemplate &parm_template) = 0;

    // This method is an alias for append that makes it easier to treat
    // ParmTemplateGroups and folders uniformly.
    void addParmTemplate(HOM_ParmTemplate &parm_template)
    { append(parm_template); }

    virtual void remove(const char *name) = 0;

    virtual void remove(HOM_ParmTemplate &parm_template) = 0;

    virtual void remove(const std::vector<int> &indices) = 0;

    virtual void hide(const std::vector<int> &indices, bool on) = 0;

    virtual void hide(const char *name, bool on) = 0;

    virtual void hide(HOM_ParmTemplate &parm_template, bool on) = 0;

    virtual void hideFolder(const char *label, bool on) = 0;

    virtual void hideFolder(const std::vector<std::string> &labels, bool on) = 0;

    virtual bool isHidden(const std::vector<int> &indices) = 0;

    virtual bool isHidden(const char *name) = 0;

    virtual bool isHidden(HOM_ParmTemplate &parm_template) = 0;

    virtual bool isFolderHidden(const char *label) = 0;

    virtual bool isFolderHidden(const std::vector<std::string> &labels) = 0;

    virtual void clear() = 0;

    SWIGOUT(%kwargs asDialogScript;)
    virtual std::string asDialogScript(bool rename_conflicting_parms=false,
				       bool full_info=false) = 0;

    virtual void setToDialogScript(const char *dialog_script) = 0;

    virtual std::string asCode(
	    const char *function_name=NULL, const char *variable_name=NULL) = 0;

    SWIGOUT(%newobject sourceNode;)
    virtual HOM_Node* sourceNode() = 0;
    SWIGOUT(%newobject sourceNodeType;)
    virtual HOM_NodeType* sourceNodeType() = 0;
};

#endif
