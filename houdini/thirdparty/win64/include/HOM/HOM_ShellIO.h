/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShellIO_h__
#define __HOM_ShellIO_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include <UT/UT_Tuple.h>
#include <string>

SWIGOUT(%rename(ShellIO) HOM_ShellIO;)

class HOM_API HOM_ShellIO
{
public:
    HOM_ShellIO()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShellIO(const HOM_ShellIO &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShellIO()
    { HOM_DESTRUCT_OBJECT(this) }

    // When this object is sys.stdin, python will call readline() to read
    // a command.
#ifdef SWIG
%extend {
    %kwargs readline;
    InterpreterObject readline(int size=-1)
    {
	// readline() is just a wrapper around readlineWithKeyboardInterrupt().
	// We need a way of knowing if this function raised a KeyboardInterrupt
	// exception, though.
	UT_Tuple<std::string, bool> result =
	    self->readlineWithKeyboardInterrupt(size);
	if (UTtupleGet<1>(result))
	{
	    // A KeyboardInterrupt exception has been set.  Return NULL
	    // to let python know to check for an exception.
	    return NULL;
	}
	return HOMconvertValueForInterpreter(UTtupleGet<0>(result),
                                             SWIG_POINTER_OWN);
    }

    void addCloseCallback(InterpreterObject callback)
    { self->addOpaqueCloseCallback(callback); }

    void removeCloseCallback(InterpreterObject callback)
    { self->removeOpaqueCloseCallback(callback); }

    // Note that we return a vector of borrowed references because swig
    // will increment the reference counts.
    std::vector<InterpreterObject> CloseCallbacks()
    {
	std::vector<void *> opaque_callbacks(self->opaqueCloseCallbacks());

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i < opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }

    // These methods are deprecated and can be removed in the future:
    void addExitCallback(InterpreterObject callback)
    { self->addOpaqueCloseCallback(callback); }
    void removeExitCallback(InterpreterObject callback)
    { self->removeOpaqueCloseCallback(callback); }
    std::vector<InterpreterObject> exitCallbacks()
    {
	std::vector<void *> opaque_callbacks(self->opaqueCloseCallbacks());

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i < opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }
}
#endif

    // readlineWithKeyboardInterrupt() is not wrapped by swig.  It is used
    // to implement readline() so readline can return a null PyObject*
    // if it got a KeyboardInterrupt exception.
    SWIGOUT(%ignore readlineWithKeyboardInterrupt;)
    virtual UT_Tuple<std::string, bool /*interrupt_occurred*/>
	readlineWithKeyboardInterrupt(int size) = 0;

    SWIGOUT(%ignore addOpaqueCloseCallback;)
    virtual void addOpaqueCloseCallback(void *callback) = 0;
    SWIGOUT(%ignore removeOpaqueCloseCallback;)
    virtual void removeOpaqueCloseCallback(void *callback) = 0;
    SWIGOUT(%ignore opaqueCloseCallbacks;)
    virtual std::vector<void *> opaqueCloseCallbacks() = 0;
    SWIGOUT(%ignore executeCloseCallbacks;)
    virtual void executeCloseCallbacks() = 0;

    // When this object is set to stdout/stderr, python will call write().
    virtual void write(const std::string &data) = 0;

    // Note that this method is not wrapped by swig.  It is called from
    // SI_PythonShell to determine if it should generate UI events to
    // update the shell with the new data.
    SWIGOUT(%ignore hasWrittenData;)
    virtual bool hasWrittenData() = 0;

    // Note that this method is not werapped by swig.
    // It is called from HOMF_RopNode and used to 
    // output verbose render messages to the Python shell.
    SWIGOUT(%ignore getDataStream;)
    virtual std::stringstream &getDataStream() = 0;

    // getAndClearWrittenData() is called by the shell window to display
    // information written by python.
    virtual std::string getAndClearWrittenData() = 0;

    // addDataForReading() is called by the shell window when the user types
    // something there.
    virtual void addDataForReading(const char *data) = 0;

    // addEOFForReading() is used to signal readline() that it should return,
    // even if no readline() was read.  It's called by the shell window when
    // the user presses Ctrl+d and python isn't waiting for commands.
    virtual void addEOFForReading() = 0;

    // interruptShellThread() is called by the shell window to raise a
    // KeyboardInterrupt exception in the shell thread.  It properly interrupts
    // the python thread if it is currently calling readline().
    virtual void interruptShellThread() = 0;

    // isWaitingForCommand() is called by the shell window to decide if
    // a Ctrl+d should close the shell window or send an EOF.
    // setIsWaitingForCommand() is called by the houdiniInterpreter module
    // to indicate that it's waiting for a command.
    virtual bool isWaitingForCommand() = 0;
    virtual void setIsWaitingForCommand(bool on) = 0;

    // Note that this method is not wrapped by swig.
    // It is called from SI_PythoNShell for synchronization purposes.
    SWIGOUT(%ignore ensureIsReady;)
    virtual void ensureIsReady() = 0;

    virtual bool isatty() = 0;
    virtual void flush() = 0;
};

#endif
