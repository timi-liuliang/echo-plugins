/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD library (C++)
 *
 * COMMENTS:	History logger for command library
 *
 */

#ifndef __CMD_History_h__
#define __CMD_History_h__

#include "CMD_API.h"
#include <UT/UT_ValArray.h>
#include <iosfwd>

class UT_String;
class UT_WorkBuffer;
class CMD_History;
class CMD_Manager;

class CMD_API CMD_HistoryTable
{
public:
     CMD_HistoryTable();
    ~CMD_HistoryTable();

    //
    //  historyExpand returns 1 if substitution was made
    int			 historyExpand(CMD_Manager *, UT_String &cmd);
    void		 addHistory(const char *cmd);
    //
    // getEntry will return the most recent first. i.e. getEntry(0) returns
    // the most recent command.
    //
    const char		*getCommand(int n) const;
    unsigned int	 entries() const	{ return myHistory.entries(); }

    void		 clearAndDestroy();
    void		 saveHistory(std::ostream &os) const;

private:
    int			 editHistory(UT_String &cmd);
    int			 getHistory(const char *pattern, UT_WorkBuffer &where);

    UT_ValArray<CMD_History *>	 myHistory;
};

#endif
