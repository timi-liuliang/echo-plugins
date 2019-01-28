/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Compress.C		UT library (C++)
 *
 * COMMENTS:
 *	Class to do simple (RLE) and complex compression (zlib). 
 *	Also has some other simple prefilter and constant checking methods.
 * 
 */
#ifndef UT_COMPRESS_C
#define UT_COMPRESS_C

#include <zlib.h>

#include "UT_Compress.h"

template <class Type>
UT_Compress<Type>::UT_Compress()
    : myChunkSize(0),
      myRLEBuf(0),
      myCompressBuf(0)
{
    ;
}

template <class Type>
UT_Compress<Type>::~UT_Compress()
{
    delete [] myRLEBuf;
    delete [] myCompressBuf;
}

template <class Type> void
UT_Compress<Type>::setChunkSize(int size)
{
    if(myChunkSize != size)
    {
	myChunkSize = size;
	
	delete [] myRLEBuf;
	delete [] myCompressBuf;

	// size restriction of zlib
	myCompressBuf = new unsigned char[sizeof(Type) * size * 11/10 + 12];
	myRLEBuf = new unsigned char[size * 2 * sizeof(Type)];
    }
}

// Constant Checking & Prefiltering ----------------------------------------

template <class Type> int
UT_Compress<Type>::isConstant(const Type *data, int size) const
{
    Type	val = *data;
    const Type	*itr = data;
    
    if(size == -1)
	size = myChunkSize;

    while(size > 0 && *itr++ == val) size--;

    return (size == 0);
}

// prefilter only really works for 8 bits. 
template<>
inline int
UT_Compress<unsigned char>::prefilter(unsigned char *data, int size)
{
    unsigned char val = *data;
    unsigned char prev,tmp;
    unsigned char *itr = data;
    int		  constant = 1;
    
    if(size == -1)
	size = myChunkSize;

    prev = val;
    itr ++;
    size --;
    
    while(size)
    {
	tmp = *itr;
	*itr -= prev;

	if(tmp != prev)
	    constant = 0;
	
	prev = tmp;
	itr++;
	size--;
    }

    return constant;
}
    
template<>
inline void
UT_Compress<unsigned char>::postfilter(unsigned char *data, int size)
{
    unsigned char prev;
    
    if(size == -1)
	size = myChunkSize;

    prev = *data;
    data ++;
    size --;
    
    while(size)
    {
	*data += prev;
	prev = *data;
	data++;
	size--;
    }
}

template<>
inline int
UT_Compress<unsigned short>::prefilter(unsigned short *data, int size)
{
    unsigned short val = *data;
    unsigned short prev,tmp;
    unsigned short *itr = data;
    int		  constant = 1;
    
    if(size == -1)
	size = myChunkSize;

    prev = val;
    itr ++;
    size --;
    
    while(size)
    {
	tmp = *itr;
	*itr -= prev;

	if(tmp != prev)
	    constant = 0;
	
	prev = tmp;
	itr++;
	size--;
    }

    return constant;
}
    
template<>
inline void
UT_Compress<unsigned short>::postfilter(unsigned short *data, int size)
{
    unsigned short prev;
    
    if(size == -1)
	size = myChunkSize;

    prev = *data;
    data ++;
    size --;
    
    while(size)
    {
	*data += prev;
	prev = *data;
	data++;
	size--;
    }
}

template<class Type> int
UT_Compress<Type>::prefilter(Type *data, int size)
{
    return isConstant(data,size);
}

template<class Type> void
UT_Compress<Type>::postfilter(Type *, int )
{
    ;
}

// RLE Encoding & Decoding --------------------------------------------------

template <class Type> void *
UT_Compress<Type>::encodeRLE(const Type *data, int &result_length,
			     int size, int stride)
{
    const Type		*ci;
    unsigned char	*co;
    int			 i,repeat;

    ci = (Type *)data;
    co = myRLEBuf;
    
    if(size == -1)
	size = myChunkSize;

    while (size > 0)
    {
	if (size < 3)
	    repeat = -size;
	else
	    repeat = repeatCount(ci, (size<128) ? size : 128, stride);
	if (repeat < 0)
	{
	    *co++ = char(repeat);
	    size += repeat;
	    if(stride == 1)
	    {
		memcpy(co, ci, sizeof(Type)*(-repeat));
		ci += (-repeat);
		co += sizeof(Type)*(-repeat);
	    }
	    else
	    {
		for (i=0; i< -repeat; i++)
		{
		    memcpy(co,ci, sizeof(Type));;
		    co += sizeof(Type);
		    ci += stride;
		}
	    }
	}
	else
	{
	    *co++ = char(repeat-1);
	    memcpy(co,ci,sizeof(Type));
	    co += sizeof(Type);
	    
	    ci += repeat * stride;
	    size -= repeat;
	}
    }
    
    result_length = co - myRLEBuf;

    return myRLEBuf;
}

template <class Type> int
UT_Compress<Type>::repeatCount(const Type *data, int max_run, int stride)
{
    const Type	*ci, *ci1, *ci2;
    const Type	*start;
    int		 repeat;

    start = ci = data;
    ci += stride;
    for (repeat = 1; repeat < max_run; repeat++)
    {
	if (*ci != *start)
	    break;
	ci += stride;
    }
    if (repeat != 1)
	return repeat;

    // Raw run - only gets here when max_run > 2
    ci2 = data;		// ci-2
    ci1 = ci2+stride;	// ci-1
    ci  = ci1+stride;	// ci
    
    for (repeat = 2; repeat < max_run; repeat++)
    {
	if (*ci2 == *ci && *ci1 == *ci)
	{	    repeat -= 2;
	    break;
	}
	ci2 = ci1;
	ci1 = ci;
	ci += stride;
    }
    
    return -repeat;
}

template<class Type> int
UT_Compress<Type>::decodeRLE(const void *cdata,int length,Type *uncdata,
			     int stride)
{
    const unsigned char	*ci, *end;
    Type		*co;
    int			 count;
    int			 size;
    int i;

    co = uncdata;
    ci = (unsigned char *)cdata;
    end = ci + length;
    size = 0;
    
    while (ci < end)
    {
	if (*ci & 0x80)		// Run of raw data
	{
	    count = 256 - int(*ci);
	    size += count*sizeof(Type)+1;	// We've read this many bytes
	    ci++;
	    
	    if(stride == 1)
	    {
		memcpy(co, ci, sizeof(Type)*count);
		ci+=sizeof(Type)*count;
		co+=count;
	    }
	    else
	    {
		for (i=0; i<count; i++)
		{
		    memcpy(co,ci,sizeof(Type));
		    ci += sizeof(Type);
		    co += stride;
		}
	    }
	}
	else
	{
	    count = 1 + int(*ci);
	    size += 1 + sizeof(Type);	// We read 2 chars from the buffer
	    ci++;		// Find out what char we repeat
	    
	    while(count--)
	    {
		memcpy(co,ci, sizeof(Type));
		co += stride;
	    }
	    
	    ci+=sizeof(Type);
	}
    }
    return (end == ci) ? 0 : size;
}

// ZLib compression --------------------------------------------------------

inline int
compressLevel(Bytef *dest, uLongf *destLen, const Bytef *source,
	      uLong sourceLen, int level)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
#ifdef MAXSEG_64K
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;
#endif
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit(&stream, level);
    if (err != Z_OK) return err;

    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}

template<class Type> void *
UT_Compress<Type>::compress(const void *data, int &result_length, int size,
			    int level)
{
    uLongf	length;
    
    if(size == -1)
	size = myChunkSize;

    length = sizeof(Type) * myChunkSize * 11 / 10 + 12;
    
    if(Z_OK == ::compressLevel((Bytef *)myCompressBuf, &length,
			       (const Bytef *)data,
			       (uLong) size*sizeof(Type), level))
    {
	result_length = (int) length;
	return myCompressBuf;
    }
    
    return 0;
}

template<class Type> int
UT_Compress<Type>::expand(const void *data, int size, void *dest)
{
    uLongf	length;
    
    length = myChunkSize * sizeof(Type);
    size  *= sizeof(Type); 
    
    if(Z_OK == ::uncompress((Bytef *)dest, &length,
			    (Bytef *)data, (uLong) size))
	return (int)length;
    
    return 0;
}

#endif
