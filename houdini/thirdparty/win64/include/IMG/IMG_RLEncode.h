/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_RLEncode.h ( IMG Library, C++)
 *
 * COMMENTS:	This class is a class which handles RLE encoding of character
 *		data.  It only works on a single channel at a time, meaning
 *		that each channel in the image is encoded separately.
 *		This is the mechanism used in Vertigo, Wavefront and several
 *		other formats.
 */

#ifndef __IMG_RLEncode__
#define __IMG_RLEncode__

#include "IMG_API.h"
class IMG_Plane;

class IMG_API IMG_RLEncode {
public:
     IMG_RLEncode();
    ~IMG_RLEncode();

    // This method will return a size (in chars) which is big enough to hold
    // any RLE scanline (in chars)
    int		 bytesPerChannel(int xres);

    // This method should be called before you do any RLE encoding
    void	 allocEncodeBuffer(int xres);

    // This method will rl encode the data into the buffer above.  It returns
    // the buffer and returns the length of the encoded data in
    // "result_length".  When encoding, you can skip N bytes by specifying a
    // stride.  The stride should be greater than 0 to work.
    char	*encode(const char *data, int data_length,
			    int &result_length, int stride=4);

    // This method returns 0 on failure, or the number of bytes read from the
    // data stream.
    int		 decode(char *result, int result_length, const char *data,
			int stride=4);

    // This method will return:
    //	> 0 = repeat of first pixel_size bytes
    //  < 0 = -raw count of first pixel_size bytes
    // For example, if the pixel_size is 1:
    //	00000100 - will return  5 (indicating a repeat count of 5)
    //  01010111 - will return -5 (indicating a raw run of 5)
    static int	getRepeatCount(const char *data, int pixel_size,
				int max_run = 128, int stride=4);

    //
    // Encode a scanline into YUV format.  The stat() is used to determine the
    // format of the rgb data as well as the length of the scanline.  The YUV
    // buffer should be at least 3*stat.getXres() bytes long.
    // The return code is the length of the encoded YUV buffer.
    static int	yuvEncode(unsigned char *yuvdata, const void *rgbdata,
			    const IMG_Plane &plane, int xres);

    // Decode is the opposite of encode.  The YUV data is decoded into the RGB
    // data.
    static int	yuvDecode(void *rgbdata, const unsigned char *yuvdata,
			    const IMG_Plane &plane, int xres);

private:
    char	*myData;
};

#endif
