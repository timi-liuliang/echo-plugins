/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Serialization.h (UT Library, C++)
 *
 * COMMENTS:	Generic class serialization
 *
 */

#ifndef __UT_SERIALIZATION_H_INCLUDED__
#define __UT_SERIALIZATION_H_INCLUDED__

/// @file
/// Generic class serialization support
///
/// Support structure for generic class serialization.If you just want to
/// save/load to/from classes that already have serialize() methods, then see
/// UT_JSONArchive.h.
///
/// To add support for serializing your data, you have two choices for
/// implementation:
///
///	-# Implement instrusive serialize() class methods. eg.
///	    @code
///		template <typename ARCHIVER_T>
///		bool LIB_MyClass::serialize(ARCHIVER_T &archiver)
///		{
///		    using namespace std;
///
///		    // serialize an unordered map of key value pairs
///		    if (!archiver(make_tuple(
///				    make_tuple("key1", myValue),
///				    make_tuple("key2", mySpecificRefArray)
///				    /* ... etc ... */
///				    )))
///			return false;
///		}
///	    @endcode
///	    @note You can just declare the template function in your header and
///	    only implement it in the .C file if you're always going to
///	    load/save within your class.
///
///	-# Partially specialize UTserialize() to serialize your data. eg.
///	    @code
///		template <typename ARCHIVER_T>
///		bool
///		UTserialize(ARCHIVER_T &archiver, LIB_MyClass &obj)
///		{
///		    using namespace std;
///
///		    // serialize an unordered map of key value pairs
///		    if (!archiver(make_tuple(
///				    make_tuple("key1", myValue),
///				    make_tuple("key2", mySpecificRefArray)
///				    /* ... etc ... */
///				    )))
///			return false;
///		}
///	    @endcode
///
/// To implement serialization for custom types that need different handling
/// between saving and loading, you specialize UTserializeSave/UTserializeLoad
/// instead. For member serialize() methods, you can also call
/// archiver.isLoading() or archiver.isSaving().
///
/// To create a new archive type for serialization to a specific format,
/// see UT_JSONArchive.h as an example. There are some requirements:
///   - Create an archive class like UT_JSONArchive. This provides a partially
///     specialized class of UT_ArchiveManip<T,Archiver>.
///   - Your Archiver class should have two nested classes within it:
///     - class Input : public UT_LoadArchiver<Input>
///     - class Output : public UT_SaveArchiver<Output>
///   - In turn, your Input/Output archiver classes need the following methods:
///     - serializeValue<T>(): serialize a value of a POD type T.
///	- serializeStringValue(): serialize a C string
///     - serializeMapBegin()/serializeMapEnd(): begin/end serialization for an
///       unordered collection of key value pairs. As an archiver implementer,
///       you're free to ignore the unordered requirement if you want but the
///       client serialize code is already setup to support it.
///	   bool serializeKey(UT_WorkBuffer &key)
///     - serializeArrayBegin()/serializeArrayEnd(): begin/end serialization for
///       an ordered sequence of values.
///	- serializeUniformArray(): complete serialization of an ordered
///	  sequence of POD type values.
///


#include "UT_IStream.h"
#include "UT_OStream.h"
#include "UT_Array.h"
#include "UT_String.h"
#include "UT_StringHolder.h"
#include "UT_TupleUtil.h"
#include "UT_ValArray.h"
#include "UT_WorkBuffer.h"
#include <SYS/SYS_Types.h>

#include <tuple>
#include <type_traits>
#include <utility>

#include <stddef.h>
#include <string.h>


/// Default function for non-instrusive serialization of classes.
/// Specialize this when you can perform the same code using the archiver for
/// serialization. If it isn't specialized, then it will call the intrusive
/// version, which you must supply.
template <typename ARCHIVER_T, typename OBJ_T>
bool
UTserialize(ARCHIVER_T &archiver, OBJ_T &obj)
{
    return obj.serialize(archiver);
}

/// Specialize UTserializeSave/UTserializeLoad for serialization when
/// saving/loading needs to be different. By default, they just forward to the
/// regular UTserialize() method.
// @{
template <typename ARCHIVER_T, typename OBJ_T>
bool
UTserializeSave(ARCHIVER_T &archiver, OBJ_T &obj)
{
    if (!UTserialize(archiver, obj))
	return false;
    return true;
}
template <typename ARCHIVER_T, typename OBJ_T>
bool
UTserializeLoad(ARCHIVER_T &archiver, OBJ_T &obj)
{
    if (!UTserialize(archiver, obj))
	return false;
    return true;
}
// @}

/// Alternatively, specialize these template structs when saving/loading need
/// to be different.  By default, they just forward to the functional forms.
/// This form exists to make the templating easier for arrays.
// @{
template <typename OBJ_T>
struct UT_SaveSerializable
{
    template <typename ARCHIVER_T>
    static bool invoke(ARCHIVER_T &archiver, OBJ_T &obj)
    {
	return UTserializeSave(archiver, obj);
    }
};
template <typename OBJ_T>
struct UT_LoadSerializable
{
    template <typename ARCHIVER_T>
    static bool invoke(ARCHIVER_T &archiver, OBJ_T &obj)
    {
	return UTserializeLoad(archiver, obj);
    }
};
// @}

// Generic serializables for basic types
#define UT_DECLARE_SERIALIZABLE(OBJ_T) \
	    template <typename ARCHIVER_T> \
	    bool UTserialize(ARCHIVER_T &archiver, OBJ_T &obj) \
	    { \
		return archiver.serializeValue(obj); \
	    } \
	    /**/
    UT_DECLARE_SERIALIZABLE(bool);
    UT_DECLARE_SERIALIZABLE(int8);
    UT_DECLARE_SERIALIZABLE(int16);
    UT_DECLARE_SERIALIZABLE(int32);
    UT_DECLARE_SERIALIZABLE(int64);
    UT_DECLARE_SERIALIZABLE(fpreal16);
    UT_DECLARE_SERIALIZABLE(fpreal32);
    UT_DECLARE_SERIALIZABLE(fpreal64);
#undef UT_DECLARE_SERIALIZABLE

// UT_String and UT_WorkBuffer serializables
template <typename ARCHIVER_T>
bool
UTserializeSave(ARCHIVER_T &archiver, UT_String &str)
{
    return archiver.serializeStringValue(str);
}
template <typename ARCHIVER_T>
bool
UTserializeLoad(ARCHIVER_T &archiver, UT_String &str)
{
    UT_WorkBuffer buf;
    if (!archiver.serializeStringValue(buf))
	return false;
    str.harden(buf.buffer());
    return true;
}
template <typename ARCHIVER_T>
bool
UTserializeSave(ARCHIVER_T &archiver, UT_WorkBuffer &str)
{
    return archiver.serializeStringValue(str.buffer());
};
template <typename ARCHIVER_T>
bool
UTserializeLoad(ARCHIVER_T &archiver, UT_WorkBuffer &str)
{
    return archiver.serializeStringValue(str);
}
template <typename ARCHIVER_T>
bool
UTserializeSave(ARCHIVER_T &archiver, UT_StringHolder &str)
{
    return archiver.serializeStringValue(str.c_str());
}
template <typename ARCHIVER_T>
bool
UTserializeLoad(ARCHIVER_T &archiver, UT_StringHolder &str)
{
    UT_WorkBuffer	buf;
    if (!archiver.serializeStringValue(buf))
	return false;
    str = buf.buffer();
    return true;
}


/// Array<T> serializable. This saves out an ordered sequence of type T's as
/// opposed to unordered maps. By design, we impose a schema where such arrays
/// always include the array length as the first element.
template <typename T, template <typename> class ARRAY>
struct UT_ArraySaveSerializable
{
    template <typename ARCHIVER_T>
    static bool invoke(ARCHIVER_T &archiver, ARRAY<T> &vec)
    {
	if (!archiver.serializeArrayBegin(vec.entries()))
	    return false;
	if (!archiver.serializeArray(vec.array(), vec.entries()))
	    return false;
	if (!archiver.serializeArrayEnd())
	    return false;
	return true;
    }
};
template <typename T, template <typename> class ARRAY>
struct UT_ArrayLoadSerializable
{
    template <typename ARCHIVER_T>
    static bool invoke(ARCHIVER_T &archiver, ARRAY<T> &vec)
    {
	int64 count;
	if (!archiver.serializeArrayBegin(count))
	    return false;
	vec.setSize(count);
	if (!archiver.serializeArray(vec.array(), vec.entries()))
	    return false;
	if (!archiver.serializeArrayEnd())
	    return false;
	return true;
    }
};
#define UT_DECLARE_ARRAY_SERIALIZABLE(ARRAY_T) \
	    template <typename T> \
	    struct UT_SaveSerializable< ARRAY_T<T> > \
		: public UT_ArraySaveSerializable<T, ARRAY_T> { }; \
	    template <typename T> \
	    struct UT_LoadSerializable< ARRAY_T<T> > \
		: public UT_ArrayLoadSerializable<T, ARRAY_T> { }; \
	    /**/
    UT_DECLARE_ARRAY_SERIALIZABLE(UT_Array)
    UT_DECLARE_ARRAY_SERIALIZABLE(UT_ValArray)
#undef UT_DECLARE_ARRAY_SERIALIZABLE

/// Base class for archivers.
// @{
template <typename ARCHIVER_T>
class UT_SaveArchiver
{
private:
    class SaveKeyValue
    {
    public:
	SaveKeyValue(ARCHIVER_T &archiver)
	    : myArchiver(archiver)
	{
	}

	template <typename PAIR_T>
	bool operator()(const PAIR_T &p)
	{
	    using namespace std;
	    typedef typename tuple_element<1,PAIR_T>::type	ref_type;
	    typedef typename decay<ref_type>::type		obj_type;

	    if (!myArchiver.serializeKey(get<0>(p)))
		return true;
	    if (!UT_SaveSerializable<obj_type>::invoke(myArchiver, get<1>(p)))
		return true;
	    // return false to continue saving
	    return false;
	}

    private:
	ARCHIVER_T &	myArchiver;
    };

public:
    bool isLoading() const	{ return false; }
    bool isSaving() const	{ return true; }

    template <typename T>
    bool serializeArray(T *vec, int64 count)
    {
	ARCHIVER_T *subclass = static_cast<ARCHIVER_T *>(this);
	for (int64 i = 0; i < count; i++)
	{
	    if (!UT_SaveSerializable<T>::invoke(*subclass, vec[i]))
		return false;
	}
	return true;
    }

    /// Called by user serialize() methods.
    /// @param pair_seq A sequence of <key_type, value_type &> sequences
    ///			where key_type is const char *.
    /// @note key literals should in all LOWERCASE
    template <typename SEQ_T>
    bool operator()(const SEQ_T &pair_seq)
    {
	ARCHIVER_T *subclass = static_cast<ARCHIVER_T *>(this);
	if (!subclass->serializeMapBegin())
	    return false;
	SaveKeyValue saver(*subclass);
	if (UTtupleAnyOf(pair_seq, saver))
	    return false;
	if (!subclass->serializeMapEnd())
	    return false;
	return true;
    }

};

template <typename ARCHIVER_T>
class UT_LoadArchiver
{
private:
    class LoadKeyValue
    {
    public:
	LoadKeyValue(ARCHIVER_T &archiver, const char *key, bool &ok)
	    : myArchiver(archiver)
	    , myKey(key)
	    , myOk(ok)
	{
	}

	// Return true when we've found the key we want and loaded it.
	template <typename PAIR_T>
	bool operator()(const PAIR_T &p) const
	{
	    using namespace std;
	    typedef typename tuple_element<1,PAIR_T>::type	ref_type;
	    typedef typename decay<ref_type>::type		obj_type;

	    if (::strcmp(myKey, get<0>(p)) != 0)
		return false;
	    myOk = UT_LoadSerializable<obj_type>::invoke(myArchiver, get<1>(p));
	    return true; // quit looping
	}

	bool ok() const
	{
	    return myOk;
	}

    private:
	ARCHIVER_T &    myArchiver;
	const char *    myKey;
	bool &		myOk;
    };

public:

    bool isLoading() const	{ return true; }
    bool isSaving() const	{ return false; }

    template <typename T>
    bool serializeArray(T *vec, int64 count)
    {
	ARCHIVER_T *subclass = static_cast<ARCHIVER_T *>(this);
	for (int64 i = 0; i < count; i++)
	{
	    if (!UT_LoadSerializable<T>::invoke(*subclass, vec[i]))
		return false;
	}
	return true;
    }

    /// Called by user serialize() methods to serialize a map.
    /// @param seq  A sequence of <key_type, value_type &> sequences where
    ///		    key_type is const char *.
    /// @note key literals should in all LOWERCASE
    template <typename SEQ_T>
    bool operator()(const SEQ_T &pair_seq)
    {
	UT_WorkBuffer	key;
	ARCHIVER_T *	subclass = static_cast<ARCHIVER_T *>(this);
	bool		ok = true;
	bool		keep_going;

	for (keep_going = subclass->serializeMapBegin();
	     keep_going;
	     keep_going = !subclass->serializeMapEnd())
	{
	    if (!subclass->serializeKey(key))
		return false;
	    LoadKeyValue loader(*subclass, key.buffer(), ok);
	    (void) UTtupleAnyOf(pair_seq, loader);
	    if (!loader.ok())
		return false;
	}
	return true; // quit looping
    }

};
// @{


/// Manipulators for streaming archivers
// @{
template <typename OBJ_T, typename ARCHIVER_T>
class UT_SaveArchiverManip
{
public:
    UT_SaveArchiverManip(OBJ_T &obj)
	: myObj(obj)
    {
    }
    UT_OStream &operator()(UT_OStream &out)
    {
	typename ARCHIVER_T::Output archiver(out);
	if (!UT_SaveSerializable<OBJ_T>::invoke(archiver, myObj))
	    out.setstate(std::ios::failbit);
	return out;
    }
private:
    OBJ_T & myObj;
};
template <typename OBJ_T, typename ARCHIVER_T>
class UT_LoadArchiverManip
{
public:
    UT_LoadArchiverManip(OBJ_T &obj)
	: myObj(obj)
    {
    }
    UT_IStream &operator()(UT_IStream &in)
    {
	typename ARCHIVER_T::Input archiver(in);
	if (!UT_LoadSerializable<OBJ_T>::invoke(archiver, myObj))
	    in.setError(true);
	return in;
    }
private:
    OBJ_T & myObj;
};
// @}

/// UT_OStream manipulator << overload
template <typename OBJ_T, typename ARCHIVER_T>
UT_OStream &
operator<<(UT_OStream &out, UT_SaveArchiverManip<OBJ_T,ARCHIVER_T> manip)
{
    return manip(out);
}
/// UT_IStream manipulator >> overload
template <typename OBJ_T, typename ARCHIVER_T>
UT_IStream &
operator>>(UT_IStream &in, UT_LoadArchiverManip<OBJ_T,ARCHIVER_T> manip)
{
    return manip(in);
}

#endif // __UT_SERIALIZATION_H_INCLUDED__
