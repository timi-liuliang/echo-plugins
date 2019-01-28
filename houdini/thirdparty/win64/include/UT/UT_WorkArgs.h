/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_WorkArgs.h ( UT Library, C++)
 *
 * COMMENTS:	Class to implement a dynamic argv array to support unlimited
 *		arguments.  This class is used to replace something like:
 *			char *argv[MAX_SIZE];
 */

#ifndef __UT_WorkArgs__
#define __UT_WorkArgs__

#include "UT_API.h"
#include "UT_Assert.h"
#include <SYS/SYS_Types.h>

#include <stdlib.h>


#define UT_INITIAL_ARGV_SIZE	256

class UT_API UT_WorkArgs {
public:
     UT_WorkArgs();
    ~UT_WorkArgs();

    void	  appendArg(const char *text)	 { setArg(myArgc, text); }
    void	  setArg(int idx, const char *text);

    int		  entries() const	{ return myArgc; }
    int		  getArgc() const	{ return myArgc; }
    const char	 *getArg(int idx) const	{ return myArgv[idx]; }
    const char *const* getArgv() const	{ return myArgv; }

    const char	 *operator[](int i) const
			{ return (i < myArgc) ? myArgv[i]:0; }
    const char	 *operator()(int i) const
			{ UT_ASSERT_P(i <= myArgc); return myArgv[i]; }

    UT_WorkArgs( const UT_WorkArgs &copy );
    UT_WorkArgs	&operator=( const UT_WorkArgs &copy );

    int64 getMemoryUsage(bool inclusive) const;

    typedef const char *const*	iterator;
    iterator	begin() const	{ return myArgv; }
    iterator	end() const	{ return myArgv+myArgc; }

private:
    void	  reserve( int num_elems );
private:
    const char	**myArgv;
    int		  myArgc, myAlloc;
    const char	 *myStackArgv[UT_INITIAL_ARGV_SIZE];
};

#endif
