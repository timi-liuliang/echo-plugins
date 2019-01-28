/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	IMG_PSD.h ( IMG Library, C++ )
*
* COMMENTS:	Read/Write Photoshop Document (.psd) files
*/

/*
* Overview
* -----------------------------------------------------------------------------
* This class enables the ability to read and write Photshop Document files
* into/from Houdini. Currently, 8 bit and 16 bit files with compatibility mode
* enabled and RGB color space can be read.
* TODO: Add more details when more fleshed out.
*
* The class is based off the Adobe specification dated October 2013 which can be
* found at http://www.adobe.com/devnet-apps/photoshop/fileformatashtml/
*
* TODO: Update when more fleshed out
* Supported:
* - .psd file format
* - .psb file format
* - RGB color mode
* - 8 bits per channel images with RLE compression (compatibility mode enabled)
* - 16 bits per channel images with raw data (compatibility mode enabled)
* - Simple Layer data
*   - Layer names
*   - Layer data, if it's a standard image layer.
* - Folders
* - Text layers
* - Masks***
*
* ***   There is information at the bottom of this section of how masks are 
*       handled.
*
* Not Supported
* - Color modes: Bitmap, grayscale, indexed, CMYK, multichannel, duotone, lab
* - 1 bit per channel images
* - 32 bits per channel images
* - Images compressed with ZIP without prediction (I have not seen any images
*   that use this compression method however!)
* - Blending modes
*
* The overall structure of a .psd file looks like the following:
*
*                  * * * * * * * * * * * * * * * *
*                  *         Header Data         *
*                  * * * * * * * * * * * * * * * *
*                  *       Color Mode Data       *
*                  * * * * * * * * * * * * * * * *
*                  *       Image Resources       *
*                  * * * * * * * * * * * * * * * *
*                  * Layer and Mask Information  *
*                  * * * * * * * * * * * * * * * *
*                  *         Image Data          *
*                  * * * * * * * * * * * * * * * *
*
* TODO: Update when more fleshed out
*  Each section starts with Length data, which is the amount of bytes within the
* given section. Apart from this information, we only care about the header,
* layer and mask information and image data sections at this point.
*
* Notes: - Compatibility mode needs to be enabled to read, because composite
*        image data is not stored unless enabled.
*        - Data is stored in Planar order within a .psd, which means all the
*        Red channel data is stored first, then all the Green channel data,
*        then all the Blue channel data. RRRRRRGGGGGGBBBBBB
*
* Header Section
* ---------------
* Consists of a summary of important data for the file.
*
* Layer and Mask Information Section
* -----------------------------------
* Note that the finer details should be referred to within the official spec,
* this will be a basic summary.
*
* Information is stored a bit oddly (in my opinion) for this section. We have
* 4 main sections of Length, Layer Info, Global Layer Mask Info, then
* Additional Layer Information. It looks as if for Photoshop 4.0 and later
* .psds, the layer info is organized as the following, if we only pay
* attention to important parts.
*
* - Length
* - Layer Info (Sometimes this section is length 0, followed by no data.)
* - Global Layer Mask Info (Sometimes this section is length 0, followed by
*                           no data.)
* - Additional Layer Info
*   - If Key code is Lr16, Lr32 or Layr, then
*       - Layer Info
*           - Layer info length (in bytes)
*           - Number of layers (or n)
*           - n Number of Layer Records (or Layer information)
*               - Top/left/bottom/right coordinates for start of image data
*                 boundaries.
*               - Number of channels for the layer (or m)
*               - m number of Channel Information
*                   - 2 bytes for Channel ID
*                   - 4 bytes for length of Channel Data
*                   - Layer name (as a Pascal string)
*           - n * m number of Channel Image Data (the actual color data)
*
* As you can see, there is a possibility of there being 2 Layer Info blocks, 
* where 1 can be a subsection of Additional Layer Info.
*
* Image Data Section
* -------------------
* This section only exists with compatibility mode enabled when saved. This
* section starts with 2 bytes for the compression method, followed by the image
* data.
*
* RLE Compression Summary
* -----------------------------------------------------------------------------
* Lossless compression that can be efficient for data that has frequently
* repeated data. However, for data that rarely has repeats the "compressed"
* data can actually turn out larger than it's uncompressed size.
*
* Photoshop uses this compression method for 8 bits/channel images. The Adobe
* spec does not specify this, however I haven't seen it used in other cases.
*
*   RLE Data format with compatibility mode ENABLED
*   ------------------------------------------------
*   In the image data section, immediately after the compression mode the data
*   starts off with all the length values which are 2 bytes each. These 2 byte
*   values refers to the length of a single channel's scanline data.
*
*   After all the length data, the rest of the file consists of compressed
*   image data. For decompression, the PackBits method is used. More info can
*   be found at http://en.wikipedia.org/wiki/PackBits and
*   http://web.archive.org/web/20080705155158/http://developer.apple.co
*                                                    m/technotes/tn/tn1023.html
*
*   As an example, for an image with height of 3 pixels and an unknown width
*   (you won't be able to tell the width of an image just based off the RLE
*    data until uncompressed, however you should know the width beforehand
*   from header data.) The data will appear in this format in the image data
*   section.
*
*                         LR1 LR1 LR2 LR2 LR3 LR3
*                         LG1 LG1 LG2 LG2 LG3 LG3
*                         LB1 LB1 LB2 LB2 LB3 LB3
*                         R1D R1D R1D R1D
*                         R2D R2D R2D R2D R2D R2D
*                         R3D R3D .... B3D B3D
*
*   L(R/G/B)n = Length for Red/Green/Blue channel data, row "n"
*   (R/G/B)nD = Red/Green/Blue channel, compressed data for row "n"
*
*   In the above example, LR1 = 4, LR2 = 6, LR3 = 3 and LB3 = 2
*   ------------------------------------------------
*   TODO: When using layers, explain how layer data is stored
*
* ZIP without Prediction Summary
* -----------------------------------------------------------------------------
* In .psd files, ZIP without prediction SEEMS to be just using the DEFLATE data
* compression algorithm based off of other programs. However, I personally have
* not seen any files which uses this method of compression.
*
* ZIP with Prediction Summary
* -----------------------------------------------------------------------------
* In .psd files, ZIP with prediction is a combination of using the DEFLATE data
* compression algorithm and Delta encoding; which is also known as data
* differencing.
*
*   DEFLATE Terminology:
*       - Deflate: to compress data
*       - Inflate: to decompress data
*
* As a note, I have only seen this compression used to compress individual layer
* data but not used for the composite image. This seems to be the case based
* off of observation, but this may not actually be the case as the psd spec
* does not go into details on which types of files uses which types of
* compression. So as of now ZIP is not supported when it comes to the composite
* image.
*
* TODO: Change this tidbit when composite will only be built off of layer data.
*
* Within the psd, we know the length of each channels data based off of the
* Channel Information, within the Layer Records section which is a subsection
* of the Layer and Mask information section. In the ZIP case, we know exactly
* how many bytes the compressed data is. So to get the raw image data, we simply
* need to read LENGTH amount of bytes to get the ZIP data, decompress the ZIP
* data, then decode the delta encoded portion.
*
* It is not required to understand DEFLATE to understand the code within
* IMG_PSD.c because ZLib is used to handle the complex decompression, hence it
* will not be explained. If you are interested however, you can find details at
* https://www.ietf.org/rfc/rfc1951.txt
*
*   Delta Encoding/Data Differencing Summary
*   ------------------------------------------------
*   The prediction portion of ZIP with prediction refers to the delta encoding
*   that is used. This is a simple encoding which stores the values as
*   differences instead of just the raw data. As an example, the raw data form
*   7 5 3 4 2 1 8, would be stored as 7 -2 -2 1 -2 -1 7 in delta encoded form.
*   More details can be found at https://en.wikipedia.org/wiki/Delta_encoding
*
*   After the data is inflated after zip decompression  each individual row
*   starts off with a non-delta value, instead of building off the previous
*   rows last value. So, for a 5px (width) x 3px (height) image we could have:
*
*       15  1   0   0   -2        will be       15  16  16  16  14
*       13  1   2   0   -3      decoded as      13  14  16  16  13
*       15  0   0   -3   4                      15  15  15  12  16
*
*                       This decoding below is WRONG
*                           15  16  16  16  14
*                           27  28  30  30  27
*                           42  42  42  39  43
*
* Planar Image Data Summary
* -----------------------------------------------------------------------------
* Photoshop stores image data in PLANAR form. This means all the image data for
* a single channel is stored, followed by all of the image data for the second
* channel, followed by the third channel etc. You can tell how much data exists
* by using the height, width, number of channels which should have been read
* earlier by the readHeader() method.
*
* As an example for a 4px(width) x 3px(height) image, with 16 bits per channel,
* and 3 channels (RGB) the image will be stored in the following format:
*
*                         RR RR RR RR RR RR RR RR
*                         RR RR RR RR RR RR RR RR
*                         RR RR RR RR RR RR RR RR
*                         GG GG GG GG GG GG GG GG
*                         GG GG GG GG GG GG GG GG
*                         GG GG GG GG GG GG GG GG
*                         BB BB BB BB BB BB BB BB
*                         BB BB BB BB BB BB BB BB
*                         BB BB BB BB BB BB BB BB
*
* Interleaved Image Data Summary
* -----------------------------------------------------------------------------
* Interleaved data simply means the data alternates between channels. Using the
* same example case as above, the image will be in the following format:
*
*  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB
*  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB
*  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB  RR RR GG GG BB BB
*
* Layers/Folders Summary
* -----------------------------------------------------------------------------
* Layers are treated as a stack and are stored within psds starting with the 
* lowest z-index, followed by each layer with a higher z-index. 
*
* Folders are treated as a Layer within psds, each Folder having a "Start"
* and "End" indication Layer. For example:
*
*                   ** Where Group n represents a folder.
*                   Visual Representation in Photoshop
*   ------------------------------------------------------------------------   
*   Group 0                                             Group 0
*       Image Layer 2                                       Image Layer 3
*       Image Layer 1                                       Group 1         
*   Image Layer 0                                               Image Layer 2
*                                                               Image Layer 1
*                                                       Image Layer 0
*
*                     Stored in PSDs with the Order
*   ------------------------------------------------------------------------ 
*   Image Layer 0                                       Image Layer 0                       
*   </End of Group 0>                                   <End of Group 0>
*   Image Layer 1                                       <End of Group 1>
*   Image Layer 2                                       Image Layer 1
*   <Start of Group 0>                                  Image Layer 2
*                                                       <Start of Group 1>
*                                                       Image Layer 3
*                                                       <Start of Group 0>
*
* Premultiplied Images
* -----------------------------------------------------------------------------
* If an image is premultiplied, as the name suggests; means that the image data
* we receive is already multiplied. This is important to note with alpha data,
* since the final image result will be drastically different depending on if 
* the data was already premultiplied or not.
* 
* Houdini expects data to be passed in premultiplied form. Within individual
* layer data in psds, the data is not premultiplied, hence we need to do
* multiplication before passing it into Houdini. However, for the composite
* image the data is already premultiplied, BUT the areas which are supposed
* to be transparent are passed in as white data. This means we need to do a 
* seperate compensation calculation to get the correct output for the 
* composite image within Houdini.
*
* Reference: http://microsoft.github.io/Win2D/html/PremultipliedAlpha.htm
*
* Mask Data
* -----------------------------------------------------------------------------
* We have mask data on a layer when a single layer has either 
* PSD_ChannelType::UserSuppliedLayerMask; ID -2 or 
* PSD_ChannelType::RealUserSuppliedLayerMask; ID -3 or a combination of the two.
*
* When all layers are loaded into Houdini, there are 3 overall cases of how 
* masks are loaded.
*
*          First case: RGBA channels have no image data, but has mask data.
* Method for handling: Mask data put into the alpha channel of the plane. If 
*                      there are multiple mask channels, the additional
*                      masks channels will have a new plane added with suffix
*                      _m and the alpha channel of these additioanl planes
*                      containing any additional mask data.
*
*         Second case: RGB channels have image data, Alpha channel has no data, 
*                      and has mask data.
* Method for handling: RGB data loaded into the plane, new plane with a suffix
*                      _m is added for each plane, with the mask data in the 
*                      alpha channel for the appropriate planes.
*           
*          Third case: RGBA channels have image data, and has mask data.
* Method for handling: Same as second case with alpha image data loaded into
*                      original plane.
*
* The very important field myMaskLayers is used to keep track of any mask
* channels that have their own plane. myMaskLayers maps a layer index to
* a PSD_ChannelType to a plane name, so we can see if a channel has their own
* dedicated plane by checking 
* myMaskLayers[layer_index].count(PSD_ChannelType::maskChannel). This was not
* left in IMG_PSDLayerRecords because we needed to know the index which was not
* known by IMG_PSDLayerRecords.
*/

#ifndef __IMG_PSD__
#define __IMG_PSD__

#include "IMG_API.h"
#include "IMG_File.h"

#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <PXL/PXL_Fill.h>

typedef uint64 psd_bytes_t;
typedef uint32 psd_half_bytes_t;

struct IMG_PSDLayerRecords;
struct IMG_PSDChannelInfo;
struct IMG_PSDLayerMaskData;

struct IMG_PSDHeader
{
    // Version, should always be 1 for .psd files or version 2 for .psb files.
    // .psb files will not be supported.
    uint16 myVersion;

    // Value should be 0, unless updated by the Adobe spec.
    uint64 myReserved;

    // Number of channels in image, including alpha channels. Official 
    // supported range by .psd spec is 1 - 56, for our purposes only 3/4 
    // are currently supported.
    uint16 myChannelsCount;

    // Height of the image.
    uint32 myHeight;

    // Width of the image.
    uint32 myWidth;

    // Number of bits per channel. 8/16 supported but 1 and 32 are not.
    uint16 myDepth;

    // Color mode of the file: Valid values are RGB. There are other color
    // modes which will not be supported as of now, including Bitmap,
    // Grayscale, Indexed, CMYK, Multichannel, Duotone, and Lab.
    uint16 myColorMode;

    // Is the file psd or psb format? Not part of the official psd header,
    // but a convenience bool instead of having to check if myVersion == 1
    // every time we need this check.  
    bool isPSD;
};

struct IMG_PSDLayerInfo
{
    // Length of the layer info section in bytes.
    psd_bytes_t myLength;

    // Total number of layers in the psd.
    short myLayerCount;

    // Information for each individual layer.
    UT_Array<IMG_PSDLayerRecords> myLayerRecords;
};

// Not sure if the rest of the global layer mask info data is needed
// currently.
struct IMG_PSDGlobalLayerMaskInfo
{
    // The length of this section in bytes.
    uint32 myLength;
};

struct IMG_PSDAdditionalLayerInfo
{
    // Signature which needs to be either 8BIM or 8B64
    char mySignature[4];

    // 4 character key code such as LMsk, Lr16 etc. Rest can
    // be found in psd spec.
    char myKey[4];

    // Length of the data.
    psd_bytes_t myLength;

    // Data here. (Not included yet, if ever)
};

// Channel information which corresponds to the third section within
// the Layer Records section.
struct IMG_PSDChannelInfo
{
    // ID 0 = Red, 1 = Green, 2 = Blue, etc. PSD_ChannelType has the various
    // types.
    int16 myChannelID;

    // Compression type, 0 = RAW, 1 = RLE, 3 = ZIP with prediction.
    uint16 myCompressionType;

    // The length of the image data for the channel ID.
    psd_bytes_t myChannelDataLength;

    // Position within the file that this channel's image data starts, including
    // the compression type.
    uint64 myImageDataPosition;

    // Used as a temporary buffer when handling RLE compressed data.
    UT_Array<psd_half_bytes_t> myRLELengths;
};

enum PSD_ChannelType
{
    RealUserSuppliedLayerMask = -3,
    UserSuppliedLayerMask = -2,
    TransparencyMask = -1, // Alpha
    Red = 0,
    Green = 1,
    Blue = 2,
};

enum PSD_LayerGroupType
{
    NonSectionDivider = 0,
    OpenFolder = 1,
    ClosedFolder = 2,
    BoundingSectionDivider = 3,
};

struct IMG_PSDLayerMaskData
{
    uint32 myLength;

    // Boundaries of layer mask, next 7 fields are used for when ChannelID = -2.
    int myTop, myLeft, myBottom, myRight;

    uint8 myDefaultColor;

    uint8 myFlags;

    // Next 7 fields are used for when ChannelID = -3.
    uint8 myRealFlags;

    uint8 myRealDefaultColor;

    int myRealTop, myRealLeft, myRealBottom, myRealRight;
};

// Information about each layer.
struct IMG_PSDLayerRecords
{
    // Image data boundaries. At which pixel does top/left/bottom/right
    // data start.
    int myTop, myLeft, myBottom, myRight;

    // Number of channels for the layer.
    uint16 myChannelsCount;

    // Channel ID and the length of data and positions for each channel.
    UT_SortedMap<PSD_ChannelType, IMG_PSDChannelInfo> myChannelInfo;

    // The order in which channel data is read is important, so we use this
    // to keep track of the insertion order into myChannelInfo.
    UT_Array<int> myChannelInfoInsertionOrder;

    // Should always be "8BIM".
    uint8 myBlendSignature[4];

    // Blend mode type, examples include 'norm', 'mul ', 'diss' etc. The
    // rest can be found on the psd spec.
    uint8 myBlendKey[4];

    // TODO: 0 = transparent, 255 = opaque. not used at the moment
    uint8 myOpacity;

    // TODO: 0 = base, 1 = non-base  not used at the moment. please see psd spec
    uint8 myClipping;

    // TODO: Not used at the moment - for extra info only.
    // Bit 0 = transparency protected, bit 1 = visible, bit 2 = obsolete, 
    // bit 3 = 1 for Photoshop 5.0 and later, tells if bit 4 has useful info.
    // Bit 4 = pixel data irrelevant to appearance of document.
    uint8 myFlags;

    // Filler here, no variable.

    // Data length for Layer Mask, Layer Blending Ranges, Layer Name and 
    // some additional info after (I'm not quite sure what that additional 
    // info is.).
    uint32 myExtraDataLength;

    IMG_PSDLayerMaskData myLayerMaskData;

    // Name of the layer.
    UT_String myLayerName;

    // Next three fields are not official spec, LayerRecord fields. These three
    // are used for convenience purposes.

    // Determines if the layer is a folder or not.
    PSD_LayerGroupType myLayerGroupType;

    // Length of layer name WITHOUT any suffixes. For example
    // LayerName = length 9, LayerName(2) = length 9
    uint8 myLayerNameLength;

    // Do the RGBA channels have image data? This is used to determine how many 
    // additional mask layers will be created.
    bool myLayerHasRGBAData;

    // Ignoring Layer blending ranges for now.

    bool isLayerAFolder()
    {
        return myLayerGroupType == PSD_LayerGroupType::ClosedFolder ||
            myLayerGroupType == PSD_LayerGroupType::OpenFolder ||
            myLayerGroupType == PSD_LayerGroupType::BoundingSectionDivider;
    }
};

class IMG_API IMG_PSD : public IMG_File
{
public:
    IMG_PSD();
    ~IMG_PSD();

    static void	 registerFormat();

    virtual const char  *className() const { return "IMG_FilePSD"; }

    /// @brief  Opens a .psd type file to prepare for reading
    ///
    virtual int open();

    /// @brief  Reads a single scanline for given row y into the buffer buf.
    ///         A scanline is all the image data required to draw a single row.
    ///
    /// @param[in]      y
    ///                 Row number to be read starting ranging from 0 to
    ///                 the images height - 1 (inclusive).            
    ///
    /// @param[out]     buf
    ///                 Buffer where the image data needs to be passed into
    ///
    virtual int readScanline(int y, void *buf);

    // TODO: TBD whether this will be implemented.
    virtual int create(const IMG_Stat &stat);

    // TODO: TBD whether this will be implemented.
    virtual int writeScanline(int scan, const void *buf);

    /// @brief  Flushes any open buffers, and releases all allocated memory.
    ///         Gets called by the destructor to do any cleanup.
    ///
    virtual int closeFile();

    const UT_Array<IMG_PSDLayerRecords> &getRawLayerRecords();

    UT_Array<int> getTreeLevelsFromRawLayerRecords();

private:
    /// @brief  Read the contents of the header section into the field 
    ///         myHeader. Assumes that the file pointer is currently pointing
    ///         to the beginning of the file. 
    ///
    bool readHeader();

    /// @brief  Reads the header sections of the file for each major section. 
    ///         i.e. Color mode data header, image resources header, layer and
    ///         mask info header.
    ///
    bool readFileSummary();

    // TODO: TBD whether this will be implemented.
    bool writeHeader();

    // CONVERSIONS / DECOMPRESSION ------------------------------------------

    /// @brief  Converts the header to the appropriate endianness.
    ///
    /// @param[in]      h
    ///                 The header to be converted.
    ///
    void convertHeader(IMG_PSDHeader &h);

    /// @brief  Converts a given buffer stream's endianness. This uses 
    ///         myHeader.myChannelsCount, myDepth and myStat.getXres() to do 
    ///         the calculations which need to be set before calling this. 
    ///         This version of the method will be used for any pixel data
    ///         usage.
    ///
    /// @param[in]      buf
    ///                 Stream that needs to be converted.
    ///
    /// @param[in]      length
    ///                 Number of data elements in the buffer to be converted.
    ///                 NOTE: This is NOT the number of bytes.
    ///
    bool convertStreamEndian(void *buf, int length);

    /// @brief  Converts a given buffer stream's endianness. General version
    ///         which can be used given a stream, the number of elements to
    ///         be converted and the number of bytes for each element.
    ///
    /// @param[in]      buf
    ///                 Stream that needs to be converted.
    ///
    /// @param[in]      length
    ///                 Number of data elements in the buffer to be converted.
    ///                 NOTE: This is NOT the number of bytes.
    ///
    /// @param[in]      bytesPerElement
    ///                 How many bytes that each element is.
    ///
    bool convertStreamEndian(void *buf, int length, int bytesPerElement);

    /// @brief  Delta decoding, or data differencing method which takes the
    ///         buffer and then decodes it. This is used in the decompression
    ///         of ZIP with prediction-compressed data.
    ///         See the top of this file for details.
    ///
    /// @param[in/out]  buf
    ///                 Buffer that will be decoded as input, then output
    ///                 when decoded.
    ///
    /// @param[in]      width
    ///                 Width of the given buffer layer. Needed to know
    ///                 how large the data is.
    ///
    /// @param[in]      height
    ///                 Height of the given buffer layer. Needed to know how
    ///                 large the data is.
    ///
    bool decodeDelta(void *buf, int width, int height = 1);

    /// @brief  Decoding RLE data, based off of IMG_RLEncode::decode, modified 
    /// to follow PackBits version of RLE decompression which can be found at
    /// http://en.wikipedia.org/wiki/PackBits
    /// Detailed information at 
    /// http://web.archive.org/web/20080705155158/http://developer.apple.co
    ///                                             m/technotes/tn/tn1023.html
    ///
    /// @param[out]     result
    ///                 Output, uncompressed stream.
    ///
    /// @param[in]      length
    ///                 The length of the expected uncompressed data.
    ///
    /// @param[in]      data
    ///                 Compressed RLE data input.
    ///
    void decodeRLE(char *result, int length, const char *data);

    /// @brief  Given the starting position within the file, the length of the
    ///         zip compressed data and the file we are writing to; this 
    ///         functions takes the zip data and deflates it using zlib. This 
    ///         is a modified version of inf() from zpipe.c located at
    ///         http://www.zlib.net/zpipe.c
    ///
    /// @param[in]      zipStartPosition
    ///                 Starting position of the zip compressed data in the psd
    /// 
    /// @param[in]      zipDataLength
    ///                 The expected length of the zip compressed data.
    ///
    /// @param[out]     dest
    ///                 The destination file of the decompressed zip data.
    ///
    int decompressZipChannel(uint64 zipStartPosition, psd_bytes_t zipDataLength,
        FILE *dest);

    /// @brief  Conversion from straight RGBA data to premultiplied RGBA data.
    ///         Houdini expects data to be passed in premultiplied. For psd
    ///         layer data, the data is stored without the multiplication,
    ///         hence this method needs to be called whenever a layer with 
    ///         alpha is parsed. This does not need to be called for the
    ///         composite layer. (More info at the top of this file.)
    ///
    /// @param[in/out]  data    
    ///                 Data to be multiplied.
    ///
    /// @param[in]      size
    ///                 How many pixels(NOT bytes) to be converted.
    ///
    /// @param[in]      do_multiply
    ///                 Do we multiply the data, or divide?
    ///
    void processAlpha(void *data, int size, bool do_multiply);

    // SEEKING TO CERTAIN FILE LOCATIONS ------------------------------------

    /// @brief  Makes the file pointer point to the very beginning of the
    ///         color mode data. Assumes header data length already 
    ///         initialized.
    ///     
    bool seekToColorModeData();

    /// @brief  Makes the file pointer point to the very beginning of the
    ///         resource mode data. Assumes header data length and color 
    ///         mode length already initialized.
    ///     
    bool seekToResourceData();

    /// @brief  Makes the file pointer point to the very beginning of the
    ///         layer mask data. Assumes header data length, color mode
    ///         length and image resource data length already initialized.
    ///     
    bool seekToLayerMaskData();

    /// @brief  Attempts to seek to the layer info block. This will return
    ///         1 navigation is successful to the first block, 2 if 
    ///         the second block and 0 if failed. For info on what these
    ///         "first" and "second" blocks are, read the Layer and Mask 
    ///         Information Section summary at the top of this file.
    ///
    int seekToLayerInfo();

    /// @brief  Makes the file pointer point to the very beginning of the
    ///         image data. Assumes header data length, color mode length, 
    ///         image resource data length amd layer mask data length 
    ///         are already initialized. This includes the compression
    ///         type which are the first two bytes, while the method
    ///         seekToCompleteImageData() does not.
    ///    
    bool seekToCompleteCompositeImageData();

    /// @brief  Assumes header data length, color mode length, image reource
    ///         data length and layer mask data length already initialized.
    ///         Does not include the compression data which are the first 
    ///         two bytes in this section. Call seekToCompleteImagedata()
    ///         if compression value is required.
    ///
    bool seekToCompositeImageDataOnly();

    /// @brief  Pointer is moved to the first scanline of compressed RLE data.
    ///         This data does not include the length of each scanlines 
    ///         length. Refer to the top of IMG_PSD.h for a overview of RLE
    ///         compression and the way its used in .psd
    ///
    ///         As an example, for a 3px x 4px image with 3 channels and 8 bits
    ///         per channel the first 24 bytes (4 (height) x 3 (channels) x 2
    ///         bytes for each length) of data are the lengths of each scanline.
    ///         This method will skip this length data and move onto the 
    ///         compressed data itself.
    ///
    bool seekToFirstRLEScanline();

    /// @brief  Within the file stream, reposition the pointer to point at the 
    ///         given scanline row and channel for composite data.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[in]      channel
    ///                 Channel number (R = 0, G = 1, B = 2 etc)
    /// 
    bool seekToScanLineComposite(int y, int channel);

    /// @brief  Within the relevant file, seek to beginning of the correct
    ///         scanline. Returns an integer of either value 0, 1 or 2.
    ///         If the return is 0, the method failed. If 1, success. If 2,
    ///         the seek failed because there is no image data for the given
    ///         y. However, the scanline itself is valid.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    ///
    /// @param[in]      channel
    ///                 Channel type. Look at the PSD_ChannelType enum
    ///                 for which values to enter.
    ///
    int seekToScanLineLayerChannel(int y, int layer_index,
        PSD_ChannelType channel);

    /// @brief  Within the relevant file, seek to beginning of the correct
    ///         scanline. Returns an integer of either value 0, 1 or 2.
    ///         If the return is 0, the method failed. If 1, success. If 2,
    ///         the seek failed because there is no image data for the given
    ///         y. However, the scanline itself is valid.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    ///
    /// @param[in]      channel
    ///                 Channel number. Look at the PSD_ChannelType enum
    ///                 for which values to enter.
    ///
    int seekToScanLineLayerChannel(int y, int layer_index, int channel);

    /// @brief  Seek within the psd file to the beginning given layer/channel's 
    ///         image data.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    ///
    /// @paramp[in]     channel
    ///                 Channel type. Look at the PSD_ChannelType enum
    ///                 for which values to enter.
    ///
    bool seekToChannelImageData(int layer_index = 0,
        PSD_ChannelType channel = PSD_ChannelType::Red);

    /// @brief  Seek within the psd file to the beginning given layer/channel's 
    ///         image data.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    ///
    /// @paramp[in]     channel
    ///                 Channel number. Look at the PSD_ChannelType enum
    ///                 for which values to enter.
    ///
    bool seekToChannelImageData(int layer_index = 0, int channel = 0);

    // CONVENIENCE METHODS FOR READING CERTAIN DATA ------------------------

    /// @brief  Read the compression type from COMPOSITE data in the 
    ///         image data section. 
    ///         0 = Raw image data 
    ///         1 = RLE compression
    ///         2 = ZIP without prediction (not implemented, unable to find
    ///		    psd with ZIP w/o prediction on composite image data.)
    ///         3 = ZIP with prediction (not implemented, unable to find
    ///		    psd with ZIP w/ prediction on composite image data.)
    ///
    bool readCompositeCompressionType();

    /// @brief  Navigate to color mode data, read in the data length of
    ///         the color mode section.
    ///
    bool readColorModeDataLength();

    /// @brief  Navigate to image resource data, read in the data length of
    ///         the image resource data section.
    ///
    bool readImageResourceDataLength();

    /// @brief  Navigate to layer mask data, read in the data length of
    ///         the layer mask section.
    ///
    bool readLayerMaskDataLength();

    /// @brief  Simply call all the functions for reading a section's data
    ///         length.
    ///
    bool readAllSectionLengths();

    /// @brief  Read the relevant contents of the major Layer and Mask 
    ///         information section.
    ///
    bool readLayerMaskInfo();

    /// @brief  Read the relevant contents of the Layer Info section, which
    ///         is a subsection of the Layer and Mask information section.
    ///
    bool readLayerInfo();

    /// @brief  Reads the Layer Record Data section, which is a subsection
    ///         of the Layer Info section. Assumes file pointer is already
    ///         at the correct location.
    ///
    bool readLayerRecordData();
    
    /// @brief  Reads in the layer/mask adjustment data section for the given
    ///         layer record.
    ///
    /// @param[in]      record
    ///                 The layer record we are reading the layer mask data 
    ///                 into.
    ///
    bool readLayerMaskAdjustmentData(IMG_PSDLayerRecords *record);

    /// @brief  Read the relevant contents of the Global Layer Mask info
    ///         section, which is a subsection of the Layer and Mask
    ///         information section.
    ///
    bool readGlobalLayerMaskInfo();

    /// @brief  Read the relevant contents of the Additional Layer info
    ///         section, which is a subsection of the Layer and Mask
    ///         information section.
    ///
    bool readAdditionalLayerInfo();

    /// @brief  When loading the file contents, the raw image data will
    ///         be decompressed into myLayerInfo.myLayerImageData(layerNum).
    ///         myImageData[Channel ID]. This method fetches the scanline
    ///         data for height y, and layer layer_index into the buffer.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[out]     buf
    ///                 Where to output the data, this should be pointing at the
    ///                 beginning of layer data already.
    ///
    /// @param[in]      layer_index
    ///                 Layer number. Layer immediately after the composite
    ///                 is considered to be layer 0.
    ///                 C
    ///                 A
    ///                 LayerHere   <---- Layer 0
    ///                 LayerHere2  <---- Layer 1
    ///
    /// @param[in]      isMaskPlane
    ///                 Is the plane we're reading image data into a dedicated
    ///                 mask plane?
    ///
    bool readLayerScanline(int y, void *buf, int layer_index);

    /// @brief  When loading the file contents, the raw image data will
    ///         be decompressed into myLayerInfo.myLayerImageData(layerNum).
    ///         myImageData[Channel ID]. This method fetches the scanline
    ///         data for height y, and layer layer_index into the buffer.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[out]     buf
    ///                 Where to output the data, this should be pointing at the
    ///                 beginning of layer data already.
    ///
    /// @param[in]      layer_name
    ///                 The name of the layer we're getting the scanline for.
    ///
    /// @param[in]      isMaskPlane
    ///                 Is the plane we're reading image data into a dedicated
    ///                 mask plane?
    ///
    bool readLayerScanline(int y, void *buf, const UT_String layer_name);

    /// @brief  We handle dedicated mask layers differently from a regular RGBA
    ///         image plane. So this method will fill the RGB planes with black
    ///         and reask in mask data into buf, for scanline y, given the
    ///         index of the "parent" plane. We also need to know the channel
    ///         type of the mask.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[in]      buf
    ///                 Where to output the data, this should be pointing at the
    ///                 beginning of layer data already.
    ///
    /// @param[in]      parent_layer_index
    ///                 Within the psd, the mask data is stored along with RGBA
    ///                 data on a single layer. In Houdini there is no standard 
    ///                 way to display > 4 channels, so we need to know the 
    ///                 "parent" layer where we retrieve the mask data from.
    ///
    /// @param[in]      channel
    ///                 Which kind of mask the data is. Should either be
    ///                 PSD_ChannelType::UserSuppliedLayerMask or
    ///                 PSD_ChannelType::RealUserSuppliedLayerMask
    ///
    bool readMaskLayerScanline(int y, void *buf, int parent_layer_index,
        PSD_ChannelType channel);

    /// @brief  We handle dedicated mask layers differently from a regular RGBA
    ///         image plane. So this method will fill the RGB planes with black
    ///         and reask in mask data into buf, for scanline y, given the
    ///         index of the "parent" plane. We also need to know the channel
    ///         type of the mask.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[in]      buf
    ///                 Where to output the data, this should be pointing at the
    ///                 beginning of layer data already.
    ///
    /// @param[in]      parent_layer_name
    ///                 Within the psd, the mask data is stored along with RGBA
    ///                 data on a single layer. In Houdini there is no standard 
    ///                 way to display > 4 channels, so we need to know the 
    ///                 "parent" layer where we retrieve the mask data from.
    ///
    /// @param[in]      channel
    ///                 Which kind of mask the data is. Should either be
    ///                 PSD_ChannelType::UserSuppliedLayerMask or
    ///                 PSD_ChannelType::RealUserSuppliedLayerMask
    ///
    bool readMaskLayerScanline(int y, void *buf, 
        const UT_String parent_layer_name, PSD_ChannelType channel);

    /// @brief  Given a layer, a channel, the row to scan and the buffer;
    ///         we read in a channel's worth of data for a scanline into
    ///         buf.
    ///
    /// @param[in]      y
    ///                 row
    ///
    /// @param[in]      buf
    ///                 Where to store the channel data.
    ///
    /// @param[in]      layer_index
    ///                 Which layer are we reading the channel image data for.
    ///
    /// @param[in]      channel
    ///                 Which channel are we reading the data from.
    ///
    bool readChannelScanline(int y, void *buf, int layer_index,
        PSD_ChannelType channel);

    /// @brief  Given a layer, a channel, the row to scan and the buffer;
    ///         we read in a channel's worth of data for a scanline into
    ///         buf.
    ///
    /// @param[in]      y
    ///                 row
    ///
    /// @param[in]      buf
    ///                 Where to store the channel data.
    ///
    /// @param[in]      layer_name
    ///                 Which layer are we reading the channel image data for.
    ///
    /// @param[in]      channel
    ///                 Which channel are we reading the data from.
    ///
    bool readChannelScanline(int y, void *buf, const UT_String layer_name,
        PSD_ChannelType channel);

    /// @brief  If compatibility mode is enabled, a single scanline's
    ///         worth of data will be read into the output buffer doing the 
    ///         endian conversions. Assumes that addDefaultPlane() has been
    ///         called by this point.
    ///
    /// @param[in]      y
    ///                 Row number.
    ///
    /// @param[out]     buf
    ///                 Where the scanline data will be stored.
    ///
    bool readCompositeScanline(int y, void *buf);

    /// @brief  Read a Pascal string, stores the string into str_dest
    ///         and the length of the string into length_dest.
    ///
    /// @param[out]     str_dest
    ///                 Where to store the string.
    ///
    /// @param[out]     length_dest
    ///                 Where to tstore the length.
    /// 
    bool readPascalString(UT_String *str_dest, uint8 *length_dest);

    /// @brief  Reads in the correct number of bytes for a variable that can
    ///         either be 4 or 8 bytes long. If the file is in PSD format
    ///         4 bytes are read; if it's in PSB format 8 bytes are read.
    ///
    ///	@param[in]      buffer
    ///                 Where to read the data into.
    ///
    bool readPSDBytes(psd_bytes_t *buffer);

    /// @brief  Reads in the correct number of bytes for a variable that can
    ///         either be 2 or 4 bytes long. If the file is in PSD format
    ///         2 bytes are read; if it's in PSB format 4 bytes are read.
    ///
    ///	@param[in]      buffer
    ///                 Where to read the data into.
    ///
    bool readPSDHalfBytes(psd_half_bytes_t *buffer);

    /// @brief  Converts the endian value for the correct number of bytes.
    ///         If the file is PSD, 4 bytes are converted; if in PSB format
    ///         8 bytes are converted.
    ///
    /// @param[in]      buffer
    ///                 Value to be converted.
    ///
    void convertEndianPSDBytes(psd_bytes_t *buffer);

    /// @brief  Converts the endian value for the correct number of bytes.
    ///         If the file is PSD, 2 bytes are converted; if in PSB format
    ///         4 bytes are converted.
    ///
    /// @param[in]      buffer
    ///                 Value to be converted.
    ///
    void convertEndianPSDHalfBytes(psd_half_bytes_t *buffer);

    /// @brief  Determines if the length data for the given key is either 
    ///         four or eight bytes. If the given key is 4 bytes, true is 
    ///         returned, false otherwise.
    ///
    /// @param[in]      myKey
    ///                 Key to be checked.
    ///
    bool isKeyLengthFourBytes(const char myKey[4]);

    /// @brief  Reads in a rows worth of channel data for the given row, layer
    ///         and channel into buf. buf should have a size set at this point.
    ///         A "rows" worth of data is given by whatever the boundary is 
    ///         within the .psd. getChannelBoundaries() will find the row
    ///         data to be of size right - left. This row can be larger or
    ///         smaller than the visible image data within Houdini.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    /// 
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    ///
    /// @param[im]      y
    ///                 Row number.
    ///
    /// @param[out]     buf
    ///                 Buffer where the data for the layer/channel/row is
    ///                 put into.
    ///
    bool readFullLayerChannelScanline(int layer_index, PSD_ChannelType channel, 
        int y, UT_Array<uint8> *buf);

    /// @brief  readFullLayerChannelScanline's buffer should be passed into this
    ///         function. This will extract only the image data that is required
    ///         for Houdini.
    ///
    /// @param[in]      layer
    ///                 Layer number.
    /// 
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    /// 
    /// @param[in]      in
    ///                 Input buffer of a row's worth of data for the given
    ///                 layer/channel. This can be smaller/larger than the
    ///                 image data we need to pass into Houdini.
    ///
    /// @param[out]     out
    ///                 Where to put the data that we extracted.
    ///
    bool extractVisibleLayerChannelScanline(int layer_index, 
        PSD_ChannelType channel, const uint8 *in, uint8 *out);

    // CONVENIENCE METHODS --------------------------------------------------   

    /// @brief  After all channel image data is read, we can call this method
    ///         to remove any folder layers to ignore them within Houdini.
    ///         Returns the number of layers removed.
    ///
    int removeFolderLayers();

    /// @brief  Adds the default, composite plane for the full image
    ///
    bool addDefaultPlane();

    /// @brief  Adds a plane corresponding to each layer into Houdini.
    ///         Returns 0 on fail, 1 on success.
    ///     
    bool addPlanes();

    /// @brief  Adds any additional layers with mask channel data into Houdini
    ///         with a _m suffix.
    ///
    bool addMaskPlanes();

    /// @brief  Checks to see if there's any mask data for the given layer.
    ///
    /// @param[in]      layer_index
    ///                 The layer that we're checking.
    ///
    bool doesLayerHaveMasks(int layer_index);

    /// @brief  Given a desired plane name, we check to see if there are any 
    ///         other layers with the same name. If there aren't any, the 
    ///         given name is returned. If there is a duplicate, we add a 
    ///         suffix with the format (number) where number is an actual 
    ///         number.
    ///
    /// @param[in]      base_name
    ///                 The name to check for.
    ///
    UT_String getUniquePlaneName(const UT_String base_name);

    /// @brief  Given a layer, and an output buffer we return how many mask
    ///         channels there are and store the contents of which types of
    ///         channels they are into out. 
    ///
    /// @param[in]      layer_index
    ///                 Which layer to check for masks.
    ///
    /// @param[in]      out
    ///                 Where to store what channel types are in the mask.
    ///
    int getLayerMaskChannels(int layer_index, UT_Array<PSD_ChannelType> *out);

    /// @brief  When the file pointer is at the beginning of the additional
    ///         layer info section, this method can be called to iterate through
    ///         each additional layer info block to determine the current
    ///         layer's folder group's type. Returns -1 on fail, 0 if the layer
    ///         is not a folder, or a value from 1 - 3 if the layer is 
    ///         some sort of folder. If the method fails it will set the file 
    ///         pointer to the end of the additional layer info section. This 
    ///         is dependant on additionalLayerInfoLength being correct.
    ///
    /// @param[in]      additionalLayerInfoLength
    ///                 Length of the additional layer info section, to know
    ///                 the max amount of data we need to parse.
    ///
    /// @param[in]      layer_group_type
    ///                 Where we will put our result in enum form.
    ///
    int determineLayerGroupType(int additionalLayerInfoLength,
        PSD_LayerGroupType *layer_group_type);

    /// @brief  Checks to see if the composite/flattened image data exists
    ///         within the PSD file. The image will only exist if compatibility
    ///         mode is enabled when the PSD file is saved. Without
    ///         compatibility mode enabled, the Image Data section will have
    ///         the whole image be white.
    ///
    bool doesCompositeImageExist();

    /// @brief  Determines if the given scanline is all white. This is used to
    ///         determine if compatibility mode is off/on for the PSD.
    ///
    /// @param[in]      y
    ///                 The scanline to check.
    ///
    bool isCompositeScanlineAllWhite(int y);

    /// @brief  Initialization to get minimal amount of data we need to know in 
    ///         regards to adding planes. We know how many planes/plane names/
    ///         bit depth of each plane/number of channels for each plane after
    ///         this method is called. At the very least, readLayerMaskInfo
    ///         needs to be called before this is called.
    ///
    bool initializeAllLayers();

    /// @brief  Takes the boundaries of the image data stored in the psd fpr
    ///         the given channel/layer (from record) and stores them in
    ///         top, bottom, left, right.
    ///
    /// @param[in]      record
    ///                 The PSD_LayerRecord for the layer.
    ///
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    /// 
    /// @param[out]     top
    ///                 Top boundary.
    ///
    /// @param[out]     bottom
    ///                 Bottom boundary.
    ///
    /// @param[out]     left
    ///                 Left boundary.
    ///
    /// @param[out]     Right
    ///                 Right boundary.
    ///
    void getChannelBoundaries(const IMG_PSDLayerRecords *record,
        PSD_ChannelType channelID, int *top, int *bottom,
        int *left, int *right);

    /// @brief  Takes the boundaries of the image data stored in the psd fpr
    ///         the given channel/layer (from record) and stores them in
    ///         top, bottom, left, right.
    ///
    /// @param[in]      record
    ///                 The PSD_LayerRecord for the layer.
    ///
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    /// 
    /// @param[out]     top
    ///                 Top boundary.
    ///
    /// @param[out]     bottom
    ///                 Bottom boundary.
    ///
    /// @param[out]     left
    ///                 Left boundary.
    ///
    /// @param[out]     Right
    ///                 Right boundary.
    ///
    void getChannelBoundaries(const IMG_PSDLayerRecords *record,
        int channel, int *top, int *bottom, int *left, int *right);

    /// @brief  Takes the boundaries of the image data stored in the psd fpr
    ///         the given channel/layer and stores them in top, bottom, left, 
    ///         right.
    ///
    /// @param[in]      record
    ///                 The layer number.
    ///
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    /// 
    /// @param[out]     top
    ///                 Top boundary.
    ///
    /// @param[out]     bottom
    ///                 Bottom boundary.
    ///
    /// @param[out]     left
    ///                 Left boundary.
    ///
    /// @param[out]     Right
    ///                 Right boundary.
    ///
    void getChannelBoundaries(int layer_index, int channel, int *top,
        int *bottom, int *left, int *right);

    /// @brief  Takes the boundaries of the image data stored in the psd fpr
    ///         the given channel/layer and stores them in top, bottom, left, 
    ///         right.
    ///
    /// @param[in]      record
    ///                 The layer number.
    ///
    /// @param[in]      channel
    ///                 Type of channel. Look at PSD_ChannelType enum for
    ///                 appropriate values.
    /// 
    /// @param[out]     top
    ///                 Top boundary.
    ///
    /// @param[out]     bottom
    ///                 Bottom boundary.
    ///
    /// @param[out]     left
    ///                 Left boundary.
    ///
    /// @param[out]     Right
    ///                 Right boundary.
    ///
    void getChannelBoundaries(int layer_index, PSD_ChannelType channelID,
        int *top, int *bottom, int *left, int *right);

    /// @brief  Gets an array layer index for myLayerInfo.myLayerRecords
    ///         given a name. The array is necessary because we need to know
    ///         the order in which the layers have been read. A layer name 
    ///         cam return a max of 1 index because each layer name is unique.
    ///         Returns a positive integer on success, -1 on fail.
    ///
    /// @param[in]      layer_name
    ///                 The layer we're looking for.
    ///
    int getLayerIndexFromName(const UT_String layer_name);

    // FIELDS ---------------------------------------------------------------

    // Header data for .psd
    IMG_PSDHeader myHeader;

    // Layer info for Layer and Mask Information section.
    IMG_PSDLayerInfo myLayerInfo;

    // Global layer mask info for Layer and Mask Information section.
    IMG_PSDGlobalLayerMaskInfo myGlobalLayerMaskInfo;

    // Additional layer info for Layer and Mask Information section.
    IMG_PSDAdditionalLayerInfo myAdditionalLayerInfo;

    // Length of each compressed data for a single channel's scanline row. 
    // 2 dimensional, 1 dimension for each channel, 2nd dimension for each
    // individual group of data. Look at the top of IMG_PSD.h for more
    // info on RLE compression. For composite data.
    psd_half_bytes_t** myRLELengths;

    // Total length of the compressed data for each channel.
    // For composite data.
    uint64* myRLEChannelLengths;

    // Length of header data section.
    uint32 myHeaderDataLength;

    // Length of color mode data section.
    uint32 myColorModeDataLength;

    // Length of image resource data section.
    uint32 myImageResourceDataLength;

    // Length of layer mask data section.
    psd_bytes_t myLayerMaskDataLength;

    // Starting position for the "Channel Image Data" section.
    uint64 myChannelImageDataPosition;
        
    // Compression mode of composite data.
    uint16 myCompressionMethod;

    // Bool to keep track of if the composite/flattened image exists or not.
    bool myCompositeImageExists;

    // Temporary files to write image data into, for ZIP compressed channels.
    UT_Map<PSD_ChannelType, FILE *> myZipDecompressedChannelFiles;

    // Int is the layer record index. This needs to be in this scope and not
    // within the IMG_PSDLayerRecord struct, because we need to know the index
    // and IMG_PSDLayerRecords does not know its own index.
    UT_Map<int, UT_Map<UT_String, PSD_ChannelType>> myMaskLayers;
};

#endif
