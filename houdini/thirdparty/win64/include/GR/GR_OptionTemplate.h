/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_OptionTemplate.h ( GR Library, C++)
 *
 * COMMENTS:
 *      A GR_OptionTemplate defines a display option type that can be
 *      used to create the corresponding GR_UserOption.
 */

#ifndef __GR_OptionTemplate__
#define __GR_OptionTemplate__

#include "GR_API.h"
#include "GR_UserOption.h"

#include <UT/UT_String.h>


class GR_API GR_OptionTemplate
{
public:
    typedef GR_UserOption *(*GR_OptionConstructor)
		            (const char *, GR_OptionTemplate *);

    GR_OptionTemplate(const char *name, const char *label,
		      GR_OptionConstructor construct,
		      GR_UserOptionType type);

    virtual 	~GR_OptionTemplate();

    const char	*getName() const { return (const char *) myName; }
    const char	*getLabel() const { return (const char *) myLabel; }
    GR_UserOptionType getType() const { return myType; }

    GR_UserOption	*constructOption(const char *name);

private:
    UT_String			myName;
    UT_String			myLabel;
    GR_OptionConstructor	myConstructor;
    GR_UserOptionType		myType;
};

#endif
