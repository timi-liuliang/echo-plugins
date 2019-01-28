/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_Error.h ( VEX Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VEX_Error__
#define __VEX_Error__

#include "VEX_API.h"
#include <UT/UT_StringArray.h>
#include <SYS/SYS_Types.h>
#include <stdarg.h>

class VEX_ErrorList;

// Pure virtual interface for accumulating error information
class VEX_API VEX_ErrorLog {
public:
    virtual ~VEX_ErrorLog() {}

    void		error(const char *fmt, ...)
				SYS_PRINTF_CHECK_ATTRIBUTE(2,3);
    void		warning(const char *fmt, ...)
				SYS_PRINTF_CHECK_ATTRIBUTE(2,3);

    void		error(const char *fmt, va_list args);
    void		warning(const char *fmt, va_list args);

    void		mergeFrom(const VEX_ErrorList &list);

protected:
    virtual void	addError(const char *msg) = 0;
    virtual void	addWarning(const char *msg) = 0;
};

// Print errors to the console
class VEX_API VEX_ErrorConsole : public VEX_ErrorLog {
public:
    VEX_ErrorConsole();

protected:
    virtual void	addError(const char *msg);
    virtual void	addWarning(const char *msg);
};

// Accumulate errors into lists
class VEX_API VEX_ErrorList : public VEX_ErrorLog {
public:
    const UT_StringArray	&errors() const { return myErrors; }
    const UT_StringArray	&warnings() const { return myWarnings; }

protected:
    virtual void	addError(const char *msg);
    virtual void	addWarning(const char *msg);

private:
    UT_StringArray	myErrors;
    UT_StringArray	myWarnings;
};

// Accumulate errors into a single string
class VEX_API VEX_ErrorString : public VEX_ErrorLog {
public:
    const UT_String	&errors() const { return myErrors; }
    const UT_String	&warnings() const { return myWarnings; }

    void                 clear();

protected:
    virtual void	addError(const char *msg);
    virtual void	addWarning(const char *msg);

private:
    UT_String		myErrors;
    UT_String		myWarnings;
};

#endif

