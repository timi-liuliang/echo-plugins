/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Compress.h		UT library (C++)
 *
 * COMMENTS:
 *	Class to do simple (RLE) and complex compression (zlib). 
 *	Also has some other simple prefilter and constant checking methods.
 * 
 */
#ifndef UT_COMPRESS_H
#define UT_COMPRESS_H

#ifdef WIN32
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif


template <class Type>
class UT_Compress
{
public:
		 UT_Compress();
		~UT_Compress();

    // initializes with a max chunk size (max amount of data you're passing in)
    // Number of elements, not bytes. 
    void	setChunkSize(int size);

    // constant checking methods (if size = -1, the chunk size is used).
    int		isConstant(const Type *data, int size = -1) const;

    // prefilters (8 bit only, currently). Returns 1 if the chunk is constant.
    // this routine modifies the data passed in. Call before RLE or compress.
    // When uncompressing, use postfilter() after RLE decode or deflate.
    // these methods do not compress data, but make it easier to compress.
    int		prefilter(Type *data, int size = -1);
    void	postfilter(Type *data, int size = -1);

    // encodes RLE. Returns a pointer to alloc'd data; do not delete.
    // decode returns zero if successful, the error location if not.
    void *	encodeRLE(const Type *data, int &result_length, int size =-1,
			  int stride = 1);
    int		decodeRLE(const void *data,int size, Type *dest,int stride=1);

    // compresses using zlib (levels: 0 fast, 1 med, 2 best compression).
    void *	compress(const void *data, int &result_length, int size =-1,
			 int level = 1);
    int		expand  (const void *data,int size, void *dest);

    unsigned char *	getRLEBuf()	 { return myRLEBuf; }
    unsigned char *	getCompressBuf() { return myCompressBuf; }
    
private:
    int		repeatCount(const Type *data, int max_run, int stride);
    
    int			myChunkSize;
    unsigned char	*myRLEBuf;
    unsigned char	*myCompressBuf;
};


// for templates.
#include "UT_Compress.C"

#endif
