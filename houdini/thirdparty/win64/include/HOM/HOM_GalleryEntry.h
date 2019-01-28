/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GalleryEntry_h__
#define __HOM_GalleryEntry_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"

class HOM_Node;

SWIGOUT(%rename(GalleryEntry) HOM_GalleryEntry;)

class HOM_API HOM_GalleryEntry
{
public:
    HOM_GalleryEntry()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GalleryEntry(const HOM_GalleryEntry &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GalleryEntry()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool canApplyToNode(HOM_Node &node) = 0;
    virtual void applyToNode(HOM_Node &node) = 0;

    virtual bool canCreateChildNode(HOM_Node &parent) = 0;
    SWIGOUT(%newobject createChildNode;)
    virtual HOM_Node *createChildNode(HOM_Node &parent) = 0;

    virtual void setEqual(HOM_GalleryEntry &entry) = 0;

    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;

    virtual std::string label() = 0;
    virtual void setLabel(const char *label) = 0;

    virtual std::string description() = 0;
    virtual void setDescription(const char *description) = 0;

    virtual std::string helpURL() = 0;
    virtual void setHelpURL(const char *helpurl) = 0;

    virtual std::string requiredHDAFile() = 0;
    virtual void setRequiredHDAFile(const char *hda_file) = 0;

    virtual std::string icon() = 0;
    virtual void setIcon(const char *icon) = 0;

    virtual bool allowIconRegeneration() = 0;
    virtual void setAllowIconRegeneration(bool on) = 0;

    SWIGOUT(%newobject bestNodeType;)
    virtual HOM_NodeType *bestNodeType() = 0;

    virtual std::vector<std::string> nodeTypeNames() = 0;
    virtual void setNodeTypeNames(const std::vector<std::string> &nodetypes) = 0;

    virtual std::vector<std::string> categories() = 0;
    virtual void setCategories(const std::vector<std::string> &categories) = 0;

    virtual std::vector<std::string> keywords() = 0;
    virtual void setKeywords(const std::vector<std::string> &keywords) = 0;

    virtual HOM_NodeTypeCategory &nodeTypeCategory() = 0;
    virtual void setNodeTypeCategory(HOM_NodeTypeCategory &category) = 0;

    virtual std::string script() = 0;
    virtual void setScript(const char *script) = 0;
    virtual void setScriptFromNode(HOM_Node *node) = 0;
    virtual void setContentsFromNode(HOM_Node *node) = 0;
};

#endif

