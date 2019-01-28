/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ChopNode_h__
#define __HOM_ChopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include "HOM_Track.h"

#include <utility>  // for std::pair

SWIGOUT(%rename(ChopNode) HOM_ChopNode;)

class HOM_API HOM_ChopNode : virtual public HOM_Node
{
public:
    HOM_ChopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ChopNode(const HOM_ChopNode &chop_node)
    : HOM_Node(chop_node),
      HOM_NetworkMovableItem(chop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ChopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isLocked() = 0;
    virtual void setLocked(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isAudioFlagSet() = 0;
    virtual void setAudioFlag(bool on) = 0;
    virtual bool isExportFlagSet() = 0;
    virtual void setExportFlag(bool on) = 0;
    virtual bool isUnloadFlagSet() = 0;
    virtual void setUnloadFlag(bool on) = 0;
    virtual bool isCurrentFlagSet() = 0;
    virtual void setCurrentFlag(bool on) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Track> > tracks(int output_index=0, bool cook=true) = 0;

    SWIGOUT(%newobject track;)
    virtual HOM_Track *track(const char *track_name, int output_index=0, bool cook=true) = 0;

    virtual double sampleRate()=0;
    virtual std::pair<double, double> sampleRange()=0;

    virtual double frameToSamples(double frame)=0;
    virtual double samplesToFrame(double samples)=0;

    virtual double timeToSamples(double time)=0;
    virtual double samplesToTime(double samples)=0;

    virtual HOM_BinaryString clipData(bool binary)=0;
    virtual HOM_BinaryString clipData(bool binary, std::vector<std::string> parms,
				      double start = -1, double end = -1)=0;
    virtual void setClipData(HOM_BinaryString source, bool binary,
			     double offset = 0, const char* rename = 0)=0;

    virtual bool saveClip( const std::string &filename) = 0;
    virtual bool saveClip(const std::string &filename, std::vector<std::string> parms,
			double start, double end) = 0;
};

#endif
