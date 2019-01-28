/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Flags for a sop (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __SOP_NodeFlags_h__
#define __SOP_NodeFlags_h__

#include "SOP_API.h"
#include <iosfwd>

class UT_IStream;

class SOP_API SOP_NodeFlags
{
public:
    SOP_NodeFlags() { setDefaults(); }
    SOP_NodeFlags(const SOP_NodeFlags &source);

    void setDefaults();

    void save(std::ostream &os, int binary = 0) const;
    bool load(UT_IStream &is, const char *path=0);

    void setInstance(bool onOff) { instance = onOff; }
    void setNeedGuide1(bool onOff) { needGuide1 = onOff; }
    void setNeedGuide2(bool onOff) { needGuide2 = onOff; }
    void setManagesDataIDs(bool onOff) { managesDataIDs = onOff; }

    bool getInstance() const { return instance; }
    bool getNeedGuide1() const { return needGuide1; }
    bool getNeedGuide2() const { return needGuide2; }
    bool getManagesDataIDs() const { return managesDataIDs; }

private:
    bool instance:1,
         needGuide1:1,
         needGuide2:1,
         managesDataIDs:1;
};

#endif
