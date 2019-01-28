/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLManager.h ( OTL Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_OTLManager__
#define __OP_OTLManager__

#include "OP_API.h"
#include "OP_Operator.h"
#include <UT/UT_Compression.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Notifier.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_UniquePtr.h>
#include <iosfwd>

class UT_WorkBuffer;
class FS_IndexFile;
class OP_Node;
class OP_OperatorTable;
class OP_OTLManagerSink;
class OP_OTLManager;
class OP_OTLLibrary;
class OP_OTLDefinition;

#define OP_TYPEBARMODE_FULLMENU		"fullmenu"
#define OP_TYPEBARMODE_DISPLAYONLY	"displayonly"
#define OP_TYPEBARMODE_NONE		"none"

// Note that the string is deliberately misleading,
// since the "compiled" flag might be just a little
// too obvious.
#define OP_IS_OTL_COMPILED              "nodeconntype"

// Black boxed assets are assets whose implementation details are hidden
// to the user. However, these need no licensing information and can be freely
// distributed. This flag is used to indicate if an asset is blackboxed and
// is purposefully obfuscated, simmilar to the flag above.
#define OP_IS_OTL_BLACKBOXED		"nodeparmtype"

// Functions get executed whenever we call OP_OTLManager::saveOpTypeFromNode.
typedef void (*OP_SaveOpTypeCallback)(OP_Node *);

class OP_API OP_OTLLoadCallback
{
public:

    OP_OTLLoadCallback();
    virtual ~OP_OTLLoadCallback()   {}

    virtual bool onOTLLoadBegin(const char* otl_name,
	    const OP_OTLLibrary* opt_lib,
	    OP_Operator* opt_operator,
	    UT_String& error_message_out) = 0;
};

class OP_API OP_OTLManager
{
public:
	     OP_OTLManager();
    virtual ~OP_OTLManager();

    // Getting and setting of configuration information.
    bool		 getSaveToHipFile() const;
    void		 setSaveToHipFile(bool savetohip);
    bool		 getSaveUnlockedToHipFile() const;
    void		 setSaveUnlockedToHipFile(bool saveunlockedtohip);
    bool		 getWarnOutOfDateOps() const;
    void		 setWarnOutOfDateOps(bool warn);
    bool		 getWarnDummyOpsOnLoad() const;
    void		 setWarnDummyOpsOnLoad(bool warn);
    bool		 getPreferLatestDate() const;
    void		 setPreferLatestDate(bool preferlatest);
    bool		 getPreferInternal() const;
    void		 setPreferInternal(bool preferinternal);
    bool		 getPreferIndexFile() const;
    void		 setPreferIndexFile(bool preferindexfile);
    bool		 getCreateBackups() const;
    void		 setCreateBackups(bool createbackups);
    bool		 getSafeguardOpDefs() const;
    void		 setSafeguardOpDefs(bool safeguardopdefs);
    bool		 getLeaveDefaultsUnchanged() const;
    void		 setLeaveDefaultsUnchanged(bool leavedefaultsunchanged);
    bool		 getUseOplibrariesFiles() const;
    void		 setUseOplibrariesFiles(bool useoplibrariesfiles);
    const UT_String	&getOperatorTypeBarMode() const;
    void		 setOperatorTypeBarMode(const char *mode);
    const UT_StringArray&getAllowedOperatorTypeBarModes() const;

    /// Register a new class to be used as a callback when an OTL library
    /// is loaded.
    static void		 registerOTLLoadCallback(OP_OTLLoadCallback * cb);

    /// Executes onOTLLoadBegin() for each registered callback and returns the result
    bool		 runOTLCallbacks(const char* otl_name,
				const OP_OTLLibrary* opt_lib,
				OP_Operator* opt_operator,
				UT_String& error_out);

    /// Gets the list of otls that have tried to load and failed.
    UT_StringArray	&getFailedOTLs();

    /// Reset the list of failed otls.
    void		 clearFailedOTLList();

    /// Load in the contents of an OPlibraries file. This is public so that
    /// when loading a hip file, the OP_Director can use this to parse the
    /// .OPlibraries packet.
    bool		 loadOpLibrariesFile(UT_IStream &is,
					    const char *metasrc, int &libpos);
    /// Saves the .OPlibraries section that will be included in a .hip file.
    /// This file must be different from the one above because it has to
    /// skip the internal hip file library.
    bool		 saveInternalMetaFile(std::ostream &os,
					      const char *prefix = 0) const;

    /// Picks a particular OTL to be the preferred library for a particular
    /// operator type. Returns true if the operator type exists in an OTL.
    /// Even if we return false, we keep the information in case this
    /// operator type gets loaded later.
    /// WARNING: This method may reload libraries, so holding OP_OTLLibrary
    ///          pointers across this call is unsafe.
    void		 setLibraryPreference(const char *tablename,
					      const char *opname,
					      const char *libfile);
    /// Sets the preference on all assets in the supplied library if
    /// they are not already the current definition.
    /// WARNING: This method may reload libraries, so holding OP_OTLLibrary
    ///          pointers across this call is unsafe.
    void		 setLibraryPreferencesIfRequired(int index);
    /// Gets the preferred library for a particular operator type. Returns
    /// null if no preference is set, or the operator type is not known.
    const char		*getLibraryPreference(const char *tablename,
					      const char *opname) const;
    /// Clear all explicit preferences.
    void		 clearLibraryPreferences(bool refreshlibs);
    /// Functions to save and load the preferred OTL information.
    bool		 loadLibraryPreferences(UT_IStream &is);
    void		 saveLibraryPreferences(std::ostream &os,
					bool includeinternal = true,
					bool protectstrings = false) const;

    /// Returns the descriptive name for a particular meta source.
    const UT_StringHolder	&getMetaSourceName(OP_OTLLibrary *lib,
					   bool withpath = true) const;
    void		 getMetaSourceName(const char *metasrc,
					   UT_String &metasrcname,
					   bool withpath = true) const;
    bool		 setMetaSourceName(const char *metasrc,
					   const char *metasrcname);

    /// Returns true if we are adding a new instance of an existing library,
    /// followed immediately be removing the old copy. If so, we can do some
    /// optimizations in OP_Director::definitionsAdded.
    bool		 getIsPerformingAtomicAddRemove() const;

    /// Returns true if we are in the middle of a call to refreshAll. If we
    /// are, we can optimize out a whole bunch of calls that can be done
    /// once at the end of the refresh.
    bool		 getIsPerformingRefreshAll() const;

    /// Returns true if the manager is in teh process of creating a new HDA.
    bool		 getIsCreatingNewHDA() const;

    /// Basic operations of adding and removing libraries.
    void		 loadAutoOTLs();
    int			 findLibrary(const char *libfile,
				     const char *metasrc = 0) const;
    int			 findNextLibrary(const char *libfile,
					 int after) const;
    bool		 refreshLibrary(int index, bool force = true);
    bool		 removeLibrary(const char *libfile,
				       const char *metasrc,
				       bool changeoplibraries);
    bool		 removeMetaSource(const char *metasrc);
    void		 refreshAll(bool reloadOpLibraries);

    /// This function adds the libfile to the specified metasrc if it
    /// isn't there already (or moves it to the specified position if
    /// it is there). This is the function external callers should
    /// always use.
    OP_OTLLibrary	*addLibrary(const char *libfile,
				    const char *metasrc,
				    bool changeoplibraries,
				    bool installinternaliffail,
				    UT_WorkBuffer &errors);

    // Get basic library information.
    int			 getNumLibraries() const;
    int			 getPreferredLibrary(const char *tablename,
					     const char *opname);
    OP_OTLLibrary	*getLibrary(int index) const;

    /// Returns the library that contains the latest (by date) definition
    /// for the specified operator.
    int			 getLatestLibrary(const char *tablename,
					  const char *opname,
					  OP_OTLLibrary *tiegoesto = 0);
    bool		 getOutOfDateInternalOps(UT_StringArray &ops);

    /// Returns the next library with a definition for the specified operator.
    /// This function loops around to the start of the library list.
    int			 getNextLibrary(const char *tablename,
					const char *opname,
					OP_OTLLibrary *startat);

    /// Adds an operator definition to an existing library, or creates
    /// a new library file if necessary.
    static bool		 addToLibrary(OP_OTLLibrary *lib,
				      OP_OTLDefinition &definition,
				      FS_IndexFile *deffile,
				      bool dummydefinition = false);
    bool		 addToLibrary(const char *libfile,
				      OP_OTLDefinition &definition,
				      FS_IndexFile *deffile,
				      bool dummydefinition = false,
				      bool create_op_type=false,
				      bool create_backup=true);

    bool		 saveCreateBackup(const UT_String &libfile,
				OP_OTLLibrary *newlib);

    /// Take the secondary library and merge it into the primary library,
    /// and save the combined file out to the primary library.
    bool		 mergeLibraries(const char *primary,
					const char *secondary,
					bool overwrite);

    /// These functions are for maintaining our one "internal" library.
    /// This library is where we keep op definitions that have been loaded
    /// form the hip file, or definitions that we want to uninstall but
    /// couldn't because of existing operators of that type.
    bool		 addToInternalLibrary(OP_OTLLibrary *newlib);
    bool		 addToInternalLibrary(UT_IStream &is, int modtime);

    /// Removes some operator definitions from an existing library. We
    /// check the file name to determine if we are removing from the
    /// internal library, or an external library file.
    bool		 removeFromLibrary(const char *libfile,
					   const UT_StringArray &ops);

    /// Update the date stamp for the specified definition to be the current
    /// date. Do a refresh of the library if it is loaded.
    bool		 touchDefinition(const char *libfile,
					 const char *tablename,
					 const char *opname,
					 int newtime = -1);

    /// This functions modifies the data in the OTLDefinition so that it
    /// will refer to the given index file so that both these elements can
    /// be embedded into an OTLLibrary.
    static void		 modifyDefinitionToInternalPaths(
					 OP_OTLDefinition &def,
					 FS_IndexFile *deffile);

    /// A utility function for saving the operator definition for a given node.
    void		 saveOpTypeFromNode( OP_Node *node, UT_String &errors );

    /// A utility function for updating the Contents section of an asset
    /// definition based on a particular OP_Node. The compile flags can be
    /// used to control whether the contents are compiled
    void		 updateContentsFromNode(FS_IndexFile *deffile,
						OP_Node *node,
						UT_CompressionType compressionType,
						bool compilebasic,
						bool compilevopnets,
						bool compilechannels,
						bool compilenodenames,
						const char *library,
						bool blackbox = false);

    /// A utility function for updating an operator definition based on
    /// a particular OP_Node.
    void		 updateDefinitionFromNode(FS_IndexFile *deffile,
						  OP_Node *node,
						  bool lockcontents,
						  bool usedsparms,
						  UT_CompressionType compressionType,
						  bool savespareparms,
						  bool compile_asset,
						  const char *library,
						  bool blackbox = false);

    /// A utility functino to make sure that the Contents section of the
    /// given file is either compressed or not, as set by gzipcontents.
    void		 setContentsCompression(FS_IndexFile *deffile,
						UT_CompressionType compressionType);
    /// Utility functions for getting/setting the comment and version data in an
    /// operator type definition index file.
    bool		 getComment(FS_IndexFile *deffile, UT_String &result);
    void		 setComment(FS_IndexFile *deffile, const char *comment);
    bool		 getVersion(FS_IndexFile *deffile, UT_String &result);
    void		 setVersion(FS_IndexFile *deffile, const char *version);

    /// Functions for listing information about libraries and ops.
    void		 listLibraries(std::ostream &os) const;
    void		 listLibrariesWithDefinition(const char *tablename,
						     const char *opname,
						     std::ostream &os) const;
    void		 listOperators(const char *libfile, 
				       std::ostream &os) const;
    void		 listOperator(const char *libfile,
				      const char *tablename,
				      const char *opname,
				      const char *format,
				      std::ostream &os) const;

    /// Function to get a list of the libraries containing a definition for
    /// the specified type.
    void		 getLibrariesWithDefinition(
				    const char *tablename,
				    const char *opname,
				    UT_ValArray<OP_OTLLibrary *> &list);

    /// Get a list of all operators (in tablename/optypenae form) that match
    /// the passed in mask, which may contain wildcards. Before adding a result
    /// to the array, it checks the symbol table to see if the result has
    /// already been found. The tablename/optype string is added to the results
    /// array. If a library pointer is passed in, only that library is searched.
    /// Optypes found in that library do not need to be installed or current in
    /// the Houdini session. If no library is passed in all current definitions
    /// are searched, and all non-current definitions are ignored (so there
    /// can be no confusion about which instance of an optype definition
    /// was found. Returns true if some matching optype was found (even if
    /// that optype wasn't added because of it already being in the symbol
    /// table).
    bool		 getAllMatchingOperators(const char *opmask,
				const OP_OTLLibrary *library,
				bool onlyotlops,
				UT_StringArray &results,
				UT_StringSet &resultstable) const;

    /// Functions for handling our interests.
    void		 addManagerSink(OP_OTLManagerSink *sink);
    void		 removeManagerSink(OP_OTLManagerSink *sink);

    /// Checks if the given table and operator type name are valid.
    bool		 checkValidNodeName(const char *name,
					    OP_OperatorTable *table,
					    UT_WorkBuffer &errors);
    /// Creates a new operator type in an OTL, and installs that OTL.
    bool		 createOptype(OP_OTLDefinition &def,
				      const char *installpath,
				      const char *extradsinfo,
				      const char *installmetasrc,
				      const UT_StringMap<UT_StringHolder> &extrasections,
				      UT_WorkBuffer &errors,
				      bool create_backup=true);


    /// Creates a new operator type in an OTL, and installs that OTL.
    bool		 createOptype(const char *tablename,
				      const char *opname,
				      const char *oplabel,
				      const char *installpath,
				      int min_input, int max_input,
				      int max_outputs,
				      const char *extradsinfo,
				      const char *installmetasrc,
				      const char *extrainfo,
				      bool subnet,
				      UT_WorkBuffer &errors,
				      bool create_backup=true);

    static bool		 getHelpTemplateFile(UT_String &helptemplatefile);
    static void		 createDefaultOptypeName(OP_Node *fromnode,
						 UT_String &name,
						 UT_String &label);
    static void		 createOptypeLabelFromName(UT_String name,
						 UT_String &label);
    static void		 createOptypeNameFromLabel(UT_String label,
						 UT_String &name);
    static void		 fullyQualifyLibFileName(const char *libfilebase,
						 UT_String &libfile,
						 const char *basedir = 0);
    void		 fullyQualifyMetaSrcName(const char *libfilebase,
						 UT_String &libfile) const;
    void		 getSimplifiedPath(UT_String &destpath,
					   const char *srcpath,
					   const char *srcmeta) const;

    bool		 canSavePreferences() const;

    /// Functions for managing fallback libraries.  These are libraries that
    /// are automatically loaded (as a last resort) if a particular type
    /// definition is missing.
    /// 
    /// The manager maintains a stack of fallback contexts.  In general, each
    /// fallback context contains the fallback paths specified in a particular
    /// file fragment (hip file, digital asset contents section) and remains
    /// active until that file fragment is processed by the loading mechanism.
    void		 pushFallbackLibraryContext();
    void		 popFallbackLibraryContext();
    void		 saveFallbackLibraryPaths(std::ostream &os,
					const OP_OperatorList &ops) const;
    bool		 loadFallbackLibraryPaths(UT_IStream &is);
    bool		 loadFallbackLibrary(const char *tablename,
					const char *opname);

    /// Functions for managing the dummy definitions for each operator.
    /// The dummy definition is just a dialog script copied from the last time
    /// the operator was loaded. It is retrieved if no other definition for
    /// the operator is available. It should be enough information to load
    /// and re-save a hip file containing an unknown operator type without
    /// losing any information.
    void		 setDummyDefinition(const OP_OTLDefinition &definition,
					const FS_IndexFile *deffile);
    bool		 getDummyDefinition(const OP_Operator *op,
					FS_IndexFile *deffile) const;
    bool		 loadDummyDefinitions(UT_IStream &is, int modtime);
    void		 saveDummyDefinitions(std::ostream &os,
					const OP_OperatorList &ops) const;

    /// Saves an empty (but valid) dialog script to a stream.
    static void		 saveDefaultDialogScript(std::ostream &os,
					const char *opname,
					const char *oplabel,
					const char *extradsinfo);
    /// Saves a default, do-nothing creation script to a stream.
    static void		 saveDefaultCreationScript(std::ostream &os,
					const char *op,
					const char *table);

    /// Writes the index file 'contents' to the output stream 'os'. Ensures
    /// that the filtering options (ie encryption) are set correctly on the 
    /// index file before it is written out.
    static void		 writeOutDefinitionContents( 
					const OP_OTLDefinition &definition,
					FS_IndexFile & contents,
					std::ostream & os);

    /// Sets the encryption and decryption filters on the contents.
    static void		 setFilters(const OP_OTLDefinition &definition,
					FS_IndexFile &contents);

    /// Reload OTL license files, and notify interested parties of the
    /// potential for change.
    void		 refreshLicenses();

    /// Get the real file name that is presently being used for the Embedded
    /// library. This should be used very carefully, and the value should not
    /// be stored, because it will change anytime a definition is added to or
    /// removed from the Embedded library.
    const UT_String	&getInternalTempFile() const;

    /// Checks if the library has a non-commercial (tainted) asset.
    /// Note, the library does not have to be loaded yet.
    bool		 hasNCAsset(const OP_OTLLibrary *lib);

    /// Returns an array of asset indices that are non-commercial
    /// Note, the library does not have to be loaded yet.
    void		 getNCAssets(const OP_OTLLibrary *lib, 
				     UT_IntArray & nc_assets);

    /// Only called by FUSE_AssetsMenu to clean dirty flags of assets.
    void                 setSaveOpTypeCallback(OP_SaveOpTypeCallback cb)
					      {mySaveOpTypeCallback = cb;}

    /// Convert libraries between different formats
    bool		 convertLibraryToFile(const char *libfilebase,
					      const char *metasrcbase);
    bool		 convertLibraryToDir(const char *libfilebase,
					     const char *metasrcbase);

private:
    struct OTLFallbackData
    {
    public:
	UT_StringArray	myPaths;
	bool		myFailFlag;
    };
    typedef UT_SymbolMap<OTLFallbackData *> FallbackContext; 

    // Helper struct for doing bulk refresh operations
    class RefreshScope;
    friend class RefreshScope;

    // Functions for sending notifications to our interests.
    void		 notifySinksOfAssetCreated(
			    const OP_OTLLibrary &library,
			    const OP_OTLDefinition &def);
    void		 notifySinksOfAssetDeleted(
			    const OP_OTLLibrary &library,
			    const OP_OTLDefinition &def);
    void		 notifySinksOfAssetSaved(
			    const OP_OTLLibrary &library,
			    const OP_OTLDefinition &def);
    void		 notifySinksOfLibraryInstalled(OP_OTLLibrary *library);
    void		 notifySinksOfLibraryUninstalled(
			    OP_OTLLibrary *library);
    void		 notifySinksOfDefinitionsAdded(int libindex,
					UT_IntArray &defarray);
    void		 notifySinksOfDefinitionsRemoved(int libindex,
					UT_IntArray &defarray,
					OP_OTLLibrary *&preserve);
    void		 notifySinksOfSourceNamesChanged();
    void		 notifySinksOfLibraryAdded(OP_OTLLibrary *library);
    void		 notifySinksOfLibraryRemoved(OP_OTLLibrary *library);
    void		 notifySinksOfConfigurationChanged();
    void		 notifySinksOfLicensesChanged();
    void		 writeSimplifiedPath(std::ostream &os,
					const char *srcpath,
					const char *srcmeta,
					const char *prefix = 0) const;

    /// Sets meta source name data for a given meta source.
    void		 setMetaSourceNameData(int pos, const char *metasrc,
					       const char *metasrcname);
    /// Saves an OPlibraries file by looking through our libraries and
    /// writing out any loaded library that uses that meta source. Optionally
    /// we can also specify a single library that should not be written, or
    /// an extra library that should be written.
    bool		 saveMetaFile(const char *metafile,
				      const char *skiplib,
				      const char *addlib) const;
    /// Adds a library to an OPlibraries file, and returns the index of
    /// where in the overall list of libraries the new library should
    /// be inserted. Used by addLibrary.
    int			 addToMetaSource(const char *libfile,
					 const char *metasrc,
					 bool changeoplibraries);
    /// Put a library into our list at a particular position. Used
    /// internally to actually add libraries.
    bool		 insertLibrary(OP_OTLLibrary *newlib, int before);
    /// Internal removeLibrary used to actually remove a library.
    bool		 removeLibrary(int index, bool external=false);
    /// Functions for loading and saving our preferences file.
    void		 loadPreferences();
    void		 savePreferences();
    /// Use this function to create a new temp file for our internal library.
    void		 moveInternalToNewTempFile();
    /// Removes from lib any definitions for which there is already a
    /// non-dummy definition in existance.
    void		 clearIfGoodDefinitionExists(OP_OTLLibrary *lib);
    /// Removes some operator definitions from an existing library file.
    bool		 removeFromExternalLibrary(const char *libfile,
						   const UT_StringArray &ops);
    /// Removes some operator defintions from the internal library, but
    /// only if it is safe to do so (either there is another library
    /// ready to take over the definition, or the opertor is unused).
    bool		 removeFromInternalLibrary(const UT_StringArray &ops);
    /// backup a libfile before replacing it with the new one.  NOTE this
    /// deletes 'newlib' before returning in order to close any file pointers
    bool		 backupAndWriteNewLibrary(const char *libfilebase,
						  const char *libfile,
						  OP_OTLLibrary *newlib);

    /// Helper functions for touchDefinition which set the modification
    /// time for an operator definition in either the internal library or
    /// an external library file.
    bool		 touchExternalDefinition(const char *libfile,
						 const char *tablename,
						 const char *opname,
						 int newtime);
    bool		 touchInternalDefinition(const char *tablename,
						 const char *opname,
						 int newtime);
    /// Adds a library from an OPlibraries file.
    void		 addFromOpLibrariesFile(const char *file,
						const char *metasrc,
						int &libpos);

    /// Private utility method for implementing the public method.
    void		 setLibraryPreference(const char *tablename,
					      const char *opname,
					      const char *libfile,
					      UT_IntArray *libstorefresh);

    /// Some private utility methods for managing the fallback library tables.
    void		 addFallbackLibraryPath(const char *tablename,
						const char *opname,
						const char *libfile);
    bool		 loadFallbackLibrary(FallbackContext *context,
					     const char *opsymbol,
					     const char *tablename,
					     const char *opname);
    void		 clearLibraryFallbacks(FallbackContext *table);

    /// Tests if the definition is for a non-commercial asset.
    bool		 isNCAsset(const FS_IndexFile *lib_file,
				   const OP_OTLDefinition & def);

    /// This static exit callback is here in case we allocate an
    /// OP_OTLManager then never free it (which we do, since we never
    /// delete our OP_Director).
    static void		 deleteLibraries(void *data);

private: // data

    UT_ValArray<OP_OTLLibrary *>	 myLibraries;
    UT_ValArray<OP_OTLManagerSink *>	 myManagerSinks;
    UT_ValArray<OP_OTLLibrary *>	 myLibrariesBeingRemoved;
    UT_String				 myInternalTempFile;
    UT_String				 myNewInternalTempFile;
    UT_StringArray			 myMetaSourcesWithNames;
    UT_StringArray			 myMetaSourceNames;
    UT_StringArray			 myMetaSourceNameWithPaths;
    UT_SymbolMap<UT_StringHolder>	 myLibraryPreferences;
    UT_ValArray<FallbackContext *>	 myLibraryFallbacks;
    UT_UniquePtr<OP_OTLLibrary>		 myDummyDefinitions;
    UT_String				 myOperatorTypeBarMode;
    UT_StringArray			 myFailedOTLs;
    OP_SaveOpTypeCallback                mySaveOpTypeCallback;
    int					 myPerformingRefreshAll;
    bool				 myIsPerformingAtomicAddRemove;
    bool				 mySaveToHipFile;
    bool				 mySaveUnlockedToHipFile;
    bool				 myWarnOutOfDateOps;
    bool				 myWarnDummyOpsOnLoad;
    bool				 myPreferLatestDateDefinition;
    bool				 myPreferInternalDefinition;
    bool				 myPreferIndexFileDefinition;
    bool				 myCreateBackups;
    bool				 mySafeguardOpDefs;
    bool				 myLeaveDefaultsUnchanged;
    bool				 myUseOplibrariesFiles;
    bool				 myIsCreatingNewHDA;

    static UT_ValArray<OP_OTLLoadCallback *>
					 theOTLLoadCallbacks;
};

class OP_API OP_OTLManagerSink
{
public:
		 OP_OTLManagerSink()	{ }
    virtual	~OP_OTLManagerSink()
		 { removeAllManagerSinks(); }

    /// Called when an asset definition is created.
    /// This differs from definitionsAdded() which is called even
    /// when a library is refreshed.
    virtual void	 assetCreated(
			    const OP_OTLLibrary &library, 
			    const OP_OTLDefinition &def) {}

    /// Called when an asset definition is deleted from the library.
    /// This differs from definitionsRemoved() which is called even
    /// when a library is refreshed.
    virtual void	 assetDeleted(
			    const OP_OTLLibrary &library,
			    const OP_OTLDefinition &def) {}

    /// Called when an asset definition is saved.
    virtual void	 assetSaved(
			    const OP_OTLLibrary &library, 
			    const OP_OTLDefinition &def) {}

    /// libraryInstalled() is called after an HDA library is installed.
    /// This differs from libraryAdded() which is called after a library
    /// is added to the manager's library list.  The latter can be called
    /// several times in a session due to a library being installed or updated.
    virtual void	 libraryInstalled(const OP_OTLLibrary *library) {}

    /// libraryUinstalled() is called after an HDA library was uninstalled.
    /// This differs from libraryRemoved() which is called after a library
    /// is removed from the manager's library list.  The latter can be
    /// called several times in a session due to a library being uninstalled
    /// or updated.
    virtual void	 libraryUninstalled(const OP_OTLLibrary *library) {}

    virtual void	 definitionsAdded(int /*libindex*/,
					  UT_IntArray & /*defindex*/)	{ }
    virtual void	 definitionsRemoved(int /*libindex*/,
					    UT_IntArray & /*defindex*/,
					    OP_OTLLibrary *& /*preserve*/){ }
    virtual void	 sourceNamesChanged()				{ }
    virtual void	 libraryAdded(OP_OTLLibrary * /*library*/)	{ }
    virtual void	 libraryRemoved(OP_OTLLibrary * /*library*/)	{ }
    virtual void	 configurationChanged()				{ }
    virtual void	 licensesChanged()				{ }

    void		 managerDeleted(OP_OTLManager *manager)
			 { removeManagerSink(manager); }

protected:
    void	 addManagerSink(OP_OTLManager *manager)
		 {
		     if( !manager ) return;

		     // Don't add if we have already added ourselves
		     // to the manager.
		     if (myManagers.find(manager) != -1)
			return;

		     manager->addManagerSink(this);
		     myManagers.append(manager, 1);
		 }
    void	 removeManagerSink(OP_OTLManager *manager)
		 {
		     if( !manager ) return;
		     manager->removeManagerSink(this);
		     myManagers.findAndRemove(manager);
		 }
    void	 removeAllManagerSinks()
		 {
		     for( int i = myManagers.entries(); i --> 0; )
			 removeManagerSink(myManagers(i));
		 }

private:
    UT_ValArray<OP_OTLManager *>	 myManagers;
};

#endif

