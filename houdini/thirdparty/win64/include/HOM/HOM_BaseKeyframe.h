/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	The user of HOM will never really know about this class.  They'll
 *	work with either Keyframe or StringKeyframe objects.
 */

#ifndef __HOM_BaseKeyframe_h__
#define __HOM_BaseKeyframe_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_EnumModules.h"
#include "HOM_PtrOrNull.h"
#include <string>

SWIGOUT(%rename(BaseKeyframe) HOM_BaseKeyframe;)

class HOM_API HOM_BaseKeyframe
{
public:
    HOM_BaseKeyframe();
    HOM_BaseKeyframe(const HOM_BaseKeyframe &base_keyframe);
    virtual ~HOM_BaseKeyframe();

    virtual bool operator==(HOM_PtrOrNull<HOM_BaseKeyframe> base_keyframe);
    virtual bool operator!=(HOM_PtrOrNull<HOM_BaseKeyframe> base_keyframe);

    virtual int __hash__();
    virtual std::string __repr__() = 0;

    virtual HOM_EnumValue &evaluatedType() = 0;

    void setExpression(const std::string &expression,
	    HOM_EnumValue *language = NULL);
    std::string expression();
    bool isExpressionSet();

    HOM_EnumValue &expressionLanguage();
    bool isExpressionLanguageSet();

    void setTime(double time);
    bool isTimeSet();
    double time();

    void setFrame(double frame);
    double frame();

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(
	    bool brief=false,
	    bool save_keys_in_frames=false,
	    const char *function_name=NULL) = 0;

protected:
    double myTime;
    bool myIsTimeSet;

    std::string myExpression;
    HOM_EnumValue *myExpressionLanguage;
    bool myIsExpressionSet;
};

#endif
