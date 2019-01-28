/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_WorkBuffer.h ( Utility Library, C++ )
 *
 * COMMENTS:
 *	A growable string buffer that can be written into.  A UT_String
 *	can be created with the contents of this buffer by calling
 *	copyIntoString().
 *
 *	It's important that a non-const version of the raw buffer is not
 *	accessible since users could write past the end of the allocated
 *	buffer.  Also note that the buffer location can change as it grows,
 *	so don't keep pointers to the buffer around.
 *
 *	Most of the time, you want to allocate an object of this class on
 *	the stack and not on the heap.
 *
 *	The buffer is kept null terminated by default.  Functions exist
 *	to verify this.  Note that the "length" of the buffer is the
 *	same as strlen - ie: it ignores the null termination!!!
 */

#ifndef __UT_WorkBuffer_h__
#define __UT_WorkBuffer_h__

#include "UT_API.h"

#include "UT_Assert.h"
#include "UT_Defines.h"
#include "UT_Format.h"
#include "UT_NonCopyable.h"
#include "UT_String.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"
#include "UT_Swap.h"
#include "UT_Unicode.h"

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// The default page size on most systems is 4K.  We choose a default
// buffer size less than half of that in the hopes that if we have
// functions with 2 work buffers or additional variables on the stack that we
// may not have to allocate multiple stack pages.
#define UT_INITIAL_BUFFER_SIZE	2000

class UT_WorkArgs;
class UT_IStream;

class UT_API UT_WorkBuffer
{
public:
    typedef char	    value_type;
    
    SYS_FORCE_INLINE
    UT_WorkBuffer()
	: myBuffer(myStackBuffer)
    {
	// Default termination.
	myBuffer[0] = '\0';
    }

    SYS_FORCE_INLINE
    explicit UT_WorkBuffer(const char *str)
	: myBuffer(myStackBuffer)
    {
	myBuffer[0] = '\0';
	append(str);
    }

    SYS_FORCE_INLINE
    explicit UT_WorkBuffer(const UT_String &str)
	: myBuffer(myStackBuffer)
    {
	myBuffer[0] = '\0';
	append(str);
    }

    SYS_FORCE_INLINE
    explicit UT_WorkBuffer(const UT_StringRef &str)
	: myBuffer(myStackBuffer)
    {
	myBuffer[0] = '\0';
	append(str);
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer(const UT_WorkBuffer &other)
	: myBuffer(myStackBuffer)
    {
	myBuffer[0] = '\0';
	append(other);
    }

    SYS_FORCE_INLINE
    ~UT_WorkBuffer()
    {
	if (myBuffer != myStackBuffer)
	{
	    UT_ASSERT(myBuffer);
	    ::free(myBuffer);
	}
    }

    /// Create a work buffer to contain a UTF-16LE (little endian)
    /// representation of the incoming UTF-8 string.
    /// The work buffer will be zero-word terminated.
    SYS_FORCE_INLINE
    static UT_WorkBuffer
    widen(const utf8 *str)
    {
	return UT_WorkBuffer(do_widen(), str);
    }

    /// Create a work buffer to contain the UTF-8 representation of the
    /// incoming UTF-16 string.  The UTF-16 string is assumed to be
    /// little-endian, unless prefixed with BOM that indicates endianness.
    /// The incoming string should be zero-word terminated.
    SYS_FORCE_INLINE
    static UT_WorkBuffer
    narrow(const utf16 *str)
    {
	return UT_WorkBuffer(do_narrow(), str);
    }

    // It's important that there is no non-const access method to the buffer.
    // Also note that the pointer to the buffer can change if the buffer
    // grows.
    SYS_FORCE_INLINE
    const char	*buffer() const { return myBuffer; }

    // Having said that, if you need a non-const pointer you must lock
    // the string. This prohibits ANY update which changes the myLength
    // variable (and thus potentially a realloc)
    // You must release the buffer before any such changes.
    // The work buffer continues to own the memory and will free it when
    // it goes out of scope so don't think this is the same as a "steal"
    // in UT_String.
    // Currently, to ensure people couple their locks & releases,
    // it asserts there is no unaccounted locks on death.  This is so
    // people who think it is steal find out otherwise.
    // Offset is where in the string to get the pointer from.
    // This is only to be used when absolutely necessary.
    // When releasing, if you have a string buffer, and you have modified the
    // length, you should set the recomputeLength flag to 1.  This will adjust
    // the internal length variable so that further concatenations will work
    // properly.
    // The reserve_bytes parameter tells the lock to ensure that there are at
    // least that many bytes in the locked buffer.
    // NOTE: Unlike other UT_WorkBuffer functions, it is the user's  
    // responsibility to maintain a NUL termination guarantee when manipulating 
    // the raw buffer.
    char	*lock(exint offset = 0, exint reserve_bytes=0);
    void	 release(int recomputeLength=0);
    void	 releaseSetLength(exint new_length);

    SYS_FORCE_INLINE
    exint	 getAllocatedSize() const { return myAllocatedSize; }

    /// Class to handle auto-locking of the UT_WorkBuffer.  This is not related
    /// to multi-threading, but to the lock/release methods above.
    ///
    /// You should never append data to a locked buffer.
    class AutoLock
    {
    public:
	SYS_FORCE_INLINE
	AutoLock(UT_WorkBuffer &buf)
	    : myBuffer(buf)
	{
	    myString = myBuffer.lock();
	}
	SYS_FORCE_INLINE
	~AutoLock()
	{
	    release();
	}
	/// @{
	/// Get access to the non-const buffer.  This may return nullptr if the
	/// lock has been released.
	SYS_FORCE_INLINE
	char	*operator*() const { return myString; }
	SYS_FORCE_INLINE
	char	*string() const { return myString; }
	/// @}

	/// You can manually release the buffer
	SYS_FORCE_INLINE
	void	 release(bool recompute_length=false)
	{
	    if (myString)
	    {
		myBuffer.release(recompute_length);
		myString = nullptr;
	    }
	}
	/// If you've manually released the lock, you can relock the buffer
	SYS_FORCE_INLINE
	void	relock()
	{
	    UT_ASSERT(!myString);
	    myString = myBuffer.lock();
	}
    private:
	UT_WorkBuffer	&myBuffer;
	char		*myString;
    };

    void	 reserve(exint bytes=0);

    // This is a read only operator.  We are avoiding the writeable
    // versions as they lead to problems when people do a:
    // foo[pastend] = foo(start)
    // causing an implicit realloc.
    SYS_FORCE_INLINE
    char	 operator()(exint idx) const
		 {
		     // We allow an index at myLength as if we have a null
		     // terminated buffer that is the null termination.
		     UT_ASSERT_P(idx >= 0 && idx <= myLength);
		     return myBuffer[idx];
		 }

    // Returns last character. Only valid if !isEmpty()
    SYS_FORCE_INLINE
    char	 first() const
		 {
		     UT_ASSERT_P(myLength > 0);
		     return myBuffer[0];
		 }
    // Returns last character. Only valid if !isEmpty()
    SYS_FORCE_INLINE
    char	 last() const
		 {
		     UT_ASSERT_P(myLength > 0);
		     return myBuffer[myLength - 1];
		 }

    // This should always be true.  It's here to act as a sanity function.
    int		 isNullTerminated() const;

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator=(const UT_WorkBuffer &other)
		 {
		     strcpy(other);
		     return *this;
		 }
    SYS_FORCE_INLINE
    UT_WorkBuffer &operator=(const char *str)
                 {
                     clear();
                     append(str);
                     return *this;
                 }
    SYS_FORCE_INLINE
    UT_WorkBuffer &operator=(const std::string &str)
                 {
                     clear();
                     append(str.c_str(), str.length());
                     return *this;
                 }

    /// Comparison operator. Null strings are considered as empty strings.
    /// @{
    SYS_FORCE_INLINE
    bool	 operator==(const char *str) const
		 {
		    if (!str)
			return isEmpty();
		    return (::strcmp(str, myBuffer) == 0);
		 }
    SYS_FORCE_INLINE
    bool	 operator==(const UT_String &str) const
		 {
		    if (!(const char *)str)
			return isEmpty();
		    return (::strcmp(str, myBuffer) == 0);
		 }
    SYS_FORCE_INLINE
    bool	 operator==(const UT_WorkBuffer &buf) const
		 {
		    if (buf.isEmpty())
			return isEmpty();
		    if (length() != buf.length())
			return false;
		    return (::memcmp(myBuffer, buf.myBuffer, myLength) == 0);
		 }
    SYS_FORCE_INLINE
    bool	 operator!=(const char *str) const
		 {
		    return !(*this == str);
		 }
    SYS_FORCE_INLINE
    bool	 operator!=(const UT_String &str) const
		 {
		    return !(*this == str);
		 }
    SYS_FORCE_INLINE
    bool	 operator!=(const UT_WorkBuffer &buf) const
		 {
		    return !(*this == buf);
		 }
    /// @}

private:
    // Reallocate the buffer until the allocated size is >= the length.  This
    // private method needs to come first so it can be inlined.
    void	 growBufferIfNeeded()
    {
	// Using a while loop instead of computing an accurate size the
	// first time is slower, but most of the time the loop will execute
	// at most once.
	// We need to use myLength+1 as we need room for the null.
	while (myLength+1 > myAllocatedSize)	// false most of the time
	    reserve(myAllocatedSize * 2);
    }

public:
    // These are standard string operators people tend to use:
    SYS_FORCE_INLINE
    void	 strcpy(const char *src)
    {
	clear();
	append(src);
    }
    SYS_FORCE_INLINE
    void	 strcpy(const UT_String &src)
    {
	clear();
	append(src);
    }
    SYS_FORCE_INLINE
    void	 strcpy(const UT_StringRef &src)
    {
	clear();
	append(src);
    }
    SYS_FORCE_INLINE
    void	 strcpy(const UT_WorkBuffer &src)
    {
	clear();
	append(src);
    }

    // NOTE: unlike strncpy(), maxlen does not include the null terminator.
    SYS_FORCE_INLINE
    void	 strncpy(const char *src, exint maxlen)
    {
	clear();
	// Ensure we have enough room:
	myLength = maxlen+1;
	growBufferIfNeeded();
	myLength = 0;
	SYSstrlcpy(myBuffer, src, maxlen+1);
	myLength = ::strlen(myBuffer);
    }

    // Note we can't just return myLength as there may be embedded NULLs.
    SYS_FORCE_INLINE
    exint	 strlen() const
    {
	UT_ASSERT_P(isNullTerminated());
	return ::strlen(myBuffer);
    }

    SYS_FORCE_INLINE
    exint	 length() const
    {
	return myLength;
    }

    SYS_FORCE_INLINE
    void	 strcat(const char *src)
    {
	append(src);
    }

    // protectedStrcat() will quote the string in double quotes if required and
    // protect any enclosed double quotes or backslashes in the source.  It
    // will not escape any other characters.
    void	 protectedStrcat(const char *str, bool force_quote=false);

    // fullyProtected*Strcat() is similar to protectedStrcat, except it escapes
    // any non-printable characters.  It will not escape single quotes, and if
    // force_quote is true, it will add double-quotes.  It will work with
    // arbitrary binary data and uses the \xNN syntax to encode bytes.
    // UT_IStream::read() is capable of loading strings encoded with this
    // method, and these strings can also be decoded in Python.  If
    // fullyProtectedBinaryStrcat is called, this method can handle data
    // containing null characters.
    void	 fullyProtectedStrcat(const char *str, bool force_quote=false);
    void	 fullyProtectedBinaryStrcat(
			const char *str, exint size, bool force_quote=false);

    /// Append a string of a given maximum length to the current string.
    /// Unlike the POSIX's strncat(3), we ignore any NUL bytes in the current 
    /// string and blindly append at the end of the work buffer.
    SYS_FORCE_INLINE
    void	 strncat(const char *src, exint len)
    {
	if (!src)
	    return;
	append(src, ::strnlen(src, len));
    }

    // Extract the first argument from the src and append it to the work
    // buffer.  This does NOT handle quotes properly (i.e. if the first word
    // is quoted with spaces).
    void	 strcatFirstWord(const char *src);

    SYS_FORCE_INLINE
    int		 strcmp(const char *src) const
    {
	UT_ASSERT_P(isNullTerminated());
	return ::strcmp(myBuffer, src);
    }

    SYS_FORCE_INLINE
    int		 strncmp(const char *src, exint n) const
    {
	UT_ASSERT_P(isNullTerminated());
	return ::strncmp(myBuffer, src, n);
    }

    SYS_FORCE_INLINE
    char	*strdup() const
    {
	UT_ASSERT(isNullTerminated());
	return ::strdup(myBuffer);
    }

    // Reset the buffer to an empty buffer.
    SYS_FORCE_INLINE
    void	 clear()
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	myLength = 0;
	myBuffer[0] = '\0';
    }

    SYS_FORCE_INLINE
    bool	 isEmpty() const
    {
	return (myLength == 0);
    }
    SYS_FORCE_INLINE
    bool	 isstring() const
    {
	return !isEmpty();
    }

    // Write into the buffer at a specific place.
    // This WILL expand the buffer if it is required and keep it null
    // terminated.
    SYS_FORCE_INLINE
    void	 write(exint offset, char c)
    {
	UT_ASSERT(offset >= 0);
	if (offset < 0) return;
	if (offset >= myLength)
	{
	    if (myLockCount) { UT_ASSERT(0); return; }
	    myLength = offset+1;
	    growBufferIfNeeded();
	    myBuffer[myLength] = '\0';
	}
	myBuffer[offset] = c;
	if (c == '\0')
	    myLength = offset;
    }

    // This does NOT write out the trailing NULL of src, but the buffer will
    // still be null-terminated.
    void	 write(exint offset, const char *src)
    {
	while (*src)
	{
	    write(offset, *src);
	    src++;
	    offset++;
	}
    }

    void         write(exint offset, const UT_StringHolder &src)
    {
        write(offset, src.c_str());
    }

    /// Load an entire file into the buffer.  Returns @b false if there was an
    /// error reading the file
    bool	readFile(const char *filename);

    // Read a line from an istream -- no matter how long the line is
    // Returns 0 if the stream read failed or 1 otherwise
    bool	 getline(std::istream &is);
    bool	 getline(FILE *fp);

    // Much like getline() except that it has more features. The string itself
    // is tokenized which the UT_WorkArgs points into.
    //   line_num is incremented for each line read.
    //   comment_chars is list of characters to treat as comments.
    //     this can be NULL if we don't want this feature.
    // Returns false if the stream read failed.
    bool	 cmdGetLine(std::istream &is, UT_WorkArgs &args, int &line_num,
			    const char *comment_chars = "#",
			    const char *separators = " \t\n\r");
    bool	 cmdGetLine(UT_IStream &is, UT_WorkArgs &args, int &line_num,
			    const char *comment_chars = "#",
			    const char *separators = " \t\n\r");
    bool	 cmdGetLine(FILE *fp, UT_WorkArgs &args, int &line_num,
			    const char *comment_chars = "#",
			    const char *separators = " \t\n\r");

    int		 sprintf(const char *fmt, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
    int		 appendSprintf(const char *fmt, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    int		 vsprintf(const char *fmt, va_list ap);

    /// Replace the contents of the work buffer using the same formatting as
    /// UTformat.
    /// Returns the size of the appended portion, in bytes.
    template<typename... Args>
    size_t	 format(const char *fmt, const Args &...args)
    {
        clear();
        return appendFormat(fmt, args...);
    }

    /// Append to the work buffer using the same formatting as UTformat.
    /// Returns the size of the appended portion, in bytes.
    template<typename... Args>
    size_t	 appendFormat(const char *fmt, const Args &...args)
    {
	if (myLockCount) { UT_ASSERT(0); return 0; }
	UT_ASSERT_P(isNullTerminated());

	using namespace UT::Format;
	Writer w;
	Formatter<Writer> f;
	size_t nb_needed = f.format(w, fmt, {args...});

	myLength += nb_needed;
	growBufferIfNeeded();

	// Format again, this time to fill in the buffer.
	w.setBuffer(myBuffer + myLength - nb_needed, nb_needed);
	f.format(w, fmt, {args...});

	myBuffer[myLength] = '\0';
	return nb_needed;
    }

    /// Replace the contents of the work buffer using UTformat formatting
    /// with an implicit "{} " for each argument, giving a Python-style
    /// print result.
    template<typename... Args>
    size_t	 print(const Args &...args)
    {
        clear();
        return appendPrint(args...);
    }

    /// Append to the work buffer using the UTformat with an implicit "{} "
    /// format for each parameter.
    /// Returns the size of the appended portion, in bytes.
    template<typename... Args>
    size_t	 appendPrint()
    {
	return 0;
    }
    template<typename T, typename... Args>
    size_t	 appendPrint(const T &value, const Args &...args)
    {
	size_t		newbytes;
	newbytes = appendFormat("{} ", value);
	newbytes += appendPrint(args...);
	return newbytes;
    }

    // These tack stuff to the end of the buffer.
    SYS_FORCE_INLINE
    void	 append(char character)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	myLength++;
	growBufferIfNeeded();
	myBuffer[myLength - 1] = character;
	myBuffer[myLength] = '\0';
    }

    void	printMemory(int64 mem)	{ clear(); appendPrintMemory(mem); }
    void	appendPrintMemory(int64 mem);

    void	 append(exint n, char character)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	myLength += n;
	growBufferIfNeeded();
	for (int i = n; i > 0; i--)
	    myBuffer[myLength - i] = character;
	myBuffer[myLength] = '\0';
    }
    
    /// Append a single Unicode code point, converted to UTF8
    void 	 append(utf32 cp)
    {
	utf8	buf[UT_UTF8_MAX_ENCODING_LEN];
	int len = UT_Unicode::convert(cp, buf, sizeof(buf));
	if (!len)
	    return;
	
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	myLength += len;
	growBufferIfNeeded();
	::memcpy(myBuffer + myLength - len, buf, len);
	myBuffer[myLength] = '\0';
    }

    void	 append(const char *data, exint size)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(data);
	UT_ASSERT_P(isNullTerminated());
	myLength += size;
	growBufferIfNeeded();
	::memcpy(myBuffer + myLength - size, data, size);
	myBuffer[myLength] = '\0';
    }

    SYS_FORCE_INLINE
    void	 append(const char *str)
    {
	if( UTisstring(str) )
	    append(str, ::strlen(str));
    }

    SYS_FORCE_INLINE
    void	 append(const UT_String &str)
    {
	if (str.isstring())
	    append((const char *)str);
    }

    SYS_FORCE_INLINE
    void	 append(const UT_StringRef &str)
    {
	if (str.isstring())
	    append(str.buffer(), str.length());
    }

    void	 append(const UT_StringArray &strs, const UT_StringRef &sep)
    {
	for (exint i = 0; i < strs.entries(); i++)
	{
	    append(strs(i));
	    if (i+1 < strs.entries())
		append(sep);
	}
    }

    SYS_FORCE_INLINE
    void	 append(const UT_WorkBuffer &wb)
    {
	append( wb.buffer(), wb.length() );
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator+=(const char *str)
    {
        append(str);
        return *this;
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator+=(const std::string &str)
    {
        append(str);
        return *this;
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator+=(const UT_WorkBuffer &wb)
    {
        append(wb);
        return *this;
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator+=(const UT_String &str)
    {
        append(str);
        return *this;
    }

    SYS_FORCE_INLINE
    UT_WorkBuffer &operator+=(const UT_StringRef &str)
    {
        append(str);
        return *this;
    }

    void	prepend(char character)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	myLength++;
	growBufferIfNeeded();
	::memmove(myBuffer+1, myBuffer, myLength);
	myBuffer[0] = character;
    }
    void	prepend(const char *data, exint size)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(data);
	UT_ASSERT_P(isNullTerminated());
	myLength += size;
	growBufferIfNeeded();
	::memmove(myBuffer+size, myBuffer, myLength+1 - size);
	::memcpy(myBuffer, data, size);
    }
    SYS_FORCE_INLINE
    void	 prepend(const char *str)
    {
	UT_ASSERT_P(str);
	prepend(str, ::strlen(str));
    }

    SYS_FORCE_INLINE
    void	 prepend(const UT_String &str)
    {
	if (str.isstring())
	    prepend((const char *)str);
    }

    /// Insert @c slen characters from @c str, at location @c pos. If @c pos 
    /// exceeds the current length, the position is truncated and to an append.
    void         insert(exint pos, const char* str, exint slen);
    
    /// Erase @c len characters from location @c pos in the string. 
    void	 erase(exint pos, exint len);

    void	 rewind()	{ backup(myLength); }

    /// Rewind by the given length
    SYS_FORCE_INLINE
    void	 backup(exint by_length)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	UT_ASSERT_P(by_length >= 0);
	myLength -= by_length;
	UT_ASSERT(myLength >= 0);
	myBuffer[myLength] = '\0';
    }

    /// Truncate the buffer to the specified length.  Truncating to 0 is
    /// identical to clear().
    SYS_FORCE_INLINE
    void	 truncate(exint new_length)
    {
	if (new_length >= myLength)
	{
	    UT_ASSERT(0 && "Truncating beyond buffer extent");
	    return;
	}
	backup(myLength-new_length);
    }

    // Delete characters off the end of the string until we hit the
    // requested character.
    void	 backupTo(char c)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	while( myLength > 0 && myBuffer[myLength-1] != c )
	    myLength--;
	myBuffer[myLength] = '\0';
    }

    void	 advance(exint by_length)
    {
	if (myLockCount) { UT_ASSERT(0); return; }
	UT_ASSERT_P(isNullTerminated());
	UT_ASSERT_P(by_length >= 0);
	myLength -= by_length;
	UT_ASSERT(myLength >= 0);
	for (int i=0; i<myLength; i++)
	    myBuffer[i] = myBuffer[by_length+i];
	myBuffer[myLength] = '\0';
    }

    // Finds the 'occurance_number'-th occurance of char c in the string.
    SYS_FORCE_INLINE
    const char *findChar(char c, int occurance_number = 1) const
    {
	return findCharFrom(c, 0, occurance_number);
    }
    // Same as findChar, but searches from the end of the string.
    const char *lastChar(char c, int occurance_number = 1) const
    {
	if (myLockCount) { UT_ASSERT(0); return NULL; }

	UT_ASSERT_P(isNullTerminated());

	for (exint i = myLength; i --> 0;)
	{
	    if(c == myBuffer[i])
	    {
		occurance_number--;
		if(occurance_number <= 0)
		{
		    return (myBuffer + i);
                }
	    }
	}

	return NULL;
    }
    // Same and findChar, bu searches from given position in the string.
    const char *findCharFrom(char c, exint position,
			     int occurance_number = 1) const
    {
	if (myLockCount) { UT_ASSERT(0); return NULL; }

	UT_ASSERT_P(isNullTerminated());

	if (position < 0 || position >= myLength) { return NULL; }

	for(exint i = position; i < myLength; ++i)
	{
	    if(c == myBuffer[i])
	    {
		occurance_number--;
		if(occurance_number <= 0)
		{
		    return (myBuffer + i);
                }
	    }
        }

	return NULL;
    }

    /// Count the occurrences of the text in the current string
    exint	count(const char *needle) const;

    // Get the next token pointed at by string and advance string past the
    // token. Returns whether or not a token was retrieved successfully.
    // Note that string is modified!!!
    bool	 getNextToken(const char *(&string),
				const UT_String separators = " \t\n");

    // Harden the contents of the buffer into a UT_String.
    void	 copyIntoString(UT_String &str) const;

    // Copy the contents into a fixed length buffer.
    // TODO: Get rid of this method, since it encourages fixed-length buffers.
    void	 copyIntoString(char *str, exint max_length) const;

    // Steal the contents of this work buffer into the string.
    void	 stealIntoString(UT_String &str);

    // Steal the contents of this work buffer into the string.
    void	 stealIntoStringHolder(UT_StringHolder &str);

    // Return a string containing the contents of this work buffer, preserving
    // any null characters in it.
    std::string toStdString() const
    { return std::string(buffer(), length()); }

    // Strips the characters after comment_char from the buffer.  This method
    // goes to some effort to enusre that the comment_char is not preceded by
    // a backslash or is not in a quoted string.  Returns true if it found a 
    // comment and modified the buffer, and false otherwise.
    bool	 stripComments(char comment_char = '#');

    /// Strips out all characters found in 'chars'. The string length will be
    /// reduced by the number of characters removed. The number of characters
    /// removed is returned.
    int		 strip(const char *chars);

    /// Remove trailing whitespace lines
    void	 removeTrailingSpaceLines();

    /// Remove trailing whitespace, return true if whitespace was removed.
    bool	 removeTrailingSpace();

    /// Remove trailing digits, return true if some were removed.
    bool	 removeTrailingDigits();
    
    /// Convert string to lower case
    void	 lower();

    /// Convert string to upper case
    void	 upper();

    /// Create a string of tabs & spaces which represents the given indent
    void	 makeIndentString(exint indent, exint tabstop=8);

    /// Remove the first n characters.
    void	 eraseHead(exint n)
    {
	if (n < myLength)
	{
	    myLength -= n;
	    ::memmove(myBuffer, myBuffer + n, myLength);
	}
	else
	    myLength = 0;
	
	myBuffer[myLength] = '\0';
    }

    /// Replace all occurances of 'find' with 'replacement'
    void	 substitute(const char *find, const char *replacement,
                            bool all = true);

    /// Given from_name which is assumed to fit from_pattern, any assigned
    /// wildcards are subsitituted in to_pattern, writing the result to this.
    /// The wildcards may also be indexed.  For example:
    ///
    /// to_pattern = b*            from_name = apple   from_pattern = a*le
    ///     ---> this = bpp
    ///
    /// to_pattern = *(1)_to_*(0)  from_name = a_to_b  from_pattern = *_to_*
    ///     ---> this = b_to_a
    bool subPatterns(
        const char *to_pattern,
        const char *from_name,
        const char *from_pattern);

    /// UTF-16 / UTF-8 conversions.
    
    /// Set the work buffer to contain the UTF-8 representation of the incoming UTF-16 string.
    /// The UTF-16 string is assumed to be little-endian, unless prefixed with BOM that 
    /// indicates endianness.
    /// The incoming string should be zero-word terminated.
    void	 setFromUTF16(const utf16 *str);
    
    /// Set the work buffer to contain a UTF-16LE (little endian) representation of the 
    /// incoming UTF-8 string.
    /// The work buffer will be zero-word terminated.
    void	 setAsUTF16(const utf8 *str);
    
    
    void	 swap(UT_WorkBuffer &other)
    {
	// Warn if we're about to swap locked buffers.
	UT_ASSERT(myLockCount==0);

	bool	this_stack = (myBuffer == myStackBuffer);
	bool	other_stack = (other.myBuffer == other.myStackBuffer);

	if (this_stack && other_stack)
	{
	    // If both buffers are using the stack buffer, just swap the
	    // buffer contents.
	    size_t max_size = (myLength > other.myLength) ? myLength 
							  : other.myLength;
	    
	    UTswap(myStackBuffer, other.myStackBuffer, max_size + 1);
	}
	else if (this_stack && !other_stack)
	{
	    ::memcpy(other.myStackBuffer, myStackBuffer, myLength + 1);
	    myBuffer = other.myBuffer;
	    other.myBuffer = other.myStackBuffer;
	}
	else if (!this_stack && other_stack)
	{
	    ::memcpy(myStackBuffer, other.myStackBuffer, other.myLength + 1);
	    other.myBuffer = myBuffer;
	    myBuffer = myStackBuffer;
	}
	else
	    UTswap(myBuffer, other.myBuffer);
	UTswap(myAllocatedSize, other.myAllocatedSize);
	UTswap(myLength, other.myLength);
	UTswap(myLockCount, other.myLockCount);
    }
    
public:
    /// Iterator compatibility.
    SYS_FORCE_INLINE
    const char	*begin() const { return myBuffer; }
    SYS_FORCE_INLINE
    const char	*end() const { return myBuffer + myLength; }

private:

    struct do_widen {};
    struct do_narrow {};

    /// Private constructors to allow for the Return Value Optimization
    /// @{
    SYS_FORCE_INLINE
    UT_WorkBuffer(do_widen, const utf8 *str)
	: myBuffer(myStackBuffer)
	, myAllocatedSize(UT_INITIAL_BUFFER_SIZE)
	, myLength(0)
	, myLockCount(0)
    {
	setAsUTF16(str);
    }
    SYS_FORCE_INLINE
    UT_WorkBuffer(do_narrow, const utf16 *str)
	: myBuffer(myStackBuffer)
	, myAllocatedSize(UT_INITIAL_BUFFER_SIZE)
	, myLength(0)
	, myLockCount(0)
    {
	setFromUTF16(str);
    }
    /// @}

    friend UT_API std::ostream	&operator<<(std::ostream &os,
					    const UT_WorkBuffer &buffer);

private:    // Data:

    char	*myBuffer;	// Do not make an access method to the data
    exint	 myAllocatedSize = UT_INITIAL_BUFFER_SIZE;
    exint	 myLength = 0;
    int		 myLockCount = 0;
    char	 myStackBuffer[UT_INITIAL_BUFFER_SIZE];
};


static inline size_t
format(char *buffer, size_t buffer_size, const UT_WorkBuffer &v)
{
    if (!buffer)
	return v.length();
    else
    {
	size_t len = std::min(size_t(v.length()), buffer_size);
	::memcpy(buffer, v.buffer(), len);
	return len;
    }
}


#endif
