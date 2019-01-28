/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_HDAOptions_h__
#define __HOM_HDAOptions_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"

SWIGOUT(%rename(HDAOptions) HOM_HDAOptions;)

SWIGOUT(%feature("notabstract") HOM_HDAOptions;)
class HOM_API HOM_HDAOptions
{
public:
#ifdef SWIG
%extend
{
    HOM_HDAOptions()
    { return HOM().newHDAOptions(); }
}
#else
    HOM_HDAOptions()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_HDAOptions(const HOM_HDAOptions &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_HDAOptions()
    { HOM_DESTRUCT_OBJECT(this) }
#endif

    virtual bool operator==(HOM_PtrOrNull<HOM_HDAOptions> options) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_HDAOptions> options) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual bool checkForExternalLinks() = 0;
    virtual void setCheckForExternalLinks(bool on) = 0;

    virtual bool compressContents() = 0;
    virtual void setCompressContents(bool on) = 0;

    virtual int compressionType() = 0;
    virtual void setCompressionType(int type) = 0;

    virtual bool forbidOutsideParms() = 0;
    virtual void setForbidOutsideParms(bool on) = 0;

    virtual bool lockContents() = 0;
    virtual void setLockContents(bool on) = 0;

    virtual bool makeInitialParmsDefaults() = 0;
    virtual void setMakeInitialParmsDefaults(bool on) = 0;

    virtual bool parametersFromVexCode() = 0;
    virtual void setParametersFromVexCode(bool on) = 0;

    virtual bool prefixDroppedParmLabels() = 0;
    virtual void setPrefixDroppedParmLabels(bool on) = 0;

    virtual bool prefixDroppedParmNames() = 0;
    virtual void setPrefixDroppedParmNames(bool on) = 0;

    virtual bool saveInitialParmsAndContents() = 0;
    virtual void setSaveInitialParmsAndContents(bool on) = 0;

    virtual bool unlockNewInstances() = 0;
    virtual void setUnlockNewInstances(bool on) = 0;

    virtual bool saveSpareParms() = 0;
    virtual void setSaveSpareParms(bool on) = 0;

    virtual bool saveCachedCode() = 0;
    virtual void setSaveCachedCode(bool on) = 0;

};

#endif
