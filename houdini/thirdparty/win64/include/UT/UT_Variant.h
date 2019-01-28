/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Variant.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_Variant__
#define __UT_Variant__

#include "UT_API.h"

#include "UT_Array.h"
#include "UT_SharedPtr.h"
#include "UT_StringMap.h"
#include "UT_VectorTypes.h"

#include <array>
#include <type_traits>
#include <memory>

class UT_Variant;

// ============================================================================
/// Wrapper around a shared variant array container. 
/// Modifications are shared among all wrappers referring to the same container.
class UT_API UT_VariantArray
{
public:
    using container_type = UT_Array<UT_Variant>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    /// Construct a variant array of a given size. The constructed variants
    /// will all be of the unknown type.
    UT_VariantArray(exint size = 0);
    
    /// Construct a variant array from an initializer list. 
    UT_VariantArray(std::initializer_list<UT_Variant> init);

    /// Construct a variant array by referencing the variant array container  
    /// from another. Only the pointer to the underlying array is copied. 
    /// The two wrappers will share the same array.
    UT_VariantArray(const UT_VariantArray &) = default;
    
    /// Move the variant array container from another variant array. 
    UT_VariantArray(UT_VariantArray &&) = default;
    
    /// Copy a variant array by referencing the variant array container from 
    /// another. The two variant arrays will share the same list container.   
    UT_VariantArray &operator=(const UT_VariantArray &) = default;
    
    /// Move the variant array container from another variant array.
    UT_VariantArray &operator=(UT_VariantArray &&) = default;

    /// Item-wise equality comparison of the variants of the two arrays.
    bool operator==(const UT_VariantArray &o) const { return m_array->operator==(*o.m_array); }

    /// Item-wise non-equality comparison of the variants of the two arrays.
    bool operator!=(const UT_VariantArray &o) const { return !operator==(o); }
    
    /// Make a deep copy of another array container. This array wrapper will not 
    /// share the array container with the other.
    void deepCopy(const UT_VariantArray &);
    
    /// Returns the number of items in the array container.
    exint size() const { return m_array->size(); }
    
    /// Returns a read-only variant at index @c index. No bound-checking is 
    /// performed.
    const UT_Variant &operator[](exint index) const { return m_array->operator [](index); }
    
    /// Returns a writable variant at index @c index. No bound-checking is 
    /// performed.
    UT_Variant &operator[](exint index) { return m_array->operator [](index); }
    
    /// Add a new variant to the array container. All instances of a array 
    /// wrapper that use the same array container will see the new item. 
    exint append(const UT_Variant &t) { return m_array->append(t); }
    
    /// Add a new variant to the array container. All instances of a array 
    /// wrapper that use the same array container will see the new item. 
    exint append(UT_Variant &&t) { return m_array->append(t); }

    /// Return a writable iterator to the beginning of the array container.
    iterator begin() { return m_array->begin(); }
    
    /// Return a read-only iterator to the beginning of the array container.
    const_iterator begin() const { return m_array->begin(); }
    
    /// Return a writable iterator to the end of the array container.
    iterator end() { return m_array->end(); }
    
    /// Return a read-only iterator to the end of the array container.
    const_iterator end() const { return m_array->end(); }
    
    /// Returns a read-only reference to the underlying array container.
    const container_type &container() const { return *m_array.get(); } 
    
    /// Returns a writable reference to the underlying array container.
    container_type &container() { return *m_array.get(); }
private:
    UT_SharedPtr<container_type> m_array;
};

// ============================================================================
/// Wrapper around a shared variant map container. 
/// Modifications are shared among all wrappers referring to the same container.
class UT_API UT_VariantMap
{
public:
    using container_type = UT_StringMap<UT_Variant>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    
    /// Construct a default, empty container.  
    UT_VariantMap();

    /// Construct a new container from a list of key/value pairs. 
    UT_VariantMap(std::initializer_list<container_type::value_type> init);
    
    /// Construct a variant map by referencing the variant map container  
    /// from another. The two map wrappers will share the same map.
    UT_VariantMap(const UT_VariantMap &) = default;
    
    /// Move the variant map container from another variant map wrapper. 
    UT_VariantMap(UT_VariantMap &&) = default;

    /// Copy a variant map by referencing the variant map container from 
    /// another. The two map wrappers will share the same map container.   
    UT_VariantMap &operator=(const UT_VariantMap &) = default;
    
    /// Move the variant map container from another variant map wrapper.
    UT_VariantMap &operator=(UT_VariantMap &&) = default;

    /// Item-wise equality comparison of the keys and variants of the two maps.
    bool operator==(const UT_VariantMap &o) const;
    
    /// Item-wise non-equality comparison of the keys and variants of the two 
    /// maps.
    bool operator!=(const UT_VariantMap &o) const { return !operator==(o); }
    
    /// Make a deep copy of another array container. This array wrapper will not 
    /// share the array container with the other.
    void deepCopy(const UT_VariantMap &);
    
    /// Returns a read-only variant associated with @c key. No error checking
    /// is performed. If the key doesn't exist, the behavior is undefined.
    const UT_Variant &operator[](const UT_StringRef &key) const;
    
    /// Returns a writable variant associated with @c key. No error checking
    /// is performed. If the key doesn't exist, the behavior is undefined.
    UT_Variant &operator[](const UT_StringHolder &key);
    
    /// Returns a read-only iterator pointing to the item associated with @c key.
    /// If the item doesn't exist, this will be the value of @end.
    const_iterator find(const UT_StringRef &key) const;
    
    /// Returns a writable iterator pointing to the item associated with @c key.
    /// If the item doesn't exist, this will be the value of @end.
    iterator find(const UT_StringRef &key);
    
    /// Returns @c true if the item associated with @c key exists in the map
    /// container.
    bool contains(const UT_StringRef &key) const { return m_map->contains(key); }
    
    /// Erase the item associated with the key @key. If the item existed and 
    /// was successfully erased, this function returns @true. 
    bool erase(const UT_StringRef &key);
    
    /// Erase the item pointed to by the iterator @c it. The returned 
    /// @c iterator will point to the next item after the one deleted.
    iterator erase(const_iterator it);
    
    /// Return a writable iterator to the beginning of the map container.
    iterator begin();
    
    /// Return a read-only iterator to the beginning of the map container.
    const_iterator begin() const;
    
    /// Return a writable iterator to the end of the map container.
    iterator end();
    
    /// Return a read-only iterator to the end of the map container.
    const_iterator end() const;
    
    /// Returns a read-only reference to the underlying map container.
    const container_type &container() const { return *m_map.get(); }
    
    /// Returns a writable reference to the underlying map container.
    container_type &container() { return *m_map.get(); }
    
private:
    UT_SharedPtr<container_type> m_map;
};

// ============================================================================

/// UT_Variant is a container that can hold a single value of many different 
/// types.
class UT_API UT_Variant
{
public:
    /// 
    enum class Type
    {
	Unknown,		///< Uninitialized variant. This is distinct from a null variant. 
	Null, 			///< A null variant. A null variant holds no value.
	Bool,			///< A boolean.
	Int,			///< A 64-bit signed integer.
	UInt,			///< A 64-bit unsigned integer.
	Float,			///< A 64-bit float value.
	String,			///< A shared string.
	Vector2,		///< @c UT_Vector2D
	Vector3,		///< @c UT_Vector3D
	Vector4,		///< @c UT_Vector4D
	Matrix2,		///< @c UT_Matrix2D
	Matrix3,		///< @c UT_Matrix3D
	Matrix4,		///< @c UT_Matrix4D
	Pointer,		///< An arbitrary @c void pointer value.
	Array,			///< A @c UT_VariantArray value
	Map			///< A @c UT_VariantMap value
    };
    
    /// A keystone type used to indicate that a variant should be initialized
    /// as a null variant.
    enum NullType { Null };
    
    template<typename T>
    struct TypeInfo { static constexpr Type typeId = Type::Unknown;
		      static constexpr bool is_trivial = false;
		      static constexpr bool is_complex = false; };
    
    template<typename T>
    struct IsTrivialType
    {
	static constexpr bool value = TypeInfo<T>::is_trivial;
    };
    
    template<typename T>
    struct IsCompoundType
    {
	static constexpr bool value = TypeInfo<T>::is_complex; 
    };

    template<typename U> struct TypeConversion { using To = U; using From = void; };
    template<typename T> struct IsConvertibleType
    {
	static constexpr bool value = 
	    !std::is_same<typename TypeConversion<T>::From, void>::value;
    };
    
    /// Construct an unknown variant.
    UT_Variant() : m_type(Type::Unknown) {}
    
    /// Construct a null variant. Call like so:
    /// @code
    /// UT_Variant null(UT_Variant::Null);
    /// @endcode
    UT_Variant(NullType) : m_type(Type::Null) {}
    
    /// Construct a boolean variant with the given value.
    UT_Variant(bool v) : m_type(Type::Bool) { m_value.m_bool = v; }
    
    /// Construct an integer variant with the given value.
    UT_Variant(int32 v) : m_type(Type::Int) { m_value.m_int = v; }
    
    /// Construct an integer variant with the given value. 
    UT_Variant(int64 v) : m_type(Type::Int) { m_value.m_int = v; }
    
    /// Construct an unsigned integer variant with the given value.
    UT_Variant(uint32 v) : m_type(Type::UInt) { m_value.m_int = int64(v); }
    
    /// Construct an unsigned integer variant with the given value.
    UT_Variant(uint64 v) : m_type(Type::UInt) { m_value.m_int = int64(v); }
    
    /// Construct a float variant with the given value.
    UT_Variant(fpreal32 v) : m_type(Type::Float) { m_value.m_float = v; }
    
    /// Construct a float variant with the given value.
    UT_Variant(fpreal64 v) : m_type(Type::Float) { m_value.m_float = v; }
    
    /// Construct a string variant with the given value.
    UT_Variant(const char *v);
    
    /// Construct a @c Type::String variant with the given value.
    UT_Variant(const UT_StringHolder &v);
    
    /// Construct a @c Type::String variant with the given value.
    UT_Variant(const UT_StringRef &v);
    
    /// Construct a @c Type::Vector2 variant with the given value.
    UT_Variant(const UT_Vector2F &v);
    
    /// Construct a @c Type::Vector2 variant with the given value.
    UT_Variant(const UT_Vector2D &v);
    
    /// Construct a @c Type::Vector3 variant with the given value.
    UT_Variant(const UT_Vector3F &v);
    
    /// Construct a @c Type::Vector3 variant with the given value.
    UT_Variant(const UT_Vector3D &v);
    
    /// Construct a @c Type::Vector4 variant with the given value.
    UT_Variant(const UT_Vector4F &v);
    
    /// Construct a @c Type::Vector4 variant with the given value.
    UT_Variant(const UT_Vector4D &v);
    
    /// Construct a @c Type::Matrix2 variant with the given value.
    UT_Variant(const UT_Matrix2F &v);
    
    /// Construct a @c Type::Matrix2 variant with the given value.
    UT_Variant(const UT_Matrix2D &v);
    
    /// Construct a @c Type::Matrix3 variant with the given value.
    UT_Variant(const UT_Matrix3F &v);
    
    /// Construct a @c Type::Matrix3 variant with the given value.
    UT_Variant(const UT_Matrix3D &v);
    
    /// Construct a @c Type::Matrix4 variant with the given value.
    UT_Variant(const UT_Matrix4F &v);
    
    /// Construct a @c Type::Matrix4 variant with the given value.
    UT_Variant(const UT_Matrix4D &v);
    
    /// Construct a @c Type::Pointer variant with the given value.
    UT_Variant(void *v) : m_type(Type::Pointer) { m_value.m_ptr = v; }
    
    /// Construct a @c Type::Array variant with the given value.
    UT_Variant(const UT_VariantArray &v);
    
    /// Construct a @c Type::Array variant with the given value.
    UT_Variant(UT_VariantArray &&v);
    
    /// Construct a @c Type::Map variant with the given value.
    UT_Variant(const UT_VariantMap &v);
    
    /// Construct a @c Type::Map variant with the given value.
    UT_Variant(UT_VariantMap &&v);
    
    /// Copy construct a variant with the given value.
    UT_Variant(const UT_Variant &v);
    
    /// Move construct a variant with the given value.
    UT_Variant(UT_Variant &&v);
    
    /// Variant destructor. For shared items, it only decreases reference 
    /// counts.
    ~UT_Variant();

    /// Copy assignment operator. For shared data, it only adds a reference to
    /// them.
    UT_Variant &operator=(const UT_Variant &v);
    
    /// Move assignment operator. The passed-in variant is cleared out and left
    /// as an unknown type.
    UT_Variant &operator=(UT_Variant &&v);
    
    /// 
    bool operator==(const UT_Variant &v) const;
    
    Type type() const { return m_type; }
    const char *typeName() const { return typeName(m_type); }
    static const char *typeName(Type type);
    
    /// Convert this variant to a different type, in-place. Returns @c true
    /// if the conversion succeeded. If the conversion failed, the variant
    /// is left as-is.
    bool convert(Type type);
    
    /// An explicit method of setting the variant type to null.
    void setNull() { convert(Type::Null); }
    
    /// @{
    /// Return the type requested by the template argument. If the contained  
    /// type is not of the requested type, a conversion is attempted.  
    /// If the conversion fails, the default instance of the return type is 
    /// returned and the value of @c ok is set to @c false.
    /// Compound values, such as the vector and matrix types are returned by
    /// reference, whereas all the others are returned by value.
    template<typename T>
    typename std::enable_if<IsTrivialType<T>::value && 
			    !IsConvertibleType<T>::value, T>::type
    get(bool &ok) const;

    template<typename T>
    typename std::enable_if<IsCompoundType<T>::value && 
			    !IsConvertibleType<T>::value, const T &>::type
    get(bool &ok) const;
    
    template<typename T>
    typename std::enable_if<!IsTrivialType<T>::value && 
			    !IsCompoundType<T>::value && 
			    IsConvertibleType<T>::value, T>::type
    get(bool &ok) const;
    /// @}

    /// @{
    /// Return the type requested by the template argument. If the contained  
    /// type is not of the requested type, a conversion is attempted.  
    /// If the conversion fails, a default value is returned for the type
    /// requested.
    template<typename T>
    typename std::enable_if<IsTrivialType<T>::value && 
			    !IsConvertibleType<T>::value, T>::type
    get() const { bool ok; return get<T>(ok); }

    template<typename T>
    typename std::enable_if<IsCompoundType<T>::value && 
			    !IsConvertibleType<T>::value, const T &>::type
    get() const { bool ok; return get<T>(ok); }
    
    template<typename T>
    typename std::enable_if<!IsTrivialType<T>::value && 
			    !IsCompoundType<T>::value && 
			    IsConvertibleType<T>::value, T>::type
    get() const { bool ok; return get<T>(ok); }
    /// @}
    
    /// Set the value and type of this variant to match the type and value of
    /// the argument passed in.
    template<typename T> void set(const T &v)
    {
	move(UT_Variant(v));
    }
    
private:
    void copy(const UT_Variant &v);
    void move(UT_Variant &&v);
    void destroy();
    
    template<typename T> 
    const T *valueGet() const;

    template<typename T> 
    T &valueAlloc();
    
    template<typename T> 
    void valueDelete();

    template<typename T>
    T valueConvert(bool &ok) const;
    
    bool parseBool(bool &ok) const;
    int64 parseInt(bool &ok) const;
    uint64 parseUInt(bool &ok) const;
    fpreal64 parseFloat(bool &ok) const;
    UT_StringHolder formatString(bool &ok) const;
    UT_StringHolder formatInt() const;
    UT_StringHolder formatUInt() const;
    UT_StringHolder formatFloat() const;
    
    union 
    {
	bool		 m_bool;
	int64		 m_int;
	fpreal64	 m_float;
	void		*m_ptr;
	
	// Any type, same size or smaller than this, will be aliased into this
	// storage. Otherwise it will be allocated on the heap. 
	// std::array is a trivial type and so is allowed in a union with no
	// special considerations.
	std::array<unsigned char, 24> m_storage;
    } m_value;
    Type	m_type;
};

// ============================================================================
// UT_VariantArray implementation

inline 
UT_VariantArray::UT_VariantArray(std::initializer_list<UT_Variant> init) :
    UT_VariantArray(init.size())
{
	for (auto &&v: init)
	    m_array->append(v);
}


// ============================================================================
// UT_VariantMap implementation

inline 
UT_VariantMap::UT_VariantMap(
    std::initializer_list<container_type::value_type> init) :
    UT_VariantMap()
{
    for (auto &&v: init)
	m_map->insert(v);
}

inline bool 
UT_VariantMap::operator==(const UT_VariantMap &o) const 
{
    return *m_map == *o.m_map;
}

inline const UT_Variant &
UT_VariantMap::operator[](const UT_StringRef &key) const 
{ 
    return m_map->operator [](key); 
}

inline UT_Variant &
UT_VariantMap::operator[](const UT_StringHolder &key) 
{ 
    return m_map->operator [](key); 
}

inline UT_VariantMap::iterator 
UT_VariantMap::find(const UT_StringRef &key) 
{ 
    return m_map->find(key); 
}

inline UT_VariantMap::const_iterator 
UT_VariantMap::find(const UT_StringRef &key) const 
{ 
    return m_map->find(key); 
}

inline bool 
UT_VariantMap::erase(const UT_StringRef &key) 
{ 
    return m_map->erase(key); 
}

inline UT_VariantMap::iterator
UT_VariantMap::erase(const_iterator it) 
{ 
    return m_map->erase(it); 
}

inline UT_VariantMap::iterator 
UT_VariantMap::begin() 
{ 
    return m_map->begin(); 
}

inline UT_VariantMap::const_iterator 
UT_VariantMap::begin() const 
{ 
    return m_map->begin(); 
}

inline UT_VariantMap::iterator 
UT_VariantMap::end() 
{ 
    return m_map->end(); 
}

inline UT_VariantMap::const_iterator 
UT_VariantMap::end() const 
{ 
    return m_map->end(); 
}

// ============================================================================
// UT_Variant implementation

#define UT_VARIANT_TYPE_ID(_T_, _ID_, _TRV_, _CPL_) \
    template<> struct UT_Variant::TypeInfo<_T_> { \
	static constexpr UT_Variant::Type typeId = UT_Variant::Type::_ID_; \
	static constexpr bool is_trivial = _TRV_; \
	static constexpr bool is_complex = _CPL_; \
    }

UT_VARIANT_TYPE_ID(void, Null, false, false);
UT_VARIANT_TYPE_ID(bool, Bool, true, false);
UT_VARIANT_TYPE_ID(int64, Int, true, false);
UT_VARIANT_TYPE_ID(uint64, UInt, true, false);
UT_VARIANT_TYPE_ID(fpreal64, Float, true, false);
UT_VARIANT_TYPE_ID(UT_StringHolder, String, true, false);
UT_VARIANT_TYPE_ID(UT_Vector2D, Vector2, false, true);
UT_VARIANT_TYPE_ID(UT_Vector3D, Vector3, false, true);
UT_VARIANT_TYPE_ID(UT_Vector4D, Vector4, false, true);
UT_VARIANT_TYPE_ID(UT_Matrix2D, Matrix2, false, true);
UT_VARIANT_TYPE_ID(UT_Matrix3D, Matrix3, false, true);
UT_VARIANT_TYPE_ID(UT_Matrix4D, Matrix4, false, true);
UT_VARIANT_TYPE_ID(void *, Pointer, true, false);
UT_VARIANT_TYPE_ID(UT_VariantArray, Array, true, false);
UT_VARIANT_TYPE_ID(UT_VariantMap, Map, true, false);

#undef UT_VARIANT_TYPE_ID

template<>
inline bool 
UT_Variant::valueConvert<bool>(bool &ok) const
{
    ok = true;
    if (m_type == Type::Bool)
	return m_value.m_bool;
    else if (m_type == Type::Int || m_type == Type::UInt)
	return m_value.m_int != 0;
    else if (m_type == Type::Float)
	return m_value.m_float != 0;
    else if (m_type == Type::String)
	return parseBool(ok);
    else
    {
	ok = false;
	return false;
    }
}

template<>
inline int64 
UT_Variant::valueConvert<int64>(bool &ok) const
{
    ok = true;
    if (m_type == Type::Bool)
	return m_value.m_bool ? 1 : 0;
    else if (m_type == Type::Int || m_type == Type::UInt)
	return m_value.m_int;
    else if (m_type == Type::Float)
	return static_cast<int64>(m_value.m_float);
    else if (m_type == Type::String)
	return parseInt(ok);
    else
    {
	ok = false;
	return false;
    }
}

template<>
inline uint64 
UT_Variant::valueConvert<uint64>(bool &ok) const
{
    ok = true;
    if (m_type == Type::Bool)
	return m_value.m_bool ? 1 : 0;
    else if (m_type == Type::Int || m_type == Type::UInt)
	return uint64(m_value.m_int);
    else if (m_type == Type::Float)
	return static_cast<uint64>(m_value.m_float);
    else if (m_type == Type::String)
	return uint64(parseInt(ok));
    else
    {
	ok = false;
	return false;
    }
}

template<>
inline fpreal64 
UT_Variant::valueConvert<fpreal64>(bool &ok) const
{
    ok = true;
    if (m_type == Type::Bool)
	return m_value.m_bool ? 1 : 0;
    else if (m_type == Type::Int)
	return fpreal64(m_value.m_int);
    else if (m_type == Type::UInt)
	return fpreal64(uint64(m_value.m_int));
    else if (m_type == Type::Float)
	return m_value.m_float;
    else if (m_type == Type::String)
	return parseFloat(ok);
    else
    {
	ok = false;
	return false;
    }
}

template<>
inline UT_StringHolder 
UT_Variant::valueConvert<UT_StringHolder>(bool &ok) const
{
    ok = true;
    if (m_type == Type::Bool)
	return UT_StringHolder(m_value.m_bool ? "true" : "false");
    else if (m_type == Type::Int)
	return formatInt();
    else if (m_type == Type::UInt)
	return formatUInt();
    else if (m_type == Type::Float)
	return formatFloat();
    else if (m_type == Type::String)
	return *valueGet<UT_StringHolder>();
    else
    {
	ok = false;
	return UT_StringHolder();
    }
}


template<>
inline void * 
UT_Variant::valueConvert<void *>(bool &ok) const
{
    if (m_type == Type::Pointer)
    {
	ok = true;
	return m_value.m_ptr;
    }
    else
    {
	ok = false;
	return nullptr;
    }
}


template<>
inline UT_VariantArray 
UT_Variant::valueConvert<UT_VariantArray>(bool &ok) const
{
    if (m_type == Type::Array)
    {
	ok = true;
	return *valueGet<UT_VariantArray>();
    }
    else
    {
	ok = false;
	return UT_VariantArray();
    }
}

template<>
inline UT_VariantMap 
UT_Variant::valueConvert<UT_VariantMap>(bool &ok) const
{
    if (m_type == Type::Map)
    {
	ok = true;
	return *valueGet<UT_VariantMap>();
    }
    else
    {
	ok = false;
	return UT_VariantMap();
    }
}


template<typename T>
typename std::enable_if<UT_Variant::IsTrivialType<T>::value && 
		        !UT_Variant::IsConvertibleType<T>::value, T>::type
UT_Variant::get(bool &ok) const
{
    return valueConvert<T>(ok);
}

template<typename T>
typename std::enable_if<UT_Variant::IsCompoundType<T>::value && 
		        !UT_Variant::IsConvertibleType<T>::value, const T &>::type
UT_Variant::get(bool &ok) const
{
    const T *v = this->valueGet<T>();
    if (v)
    {
	ok = true;
	return *v;
    }
    else
    {
	static T s_default;
	ok = false;
	return s_default;
    }
}

// Conversion templates 
template<> struct UT_Variant::TypeConversion<int32> { using From = int64; };
template<> struct UT_Variant::TypeConversion<uint32> { using From = uint64; };
template<> struct UT_Variant::TypeConversion<fpreal32> { using From = fpreal64; };
template<> struct UT_Variant::TypeConversion<UT_Vector2F> { using From = UT_Vector2D; };
template<> struct UT_Variant::TypeConversion<UT_Vector3F> { using From = UT_Vector3D; };
template<> struct UT_Variant::TypeConversion<UT_Vector4F> { using From = UT_Vector4D; };
template<> struct UT_Variant::TypeConversion<UT_Matrix2F> { using From = UT_Matrix2D; };
template<> struct UT_Variant::TypeConversion<UT_Matrix3F> { using From = UT_Matrix3D; };
template<> struct UT_Variant::TypeConversion<UT_Matrix4F> { using From = UT_Matrix4D; };
template<> struct UT_Variant::TypeConversion<const char *> { using From = UT_StringHolder; };


template<typename T>
typename std::enable_if<!UT_Variant::IsTrivialType<T>::value && 
			!UT_Variant::IsCompoundType<T>::value && 
			UT_Variant::IsConvertibleType<T>::value, T>::type
UT_Variant::get(bool &ok) const
{
    return static_cast<T>(this->get<typename TypeConversion<T>::From>(ok));
}

UT_API size_t format(char *buffer, size_t buffer_size, const UT_Variant &v);


#endif // __UT_Variant__
