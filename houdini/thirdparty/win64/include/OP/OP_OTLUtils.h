/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLUtils.h (OP Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_OTLUtils__
#define __OP_OTLUtils__

#include "OP_API.h"
#include <UT/UT_Compression.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class OP_Network;
class OP_OTLLibrary;
class FS_IndexFile;
class UT_IStream;
class UT_String;
class UT_WorkBuffer;


class OP_API OP_OTLUtils
{
public:

    OP_OTLUtils();

    static OP_OTLUtils	*getInstance()
			{
			    if (!theInstance)
				theInstance = new OP_OTLUtils();
			    return theInstance;
			}
    // -----------------------------------------------------------------------
    // High level routines to handle compression/decompression and
    // conversion to/from MIME

    // auto decompress Contents and load into buffer
    static bool	 getContentsSection(FS_IndexFile &indexFile,
				    UT_WorkBuffer &data);

    // provide stream for auto decompressed Contents stream
    static UT_UniquePtr<UT_IStream> getContentsStream(
		FS_IndexFile &deffile,
		exint *uncompressed_buflen = nullptr);
    // provide stream for auto decompressed Contents stream
    static UT_UniquePtr<UT_IStream> getContentsStream(UT_IStream &is,
		exint nbytes,
		const UT_StringHolder &file_name,
		exint *uncompressed_buflen = nullptr);

    // save the given buffer into the contents section appropriate
    // for the indexfile given
    static bool	 saveContentsSection(FS_IndexFile &indexFile,
				     const UT_WorkBuffer &contents_cpio);

    // If the contents section for the specified definition needs to be
    // processed in any way then do so.
    static bool	 processContentsSection(FS_IndexFile &indexFile,
					const UT_String &section_name);

    // expand an indexfile into a directory and optionally convert
    // the Contents section to the VCS friendly format if possible.
    static bool	 expandToDirectory(OP_OTLLibrary *library,
				   const char *destdir,
				   bool vcsfriendly = false);

    static bool	 collapseLibrary(const char *libfile,
				 const char *srcdir,
				 bool createbackup = true);
    // -----------------------------------------------------------------------
    // Utility Functions for examining directories and such

    // expand an indexfile into a directory and optionally convert
    // the Contents section to the VCS friendly format if possible.

    // Search all the assets for a filename within them
    static void	 findAllSections(const char *srcdir,
				 const char *filename,
				 UT_StringArray &allfiles,
				 bool append = false);

    // Search all the assets for an existing MIME converted Contents section
    static void	 findAllMimeSections(const char *srcdir,
				     UT_StringArray &allfiles,
				     bool append = false);

    // Determine if this directory is probably a valid HDA.
    static bool	 isExpandedOtl(const char *src);

    // -----------------------------------------------------------------------
    // Functions to help out doing diffs of assets.
    
    // Get the contents as text format regardless of how it's stored.
    // This isn't guaranteed to be strictly the MIME format..
    static bool	 getContentsAsText(FS_IndexFile &indexfile,
				   UT_WorkBuffer &contents_text,
				   bool strip_modtime = true);


    static bool	 getNetworkSummary(UT_IStream &is,
				   UT_WorkBuffer &summary,
				   const char *prefix = nullptr,
				   bool isHipFile = false);

    static bool	 getNetworkSummary(OP_Network *network,
				   UT_WorkBuffer &summary,
				   const char *prefix = nullptr,
				   const char *root_name = nullptr);

    // -----------------------------------------------------------------------
    // Mid level routines to handle compression/decompression and
    // conversion to/from MIME.

    // Compress and write the given contents buffer
    static bool	 compressContents(const UT_WorkBuffer &contents_cpio,
				  UT_CompressionType compression_type,
				  std::ostream *os);
	
    static bool	 convertContentsFromText(const UT_StringArray &files,
					 UT_StringArray &out_delete_files);
    static bool	 convertContentsToText(const UT_StringArray &files,
				       bool preserve_modtime,
				       UT_StringArray &out_delete_files);

    // -----------------------------------------------------------------------
    // Node data conversion	

    typedef bool(*ConvertSectionToTextFunc)(const UT_StringHolder &src_path,
					    const UT_WorkBuffer &in_data,
					    UT_WorkBuffer &out_data,
					    bool &store_external);

    typedef bool(*ConvertSectionFromTextFunc)(const UT_StringHolder &src_path,
					      const UT_WorkBuffer &in_data,
					      UT_WorkBuffer &out_data);

    struct ConversionFuncs
    {
	ConvertSectionToTextFunc     myToText;
	ConvertSectionFromTextFunc   myFromText;
	ConversionFuncs(ConvertSectionToTextFunc to_text = nullptr,
			ConvertSectionFromTextFunc from_text = nullptr)
	    : myToText(to_text), myFromText(from_text)
	{}
    };
    typedef UT_StringMap<ConversionFuncs> ConversionList;


    static bool	 registerConversionFuncs(const UT_StringHolder &extension,
					 ConvertSectionToTextFunc to_text,
					 ConvertSectionFromTextFunc from_text);

    static const ConversionList &getConversions()
			{ return getInstance()->myConversions; }

    // -----------------------------------------------------------------------
    // Raw Contents conversions

    static bool	 CPIOToMIME(UT_IStream &is,
			    std::ostream &os,
			    bool preserve_modtime,
			    const char *externals_dest = nullptr);
    static bool	 MIMEToCPIO(UT_IStream &is,
			    std::ostream &os,
			    const char *externals_dest = nullptr);

    // -----------------------------------------------------------------------
    // Parameter conversions

    static UT_StringHolder	 getNameFromCompression(
		UT_CompressionType compression_type);
    static UT_CompressionType	 	 getCompressionFromName(
		const UT_StringHolder &section_name);

    // -----------------------------------------------------------------------
    // Public data

    typedef  UT_Map<UT_StringHolder, UT_CompressionType> CompressionMap;

    static const UT_StringArray		 allContentsSectionNames;
    static const CompressionMap		 contentsCompressionNameMap;

private:
    ConversionList			 myConversions;

    static OP_OTLUtils			*theInstance;

    static ConversionList &_getConversions()
			{ return getInstance()->myConversions; }

};

#endif

