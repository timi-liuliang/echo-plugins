/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:  This is a general purpose skeleton of a parser.
 *            It has facilities to provide failry straightforward parsing
 *            of a "language".  
 *
 *	      To parse a string, just instantiate the derived parser and
 *	      call the doIt() method.
 *
 */

#ifndef __GOP_Parser_h__
#define __GOP_Parser_h__

#include "GOP_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_String.h>


// These are the state constants that are common to all parsers.
// Most parsers will probably want to add their own, must be
// careful to number them uniquely.
#define		ST_START	0
#define		ST_ERROR	99


class GOP_API GOP_Parser
{
public:
    GOP_Parser(const UT_String &str); 
    GOP_Parser(const UT_StringHolder &str); 
    virtual ~GOP_Parser() {}

    // This is the method that the user would call to start the
    // parsing.  The return value can be parser-depenedent.  
    // (e.g. the number of items parsed, an error value, etc...)
    // The return value should be placed in myReturnVal
    virtual int doIt();

protected:
    UT_String	        myString;

    
    
    //////////// State Handlers
    virtual void	handleStart() = 0;
    virtual void	handleError() = 0;

    //////////// Utility Methods
    void		startToken();
    void		endToken();

    // Each parser should override this method and use it to 
    // manage state transitions.  Return false if you want the
    // dispatcher to stop (e.g. when an error occurs)
    virtual bool	dispatch(char c) = 0; 

    // Method that loops on the characters of the string and calls dispatch.
    // There should be no need to override this method.
    virtual void	parseDispatcher();

    // Called during the constructor to initialize the state machine
    // before any parsing is done.
    virtual void	init();

    // Called after the parsing is done.
    virtual void	finish();
    
    // Grabs the current token, goes to 'state' and returns true if
    // successful.  Otherwise goes to state ST_ERROR and returns false
    bool		getInt  (GA_Index &retval, unsigned state);
    bool		getInt  (int   &retval, unsigned state);
    bool		getFloat(float &retval, unsigned state);
    bool		getFloat(double &retval, unsigned state);

    // Checks if the current token is the character c, (stripping out
    // all leading and trailing spaces
    bool		isChar(char c);

    // We also need to count '\0' as a space
    bool		isSpace(char c) { return (isspace(c) || c == '\0'); }



    //////////// State Variables
    char 		*myTokBegin;
    char		*myTokEnd;
    char 		*myTokLastGood;
    unsigned 	 	 myCurrentState;
    int			 myReturnVal;

    //////////// Utility Variables
    char	 	myTempChar;
};


#endif
