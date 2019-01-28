/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLNode.h (  Library, C++)
 *
 * COMMENTS:
 *  The XML node uses the libxml2 API documented at:
 *  http://xmlsoft.org/html/libxml-tree.html#xmlNode
 *
 */

#ifndef __UT_XMLNode__
#define __UT_XMLNode__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_StringHolder.h"

// When compiling a library as static, we need to define LIBXML_STATIC
#if !defined(LIBXML_STATIC) && defined(MAKING_STATIC)
#define LIBXML_STATIC
#endif
#include <libxml/tree.h>

struct _xmlNode;
typedef struct _xmlNode *xmlNodePtr;

class UT_XMLNode;
typedef UT_Array<UT_XMLNode *> UT_XMLNodeList;

// Enumeration of different XML node types.
// http://xmlsoft.org/html/libxml-tree.html#xmlElementType
typedef xmlElementType UT_XMLNodeType;
#define UT_XML_NO_NODETYPE  0

class UT_API UT_XMLNode
{
public:
    /// Constructor.
    explicit UT_XMLNode(xmlNodePtr node);

    /// Destructor.
    ~UT_XMLNode();

    /// Return the node name.
    const UT_StringHolder   &getName();

    /// Return the list of children
    /// @warning Not threadsafe
    const UT_XMLNodeList    &getChildren();

    /// Return the contents of the node.
    /// Return an empty string if this is not a contents node.
    /// @warning Not threadsafe
    const UT_StringHolder   &getContents();

    /// Return the line number that this node appears in
    /// in the file or in memory.
    int			     getLineNumber() const;

private:
    xmlNodePtr		     myNode;
    UT_StringHolder	     myName;
    UT_StringHolder	     myContents;
    UT_XMLNodeList	     myChildren;
};

#endif

