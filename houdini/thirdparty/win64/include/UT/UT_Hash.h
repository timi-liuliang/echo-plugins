/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Hash.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_Hash__
#define __UT_Hash__

#include "UT_API.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Math.h>
#include "UT_String.h"
#include "UT_SmallObject.h"

SYS_DEPRECATED_PUSH_DISABLE()

class SYS_DEPRECATED_REPLACE(16.0, "SYShash") UT_API UT_Hash
#if !SYS_IS_GCC_GE(4,7)
    : public UT_SmallObject<UT_Hash,
    			    UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
     			    UT_SMALLOBJECT_PAGESIZE_DEFAULT,
     			    UT_SMALLOBJECT_THREADSAFE_ON>
#endif // GCC 4.7+ crashes in some cases with UT_Hash as a small object.
{
    public:
	virtual ~UT_Hash() { }

	// return 0 on equality
	//
	// Often, the comparison function for a UT_Hash is more expensive
	// than computing the unsigned integer hash code.  However, it's
	// possible for the compare() function to be called even when
	// there is no collision on the hash codes, therefore, if you
	// have the hash codes cached, its often efficient to do
	// a quick comparison of the hash codes before computing the
	// expensive compare function.
	virtual int		compare(const UT_Hash &a) const = 0;

	// Copies data from another UT_Hash into this one.
	virtual void		copy(const UT_Hash &a) = 0;

	// return hash value
	// should never change i.e. should rely on const data
	virtual unsigned	hash() const = 0;

	virtual UT_Hash*	copy() const = 0;

	virtual int64		getMemoryUsage(bool inclusive) const = 0;
};

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<string>") UT_API UT_Hash_String : 
    public UT_Hash 
{
    private:
	char		*myData;
    public:
	UT_Hash_String( const char *s )
	{
	    myData = strdup( s );
	}

	virtual ~UT_Hash_String()
	{
	    free(myData);
	}

	virtual int compare( const UT_Hash & a) const
	{
	    return strcmp(myData, ((const UT_Hash_String&)a).myData);
	}

	virtual void		copy(const UT_Hash &a)
	{
	    free(myData);
	    myData = strdup(static_cast<const UT_Hash_String &>(a).myData);
	}

	virtual unsigned	hash() const
	{
	    return UT_String::hash(myData);
	}

	virtual UT_Hash*	copy() const
	{
	    return new UT_Hash_String( myData );
	}

	virtual int64		getMemoryUsage(bool inclusive) const
	{
	    return (inclusive ? sizeof(*this) : 0)
                + (::strlen(myData) + 1) * sizeof(char);
	}

	const char*	getString() const
	{
	    return myData;
	}
};

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<int>") UT_API UT_Hash_Int : 
    public UT_Hash {
    private:
	int			myData;
	unsigned		myKey;
    public:
	UT_Hash_Int( int a ) : myData( a )
	{
	    myKey = (unsigned) a;
	    myKey = SYSwang_inthash(myKey);
	}

	virtual ~UT_Hash_Int()
	{
	}

	virtual int compare( const UT_Hash & a) const
	{
	    return !(myData==((const UT_Hash_Int&)a).myData);
	}

	virtual void		copy(const UT_Hash &a)
	{
	    myData = static_cast<const UT_Hash_Int &>(a).myData;
	    myKey = static_cast<const UT_Hash_Int &>(a).myKey;
	}

	virtual unsigned	hash() const
	{
	    return myKey;
	}

	virtual UT_Hash*	copy() const
	{
	    return new UT_Hash_Int( myData );
	}

	virtual int64		getMemoryUsage(bool inclusive) const
	{
	    return inclusive ? sizeof(*this) : 0;
	}

	int		getData() const { return myData; }
};

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<int64>") UT_API UT_Hash_Int64 : public UT_Hash {
    private:
	int64			myData;
	uint64			myKey;
    public:
	UT_Hash_Int64( int64 a ) : myData( a )
	{
	    myKey = (uint64) a;
	    myKey = SYSwang_inthash64(myKey);
	}

	virtual ~UT_Hash_Int64()
	{
	}

	virtual int compare( const UT_Hash & a) const
	{
	    return !(myData==((const UT_Hash_Int64&)a).myData);
	}

	virtual void		copy(const UT_Hash &a)
	{
	    myData = static_cast<const UT_Hash_Int64 &>(a).myData;
	    myKey = static_cast<const UT_Hash_Int64 &>(a).myKey;
	}

	virtual unsigned	hash() const
	{
	    return myKey;
	}

	virtual UT_Hash*	copy() const
	{
	    return new UT_Hash_Int64( myData );
	}

	virtual int64		getMemoryUsage(bool inclusive) const
	{
	    return inclusive ? sizeof(*this) : 0;
	}

	int64		getData() const { return myData; }
};

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<void *>") UT_API UT_Hash_Ptr : public UT_Hash {
    private:
	const void	*myData;
    public:
	UT_Hash_Ptr( const void *p )
	    : myData(p)
	{
	}

	virtual ~UT_Hash_Ptr()
	{
	}

	virtual int compare(const UT_Hash &a) const
	{
	    return !(myData==((const UT_Hash_Ptr&)a).myData);
	}

	virtual void		copy(const UT_Hash &a)
	{
	    myData = static_cast<const UT_Hash_Ptr &>(a).myData;
	}

	virtual unsigned	hash() const
	{
	    unsigned		key;
	    key = SYSpointerHash(myData);
	    key = SYSwang_inthash(key);
	    return key;
	}

	virtual UT_Hash*	copy() const
	{
	    return new UT_Hash_Ptr(myData);
	}

	virtual int64		getMemoryUsage(bool inclusive) const
	{
	    return inclusive ? sizeof(*this) : 0;
	}

	/// Get the pointer as a given type
	template <typename T>
	    const T	*asPointer() const { return (const T *)(myData); }

	/// Casting operators
	operator const void *() const
	{
	    return myData;
	}
	operator void *() const
	{
	    return const_cast<void *>(myData);
	}
};

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<const void *>") UT_API 
    UT_Hash_Const_Ptr : public UT_Hash {
    private:
	const void  *myData;
    public:
	UT_Hash_Const_Ptr( const void *p ) : myData(p)
	{
	}

	virtual ~UT_Hash_Const_Ptr()
	{
	}

	virtual int compare(const UT_Hash &a) const
	{
	    return !(myData==((const UT_Hash_Const_Ptr&)a).myData);
	}

	virtual void		copy(const UT_Hash &a)
	{
	    myData = static_cast<const UT_Hash_Const_Ptr &>(a).myData;
	}

	virtual unsigned	hash() const
	{
	    unsigned		key;
	    key = SYSpointerHash(myData);
	    key = SYSwang_inthash(key);
	    return key;
	}

	virtual UT_Hash*	copy() const
	{
	    return new UT_Hash_Const_Ptr(myData);
	}

	virtual int64		getMemoryUsage(bool inclusive) const
	{
	    return inclusive ? sizeof(*this) : 0;
	}
};

SYS_DEPRECATED_POP_DISABLE()

#endif

