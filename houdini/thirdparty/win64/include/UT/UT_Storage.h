/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Storage.h (UT Library, C++)
 *
 * COMMENTS:    An enum for common, numeric data types.
 */

#pragma once

#ifndef __UT_Storage__
#define __UT_Storage__

#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>
#include <SYS/SYS_Inline.h>

/// An enum representing numeric data storage types.
///
/// NOTE: The order must be the conversion precedence order.
///       For example, int8(0)+int16(0) will be an int16,
///       and int64(0)+fpreal32(0) will be an fpreal32.
///       This enables UT_StorageBetter.
enum class UT_Storage
{
    INVALID = -1,
    INT8,
    INT16,
    INT32,
    INT64,
    REAL16,
    REAL32,
    REAL64
};

/// Returns true iff the given storage type represents an integer.
SYS_FORCE_INLINE
constexpr bool UTisIntStorage(UT_Storage storage)
{
    return (int(storage) >= int(UT_Storage::INT8) && int(storage) <= int(UT_Storage::INT64));
}

/// Returns true iff the given storage type represents a floating-point number.
SYS_FORCE_INLINE
constexpr bool UTisFloatStorage(UT_Storage storage)
{
    return (int(storage) >= int(UT_Storage::REAL16) && int(storage) <= int(UT_Storage::REAL64));
}

/// Returns the number of bytes in the given storage type.
SYS_FORCE_INLINE
constexpr int UTstorageSize(UT_Storage storage)
{
    // This should really be a switch statement, but constexpr doesn't
    // allow that in C++11, so enjoy this chain of ternary operators! :)
    return (storage == UT_Storage::REAL32 || storage == UT_Storage::INT32) ? 4
        : ((storage == UT_Storage::REAL64 || storage == UT_Storage::INT64) ? 8
        : ((storage == UT_Storage::REAL16 || storage == UT_Storage::INT16) ? 2
        : ((storage == UT_Storage::INT8) ? 1
        : 0)));
}

/// These template classes get the UT_Storage from the type, e.g.
/// UT_Storage s = UT_StorageType<DATA_T>::theStorage;
/// @{
template<typename T>
struct UT_StorageNum {
    static const UT_Storage theStorage = UT_Storage::INVALID;
    typedef void Type;
    typedef void AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef void SecondGuess;
    static const bool theIsFloat = false;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageNum<int8> {
    static const UT_Storage theStorage = UT_Storage::INT8;
    typedef int8 Type;
    typedef int32 AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef int32 SecondGuess;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageNum<int16> {
    static const UT_Storage theStorage = UT_Storage::INT16;
    typedef int16 Type;
    typedef int32 AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef int32 SecondGuess;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageNum<int32> {
    static const UT_Storage theStorage = UT_Storage::INT32;
    typedef int32 Type;
    typedef int32 AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef int64 SecondGuess;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageNum<int64> {
    static const UT_Storage theStorage = UT_Storage::INT64;
    typedef int64 Type;
    typedef int64 AtLeast32Bit;
    typedef fpreal64 MathFloat;
    typedef int32 SecondGuess;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageNum<fpreal16> {
    static const UT_Storage theStorage = UT_Storage::REAL16;
    typedef fpreal16 Type;
    typedef fpreal32 AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef fpreal32 SecondGuess;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageNum<fpreal32> {
    static const UT_Storage theStorage = UT_Storage::REAL32;
    typedef fpreal32 Type;
    typedef fpreal32 AtLeast32Bit;
    typedef fpreal32 MathFloat;
    typedef fpreal64 SecondGuess;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageNum<fpreal64> {
    static const UT_Storage theStorage = UT_Storage::REAL64;
    typedef fpreal64 Type;
    typedef fpreal64 AtLeast32Bit;
    typedef fpreal64 MathFloat;
    typedef fpreal32 SecondGuess;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
/// @}

/// These template classes get the type from the UT_Storage, e.g.
/// return (UT_StorageType<STORAGE>::theType)source;
/// @{
template<UT_Storage STORAGE>
struct UT_StorageType {
    static const UT_Storage theStorage = UT_Storage::INVALID;
    typedef void Type;
    static const bool theIsFloat = false;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageType<UT_Storage::INT8> {
    static const UT_Storage theStorage = UT_Storage::INT8;
    typedef int8 Type;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageType<UT_Storage::INT16> {
    static const UT_Storage theStorage = UT_Storage::INT16;
    typedef int16 Type;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageType<UT_Storage::INT32> {
    static const UT_Storage theStorage = UT_Storage::INT32;
    typedef int32 Type;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageType<UT_Storage::INT64> {
    static const UT_Storage theStorage = UT_Storage::INT64;
    typedef int64 Type;
    static const bool theIsFloat = false;
    static const bool theIsInt = true;
};
template<>
struct UT_StorageType<UT_Storage::REAL16> {
    static const UT_Storage theStorage = UT_Storage::REAL16;
    typedef fpreal16 Type;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageType<UT_Storage::REAL32> {
    static const UT_Storage theStorage = UT_Storage::REAL32;
    typedef fpreal32 Type;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
template<>
struct UT_StorageType<UT_Storage::REAL64> {
    static const UT_Storage theStorage = UT_Storage::REAL64;
    typedef fpreal64 Type;
    static const bool theIsFloat = true;
    static const bool theIsInt = false;
};
/// @}

/// This allows one to determine the type of T0+T1 at compile-time.
template<typename T0,typename T1>
struct UT_StorageBetter {
    // NOTE: The casts to int are only necessary for GCC 4.4's broken support for enum classes
    typedef typename SYS_SelectType<T0,T1,(int(UT_StorageNum<T0>::theStorage) < int(UT_StorageNum<T1>::theStorage))>::type Type;
};

/// This allows one to determine the type of T0+T1 at compile-time.
template<typename T0,typename T1>
struct UT_StorageAtLeast32Bit {
    using Type = typename SYS_SelectType<typename UT_StorageNum<typename UT_StorageBetter<T0,T1>::Type>::AtLeast32Bit,T0,UT_StorageNum<typename UT_StorageBetter<T0,T1>::Type>::theStorage==UT_Storage::INVALID>::type;
};

template<exint TSIZE,typename DATA_T>
struct UT_TupleType
{
    SYS_FORCE_INLINE exint getTupleSize() const { return TSIZE; }
    SYS_FORCE_INLINE void setTupleSize(exint tuplesize) { UT_ASSERT_P(tuplesize == TSIZE); }
    SYS_FORCE_INLINE UT_Storage getStorage() const { return UT_StorageNum<DATA_T>::theStorage; }
    SYS_FORCE_INLINE void setStorage(UT_Storage storage) { UT_ASSERT_P(storage == UT_StorageNum<DATA_T>::theStorage); }
};

template<typename DATA_T>
struct UT_TupleType<-1,DATA_T>
{
    SYS_FORCE_INLINE exint getTupleSize() const { return myTupleSize; }
    SYS_FORCE_INLINE void setTupleSize(exint tuplesize) { myTupleSize = tuplesize; }
    SYS_FORCE_INLINE UT_Storage getStorage() const { return UT_StorageNum<DATA_T>::theStorage; }
    SYS_FORCE_INLINE void setStorage(UT_Storage storage) { UT_ASSERT_P(storage == UT_StorageNum<DATA_T>::theStorage); }
private:
    int myTupleSize;
};

template<exint TSIZE>
struct UT_TupleType<TSIZE, void>
{
    SYS_FORCE_INLINE exint getTupleSize() const { return TSIZE; }
    SYS_FORCE_INLINE void setTupleSize(exint tuplesize) { UT_ASSERT_P(tuplesize == TSIZE); }
    SYS_FORCE_INLINE UT_Storage getStorage() const { return myStorage; }
    SYS_FORCE_INLINE void setStorage(UT_Storage storage) { myStorage = storage; }
private:
    int myTupleSizePlaceholder;
    UT_Storage myStorage;
};

template<>
struct UT_TupleType<-1, void>
{
    SYS_FORCE_INLINE exint getTupleSize() const { return myTupleSize; }
    SYS_FORCE_INLINE void setTupleSize(exint tuplesize) { myTupleSize = tuplesize; }
    SYS_FORCE_INLINE UT_Storage getStorage() const { return myStorage; }
    SYS_FORCE_INLINE void setStorage(UT_Storage storage) { myStorage = storage; }
private:
    int myTupleSize;
    UT_Storage myStorage;
};

template<typename TO,typename FROM>
SYS_FORCE_INLINE
TO UTconvertStorage(FROM from)
{ return TO(from); }

#define DECLARE_CONVERT_STORAGE_ROUND(T,F) \
template<> \
SYS_FORCE_INLINE \
T UTconvertStorage<T,F>(F from) \
{ return T(from + ((from > 0) ? F(0.5) : F(-0.5))); } \
/* end of macro */

DECLARE_CONVERT_STORAGE_ROUND(int8,fpreal16)
DECLARE_CONVERT_STORAGE_ROUND(int8,fpreal32)
DECLARE_CONVERT_STORAGE_ROUND(int8,fpreal64)
DECLARE_CONVERT_STORAGE_ROUND(int16,fpreal16)
DECLARE_CONVERT_STORAGE_ROUND(int16,fpreal32)
DECLARE_CONVERT_STORAGE_ROUND(int16,fpreal64)
DECLARE_CONVERT_STORAGE_ROUND(int32,fpreal16)
DECLARE_CONVERT_STORAGE_ROUND(int32,fpreal32)
DECLARE_CONVERT_STORAGE_ROUND(int32,fpreal64)
DECLARE_CONVERT_STORAGE_ROUND(int64,fpreal16)
DECLARE_CONVERT_STORAGE_ROUND(int64,fpreal32)
DECLARE_CONVERT_STORAGE_ROUND(int64,fpreal64)

#undef DECLARE_CONVERT_STORAGE_ROUND


#endif
