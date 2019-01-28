/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Defaults.h ( GA Library, C++)
 *
 * COMMENTS:	Defaults for attribute data
 */

#ifndef __GA_Defaults__
#define __GA_Defaults__

#include "GA_API.h"
#include "GA_Types.h"
#include <UT/UT_Defaults.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_StaticAssert.h>

class UT_JSONWriter;
class UT_JSONParser;
class UT_Options;
class UT_StringRef;

#define GA_DEFAULT_BUFSIZE 4

#define USE_UT_DEFAULTS 1

///@brief Class which stores the default values for a GA_Attribute
///
/// This class stores a list of integer/real values which can be used to
/// initialize attribute data.
#if USE_UT_DEFAULTS
class GA_API GA_Defaults : public UT_Defaults
#else
class GA_API GA_Defaults
#endif
{
public:
    typedef UT_Defaults Base;

    /// Default constructor -- the default will be 0.
#if USE_UT_DEFAULTS
    GA_Defaults() : Base() {}
#else
    GA_Defaults() { ctorInit(); }
#endif

    /// Construct by extracting data out of a UT_Options
    explicit GA_Defaults(const UT_Options *options);

    /// Construct a set of defaults from variadic arguments.  For example
    /// @code
    ///	  GA_Defaults	a(GA_STORE_INT32, 3, 0, 1, 2);
    ///	  GA_Defaults	a(GA_STORE_INT64, 2, int64(0), int64(1));
    ///	  GA_Defaults	b(GA_STORE_REAL64, 1, fpreal64(0));
    /// @endcode
    /// 
    /// This style of construction is a little bit dangerous, so please ensure
    ///    - All data arguments are the right type
    ///    - You have the proper count of variadic arguments
    ///    - You use only one of the supported types: INT32, INT64 or REAL64
    ///      All other types will either cause compiler errors or crashing
    /// Otherwise, core dumping may occur.
    GA_Defaults(GA_Storage storage, int count, ...);

    /// @{
    /// Type classes to construct an identity matrix as a default
    class matrix3 {};
    class matrix4 {};
    GA_Defaults(matrix3);
    GA_Defaults(matrix4);

    /// @{
    /// Construct by repeating the value
    explicit GA_Defaults(int64 value)	// Repeated value
#if USE_UT_DEFAULTS
        : Base(value)
#endif
    {
#if !USE_UT_DEFAULTS
	myData.myInt = myBuffer.myInt;
	myBuffer.myInt[0] = value;
	mySize = 1;
	myStorage = GA_STORE_INT64;
#endif
    }
    explicit GA_Defaults(int32 value)	// Repeated value
#if USE_UT_DEFAULTS
        : Base(value)
#endif
    {
#if !USE_UT_DEFAULTS
        myData.myInt = myBuffer.myInt;
	myBuffer.myInt[0] = value;
	mySize = 1;
	myStorage = GA_STORE_INT64;
#endif
    }
    /// Construct by repeating the value
    explicit GA_Defaults(fpreal64 value)	// Repeated value
#if USE_UT_DEFAULTS
        : Base(value)
#endif
    {
#if !USE_UT_DEFAULTS
        myData.myReal = myBuffer.myReal;
	myBuffer.myReal[0] = value;
	mySize = 1;
	myStorage = GA_STORE_REAL64;
#endif
    }
    explicit GA_Defaults(fpreal32 value)	// Repeated value
#if USE_UT_DEFAULTS
        : Base(value)
#endif
    {
#if !USE_UT_DEFAULTS
        myData.myReal = myBuffer.myReal;
	myBuffer.myReal[0] = value;
	mySize = 1;
	myStorage = GA_STORE_REAL64;
#endif
    }
    /// @}

    /// @{
    /// Construct by passing a vector of scalar values
    /// Note that the defaults will @b copy the data out of the values passed
    /// in (so the array doesn't have to be persistent).
#if USE_UT_DEFAULTS
    GA_Defaults(const int32 *values, int count) : Base(values, count) {}
    GA_Defaults(const int64 *values, int count) : Base(values, count) {}
    GA_Defaults(const fpreal32 *values, int count) : Base(values, count) {}
    GA_Defaults(const fpreal64 *values, int count) : Base(values, count) {}
#else
    GA_Defaults(const int32 *values, int count);
    GA_Defaults(const int64 *values, int count);
    GA_Defaults(const fpreal32 *values, int count);
    GA_Defaults(const fpreal64 *values, int count);
#endif
    /// @}

    /// Copy constructor
#if USE_UT_DEFAULTS
    GA_Defaults(const GA_Defaults &src) : Base((const Base &)src) {}
    ~GA_Defaults() {}

    GA_Defaults	&operator=(const GA_Defaults &src)
    {
        *((Base*)this) = (const Base&)src;
        return *this;
    }
#else
    GA_Defaults(const GA_Defaults &src)
    { ctorInit(); copy(src); }
    ~GA_Defaults()
    { clear(); }

    GA_Defaults	&operator=(const GA_Defaults &src)
    { copy(src); return *this; }
#endif

#if USE_UT_DEFAULTS
    /// Copy values from another set of defaults
    SYS_FORCE_INLINE void copy(const GA_Defaults &src)
    {
        *this = src;
    }
#else
    /// Report memory usage
    int64	getMemoryUsage(bool inclusive) const;

    /// Copy values from another set of defaults
    SYS_FORCE_INLINE void	copy(const GA_Defaults &src)
    {
	// Free our buffer if present.
	clear();
	if (src.mySize)
	{
	    if (src.mySize <= GA_DEFAULT_BUFSIZE)
	    {
		// Build inline!
		myData.myInt = myBuffer.myInt;
		SYS_STATIC_ASSERT(GA_DEFAULT_BUFSIZE == 4);
		myData.myInt[0] = src.myData.myInt[0];
		myData.myInt[1] = src.myData.myInt[1];
		myData.myInt[2] = src.myData.myInt[2];
		myData.myInt[3] = src.myData.myInt[3];
		myStorage = src.myStorage;
		mySize = src.mySize;
	    }
	    else
	    {
		switch (src.myStorage)
		{
		    case GA_STORE_INT64:
			set(src.myData.myInt, src.mySize);
			break;
		    case GA_STORE_REAL64:
			set(src.myData.myReal, src.mySize);
			break;
		    default:
			UT_ASSERT(0);
		}
	    }
	}
    }
#endif

    /// Set the GA_OPTION_DEFAULTS option in the options
    /// If @c always is false and there are no defaults (i.e. getSize() == 0),
    /// then the options will not be written.
    void	saveToOptions(UT_Options &options, bool always=true) const;

    /// @{
    /// Get a value out of the defaults.
    /// - Automatically casts defaults into appropriate return type
    /// - Clamps index to tuple size
    /// - If there is no default data, then, the result will be 0
#if !USE_UT_DEFAULTS
    int64	 getI(int index=0) const;
    fpreal	 getF(int index=0) const;
#endif
    void	 get(int index, int32 &v) const
		    { v = getI(index); }
    void	 get(int index, int64 &v) const
		    { v = getI(index); }
    void	 get(int index, fpreal32 &v) const
		    { v = getF(index); }
    void	 get(int index, fpreal64 &v) const
		    { v = getF(index); }
    /// @}

    /// Set defaults to the list of integer values.
    void	 set(const int32 *values, int count);
    /// Set defaults to the list of integer values.
    void	 set(const int64 *values, int count);
    /// Set defaults to the list of real values.
    void	 set(const fpreal32 *values, int count);
    /// Set defaults to the list of real values.
    void	 set(const fpreal64 *values, int count);

    /// Set this to a reasonable guess based on the specified name,
    /// e.g. Cd -> 1.0; id -> -1; orient -> (0,0,0,1)
    void guessFromName(const UT_StringRef &name, int tuple_size);

    /// Get the size of the defaults.
    /// You can have an attribute tuple size that's
    /// larger than its GA_Defaults size; the GA_Defaults
    /// will repeat its last element.
    SYS_FORCE_INLINE
    int          size() const
    {
#if USE_UT_DEFAULTS
        return getTupleSize();
#else
        return mySize;
#endif
    }

    /// Save data to a JSON stream.
    /// @section JSON-GA_Defaults JSON Schema: GA_Defaults
    /// @code
    /// {
    ///     "name"          : "GA_Defaults",
    ///     "description"   : "An array of numbers used for attribute defaults",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "size": {
    ///             "type"  : "integer",
    ///             "description" : "Number of values",
    ///         },
    ///         "storage": {
    ///             "type"  : "string",
    ///             "enum"  : ["fpreal64", "int64"],
    ///             "description" : "The type data",
    ///         },
    ///         "values": {
    ///             "type"  : "array",
    ///             "items" : "number",
    ///             "description" : "The default values",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	 jsonSave(UT_JSONWriter &w) const;

    /// Load data from a JSON stream
    bool	 jsonLoad(UT_JSONParser &p);

#if !USE_UT_DEFAULTS
private:
    SYS_FORCE_INLINE void	 clear()
    {
	if (mySize > GA_DEFAULT_BUFSIZE)
	{
	    switch (myStorage)
	    {
		case GA_STORE_INT64:
		    UT_ASSERT(myData.myInt != myBuffer.myInt);
		    delete [] myData.myInt;
		    break;
		case GA_STORE_REAL64:
		    UT_ASSERT(myData.myReal != myBuffer.myReal);
		    delete [] myData.myReal;
		    break;
		default:
		    UT_ASSERT(0);
	    }
	}
	mySize = 0;
    }
    SYS_FORCE_INLINE void	 ctorInit()
		 {
		     myData.myInt = 0;
		     myStorage = GA_STORE_INT64;
		     mySize = 0;
		 }

    union {
	int64		*myInt;
	fpreal64	*myReal;
    } myData;
    union {
	int64		 myInt[GA_DEFAULT_BUFSIZE];
	fpreal64	 myReal[GA_DEFAULT_BUFSIZE];
    } myBuffer;
    GA_Storage		myStorage;
    int			mySize;
#endif
};

#endif

