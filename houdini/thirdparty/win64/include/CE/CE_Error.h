/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CE_Error.h ( CE Library, C++)
 *
 * COMMENTS:	Compute Engine Error.
 */

#ifndef __CE_Error__
#define __CE_Error__

#include "CE_API.h"

#ifdef CE_ENABLED

#include <UT/UT_String.h>

/// A class derived from cl::Error that allows arbitrary error messages to
/// be stored with the exception.
class CE_API CE_Error: public cl::Error
{
public:
    CE_Error(cl_int err, const char * errStr = NULL) : 
			cl::Error(err, NULL), myErrStr(errStr)
    {
		myErrStr.harden();
	}

    ~CE_Error() throw() {}
	
    virtual const char * what() const throw ()
    {
		return myErrStr.buffer();
    }
	
private:
	UT_String	myErrStr;
};


#endif
#endif
