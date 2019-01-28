/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLSaxParser.h (  Library, C++)
 *
 * COMMENTS:
 *  The SAX parser uses the libxml2 API documented at:
 *  http://xmlsoft.org/html/libxml-parser.html
 *  http://xmlsoft.org/html/libxml-tree.html
 *
 */

#ifndef __UT_XMLSaxParser__
#define __UT_XMLSaxParser__

#include "UT_API.h"
#include "UT_Pair.h"
#include "UT_ValArray.h"
#include "UT_String.h"
#include "UT_XMLParser.h"

struct _xmlSAXHandler;
typedef struct _xmlSAXHandler *xmlSAXHandlerPtr;

class UT_API UT_XMLSaxParser : public UT_XMLParser
{
public:
    /// Standard constructor.
    UT_XMLSaxParser();

    /// Standard destructor.
    virtual ~UT_XMLSaxParser();

protected:

    /// An XML element attribute as defined by a name and a value.
    typedef UT_Pair<UT_String, UT_String>   UT_XMLAttribute;

    /// A list of XML element attributes.
    typedef UT_ValArray<UT_XMLAttribute *>    UT_XMLAttributeList;

    /// Callback methods that are called at various times
    /// of the parse.
    virtual void	     onStartDocument();
    virtual void	     onEndDocument();
    virtual void	     onStartElement(const UT_String &name,
				const UT_XMLAttributeList &attribs);
    virtual void	     onEndElement(const UT_String &name);
    virtual void	     onCharacters(const UT_String &characters);
    virtual void	     onCDATABlock(const UT_String &characters);
    virtual void	     onComment(const UT_String &text);
    virtual void	     onWarning(const UT_String &text);
    virtual void	     onError(const UT_String &text);
    virtual void	     onFatalError(const UT_String &text);

    /// Parse XML using the current context.
    /// Return true on a successful parse.  Return false otherwise.
    bool		     parseContextSubclass();

    /// Utility method for getting the value of an attribute.
    static bool		     getAttribute(const UT_XMLAttributeList &attribs,
					  const char * name, UT_String & value);
private:

    /// SAX parsing callback handler.
    xmlSAXHandlerPtr	     myCallbackHandler;

    /// We need to give the callback helper class
    /// access to the parser's callback functions (i.e. onStartElement).
    friend class utSaxParserCallback;
};

#endif

