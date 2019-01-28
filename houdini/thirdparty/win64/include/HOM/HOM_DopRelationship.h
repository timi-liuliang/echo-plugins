/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopRelationship_h__
#define __HOM_DopRelationship_h__

#include "HOM_API.h"
#include "HOM_DopData.h"

class HOM_DopObject;

SWIGOUT(%rename(DopRelationship) HOM_DopRelationship;)

class HOM_API HOM_DopRelationship : virtual public HOM_DopData
{
public:
    HOM_DopRelationship()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopRelationship(const HOM_DopRelationship &doprelationship)
    : HOM_DopData(doprelationship)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopRelationship()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;

    virtual bool matches(const char *pattern) = 0;

    virtual void setGroup(const std::vector<HOM_DopObject *> &objects) = 0;

    virtual void setAffectorGroup(const std::vector<HOM_DopObject *> &objects) = 0;
};

#endif

