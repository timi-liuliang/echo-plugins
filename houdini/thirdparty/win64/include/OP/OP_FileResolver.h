/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_FileResolver.h ( OP Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_FileResolver__
#define __OP_FileResolver__

#include "OP_API.h"

#include "OP_Context.h"

#include <UT/UT_StringMap.h>
#include <FS/FS_FileResolver.h>

class FS_Writer;
class OP_Node;


/// Cooks node data and makes it available through the reader.
class OP_API OP_FileResolver : public FS_FileResolver
{
public:
    /// Constructor.
    OP_FileResolver();

    /// Checks if the resolver can provide the file data for the given name.
    virtual bool		getCanResolve( const char * name );

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
					   const char * name );

    /// Closes stream (if they are still open) and deletes the reader. 
    /// After this call, the reader pointer will no longer be valid.
    /// 
    /// @param	reader	The reader to be freed. 
    ///
    virtual void 		deleteReader( FS_Reader * reader );

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
					    UT_String & file_path );

    /// Releases the file path and allows the resolver to (optionally) free
    /// the file resources.
    /// 
    /// @param	file_path   The file path previously obtained with
    ///			    FS_FileResolver::acquireFilePath().
    virtual void		relinquishFilePath( const char * file_path );


    /// Returns an instance-unique identifier for a path that can be used to
    /// check if the underlying resource data has changed, and should be
    /// re-read by the clients of this class.

    /// @param name	The name of the data to get the identifier for.
    ///
    /// @return		The revision id of the data. Returns 0 if the resource
    ///			does not exist or revision IDs are not supported.
    virtual int		getRevisionID(const char *name);
private:
    /// Saves the node data to a file according to the given specifications 
    /// in the options.  This class stores an FS_Writer.  If no FS_Reader is
    /// created from the filename, then the temporary file may be deleted.
    class NodeSaver
    {
    public:
	 NodeSaver() {}
	~NodeSaver();

	const UT_StringHolder	&filename() const { return myFilename; }
	bool			 saveNode(const FS_FileResolverOptions &opts,
					const char *name);
    private:
	UT_StringHolder	 myFilename;
    };

private:
    /// A map from issued reader pointers or from issued file paths to
    /// the names used for registering the resource with the base class.
    UT_Map<FS_Reader *, UT_StringHolder>	myRegisteredReaderFilenameMap;
    UT_StringMap<UT_StringHolder>		myRegisteredPathFilenameMap;
};



/// Used to control how the node data is being saved when the file resolver
/// requests a cook.
class OP_API OP_FileResolverContextData : public OP_ContextData
{
public:
    /// Constructor.
				OP_FileResolverContextData()
				    :	myIsEnvMap( false )
				    {}

    /// Indicates whether the COP should save an image as an environment map.
    bool			getIsEnvMap() const
				    { return myIsEnvMap; }
    void			setIsEnvMap( bool env_map )
				    { myIsEnvMap = env_map; }

private:
    /// Flag: save image as an environment map.
    bool			myIsEnvMap;
};

#endif

