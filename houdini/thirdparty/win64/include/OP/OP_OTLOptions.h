/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLOptions.h ( OTL Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_OTLOptions__
#define __OP_OTLOptions__

#include "OP_API.h"

#include <UT/UT_Compression.h>

class FS_IndexFile;

#define OP_PARMSFROMVFL			"ParmsFromVfl"
#define OP_PREFIXDROPPEDNPARMS		"PrefixDroppedParms"
#define OP_PREFIXDROPPEDNPARMNAME	"PrefixDroppedParmName"
#define OP_PREFIXDROPPEDNPARMLABEL	"PrefixDroppedParmLabel"
#define OP_FORBIDOUTSIDEPARMS		"ForbidOutsideParms"
#define OP_MAKEDEFAULT			"MakeDefault"
#define OP_USEDSPARMS			"UseDSParms"
#define OP_LOCKCONTENTS			"LockContents"
#define OP_UNLOCKONCREATE		"UnlockOnCreate"
#define OP_GZIPCONTENTS			"GzipContents"
#define OP_CONTENTESCOMPRESSIONTYPE	"ContentsCompressionType"
#define OP_CHECKEXTERNAL		"CheckExternal"
#define OP_SAVECACHEDCODE		"SaveCachedCode"
#define OP_SAVESPAREPARMS		"SaveSpareParms"
#define OP_SAVEICON			"SaveIcon"

// Simple storage class for HDA options
class OP_API OP_OTLOptions
{
public:
    OP_OTLOptions()
	: myParmsFromVfl(false)
	, myPrefixDroppedParmName(true)
	, myPrefixDroppedParmLabel(true)
	, myForbidOutsideParms(true)
	, myMakeDefault(true)
	, myUseDSParms(true)
	, myLockContents(true)
	, myUnlockOnCreate(false)
	, myCompressionType(UT_COMPRESSION_TYPE_GZIP)
	, myCheckExternal(true)
	, mySaveCachedCode(false)
	, mySaveSpareParms(false)
	, mySaveIcon(true)
	{}

    bool operator==(const OP_OTLOptions &options) const
    {
	return myParmsFromVfl == options.myParmsFromVfl &&
	    myPrefixDroppedParmName == options.myPrefixDroppedParmName &&
	    myPrefixDroppedParmLabel == options.myPrefixDroppedParmLabel &&
	    myForbidOutsideParms == options.myForbidOutsideParms &&
	    myMakeDefault == options.myMakeDefault &&
	    myUseDSParms == options.myUseDSParms &&
	    myLockContents == options.myLockContents &&
	    myUnlockOnCreate == options.myUnlockOnCreate &&
	    myCompressionType == options.myCompressionType &&
	    myCheckExternal == options.myCheckExternal &&
	    mySaveCachedCode == options.mySaveCachedCode &&
	    mySaveSpareParms == options.mySaveSpareParms &&
	    mySaveIcon == options.mySaveIcon;
    }

    bool operator!=(const OP_OTLOptions &options) const
    { return !operator==(options); }

    bool	 myParmsFromVfl;
    bool	 myPrefixDroppedParmName;
    bool	 myPrefixDroppedParmLabel;
    bool	 myForbidOutsideParms;
    bool	 myMakeDefault;
    bool	 myUseDSParms;
    bool	 myLockContents;
    bool	 myUnlockOnCreate;
    UT_CompressionType		myCompressionType;
    bool	 myCheckExternal;
    bool	 mySaveCachedCode;
    bool	 mySaveSpareParms;
    bool	 mySaveIcon;
};

/// Fills the OTL options object with type properties options from
/// the definition file.  Returns false if unable to get the options,
/// possibly because they don't yet exist.
OP_API bool	     OPgetOTLOptions(FS_IndexFile *deffile,
				     OP_OTLOptions &options);
/// Sets the OTL options in the definition file.
OP_API void	     OPsetOTLOptions(FS_IndexFile *deffile,
				     const OP_OTLOptions &options);

#endif

