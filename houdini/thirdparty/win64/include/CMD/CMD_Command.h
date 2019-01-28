/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Command Class for Command Library (C++)
 *
 * COMMENTS:	This is the class for a general command.
 *		Instansiating one of these will automatically install it
 *		in the command manager.
 *		The name, options, and help are not duplicated, so don't
 *		pass in volatile data please.
 *
 */

#ifndef __CMD_Command_H__
#define __CMD_Command_H__

#include "CMD_API.h"
class CMD_Args;

typedef void	(*CMD_Callback)(CMD_Args &args);

class CMD_API CMD_Command {
public:
     const char		*getName() const	{ return myName; }
     const char		*getOptions() const	{ return myOptions; }
     CMD_Callback	&getCallback()		{ return myCallback; }

     CMD_Command	*getChainCommand()	{ return myChain; }
     const char		*getDSOLocation()	{ return myDSOLocation; }
     bool		 isSafe() const		{ return myIsSafe; }
protected:
private:
     const char		*myName;
     const char		*myOptions;
     char		*myDSOLocation;
     CMD_Callback	 myCallback;
     CMD_Command	*myChain;
     int		 myIndex;		// Index into command table
     bool		 myIsSafe;

     CMD_Command(const char *n, const char *o, CMD_Callback c, bool is_safe);
    ~CMD_Command();

     friend class	CMD_Manager;
};

CMD_API void CMDpythonCommandCallback(CMD_Args &args);

#endif
