/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_Verbose.h ( ROP_Verbose Library, C++)
 *
 * COMMENTS:	Keeps track of an output stream and other flags about how to
 *		output data.
 */

#ifndef __ROP_Verbose__
#define __ROP_Verbose__

#include "ROP_API.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iostream>

class ROP_API ROP_Verbose {
public:
     ROP_Verbose(std::ostream *os=NULL)
	: myStream(os),
	  myLevel(1),
	  myAlfred(false)
     {
     }
     ROP_Verbose(const ROP_Verbose &s)
	 : myStream(s.myStream),
	   myLevel(s.myLevel),
	   myAlfred(s.myAlfred)
     {
     }
    ~ROP_Verbose()
     {
     }

    /// Clear (i.e. set the stream to null & clear levels
    void	clear()
		{
		    myStream = NULL;
		    myLevel = 1;
		    myAlfred = false;
		}

    /// Check to see if the stream is valid
    bool	isValid() const	{ return myStream != NULL; }

    /// Check verbosity level.  For example:
    ///	@code
    ///	   if (verbose.isLevel(2))
    ///		detailedInformation();
    ///	   else if (verbose.isLevel(1))
    ///		briefInformation();
    /// @endcode
    bool	isLevel(int level) const
		    { return myStream && level <= myLevel; }

    /// Assignment operator
    ROP_Verbose	&operator=(const ROP_Verbose &s)
		{
		    myStream = s.myStream;
		    myLevel = s.myLevel;
		    myAlfred = s.myAlfred;
		    return *this;
		}

    /// Copy from a source verbose or clear if the source is NULL
    void	setFrom(const ROP_Verbose *s)
		{
		    if (s)
			*this = *s;
		    else
			clear();
		}

    /// @{
    /// Manage the output stream
    std::ostream *getStream() const		{ return myStream; }
    void	 setStream(std::ostream *s)	{ myStream = s; }
    /// @}

    /// @{
    /// Manage the verbosity level
    int		getLevel() const	{ return myLevel; }
    void	setLevel(int level)	{ myLevel = level; }
    /// @}

    /// @{
    /// Manage the alfred flag (turns on alfred style progress)
    bool	getAlfred() const		{ return myAlfred; }
    void	setAlfred(bool flag=true)	{ myAlfred = flag; }
    /// @}

    /// Print out progress (@c percent should be 0-1)
    void	showProgress(fpreal percent) const
		{
		    if (myStream && myAlfred)
		    {
			int	pct;
			pct = SYSclamp((int)(percent*100 + 0.5), 0, 100);
			*myStream << "ALF_PROGRESS " << pct << "%\n";
			myStream->flush();
		    }
		}

    /// Print out progress based on counts
    void	showProgress(int current, int total) const
		{
		    if (total)
			showProgress((fpreal)current/(fpreal)total);
		}

private:
    std::ostream	*myStream;
    int		 	 myLevel;
    bool	 	 myAlfred;
};

#endif
