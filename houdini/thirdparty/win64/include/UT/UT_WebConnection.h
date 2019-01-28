/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_WebConnection.h ( Utility Library, C++ )
 *
 * COMMENTS:
 *	This class connects to a web server and retrieves a given page via
 *	GET, strips out the header information, and returns the result.
 */

#ifndef __UT_WebConnection_h__
#define __UT_WebConnection_h__

#include "UT_API.h"
#include "UT_Pair.h"
#include "UT_String.h"
#include "UT_Array.h"
class UT_WorkBuffer;

class UT_API UT_WebConnection
{
public:
    // The host name is not set by default.
    UT_WebConnection();
    ~UT_WebConnection();

    void		 setHostName(const char *host_name);

    // The default port is 80.
    void		 setPort(int port) { myPort = port; }

    // Set or clear a proxy to use.
    void		 setProxy(const char *host_name, int port);
    void		 doNotUseProxy();

    // Set the name of the page to be retrieved.
    void		 setPageName(const char *page_name);

    // Set the session id for the http request.  By default, no session
    // id is sent to the web server.
    void		 setSessionId(const char *session_id);

    // Add a variable and its corresponding value that will be appended
    // to the URI.
    void		 addVariable(const char *variable_name,
				     const char *variable_value);
    void		 addVariable(const char *variable_name,
				     int variable_value);
    void		 clearVariables();

    // Connect to the web server and retrieve the page.  False is returned
    // if an error occurred and result contains the error message.  Otherwise,
    // result contains the web page, minus the http headers.  If the
    // web server sent a session id then it is returned in session_id.
    // Otherwise, session_id is blank.
    bool 		 retrievePage(UT_WorkBuffer &result,
				      UT_WorkBuffer &session_id);
    void		 setModTime(const char *src);
    int			 getModTime() const;

    // This method encodes data into a format that can be used in a URI
    // (replacing characters with %xx format).
    static void		 encodeURIData(const char *data_to_encode,
				       UT_WorkBuffer &encoded_value);

    static void		 parseURL(const char *source,
				  UT_String &host,
				  int &port,
				  UT_String &page);

private:

    typedef UT_Pair<UT_String, UT_String> UT_Variable;

    void		 setProxyFromEnvironmentVariable();
    bool		 sendHttpRequest(
				const char *host,
				int port,
				const char *page_name,
				const UT_Array<UT_Variable> &variables,
				UT_WorkBuffer &result,
				UT_WorkBuffer &session_id,
				UT_WorkBuffer &redirect);

    UT_String			 myHostName;
    UT_String			 myProxyHostName;
    int				 myPort;
    int				 myProxyPort;
    UT_String			 myPageName;
    UT_String			 mySessionId;
    UT_Array<UT_Variable>	 myVariables;
    int				 myModTime;
};

#endif
