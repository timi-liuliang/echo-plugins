/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *
 * Sample usage:
 *     FS_WebAPIResponse response =
 *         FS_WebAPI("http://www.orbolt.com/api").call1(
 *             "is_asset_in_store", "SideFX::spaceship");
 *      response.myJSONResult.dump();
 *      std::cout << "\nerrors: " << response.myErrors.nonNullBuffer()
 *          << std::endl;
 * Prints:
 *     true
 *     errors:
 */

#ifndef __HOM_WebAPI_h__
#define __HOM_WebAPI_h__

#include "FS_API.h"
#include <UT/UT_JSONValue.h>
#include <UT/UT_JSONWriter.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_DeepString.h>
#include <UT/UT_Lock.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_Map.h>

class FS_API FS_WebAPIResponse
{
public:
    UT_JSONValue myJSONResult;
    UT_DeepString myErrors;
};

class FS_API FS_WebAPI
{
public:
    FS_WebAPI(const char *api_url);

    void setProxy(const char *proxyUrl, int port);

    void clearProxy();

    FS_WebAPIResponse post(
	const UT_Map<UT_StringHolder, UT_StringHolder> &variables) const;

    // The following methods let you make API calls to named functions in
    // the Web API.  Call the appropriate method for the given number of
    // arguments.
    FS_WebAPIResponse call0(const char *api_function) const;

    template <typename T1>
    FS_WebAPIResponse call1(const char *api_function,
	const T1 &arg1) const;

    template <typename T1, typename T2>
    FS_WebAPIResponse call2(const char *api_function,
	const T1 &arg1, const T2 &arg2) const;

    template <typename T1, typename T2, typename T3>
    FS_WebAPIResponse call3(const char *api_function,
	const T1 &arg1, const T2 &arg2, const T3 &arg3) const;

    template <typename T1, typename T2, typename T3, typename T4>
    FS_WebAPIResponse call4(const char *api_function,
	const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4) const;

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    FS_WebAPIResponse call5(const char *api_function,
	const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4,
	const T5 &arg5) const;

private:
    // This is a really simple implementation. It doesn't check for duplicates
    // or anything like that.
    void addCookie(const char *name, const char *value);
    void clearCookies();

    FS_WebAPIResponse callFunctionWithJSONArgs(
	const UT_WorkBuffer &json_data) const;

    FS_WebAPIResponse postAndGetJSONResponse(
	void *curl, const char *post_data) const;

    void chooseCSRFToken(UT_WorkBuffer &csrf_token) const;

    void ensureCURLIsInitialized();

    void writeAPICallPrefixToJSON(
        UT_JSONWriter &writer, const char *api_function) const;
    void writeAPICallSuffixToJSON(UT_JSONWriter &writer) const;

    // These helper templated functions know how to write different value types
    // to a UT_JSONWriter.
    // TODO: Make these templated methods of UT_JSONWriter.
    template <typename T>
    void writeValue(UT_JSONWriter &writer, T value) const
    { writer.jsonValue(value); }

    UT_String myAPIURL;
    UT_StringHolder myProxyHostname;
    int myProxyPort;
    UT_Lock myCurlInitLock;
    UT_WorkBuffer myCookies;
    bool myHasInitializedCurl;
};

//---------------------------------------------------------------------------


template <typename T1>
FS_WebAPIResponse
FS_WebAPI::call1(
    const char *api_function, const T1 &arg1) const
{
    UT_WorkBuffer json_data;
    UT_UniquePtr<UT_JSONWriter> json_writer(
	UT_JSONWriter::allocWriter(json_data));

    writeAPICallPrefixToJSON(*json_writer, api_function);
    writeValue(*json_writer, arg1);
    writeAPICallSuffixToJSON(*json_writer);

    return callFunctionWithJSONArgs(json_data);
}

template <typename T1, typename T2>
FS_WebAPIResponse
FS_WebAPI::call2(
    const char *api_function,
    const T1 &arg1, const T2 &arg2) const
{
    UT_WorkBuffer json_data;
    UT_UniquePtr<UT_JSONWriter> json_writer(
	UT_JSONWriter::allocWriter(json_data));

    writeAPICallPrefixToJSON(*json_writer, api_function);
    writeValue(*json_writer, arg1);
    writeValue(*json_writer, arg2);
    writeAPICallSuffixToJSON(*json_writer);

    return callFunctionWithJSONArgs(json_data);
}

template <typename T1, typename T2, typename T3>
FS_WebAPIResponse
FS_WebAPI::call3(
    const char *api_function,
    const T1 &arg1, const T2 &arg2, const T3 &arg3) const
{
    UT_WorkBuffer json_data;
    UT_UniquePtr<UT_JSONWriter> json_writer(
	UT_JSONWriter::allocWriter(json_data));

    writeAPICallPrefixToJSON(*json_writer, api_function);
    writeValue(*json_writer, arg1);
    writeValue(*json_writer, arg2);
    writeValue(*json_writer, arg3);
    writeAPICallSuffixToJSON(*json_writer);

    return callFunctionWithJSONArgs(json_data);
}

template <typename T1, typename T2, typename T3, typename T4>
FS_WebAPIResponse
FS_WebAPI::call4(
    const char *api_function,
    const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4) const
{
    UT_WorkBuffer json_data;
    UT_UniquePtr<UT_JSONWriter> json_writer(
	UT_JSONWriter::allocWriter(json_data));

    writeAPICallPrefixToJSON(*json_writer, api_function);
    writeValue(*json_writer, arg1);
    writeValue(*json_writer, arg2);
    writeValue(*json_writer, arg3);
    writeValue(*json_writer, arg4);
    writeAPICallSuffixToJSON(*json_writer);

    return callFunctionWithJSONArgs(json_data);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
FS_WebAPIResponse
FS_WebAPI::call5(
    const char *api_function,
    const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4,
    const T5 &arg5) const
{
    UT_WorkBuffer json_data;
    UT_UniquePtr<UT_JSONWriter> json_writer(
	UT_JSONWriter::allocWriter(json_data));

    writeAPICallPrefixToJSON(*json_writer, api_function);
    writeValue(*json_writer, arg1);
    writeValue(*json_writer, arg2);
    writeValue(*json_writer, arg3);
    writeValue(*json_writer, arg4);
    writeValue(*json_writer, arg5);
    writeAPICallSuffixToJSON(*json_writer);

    return callFunctionWithJSONArgs(json_data);
}

#endif
