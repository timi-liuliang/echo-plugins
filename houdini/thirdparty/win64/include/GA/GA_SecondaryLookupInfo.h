/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SecondaryLookupInfo.h (GA  Library, C++)
 *
 * COMMENTS:
 * 	This class holds the information necessary to lookup a specific
 * 	secondary primitive from the top level primary primitive.
 */

#ifndef __GA_SecondaryLookupInfo__
#define __GA_SecondaryLookupInfo__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_VectorTypes.h>


template <typename T> class UT_Array;


/// @brief Information necessary to lookup a secondary primitive.
///
/// Primitives may contain secondary primitives, which may themselves contain
/// other secondary primitives.  This class allows us to store all the nested
/// data necessary to traverse the secondary hierarchy.
class GA_API GA_SecondaryLookupInfo 
{
public:
    class GA_API node
    {
	public:
	    bool operator==(const node &src) const
	    { return myDetail == src.myDetail && myOffset == src.myOffset; }
	    bool operator!=(const node &src) const
	    { return !operator==(src); }

	    GA_Index	myDetail;
	    GA_Offset	myOffset;
    };

     GA_SecondaryLookupInfo();
     GA_SecondaryLookupInfo(GA_Index detail1, GA_Offset offset1);
     GA_SecondaryLookupInfo(GA_Index detail1, GA_Offset offset1,
			    GA_Index detail2, GA_Offset offset2);
     GA_SecondaryLookupInfo(GA_Index detail1, GA_Offset offset1,
			    GA_Index detail2, GA_Offset offset2,
			    GA_Index detail3, GA_Offset offset3);
     GA_SecondaryLookupInfo(const UT_Array<node> &list);
     GA_SecondaryLookupInfo(const UT_Int64Array &detail_indices,
			    const UT_Int64Array &prim_offsets);
     GA_SecondaryLookupInfo(const GA_SecondaryLookupInfo &);
    ~GA_SecondaryLookupInfo();

    unsigned	 getDepth() const { return mySize; }
    const node	&getLookup(unsigned i) const { return myData[i]; }

    GA_SecondaryLookupInfo	&operator=(const GA_SecondaryLookupInfo &src);
    bool	 operator==(const GA_SecondaryLookupInfo &) const;
private:
    node	*myData;
    unsigned	 mySize;
};

#endif

