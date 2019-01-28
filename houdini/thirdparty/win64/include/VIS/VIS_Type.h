/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VIS_Type.h ( GR Library, C++)
 *
 */

#ifndef __VIS_Type__
#define __VIS_Type__

#include "VIS_API.h"

#include <UT/UT_String.h>
#include <PRM/PRM_ParmList.h>
#include <PRM/PRM_Template.h>

#include "VIS_Visualizer.h"


typedef VIS_Visualizer 	*(*VIS_Constructor)();

class VIS_API VIS_Type
{
public:

    VIS_Type(const VIS_Type &src);
    VIS_Type(const char *type_name, const char *description,
             PRM_Template *template_list, VIS_Constructor constructor,
             const char *icon, bool is_public = true);

    int 		 getIndex() const;
    const char		*getName() const;
    const char		*getDescription() const;
    const char		*getIcon() const;
    VIS_Constructor 	 getConstructor() const;
    PRM_Template	*getParmTemplates();
    bool		 isPublic() const;

    VIS_Visualizer	*createVisualizer();

private:
    friend class VIS_Manager;

    int			 myIndex;	// index in type table
    UT_String		 myName;
    UT_String		 myDescription;
    UT_String		 myIcon;
    VIS_Constructor	 myConstructor;
    PRM_Template	*myTemplateList;
    bool		 myIsPublic;

    void 		 setIndex(int id);
};

#endif
