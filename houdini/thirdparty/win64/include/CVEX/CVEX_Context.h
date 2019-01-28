/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX_Context.h ( CVEX Library, C++)
 *
 * COMMENTS:	C++ interface to VEX.  This class defines a parameter to the
 *		VEX function.
 */

#ifndef __CVEX_Context__
#define __CVEX_Context__

#include "CVEX_API.h"
#include "CVEX_Function.h"
#include "CVEX_ValueList.h"
#include "CVEX_Transform.h"
#include <VEX/VEX_PodTypes.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_UniquePtr.h>

class cvex_RunData;
class UT_OpCaller;
template <VEX_Precision PREC> class VEX_Instance;
class VEX_GeoInputs;
template <VEX_Precision PREC> class VEX_GeoCommandQueue;
class VEX_ChannelCache;
class VEX_FileCache;

/// @brief Per-run data for CVEX execution
///
/// This class is used to set and retrieve data specific to an individual
/// execution run using CVEX_Context::run().
class CVEX_API CVEX_RunData : public UT_NonCopyable
{
public:
    CVEX_RunData();
    ~CVEX_RunData();

    /// Resets the run data for re-use.
    void		 clear();

    /// Set the evaluation time.  This is what will be used by op: references
    /// triggered by VEX commands like volumesample.  If not set, the current
    /// channel time is used instead (if an OP_Director is available).
    void		 setTime(fpreal time)
			 {
			     myTimeSpecified = true;
			     myTransform.setTime(time);
			 }

    /// Sets the operator working directory.  This is used by ch() 
    /// style vex functions to determine where the relative path for
    /// path resolution should be.
    /// Use OP_Node::getUniqueId() to pass this in.
    void		 setCWDNodeId(int id)
			 {
			     myTransform.setCwdId(id);
			 }
    /// The world id is the node which defines the transform space for the CWD.
    /// If it's not defined the object containing the CWD will be used (or the
    /// CWD if it's not part of an object network)
    void		setWorldNodeId(int id)
			{
			    myTransform.setWorldId(id);
			}

    /// Sets the OP callback. This is used to setup dependencies on any
    /// referenced op: expressions.  Can be applied to the context at any time.
    void		 setOpCaller(UT_OpCaller *caller)
			 {
			     myTransform.setOpCaller(caller);
			 }

    /// Access to the OP_Caller
    UT_OpCaller		*getOpCaller() const { return myTransform.opCaller(); }

    /// Returns true when, after running the CVEX_Context, a ch() function
    /// reported that it was time dependent.
    bool		 isTimeDependent() const { return myTimeDependent; }

    /// Sets the geo input callback for controlling how opinput: references
    /// are handled
    void		 setGeoInputs(const VEX_GeoInputs *geo)
			 {
			     myGeoInputs = geo;
			 }
    const VEX_GeoInputs *getGeoInputs() const { return myGeoInputs; }

    /// Sets the proc id array.  Owned by the caller.  Must be at least
    /// the length of your size.
    void		setProcId(exint *procid)
			{
			    myProcId = procid;
			}
    const exint		*getProcId() const { return myProcId; }

    /// Sets the command queue for this context
    template <VEX_Precision PREC>
    void		 setGeoCommandQueue(VEX_GeoCommandQueue<PREC> *geocmd)
    {
	myGeoCommandQueue = (void*) geocmd;
    }
    template <VEX_Precision PREC>
    VEX_GeoCommandQueue<PREC> *getGeoCommandQueue() const
    { 
    	return (VEX_GeoCommandQueue<PREC>*) myGeoCommandQueue;
    }

    VEX_ChannelCache *getChannelCache() { return myChannelCache; }

    VEX_FileCache* getFileCache() { return myFileCache; }

    /// Every VEX function has a transform context associated with it.  This
    /// transform context is used by VEX functions like ptransform() to provide
    /// ways to transform to other spaces (like "space:world" or
    /// "space:object").  This method allows you to modify the transform
    /// context of this shader.
    CVEX_Transform	&getTransform()	{ return myTransform; }

    /// @{
    /// Accessors
    bool		 timeSpecified() const	{ return myTimeSpecified; }
    bool		 timeDependent() const	{ return myTimeDependent; }
    int			 cwdId() const		{ return myTransform.cwdId(); }
    int			 worldId() const	{ return myTransform.worldId();}
    fpreal		 time() const		{ return myTransform.time(); }
    /// @}
    /// Set as time dependent flag
    void		setTimeDependent(bool v)	{ myTimeDependent = v; }

private:
    CVEX_Transform	 myTransform;
    const VEX_GeoInputs	*myGeoInputs;
    void		*myGeoCommandQueue;
    exint		*myProcId;
    bool		 myTimeSpecified;
    bool		 myTimeDependent;
    VEX_ChannelCache	*myChannelCache;
    VEX_FileCache       *myFileCache;
};

/// @brief Call VEX from C++
///
/// The CVEX_Context class provides the main interface to let C++ code call VEX
/// to perform computations.  This allows users to modify algorithms by
/// performing computations in VEX.
/// - VEX automatically takes advantage of SSE
/// - VEX can perform run-time optimization
class CVEX_API CVEX_Context : public UT_NonCopyable 
{
public:
     CVEX_Context();
    ~CVEX_Context();

    /// clearing the context will allow you to set up the input and output
    /// parameters again.
    /// @note load() must be called again before you can run the VEX code.
    void	clear();

    /// calling clearAllFunctions() will force all CVEX object code to be
    /// flushed out of memory and to be reloaded.  Be cautioned that this may
    /// have side-effects, and should only be called at a safe time.
    /// @note This will also cause *all* functions to be cleared (see clear()).
    static void	clearAllFunctions();

    /// This method will return true if the code referenced by the context has
    /// been deleted (see @clearAllFunctions()).  If you've cached a
    /// CVEX_Context, then this can be used to see if it's still valid.
    bool	isValid() const;

    /// Add possible input parameters to the function.  These are parameters
    /// whose values are overridden by values you pass in.  If the user's VEX
    /// function has these parameters the C++ code should assign the values
    /// after calling load(), but before calling run().
    ///
    /// Calling this version of addInput() allows you to defer computing the
    /// value of the variable until you know whether it will actually be used
    /// by the VEX function.
    bool	addInput(const UT_StringHolder &name,
                         CVEX_Type type, VEX_Precision, bool varying);
    bool	addInput(const UT_StringHolder &name,
                         CVEX_Type type, bool varying)
    {
    	return addInput(name, type, VEX_32, varying);
    }

    /// If you know the value beforehand, you can add the symbol and it's
    /// value at the same time.
    /// Note: The data is referenced, not copied, so keep it live until after
    /// run() has been called.
    bool	addInput(const UT_StringHolder &name,
                         CVEX_Type type, VEX_Precision,
        	         void *data, int array_size);
    bool	addInput(const UT_StringHolder &name,
                         CVEX_Type type,
        	         void *data, int array_size)
    {
    	return addInput(name, type, VEX_32, data, array_size);
    }

    /// Adds a constant input.  You should still maintain the referenc,e
    /// but the data may be constant folded into the assemble, so the
    /// values *must* be set ahead of time and possibly will not update
    /// if you change the original.
    bool	addConstantInput(const UT_StringHolder &name,
                         CVEX_Type type, VEX_Precision,
        	         void *data, int array_size);
    bool	addConstantInput(const UT_StringHolder &name,
                         CVEX_Type type,
        	         void *data, int array_size)
    {
    	return addConstantInput(name, type, VEX_32, data, array_size);
    }
    bool	addConstantInput(const UT_StringHolder &name, CVEX_StringArray &strings);

    /// Add an "string <name>" input.  An array length of 1 makes the variable
    /// uniform.
    /// Note: The strings are referenced, not copied, so keep it live until
    /// after run() has been called.
    bool	addInput(const UT_StringHolder &name, CVEX_StringArray &strings);

    /// Add a required output. If no required output is specified, all
    /// exports/outputs are computed.
    /// Note: Due to the varying/uniform state of an output depending
    /// significantly on the inputs' varying/uniform state -- and operations
    /// performed -- then, unlike addInput, no storage can be allocated until
    /// after load.
    /// Note: If no storage is allocated, the output is still computed but
    /// the result is thrown away.
    bool	addRequiredOutput(const UT_StringHolder &name, CVEX_Type type, VEX_Precision);
    bool	addRequiredOutput(const UT_StringHolder &name, CVEX_Type type)
    {
    	return addRequiredOutput(name, type, VEX_32);
    }

    /// Checks if the VEX function by the given name already exists.
    bool	hasFunction(const UT_StringRef &name) const;

    /// Load the definition of the VEX function.
    /// Usually VEX functions are loaded from compiled VEX code stored
    /// in files on the search path. But, callers can  use this method  to  
    /// define a VEX function from the stream.
    /// The module name can be optionally overriden with a name argument (if
    /// it's NULL, the name in the stream is used implicitly).
    /// The final name of the module is returned in actual_name (if not NULL).
    /// If override_old is true, if the old function by that name is found,
    /// then it will be overriden and updated with the new one.
    bool	preloadFile(UT_IStream &is, const char *name, 
		    UT_String *actual_name, bool override_old);

    /// Loads the given file.  Instead of registering the loaded
    /// function in the global function table, returns it as a CVEX_Function
    CVEX_Function	preloadFunction(UT_IStream &is);

    /// Loads the functin form the global function table.
    CVEX_Function	preloadGlobalFunction(const char *funcname);

    /// Load the VEX function.
    /// Inputs must be specified @b before this function is called.  After
    /// loading, the input list will have flags set telling you whether the
    /// input parameter is used.  At this point, you should set the data for
    /// all used inputs.
    ///
    /// The list of outputs will also be defined, meaning that you can figure
    /// out what's going to be written by the VEX function.
    template <VEX_Precision PREC = VEX_32>
    bool	load(int argc, const char *const argv[]);

    /// With load function we already have pre-loaded the CVEX_Function
    /// so the argv[0] is ignored.
    template <VEX_Precision PREC = VEX_32>
    bool	loadFunction(CVEX_Function function, int argc, const char *const argv[]);

    /// Quick test to see if the function has been loaded.
    bool	isLoaded() const;

    /// The list of input parameters to the function.  It's possible that
    /// these values may be shared between the input and output lists.
    CVEX_ValueList	&getInputList()		{ return myInputs; }
    const CVEX_ValueList &getInputList() const	{ return myInputs; }
    
    /// Find an input by name/type.
    const CVEX_Value	*findInput(const UT_StringRef &name, CVEX_Type type) const
			 { return myInputs.getValue(name, type); }
    CVEX_Value		*findInput(const UT_StringRef &name, CVEX_Type type)
			 { return myInputs.getValue(name, type); }
    
    /// Find an input by name.
    const CVEX_Value	*findInput(const UT_StringRef &name) const
			 { return myInputs.getValue(name, CVEX_TYPE_INVALID); }
    CVEX_Value		*findInput(const UT_StringRef &name)
			 { return myInputs.getValue(name, CVEX_TYPE_INVALID); }

    /// The list of output parameters from the function.  After the function
    /// has been run, the output parameters will have their values written to
    /// by VEX.
    ///
    /// If the output has not had CVEX_Value::setData() called, then the data
    /// will have been written to internal storage and can be retrieved calling
    /// CVEX_Value::getData().
    const CVEX_ValueList &getOutputList() const	{ return myOutputs; }
    CVEX_ValueList &getOutputList() 		{ return myOutputs; }
    
    /// Find an output by name/type.
    const CVEX_Value	*findOutput(const UT_StringRef &name, CVEX_Type type) const
			 { return myOutputs.getValue(name, type); }
    CVEX_Value		*findOutput(const UT_StringRef &name, CVEX_Type type)
			 { return myOutputs.getValue(name, type); }
    
    /// Find and output by name.
    const CVEX_Value	*findOutput(const UT_StringRef &name) const
			 { return myOutputs.getValue(name, CVEX_TYPE_INVALID); }
    CVEX_Value		*findOutput(const UT_StringRef &name)
			 { return myOutputs.getValue(name, CVEX_TYPE_INVALID); }

    /// Initializes the values array with the defaults of the given parameter.
    /// Leaves values empty if it can't find the parameter.
    void		 getParameterDefaults(const UT_StringRef &name,
				CVEX_Type ctype,
				UT_DoubleArray &values) const;

    /// Run the VEX function given a list of input variables and a list of
    /// output parameters.  Each input/output parameter under your control
    /// should have an array size of either 1 or at least the array_size given
    /// to the run() method.  It's possible to run on fewer array elements
    /// than have been allocated, but an error will be returned if there are
    /// input parameters which don't have enough allocation.
    ///
    /// Pass in true for interruptable when running from within Houdini.
    ///
    /// The run() function may be called multiple times, provided that the
    /// input parameters don't change.  So, if you need to evaluate the data
    /// in chunks, you can do this by re-initializing the input parameter data
    /// between calls to run().  However, you should not change the
    /// uniform/varying state of any input parameters without doing a re-load
    /// of the VEX function.
    /// @param array_size The size of varying arrays.  All varying arrays must
    ///   be this size.
    /// @param interruptable If true, VEX will check the state of the
    ///   UT_Interrupt.  This should be enabled when called from within
    ///   Houdini.  If interruptable is false, then the user will @b not be
    ///   able to break out of endless loops in VEX.  It's better to leave it
    ///   true if you are unsure.
    /// @param rundata Data that matches the precision PREC.
    template <VEX_Precision PREC = VEX_32>
    bool	run(int array_size, bool interruptable,
    	    CVEX_RunData *rundata = nullptr);

    /// If load() or run() return false, this will return the error that
    /// triggered the CVEX failure. Note that this is distinct from errors
    // and warnings occurring when running VEX.
    const char *getLastError() const { return myError; }

    /// If load() or run() failed or reported warnings, these methods will
    /// return the errors reported by VEX.
    /// {
    const UT_String	&getVexErrors() const;
    const UT_String	&getVexWarnings() const;
    /// }

    /// Clear the errors reported by getVexErrors() / getVexWarnings().
    void                 clearVexErrors();

private:
    struct cvex_BoundValue
    {
	cvex_BoundValue()
	    : myValue(nullptr)
	    , myBindPtr(nullptr)
	    , myStoragePtr(nullptr)
	{}
	CVEX_Value	*myValue;
	void		*myBindPtr;
	void		*myStoragePtr;
    };

    template <VEX_Precision PREC>
    bool		 validateValue(const CVEX_Value &value,
				VEX_Instance<PREC> &state, int nproc);
    template <VEX_Precision PREC>
    void		 copyBoundData(cvex_BoundValue &bound,
				VEX_Instance<PREC> &state,
				int start, int nproc);
    template <VEX_Precision PREC>
    bool		 bindValues(UT_Array<cvex_BoundValue> &bound_vals,
				VEX_Instance<PREC> &state, int nproc);
    template <VEX_Precision PREC>
    void		 extractStrings(CVEX_Value &value,
				VEX_Instance<PREC> &state,
				int start, int nproc);
    template <VEX_Precision PREC>
    void		 extractArrays(CVEX_Value &value,
				VEX_Instance<PREC> &state,
				int start, int nproc);
    template <VEX_Precision PREC>
    void		 extractUniform(CVEX_Value &value,
				VEX_Instance<PREC> &state);

    template <VEX_Precision PREC>
    bool	loadPrivate(const VEX_AssemblePtr &as,
			int argc, const char *const argv[]);

    CVEX_ValueList		myInputs;
    CVEX_ValueList		myOutputs;
    UT_UniquePtr<cvex_RunData>	myRunData;
    UT_String			myError;
};

#endif
