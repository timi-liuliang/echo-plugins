/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_Reader.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_Reader__
#define __FS_Reader__

#include "FS_API.h"
#include "FS_ReaderStream.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

#include <time.h>

class UT_IStream;
class UT_Options;
class UT_String;
class UT_StringArray;
class UT_StringHolder;
class FS_IndexFile;
class FS_ReaderHelper;
class FS_WriteFilterFactory;

/// Class for reading files
class FS_API FS_Reader : public UT_NonCopyable
{
public:
	     FS_Reader();

    /// Use this constructor to create a new reader. The specified file is
    /// opened immediately.
    /// Common options:
    ///	 - "string cache_mode" - Specify the "caching" pattern.  This gives a
    ///	   hint to the file system about how the stream will be accessed.
    ///	   * "normal" - Normal caching
    ///	   * "random" - Access pattern is random
    ///	   * "sequential" - Access pattern is sequential
    ///  - "int buffer_size" - Specify the buffer size (see setvbuf)
    ///  - "bool ascii" - Specify whether the file should be opened in ASCII  
    ///	                  read mode for automatically translating line-endings.
	     FS_Reader(const char *source, const UT_Options *options=0);

    /// NOTE: This new FS_Reader takes ownership of the FS_ReaderStream
	     FS_Reader(FS_ReaderStream *sourcestream);
    virtual ~FS_Reader();

    /// Get the amount of memory owned by this FS_Reader
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Closes the stream. After calling this function, the getStream()
    /// function will always return @c nullptr.
    void		 close();

    /// This function is used to check if the file was successfully opened
    bool		 isGood()
			    { return myStream && myStream->isGood(); }

    /// This function retrieves the C++ stream object which you can interact
    /// with in the normal ways. If this function returns @c nullptr, the file
    /// specified in the constructor could not be found.
    UT_IStream		*getStream() const;

    // Obtains a stream reader that reads a portion of this reader's stream.
    // Note, the underlying buffers are shared, invoking this method will
    // advance the current read position to the subset's beginning, and any 
    // subsequent reading will further advance the read position.
    FS_ReaderStream	*shareDataAndGetSubstream(int64 stream_offset, 
				int64 sub_stream_size, int64 sub_data_size,
				const FS_IStreamFilterFactory * f = nullptr) const;

    time_t		 getModTime() const;
    int64		 getLength() const;
    const UT_StringHolder &getFilename() const;

    // Obtains filter factories that are piggyacked from another FS_IndexFile.
    FS_IStreamFilterFactory	*getSubStreamReadFilterFactory() const;
    FS_WriteFilterFactory	*getSubStreamWriteFilterFactory() const;

    // Functions for adding and removing helpers.
    static void		 addReaderHelper(FS_ReaderHelper *helper);
    static void		 removeReaderHelper(FS_ReaderHelper *helper);

    /// @{
    /// Utility fuction to split the original source path into the index file
    /// path and the section name. It asks all registered reader helpers 
    /// whether they recognize the source path protocol and can provide the
    /// section name. If none of the helpers do, then it tries FS_IndexFile to
    /// split the source path on '?' by default.
    /// @param[in]  source_section_path  The original source path to split.
    /// @param[out] index_file_path If the original source path refers to 
    ///			an index file section, this parameter is set to the 
    ///			index file path. Otherwise, it is set to the
    ///			source_section_path parameter.
    /// @param[out] section_name    If the original source path refers to 
    ///			and index file section, this parameter is set to the
    ///			name of that section. Otherwise, it is set to an empty
    ///			string. If there are many nested sections in the source
    ///			path, then this argument will contain the innermost
    ///			section name, and the index_file_path will point
    ///			to the containing section (itself an index file).
    /// @param[out] section_names    If the original source path refers to 
    ///			a section that is nested within other
    ///			sections (that themselves are index files), then
    ///			this parameter is set to these section names, with 
    ///			the outermost section at the beginning of the array
    ///			and the innermost section (contained within the outer
    ///			sections) at the end of the array. If there is just
    ///			a single section, the array will contain just one
    ///			entry (ie, the single section name),
    ///			and the call will be equivalent to the method
    ///			that takes just the single string.
    /// @return	    True if the path refered to a section name and the outgoing
    ///		    parameter 'section_name' contains a non-empty string;
    ///		    otherwise, returns false.
    static bool		 splitIndexFileSectionPath(
					const char *source_section_path,
					UT_String &index_file_path,
					UT_String &section_name);
    static bool		 splitIndexFileSectionPath(
					const char *source_section_path,
					UT_StringHolder &index_file_path,
					UT_StringHolder &section_name);
    static bool		 splitIndexFileSectionPath(
					const char *source_section_path,
					UT_String &index_file_path,
					UT_StringArray &section_names );
    /// @}

    /// @{
    /// Utility function to combine index file name and section name into the
    /// section path. It asks all registered reader helpers 
    /// whether they recognize the index_file_path path protocol and can
    /// combine it with the section name into the section full path.
    /// If none of the helpers do, then it asks FS_IndexFile to
    /// combine the components, joining them with '?' by default.
    /// @param[out]  source_section_path  The combined section full path.
    /// @param[in] index_file_path The base path to the index file.
    /// @param[in] section_name The name of the section inside the index file.
    /// @param[in] section_names    The names of the nested sections in
    ///			the index file, with the outermost section at the 
    ///			beginning of the array and the innermost section 
    ///			(contained within the outer sections) at the end 
    ///			of the array. If the array contains only one entry,
    ///			it's equivalent to the method that takes just a single
    ///			string.
    static void		 combineIndexFileSectionPath( 
					UT_String &source_section_path,
					const char *index_file_path,
					const char *section_name);
    static void		 combineIndexFileSectionPath(
					UT_String &source_section_path,
					const char *index_file_path,
					const UT_StringArray &section_names );
    /// @}


private:
    void		 createStreamFromSource(const char *source,
				    const UT_Options *options);

    static UT_ValArray<FS_ReaderHelper *>	&getHelpers();

    FS_ReaderStream				*myStream;
};

/// This class provides a plug-in method for adding a custom "file-system"
/// @see FS_WriterHelper, FS_InfoHelper
class FS_API FS_ReaderHelper
{
public:
	     FS_ReaderHelper()
	     { FS_Reader::addReaderHelper(this); }
    virtual ~FS_ReaderHelper()
	     { FS_Reader::removeReaderHelper(this); }

    /// Return an FS_ReaderStream if the helper is able to open the source
    /// filename.
    ///
    /// The options are passed through directly from the FS_Reader
    /// constructor.  See FS_Reader for list of options.
    virtual FS_ReaderStream	*createStream(const char *source,
					const UT_Options *options=nullptr) = 0;

    /// Parse the source path into the index file path and the section name.
    /// 
    /// The FS library recognizes a special source format for FS_IndexFile,
    /// which is a file containing several sections indexed and accessible
    /// by their name. That source format contains '?' as a separator between 
    /// the file path and the section name. 
    /// FS_Reader scans for '?' in the source path before asking helpers 
    /// to handle it. This way index files are supported for custom protoclos 
    /// automatically without them needing to implement it explicitly. 
    /// Eg, "myfiles:path/to/indexfile?section" will work automaticaly, since
    /// FS_Reader will ask the helper for a read stream for
    /// "myfiles:/path/to/indexfile" but will create and use the "section" 
    /// read stream itself.
    /// The downside is that custom protocols can't use '?' in their path
    /// format. To address this isse, this method has been added to let the
    /// helpers decide how to handle the '?'. 
    /// 
    /// If the helper does not use '?' for any special purpose, it does not need
    /// to override this virtual (or it can overloaded it and return false).
    /// If the helper uses '?' for some special meaning (or if it supports some
    /// custom syntax to refer to sections of an index file), then it should 
    /// override it and return true. Further, it can return a non-empty section 
    /// name if the protocol supports section naming and the source path indeed 
    /// refers to a section in an index file. Or, it can return an empty string
    /// for the section name, if the source path does not refer to an index
    /// file (or if helper does not want the automatic index file handling and
    /// plans to handle index files explicitly by itself). 
    ///
    /// @param[in]  source_section_path Original source path to be opened for
    ///		    reading.
    /// @param[out] index_file_path If source_section_path refers to a section
    ///		    in an index file, then this parameter is set to the index 
    ///		    file path (and method returns true).
    ///		    If source_section_path does not refer to a section, then
    ///		    this parameter is set to original path, ie, equal to
    ///		    source_section_path (and method returns true).
    ///		    If the helper does not recognize the source_section_path
    ///		    (ie, it's another protocol), or if recognizes it but does
    ///		    not need (nor want) custom handling for '?' separator,
    ///		    then this parameter is not set (and method returns false).
    /// @param[out] section_name This Similar to index_file_path, but this
    ///		    outgoing parameter is set to the section name, if 
    ///		    source_section_path refers to a section and is set to
    ///		    empty-string if it does not (and method returns true).
    ///		    If the method returns false, it is not set.
    ///
    /// @return	    True if the source_section_path can be handled by the
    ///		    helper, in which case index_file_path and section_name
    ///		    are also set (though section can be an empty-string).
    ///		    False if the source_code_path is not handled by helper,
    ///		    or if helper wants default index file naming convention
    ///		    (with '?' splitting the file and section name).
    ///
    virtual bool    splitIndexFileSectionPath(const char *source_section_path,
					UT_String &index_file_path,
					UT_String &section_name)
		    {
			return false;
		    }

    /// Utility function to combine index file name and section name into the
    /// section path. Performs the reverse of splitIndexFileSectionPath().
    /// If the helper does not use '?' for any special purpose in the source
    /// path, then it does not need to override this virtual (see above).
    /// @param[out]  source_section_path  The combined full section path.
    /// @param[in] index_file_path The base path to the index file.
    /// @param[in] section_name The name of the section inside the index file.
    /// @return	    True if the helper recognizes the 'index_file_path' as its
    ///		    own file protocol. False, if it does not recognize the
    ///		    protocol or if it wants the default joining of file and 
    ///		    section names using the default convention of '?' to 
    ///		    join the components.
    virtual bool    combineIndexFileSectionPath( 
					UT_String &source_section_path,
					const char *index_file_path,
					const char *section_name)
		    {
			return false;
		    }
};

#endif

