/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLLicenseType.h ( OTL Library, C++)
 */

#ifndef __OP_OTLLicenseType__
#define __OP_OTLLicenseType__

#include "OP_API.h"

class UT_String;

// This enumeration describes the different license types that are available.
enum OP_OTLLicenseType
{
    OTL_LICENSE_NONE = 0,
    OTL_LICENSE_EXECUTE = 10,
    OTL_LICENSE_READONLY = 20,
    OTL_LICENSE_FULL = 30
};

OP_API extern void OPstringFromLicenseType(OP_OTLLicenseType licensetype,
					   UT_String &licensetypestr);

#endif
