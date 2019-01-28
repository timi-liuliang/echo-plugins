/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLParser.h (  Library, C++)
 *
 * COMMENTS:
 *  The abstract parser uses the libxml2 API documented at:
 *  http://xmlsoft.org/html/libxml-parser.html
 *  http://xmlsoft.org/html/libxml-tree.html
 *
 */

#ifndef __UT_XMLParser__
#define __UT_XMLParser__

#include "UT_API.h"
#include "UT_String.h"

struct _xmlParserCtxt;
typedef struct _xmlParserCtxt *xmlParserCtxtPtr;

class UT_API UT_XMLParser
{
public:
    /// Standard constructor.
    UT_XMLParser();

    /// Standard destructor.
    virtual ~UT_XMLParser();

    /// Set whether the parser will automatically substitute entity references
    /// with the text of the entities' definitions.  By default,
    /// the parser will not substitute entities so that you do not lose
    /// the entity reference information.
    void		     setSubstituteEntities(bool substitute);

    /// Parse XML contents stored in memory.
    /// Return true on a successful parse.  Return false otherwise.
    bool		     parseMemory(const char *contents, int size=-1);

    /// Parse XML in the given file.
    /// Return true on a successful parse.  Return false otherwise.
    bool		     parseFile(const char *filename);

    /// Return the error message for the last parsing run.
    /// If no error was thrown in the last parse, then return an empty string.
    const char		    *getError() const;

protected:
    /// Parse XML using the current context.
    /// Return true on a successful parse.  Return false otherwise.
    bool		     parseContext();

    /// Helper method for parseContext().
    /// This must be overridden by the subclass.
    virtual bool	     parseContextSubclass() = 0;

    /// Return the current parsing context.
    xmlParserCtxtPtr	     getContext() const;

    /// Sets the error message for the current parsing run.
    void		     setError(const char *error);

private:

    /// Initialize the parsing context.
    /// Called immediately before a parse takes place.
    void		     initializeContext();

    /// Clean up the parsing context.
    /// Called immediately after a parse takes place.
    void		     cleanupContext();

    /// The parsing context.
    xmlParserCtxtPtr	     myContext;

    /// Whether the parser should substitute entities.
    bool		     mySubstituteEntities;

    /// Store the error encountered in the last parse run.
    /// If there was no error in the last parse run,
    /// then this string is empty.
    UT_String		     myError;
};

#endif

