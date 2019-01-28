/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_BuilderStatus.h ( GT Library, C++)
 *
 * COMMENTS:	Status information about the build process
 */

#ifndef __GT_BuilderStatus__
#define __GT_BuilderStatus__

#include "GT_API.h"
#include <UT/UT_WorkBuffer.h>

/// Return the status of primitive creation from GT_PrimitiveBuilder
class GT_API GT_BuilderStatus {
public:
     GT_BuilderStatus()
	: myStatus(true) {}
    ~GT_BuilderStatus() {}

    /// @{
    /// Called in the building API to add errors or warnings
    void	addError(const char *fmt, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
    void	addWarning(const char *fmt, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
    /// @}

    /// Return success or failure
    bool	 getStatus() const	{ return myStatus; }
    /// Return a string of all the errors (may be a NULL ptr for no errors)
    const char	*getErrors() const
		    { return myErrors.length() ? myErrors.buffer() : NULL; }
    /// Return a string of all the warnings (may be a NULL ptr for no warnings)
    const char	*getWarnings() const
		    { return myWarnings.length() ? myWarnings.buffer() : NULL; }

private:
    UT_WorkBuffer	myErrors;
    UT_WorkBuffer	myWarnings;
    bool		myStatus;
};

#endif

