/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_IndexFile.h ( FS Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __FS_IndexFile__
#define __FS_IndexFile__

#include "FS_API.h"

#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_Defines.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class FS_IndexFileHandle;
class FS_IStreamFilterFactory;
class FS_Reader;
class FS_ReaderStream;
class FS_Section;
class FS_WriteFilterFactory;
class UT_Options;
class UT_String;
class UT_WorkBuffer;

#define FS_SECTION_SEPARATOR		UT_SECTION_SEPARATOR_CHAR
#define FS_SECTION_SEPARATOR_STRING	UT_SECTION_SEPARATOR_STRING

// This class implements a file that contains several sections that can be found
// at certain offsets of a main file.
class FS_API FS_IndexFile
{
public:
    /// Default constructor.
    FS_IndexFile();

    /// This constructor takes a path of some sort (e.g. a file path, an oplib:
    /// path, an opdef: path, an http:// path, etc) to the index file.
    FS_IndexFile(const char *source);

    /// This constructor takes a reader that will be used to retrieve section
    /// data. The index file takes ownership and will delete the reader in the
    /// destructor.  (If null is passed in, this object creates its own default
    /// FS_Reader, but unless a source is given nothing can be read from such
    /// an index file.)
    ///
    /// If no source is given, closing the index file and reopening it again
    /// will have no effect on the underlying reader.  The reader will only be
    /// closed and deleted when the index file object is deleted.  If a source
    /// is passed in, though, the reader will be deleted when the index file is
    /// closed and subsequent openings will use the source path to construct a
    /// new reader.
    FS_IndexFile(FS_Reader *reader, const char *source=nullptr);

    virtual ~FS_IndexFile();

    // Some basic operations on sections (add, remove, modify).
    // These functions don't affect the source file. The changes
    // are only evident when we call writeFile.
    // Reading is NOT currently threadsafe.
    inline bool	 hasSection(const UT_StringRef &section) const
		    { return mySectionTable.contains(section) ||
			     myTempSectionTable.contains(section); }
    inline bool	 hasTempSection(const UT_StringRef &section) const
		    { return myTempSectionTable.contains(section); }
    inline bool	 hasRegularSection(const UT_StringRef &section) const
		    { return mySectionTable.contains(section); }
    int		 getSectionModTime(const UT_StringRef &section) const;
    int		 getSectionDataSize(const UT_StringRef &section) const;
    bool	 readSection(const UT_StringRef &section,
			     char *buffer) const;
    bool	 readSection(const UT_StringRef &section,
        	             UT_WorkBuffer &buffer) const;
    bool	 readOptionsSection(const UT_StringRef &section,
				    UT_Options &options) const;
    void	 addSection(const UT_StringRef &section,
			    const char *filename);
    void	 addSection(const UT_StringRef &section,
        	            const UT_WorkBuffer &buf,
			    int modtime = -1);
    void	 addSection(const UT_StringRef &section,
			    const char *buffer,
			    int len, int modtime = -1);
    void	 addSection(const UT_StringRef &section,
			    const FS_IndexFile &file);
    void	 modifySection(const UT_StringRef &section,
			       const char *buffer,
			       int len, int modtime = -1);
    void	 modifySection(const UT_StringRef &section,
			       const UT_WorkBuffer &buf,
			       int modtime = -1);
    void	 removeSection(const UT_StringRef &section);
    void	 mergeIndexFile(const FS_IndexFile &file, bool overwrite);

    // Change the ordering of some sections. This is basically a book-keeping
    // change, and only affects things when this index file is then saved.
    void	 moveSections(int first, int last, int offset);

    // Get the modification time for the index file as a whole.
    int		 getModTime() const;

    // Get the size of the whole index file
    int64	 getFileDataSize() const;

    // Sets the stream filters used for processing the data when it is read
    // in or written out.
    void	 setFilters(FS_WriteFilterFactory *encrypt_factory,
			    FS_IStreamFilterFactory *decrypt_factory);

    // Gets the decryption and encryption filter factories.
    FS_WriteFilterFactory	*getEncryptionFilter() const;
    FS_IStreamFilterFactory	*getDecryptionFilter() const;

    // Get an opaque pointer to a section.
    FS_Section		*getSection(const UT_StringRef &section) const;

    // Get an FS_Reader for reading a section of the index.
    FS_Reader		*getSectionReader(const UT_StringRef &section) const;

    // These utility functions should only be used by FS_ReaderHelpers
    FS_ReaderStream	*getSectionStream(const UT_StringRef &section) const;
    FS_ReaderStream	*getStreamCopy() const;

    // Given an opaque section pointer get an FS_IndexFile it represents.
    FS_IndexFileHandle	 getIndexFileFromSection(const UT_StringRef &section,
						 const char *source = nullptr) const;

    // Simple access functions to find out about our sections.
    int			 getNumSections() const { return mySections.size(); }
    const UT_StringHolder &getSectionName(int index) const;

    // Access our source file name (which may be empty).
    const UT_StringHolder &getSourceFile() const { return mySourceFile; }

    // Access functions for our description string.
    const UT_StringHolder &getDescription() const { return myDescription; }
    void		   setDescription(const UT_StringHolder &description)
			    { myDescription = description; setModified(); }

    // If we are going to be written to a strstream, this function returns
    // a sensible guess about the buffer size that should be allocated.
    exint		 guessStreamSize() const;

    // Write us out to a stream (with all modifications).
    // Note: Don't write out an index file to the same file it was loaded
    // from, since writing implicitly reads the file. Instead, write it to a
    // separate file and then copy it over. This will also require this index
    // file to be reloaded.
    virtual void	 writeFile(std::ostream &os) const;

    // This functions are for expanding and collapsing an index file,
    // much like hexpand and hcollapse do for cpio archives.
    virtual bool	 expandToDirectory(const char *destdir);
    virtual bool	 collapseFromDirectory(const char *srcdir);

    // Get the current time, used to set the section modification time.
    static int		 getCurrentTime();

    /// Tests if the index file is copy-protected.
    bool		 isCopyProtected() const;

    /// Tests if the index file is black boxed.
    bool		 isBlackBoxed() const;

    /// Tests if the index file is stored as expanded.
    bool		 isExpanded() const
			    { return myIsExpanded; }

    static bool		 canWriteFile(const char *filename);

    bool		 getSectionFileName(const char *section,
					    UT_String &filename);

    // -----------------------------------------------------------------------
    // Section data conversion to/from text

    typedef bool(*ConvertFunc)(const UT_StringHolder &src_path,
			       const UT_WorkBuffer &in_data,
			       UT_WorkBuffer &out_data);

    struct ConversionFuncs
    {
	ConversionFuncs(const char *bin_file_pattern = "",
			const char *text_file_pattern = "",
			ConvertFunc to_text = nullptr,
			ConvertFunc from_text = nullptr)
	    : myToText(to_text),
	      myFromText(from_text),
	      myBinFilePattern(bin_file_pattern),
	      myTextFilePattern(text_file_pattern)
	{}

	void clear()
	{
	    myToText = nullptr;
	    myFromText = nullptr;
	    myBinFilePattern.clear();
	    myTextFilePattern.clear();
	}

	bool isClear() const
	{
	    // sufficient to just text first data member
	    return myToText == nullptr;
	}

	ConvertFunc		 myToText;
	ConvertFunc		 myFromText;
	UT_StringHolder		 myBinFilePattern;
	UT_StringHolder		 myTextFilePattern;
    };
    typedef UT_ArrayStringMap<ConversionFuncs> ConversionList;


    static bool	 registerConversionFuncs(const char *bin_file_pattern,
					 const char *text_file_pattern,
					 ConvertFunc to_text,
					 ConvertFunc from_text);

    static const ConversionList &getConversionsToText();
    static const ConversionList &getConversionsFromText();


protected:
    void		 setModified() { myModified = true; }

    exint		 writeHeader(std::ostream &os) const;

    static ConversionList &_getConversionsToText();
    static ConversionList &_getConversionsFromText();

private:
    typedef UT_ValArray<FS_Section *> FS_SectionArray;

    // Functions for opening and closing our stream.
    void		 openStream(const UT_Options *options = nullptr) const;
    void		 closeStream() const;

    // Helper functions for reading and writing ourselves to a directory.
    void		 makeValidFileName(UT_String &filename);
    bool		 writeSectionListFile(const char *sectionfile);

    // Writes out a section to the output stream. If the filter factory
    // has been set, the filter will be applied to the section data before
    // it is written out.
    void writeSection(FS_Section *section,
		      std::ostream &os,
		      bool as_text) const;

    // Get an FS_Reader for reading a section of the index.
    FS_Reader		*getSectionReader(FS_Section *section) const;

    // These utility functions should only be used by FS_ReaderHelpers
    FS_ReaderStream	*getSectionStream(FS_Section *section) const;

    // Read in our index and use it to create our section data structures.
    void		 readIndex();
    void		 readDirectoryIndex(
				    const UT_String &source_file_path,
				    const UT_String &sections_file_path);
    void		 readStreamIndex();

    // Obtains a size of the i-th section after it has been filtered (if at all)
    int			 getWriteFilteredSectionSize( int index ) const;

    // Recalculates the data sizes of our sections when the read filter is set.
    void		 recalculateFilteredSectionSizes();

    using SectionMap = UT_ArrayStringMap<FS_Section*>;

    // Data:
    mutable FS_Reader		*myReader;
    FS_SectionArray		 mySections;
    SectionMap			 mySectionTable;
    FS_SectionArray		 myTempSections;
    SectionMap			 myTempSectionTable;
    UT_StringHolder		 myDescription;
    UT_StringHolder		 mySourceFile;
    FS_WriteFilterFactory	*myWriteFilter;
    FS_IStreamFilterFactory	*myReadFilter;
    int				 myModTime;
    int64 			 mySize;
    int				 myStreamStart;
    bool			 myModified;
    bool			 myIsExpanded;

    friend class FS_Section;
    friend class FS_EmbeddedSection;
    friend class FS_DiskSection;
    friend class FS_IndexFileHandle;
};

class FS_API FS_IndexFileHandle : public UT_SharedPtr<FS_IndexFile>
{
public:
    FS_IndexFileHandle();
    FS_IndexFileHandle(FS_IndexFile *);
    virtual ~FS_IndexFileHandle();

private:

    friend class FS_Section;
    friend class FS_EmbeddedSection;
    friend class FS_DiskSection;
    friend class FS_IndexFile;
};

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <>
struct DefaultClearer<FS_IndexFile::ConversionFuncs>
{
    using ValueT = FS_IndexFile::ConversionFuncs;
    static void clear(ValueT &v) { v.clear(); }
    static bool isClear(const ValueT &v) { return v.isClear(); }
    static void clearConstruct(ValueT *p)
    {
        new ((void *)p) ValueT();
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif

