/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_FileResolver.h (FS  Library, C++)
 *
 * COMMENTS:	This class resolves the data specified by its name. The name can
 *		a file path or an "op:" path. 
 *
 *		In applications that can cook the op data (eg, Houdini and
 *		Batch), the resolver instance actually searches out the node and
 *		asks it for the data, then it creates a stream and returns it as
 *		a file reader object. To release this temporarily created
 *		stream, the requester needs to let the resolver know so that
 *		it can relinquish it.
 *
 *		In applications that don't have access to nodes (eg, Mantra),
 *		the resolver instance needs to rely on the data given to
 *		it (eg, inside the IFD stream). Since other entities (eg,
 *		shaders) still refer to the data as "op:", the resolver
 *		needs to figure out which data to return for each request.
 *		The data is returned in a form of FS_Reader, but the actual
 *		data may be in memory, in a temp file on disk, or elsewhere.
 *		The requester may notify the resolver that it is done with the
 *		data and to relinquish it, but the resolver will probably need
 *		to keep the data around for the future requests, since there is
 *		no source for that data (no nodes, and IFD has been already
 *		sourced before).
 *
 */

#ifndef __FS_FileResolver__
#define __FS_FileResolver__

#include "FS_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SymbolTable.h>

class UT_IStream;
class FS_Reader;


//============================================================================= 
/// Provides various options for creating and obtaining the file data.
class FS_API FS_FileResolverOptions
{
public:
    /// Constructor.
     FS_FileResolverOptions();
     FS_FileResolverOptions( const FS_FileResolverOptions & src );

    /// Assignment operator.
    const FS_FileResolverOptions &  operator=( 
					const FS_FileResolverOptions & src );

    /// Gets the time at which a node should be evaluated to obtain the data.
    fpreal			getTime() const
				{ return myTime; }
    void			setTime( fpreal time )
				{ myTime = time; }

    /// Gets the extension for the file that will contain the resolved data.
    /// It indirectly determines the data type for stream readers and writers.
    const UT_StringHolder	&getFileExtension() const
				{ return myFileExtension; }
    void			setFileExtension(const UT_StringHolder &ext) 
				{ myFileExtension = ext; }

    /// Checks if the file should be treated as an environment map.
    bool			getIsEnvMap() const
				{ return myIsEnvMap; }
    void			setIsEnvMap( bool env_map )
				{ myIsEnvMap = env_map; }

   
private:
    UT_StringHolder	myFileExtension;    // extension of the data file
    fpreal		myTime;		    // node evaluation time
    bool		myIsEnvMap;	    // is environment map
};


//============================================================================= 
/// Maps the operator paths (eg, "op:/path"), and other data sources,
/// to file readers that access their data.
class FS_API FS_FileResolver
{
public:
    /// Constructor.
				 FS_FileResolver();
    /// Destructor.
    virtual			~FS_FileResolver();

    /// Sets the instance that will resolve the operator paths to data files.
    static void			setResolver( FS_FileResolver * resolver );

    /// Returns the currently set resolver.
    static FS_FileResolver *	getResolver();


    /// Checks if the resolver can provide the file data for the given name.
    // TODO: this method may need to take FS_FileResolverOptions argument
    virtual bool		getCanResolve( const char * name ) = 0;

    /// @brief Creates a new reader object given the file (or data) name. 
    ///
    /// The returned reader can be used to read the data, and when finished,
    /// it should be freed by passing it to the FS_FileResolver::deleteReader()
    /// method.
    ///
    /// @param	opts	The options that indicate how the data should be
    ///			generated, if applicable. Eg, it contains the evaluation
    ///			time for nodes or the file format in which the data
    ///			should be available.
    ///
    /// @param	name	The name of the data to be read. Can be a file
    ///			specified with "file:/path" or an operator node
    ///			data specified with "op:/path", or any other standard
    ///			way of specifying data in Houdini.
    //
    /// @return		The reader for the specified data.
    ///
    virtual FS_Reader *		newReader( const FS_FileResolverOptions & opts,
					   const char * name ) = 0;


    /// Closes stream (if they are still open) and deletes the reader. 
    /// After this call, the reader pointer will no longer be valid.
    /// 
    /// @param	reader	The reader to be freed. 
    ///
    virtual void 		deleteReader( FS_Reader * reader ) = 0;


    /// Obtains the file path to the data of a given name. On success,
    /// 'file_path' will contain a non-epnty file path. This file path
    /// should be then released using FS_FileResolver::relinquishFilePath().
    ///
    /// @param opts	The options that describe the data.
    /// @param name	The name of the data available in the file.
    /// @param file_path    The output parameter which will have the
    ///			path of the file that contains the data. If
    ///			the request fails, the path will be an empty string.
    virtual void		acquireFilePath( 
					    const FS_FileResolverOptions & opts,
					    const char * name,
					    UT_String & file_path ) = 0;

    /// Releases the file path and allows the resolver to (optionally) free
    /// the file resources.
    /// 
    /// @param	file_path   The file path previously obtained with
    ///			    FS_FileResolver::acquireFilePath().
    virtual void		relinquishFilePath( const char * file_path ) = 0;

    /// Adds the data to the resolver, so that the resolver can handle
    /// future requests for the data given this name. 
    /// 
    /// @param	opts	Options describe how the data should be interpreted.
    ///
    /// @param	name	The name of the resource. Future requests for this
    ///			resource using FS_FileResolver::newReader() will returno
    ///			a reader that provides this data.
    ///
    /// @param	data	The stream containing the data. The resolver will
    ///			read in and store this data for future use.
    virtual void		registerFile(const FS_FileResolverOptions &opts,
					     const UT_StringHolder &name,
					     UT_IStream & data );

    /// Returns an instance-unique identifier for a path that can be used to
    /// check if the underlying resource data has changed, and should be 
    /// re-read by the clients of this class.
    
    /// @param name	The name of the data to get the identifier for.
    /// 
    /// @return		The revision id of the data. Returns 0 if the resource
    ///			does not exist or revision IDs are not supported.
    virtual int		getRevisionID(const char *name) = 0;
protected:

    /// Contains details about the data that the resolver can provide.
    class FS_API FS_ResolverFileEntry
    {
    public:
	// constructor
	FS_ResolverFileEntry(const UT_StringHolder &name,
			const UT_StringHolder &file_name,
			const FS_FileResolverOptions &opts);

	// Accessors.
	const UT_StringHolder	&getName() const { return myName; }
	const UT_StringHolder	&getFileName() const { return myFileName; }

	const FS_FileResolverOptions	&getOptions() const
						{ return myOptions; }

    private:
	/// Name of the resource this entry represents.
	UT_StringHolder 	myName;

	/// The file name in which the resource data is saved.
	UT_StringHolder 	myFileName;

	/// Options with which the file has been crated.
	FS_FileResolverOptions	myOptions;
    };


    /// Adds the file to the table.
    /// The resolver takes ownership of the file and will delete it
    /// in the destructor, unless it is unregistered from the table beforehand,
    /// with FS_FileResolver::unregisterFile().
    void			registerFile( 
					const FS_FileResolverOptions &opts,
					const UT_StringHolder &name,
					const UT_StringHolder &file_path );

    /// Removes the file from table, and deletes the file if 'delete_file' is
    /// true.
    void			unregisterFile( const UT_StringRef &name,
						bool delete_file );

    /// Looks up the table entry for the given name key and returns the
    /// entry. If not found, returns NULL.
    FS_ResolverFileEntry *	findFileInTable( const char * name );
    
private:
    /// Adds the file to the table.
    void			addFileToTable(const UT_StringHolder &name,
						FS_ResolverFileEntry *entry);

    /// Removes the file entry from the table.  If 'delete_file' is true,
    /// the file on disk is also deleted.
    void			removeFileFromTable(const UT_StringRef &name,
						     bool delete_file);
						

    /// Deletes the entry and any temporary files that hold the resource data.
    /// If 'delete_file' is true, the file on disk is also deleted.
    void			deleteEntry(FS_ResolverFileEntry *entry,
					     bool delete_file );


private:
    /// The current resolver.
    static FS_FileResolver	*theResolver;

    /// The table that maps the data name to the file containing that data.
    UT_SymbolMap<FS_ResolverFileEntry *> myFileTable;
};


//============================================================================= 
/// Utility class for auto destruction and cleanup of the file resolver. 
class FS_API FS_AutoFileResolverDestroyer
{
public:
    /// Constructor
     FS_AutoFileResolverDestroyer( FS_FileResolver * resolver );

    /// Destructor. Deletes the resolver it holds (if not NULL), and if
    /// it is the same as the current global resolver, it resets it to NULL.
    ~FS_AutoFileResolverDestroyer();

    /// Accessor for the resolver it holds.
    FS_FileResolver *	    getResolver() const
			    { return myResolver; }

    /// Sets the resolver it cleans up after.
    void		    setResolver( FS_FileResolver * resolver ) 
			    { myResolver = resolver; }

    /// Destroys the resolver right away rather than waiting for the
    /// destructor to be invoked.
    void		    destroyResolver();

private:
    // the resolver to clean up after
    FS_FileResolver *	    myResolver;
			    
};

#endif

