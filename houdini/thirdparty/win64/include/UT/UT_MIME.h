/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	MIME packetting for I/O
 *
 */

#ifndef __UT_MIME_h__
#define __UT_MIME_h__

#include "UT_API.h"
#include "UT_StringHolder.h"
#include "UT_StringMap.h"
#include "UT_WorkBuffer.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class UT_String;
class UT_StringArray;

enum UT_MIME_Status {
    UT_MIME_ERROR = -1,
    UT_MIME_OK = 0,
    UT_MIME_EOF_OK,		// End of file reached
    UT_MIME_EXTERNAL_OK,
    UT_MIME_BINARY,		// Binary MIME format not supported yet
    UT_MIME_BAD_EOF,		// Unexpected EOF
    UT_MIME_BAD_HEADER,		// Bad header
    UT_MIME_BAD_CONTENTS,	// Bad contents in MIME packet
    UT_MIME_BAD_STREAM,		// Bad stream (i.e. bad calling sequence)
    UT_MIME_USER_ABORT		// User cancelled reading the stream
};

class UT_API UT_MIME {
public:
	     UT_MIME();
    virtual ~UT_MIME();


    // --------------------------------------------------------
    // Reading

    UT_MIME_Status	 open(UT_IStream &is);

    /// Read the next part of the MIME file.
    /// 'external_src' is optionally the directory the external files are
    /// stored in.  If this is not specified then UT_MIME_EXTERNAL_OK is
    /// returned for parts that are stored externally.  If the external can
    /// be read its contents will be placed in 'data'.
    UT_MIME_Status	 readPart(UT_IStream &is,
				  UT_String &path,
				  UT_WorkBuffer &data,
				  exint *modtime = nullptr,
				  const char *ext_src = nullptr);
    void		 close(UT_IStream &is) {};

    // --------------------------------------------------------
    // Writing

    void		 open(std::ostream &os,
			      const char *boundaryText = nullptr);

    /// Create a new MIME part in the file with the data specified.
    void		 writePart(std::ostream &os,
				   const UT_StringHolder &path,
				   const UT_WorkBuffer &data,
				   const exint modtime = -1);

    /// Create a new MIME part in the file to indicate externally stored data.
    void		 writeExternalReference(std::ostream &os,
				   const UT_StringHolder &path,
				   const exint modtime = -1);
    void		 close(std::ostream &os);

    // --------------------------------------------------------
    // Filtering
    typedef bool(*TextFilterFunc)(const UT_WorkBuffer &in_data,
				  UT_WorkBuffer &out_data,
				  void *filter_data);

    struct TextFilter
    {
	TextFilterFunc		 myFilter;
	void			*myFilterData;
	
	TextFilter(TextFilterFunc filter, void *filterData)
	    : myFilter(filter),
	      myFilterData(filterData)
	{}
    };
    typedef UT_StringMap<TextFilter> TextFilterList;
    
    static bool		 filter(UT_IStream &is,
				std::ostream &os,
				const char * inclusions,
				const char * exclusions,
				const TextFilterList * filters = nullptr);


    // --------------------------------------------------------
    // Info

    /// Returns true for any non-error status.
    static bool		 StatusOK(UT_MIME_Status status);

    const UT_WorkBuffer &getBoundaryText() const { return myBoundaryText; }
    
    /// Return a list of all the files stored externally from this MIME file.
    /// 'external_src' is optionally the directory the files are stored in.
    static UT_MIME_Status getAllExternalRefs(UT_IStream &is,
					     UT_StringArray &external_files,
					     const char *ext_src = nullptr);
    /// Return a list of all the files stored externally from this MIME file.
    /// 'external_src' is optionally the directory the files are stored in.
    static UT_MIME_Status getAllExternalRefs(const UT_StringHolder &filename,
					     UT_StringArray &external_files,
					     const char *ext_src = nullptr);

    static const char * const theDefaultBoundaryString;

    
private:
    UT_WorkBuffer	 myBoundaryText;



};

#endif
