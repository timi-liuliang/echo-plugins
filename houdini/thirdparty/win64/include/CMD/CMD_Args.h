/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Command Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __CMD_Args_H__
#define __CMD_Args_H__

#include "CMD_API.h"
#include <iosfwd>
#include <UT/UT_Args.h>
#include <UT/UT_String.h>

class CMD_Manager;
class CMD_Source;
class CMD_Command;

#define	CMD_MAX_OPTIONS		UT_MAX_OPTIONS

class CMD_API CMD_Args : public UT_Args
{
public:
	     CMD_Args();
    virtual ~CMD_Args();

    std::ostream &out() { return *my_out; }
    std::ostream &err(int show_line_number = 1);

    /// Print an error message on err()
    void	 error(const char *fmt, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    void	 quietErrors() { myFirstError = 0; }
    void	 showUsage();
    void	 callBaseClassCommand();

    UT_String	 commandName();

private:
    void	 init(std::ostream *os, std::ostream *es, CMD_Source *src);

    std::ostream *my_out;
    std::ostream *my_err;
    UT_String	 myFilename;
    int		 myLineNumber;
    CMD_Command	*myCommand;
    int		 myFirstError;

    friend class	CMD_Manager;
};
#endif
