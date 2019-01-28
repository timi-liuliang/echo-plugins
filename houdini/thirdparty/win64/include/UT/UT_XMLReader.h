/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLReader.h ( UT Library, C++)
 *
 * COMMENTS:
 *  The reader uses the libxml2 API documented at:
 *  http://xmlsoft.org/html/libxml-xmlreader.html
 *  http://xmlsoft.org/examples/reader1.c
 *
 * COMMENTS:
 */

#ifndef __UT_XMLReader__
#define __UT_XMLReader__

#include "UT_API.h"
#include "UT_StringHolder.h"
#include "UT_UniquePtr.h"
#include "UT_WorkBuffer.h"
#include "UT_XMLNode.h"

struct _xmlTextReader;
typedef struct _xmlTextReader *xmlTextReaderPtr;

class UT_API UT_XMLReader
{
public:
    /// Standard constructor.
    UT_XMLReader();

    /// Standard destructor.
    ~UT_XMLReader();

    /// Initialize the reader to read from file.
    bool			 beginReadingFromFile(const char *file);

    /// Initialize the reader to read from memory buffer.
    bool			 beginReadingFromMemory(
				    const UT_WorkBuffer &memory,
				    const char *URL,
				    const char *encoding = nullptr);

    /// Read the next XML element.
    bool			 read();

    /// Return the currently read node.
    /// Return NULL if no node has been read.
    const UT_XMLNode		*getNode();

    /// Return the type for the currently read XML node.
    UT_XMLNodeType		 getNodeType() const;

    /// Pass back the name of the currently read XML node.
    /// @{
    void			 getNodeName(UT_WorkBuffer &name) const;
    void			 getNodeName(UT_StringHolder &name) const
				 {
				     UT_WorkBuffer tmp;
				     getNodeName(tmp);
				     tmp.stealIntoStringHolder(name);
				 }
    /// @}

    /// Pass back the contents of the currently read XML element or text node.
    /// @{
    void			 getNodeContents(UT_WorkBuffer &contents) const;
    void			 getNodeContents(
					UT_StringHolder &contents) const
				 {
				     UT_WorkBuffer tmp;
				     getNodeContents(tmp);
				     tmp.stealIntoStringHolder(contents);
				 }
    /// @}

    /// Return the depth of the current node in the XML document.
    int				 getNodeDepth() const;

    /// Return true if the currently read node has attributes.
    /// Return false otherwise.
    bool			 hasNodeAttributes() const;

    /// Pass back the value of the specified attribute on the current node.
    /// @{
    void			 getNodeAttribute(const char *attr_name, 
					UT_WorkBuffer &attr_val) const;
    void			 getNodeAttribute(const char *attr_name, 
					UT_StringHolder &attr_val) const
				 {
				     UT_WorkBuffer tmp;
				     getNodeAttribute(attr_name, tmp);
				     tmp.stealIntoStringHolder(attr_val);
				 }
    /// @}

    /// Return true if the currently read node has a value.
    /// Return false otherwise.
    bool			 hasNodeValue() const;

    /// Pass back the value of the current node.
    /// @{
    void			 getNodeValue(UT_WorkBuffer &node_val) const;
    void			 getNodeValue(UT_StringHolder &node_val) const
				 {
				     UT_WorkBuffer tmp;
				     getNodeValue(tmp);
				     tmp.stealIntoStringHolder(node_val);
				 }
    /// @}

private:
    static void deleteTextReader(xmlTextReaderPtr p);
    using TextReaderPtr = UT_UniquePtr<_xmlTextReader,
				       decltype(&deleteTextReader)>;

    /// The reader that does the actual reading:
    TextReaderPtr		 myReader;

    /// Temporary storage for the current node.
    UT_UniquePtr<UT_XMLNode>	 myNode;
};

#endif

