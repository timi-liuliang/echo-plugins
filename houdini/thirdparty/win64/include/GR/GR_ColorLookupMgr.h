/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_ColorLookupMgr.h (GR library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __GR_ColorLookupMgr_h__
#define __GR_ColorLookupMgr_h__

#include "GR_API.h"

#include <UT/UT_StringMap.h>
#include <UT/UT_Array.h>

class gr_ColorLookupInfo;

class GR_API GR_ColorLookupMgr
{
public:
    GR_ColorLookupMgr();
    ~GR_ColorLookupMgr() {}

    int findType(const char *token) const;

    int getNumTypes() const { return myTypes.entries(); }
    const char *getTypeLabel(int i) const;

private:
    bool registerType(const char *token, const char *label);

    class gr_ColorLookupInfo
    {
    public:
        gr_ColorLookupInfo() {}
        gr_ColorLookupInfo(const char *name, const char *label)
        {
	    myName.harden(name);
	    myLabel.harden(label);
        }

        UT_StringHolder myName;
        UT_StringHolder myLabel;
    };

    UT_StringMap<int> myTable;
    UT_Array<gr_ColorLookupInfo> myTypes;
};

GR_API extern GR_ColorLookupMgr *GRgetColorLookupMgr();

#endif // __GR_ColorLookupMgr_h__
