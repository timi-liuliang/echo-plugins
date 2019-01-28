/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Hash.h (SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_Hash__
#define __SYS_Hash__

#include "SYS_API.h"
#include "SYS_Math.h"
#include <hboost/functional/hash.hpp>

/// Define the type for hash values
typedef std::size_t	SYS_HashType;

template <typename VALUE_TYPE>
static inline SYS_HashType
SYShash(const VALUE_TYPE &value)
{
    return hboost::hash<VALUE_TYPE>()(value);
}

/// Combine two hash values.  This will compute the hash of the @c VALUE_TYPE
/// and combine it with an existing hash in an asymmetric fashion.
/// There must be a static function @code
///	static size_t hash(const VALUE_TYPE &value);
/// @endcode
template <typename VALUE_TYPE>
static inline void
SYShashCombine(SYS_HashType &hash, const VALUE_TYPE &value)
{
    hboost::hash_combine(hash, value);
}

/// Compute the hash for a range of values.  The @c IT type must be hashable
/// @see SYShashCombine()
template <typename IT>
static inline SYS_HashType
SYShashRange(IT it, const IT &end)
{
    SYS_HashType	hash = 0;
    for (; it != end; ++it)
	SYShashCombine(hash, *it);
    return hash;
}

/// Compute the hash for a range of values.  The @c IT type must be hashable
/// @see SYShashCombine()
template <typename IT>
static inline void
SYShashRange(SYS_HashType &hash, IT it, const IT &end)
{
    for (; it != end; ++it)
	SYShashCombine(hash, *it);
}

#endif
