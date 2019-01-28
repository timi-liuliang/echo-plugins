/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD library (C++)
 *
 * COMMENTS:	A source for commands
 *
 */

#ifndef __CMD_Source_h__
#define __CMD_Source_h__

#include "CMD_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>

#define CMD_MAX_IF_NESTS	128

class UT_WorkBuffer;
class FS_Reader;
class CMD_VariableTable;
class CMD_HistoryTable;
class CMD_Manager;
class CMD_Loop;

enum CMD_IfState {
    CMD_IF_NOTVALID = 0,	// Not in an if statement
    CMD_IF_NOTDONE,		// Haven't found a true branch yet
    CMD_IF_DONE,		// We've completed the true branch
    CMD_IF_BYPASS,		// Nested if inside a false branch
    CMD_IF_DOING		// Doing the true branch
};

class CMD_API CMD_Source {
public:
     CMD_Source(const char *filename, int interactive = 0);
    ~CMD_Source();

    void		 addHistory(const char *cmd);
    CMD_VariableTable	*getLocalVariables()	{ return  myVariables; }

    const char		*getFilename()		{ return myFilename; }

    CMD_HistoryTable	*getHistory()		{ return  myHistory; }

    CMD_IfState		 getIfState()	{ return myIfState[myIfNestLevel]; }
    void		 setIfState(CMD_IfState state)
			 {
			    myIfState[myIfNestLevel] = state;
			 }
    int			 bumpIfNest(int dir);
    const char		*getLine(UT_WorkBuffer &buf);
    int			 getLineNumber() const;
    int			 getIfNestLevel() const		{ return myIfNestLevel;}
    void		 setIfNestLevel(int l)		{ myIfNestLevel = l; }
    void		 setEchoState(int onoff)	{ myEchoState = onoff; }
    int			 getEchoState() const		{ return myEchoState; }

    // Returns true if this source is stdin
    bool		 isStdIn() const;

    int			 doIfStatements()
			 {
			    return getIfState() == CMD_IF_NOTVALID ||
				   getIfState() == CMD_IF_DOING;
			 }

    UT_IStream		*getInputStream() { return myInput; }
    bool		 isInteractive() const
			    { return myIsInteractive; }
    void		 setIsInteractive(bool is_interactive)
			    { myIsInteractive = is_interactive; }

    void		 pushUnfinishedCommands(const char *);
    bool		 hasUnfinishedCommands();
    void		 startBlock();
    void		 endBlock();

private:
    void		 initCommon(const char *filename);
    void		 setInput(char *buffer, int64 buflen);

    CMD_VariableTable	*myVariables;
    CMD_HistoryTable	*myHistory;
    char		*myFilename;
    CMD_Loop		*myCurrentLoop;
    UT_IStream		*myInput;
    char		*myBuffer;
    int			 myLoopStatus;
    int			 myIsInteractive;
    int			 myLineNum;
    int			 myEchoState;
    CMD_IfState		 myIfState[CMD_MAX_IF_NESTS];
    int			 myIfNestLevel;
    UT_IntArray		 myUnfinishedCommandsWaterlines;
    UT_StringArray	 myUnfinishedCommands;

    friend class	 CMD_Manager;
};

#endif
