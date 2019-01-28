/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_StringMMPattern.h
 *
 * COMMENTS:
 * 	Class holding a compiled pattern for UT_String::multiMatch().
 */

#ifndef __UT_StringMMPattern_h__
#define __UT_StringMMPattern_h__

#include "UT_API.h"

#include <SYS/SYS_Types.h>
#include "UT_NonCopyable.h"

class ut_CompiledMMPatternImpl;

class UT_API UT_StringMMPattern : UT_NonCopyable
{
public:
     UT_StringMMPattern() : myImpl(0) {}
    ~UT_StringMMPattern() { clear(); }

    bool	isEmpty() const { return myImpl == NULL; }
    void	clear();
    void	compileInPlace(char *pattern_buffer, int caseSensitive = 1,
			       const char *separators = ", ");
    void	compileInPlace(char *pattern_buffer, int caseSensitive,
			       char separator)
		{ 
		    char separators[2];
		    separators[0] = separator;
		    separators[1] = 0;
		    compileInPlace(pattern_buffer, caseSensitive, separators);
		}
    void	compile(const char *pattern_buffer, int caseSensitive = 1,
			const char *separators = ", ");
    void	compile(const char *pattern_buffer, int caseSensitive,
			char separator)
		{
		    char separators[2];
		    separators[0] = separator;
		    separators[1] = 0;
		    compile(pattern_buffer, caseSensitive, separators);
		}

    int64 getMemoryUsage(bool inclusive) const;

private:
    ut_CompiledMMPatternImpl	*myImpl;

    friend class UT_String;
};

#endif
