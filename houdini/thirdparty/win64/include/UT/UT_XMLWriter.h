/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_XMLWriter.h (UT Library, C++)
 *
 * COMMENTS:
 *		A simple wrapper for the xmlwriter. Implements a few methods
 *		that facilitate some common operations.
 *		
 *		This class uses the libxml2 API documented at:
 *		http://xmlsoft.org/html/libxml-xmlwriter.html
 *		http://xmlsoft.org/examples/testWriter.c
 */

#ifndef __UT_XMLWriter__
#define __UT_XMLWriter__

#include "UT_API.h"

struct _xmlTextWriter;
typedef struct _xmlTextWriter *xmlTextWriterPtr;
struct _xmlBuffer;
typedef struct _xmlBuffer *xmlBufferPtr;
class UT_String;



class UT_API UT_XMLWriter
{
public:
    /// Standard constructor.
		 UT_XMLWriter();
		~UT_XMLWriter();


    /// Initializes the writer to write either to memory or to the file.
    bool	beginWritingToFile( const char * file );
    bool	beginWritingToMemory( UT_String & memory );

    /// Finishes writing, flushes data, and cleans up after writing.
    /// If writing to memory, this method copies the XML stuff to the
    /// UT_String passed in earlier. 
    /// If writing to file, it flushes the document and closes the file.
    bool	endWriting();


    /// Sets the indentation of the XML document elements.
    bool	setIndentation( int spaces_count );

    /// Write the whole element, ie: @verbatim
    ///	   <tag>string</tag>
    /// @endverbatim
    bool	writeElement( const char * tag, const char * string );


    /// Begins an element that will contain some other elements or data.
    bool	startElement( const char * tag );

    /// Ends an element started earlier.
    bool	endElement();


    /// Sets an attribute for the current element that was started but not yet
    /// ended.
    bool	writeAttribute( const char * name, const char * value );

    /// Writes a text string contents data for the current element that was
    /// started but not yet ended.
    bool	writeString( const char * string );


    /// Writes a comment.
    bool	writeComment( const char * string );

    /// Writes string as raw characters. The characters are written exactly
    /// as they appear, so be careful to use valid XML format, etc.
    bool	writeRawString( const char * string );

    /// Writes a CDATA element. If data contains "]]>" (which is the CDATA 
    /// termination sequence, then it is split into a few CDATA elements
    /// (ending in "]]" and starting with ">" to prevent the occurance of this
    /// reserved sequence.
    bool	writeCDataElement( const char * data );


private:
    // The writer that does the actual writing:
    xmlTextWriterPtr	    myWriter;

    // If the writer is writing to memory, this is the actual buffer the
    // xml code is written to.
    xmlBufferPtr	    myBuffer;

    // If the writer is writing to memory, this is the string into which the
    // buffer needs to be copied once writing is done.
    UT_String *		    myMemoryString;
};

#endif

