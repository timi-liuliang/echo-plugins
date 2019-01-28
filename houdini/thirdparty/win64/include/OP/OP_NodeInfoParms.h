/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_NodeInfoParms.h ( OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_NodeInfoParms__
#define __OP_NodeInfoParms__

#include "OP_API.h"
#include <UT/UT_WorkBuffer.h>

/// Parameters for OP_Node::getInfoText()/OP_Node::getNodeSpecificInfoText()
class OP_API OP_NodeInfoParms
{
public:
    explicit OP_NodeInfoParms()
	: myText(),
	  myVerbose(false),
	  myForceCook(true),
	  mySeparatorLength(-1)
    { }
    ~OP_NodeInfoParms()
    { }

    /// Clear the text
    void		clearText()
			{ myText.clear(); }

    /// The text buffer in which to write the information
    UT_WorkBuffer	&getText()
			{ return myText; }

    /// Append a string to the text buffer.
    void		append(const char *string)
			{ myText.append(string); }

    /// Append text to the buffer.  If you have a single string to
    /// append, to avoid printf errors in the string text, please use: @code
    ///   append("%s", string);
    /// @endcode
    int			 appendSprintf(const char *format, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    /// Append a section separator
    void		 appendSeparator();

    /// Return the separator string token (which is replaced in
    /// @c replaceSeparators()).
    static const char	*getSeparatorToken();

    /// Append "not cooked"
    void		 appendNotCooked(const char *node_type = "Node");

    /// @{
    /// Verbose info
    void		 setVerbose(bool v)
			 { myVerbose = v; }
    bool		 getVerbose() const
			 { return myVerbose; }
    /// @}

    /// @{
    /// Force cook
    void		 setForceCook(bool v)
			 { myForceCook = v; }
    bool		 getForceCook() const
			 { return myForceCook; }
    /// @}

    /// @{
    /// Separator length
    void		 setSeparatorLength(int v)
			 { mySeparatorLength = v; }
    int			 getSeparatorLength() const
			 { return mySeparatorLength; }
    /// @}

    /// Make a separator line (prefixed and suffixed with newlines).
    /// The separator will be a line of '-' which matches the line length of
    /// the longest line in the text (or getSeparatorLength() if the lines are
    /// too long).
    void		 replaceSeparators(char dash='_');

private:
    UT_WorkBuffer	 myText;
    int			 mySeparatorLength;
    bool		 myVerbose;
    bool		 myForceCook;
};

/// Parameters for OP_Node::fillInfoTree()/OP_Node::fillInfoTreeNodeSpecific()
class OP_API OP_NodeInfoTreeParms
{
public:
    explicit OP_NodeInfoTreeParms(fpreal time)
	: myTime(time),
	  myVerbose(false)
    { }
    ~OP_NodeInfoTreeParms()
    { }

    void		 setTime(fpreal time)
			 { myTime = time; }
    fpreal		 getTime() const
			 { return myTime; }

    void		 setVerbose(bool verbose)
			 { myVerbose = verbose; }
    bool		 getVerbose() const
			 { return myVerbose; }

    void		 setDebug(bool debug)
			 { myDebug = debug; }
    bool		 getDebug() const
			 { return myDebug; }

private:
    fpreal		 myTime;
    bool		 myVerbose;
    bool		 myDebug;
};

#endif
