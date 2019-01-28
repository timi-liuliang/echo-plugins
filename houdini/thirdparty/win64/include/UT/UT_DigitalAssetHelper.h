/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *
 * NAME:    Utility Library (C++)
 *
 * COMMENTS:    DigitalAssetHelper class
 *
 */

#ifndef	__UT_DigitalAssetHelper_h__
#define	__UT_DigitalAssetHelper_h__

#include "UT_API.h"
#include "UT_ValArray.h"
#include <SYS/SYS_Types.h>

#define UT_AUTOCREATE_HDA_OPT "--autocreate-hda"

enum UT_DigitalAssetExtensions {
    UT_OTL_FILE_EXTENSION = 0,
    UT_OTLNC_FILE_EXTENSION,
    UT_OTLLC_FILE_EXTENSION,
    UT_HDA_FILE_EXTENSION,
    UT_HDANC_FILE_EXTENSION,
    UT_HDALC_FILE_EXTENSION,
    UT_NUMBER_OF_HDA_EXTENSIONS
};

class UT_String;
class UT_StringArray;

/*
 *  UT_DigitalAssetHelper is used to identify which digital asset
 *  filetypes are supported
 */

class UT_API UT_DigitalAssetHelper {
public:
    UT_DigitalAssetHelper();
    ~UT_DigitalAssetHelper();
    
    static const UT_String getDefaultFileExtension();

    static bool isSupportedFileExtension(UT_String file_extension);
    static bool isSupportedNoDotFileExtension(UT_String no_dot_file_ext);
    static bool isSupportedFile(UT_String file_name);

    static bool isCommercialExtension(UT_String ext);
    static bool isCommercialExtension(UT_DigitalAssetExtensions ext);

    static const UT_String getFileExtension(UT_DigitalAssetExtensions ext);
    static void getFileExtensions(UT_StringArray &file_extensions);
    static void getFileExtensions(UT_ValArray<const char *> &list);

    static const UT_String getNoDotFileExtension(UT_DigitalAssetExtensions ext);
    static void getNoDotFileExtensions(UT_StringArray &file_extensions);
    static void getNoDotFileExtensions(UT_ValArray<const char *> &list);

    static const UT_String getFilePattern(UT_String ext);
    static const UT_String getFilePattern(UT_DigitalAssetExtensions ext);
    static void getFilePatternsArray(UT_StringArray &file_patterns);
    static void getFilePatternsString(UT_String &file_patterns);
    
protected:


private:

};

#endif
