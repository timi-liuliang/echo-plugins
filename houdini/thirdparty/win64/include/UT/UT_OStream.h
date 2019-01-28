/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OStream.h (UT Library, C++)
 *
 * COMMENTS:	Simple analogue to UT_IStream. It's just a thin wrapper on top
 *		of ostream for now, to be expanded as needed.
 */

#ifndef __UT_OSTREAM_H_INCLUDED__
#define __UT_OSTREAM_H_INCLUDED__

#include "UT_API.h"
#include "UT_IOS.h"
#include "UT_NTStreamUtil.h"
#include "UT_NonCopyable.h"
#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>
#include <string>


#if defined(MBSD) || (!defined(AMD64) && !defined(WIN32))
    #define UT_NEED_LONG_OSTREAM_INSERTER
#endif


class UT_String;
class UT_StringRef;


class UT_API UT_OStream : UT_NonCopyable
{
public:
    typedef char	char_type;
    typedef int64	pos_type;
    typedef UT_IOS_SEEK	seek_type;

    /// Tag for suppressing uninitialization
    enum Uninitialized	{ NO_INIT };


    /// Construct UT_OStream with reference to the given std::ostream and
    /// streaming mode.
    // @{
			UT_OStream(std::ostream &os, UT_IOS_TYPE bin)
			    : myOS(&os)
			    , myIsBinary(bin == UT_IOS_BINARY)
			{
			}
			UT_OStream(std::ostream &os, bool binary)
			    : myOS(&os)
			    , myIsBinary(binary)
			{
			}
    // @}
    /// Construct uninitialized UT_OStream with a given streaming mode. You
    /// must call setOStream() before this object can be used.
			UT_OStream(Uninitialized, UT_IOS_TYPE bin)
			    : myOS(NULL)
			    , myIsBinary(bin == UT_IOS_BINARY)
			{
			}

    /// Compatibility methods for std::ostream replacement.
    // @{

    UT_OStream &	put(char_type ch);
    UT_OStream &	write(const char_type *str, int64 count);
    UT_OStream &	flush();

    UT_OStream &	seekp(pos_type offset, seek_type dir = UT_IOS_SEEK_BEG);
    pos_type		tellp();

    int			precision() const;
    int			precision(int dig);
    int			width() const;
    int			width(int dig);

    SYS_SAFE_BOOL	operator bool() const;
    bool		operator!() const;

    bool		bad() const;
    bool		eof() const;
    bool		fail() const;
    bool		good() const;

    void		setstate(std::ios::iostate state)
			    { myOS->setstate(state); }

    friend UT_OStream &	operator<<(UT_OStream &os, int16 value);
    friend UT_OStream &	operator<<(UT_OStream &os, uint16 value);
    friend UT_OStream &	operator<<(UT_OStream &os, int32 value);
    friend UT_OStream &	operator<<(UT_OStream &os, uint32 value);
    friend UT_OStream &	operator<<(UT_OStream &os, int64 value);
    friend UT_OStream &	operator<<(UT_OStream &os, uint64 value);
    friend UT_OStream &	operator<<(UT_OStream &os, fpreal32 value);
    friend UT_OStream &	operator<<(UT_OStream &os, fpreal64 value);
    friend UT_OStream &	operator<<(UT_OStream &os, char value);
    friend UT_OStream &	operator<<(UT_OStream &os, unsigned char value);
    friend UT_OStream &	operator<<(UT_OStream &os, const char *value);

#ifdef UT_NEED_LONG_OSTREAM_INSERTER
    friend UT_OStream &	operator<<(UT_OStream &os, long value);
    friend UT_OStream &	operator<<(UT_OStream &os, unsigned long value);
#endif


    // @}

    /// Manipulation of the underlying std::ostream reference
    // @{
    std::ostream &	getOStream()			{ return *myOS; }
    void		setOStream(std::ostream &os)	{ myOS = &os; }
    // @}

    /// Set the stream reference to same as the given UT_OStream object.
    /// @note The binary flag of this stream is not modified.
    void		setStreamRef(UT_OStream &os)
			    { myOS = &os.getOStream(); }

    /// Determine if the stream is currently in ASCII or binary mode.
    // @{
    bool		isAscii() const			{ return !myIsBinary; }
    bool		isBinary() const		{ return  myIsBinary; }
    // @}

    /// Write array data to the stream. It will call bwrite() or awrite()
    /// depending on the current streaming mode. For binary output, the data
    /// will be byte-swapped according to the Houdini standard.
    ///
    /// @param	buf	The array to write.
    /// @param	cnt	The number of array elements to write.
    /// @param	nl	If true, a newline is appended, else a space is 
    ///			appended. This parameter is ignored for binary writes.
    /// @return		The number of elements written.
    // @{
    int64		write(const bool *buf, int64 cnt=1, bool nl=false);
    int64		write(const int16 *buf, int64 cnt=1, bool nl=false);
    int64		write(const uint16 *buf, int64 cnt=1, bool nl=false);
    int64		write(const int32 *buf, int64 cnt=1, bool nl=false);
    int64		write(const uint32 *buf, int64 cnt=1, bool nl=false);
    int64		write(const int64 *buf, int64 cnt=1, bool nl=false);
    int64		write(const uint64 *buf, int64 cnt=1, bool nl=false);
    template <typename DEST_TYPE>
    int64		write(const fpreal32 *buf, int64 cnt=1, bool nl=false);
    template <typename DEST_TYPE>
    int64		write(const fpreal64 *buf, int64 cnt=1, bool nl=false);
    // @}

    /// Write string data to the stream. It will call bwrite() or awrite()
    /// depending on the current streaming mode. 
    ///
    /// @param	str	The string to write.
    /// @param	nl	If true, a newline is appended, else a space is 
    ///			appended. This parameter is ignored for binary writes.
    /// @return		@c true if the string was successfully written out.
    /// @{
    bool		write(const UT_String &str, bool nl=false);
    bool		write(const UT_StringRef &str, bool nl=false);
    bool		write(const std::string &str, bool nl=false);
    /// @}

    /// Platform consistent binary writes of data. The data will be
    /// byte-swapped according to the Houdini standard.
    ///
    /// @param	buf	The array to write.
    /// @param	cnt	The number of array elements to write.
    /// @return		The number of elements written.
    /// @{
    int64		bwrite(const bool *buf, int64 cnt=1);
    int64		bwrite(const char *buf, int64 cnt=1);
    int64		bwrite(const unsigned char *buf, int64 cnt=1);
    int64		bwrite(const signed char *buf, int64 cnt=1);
    int64		bwrite(const int16 *buf, int64 cnt=1);
    int64		bwrite(const uint16 *buf, int64 cnt=1);
    int64		bwrite(const int32 *buf, int64 cnt=1);
    int64		bwrite(const uint32 *buf, int64 cnt=1);
    int64		bwrite(const int64 *buf, int64 cnt=1);
    int64		bwrite(const uint64 *buf, int64 cnt=1);
    template <typename DEST_TYPE>
    int64		bwrite(const fpreal32 *buf, int64 cnt=1);
    template <typename DEST_TYPE>
    int64		bwrite(const fpreal64 *buf, int64 cnt=1);
    /// @}
    
    /// Platform consistent binary writes of strings. 
    ///
    /// @param	str	The string to write.
    /// @return		@c true if the string was successfully written out.
    /// @{
    bool		bwrite(const UT_String &str);
    bool		bwrite(const UT_StringRef &str);
    bool		bwrite(const std::string &str);
    /// @}
    

    /// Platform consistent ASCII writes of data.
    ///
    /// @param	buf	The array to write.
    /// @param	cnt	The number of array elements to write.
    /// @param	nl	If true, a newline is appended, else a space is 
    ///			appended.
    /// @return		The number of elements written.
    // @{
    int64		awrite(const bool *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const char *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const unsigned char *buf, int64 cnt=1,
			       bool nl=false);
    int64		awrite(const signed char *buf, int64 cnt=1,
			       bool nl=false);
    int64		awrite(const int16 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const uint16 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const int32 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const uint32 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const int64 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const uint64 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const fpreal32 *buf, int64 cnt=1, bool nl=false);
    int64		awrite(const fpreal64 *buf, int64 cnt=1, bool nl=false);
    /// @}
    
    /// Platform consistent ASCII writes of strings.
    ///
    /// @param	str	The string to write.
    /// @param	nl	If true, a newline is appended, else a space is 
    ///			appended. This parameter is ignored for binary writes.
    /// @return		@c true if the string was successfully written out.
    /// @{
    bool		awrite(const UT_String &str, bool nl=false);
    bool		awrite(const UT_StringRef &str, bool nl=false);
    bool		awrite(const std::string &str, bool nl=false);
    /// @}

private:
    UT_OStream();   // not implemented

private:
    std::ostream *	myOS;
    bool		myIsBinary;

    friend class UT_API UT_OStreamAutoBinary;
};

//////////////////////////////////////////////////////////////////////////////

// Null stream, for when a stream is required but we don't need the output.
class UT_API UT_ONullStream : public std::ostream
{
public:
    UT_ONullStream() : std::ios(0), std::ostream(0) {}

    // Streams cannot be moved. This results MSVC 2015 failing to build 
    // when trying to export the move constructor, which std::ios_base
    // does not have.
    UT_ONullStream(UT_ONullStream &&) = delete;
};


//////////////////////////////////////////////////////////////////////////////

/// Turn the stream into binary or ASCII while in variable scope.
class UT_API UT_OStreamAutoBinary
{
public:
    explicit	UT_OStreamAutoBinary(UT_OStream &os, UT_IOS_TYPE bin)
		    : myOS(os)
		    , myOldIsBinary(os.isBinary())
		{
		    myOS.myIsBinary = (bin == UT_IOS_BINARY);
		}
    explicit	UT_OStreamAutoBinary(UT_OStream &os, bool bin)
		    : myOS(os)
		    , myOldIsBinary(os.isBinary())
		{
		    myOS.myIsBinary = bin;
		}

		~UT_OStreamAutoBinary()
		{
		    myOS.myIsBinary = myOldIsBinary;		// restore
		}

private:
    UT_OStream &    myOS;
    bool	    myOldIsBinary;
};


//////////////////////////////////////////////////////////////////////////////

/// UT_AutoPrecision allows one to temporarily change a stream's precision
/// for the life of this object.
class UT_API UT_AutoPrecision
{
public:
    explicit	UT_AutoPrecision(UT_OStream &os, int new_precision)
		    : myStream(os)
		    , myOldPrecision(os.precision(new_precision))
		{
		}

		~UT_AutoPrecision()
		{
		    (void) myStream.precision(myOldPrecision);	// restore
		}

private:
    UT_OStream &    myStream;
    int		    myOldPrecision;
};


//////////////////////////////////////////////////////////////////////////////
//
// UT_OStream Implementation
//

//
// Manipulators
//

inline UT_OStream &
operator<<(UT_OStream &os, UT_OStream & (*manip)(UT_OStream &))
{
    return manip(os);
}


//
// Inline function implementations
//

inline UT_OStream &
UT_OStream::put(char_type ch)
{
    myOS->put(ch);
    return *this;
}

inline UT_OStream &
UT_OStream::write(const char_type *str, int64 count)
{
    myOS->write(str, (std::streamsize)count);
    return *this;
}

inline UT_OStream &
UT_OStream::flush()
{
    myOS->flush();
    return *this;
}

inline UT_OStream &
UT_OStream::seekp(UT_OStream::pos_type offset, UT_OStream::seek_type dir)
{
    switch (dir)
    {
	case UT_IOS_SEEK_BEG:
	    myOS->seekp(offset, std::ios_base::beg);
	    break;
	case UT_IOS_SEEK_CUR:
	    myOS->seekp(offset, std::ios_base::cur);
	    break;
	case UT_IOS_SEEK_END:
	    myOS->seekp(offset, std::ios_base::end);
	    break;
    }
    return *this;
}

inline UT_OStream::pos_type
UT_OStream::tellp()
{
    return myOS->tellp();
}


#define UT_OSTREAM_ACCESSOR(FUNCNAME) \
inline int UT_OStream::FUNCNAME() const \
{ \
    return myOS->FUNCNAME(); \
} \
inline int UT_OStream::FUNCNAME(int value) \
{ \
    return myOS->FUNCNAME(value); \
} \
/**/
UT_OSTREAM_ACCESSOR(precision)
UT_OSTREAM_ACCESSOR(width)
#undef UT_OSTREAM_ACCESSOR


#define UT_OSTREAM_INSERTER(T) \
inline UT_OStream & \
operator<<(UT_OStream &os, T value) \
{ \
    *os.myOS << value; \
    return os; \
} \
/**/
UT_OSTREAM_INSERTER(int16)
UT_OSTREAM_INSERTER(uint16)
UT_OSTREAM_INSERTER(int32)
UT_OSTREAM_INSERTER(uint32)
UT_OSTREAM_INSERTER(int64)
UT_OSTREAM_INSERTER(uint64)
UT_OSTREAM_INSERTER(fpreal32)
UT_OSTREAM_INSERTER(fpreal64)
UT_OSTREAM_INSERTER(char)
UT_OSTREAM_INSERTER(unsigned char)
UT_OSTREAM_INSERTER(const char *)
#ifdef UT_NEED_LONG_OSTREAM_INSERTER
    UT_OSTREAM_INSERTER(long)
    UT_OSTREAM_INSERTER(unsigned long)
#endif
#undef UT_OSTREAM_INSERTER


inline
UT_OStream::operator bool() const
{
    return !myOS->fail();
}

inline bool
UT_OStream::operator!() const
{
    return myOS->fail();
}


#define UT_OSTREAM_FLAG_GETTOR(FUNCNAME) \
inline bool UT_OStream::FUNCNAME() const { return myOS->FUNCNAME(); }
/**/
UT_OSTREAM_FLAG_GETTOR(bad)
UT_OSTREAM_FLAG_GETTOR(eof)
UT_OSTREAM_FLAG_GETTOR(fail)
UT_OSTREAM_FLAG_GETTOR(good)
#undef UT_OSTREAM_FLAG_GETTOR


#define UT_OSTREAM_WRITE(TYPENAME) \
inline int64 \
UT_OStream::write(const TYPENAME *buf, int64 cnt, bool nl) \
{ \
    return myIsBinary ? bwrite(buf, cnt) : awrite(buf, cnt, nl); \
} \
/**/
UT_OSTREAM_WRITE(bool)
UT_OSTREAM_WRITE(int16)
UT_OSTREAM_WRITE(uint16)
UT_OSTREAM_WRITE(int32)
UT_OSTREAM_WRITE(uint32)
UT_OSTREAM_WRITE(int64)
UT_OSTREAM_WRITE(uint64)
#undef UT_OSTREAM_WRITE
inline bool
UT_OStream::write(const UT_String &str, bool nl)
{
    return myIsBinary ? bwrite(str) : awrite(str, nl);
}
inline bool
UT_OStream::write(const UT_StringRef &str, bool nl)
{
    return myIsBinary ? bwrite(str) : awrite(str, nl);
}
inline bool
UT_OStream::write(const std::string &std_str, bool nl)
{
    return myIsBinary ? bwrite(std_str) : awrite(std_str, nl);
}

#define UT_OSTREAM_BWRITE(TYPENAME) \
inline int64 \
UT_OStream::bwrite(const TYPENAME *buf, int64 cnt) \
{ \
    return UTwrite(*myOS, buf, cnt).bad() ? 0 : cnt; \
} \
/**/
UT_OSTREAM_BWRITE(char)
UT_OSTREAM_BWRITE(unsigned char)
UT_OSTREAM_BWRITE(signed char)
UT_OSTREAM_BWRITE(int16)
UT_OSTREAM_BWRITE(uint16)
UT_OSTREAM_BWRITE(int32)
UT_OSTREAM_BWRITE(uint32)
UT_OSTREAM_BWRITE(int64)
UT_OSTREAM_BWRITE(uint64)
#undef UT_OSTREAM_BWRITE
inline int64
UT_OStream::bwrite(const bool *buf, int64 cnt)
{
    return UTwrite(*myOS, (char *)buf, cnt).bad() ? 0 : cnt;
}


#define UT_OSTREAM_AWRITE(TYPENAME) \
inline int64 \
UT_OStream::awrite(const TYPENAME *buf, int64 cnt, bool nl) \
{ \
    if (cnt < 1) \
	return 0; \
    *myOS << buf[0]; \
    for (int64 i = 1; i < cnt; i++) \
	*myOS << ' ' << buf[i]; \
    *myOS << (nl ? '\n' : ' '); \
    return myOS->bad() ? 0 : cnt; \
} \
/**/
UT_OSTREAM_AWRITE(char)
UT_OSTREAM_AWRITE(unsigned char)
UT_OSTREAM_AWRITE(signed char)
UT_OSTREAM_AWRITE(int16)
UT_OSTREAM_AWRITE(uint16)
UT_OSTREAM_AWRITE(int32)
UT_OSTREAM_AWRITE(uint32)
UT_OSTREAM_AWRITE(int64)
UT_OSTREAM_AWRITE(uint64)
UT_OSTREAM_AWRITE(fpreal32)
UT_OSTREAM_AWRITE(fpreal64)
#undef UT_OSTREAM_AWRITE
inline int64
UT_OStream::awrite(const bool *buf, int64 cnt, bool nl)
{
    if (cnt < 1)
	return 0;
    *myOS << int32(buf[0] ? 1 : 0);
    for (int64 i = 1; i < cnt; i++)
	*myOS << ' ' << int32(buf[i] ? 1 : 0);
    *myOS << (nl ? '\n' : ' ');
    return myOS->bad() ? 0 : cnt;
}


#define UT_OSTREAM_WRITEFLOAT(TYPENAME) \
template <typename DEST_TYPE> \
inline int64 \
UT_OStream::write(const TYPENAME *buf, int64 cnt, bool nl) \
{ \
    return myIsBinary ? bwrite<DEST_TYPE>(buf, cnt) : awrite(buf, cnt, nl); \
} \
template <typename DEST_TYPE> \
inline int64 \
UT_OStream::bwrite(const TYPENAME *buf, int64 cnt) \
{ \
    return UTwrite<DEST_TYPE>(*myOS, buf, cnt).bad() ? 0 : cnt; \
} \
/**/
UT_OSTREAM_WRITEFLOAT(fpreal32)
UT_OSTREAM_WRITEFLOAT(fpreal64)
#undef UT_OSTREAM_WRITEFLOAT


#endif // __UT_OSTREAM_H_INCLUDED__
