/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PythonPanelInterface_h__
#define __HOM_PythonPanelInterface_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include "HOM_EnumModules.h"
//#include <UT/UT_Options.h>
#include <string>
#include <vector>

SWIGOUT(%rename(PythonPanelInterface) HOM_PythonPanelInterface;)

class HOM_API HOM_PythonPanelInterface
{
public:
    HOM_PythonPanelInterface()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PythonPanelInterface(const HOM_PythonPanelInterface &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PythonPanelInterface()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_PythonPanelInterface> interface) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_PythonPanelInterface> interface) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual int id() const = 0;
    virtual void setId(int id) = 0;

    virtual std::string name() const = 0;
    virtual void setName(const char *name) = 0;

    virtual std::string label() const = 0;
    virtual void setLabel(const char *label) = 0;

    virtual std::string icon() const = 0;
    virtual void setIcon(const char *icon) = 0;

    virtual std::string script() const = 0;
    virtual void setScript(const char *script) = 0;

    virtual std::string help() const = 0;
    virtual void setHelp(const char *script) = 0;

    virtual std::string helpURL() const = 0;
    virtual void setHelpURL(const char *script) = 0;

    virtual std::string filePath() const = 0;
    virtual void setFilePath(const char *filePath) = 0;
};

#endif
