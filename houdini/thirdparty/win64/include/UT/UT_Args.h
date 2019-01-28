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

#ifndef __UT_Args_H__
#define __UT_Args_H__

#include "UT_API.h"
#include "UT_WorkArgs.h"
#include "UT_Assert.h"
#include <stdlib.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_ParseNumber.h>

#define	UT_MAX_OPTIONS		4	// A maximum of three args following
					//  an option
class UT_WorkBuffer;
class UT_String;
class UT_StringRef;
class UT_StringHolder;

class UT_API UT_Args
{
public:
    /// Default constructor
    UT_Args();
    /// Convenience constructor.  This is equivalent to: @code
    ///   UT_Args  args;
    ///   args.initialize(argc, argv);
    ///   args.stripOptions(options);
    /// @endcode
    UT_Args(int argc, const char *const argv[], const char *options);
    /// Destructor
    virtual ~UT_Args();

    int		 argc()	const		{ return my_argc; }
    const char	*argv(unsigned i) const	{ return myArgv(i); }
    const char *const	*argv() const		{ return myArgv.getArgv(); }
    const char	*operator[](unsigned i) const
		 {
		     return (i < my_argc) ? argv(i) : 0;
		 }
    const char	*operator()(unsigned i) const
		 {
		     UT_ASSERT_P(i < my_argc);
		     return argv(i);
		 }

    int		 found(int opt) const
		 {
		     UT_ASSERT_P(opt >= 0 && opt < 128);
		     return my_found[opt];
		 }
    const char	*argp(int opt, int which=0) const
		 {
		     UT_ASSERT_P(opt >= 0 && opt < 128);
		     UT_ASSERT_P(which >= 0 && which < UT_MAX_OPTIONS);
		     return my_opt[opt][which];
		 }
    const char	*argp2(int opt) const
		 {
		     return my_opt[opt][1];
		 }
    const char	*argp3(int opt) const
		 {
		     UT_ASSERT_P(opt >= 0 && opt < 128);
		     return my_opt[opt][2];
		 }

    fpreal	 fargp(int opt, int which=0) const
		 {
		     UT_ASSERT_P(opt >= 0 && opt < 128);
		     UT_ASSERT_P(which >= 0 && which < UT_MAX_OPTIONS);
		     return SYSatof(my_opt[opt][which]);
		 }
    fpreal	 fargp2(int opt) const { return fargp(opt, 1); }
    fpreal	 fargp3(int opt) const { return fargp(opt, 2); }

    int		 iargp(int opt, int which=0) const
		 {
		     UT_ASSERT_P(opt >= 0 && opt < 128);
		     UT_ASSERT_P(which >= 0 && which < UT_MAX_OPTIONS);
		     return SYSatoi(my_opt[opt][which]);
		 }
    int		 iargp2(int opt) const { return (int)fargp(opt, 1); }
    int		 iargp3(int opt) const { return (int)fargp(opt, 2); }

    void	 initialize(int argc, const char *const argv[]);
    /// Initialize with an array of UT_StringRef objects.  This method
    /// extracts the c_str() from the UT_StringRef, so the UT_StringRef objects
    /// need to remain in scope while the UT_Args is in existence.
    void	 initialize(int argc, const UT_StringRef *argv);
    void	 initialize(int argc, const UT_StringHolder *argv);
    void	 appendArg(const char *arg);
    void	 stripOptions(const char *options);

    /// This method will make a command string out of all the arguments (and
    /// options).  Arguments will be quoted according to 'csh' syntax.
    int		 fillCommandLine(UT_WorkBuffer &buf,
				 int first_arg=0,
				 int include_options=1) const;

    /// This is a version of fillCommandLine that takes a UT_String for the
    /// result.
    void	 makeCommandLine(UT_String &str, int first_arg=1) const;

    /// Set an argument to a specific value
    void	 setArgument(int idx, const char *text);

    /// Dump options
    void	 display();

private:
    int		  my_argc;		// Arg count (after options)
    UT_WorkArgs	  myArgv;
    char	  my_found[128];	// Options found (one for each char)
    const char	 *my_opt[128][UT_MAX_OPTIONS];
};
#endif
