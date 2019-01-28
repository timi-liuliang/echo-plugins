/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_FolderSetParmTemplate_h__
#define __HOM_FolderSetParmTemplate_h__

#include "HOM_API.h"
#include "HOM_ParmTemplate.h"

SWIGOUT(%feature("notabstract") HOM_FolderSetParmTemplate;)
SWIGOUT(%rename(FolderSetParmTemplate) HOM_FolderSetParmTemplate;)

class HOM_API HOM_FolderSetParmTemplate : virtual public HOM_ParmTemplate
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_FolderSetParmTemplate;
    HOM_FolderSetParmTemplate(
            const char *name,
	    const std::vector<std::string> &folder_names,
	    HOM_EnumValue &folder_type = HOM_folderType::Tabs,
	    const std::map<std::string, std::string> &tags =
		HOM_StdMapStringString())
    {
	return HOM().newFolderSetParmTemplate(
	    name, folder_names, folder_type, tags);
    }
};
#else
    HOM_FolderSetParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_FolderSetParmTemplate(const HOM_FolderSetParmTemplate &parm_template)
    : HOM_ParmTemplate(parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_FolderSetParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig think we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__();)

    virtual std::vector<std::string> folderNames() = 0;
    virtual void setFolderNames(
	    const std::vector<std::string> &folder_names) = 0;

    virtual HOM_EnumValue &folderType() = 0;
    virtual void setFolderType(HOM_EnumValue &folder_type) = 0;

    // This method is deprecated in favour of folderType().
    HOM_EnumValue &folderStyle()
    { return folderType(); }
};

#endif
