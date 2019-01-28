/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_NTCOINITIALIZESCOPE.h (SYS Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __SYS_NTCOINITIALIZESCOPE_H_INCLUDED__
#define __SYS_NTCOINITIALIZESCOPE_H_INCLUDED__

#include "SYS_API.h"

/// SYS_NTCoInitializeScope represents a COM initialization scope.
///
/// For each thread that uses COM, it must reside within the scope of one of
/// these objects.  These scopes may be nested if necessary.
///
class SYS_API SYS_NTCoInitializeScope
{
public:
			SYS_NTCoInitializeScope();
			~SYS_NTCoInitializeScope();

    bool		isInitialized() const;

};

#endif // __SYS_NTCOINITIALIZESCOPE_H_INCLUDED__
