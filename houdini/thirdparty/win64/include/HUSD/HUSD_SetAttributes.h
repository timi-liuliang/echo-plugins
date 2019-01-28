/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_SetAttributes_h__
#define __HUSD_SetAttributes_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <UT/UT_StringHolder.h>

class HUSD_API HUSD_SetAttributes
{
public:
			 HUSD_SetAttributes(HUSD_AutoWriteLock &lock);
			~HUSD_SetAttributes();

    template<typename UtValueType>
    bool		 setPrimvar(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				const UT_StringRef &interpolation,
				const UtValueType &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType = 
				    UT_String::getEmptyString()) const;

    template<typename UtValueType>
    bool		 setAttribute(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				const UtValueType &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType = 
				    UT_String::getEmptyString()) const;


    // The following methods perform exactly the same thing as the aboves,
    // but accept the subclasses from the UT_Array. 
    template<typename UtValueType>
    bool		 setPrimvarArray(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				const UT_StringRef &interpolation,
				const UT_Array<UtValueType> &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType = 
				    UT_String::getEmptyString()) const
			 { return setPrimvar(primpath, attrname, interpolation,
			     value, timecode, valueType); }

    template<typename UtValueType>
    bool		 setAttributeArray(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				const UT_Array<UtValueType> &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType = 
				    UT_String::getEmptyString()) const
			 { return setAttribute(primpath, attrname,
			     value, timecode, valueType); }

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif
