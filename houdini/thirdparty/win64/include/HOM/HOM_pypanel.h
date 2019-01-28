/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_pypanel_h__
#define __HOM_pypanel_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include "HOM_EnumModules.h"
//#include <UT/UT_Experimental.h>
#include <map>
#include <vector>
#include <string>


class HOM_PythonPanelInterface;

SWIGOUT(%rename(pypanel) HOM_pypanel;)

class HOM_API HOM_pypanel
{
public:
    virtual ~HOM_pypanel(){}

    virtual std::string __repr__() = 0;

    virtual void installFile(const char *file_path) const = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PythonPanelInterface> >
	interfacesInFile(const char* file_path) = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_PythonPanelInterface> >
	interfaces() = 0;

    virtual HOM_PythonPanelInterface* interfaceByName(const char* name) = 0;

    virtual void setMenuInterfaces(std::vector<std::string> names) const = 0;

    virtual std::vector<std::string> menuInterfaces() = 0;
};

#endif
