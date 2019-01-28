/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLEncrypt.h ( OTL Library, C++)
 */

#ifndef __OP_OTLEncrypt__
#define __OP_OTLEncrypt__

#include "OP_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringArray.h>
#include "OP_OTLLicenseType.h"

class OP_Node;

class OP_API OP_OTLEncrypt
{
public:
			 OP_OTLEncrypt();
			~OP_OTLEncrypt();

    // Login to the asset creator account at SESI licensing site.
    const UT_String	&getEmail() const;
    void		 setEmail(const char *email);
    const UT_String	&getPassword() const;
    void		 setPassword(const char *password);

    // Various options for compiling the node during licensing process.
    bool		 getCompileBasic() const;
    void		 setCompileBasic(bool compile);
    bool		 getCompileVopnets() const;
    void		 setCompileVopnets(bool compile);
    bool		 getCompileChannels() const;
    void		 setCompileChannels(bool compile);
    bool		 getCompileNodeNames() const;
    void		 setCompileNodeNames(bool compile);

    // Management of the required license names.
    int			 getNumLicenseNames() const;
    const UT_StringHolder	&getLicenseName(int index) const;
    void		 removeLicenseName(int index);
    void		 addLicenseName(const char *name);

    /// Licenses and encrypts the HDA node and saves it in a given library.
    bool		 encryptNode(OP_Node *node, const char *libpath);

    /// Obtains the messages for the errors that occured while attempting
    /// to encrypt the asset.
    const UT_String	&getErrorMsg() const;

private:
    // This method obtains a cryptographically signed document (in XML form)
    // that specifies the licenses required to run this HDA. The info also
    // contains the encryption specification for encrypting/decrypting the 
    // contents of the HDA. This method connects to the SESI licensing site
    // to obtain this requirement info.
    bool		 getLicenseRequirementInfo( UT_String & info );

private:
    UT_String		 myEmail;
    UT_String		 myPassword;
    UT_StringArray	 myLicenseNames;

    bool		 myCompileBasic;
    bool		 myCompileVopnets;
    bool		 myCompileChannels;
    bool		 myCompileNodeNames;

    UT_String		 myErrors;
};

#endif

