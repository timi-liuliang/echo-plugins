/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Notes.h ( CHILI Library, C++)
 *
 * COMMENTS:
 *
 *		A list of index notes overlayed on the graph
 */

#ifndef __CHOP_Notes__
#define __CHOP_Notes__

#include "CHOP_API.h"

#include <UT/UT_FloatArray.h>
#include <UT/UT_ValArray.h>

typedef UT_ValArray<const char *>   CHOP_NoteList;
typedef UT_FprealArray		    CHOP_TimeList;

class UT_IStream;
class UT_OStream;

class CHOP_API CHOP_Notes
{

public:

     CHOP_Notes();
    ~CHOP_Notes();

    int			 putNote(fpreal t, int index);
    int			 putNote(int index, const char *);
    int			 putNote(fpreal t, const char *);

    const char		*getNote(int index) const;
    const char		*getNote(fpreal t0) const;
    int			 getNotes(fpreal t0, fpreal t1, 
				  CHOP_NoteList &clist,
				  CHOP_TimeList &tlist) const;
    void		 clearNotes();

    bool		 save(UT_OStream &os);
    bool		 load(UT_IStream &is);

    int64	getMemoryUsage(bool inclusive) const;

private:

    UT_ValArray<char *>		myStrings;
    UT_FprealArray		myTimes;

    void			reset(void);
    int				removeNote(fpreal t);
    int				removeNote(int idx);

    int				findNote(fpreal t) const;
    int				findOrInsertNote(fpreal t);

};

#endif

