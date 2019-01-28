/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StringStream.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_StringStream__
#define __UT_StringStream__

#include "UT_API.h"
#include "UT_WorkBuffer.h"

#include <iostream>

/// An a string buffer based on UT_WorkBuffer. Supports both input and output.
/// Provides cleaner semantics than UT_OStrStream, which is based on the
/// obsolete std::strstream class. Unlike UT_OStrStream, it is not necessary
/// to manage the stream's memory externally, if it was dynamically allocated,
/// or add a null byte termination. 
/// For example, this code:
/// @code{.cpp}
/// UT_OStrStream	 os;
/// SomeClass::save(os);  // write things to os.
/// os << std::ends;
/// fprintf(stderr, "Buffer: %s", os.str());
/// os.rdbuf()->freeze(false);
/// @endcode
/// Turns into:
//// @code{.cpp}
/// UT_OStringStream	 os;
/// SomeClass::save(os);  // write things to os.
/// fprintf(stderr, "Buffer: %s", os.str());
/// @endcode
/// Note that the explicit requirement for null termination, and the call
/// to @c freeze to relinquish the buffer use, are both gone.
///
/// However, the downside is that you cannot pass explicit buffers to the 
/// stream. On the other hand, you should not be using fixed-sized buffers for
/// output streams in the first place.
/// If an externally used buffer is wanted, there's UT_OStringRefStream, which
/// takes a reference to an existing UT_WorkBuffer. The stream will carefully
/// maintain null termination, so that the buffer can be accessed at any time.
///
/// There's also support for input and input/output buffers, using 
/// UT_IStringStream/UT_IStringRefStream and UT_StringStream/UT_StringRefStream.

namespace
{
    static const std::ios_base::openmode in_out = std::ios_base::out |
						  std::ios_base::in;
}

/// Base class for the memory storage and stream buffering.
template<typename T>
class UT_StringStreamBufferBase : public std::streambuf
{
public:
    typedef std::ios_base::openmode openmode;
    typedef std::streamsize streamsize;

    virtual 		~UT_StringStreamBufferBase();

    // Resets the buffer to an empty state. Does not de-allocate existing
    // memory.
    void		 reset()
			 {
			     buffer().clear();
			     init();
			 }

protected:
    UT_StringStreamBufferBase(openmode mode) : myMode(mode) {}
    
    void		 init(bool use_existing = false);
    
    // Returns a reference to the current string.
    const UT_WorkBuffer &str() const;
    
    // Updates the underlying buffer with proper termination and length.
    void		 updateBufferEnd();
    
    // iostream implementation helpers
    virtual pos_type	 seekoff(off_type offset,
                    	         std::ios_base::seekdir dir,
                    	         openmode mode = in_out);
    virtual pos_type	 seekpos(pos_type pos,
                    	         std::ios_base::openmode mode = in_out);

    // istream implementation helpers
    virtual int_type 	 underflow();
    virtual streamsize 	 xsgetn(char_type *dst, streamsize num);
    virtual streamsize	 showmanyc();

    // ostream implementation helpers
    virtual int_type 	 overflow(int_type c);
    virtual streamsize 	 xsputn(const char_type *src, streamsize num);

    openmode		 mode() const { return myMode; }

    void		 setp64(char *first, char *last);
    void		 setg64(char *first, char *next, char *last);

    void		 pbump64(exint offset);
    void		 gbump64(exint offset);

    char *		 epptr64() const;
    char *		 egptr64() const;

    bool		 validatePPtr() const;
    bool		 validateGPtr() const;
    
private:
    template<bool bump_write_ptr>
    void		 bump64(exint offset);

    void		 updateReadEnd();
    void		 writeToBuffer(const char *ptr, exint bytes);

    // Helpers to realize the CRTP for the static polymorphism we're after.
    UT_WorkBuffer	&buffer() { return static_cast<T *>(this)->buffer(); }
    const UT_WorkBuffer	&buffer() const { return static_cast<const T *>(this)->buffer(); }
    
    openmode		myMode;
    char *		myEGPtr;
    char *		myEPPtr;
};

/// ======================================================================

/// Specialization of UT_StringStreamBufferBase which owns its own
/// UT_WorkBuffer.
class UT_StringStreamBuffer 
    : public UT_StringStreamBufferBase<UT_StringStreamBuffer>
{
    typedef UT_StringStreamBufferBase<UT_StringStreamBuffer> Base;
public:
    // Start with an empty buffer.
    explicit 		 UT_StringStreamBuffer(openmode mode = in_out)
			 : Base(mode)
    {
	init();
    }

    // Start with an empty string buffer with a given size reservation
    explicit 		 UT_StringStreamBuffer(exint reserved_size,
             		                       openmode mode = in_out)
			 : Base(mode)
    {
	init(NULL, reserved_size);
    }
    
    // Provide a buffer to populate the existing work buffer with.
    explicit		 UT_StringStreamBuffer(const UT_WorkBuffer &buf,
			                       openmode mode = in_out)
			 : Base(mode)
    {
	init(buf.buffer(), buf.length());
    }

    // Provide a block of memory to populate the existing work buffer with.
			 UT_StringStreamBuffer(const char *buf,
			                       exint len = -1,
			                       openmode mode = in_out)
			 : Base(mode)
    {
	init(buf, len);
    }

    const UT_WorkBuffer &str() const { return Base::str(); } 
			 
    // Set the buffer to a new value. Resets all current read/write offsets.
    void		 str(const char *buf, exint len = -1)
			 {
			    init(buf, len);
			 }


    void 		 str(const UT_WorkBuffer &wb)
			 {
			    init(wb.buffer(), wb.length());
			 }

    /// Swaps the contents of our buffer with the given buffer and resets
    /// the stream to the swapped-in buffer.
    void		 swap(UT_WorkBuffer &buf);
    
    /// Steals the contents of the buffer into the UT_String object given and
    /// resets the stream.
    void		 steal(UT_String &str);
    
protected:
    friend class UT_StringStreamBufferBase<UT_StringStreamBuffer>;
    UT_WorkBuffer	&buffer() { return myBuffer; }
    const UT_WorkBuffer	&buffer() const { return myBuffer; }
    static bool		autoTerminate() { return false; }
    
private:
    
    void		 init(const char *buf = NULL, exint len = -1);
    
    UT_WorkBuffer	 myBuffer;
};

/// An input stream object that owns its own string buffer storage.  
class UT_IStringStream : public std::istream
{
public:
    /// Start with an empty string buffer. Use rdbuf()->swap() to swap in new
    /// data when required. Otherwise it will simply eof right from the start.
    explicit		 UT_IStringStream(openmode m = ios_base::in)
			 : myBuffer(m | ios_base::in), 
			   std::istream(&myBuffer)
			 { }

    /// Populate the input stream with an existing UT_WorkBuffer. The contents
    /// of the buffer are copied into the internal storage. The input position
    /// start at the beginning of the buffer.
    explicit		 UT_IStringStream(const UT_WorkBuffer &buf,
			                  openmode m = ios_base::in)
			 : myBuffer(buf, m | ios_base::in), 
			   std::istream(&myBuffer)
			 { }

    /// Populate the input stream with an existing raw character buffer. 
    /// The contents of the buffer are copied into the internal storage.
    /// The input position start at the beginning of the buffer.
    explicit		 UT_IStringStream(const char *buf, exint len = -1,
            				  openmode m = ios_base::in)
			 : myBuffer(buf, len, m | ios_base::in),
			   std::istream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing input data.
    void		 reset() { myBuffer.reset(); }
private:
    UT_StringStreamBuffer	myBuffer;
};

/// An output stream object that owns its own string buffer storage.  
class UT_OStringStream : public std::ostream
{
public:
    /// Start with an empty string buffer.
    explicit		 UT_OStringStream(openmode m = ios_base::out)
			 : myBuffer(m | ios_base::out), 
			   std::ostream(&myBuffer)
			 { }

    /// Start with an empty string buffer with a given size reservation, if it
    /// is known in advance how much data will be written out. 
    explicit		 UT_OStringStream(exint reserved_size, 
            		                  openmode m = ios_base::out)
			 : myBuffer(reserved_size, m | ios_base::out), 
			   std::ostream(&myBuffer)
			 { }

    /// Populate the input stream with an existing UT_WorkBuffer. The contents
    /// of the buffer are copied into the internal storage and the output
    /// position set to the end of the buffer.
    explicit		 UT_OStringStream(const UT_WorkBuffer &buf,
			                  openmode m = ios_base::out)
			 : myBuffer(buf, m | ios_base::out), 
			   std::ostream(&myBuffer)
			 { }
    
    /// Populate the input stream with an existing raw character buffer. 
    /// The contents of the buffer are copied into the internal storage and 
    /// the output position set to the end of the buffer.
    explicit		 UT_OStringStream(const char *buf, exint len = -1,
            				  openmode m = ios_base::out)
			 : myBuffer(buf, len, m | ios_base::out), 
			   std::ostream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing input data.
    void		 reset() { myBuffer.reset(); }
private:
    UT_StringStreamBuffer	myBuffer;
};

/// An bi-directional stream object that owns its own string buffer storage.  
class UT_StringStream : public std::iostream
{
public:
    /// Start with an empty string buffer. Use rdbuf()->swap() to swap in new
    /// data if required. Otherwise it will simply eof, on read, right from 
    /// the start.
    explicit		 UT_StringStream(openmode m = in_out)
			 : myBuffer(m),
			   std::iostream(&myBuffer)
			 { }

    /// Start with an empty string buffer with a given size reservation, if it
    /// is known in advance how much data will be written out. The input stream
    /// will still be empty.
    explicit		 UT_StringStream(exint reserved_size, 
            		                 openmode m = in_out)
			 : myBuffer(reserved_size, m), 
			   std::iostream(&myBuffer)
			 { }
    
    /// Populate the input stream with an existing UT_WorkBuffer. The contents
    /// of the buffer are copied into the internal storage, the output
    /// position set to the end of the buffer, and the input position at the
    /// beginning of the buffer.
    explicit		 UT_StringStream(const UT_WorkBuffer &buf,
			                 openmode m = in_out)
			 : myBuffer(buf, m),
			   std::iostream(&myBuffer)
			 { }

    /// Populate the input stream with an existing raw character buffer. 
    /// The contents of the buffer are copied into the internal storage, 
    /// the output position set to the end of the buffer, and the 
    /// input position at the beginning of the buffer.
    explicit		 UT_StringStream(const char *buf, exint len = -1,
            				 openmode m = in_out)
			 : myBuffer(buf, len, m),
			   std::iostream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing input data.
    void		 reset() { myBuffer.reset(); }
private:
    UT_StringStreamBuffer	myBuffer;
};

/// ======================================================================
/// A version of std::streambuf which holds a reference to an external 
/// UT_WorkBuffer object. It is important that the life cycle of the external
/// UT_WorkBuffer fully encloses the life cycle of the UT_StringRefStreamBuffer
/// object. Upon destruction of the UT_StringRefStreamBuffer object the
/// UT_WorkBuffer object will be guaranteed to be null terminated. While the
/// UT_StringRefStream is live, no such guarantee is made. To force a null 
/// termination, call UT_StringRefStream::sync().
/// If the external buffer is changed outside of the stream object, then
/// it is necessary to call UT_StringRefStream::sync() so that any references
/// to the buffer can be updated. In this case, an output stream will be reset
/// to the end of the buffer. The input stream is left to point to the same
/// location, unless it now points beyond the end, in which case it gets marked
/// as eof.
/// Note that there is a difference between ref-buffer and non-ref-buffer
/// when writing data after seeking back in the stream. Because the ref-buffer
/// auto terminates after the newly written characters, it may terminate 
/// the string before the end of previously written characters, effectively
/// discarding these older characters.

class UT_StringRefStreamBuffer 
    : public UT_StringStreamBufferBase<UT_StringRefStreamBuffer>
{
    typedef UT_StringStreamBufferBase<UT_StringRefStreamBuffer> Base;
public:
    /// Take a reference to an existing UT_WorkBuffer. The output position set 
    /// to the end of the buffer, and the input position at the beginning of 
    /// the buffer.
    explicit 		 UT_StringRefStreamBuffer(UT_WorkBuffer &buf,
						  openmode mode = in_out)
			 : Base(mode),
			   myBuffer(buf)
    {
	Base::init(true);
    }

    /// Returns a read-only reference to the underlying storage.  
    const UT_WorkBuffer &str() const { return Base::str(); } 

protected:
    /// std::streambuf helpers. 
    virtual int		sync();

    friend class UT_StringStreamBufferBase<UT_StringRefStreamBuffer>;
    UT_WorkBuffer	&buffer() { return myBuffer; }
    const UT_WorkBuffer	&buffer() const { return myBuffer; }
    static bool		autoTerminate() { return true; }
    
private:
    UT_WorkBuffer	&myBuffer;
};

/// An input stream object that keeps a reference to an external string 
/// buffer storage.
/// See @c UT_StringRefStreamBuffer for notes on modifying the external buffer
/// while doing stream operations on it.
class UT_IStringRefStream : public std::istream
{
public:
    /// Take a reference to an existing UT_WorkBuffer. The input position at 
    /// the beginning of the buffer.
    explicit		 UT_IStringRefStream(UT_WorkBuffer &buf,
            		                     openmode m = ios_base::in)
			 : myBuffer(buf, m | ios_base::in), 
			   std::istream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringRefStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing input data.
    void		 reset() { myBuffer.reset(); }
private:
    UT_StringRefStreamBuffer myBuffer;
};

/// An output stream object that keeps a reference to an external string 
/// buffer storage.
/// See @c UT_StringRefStreamBuffer for notes on modifying the external buffer
/// while doing stream operations on it.
class UT_OStringRefStream : public std::ostream
{
public:
    /// Take a reference to an existing UT_WorkBuffer. The output position set 
    /// to the end of the buffer.
    explicit		 UT_OStringRefStream(UT_WorkBuffer &buf,
            		                     openmode m = ios_base::out)
			 : myBuffer(buf, m | ios_base::out), 
			   std::ostream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringRefStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing output data.
    void		 reset() { myBuffer.reset(); }
    
private:
    UT_StringRefStreamBuffer myBuffer;
};

/// An bi-directional stream object that keeps a reference to an external  
/// string buffer storage.
/// See @c UT_StringRefStreamBuffer for notes on modifying the external buffer
/// while doing stream operations on it.
class UT_StringRefStream : public std::iostream
{
public:
    /// Take a reference to an existing UT_WorkBuffer. The output position set 
    /// to the end of the buffer, and the input position at the beginning of 
    /// the buffer.
    explicit		 UT_StringRefStream(UT_WorkBuffer &buf,
            		                    openmode m = in_out)
			 : myBuffer(buf, m),
			   std::iostream(&myBuffer)
			 { }

    /// Returns the underlying stream buffer object.
    UT_StringRefStreamBuffer *rdbuf() const { return SYSconst_cast(&myBuffer); }

    /// Returns a read-only reference to the underlying UT_WorkBuffer. 
    const UT_WorkBuffer  &str() const { return myBuffer.str(); }

    /// Resets the input stream and clears all existing data.
    void		 reset() { myBuffer.reset(); }
private:
    UT_StringRefStreamBuffer	myBuffer;
};


// Implementation file
#include "UT_StringStreamImpl.h"

#endif //__UT_StringStream__
