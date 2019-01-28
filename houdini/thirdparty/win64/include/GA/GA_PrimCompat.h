/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimCompat.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimCompat_h__
#define __GA_PrimCompat_h__

#include "GA_API.h"
#include "GA_PrimitiveTypeMask.h"

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Types.h>

class UT_TokenString;

/// Namespace for storing transitional types to provide source compatibility
/// with the old bitmask primitive type detection.

namespace GA_PrimCompat
{

// Ensure enum is in the GA_PrimCompat namespace
typedef uint64	value_type;
enum
{
    TypeMaskBits = std::numeric_limits<value_type>::digits
};

class GA_API TypeMask
{
public:
    TypeMask() {}
    explicit TypeMask(value_type mask)
	: myMask(mask) {}

    static TypeMask fullMask()	{ return TypeMask(~(value_type)0); }

    void	 operator|=(const TypeMask &m)
		    { myMask |= m.myMask; }
    void	 operator&=(const TypeMask &m)
		    { myMask &= m.myMask; }
    void	 operator^=(const TypeMask &m)
		    { myMask ^= m.myMask; }
    bool	 operator==(const TypeMask &m) const
		    { return myMask == m.myMask; }
    bool	 operator!=(const TypeMask &m) const
		    { return !operator==(m); }

    void	 save(UT_TokenString &token_string) const;
    
    /// Convert from new-style primitive type mask to this old-style one. 
    /// Avoid using if possible and modify the function that requires the
    /// mask as an argument, instead.
    /// Returns \c false if it was unable to convert all the bits from the
    /// type mask into the old-school mask.
    bool 	 setFromPrimitiveTypeMask(const GA_PrimitiveTypeMask &mask);
    
    operator GA_PrimitiveTypeMask() const;
    
    SYS_SAFE_BOOL inline operator bool() const  { return myMask != 0; }

private:
    friend const TypeMask operator|(const TypeMask &m1, const TypeMask &m2);
    friend const TypeMask operator&(const TypeMask &m1, const TypeMask &m2);
    friend const TypeMask operator^(const TypeMask &m1, const TypeMask &m2);
    friend const TypeMask operator~(const TypeMask &m1);

    value_type	 myMask;
};

// Binary Operations on GA_TypeMask
inline const TypeMask
operator|(const TypeMask &m1, const TypeMask &m2)
{
    return TypeMask(m1.myMask | m2.myMask);
}
inline const TypeMask
operator&(const TypeMask &m1, const TypeMask &m2)
{
    return TypeMask(m1.myMask & m2.myMask);
}

inline const TypeMask
operator^(const TypeMask &m1, const TypeMask &m2)
{
    return TypeMask(m1.myMask ^ m2.myMask);
}
inline const TypeMask
operator~(const TypeMask &m1)
{
    return TypeMask(~m1.myMask);
}

}

#endif
