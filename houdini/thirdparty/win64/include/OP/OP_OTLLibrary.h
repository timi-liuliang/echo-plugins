/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLLibrary.h ( OTL Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_OTLLibrary__
#define __OP_OTLLibrary__

#include "OP_API.h"
#include "OP_OTLDefinition.h"
#include "OP_OTLSectionNames.h"
#include <VEX/VEX_VexTypes.h>
#include <FS/FS_IndexFile.h>
#include <UT/UT_Defines.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_TimeGate.h>


// Prefix "oplib:" for referencing OTL library files. Eg, "oplib:/Object/hda".
#define OTL_LIBRARY_PREFIX		UT_OTL_LIBRARY_PREFIX

// Prefix "opdef:" for referencing HDA definition index file ("opdef:/Sop/hda").
#define OTL_DEFINITION_PREFIX		UT_HDA_DEFINITION_PREFIX

#define OTL_INTERNAL			"Embedded"
#define OTL_INTERNAL_SH			"Embedded"_UTsh
#define OTL_INTERNAL_META		"Current HIP File"
#define OTL_INTERNAL_META_SH		"Current HIP File"_UTsh
#define OTL_FALLBACK_META		"Fallback Libraries"
#define OTL_OTLSCAN_META		"Scanned Asset Library Directories"
#define OTL_ROOTHDANAME                 "hdaroot"



class OP_API OP_OTLLibrary : public FS_IndexFile
{
public:
	     OP_OTLLibrary();
	     OP_OTLLibrary(FS_Reader *reader);
	     OP_OTLLibrary(const char *source, const char *metasrc);
    virtual ~OP_OTLLibrary();

    const UT_StringHolder	&getMetaSource() const;
    const UT_StringHolder	&getSource() const;

    enum OP_ModTimeStatus 
    {
	MOD_TIME_OK,		//modifiction time was obtained without errors
	MOD_TIME_INTERNAL,	//error; definition is internal (embedded)
	MOD_TIME_SAVE_QUIT,     //error; user chose save and quit
	MOD_TIME_DISCARD_QUIT,	//error; user chose discard and quit
	MOD_TIME_EMBED,		//error; user chose to embed dummy definition
	MOD_TIME_REFRESH_ALL,	// error: user chose to refresh otls and retry
	MOD_TIME_LOAD_FILE	// error: user chose to load alternative file
    };

    /// Checks if filename has a .otl, if not, appends it.
    /// Respects OTL_INTERNAL and other magic constants.
    /// Returns true if filename was modified
    static bool		 forceOTLExtension(UT_String &filename);
    
    /// Constructs a good vfl and vex code section names given the original
    /// suggestion.
    SYS_DEPRECATED(13.0)
    static void		 getVexAndVflSectionNames( const char * suggestion,
				    UT_String & vexsect, UT_String & vflsect);

    /// Constructs a valid VEX code section, with an optional prefix.
    static void		 getVexCodeSectionName(const char * prefix,
               		                       UT_String & vex_code_sect);

    // obtains the source's modification time. If source is not accessible,
    // a pop-up dialog is displayed to prompt for an action:
    // save & quit, discard & quit, retry, or embed)
    // OUTPUTS:
    //  mod_time - a modification time of the source
    // RETURNS:
    //  status of the attempt of getting modification time
    OP_ModTimeStatus 	 getSourceModTime( int &mod_time,
					   const char *type_name = NULL) const;

    bool		 addDefinition(const OP_OTLDefinition &definition,
				       int modtime = -1);
    bool		 removeDefinition(int index);

    int			 getNumDefinitions() const
				{ return myDefinitions.entries(); }
    const OP_OTLDefinition &
			 getDefinition(int index) const
				{ return *myDefinitions(index); }

    int			 getDefinitionIndex(const char *tablename,
					    const char *opname) const;
    int			 getDefinitionModTime(int index) const;
    const UT_StringHolder &getDefinitionName(int index) const;
    const UT_StringHolder &getDefinitionOpTableName(int index) const;
    bool		 getDefinitionIsDummy(int index) const;
    bool		 getDefinition(int index,
				       OP_OTLDefinition &definition) const;

    // Get the FS_IndexFile for an asset definition in this library.  Returns
    // null if no such asset exists.  Otherwise, it is up to the caller to
    // delete the FS_IndexFile returned. 
    // NOTE: that this will NOT properly get the definition index file if the 
    // actual library has not been installed into the scene yet, even if 
    // all you want is an FS_IndexFile.
    // WARNING: Caller gains ownership of the returned index file. You *MUST*
    //          delete the returned pointer when done to avoid memory leaks.
    FS_IndexFileHandle	 getDefinitionIndexFile(
			    const char *tablename, const char *opname) const;

    // Remove a section from the given definition, if it is empty.  This
    // loads the index file that corresponds to the given section, modifies
    // it, and then saves the modified section back to the otl.  Returns
    // true when modification occurred.
    bool		 removeDefinitionSectionIfEmpty(
			    int index, const char *definition_section);

    void		 mergeLibrary(const OP_OTLLibrary &lib);

    virtual bool	 expandToDirectory(const char *destdir);
    virtual bool	 collapseFromDirectory(const char *srcdir);
    virtual void	 writeFile(std::ostream &os) const;

    void		 setIsBeingRefreshed(bool beingrefreshed);
    bool		 getIsBeingRefreshed() const;

    // Use these methods to start and end source modfication time caching,
    // preventing redundant stat() calls on the source file when we expect
    // multiple successive calls to getSourceModTime().
    void		 beginCachingSourceModTime();
    void		 endCachingSourceModTime();

    bool		 getIsModTimeAccessOK() const;

    const UT_StringArray    &getErrorMessages() const;

    static bool		 createBackupFile(const char *filename,
					  bool domove = false);
    static bool		 isReservedSectionName(const UT_StringRef &sectname);
    static const char	*getEventName(int index);
    static const UT_StringHolder &getEventSectionName(int index);

    static const UT_StringHolder &getVexCodeSectionName(VEX_ContextType type);
    static const UT_StringHolder &getVflCodeSectionName(VEX_ContextType type);
    static const UT_StringHolder &getEncapsulatedVexSectionName(VEX_ContextType type);

    // Returns a list of the files that are used in the OTL_Library 'libfile'.
    // This will ignore extraneous files in the file system.
    // NOTE: This is only currently implemented for expanded HDAs (OTLs).
    static bool		 getFilenames(const UT_StringHolder &libfile,
				      UT_StringArray &filenames);

private:
    void		 readDefinitions();
    void		 saveDefinitionsToSection(bool binary);

    UT_StringHolder		 mySource;
    UT_StringHolder		 myMetaSource;
    OP_OTLDefinitionArray	 myDefinitions;
    UT_SymbolMap<int>		 myDefinitionIndexMap;
    UT_StringArray		 myErrorMessages;
    int				 myUseCachedModTime;
    mutable UT_TimeGate<1000>	 myModTimeGate;
    mutable bool		 myIsModTimeAccessOK;
    bool			 myIsBeingRefreshed;
};

#endif

