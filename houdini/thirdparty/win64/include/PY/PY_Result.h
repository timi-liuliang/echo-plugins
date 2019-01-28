/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This class holds the result of python code that was run.
 */

#ifndef __PY_Result_h__
#define __PY_Result_h__

#include "PY_API.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_Options.h>
#include <UT/UT_Variant.h>
#include <vector>

// This class contains information about the return value of python code
// that was executed.
class PY_API PY_Result
{
public:
    PY_Result()
    : myResultType(NONE)
    , myStringValue(UT_String::ALWAYS_DEEP)
    , myErrValue(UT_String::ALWAYS_DEEP)
    , myDetailedErrValue(UT_String::ALWAYS_DEEP)
    , myExceptionClass(UT_String::ALWAYS_DEEP)
    , myOpaquePyObject(NULL)
    , myVoidPtr(NULL)
    {}

    UT_StringHolder asStringForDebugging();

    UT_Variant myValue;
    
    // Errors (uncaught exceptions) and exit requests (because sys.exit()
    // was called) are possible result types, in addition to the standard
    // int, double, and string types.  (Python uses doubles internally,
    // so double is used instead of float.)  If the python None object is
    // returned or some other object is returned, the result type is "none".
    // If you request a result type of PY_OBJECT, the raw PyObject * is
    // set and it is up to you to decrement the reference count on it.
    enum Type { INT, DOUBLE, STRING, INT_ARRAY, DOUBLE_ARRAY, STRING_ARRAY,
		OPTIONS, PY_OBJECT, VOID_PTR, NONE, EXIT, ERR, VARIANT };
    Type myResultType;

    // Type     	Fields Used
    // -------  	-----------
    // INT      	myIntValue
    // DOUBLE   	myDoubleValue
    // STRING   	myStringValue
    // INT_ARRAY	myIntArray
    // DOUBLE_ARRAY	myDoubleArray
    // STRING_ARRAY	myStringArray
    // OPTIONS		myOptions
    // PY_OBJECT	myOpaquePyObject
    // VOID_PTR		myVoidPtr
    // VARIANT		myValue - Always filled in.
    // NONE     	none
    // EXIT     	myIntValue for exit code, myStringValue in case
    //			user called sys.exit() with a non-integer object
    // ERR      	myDetailedErrValue for exception back trace
    //			myErrValue for a simplified error message
    //			myExceptionClass is the name of the exception (if any)

    int myIntValue;
    double myDoubleValue;
    UT_String myStringValue;
    std::vector<int> myIntArray;
    std::vector<double> myDoubleArray;
    UT_StringArray myStringArray;
    UT_Options myOptions;
    UT_String myErrValue;
    UT_String myDetailedErrValue;
    UT_String myExceptionClass;
    void *myOpaquePyObject;
    void *myVoidPtr;
};

#endif
