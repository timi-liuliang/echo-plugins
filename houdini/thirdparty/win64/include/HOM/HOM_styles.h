/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_styles_h__
#define __HOM_styles_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include <vector>

SWIGOUT(%rename(styles) HOM_styles;)

class HOM_API HOM_styles
{
public:
    virtual ~HOM_styles()
    {}

    virtual std::string __repr__() = 0;

    virtual bool hasStyle(const char *name) = 0;

    virtual std::vector<std::string> styles(const char *pattern = NULL) = 0;

    virtual std::string description(const char *style) = 0;

    virtual std::string stylesheet(const char *style) = 0;

    virtual std::string errors(const char *style) = 0;

    virtual void addStyle(const char *name,
			  const char *description,
			  const char *stylesheet) = 0;

    virtual void renameStyle(const char *old_name,
			  const char *new_name) = 0;

    virtual void reorderStyles(const std::vector<std::string> &names) = 0;

    virtual void removeStyle(const char *name) = 0;

    virtual void removeAll() = 0;
};

#endif

