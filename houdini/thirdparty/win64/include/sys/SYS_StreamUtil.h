/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_StreamUtil.h (SYS Library, C++)
 *
 * COMMENTS:	Utilities for dealing streams
 */

#ifndef __SYS_STREAMUTIL_H_INCLUDED__
#define __SYS_STREAMUTIL_H_INCLUDED__

#include "SYS_API.h"
#include <iostream>

/// SYS_AutoPrecision allows one to temporarily change a stream's precision
/// for the life of this object.
class SYS_API SYS_AutoPrecision
{
public:
    explicit SYS_AutoPrecision(std::ostream &os, int new_precision)
	: myStream(os)
	, myOldPrecision(os.precision(new_precision))
    {
    }
    ~SYS_AutoPrecision()
    {
	(void) myStream.precision(myOldPrecision); // restore
    }

private:
    std::ostream &  myStream;
    std::streamsize myOldPrecision;
};

#endif // __SYS_STREAMUTIL_H_INCLUDED__
