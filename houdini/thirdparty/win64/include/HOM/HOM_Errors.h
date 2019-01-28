/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Errors_h__
#define __HOM_Errors_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include <string>

// HOM_SystemExit is a special exception that gets translated into a python
// SystemExit exception via swig typemaps.  hou.exit() will raise a
// SystemExit exception, much like sys.exit() does, to unroll out of the
// python call stack.
SWIGOUT(%exceptionclass HOM_SystemExit;)
SWIGOUT(%rename(SystemExit) HOM_SystemExit;)
class HOM_API HOM_SystemExit
{
public:
    HOM_SystemExit(int exit_code)
    : myExitCode(exit_code)
    {}

    int code()
    { return myExitCode; }

private:
    int myExitCode;
};


// This is the base class for all HOM error exceptions.  Having a base class
// makes it easier to catch HOM error exceptions, both in C++ (from Houdini)
// and in scripting languages.
SWIGOUT(%exceptionclass HOM_Error;)
// HOM_Error::clone() should only be useful in C++ context, and doesn't need to
// be wrapped by SWIG. Ignoring the clone() method in the base class will also
// ignore the clone() method in the derived classes.
SWIGOUT(%ignore HOM_Error::clone;)
SWIGOUT(%rename(Error) HOM_Error;)
class HOM_API HOM_Error
{
public:
    HOM_Error(const char *instance_message = "")
    : myInstanceMessage(instance_message ? instance_message : "")
    {}

    virtual ~HOM_Error()
    {}

    virtual HOM_Error* clone() { return new HOM_Error(*this); } \

    virtual std::string exceptionTypeName() { return "Error"; }

    virtual std::string description() { return ""; }

    virtual std::string instanceMessage() { return myInstanceMessage; }

    std::string __repr__()
    {
	std::string result = "<hou.";
	result += exceptionTypeName();
	result += ">";
	return result;
    }

    std::string __str__()
    {
	std::string result;
	if (description().size())
	{
	    result += description();
	    result += ".";
	}
	std::string instance_msg = instanceMessage();
	if (instance_msg.size())
	{
	    if (description().size())
		result += "\n";
	    result += instance_msg;
	}
	return result;
    }

private:
    std::string myInstanceMessage;
};

// We tell swig that each subclass overrides __repr__.  Otherwise, swig
// will create its own __repr__ for each subclass and won't call the base
// class's one.
#define SIMPLE_EXCEPTION(exception_name, description_value) \
SWIGOUT(%rename(exception_name) HOM_ ## exception_name;) \
class HOM_API HOM_ ## exception_name : public HOM_Error \
{ \
public: \
    HOM_ ## exception_name(const char* instance_message = "") \
    : HOM_Error(instance_message) \
    {} \
    virtual HOM_Error* clone() { return new HOM_ ## exception_name(*this); } \
    virtual std::string exceptionTypeName() { return #exception_name; } \
    virtual std::string description() { return description_value; } \
    SWIGOUT(std::string __repr__();) \
    SWIGOUT(std::string __str__();) \
};

#define SIMPLE_EXCEPTION_DFLT_INSTANCE(exception_name, description_value, \
				       default_instance_value) \
SWIGOUT(%rename(exception_name) HOM_ ## exception_name;) \
class HOM_API HOM_ ## exception_name : public HOM_Error \
{ \
public: \
    HOM_ ## exception_name(const char* instance_message = "") \
    : HOM_Error(instance_message) \
    {} \
    virtual HOM_Error* clone() { return new HOM_ ## exception_name(*this); } \
    virtual std::string exceptionTypeName() { return #exception_name; } \
    virtual std::string description() { return description_value; } \
    virtual std::string instanceMessage() \
    { \
	std::string instance_msg = HOM_Error::instanceMessage(); \
	if (!instance_msg.size()) instance_msg = default_instance_value; \
	return instance_msg; \
    } \
    SWIGOUT(std::string __repr__();) \
    SWIGOUT(std::string __str__();) \
};

SIMPLE_EXCEPTION(NotAvailable,
		 "Not available in this context")
SIMPLE_EXCEPTION(ObjectWasDeleted,
		 "Attempt to access an object that no longer exists in Houdini")
SIMPLE_EXCEPTION(InvalidInput,
		 "Invalid input")
SIMPLE_EXCEPTION(InvalidSize,
		 "Invalid size")
SIMPLE_EXCEPTION(TypeError,
		 "Invalid type")
SIMPLE_EXCEPTION(ValueError,
		 "Invalid value")
SIMPLE_EXCEPTION(OperationFailed,
		 "The attempted operation failed")
SIMPLE_EXCEPTION(InvalidNodeType,
		 "The node type is invalid")
SIMPLE_EXCEPTION(InitScriptFailed,
		 "Node initialization script failed")
SIMPLE_EXCEPTION(MatchDefinitionError,
		 "Failed to match node type definition")
SIMPLE_EXCEPTION_DFLT_INSTANCE(PermissionError, "",
		 "Failed to modify node or parameter because of a permission "
		 "error.  Possible causes include locked assets, takes, "
		 "product permissions or user specified permissions")
SIMPLE_EXCEPTION(GeometryPermissionError,
		 "Geometry is read-only")
SIMPLE_EXCEPTION(KeyframeValueNotSet,
		 "This keyframe value is not set")
SIMPLE_EXCEPTION(OperationInterrupted,
		 "The requested operation was interrupted")
SIMPLE_EXCEPTION(LoadWarning,
		 "Warnings were generated during load")
SIMPLE_EXCEPTION(NodeError,
		 "Error generated by Python node")
SIMPLE_EXCEPTION(NodeWarning,
		 "Warning generated by Python node")
SIMPLE_EXCEPTION(NameConflict,
		 "Name conflict was detected")

#endif
