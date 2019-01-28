/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_properties_h__
#define __HOM_properties_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include <vector>

SWIGOUT(%rename(properties) HOM_properties;)

class HOM_ParmTemplate;

class HOM_API HOM_properties
{
public:
    virtual ~HOM_properties()
    {}

    virtual std::string __repr__() = 0;

    virtual std::vector<std::string> classes(const char *tags = "*") = 0;
    virtual std::string classLabel(const char *class_name) = 0;
    virtual std::vector<std::string> categories(const char *class_name) = 0;
    virtual std::vector<std::string> parameters(const char *class_name,
						const char *category_name) = 0;

    SWIGOUT(%newobject parmTemplate);
    virtual HOM_ParmTemplate *parmTemplate(const char *class_name,
					   const char *parm_name) = 0;
};

#endif

