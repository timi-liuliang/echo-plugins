/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS library (C++)
 *
 * COMMENTS:	List of commands
 */

#ifndef __DS_CommandList_h__
#define __DS_CommandList_h__

#include "PRM_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

class DS_Command;

class PRM_API DS_CommandList {
public:
     enum {
	DS_COMMAND_NONE,		// Plain syntax
	DS_COMMAND_RMAN,		// RenderMan Syntax
	DS_COMMAND_VEX			// Toggles have value with them
     };
     DS_CommandList(const char *filename);
     DS_CommandList(const char * const filename[]);	// Multiple filenames
    ~DS_CommandList();

    int		 findCommand(const char *name);
    DS_Command	*getCommand(const char *name);
    DS_Command	*getCommand(int i)	{ return myCommands(i); }
    int		 getNCommands()		{ return myCommands.entries(); }
    const char	*getFileName() const	{ return myFileName; }
    void	 enforceOptions();

    void	 setSyntax(int syntax);

private:
    void			 load(const char *filename);
    void			 defaultEntry();

    UT_ValArray<DS_Command *>	 myCommands;
    UT_String			 myFileName;
};

#endif
