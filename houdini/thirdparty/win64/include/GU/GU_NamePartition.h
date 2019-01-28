/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_NamePartition.h (C++)
 *
 * COMMENTS:	Primitive partitioning operation that splits primitives
 *		into sets based on the "name" attribute.  This is used to
 *		consistently create attribute mappings for rendering of
 *		volume primitives.
 */

#ifndef __GU_NamePartition_h__
#define __GU_NamePartition_h__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_IntArray.h>

class UT_BitArray;
class GU_Detail;

class GU_API GU_NamePartition
{
public:
     GU_NamePartition()	{}
    ~GU_NamePartition()	{}

    // Partition a set of primitives by the primitive "name" attribute.
    // Primitives with the same name will be allocated to different entries
    // in the list.  Primitives with no name will be placed in their own
    // entry in the prims array.
    //
    // This operation recognizes vectors using the following component
    // syntax where val is any name:
    // - val
    //		Non-vector (scalar) values
    // - val.x, val.y, val.z
    //		A 3-component vector
    // - val.xx, val.xy, val.xz,
    //	 val.yx, val.yy, val.yz,
    //	 val.zx, val.zy, val.zz
    //		A 3x3 matrix
    //
    // Parameters:
    // - mask:	The group of primitives to be processed
    // - gdp:	The geometry to partition
    void	partitionByName(const UT_BitArray &mask,
				const GU_Detail *gdp,
				const char *noname_name = 0);

    // Retrieve the vector names or vector sizes within each partition
    int		 nameCount() const		{ return myNames.entries(); }
    const UT_StringHolder	&getName(int name)		{ return myNames(name); }
    int		 getVectorSize(int name)	{ return mySizes(name); }

    // Retrieve the primitives in a vector of a partition
    //
    // Usage is:
    //	for (int i = 0; i < partition.entries(); i++)
    //	{
    //		for (int j = 0; j < partition.nameCount(); j++)
    //		{
    //			partition.getPrimitiveVector(i, j, prims);
    //			// Do something with the primitive array
    //		}
    //	}
    int		 entries() const	{ return myPrims.entries(); }
    void	 getPrimitiveVector(int prim, int name,
				    UT_IntArray &prims);

    bool	 hasUnmatchedNames() const	{ return myUnmatchedNames; }

private:
    UT_Array<UT_IntArray>	 myPrims;
    UT_StringArray		 myNames;
    UT_IntArray			 mySizes;
    UT_IntArray			 myOffsets;
    bool			 myUnmatchedNames;
};

#endif
