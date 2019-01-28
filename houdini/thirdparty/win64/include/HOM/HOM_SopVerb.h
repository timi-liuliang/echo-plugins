/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SopVerb_h__
#define __HOM_SopVerb_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
class HOM_Geometry;
class HOM_SopNode;

SWIGOUT(%rename(SopVerb) HOM_SopVerb;)

class HOM_API HOM_SopVerb
{
public:
    HOM_SopVerb()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_SopVerb(const HOM_SopVerb &sop_verb)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_SopVerb()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void execute(HOM_Geometry &dest, const std::vector<HOM_Geometry *> &inputs) = 0;

    virtual void loadParmsFromNode(HOM_SopNode &sopnode) = 0;

    virtual std::map<std::string, hboost::any> parms() = 0;
    virtual void setParms(const std::map<std::string, hboost::any> &p) = 0;

    virtual int minNumInputs() = 0;
};

#endif
