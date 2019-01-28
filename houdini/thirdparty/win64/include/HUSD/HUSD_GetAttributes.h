/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_GetAttributes_h__
#define __HUSD_GetAttributes_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <UT/UT_StringHolder.h>
class HUSD_API HUSD_GetAttributes
{
public:
			 HUSD_GetAttributes(HUSD_AutoAnyLock &lock);
			~HUSD_GetAttributes();

    template<typename UtValueType>
    bool		 getPrimvar(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				UtValueType &value,
				const HUSD_TimeCode &timecode) const;

    template<typename UtValueType>
    bool		 getAttribute(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				UtValueType &value,
				const HUSD_TimeCode &timecode) const;


    // The following methods perform exactly the same thing as the aboves,
    // but accept the subclasses from the UT_Array. 
    template<typename UtValueType>
    bool		 getPrimvarArray(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				UT_Array<UtValueType> &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType = 
				    UT_String::getEmptyString()) const
    { return getPrimvar(primpath, attrname, value, timecode); }

    template<typename UtValueType>
    bool		 getAttributeArray(const UT_StringRef &primpath,
				const UT_StringRef &attrname,
				UT_Array<UtValueType> &value,
				const HUSD_TimeCode &timecode,
				const UT_StringRef &valueType =
				    UT_String::getEmptyString()) const
    { return getAttribute(primpath, attrname, value, timecode); }

    // Return true if any attributes we have fetched are time varying.
    bool		 getIsTimeVarying() const
			 { return myTimeVarying; }

private:
    HUSD_AutoAnyLock	&myReadLock;
    mutable bool	 myTimeVarying;
};
#endif
