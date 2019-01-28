/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT_ParmDataItem.h ( GU Library, C++)
 *
 * COMMENTS:    Represents geometry delta in parameter.
 */

#ifndef __GDT_ParmDataItem__
#define __GDT_ParmDataItem__

#include "GDT_API.h"
#include "GDT_Detail.h"
#include <PRM/PRM_Parm.h>
#include <PRM/PRM_Type.h>

class GDT_API GDT_ParmDataItem : public PRM_DataItem
{
public:
    GDT_ParmDataItem() 
	: myDelta(0)
    {}

    // We gain ownership of this delta, and will free it on completion.
    GDT_ParmDataItem(GDT_Detail *delta)
	: myDelta(delta) 
    {}

    virtual ~GDT_ParmDataItem() 
    { 
	delete myDelta;
    }

    virtual PRM_Type::PRM_DataType getDataType() const 
	{ return PRM_Type::PRM_DATA_GEODELTA; }
    virtual const char *getDataTypeToken() const { return "geodelta"; }

    virtual bool saveAscii(std::ostream &os) const;
    virtual bool saveBinary(std::ostream &os) const;

    virtual int64 getMemoryUsage() const
    {
	return sizeof(this) 
	    // TODO: Memory usage of GDT_Detail
	    // + (myDelta ? myDelta->getMemoryUsage() : 0)
	    ;
    }

    /// The parameters delta, can be null.
    const GDT_Detail 	*delta() const { return myDelta; }

    /// Steals the delta & resets this to null.  Care must be taken
    /// that the parent handle is unique or you may destroy deltas
    /// held by other parametrs.
    GDT_Detail		*steal() { GDT_Detail *result = myDelta; myDelta = 0; return result; }

private:
    GDT_Detail		*myDelta;
};

#endif
