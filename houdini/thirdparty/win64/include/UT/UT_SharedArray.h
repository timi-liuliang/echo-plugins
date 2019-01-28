/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SharedArray.h (UT Library, C++)
 *
 * COMMENTS:	A shared ptr for encapsulating dynamically allocated arrays
 *
 */

#ifndef __UT_SHAREDARRAY_H_INCLUDED__
#define __UT_SHAREDARRAY_H_INCLUDED__

#include <hboost/shared_array.hpp>
#include <stddef.h>

/// @brief A shared ptr for encapsulating dynamically allocated arrays
///
/// An shared array takes ownership of the object referenced.  The reference
/// counting is done outside the object interface, so the size of a
/// UT_SharedArray is larger than a vanilla pointer.
///
/// As this is a wrapper around the boost hboost::shared_array template, the
/// type (T) must have a <b>Copy Constructor</b> and a valid <b>Assignment
/// Operator</b> @see UT_IntrusivePtr
template<class T>
class UT_SharedArray : public hboost::shared_array<T>
{
public:
    UT_SharedArray(T *p = 0) : hboost::shared_array<T>(p)	{ }

    /// Convenience method to clear the pointer
    void	clear()		{ *this = NULL; }

    /// Steal and take ownership of the data passed in.
    void	steal(T *src)	{ reset(src); }
};

#endif // __UT_SHAREDARRAY_H_INCLUDED__
