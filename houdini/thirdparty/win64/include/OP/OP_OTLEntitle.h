/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLEntitle.h ( OTL Library, C++)
 *
 * NOTE:     we probably will not ever create entitlement directly from 
 *	     Houdini. Perhaps we may do it explicitly (or automatically) 
 *	     on the licensing website during a purchasing process.
 *	     This class is just temporarly used for development and
 *	     testing, and can be entirely deleted once it served its purpose.
 */

#ifndef __OP_OTLEntitle__
#define __OP_OTLEntitle__

#include "OP_API.h"
#include <sys/time.h>
#include <UT/UT_String.h>
#include "OP_OTLLicenseType.h"

class OP_API OP_OTLEntitle
{
public:
			 OP_OTLEntitle();
			~OP_OTLEntitle();

    const UT_String	&getEmail() const;
    void		 setEmail(const char *email);
    const UT_String	&getPassword() const;
    void		 setPassword(const char *password);

    const UT_String	&getLicenseName() const;
    void		 setLicenseName(const char *name);
    OP_OTLLicenseType	 getLicenseType() const;
    void		 setLicenseType(OP_OTLLicenseType licensetype);
    time_t		 getExpiry() const;
    void		 setExpiry(time_t expiry);
    const UT_String	&getServerCode() const;
    void		 setServerCode(const char *name);

    const UT_String	&getOwnerEmail() const;
    void		 setOwnerEmail(const char *owneremail);

    bool		 entitleOwner();
    const UT_String	&getErrorMsg() const;

private:
    UT_String		 myEmail;
    UT_String		 myPassword;
    UT_String		 myLicenseName;
    UT_String		 myServerCode;
    OP_OTLLicenseType	 myLicenseType;
    time_t		 myExpiry;
    UT_String		 myOwnerEmail;
};

#endif

