/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_TemplateBuilder.h (Parameter Library)
 *
 * COMMENTS:
 *	Constructs a PRM_Template * list from an embedded .ds file.
 *
 */

#ifndef __PRM_TemplateBuilder__
#define __PRM_TemplateBuilder__

#include "PRM_API.h"
#include <UT/UT_StringHolder.h>
#include "PRM_Type.h"

class PRM_Template;
class PRM_ScriptPage;

class PRM_API PRM_TemplateBuilder
{
public:
    PRM_TemplateBuilder(const UT_StringHolder &name, const char *dstext);
    ~PRM_TemplateBuilder();

    PRM_Template	*templates() { return myTemplate; }

    void		 setCallback(const UT_StringRef &name, PRM_Callback callback);
    void		 setChoiceListPtr(const UT_StringRef &name, PRM_ChoiceList *list);

    void		 setNoResim(const UT_StringRef &name, bool noresim);
    void		 setNoCook(const UT_StringRef &name, bool nocook);

    /// Returns if it was just built, used for any successive one
    /// time initialization.  Calling this function will mark it as
    /// built.
    bool		justBuilt();

protected:
    PRM_Template		*myTemplate;
    int				 myTemplateLength;
    PRM_ScriptPage		*myPage;
    bool			 myJustBuilt;
    UT_StringHolder		 myName;
};

#endif
