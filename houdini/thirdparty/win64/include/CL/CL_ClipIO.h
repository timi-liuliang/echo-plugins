/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        CL_ClipIO.h
 *
 * COMMENTS:
 *	This singleton object stores a set of clip reader and clip writer
 *	objects.  When told to read or write a clip file, it choses which
 *	reader or writer to use based on the file extension.
 */

#ifndef __CL_ClipIO_h__
#define __CL_ClipIO_h__

#include "CL_API.h"
#include <UT/UT_IndexedHashMapT.h>
class CL_Clip;
class CL_ClipReader;
class CL_ClipWriter;

class CL_API CL_ClipIO
{
public:
    // Use this static method to access the clipIO singleton object.
    static CL_ClipIO &inst();

    // These methods read a clip from a file or write a clip to a file.
    // If the clip could not be read/written, false will be returned.
    bool readClip(const char *file_name, CL_Clip &clip);
    bool writeClip(const char *file_name, const CL_Clip &clip);

    /// Returns true if the file extension is supported. The input can be the
    /// full file path.
    bool canReadFileType(const char *file_name);
    bool canWriteFileType(const char *file_name);

    // These methods return information about which file extensions are
    // supported for reading and writing.
    int getNumReadableExtensions() const
	{ return myClipReaders.entries(); }
    const char *getReadableExtension(int index) const
	{ return myClipReaders[index]->getKey().getName(); }

    int getNumWritableExtensions() const
	{ return myClipWriters.entries(); }
    const char *getWritableExtension(int index) const
	{ return myClipWriters[index]->getKey().getName(); }

    // Clip readers and writers register themselves by calling these methods.
    // This class assumes that readers and writers are never deleted.  Note
    // that the extension begins with a dot, like it does in UT_IOTable.
    void registerClipReader(const char *extension,
			    CL_ClipReader &clip_reader);
    void registerClipWriter(const char *extension,
			    CL_ClipWriter &clip_writer);

protected:

    CL_ClipReader	*findReader(const char *name) const
			 {
			     MapItem	*item=myClipReaders.find(MapKey(name));
			     return item ? item->getKey().getReader() : NULL;
			 }
    CL_ClipWriter	*findWriter(const char *name) const
			 {
			     MapItem	*item=myClipWriters.find(MapKey(name));
			     return item ? item->getKey().getWriter() : NULL;
			 }

private:
    CL_ClipIO();
    void registerClipHandlers();

    // Find a CL_ClipReader for the path, searching through compound extensions
    // (e.g. .clip.gz) from longest to shortest.
    CL_ClipReader *findReaderForPath(const char *path) const;

    CL_ClipWriter *findWriterForPath(const char *path) const;

    class MapKey
    {
    public:
	MapKey(const char *name)
	{
	    init(name, NULL, NULL);
	}
	MapKey(const char *name, CL_ClipReader &reader)
	{
	    init(name, &reader, NULL);
	}
	MapKey(const char *name, CL_ClipWriter &writer)
	{
	    init(name, NULL, &writer);
	}
	MapKey(const MapKey &k)
	{
	    init(k.myName, k.myReader, k.myWriter);
	    myName.harden();	// Hard copy
	}

	void	init(const char *name, CL_ClipReader *r, CL_ClipWriter *w)
		{
		    myName = name;	// Shallow copy
		    myReader = r;
		    myWriter = w;
		}

	const char	*getName() const	{ return myName; }
	CL_ClipReader	*getReader() const	{ return myReader; }
	CL_ClipWriter	*getWriter() const	{ return myWriter; }

	uint		hash() const	{ return myName.hash(); }
	bool		isEqual(const MapKey &key) const
				{ return myName == key.myName; }
	bool		operator<(const MapKey &key) const
				{ return strcmp(myName, key.myName) < 0; }

    private:
	UT_String	 myName;
	CL_ClipReader	*myReader;
	CL_ClipWriter	*myWriter;
    };
    typedef UT_IndexedHashMapItemKeyContainer<MapKey>		MapItem;
    typedef ut_IndexedHashMapDeferItemAlloc<MapKey, MapItem>	MapAlloc;

    UT_IndexedHashMapT<MapKey, MapItem, MapAlloc>	myClipReaders;
    UT_IndexedHashMapT<MapKey, MapItem, MapAlloc>	myClipWriters;
};

// These functions are called from shared objects/dll's to register readers
// and writers.
extern "C" {
    SYS_VISIBILITY_EXPORT extern void CLregisterClipReader(void *);
    SYS_VISIBILITY_EXPORT extern void CLregisterClipWriter(void *);
}

#endif
