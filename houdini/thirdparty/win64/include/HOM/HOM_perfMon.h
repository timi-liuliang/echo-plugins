/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_perfMon_h__
#define __HOM_perfMon_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include "HOM_PerfMonRecordOptions.h"
#include <vector>

class HOM_PerfMonEvent;
class HOM_PerfMonProfile;

SWIGOUT(%rename(perfMon) HOM_perfMon;)
class HOM_API HOM_perfMon
{
public:
    virtual ~HOM_perfMon()
    {}

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs startProfile;)
    SWIGOUT(%newobject startProfile;)
    virtual HOM_PerfMonProfile *startProfile(const char *title,
	HOM_PerfMonRecordOptions *options=NULL) = 0;

    SWIGOUT(%newobject loadProfile;)
    virtual HOM_PerfMonProfile *loadProfile(const char *file_path) = 0;
    SWIGOUT(%newobject loadProfile;)
    virtual HOM_PerfMonProfile *activeProfile() = 0;

    // This method is deprecated.  Use startEvent() instead.
    SWIGOUT(%newobject startTimedEvent;)
    SWIGOUT(%kwargs startTimedEvent;)
    virtual HOM_PerfMonEvent *startTimedEvent(
	const char *description,
	bool auto_nest_events=true) = 0;
    SWIGOUT(%newobject startEvent;)
    SWIGOUT(%kwargs startEvent;)
    virtual HOM_PerfMonEvent *startEvent(
	const char *description,
	bool auto_nest_events=true) = 0;

    // This method is deprecated.  Use startCookEvent() instead.
    SWIGOUT(%newobject startTimedCookEvent;)
    SWIGOUT(%kwargs startTimedCookEvent;)
    virtual HOM_PerfMonEvent *startTimedCookEvent(
	const char *description, HOM_Node &node) = 0;
    SWIGOUT(%newobject startCookEvent;)
    SWIGOUT(%kwargs startCookEvent;)
    virtual HOM_PerfMonEvent *startCookEvent(
	const char *description, HOM_Node &node) = 0;

    virtual bool isRecording() = 0;
};

#endif
