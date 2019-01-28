/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Action (C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_LMHost_h__
#define __UT_LMHost_h__

#include "UT_API.h"

#define UT_SESI_LMHOST_REGISTRY_VALUE	\
	"LicenseServer"
#define UT_SESI_LMHOST_ENVVAR		\
	"SESI_LMHOST"

class UT_String;
class UT_StringArray;

class UT_API UT_LMHost
{
public:
    static void	 getLMHost(UT_String &host_str, bool canUseHserver);
    static void	 getLMHosts(UT_StringArray &hosts, bool canUseHserver);

    // if this flag is true, setLMHost will always set the host in its
    // temporary static variable. Ie, when this flag is true, setLMHost
    // will not set the hserver, environment variable, or write to a file.
    static void	 setLMHostAlwaysAsTemp(bool flag);
    static bool	 getLMHostAlwaysAsTemp();

    // NB: doHserverToo is disregarded when AlwaysAsTemp is true
    static void	 setLMHost(const UT_String &host, bool doHserverToo);
    static void	 setLMHosts(const UT_StringArray &host, bool doHserverToo);


    static void	 parseHostString(const char *host_str, UT_StringArray &hosts);
    static void	 packHostString(const UT_StringArray &hosts,
				    UT_String &host_str);
};

#endif
