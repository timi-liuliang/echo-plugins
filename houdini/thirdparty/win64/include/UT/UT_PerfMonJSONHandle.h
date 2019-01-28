/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonJSONHandle.h ( UT Library, C++)
 *
 * COMMENTS:	This class handles the parsing of performance monitor
 *		profile files stored in JSON format.
 *
 */

#ifndef __UT_PerfMonJSONHandle__
#define __UT_PerfMonJSONHandle__

#include "UT_API.h"

#include "UT_JSONHandle.h"
#include "UT_JSONParser.h"
#include "UT_String.h"

class UT_PerfMonProfile;
class UT_StringArray;
struct ut_ParseInputData;
struct ut_ParseStateData;

class UT_API UT_PerfMonJSONHandle : public UT_JSONHandle
{
public:
    /// `profile` is an empty profile that will be populated at parse time
    /// by the JSON contents stored in the given file.
    UT_PerfMonJSONHandle(const char *file_path, UT_PerfMonProfile *profile);

    virtual ~UT_PerfMonJSONHandle();

    /// Parse the JSON file and populate the profile with the data
    /// stored in the file.
    bool			 parseJSON(UT_StringArray &errors);

    /// Inherited from UT_JSONHandle.
    virtual bool		 jsonNull(UT_JSONParser &p);
    virtual bool		 jsonBool(UT_JSONParser &p, bool value);
    virtual bool		 jsonInt(UT_JSONParser &p, int64 value);
    virtual bool		 jsonReal(UT_JSONParser &p, fpreal64 value);
    virtual bool		 jsonString(
				    UT_JSONParser &p, const char *value,
				    int64 len);
    virtual bool		 jsonKey(
				    UT_JSONParser &p, const char *v, 
				    int64 len);
    virtual bool		 jsonBeginMap(UT_JSONParser &p);
    virtual bool		 jsonEndMap(UT_JSONParser &p);
    virtual bool		 jsonBeginArray(UT_JSONParser &p);
    virtual bool		 jsonEndArray(UT_JSONParser &p);

private:

    bool			 processInput_(UT_JSONParser &p, 
				    const ut_ParseInputData &input);

    UT_String			 myFilePath;
    ut_ParseStateData		*myData;
};

#endif
