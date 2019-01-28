/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONHandle.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_JSONHandle__
#define __UT_JSONHandle__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_String.h>
#include <iosfwd>

class UT_JSONParser;
class UT_IStream;
class UT_WorkBuffer;
class UT_StringHolder;

/// @brief UT_JSONHandle processes events from a UT_JSONParser parser
///
/// The UT_JSONParser is passed a UT_JSONHandle to interpret the JSON tokens.
/// As each entity is encountered, an event callback is invoked on the handle
/// to process the token.
///
/// It's possible to recurse into the parser from within a callback with a
/// different handler and possibly even a different input stream by calling
/// UT_JSONParser::parseObject().
///
/// @see UT_JSONParser, UT_JSONHandleError, UT_JSONHandleNull, UT_JSONIStream

class UT_API UT_JSONHandle {
public:
	     UT_JSONHandle() {}
    virtual ~UT_JSONHandle() {}

    /// Convenience method to parse a single object
    bool		parseObject(UT_JSONParser &p, UT_IStream *is=0);

    /// The Null Handle acts as a pass through when parsing a JSON file.  It
    /// can be used for simple syntax checking:
    /// @code
    ///    UT_JSONHandle        *h = UT_JSONHandle::getNullHandle();
    ///    UT_JSONParser         p;
    ///    success = p.parseObject(*h);
    ///    UT_JSONHandle::releaseNullHandle(h);
    /// @endcode
    static UT_JSONHandle	*getNullHandle();
    /// Instead of deleting the null handle, please call releaseNullHandle()
    static void			 releaseNullHandle(UT_JSONHandle *h);

    /// Event method to process a null token
    virtual bool	jsonNull(UT_JSONParser &p) = 0;
    /// Event method to process a bool (true or false tokens)
    virtual bool	jsonBool(UT_JSONParser &p, bool value) = 0;

    /// Event method to process an integer
    virtual bool	jsonInt(UT_JSONParser &p, int64 value) = 0;
    /// Event method to process a real/float
    virtual bool	jsonReal(UT_JSONParser &p, fpreal64 value) = 0;

    /// Event method to process a string value
    virtual bool	jsonString(UT_JSONParser &p, const char *value,
				int64 len) = 0;
    /// The string event taking a UT_StringHolder calls the jsonString() method
    /// by default.  However, if you can process a UT_StringHolder instead, you
    /// can get a direct reference to the UT_StringHolder.
    virtual bool	jsonStringHolder(UT_JSONParser &p,
				const UT_StringHolder &s);
    /// Event method to process the key of a map/object is read
    virtual bool	jsonKey(UT_JSONParser &p, const char *v, int64 len)=0;
    virtual bool	jsonKeyHolder(UT_JSONParser &p,
				const UT_StringHolder &s);

    /// Event method invoked at the start of a map/object
    virtual bool	jsonBeginMap(UT_JSONParser &p) = 0;
    /// Event method invoked at the end of a map/object
    virtual bool	jsonEndMap(UT_JSONParser &p) = 0;
    /// Event method invoked at the beginning of an array object
    virtual bool	jsonBeginArray(UT_JSONParser &p) = 0;
    /// Event method invoked at the end of an array object
    virtual bool	jsonEndArray(UT_JSONParser &p) = 0;

    /// Event method to handle  for uniform array of bool (encoded as bit
    /// fields)
    virtual bool	uaBool(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of int8 data
    virtual bool	uaInt8(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of int16 data
    virtual bool	uaInt16(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of int32 data
    virtual bool	uaInt32(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of int64 data
    virtual bool	uaInt64(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of real16 data
    virtual bool	uaReal16(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of real32 data
    virtual bool	uaReal32(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of real64 data
    virtual bool	uaReal64(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of unsigned uint8 data
    virtual bool	uaUInt8(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of unsigned uint16 data
    virtual bool	uaUInt16(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of string data (use uaReadString)
    virtual bool	uaString(UT_JSONParser &p, int64 length);
    /// Event method to handle uniform array of string token data (use
    /// uaReadStringToken)
    virtual bool	uaStringToken(UT_JSONParser &p, int64 length);

    /// This method is called by addError to create the prefix for error
    /// messages.  By default, the method looks something like:
    /// @code
    ///    msg.sprintf("JSON ERROR[%ld]:", (long)p.getStreamPosition());
    /// @endcode
    virtual void	errorPrefix(UT_JSONParser &p, UT_WorkBuffer &msg);

protected:
    /// Convenience method to test key values
    bool	isKey(const char *s1, const char *s2) const
		    { return !SYSstrcasecmp(s1, s2); }
    /// Convenience method to add an error about a bad key name
    ///   addError("Unexpected key token '%s'" % key);
    /// This also skips the next object.
    void	skipBadKey(UT_JSONParser &p, const char *key);
    /// Convenience method to add a fatal error about a bad key name
    ///   addError("Unexpected key token '%s'" % key);
    void	fatalBadKey(UT_JSONParser &p, const char *key);
};

/// @brief This class generates errors on any JSON events.
///
/// The UT_JSONHandleError class will throw errors on any JSON events.  It's
/// useful as a base class for an event handler which only expects certain
/// events.  For example, if you only expect the keyword argument, you might
/// have a class like: @code
///    class MyHandle : public UT_JSONHandleError {
///        virtual bool jsonKey(UT_JSONParser &p, const char *v, int64 len);
///        virtual const char   *className() { return "MyClass" }
///    }
/// @endcode
/// The base class would handle trapping errors on anything other than a
/// map key.
///
/// @note Uniform arrays are handled by the UT_JSONHandle base class
///
/// @see UT_JSONHandle, UT_JSONHandleNull 
class UT_API UT_JSONHandleError : public UT_JSONHandle {
public:
    /// @param fatal @n
    /// If fatal is true then errors will cause parsing to terminate.
    /// Otherwise, parsing will continue and errors will cascade.
	     UT_JSONHandleError(bool fatal=true) : myFatal(fatal) {}
    virtual ~UT_JSONHandleError() {}

    /// getClassName() should return a meaningful label.  The errors generated
    /// will be:
    ///	   Error parsing %s: Unexpected "type" JSON object
    /// The default is an empty string.
    virtual const char	*getClassName() const;

    virtual bool	jsonNull(UT_JSONParser &p);
    virtual bool	jsonBool(UT_JSONParser &p, bool value);
    virtual bool	jsonInt(UT_JSONParser &p, int64 value);
    virtual bool	jsonReal(UT_JSONParser &p, fpreal64 value);
    virtual bool	jsonString(UT_JSONParser &p, const char *v, int64 len);
    virtual bool	jsonKey(UT_JSONParser &p, const char *v, int64 len);
    virtual bool	jsonBeginMap(UT_JSONParser &p);
    virtual bool	jsonEndMap(UT_JSONParser &p);
    virtual bool	jsonBeginArray(UT_JSONParser &p);
    virtual bool	jsonEndArray(UT_JSONParser &p);
protected:
    /// Add a type error for the given name
    virtual bool	typeError(UT_JSONParser &p, const char *name);
    /// Check whether errors should be fatal (or just warnings)
    bool		getFatal() const	{ return myFatal; }
    /// Set whether errors should be fatal (or just warnings)
    void		setFatal(bool f)	{ myFatal = f; }

private:
    /// Whether errors terminate parsing.
    bool		myFatal;
};

/// @brief This class skips over any JSON events.
///
/// The UT_JSONHandleNull will simply skip over any JSON events.
/// useful as a base class for an event handler which only expects certain
/// events.  For example, if you only expect the keyword argument, you might
/// have a class like: @code
///    class MyHandle : public UT_JSONHandleNull {
///        virtual bool jsonInt(UT_JSONParser &p, int64 len);
///        virtual bool jsonReal(UT_JSONParser &p, fpreal64 len);
///        virtual const char   *className() { return "MyClass" }
///    }
/// @endcode
/// The base class would skip over anything other than an integer or real.
///
/// @note Uniform arrays are handled by the UT_JSONHandle base class
///
/// @see UT_JSONHandle, UT_JSONHandleError
class UT_API UT_JSONHandleNull : public UT_JSONHandle {
public:
	     UT_JSONHandleNull() {}
    virtual ~UT_JSONHandleNull() {}
    virtual bool	jsonNull(UT_JSONParser &p);
    virtual bool	jsonBool(UT_JSONParser &p, bool value);
    virtual bool	jsonInt(UT_JSONParser &p, int64 value);
    virtual bool	jsonReal(UT_JSONParser &p, fpreal64 value);
    virtual bool	jsonString(UT_JSONParser &p, const char *v, int64 len);
    virtual bool	jsonKey(UT_JSONParser &p, const char *v, int64 len);
    virtual bool	jsonBeginMap(UT_JSONParser &p);
    virtual bool	jsonEndMap(UT_JSONParser &p);
    virtual bool	jsonBeginArray(UT_JSONParser &p);
    virtual bool	jsonEndArray(UT_JSONParser &p);
};


#endif
