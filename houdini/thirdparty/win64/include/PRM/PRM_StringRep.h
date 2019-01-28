/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_StringRep.h (PRM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PRM_STRINGREP_H_INCLUDED__
#define __PRM_STRINGREP_H_INCLUDED__

#include "PRM_API.h"
#include "PRM_Lock.h"
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_Types.h>
#include <string.h>

/// A specialized deep string class that implements a special case of hazard
/// pointers where only 1 read-lock per thread for *any* PRM_StringRep
/// object can be active.
class PRM_API PRM_StringRep 
{
public:
				PRM_StringRep()
				    : myData(NULL)
				{
				}
				~PRM_StringRep()
				{
				    ::free(myData);
				}
    /// Thread-safe accessors
    // @{
    void			get(PRM_SpinLock &lock, UT_WorkBuffer &result);
    void			get(PRM_SpinLock &lock, UT_String &result);
    void			harden(PRM_SpinLock &lock, const char *source);
    void			hardenIfNeeded(
				    PRM_SpinLock &lock,
				    const char *source);
    // @}

    /// Non-thread-safe accessors
    // @{
    const char *		unsafeGet() const
				    { return myData; }
    void			unsafeHarden(const char *str)
				{
				    ::free(myData);
				    myData = safeDup(str);
				}
    void			unsafeAdopt(UT_String &str)
				{
				    ::free(myData);
				    myData = str.steal();
				}
    void			unsafeHarden(const PRM_StringRep &str)
				{
				    ::free(myData);
				    myData = safeDup(str.unsafeGet());
				}

    exint			getMemoryUsage() const;
    bool			findString(
				    const char *str,
				    bool fullword,
				    bool usewildcards) const;
    bool			findVariable(const char *str) const;
    // @}

private:
    static inline char *	safeDup(const char *str)
				{
				    if (str != NULL && *str)
					return ::strdup(str);
				    return NULL;
				}

    char *			myData;
};

#endif // __PRM_STRINGREP_H_INCLUDED__
