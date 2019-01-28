/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VISF_PrimitiveType.h (VIS Library, C++)
 *
 * COMMENTS:
 * 	A simple class encapsulating the GT/GEO combination of
 * 	primitive types used for a primitive hook.
 */

#ifndef __VISF_PrimitiveType__
#define __VISF_PrimitiveType__

#include "VISF_API.h"

#include <GT/GT_PrimitiveTypes.h>
#include <GA/GA_PrimitiveTypeId.h>
#include <SYS/SYS_Types.h>

struct VISF_PrimitiveType
{
    VISF_PrimitiveType();
    VISF_PrimitiveType(GT_PrimitiveType gt_type);
    VISF_PrimitiveType(GA_PrimitiveTypeId geo_type);
    VISF_PrimitiveType(GT_PrimitiveType gt_type, GA_PrimitiveTypeId geo_type);

    bool	 operator==(const VISF_PrimitiveType &other) const;
    bool	 isValid() const;
    bool	 isMatch(GT_PrimitiveType gt_type) const;
    bool	 isMatch(GA_PrimitiveTypeId geo_type) const;
    bool	 isMatch(GT_PrimitiveType gt_type,
        	         GA_PrimitiveTypeId geo_type) const;
    bool	 isGTType() const;
    bool	 isGEOType() const;

    GT_PrimitiveType	 myGTType;
    int			 myGEOType;
};


inline size_t hash_value(const VISF_PrimitiveType &vt)
{
    return (int64(vt.myGTType) << 32) | int64(vt.myGEOType);
}

#endif
