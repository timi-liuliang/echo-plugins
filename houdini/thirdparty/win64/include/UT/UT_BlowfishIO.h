/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BlowfishIO.h (  Library, C++)
 *
 * COMMENTS:
 *		Utility methods for reading and writing encrypted streams.
 */

#ifndef __UT_BlowfishIO__
#define __UT_BlowfishIO__

#include "UT_API.h"
#include "UT_NTStreamUtil.h"

class UT_Blowfish;


// Encrypts the stream using the blowfish algorithm.  
class UT_API UT_BlowfishEncryptFilter : public UT_StreamBufferFilter
{
public:
    /// constructor and destructor.
		     UT_BlowfishEncryptFilter( const uint8* key_bytes, 
					       int key_size );
    virtual	    ~UT_BlowfishEncryptFilter();

    // Returns the size of the encrypted data based on its original 
    // (plain-text) size.
    static int	    getFilteredSize( int original_size );

    /// This method returns the the maximum input size (no larger than the
    /// specified buffer size) with which the filter can work, given the buffer
    /// size.
    virtual int	    getDataChunkLength( int buffer_size );

    /// Writes out the length of the original stream to the buffer. It is used
    /// when decrypting.
    virtual int	    beginFilter(char *buffer, int buffer_size, int stream_size);

    /// Performs the encryption of the data in the buffer. 
    virtual int	    doFilterChunk(char *buffer, int data_size, int buffer_size);

    /// Writes out any remaining ciphertext that has not been written out yet,
    /// and concludes encryption.
    virtual int	    endFilter( char *buffer, int buffer_size );

    /// Gets the amount of memory owned by this UT_BlowfishEncryptFilter
    virtual int64   getMemoryUsage(bool inclusive) const;

private:
    UT_Blowfish *	myEncryptor;	    // helper doing the encryption job
    bool		myFullBlocksFlag;   // true if no padding occured
};

// Decrypts the stream encrypted with the filter above.
class UT_API UT_BlowfishStreamDecryptor
{
public:
    // Constructor and destructor.
		     UT_BlowfishStreamDecryptor( const uint8 * key, 
						 int key_length );
		    ~UT_BlowfishStreamDecryptor();

    /// Wrapper for UT_Blowfish::getEncryptionOutputSize()
    static int	    getEncryptedSize( int plain_text_size );

    /// Wrapper for UT_Blowfish::getCryptBufferSize()
    static int	    getCryptBufferSize( int max_buffer_size );
    
    /// Wrapper for UT_Blowfish::getBlockSize()
    static int	    getBlockSize();

    /// Gets the preamble size. The pramble contains some meta-data such as
    /// plain-text size, encryption version, and magic number.
    static int	    getPreambleSize();

    /// Obtains the blowfish algorithm version used for the stream.
    static bool	    getVersion( const char * preamble, int preamble_length,
				int & version );

    /// Obtains the plain-text length that is encoded in the cipher.
    static bool	    getPlainTextLength( const char * preamble, 
				int preamble_length, int64 & length );


    /// Decrypts the input buffer into output buffer. See UT_Blowfish::decrypt()
    /// for details.
    bool	    decrypt( const uint8 * input, uint8 * output, 
			     int input_size );

    /// Gets the amount of memory owned by this UT_BlowfishStreamDecryptor
    int64	    getMemoryUsage(bool inclusive) const;

private:
    UT_Blowfish *   myDecryptor;    // helper doing the decryption job
};

#endif	// __UT_BlowfishIO__



