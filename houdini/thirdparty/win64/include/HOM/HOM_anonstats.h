/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_anonstats_h__
#define __HOM_anonstats_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"

SWIGOUT(%rename(anonstats) HOM_anonstats;)
class HOM_API HOM_anonstats
{
public:
    virtual ~HOM_anonstats()
    {}

    virtual std::string __repr__() = 0;

    virtual void recordOccurrence(const char *key) = 0;

    virtual void recordString(const char *key, const char *value) = 0;

    virtual bool hasString(const char *key) = 0;

    virtual std::string getString(const char *key) = 0;

    virtual void incrementCount(const char *key, int count=1) = 0;

    virtual void logEvent(const char *key, const char *message) = 0;

    virtual void addSum(const char *key, const double stat_value, int count=1) = 0;
};

#endif
