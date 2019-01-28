/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StringStreamImpl.h ( UT Library, C++)
 *
 * COMMENTS:
 */


#ifndef __UT_StringStreamImpl__
#define __UT_StringStreamImpl__

#ifndef __UT_StringStream__
#error UT_StringStreamImpl.h should only be included by UT_StringStream.h
#endif

#include "UT_ArrayHelp.h"

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <limits>

template<typename T>
inline
UT_StringStreamBufferBase<T>::~UT_StringStreamBufferBase()
{
    if ((myMode & std::ios_base::out) && T::autoTerminate())
	updateBufferEnd();
}

template<typename T>
inline void
UT_StringStreamBufferBase<T>::init(bool use_existing)
{
    // Init the write pointers. The output end is the next character after
    // the allocated buffer size, less one for the null terminator.
    char	*rw_buffer = const_cast<char *>(buffer().buffer());
    if (myMode & std::ios_base::out)
    {
	setp64(rw_buffer, rw_buffer + buffer().getAllocatedSize() - 1);

	// If handed a buffer, the output starts at the end of that buffer.
	if (use_existing)
	    pbump64(buffer().length());
	UT_ASSERT_P(validatePPtr());
    }
    else
    {
	setp64(nullptr, nullptr);
	UT_ASSERT_P(validatePPtr());
    }

    // Init the input pointers. The input end is the next character after
    // the last readable.
    if (myMode & std::ios_base::in)
	setg64(rw_buffer, rw_buffer, rw_buffer + buffer().length());
    else
	setg64(pptr(), pptr(), pptr());
    UT_ASSERT_P(validateGPtr());
}


template<typename T>
inline const UT_WorkBuffer &
UT_StringStreamBufferBase<T>::str() const
{
    SYSconst_cast(this)->updateBufferEnd();
    return buffer();
}



template<typename T>
inline void
UT_StringStreamBufferBase<T>::updateBufferEnd()
{
    // Ensure the buffer is properly terminated.
    exint	len;
    if (egptr64() > pptr())
    {
	*egptr64() = 0;
	len = egptr64() - eback();
    }
    else
    {
	*pptr() = 0;
	len = pptr() - pbase();
    }

    // Set the current length of the buffer. We can't have UT_WorkBuffer
    // recompute it, since we want to support embedded null characters in
    // the stream.
    buffer().lock();
    buffer().releaseSetLength(len);
}


template<typename T>
inline void
UT_StringStreamBufferBase<T>::updateReadEnd()
{
    if (pptr() && pptr() > egptr64())
    {
	if (myMode & std::ios_base::in)
	    setg64(eback(), gptr(), pptr());
	else
	    setg64(pptr(), pptr(), pptr());
	UT_ASSERT_P(validateGPtr());
    }
}

template<typename T>
inline void
UT_StringStreamBufferBase<T>::setp64(char *first, char *last)
{
    setp(first, last);
    myEPPtr = last;
}

template<typename T>
inline void
UT_StringStreamBufferBase<T>::setg64(char *first, char *next, char *last)
{
    setg(first, next, last);
    myEGPtr = last;
}

template<typename T>
template<bool bump_write_ptr> inline void
UT_StringStreamBufferBase<T>::bump64(exint offset)
{
    // The streambuf designers did the moronic thing and left gbump()/pbump()
    // with an int argument, thereby negating all benefits of using ptrdiff_t
    // for the streamsize, even on a 64-bit platform. Sigh.
    // So we bump in int.max() - 1 increments until the desired offset is
    // reached.
    // There are no checks for over/under-flow, so beware.
    const int	max_bump = std::numeric_limits<int>::max() - 1;
    int		sign = 1;

    if (offset < 0)
    {
	sign = -1;
	offset = -offset;
    }

    while(offset)
    {
	int	bump = int(SYSmin(exint(max_bump), offset));
	if (bump_write_ptr)
	    pbump(bump * sign);
	else
	    gbump(bump * sign);
	offset -= bump;
    }
}


template<typename T>
inline void
UT_StringStreamBufferBase<T>::gbump64(exint offset)
{
    bump64<false>(offset);
}


template<typename T>
inline void
UT_StringStreamBufferBase<T>::pbump64(exint offset)
{
    bump64<true>(offset);
}

template<typename T>
inline char *
UT_StringStreamBufferBase<T>::epptr64() const
{
#if 1
    // Reimplementation of epptr() to avoid 32-bit limitation on Windows
    UT_ASSERT_P(validatePPtr());
    return myEPPtr;
#else
    return epptr();
#endif
}

template<typename T>
inline char *
UT_StringStreamBufferBase<T>::egptr64() const
{
#if 1
    // Reimplementation of epptr() to avoid 32-bit limitation on Windows
    UT_ASSERT_P(validateGPtr());
    return myEGPtr;
#else
    return egptr();
#endif
}

template<typename T>
SYS_FORCE_INLINE bool
UT_StringStreamBufferBase<T>::validateGPtr() const
{
    exint s = myEGPtr - gptr();
    if (s < 0)
	return false;
    if (s < (exint(1) << 32) && myEGPtr != egptr())
	return false;
    return true;
}

template<typename T>
SYS_FORCE_INLINE bool
UT_StringStreamBufferBase<T>::validatePPtr() const
{
    exint s = myEPPtr - pptr();
    if (s < 0)
	return false;
    if (s < (exint(1) << 32) && myEPPtr != epptr())
	return false;
    return true;
}

namespace
{
    // Taken from UTbumpAlloc but with modifications to suit UT_WorkBuffer
    // and minimize allocations for large writes.
    static inline exint
    bumpAlloc(exint current_size)
    {
	static exint big_alloc_threshold = 1024 * 1024;
	exint bump;

	// Less than the static buffer, leave as-is until exhausted and we move
	// to heap storage.
	if (current_size < UT_INITIAL_BUFFER_SIZE)
	    return current_size;

	// Less than a page, go up to a page.
	if (current_size < 4096)
	    return 4096;

	// As the buffer get bigger, we make larger allocations, assuming that
	// we're racing to write data, at which point the writes become
	// more and more time-critical.
	if (current_size < big_alloc_threshold)
	    bump = current_size / 8;
	else if (current_size < big_alloc_threshold * 8)
	    bump = current_size / 4;
	else
	    bump = current_size / 2;

	current_size += bump;
	return current_size;
    }
}

template<typename T>
inline void
UT_StringStreamBufferBase<T>::writeToBuffer(const char *ptr, exint bytes)
{
    // This should never be called for read-only buffers.
    UT_ASSERT_P(myMode & std::ios_base::out);

    exint	 ppos = exint(pptr() - pbase());	// Write pos
    exint	 gpos = 0;

    if (myMode & std::ios_base::in)
	gpos = exint(gptr() - eback());

    const char	*old_buffer = buffer().buffer();
    bool	 bumped_alloc = false;

    // Lock down large enough range for the incoming characters + a terminating
    // null character + pad to avoid too many allocation on massive writes.
    exint	bytes_to_lock = ppos + bytes + 1;
    if (bytes_to_lock > buffer().getAllocatedSize())
    {
	bytes_to_lock = bumpAlloc(bytes_to_lock);
	bumped_alloc = true;
    }
    char *wr_buffer = buffer().lock(0, bytes_to_lock);
    ::memcpy(wr_buffer + ppos, ptr, bytes);
    
    // If set to auto-terminate, put a null char here now, otherwise we may
    // leave the underlying referenced buffer in a bad state. This would 
    // normally be handled in updateBufferEnd() when str() is called, but that
    // is not the case for referenced buffers. This causes slightly different
    // behavior in the presence of a seek back in the stream, where the ref
    // buffer case will always terminate, but the non-ref case will simply
    // overwrite what's there.
    if (T::autoTerminate())
	wr_buffer[ppos + bytes] = '\0';
    
    // Don't auto-compute since we may have binary data with null bytes.
    buffer().releaseSetLength(ppos + bytes);

    // If the buffer size was bumped or the buffer location changed, re-adjust
    // all stream pointers. Adjust the current read/write positions as well.
    if (buffer().buffer() != old_buffer || bumped_alloc)
    {
	char	*rw_buffer = const_cast<char *>(buffer().buffer()); 

	// Re-set the write pointers to correspond to the new location in
	// memory of the UT_WorkBuffer data. However, unlike setg(),
	// setp() does not take a third argument for current offset,
	// so we call pbump to move the current write position :-/
	// NB: less one for the null terminator.
	setp64(rw_buffer, rw_buffer + buffer().getAllocatedSize() - 1);
	pbump64(ppos);
	UT_ASSERT_P(validatePPtr());

	char	*ro_buffer_end = rw_buffer + buffer().length();
	if (myMode & std::ios_base::in)
	    setg64(rw_buffer, rw_buffer + gpos, ro_buffer_end);
	else
	    setg64(ro_buffer_end, ro_buffer_end, ro_buffer_end);
	UT_ASSERT_P(validateGPtr());
    }
}


template<typename T>
inline std::streambuf::pos_type
UT_StringStreamBufferBase<T>::seekoff(std::streambuf::off_type offset,
                                      std::ios_base::seekdir dir,
                                      std::ios_base::openmode mode)
{
    pos_type	result = pos_type(-1);
    bool	in = (myMode & mode & std::ios_base::in);
    bool	out = (myMode & mode & std::ios_base::out);

    if (!in && !out)
	return result;

    // We can't do a offset from current for both in and out.
    if (in && out && dir == std::ios_base::cur)
	return result;

    const char	*start = in ? eback() : pbase();
    if (start || !offset)
    {
	updateReadEnd();

	exint	end = egptr64() - start;
	exint	in_pos, out_pos;

	in_pos = out_pos = offset;
	if (dir == std::ios_base::cur)
	{
	    in_pos  += gptr() - start;
	    out_pos += pptr() - start;
	}
	else if (dir == std::ios_base::end)
	{
	    in_pos += end;
	    out_pos += end;
	}

	if (in && (in_pos >= 0) && (in_pos <= end))
	{
	    gbump64(start - gptr() + in_pos);
	    result = in_pos;
	}
	if (out && (out_pos >= 0) && (out_pos <= end))
	{
	    pbump64(start - pptr() + out_pos);
	    result = out_pos;
	}
    }

    return result;
}


template<typename T>
inline std::streambuf::pos_type
UT_StringStreamBufferBase<T>::seekpos(std::streambuf::pos_type pos,
                	       std::ios_base::openmode mode)
{
    pos_type	 result = pos_type(-1);
    bool	 in = (myMode & mode & std::ios_base::in);
    bool	 out = (myMode & mode & std::ios_base::out);
    if (!in && !out)
	return result;

    const char	*start = in ? eback() : pbase();
    if (start || !pos)
    {
	updateReadEnd();

	if (pos >= 0 && pos <= (egptr64() - start))
	{
	    if (in)
		gbump64(start - gptr() + pos);
	    if (out)
		pbump64(start - pptr() + pos);
	    result = pos;
	}
    }

    return result;
}


template<typename T>
inline std::streambuf::int_type
UT_StringStreamBufferBase<T>::underflow()
{
    if (!(myMode & std::ios_base::in))
	return traits_type::eof();

    updateReadEnd();

    // Have we reached the end?
    if (gptr() >= egptr64())
	return traits_type::eof();

    return traits_type::to_int_type(*gptr());
}


template<typename T>
inline std::streamsize
UT_StringStreamBufferBase<T>::xsgetn(std::streambuf::char_type *dst,
                                     std::streamsize num)
{
    if (!(myMode & std::ios_base::in))
	return traits_type::eof();

    updateReadEnd();

    exint	max_to_read = SYSmin(exint(num), exint(egptr64() - gptr()));
    if (max_to_read)
    {
	::memcpy(dst, gptr(), max_to_read);
	gbump64(max_to_read);
    }

    return max_to_read;
}


template<typename T>
inline std::streamsize
UT_StringStreamBufferBase<T>::showmanyc()
{
    if (!(myMode & std::ios_base::in))
	return -1;

    updateReadEnd();
    return egptr64() - gptr();
}


template<typename T>
inline std::streambuf::int_type
UT_StringStreamBufferBase<T>::overflow(std::streambuf::int_type c)
{
    if (!(myMode & std::ios_base::out))
	return traits_type::eof();

    if (c != traits_type::eof())
    {
	char	ch = traits_type::to_char_type(c);

	writeToBuffer(&ch, 1);
	pbump64(1);
    }
    return c;
}


template<typename T>
inline std::streamsize
UT_StringStreamBufferBase<T>::xsputn(const std::streambuf::char_type *src,
                                     std::streamsize num)
{
    if (!(myMode & std::ios_base::out))
	return 0;

    writeToBuffer(src, num);
    pbump64(num);

    return num;
}

// Specializations
inline void
UT_StringStreamBuffer::init(const char *buf, exint len)
{
    if (buf)
    {
	if (len >= 0)
	    myBuffer.append(buf, len);
	else
	    myBuffer.strcpy(buf);
    }
    else if (len > 0)
    {
	myBuffer.reserve(len);
    }

    Base::init(buf != NULL);
}

inline void
UT_StringStreamBuffer::swap(UT_WorkBuffer &buf)
{
    Base::updateBufferEnd();

    buf.swap(myBuffer);
    Base::init();
}

inline void
UT_StringStreamBuffer::steal(UT_String &str)
{
    Base::updateBufferEnd();

    myBuffer.stealIntoString(str);
    Base::init();
}

inline int
UT_StringRefStreamBuffer::sync()
{
    // Init the write pointers. The output end is the next character after
    // the allocated buffer size, less one for the null terminator.
    char	*rw_buffer = const_cast<char *>(myBuffer.buffer());
    if (mode() & std::ios_base::out)
    {
	char	*rw_buffer_end = rw_buffer + buffer().getAllocatedSize() - 1;
	exint	 len = pptr() - pbase();
	
	if (pbase() == rw_buffer && epptr64() == rw_buffer_end &&
	    len == buffer().length())
	{
	    // If the buffer didn't change externally, then assume the user
	    // is asking that the buffer is made externally valid by null 
	    // terminating it.
	    Base::updateBufferEnd();
	}
	else
	{
	    // Otherwise, update the write pointers.
	    setp64(rw_buffer, rw_buffer_end);
    
	    // Put the output location to the end of the buffer.
	    pbump64(buffer().length());
	    UT_ASSERT_P(validatePPtr());
	}
    }
    else
    {
	setp64(nullptr, nullptr);
    }

    if (mode() & std::ios_base::in)
    {
	exint		l = myBuffer.length();
	ptrdiff_t	pos = gptr() - eback();

	// If the input pos is still within the buffer area, just update
	// the pointers, otherwise mark the buffer as being EOF.
	if (pos < l)
	    setg64(rw_buffer, rw_buffer + pos, rw_buffer + buffer().length());
	else
	    setg64(pptr(), pptr(), pptr());
	UT_ASSERT_P(validateGPtr());
    }
    else
    {
	setg64(pptr(), pptr(), pptr());
	UT_ASSERT_P(validateGPtr());
    }

    return 0;
}



#endif // UT_StringStreamImpl
