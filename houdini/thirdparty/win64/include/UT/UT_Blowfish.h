/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Blowfish.h (  Library, C++)
 *
 * COMMENTS:
 *	Based on a free source code from:
 *	    http://www.schneier.com/blowfish.html
 *	    http://www.schneier.com/blowfish-download.html
 *
 *	blowfish.h     interface file for blowfish.cpp
 *	_THE BLOWFISH ENCRYPTION ALGORITHM_
 *	by Bruce Schneier
 *	Revised code--3/20/94
 *	Converted to C++ class 5/96, Jim Conger
 */

#ifndef __UT_Blowfish__
#define __UT_Blowfish__

#include "UT_API.h"
#include <SYS/SYS_Types.h>


// the size of the s-box used by the blofish:
#define UT_BLOWFISH_SBOX_SIZE	256


class UT_API UT_Blowfish
{
public:
    // Construct an encryptor/decryptor object with a given secret key.
     UT_Blowfish( const uint8 key[], int key_size );
    ~UT_Blowfish();

    // Get the amount of memory owned by this UT_Blowfish object
    int64	    getMemoryUsage(bool inclusive) const;

    // Get output cipher length given the input message length. The output
    // cipher length must be divisible by 8 because it consists of 64-bit blocks
    // (8 bytes x 8 bits = 64 bit block).
    static int	    getEncryptionOutputSize( int input_size );

    // Gets the maximum data chunk size on which the algorithm can work and that
    // is no larger than the specifies 'max_buffer_size'. This is a convenience
    // method that returns the size that is the multiple of the block size.
    static int	    getCryptBufferSize( int max_buffer_size );

    // Gets the block size. The alogrithm will work on sizes that are multiples
    // of the block size.
    static int	    getBlockSize();

    // Eencrypt input into output. Input length is input_size. Returned value
    // is length of output (which will be divisible by 8 bytes).  
    // Input and output buffers can be the same, but be sure buffer length
    // is divisible by 8, so that it can contain the output cipher.
    int		    encrypt( const uint8 * input, uint8 * output, 
			     int input_size );

    // Decrypt input into output.  Input length in input_size (and is divisible
    // by 8, since it contains a number of 64-bit (8-byte) blocks).
    // Input and output buffer can be the same. The length of output buffer must
    // be the same as (or larger than) the input buffer. The amount of data
    // in the output buffer is the same as 'input_size'.
    // Returns true if all is OK; false if errors occured
    bool	    decrypt( const uint8 * input, uint8 * output, 
			     int input_size );


private:
    // Constructs the enctryption sieve using the secret key.
    void 	    initialize( const uint8 key[], int key_size );

    // Encrypts the 64-bit block that has been split into left and right 32-bit
    // halves. The encrypted cipher replaces the original data in the block.
    void 	    encipherBlock( uint32 *xl, uint32 *xr );

    // Decrypts the 64-bit block that has been split into left and right 32-bit
    // halves. The decrypted text replaces the original cipher in the block.
    void 	    decipherBlock( uint32 *xl, uint32 *xr );

    // Encrypts the 64-bit block from the input source into the destination
    // output. The source and destination can be the same. The pointers
    // point to an 8-byte (64-bit) memory chunk.
    void 	    encipherBlock( const uint8 *input, uint8 *output );

    // Decrypts the 64-bit block from the input source into the destination
    // output. The source and destination can be the same. The pointers
    // point to an 8-byte (64-bit) memory chunk.
    void 	    decipherBlock( const uint8 *input, uint8 *output );

    // Array (p-array) containing the subkeys used for xor'ing block's xL half.
    uint32	    *myPArray;

    // Lookup tables (S-boxes) that implement non-reversible function F.
    uint32	    (*mySBoxes)[ UT_BLOWFISH_SBOX_SIZE ];
};

#endif	// __UT_Blowfish__



