/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DD_ChoiceList.h ( DD Library, C++)
 *
 * COMMENTS:	Contains a list of all the drag/drop choices.
 */

#ifndef __DD_ChoiceList__
#define __DD_ChoiceList__

#include "DD_API.h"
#include <UT/UT_StringArray.h>

class DD_API DD_ChoiceList {
public:
     DD_ChoiceList();
    ~DD_ChoiceList();

    void	clear();
    void	addChoice(const char *token,
			  const char *label,
			  const char *help);

    int		 entries() const	{ return myTokens.entries(); }
    const char	*getToken(int i) const	{ return myTokens(i); }
    const char	*getLabel(int i) const	{ return myLabels(i); }
    const char	*getHelp(int i) const	{ return myHelp(i); }

private:
    UT_StringArray	myTokens, myLabels, myHelp;
};

#endif

