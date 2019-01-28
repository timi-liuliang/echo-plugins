/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_StyleSheet_h__
#define __HOM_StyleSheet_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Node.h"
#include "HOM_Prim.h"
#include "HOM_PtrOrNull.h"
#include <string>

SWIGOUT(%rename(StyleSheet) HOM_StyleSheet;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_StyleSheet;)

class HOM_API HOM_StyleSheet
{
public:
#ifdef SWIG
%extend
{
    HOM_StyleSheet()
    { return HOM().newStyleSheet(); }
    HOM_StyleSheet(const char *json_text)
    { return HOM().newStyleSheet(json_text); }
}
#else
    HOM_StyleSheet()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_StyleSheet()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_StyleSheet> stylesheet) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_StyleSheet> stylesheet) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject clone;)
    virtual HOM_StyleSheet *clone() = 0;

    SWIGOUT(%kwargs combine;)
    SWIGOUT(%newobject cloneWithObject;)
    virtual HOM_StyleSheet *cloneWithObject(HOM_Node &obj) = 0;

    SWIGOUT(%kwargs combine;)
    SWIGOUT(%newobject cloneWithPrim;)
    virtual HOM_StyleSheet *cloneWithPrim(HOM_Prim &prim) = 0;

    SWIGOUT(%kwargs combine;)
    SWIGOUT(%newobject cloneWithShape;)
    virtual HOM_StyleSheet *cloneWithShape(const char *shape_name,
	    HOM_Prim &agent_prim)=0;

    SWIGOUT(%kwargs cloneWithAddedStyleSheet;)
    SWIGOUT(%newobject cloneWithAddedStyleSheet;)
    virtual HOM_StyleSheet *cloneWithAddedStyleSheet(
	    HOM_StyleSheet &stylesheet,
	    const std::string &target = std::string()) = 0;

    virtual std::string errors() = 0;

    virtual std::string asJSON() = 0;
};

#endif
