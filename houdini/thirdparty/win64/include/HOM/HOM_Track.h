/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Track_h__
#define __HOM_Track_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>

class HOM_ChopNode;

SWIGOUT(%rename(Track) HOM_Track;)

class HOM_API HOM_Track
{
public:
    HOM_Track()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Track(const HOM_Track &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Track()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject chopNode;)
    virtual HOM_ChopNode *chopNode() = 0;

    virtual std::string name() = 0;

    virtual double eval() = 0;
    virtual double evalAtTime(double time) = 0;
    virtual double evalAtFrame(double frame) = 0;
    virtual double evalAtSample(double sample) = 0;

    virtual std::vector<double> evalAtTimeRange(double start, double end) = 0;
    virtual std::vector<double> evalAtFrameRange(double start, double end) = 0;
    virtual std::vector<double> evalAtSampleRange(double start, double end) = 0;

    virtual int numSamples() = 0;

    virtual std::vector<double> allSamples() = 0;

    virtual void *_asVoidPointer() = 0;

    // This method is deprecated $SHC/HOM/hom.i
    virtual double evalAtSampleIndex(int index) = 0;

};

#endif
