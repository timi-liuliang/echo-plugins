/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_File.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	Generic interface for loading and saving image files.
 */

#ifndef __IMG_SaveRastersToFilesParms__
#define __IMG_SaveRastersToFilesParms__

#include "IMG_API.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
class UT_StringArray;
class IMG_FileParms;
class IMG_Format;

/// Parameters for the @c saveRaster[s]AsFile[s]() methods of IMG_File
class IMG_API IMG_SaveRastersToFilesParms
{
public:
    IMG_SaveRastersToFilesParms(const UT_StringHolder &format=UT_StringHolder(),
		    UT_StringArray *created_files=nullptr,
		    const IMG_FileParms *fparms=nullptr,
		    const UT_StringHolder &linear_planes=UT_StringHolder(),
		    const UT_StringHolder &separator="_",
		    const UT_StringMap<UT_StringHolder> &planeToVarMap=UT_StringMap<UT_StringHolder>())
	: myFileParms(nullptr)
	, myFormat(nullptr)
	, myCreatedFiles(nullptr)
    {
	init(format, created_files, fparms, linear_planes, separator, planeToVarMap);
    }
    IMG_SaveRastersToFilesParms(const IMG_FileParms *fparms,
			const IMG_Format *format)
    {
	init(UT_StringHolder(), nullptr, fparms,
		UT_StringHolder(), UT_StringHolder(),
		UT_StringMap<UT_StringHolder>());
	myFormat = format;
    }
    IMG_SaveRastersToFilesParms(const IMG_SaveRastersToFilesParms &src,
			const IMG_Format *override_format,
			const IMG_FileParms *override_parms)
    {
	if (!override_parms)
	    override_parms = myFileParms;
	init(src.myFormatName, src.myCreatedFiles, override_parms,
		src.myLinearPlanes, src.mySeparator,
		src.myPlaneToVar);
	if (override_format)
	    myFormat = override_format;
    }
    void	init(const UT_StringHolder &format_name,
		UT_StringArray *created_files,
		const IMG_FileParms *fparms,
		const UT_StringHolder &linear_planes,
		const UT_StringHolder &separator,
		const UT_StringMap<UT_StringHolder> &planeToVarMap);
    const IMG_Format	*format() const { return myFormat; }
    const UT_StringHolder	&formatName() const { return myFormatName; }
    const IMG_FileParms	*fileParms() const { return myFileParms; }
    UT_StringArray		*createdFiles() const { return myCreatedFiles; }
    const UT_StringHolder	&linearPlanes() const { return myLinearPlanes; }
    const UT_StringHolder	&separator() const { return mySeparator; }
    const UT_StringMap<UT_StringHolder> &planeToVar() const { return myPlaneToVar; }

    void	setFormat(const IMG_Format *format) { myFormat = format; }
    void	setFileParms(const IMG_FileParms *p) { myFileParms = p; }
private:
    const IMG_FileParms		    *myFileParms;
    const IMG_Format		    *myFormat;
    UT_StringArray		    *myCreatedFiles;
    UT_StringHolder		     myFormatName;
    UT_StringHolder		     myLinearPlanes;
    UT_StringHolder		     mySeparator;
    UT_StringMap<UT_StringHolder>    myPlaneToVar;
};

#endif
