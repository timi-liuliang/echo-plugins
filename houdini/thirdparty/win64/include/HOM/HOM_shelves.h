/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_shelves_h__
#define __HOM_shelves_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_EnumModules.h"
#include "HOM_ElemPtr.h"
#include <map>
#include <vector>

class HOM_NodeTypeCategory;
class HOM_ShelfSet;
class HOM_Shelf;
class HOM_Tool;

SWIGOUT(%rename(shelves) HOM_shelves;)

class HOM_API HOM_shelves
{
public:
    virtual ~HOM_shelves()
    {}

    virtual std::string __repr__() = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_ShelfSet> > shelfSets() = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_Shelf> > shelves() = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_Tool> > tools() = 0;

    SWIGOUT(%newobject tool;)
    virtual HOM_Tool *tool(const char *tool_name) = 0;

    virtual bool isToolDeleted(const char *tool_name) = 0;

    virtual void loadFile(const char *file_path) = 0;
    virtual void reloadShelfFiles() = 0;

    virtual HOM_Tool *runningTool() = 0;

    virtual void beginChangeBlock() = 0;
    virtual void endChangeBlock() = 0;

    SWIGOUT(%newobject newShelfSet;)
    SWIGOUT(%kwargs newShelfSet;)
    virtual HOM_ShelfSet *newShelfSet(const char *file_path = NULL,
				      const char *name = NULL,
				      const char *label = NULL) = 0;

    SWIGOUT(%newobject newShelf;)
    SWIGOUT(%kwargs newShelf;)
    virtual HOM_Shelf *newShelf(const char *file_path = NULL,
				const char *name = NULL,
				const char *label = NULL) = 0;

    SWIGOUT(%newobject newTool;)
    SWIGOUT(%kwargs newTool;)
    virtual HOM_Tool *newTool(const char *file_path = NULL,
		const char *name = NULL,
		const char *label = NULL,
		const char *script = NULL,
		HOM_EnumValue &language = HOM_scriptLanguage::Python,
		const char *icon = NULL,
		const char *help = NULL,
		const char *help_url = NULL,
		const std::vector<HOM_NodeTypeCategory *> &network_categories =
		    std::vector<HOM_NodeTypeCategory *>(),
		const std::vector<HOM_NodeTypeCategory*> &viewer_categories =
		    std::vector<HOM_NodeTypeCategory *>(),
		const std::vector<HOM_NodeTypeCategory*> &cop_viewer_categories
		    = std::vector<HOM_NodeTypeCategory *>(),
		const char *network_op_type = NULL,
		const char *viewer_op_type = NULL,
		const std::vector<std::string> &locations =
		    std::vector<std::string>(),
		const std::vector<std::string> &keywords =
		    std::vector<std::string>()) = 0;

    SWIGOUT(%newobject _newAssetTool;)
    SWIGOUT(%kwargs _newAssetTool;)
    virtual HOM_Tool *_newAssetTool(const char *name = NULL,
    		const char *label = NULL,
    		const char *icon = NULL,
    		const char *help = NULL,
    		const char *help_url = NULL) = 0;

    virtual std::string defaultFilePath() = 0;

    virtual std::string defaultToolName(const char *nodetype_category_name,
					const char *nodetype_name) = 0;
};

#endif

