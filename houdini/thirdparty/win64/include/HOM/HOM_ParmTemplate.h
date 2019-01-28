/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ParmTemplate_h__
#define __HOM_ParmTemplate_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include <string>

SWIGOUT(%rename(ParmTemplate) HOM_ParmTemplate;)

class HOM_API HOM_ParmTemplate
{
public:
    HOM_ParmTemplate()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ParmTemplate(const HOM_ParmTemplate &parm_template)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ParmTemplate()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_ParmTemplate> parm_template) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ParmTemplate> parm_template) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject clone;)
    virtual HOM_ParmTemplate *clone() = 0;

    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;

    virtual std::string label() = 0;
    virtual void setLabel(const char *label) = 0;

    virtual HOM_EnumValue &type() = 0;
    virtual HOM_EnumValue &dataType() = 0;

    virtual HOM_EnumValue &look() = 0;
    virtual void setLook(HOM_EnumValue &look) = 0;

    virtual HOM_EnumValue &namingScheme() = 0;
    virtual void setNamingScheme(HOM_EnumValue &naming_scheme) = 0;

    virtual int numComponents() = 0;
    virtual void setNumComponents(int num_components) = 0;

    virtual std::string disableWhen() = 0;
    virtual void setDisableWhen(const char *disable_when) = 0;
    
    virtual std::map<HOM_EnumValue, std::string> conditionals() = 0;
    virtual void setConditional(const HOM_EnumValue &cond_type,
				const char *conditional) = 0;
				
    virtual bool isHidden() = 0;
    virtual void hide(bool on) = 0;

    virtual bool isLabelHidden() = 0;
    virtual void hideLabel(bool on) = 0;

    virtual bool joinsWithNext() = 0;
    virtual void setJoinWithNext(bool on) = 0;
    // joinWithNext is deprecated in favor of joinsWithNext.
    bool joinWithNext()
    { return joinsWithNext(); }

    virtual std::string help() = 0;
    virtual void setHelp(const char *help) = 0;

    virtual std::string scriptCallback() = 0;
    virtual void setScriptCallback(const char *script_callback) = 0;

    virtual HOM_EnumValue &scriptCallbackLanguage() = 0;
    virtual void setScriptCallbackLanguage(
	    HOM_EnumValue &script_callback_language) = 0;

    virtual std::map<std::string, std::string>	tags() = 0;
    virtual void setTags(const std::map<std::string, std::string> &tags) = 0;

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(
	    const char *function_name=NULL, const char *variable_name=NULL) = 0;
};

#endif
