/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Tool_h__
#define __HOM_Tool_h__

#include "HOM_API.h"
#include "HOM_EnumModules.h"
#include "HOM_ShelfElement.h"

class HOM_NodeTypeCategory;

SWIGOUT(%rename(Tool) HOM_Tool;)

class HOM_API HOM_Tool : virtual public HOM_ShelfElement
{
public:
    HOM_Tool()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Tool(const HOM_Tool &shelftool)
    : HOM_ShelfElement(shelftool)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Tool()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual int __hash__() = 0;

    virtual std::string script() = 0;
    virtual void setScript(const char *script) = 0;

    virtual HOM_EnumValue &language() = 0;
    virtual void setLanguage(HOM_EnumValue &language) = 0;

    virtual std::string icon() = 0;
    virtual void setIcon(const char *icon) = 0;

    virtual std::string help() = 0;
    virtual void setHelp(const char *help) = 0;

    virtual std::string helpURL() = 0;
    virtual void setHelpURL(const char *help_url) = 0;

    virtual std::vector<HOM_NodeTypeCategory *> toolMenuCategories(
				HOM_EnumValue &pane_type) = 0;
    virtual void setToolMenuCategories(HOM_EnumValue &pane_type,
	    const std::vector<HOM_NodeTypeCategory *> &categories) = 0;

    virtual std::string toolMenuOpType(HOM_EnumValue &pane_type) = 0;
    virtual void setToolMenuOpType(
	    HOM_EnumValue &pane_type, const char *op_type) = 0;

    virtual std::vector<std::string> toolMenuLocations() = 0;
    virtual void setToolLocations(const std::vector<std::string> &locations) = 0;

    virtual std::vector<std::string> keywords() = 0;
    virtual void setKeywords(const std::vector<std::string> &keywords) = 0;


    SWIGOUT(%kwargs setData;)
    virtual void setData(const char *script = NULL,
		 HOM_EnumValue &language = HOM_scriptLanguage::Python,
		 const char *icon = NULL,
		 const char *help = NULL,
		 const char *help_url = NULL,
		 const std::vector<HOM_NodeTypeCategory*> &network_categories =
		    std::vector<HOM_NodeTypeCategory *>(),
		 const std::vector<HOM_NodeTypeCategory*> &viewer_categories =
		    std::vector<HOM_NodeTypeCategory *>(),
		 const std::vector<HOM_NodeTypeCategory*> &cop_viewer_categories
		    = std::vector<HOM_NodeTypeCategory *>(),
		 const char * network_op_type = NULL,
		 const char * viewer_op_type = NULL,
		 const std::vector<std::string> &locations =
		    std::vector<std::string>(),
		 const std::vector<std::string> &keywords =
		    std::vector<std::string>()) = 0;

    virtual void destroy() = 0;
};

#endif
