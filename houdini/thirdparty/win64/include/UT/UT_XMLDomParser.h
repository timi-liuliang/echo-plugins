/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLDomParser.h (  Library, C++)
 *
 * COMMENTS:
 *  The DOM parser uses the libxml2 API documented at:
 *  http://xmlsoft.org/html/libxml-parser.html
 *  http://xmlsoft.org/html/libxml-tree.html
 *
 */

#ifndef __UT_XMLDomParser__
#define __UT_XMLDomParser__

#include "UT_API.h"
#include "UT_XMLParser.h"
#include "UT_ValArray.h"

class UT_XMLNode;

class UT_API UT_XMLDomParser : public UT_XMLParser
{
public:
    /// Standard constructor.
    UT_XMLDomParser();

    /// Standard destructor.
    virtual ~UT_XMLDomParser();

    /// Return the root node of the document that was parsed.
    /// Return NULL if the parse failed and no document was created
    /// or if no parse has taken place.
    UT_XMLNode	    *getRootNode() const;

protected:
    /// Parse XML using the current context.
    /// Return true on a successful parse.  Return false otherwise.
    bool		     parseContextSubclass();

private:
    void		     clean();
    UT_XMLNode		    *myRootNode;
};

#endif

