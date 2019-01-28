/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library.  (Geo Delta)
 *
 * COMMENTS:
 *
 */
#ifndef __GDT_CoordinateFrames_h__
#define __GDT_CoordinateFrames_h__

#include "GDT_API.h"
#include <GEO/GEO_Detail.h>
#include <UT/UT_Array.h>
#include <UT/UT_Matrix3.h>

class GDT_API GDT_CoordinateFrames
{
public:
    GDT_CoordinateFrames() {}
    GDT_CoordinateFrames(const GDT_CoordinateFrames &other);
    virtual ~GDT_CoordinateFrames() {}

    void	setLocalCoordsFromTopology(const GEO_Detail *ref, const GEO_Detail *gdp);
    void	setLocalCoords(const UT_Array<UT_Matrix3> &xforms);
    void	clear();

    unsigned	entries() const { return myTransforms.entries(); }
    const UT_Matrix3	&getTransform(int i) const { return myTransforms(i); }
    virtual int64	 getMemoryUsage(bool inclusive) const
    {
	int64		memusage = 0;
	if (inclusive)
	    memusage += sizeof(*this);
	memusage += myTransforms.getMemoryUsage(false);
	return memusage;
    }

private:
    UT_Array<UT_Matrix3> myTransforms;
};

#endif
