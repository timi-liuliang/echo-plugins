/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONValueMap.h ( UT Library, C++)
 *
 * COMMENTS:	The order is maintained as objects are added to the map
 */

#ifndef __UT_JSONValueMap__
#define __UT_JSONValueMap__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_ArrayStringMap.h"
#include "UT_StringHolder.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>

class UT_JSONValue;
class UT_JSONValueArray;
class UT_JSONWriter;
class UT_StringArray;

/// @brief UT_JSONValueMap stores a map/dictionary of UT_JSONValue objects
///
/// UT_JSONValueMap stores a map of UT_JSONValue objects.  Unlike a standard
/// Houdini UT_StringMap, this object maintains the sort order in which
/// objects were added.
///
/// @see UT_JSONValue, UT_JSONValueArray
class UT_API UT_JSONValueMap {
public:
     UT_JSONValueMap();
    ~UT_JSONValueMap();

    /// Reference count (for shared maps).
    void		 bumpRef(int d)
			 {
			     if (!myRef.add(d))
				 delete this;
			 }
    /// Get the number of references to this map.
    int			 getRef() const	{ return myRef.relaxedLoad(); }

    /// Save to an output stream
    bool		 save(UT_JSONWriter &os) const;

    /// Dump to stdout (same as save to std::cout)
    void		 dump() const;

    /// Get the list of keys
    int64		 getKeys(UT_StringArray &keys) const;

    /// Get a list of the keys.  However, rather than duplicating the string
    /// values, this method simply keeps shallow references.  While it's more
    /// efficient, it's also relies on the fact that the map will not change
    /// while the string array exists.
    int64		 getKeyReferences(UT_StringArray &keys) const;

    /// Return the number of entries in the map
    int64		 entries() const
			 { return myArray.entries(); }

    /// Clear the entries in the map.
    void		 clear();

    /// Access const entry by index
    const UT_JSONValue	*operator()(int64 i) const
			 { return get(i); }
    /// Access entry by index
    UT_JSONValue	*operator()(int64 i)
			 { return get(i); }
    /// Access const entry by name (may be NULL pointer if k is not a key)
    const UT_JSONValue	*operator()(const UT_StringRef &k) const
			 { return get(k); }
    /// Access entry by name (may be NULL pointer if k is not a key)
    UT_JSONValue	*operator()(const UT_StringRef &k)
			 { return get(k); }

    /// Access const entry by index
    const UT_JSONValue	*operator[](int64 i) const
			 { return get(i); }
    /// Access entry by index
    UT_JSONValue	*operator[](int64 i)
			 { return get(i); }
    /// Access const entry by name (may be NULL pointer if k is not a key)
    const UT_JSONValue	*operator[](const UT_StringRef &k) const
			 { return get(k); }
    /// Access entry by name (may be NULL pointer if k is not a key)
    UT_JSONValue	*operator[](const UT_StringRef &k)
			 { return get(k); }

    /// Access a const entry by index
    const UT_JSONValue	*get(int64 i) const
			 { return myArray(i); }
    /// Access an entry by index
    UT_JSONValue	*get(int64 i)
			 { return myArray(i); }
    /// Access const entry by name (may be NULL pointer if k is not a key)
    const UT_JSONValue	*get(const UT_StringRef &k) const;
    /// Access entry by name (may be NULL pointer if k is not a key)
    UT_JSONValue	*get(const UT_StringRef &k);

    /// Add key/value pair to the map.  If the key already exists, the previous
    /// value will be deleted.
    int64		 append(const UT_StringHolder &key, UT_JSONValue *v);

    /// Remove key/value pair from the map.
    /// Do nothing if the key does not exist.
    void		 remove(const UT_StringHolder &key);

    /// Rename a key in the map.
    /// Do nothing if the old key does not exist.
    /// If the old key exists and another entry uses the new key
    /// then remove the other entry and rename the entry with the old key.
    void		 rename(
			    const UT_StringHolder &old_key, 
			    const UT_StringHolder &new_key);

    /// Adds a new array child to this map.
    UT_JSONValueArray	*addArrayChild(const UT_StringHolder &map_key);

    /// Adds a new map child to this map.
    UT_JSONValueMap	*addMapChild(const UT_StringHolder &map_key);

    /// Key/Value imports.  This is implemented for T in:
    ///	- bool
    /// - int64
    /// - fpreal64
    /// - UT_WorkBuffer/UT_StringHolder
    /// - UT_Int64Array
    /// - UT_Fpreal64Array
    /// Returns false if the key isn't in the map, or if the import from the
    /// UT_JSONValue fails.
    template <typename T>
    bool	import(const UT_StringRef &key, T &result) const;

private:
    UT_Array<UT_JSONValue *>		 myArray;
    UT_ArrayStringMap<UT_JSONValue *>	 myMap;
    SYS_AtomicInt32			 myRef;
};

#endif

