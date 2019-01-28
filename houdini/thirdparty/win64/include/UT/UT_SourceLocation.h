/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SourceLocation.h ( VEX Library, C++)
 *
 * COMMENTS:
 *   A structure to keep track of source locations.
 */

#ifndef __UT_SourceLocation__
#define __UT_SourceLocation__

#include "UT_API.h"
#include "UT_String.h"

class UT_API UT_SourceLocation
{
public:
    UT_SourceLocation(const char *source = NULL, int line = 0,
                      int column_start = 0, int column_end = 0) :
	mySource(source), myLine(line),
	myColumnStart(column_start), myColumnEnd(column_end)
    {
    }

    UT_SourceLocation(const UT_SourceLocation &src) :
	mySource(src.mySource), myLine(src.myLine),
	myColumnStart(src.myColumnStart), myColumnEnd(src.myColumnEnd)
    {
	if (src.mySource.isHard())
	    mySource.harden();
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += mySource.getMemoryUsage(false);
        return mem;
    }

    void		 harden()		{ mySource.harden(); }
    const UT_String	&source() const		{ return mySource; }
    int			 line() const		{ return myLine; }
    int			 columnStart() const	{ return myColumnStart; }
    int			&columnStart()		{ return myColumnStart; }
    int			 columnEnd() const	{ return myColumnEnd; }
    int			&columnEnd()		{ return myColumnEnd; }

    UT_SourceLocation &operator=(const UT_SourceLocation &src)
    {
	mySource = src.mySource;
	if (src.mySource.isHard())
	    mySource.harden();
	myLine = src.myLine;
	myColumnStart = src.myColumnStart;
	myColumnEnd = src.myColumnEnd;
	return *this;
    }

private:
    UT_String	 mySource;
    int		 myLine;
    int		 myColumnStart, myColumnEnd;
};

UT_API std::ostream &
operator<<(std::ostream &os, const UT_SourceLocation &loc);


#endif // __UT_SourceLocation__
