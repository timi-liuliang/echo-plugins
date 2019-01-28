/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NonCopyable (UT Library, C++)
 *
 * COMMENTS:	Inherit UT_NonCopyable to deny class copying
 *
 */

#ifndef __UT_NONCOPYABLE_H_INCLUDED__
#define __UT_NONCOPYABLE_H_INCLUDED__

namespace UT_NonCopyableNS
{
struct UT_NonCopyable
{
protected:
    UT_NonCopyable() = default;
    ~UT_NonCopyable() = default;
    
    UT_NonCopyable( const UT_NonCopyable& ) = delete;
    UT_NonCopyable& operator=( const UT_NonCopyable& ) = delete;
};
}

typedef UT_NonCopyableNS::UT_NonCopyable UT_NonCopyable;

#endif // __UT_NONCOPYABLE_H_INCLUDED__
