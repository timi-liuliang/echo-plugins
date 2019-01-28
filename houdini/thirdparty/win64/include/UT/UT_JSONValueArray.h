/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONValueArray.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_JSONValueArray__
#define __UT_JSONValueArray__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_StringArray.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Inline.h>

class UT_JSONValue;
class UT_JSONWriter;
class UT_JSONValueMap;

/// @brief UT_JSONValueArray stores a list of UT_JSONValue objects
///
/// UT_JSONValueArray stores a list of UT_JSONValue objects.
///
/// @see UT_JSONValue, UT_JSONValueMap
class UT_API UT_JSONValueArray {
public:
     UT_JSONValueArray();
    ~UT_JSONValueArray();

    /// Reference count (for shared arrays).
    void		 bumpRef(int d)
			 {
			     if (!myRef.add(d))
				 delete this;
			 }
    /// Get the number of references to this array
    SYS_FORCE_INLINE int getRef() const	{ return myRef.relaxedLoad(); }

    /// Save to an output stream
    bool		 save(UT_JSONWriter &os) const;

    /// Dump to stdout (same as save to std::cout)
    void		 dump() const;

    /// Return size of the array
    SYS_FORCE_INLINE int64 size() const		{ return myArray.size(); }
    /// Return the number of entries in the array
    SYS_FORCE_INLINE int64 entries() const	{ return myArray.size(); }
    /// Access a const entry by index
    SYS_FORCE_INLINE const UT_JSONValue *operator()(int64 i) const { return myArray[i]; }
    /// Access an entry by index
    SYS_FORCE_INLINE UT_JSONValue *operator()(int64 i) { return myArray[i]; }
    /// Access a const entry by index
    SYS_FORCE_INLINE const UT_JSONValue *operator[](int64 i) const { return myArray[i]; }
    /// Access an entry by index
    SYS_FORCE_INLINE UT_JSONValue *operator[](int64 i) { return myArray[i]; }

    /// Access a const entry by index
    SYS_FORCE_INLINE const UT_JSONValue *get(int64 i) const { return myArray[i]; }
    /// Access an entry by index
    SYS_FORCE_INLINE UT_JSONValue *get(int64 i) { return myArray[i]; }

    /// Import an array of integer values (fails if the array size is too small
    /// or if one of the elements of the array cannot be interpreted as an
    /// integer).
    bool		import(int64 *result, int64 size) const;
    /// Import an array of float values (fails if the array size is too small
    /// or if one of the elements of the array cannot be interpreted as an
    /// integer).
    bool		import(fpreal64 *result, int64 size) const;

    /// Import an array of integer values (fails if the elements of the array
    /// cannot be interpreted as an integer).
    bool		import(UT_Array<int64> &array) const;
    /// Import an array of real values (fails if the elements of the array
    /// cannot be interpreted as such).
    bool		import(UT_Array<fpreal64> &array) const;
    /// Import an array of string values (fails if the elements of the array
    /// cannot be interpreted as such).
    bool		import(UT_StringArray &array) const;

    /// Add an element to the array
    int64		 append(UT_JSONValue *v) { return myArray.append(v); }

    /// Remove and delete an element by index.
    void		 remove(int64 index);

    /// Remove and delete an element.
    void		 remove(UT_JSONValue *v);

    /// Remove and delete the last added element.
    void		 removeLast();

    /// Remove and delete all elements.
    void		 clear();

    /// Inserts a value v before the given element after_value.
    void		 insertBefore(UT_JSONValue* v, UT_JSONValue* after_value);

    /// Adds a new array child to this map.
    UT_JSONValueArray	*appendArrayChild();

    /// Adds a new map child to this map.
    UT_JSONValueMap	*appendMapChild();

private:
    template <typename ARRAY_T>
    bool		 importArray(ARRAY_T &array) const;

private:
    UT_Array<UT_JSONValue *>	myArray;
    SYS_AtomicInt32 		myRef;
};

#endif
