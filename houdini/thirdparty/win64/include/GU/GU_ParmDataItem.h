/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_ParmDataItem.h ( GU Library, C++)
 *
 * COMMENTS:    Represents geometry data in parameter.
 */

#ifndef __GU_ParmDataItem__
#define __GU_ParmDataItem__

#include "GU_API.h"
#include "GU_DetailHandle.h"
#include <PRM/PRM_Parm.h>
#include <PRM/PRM_Type.h>

class GU_API GU_ParmDataItem : public PRM_DataItem
{
public:
    GU_ParmDataItem() {}
    GU_ParmDataItem(const GU_ConstDetailHandle &gdh)
	: myGdh(gdh) {}

    virtual ~GU_ParmDataItem() {}

    virtual PRM_Type::PRM_DataType getDataType() const 
	{ return PRM_Type::PRM_DATA_GEOMETRY; }
    virtual const char *getDataTypeToken() const { return "geometry"; }

    virtual bool saveAscii(std::ostream &os) const;
    virtual bool saveBinary(std::ostream &os) const;

    virtual int64 getMemoryUsage() const
    {
	return sizeof(this) + myGdh.getMemoryUsage(false);
    }

    const GU_ConstDetailHandle &gdh() const { return myGdh; }

private:
    GU_ConstDetailHandle myGdh;
};

#endif
