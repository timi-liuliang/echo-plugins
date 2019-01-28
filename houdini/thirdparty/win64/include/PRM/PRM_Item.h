/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Item
 *
 * COMMENTS:
 *      An extension to PRM_Name which allows icons to be specified.
 */
#ifndef PRM_ITEM_H
#define PRM_ITEM_H

#include "PRM_Name.h"
#include <UT/UT_String.h>

class PRM_API PRM_Item : public PRM_Name
{
public:
    /// NOTE: Need an explicit default constructor to work around internal
    /// compiler error in Visual Studio 2015 Update 3.
    /// See: https://connect.microsoft.com/VisualStudio/feedback/details/2869531
    PRM_Item();

	     PRM_Item(const char *token,
		      const char *label = NULL,
		      const char *iconname = NULL);
    virtual ~PRM_Item();

    const char *getIcon() const		{ return myIcon; }
    void	setIcon(const char *s)	{ myIcon.harden(s); }

private:
    UT_String	myIcon;
};

#endif
