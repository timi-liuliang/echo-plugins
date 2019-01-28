/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_Context.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Contexts.
 */

#ifndef __CE_Context__
#define __CE_Context__

#include "CE_API.h"

#ifdef CE_ENABLED

#include <SYS/SYS_Types.h>

#include <UT/UT_StringMap.h>
#include <UT/UT_Map.h>
#include <UT/UT_Error.h>
#include <UT/UT_Array.h>

class CE_MemoryPool;

typedef void (*CE_ErrorCB)(const char *errmsg, UT_ErrorSeverity severity, 
			   void *data);

class CE_API CE_DelayedOGLBindBuffer
{
public:
    CE_DelayedOGLBindBuffer(){}
    virtual ~CE_DelayedOGLBindBuffer(){}

    virtual void rebindOGLBuffer( uint buf_obj ) = 0;
    virtual void unbindOGLBuffer() = 0;
    virtual bool isBinded() = 0;
};

/// CE_Context encapsulates the OpenCL context and provides various convenience
/// functions for loading kernel programs and allocating GPU memory.
class CE_API CE_Context
{
public:
    CE_Context();
    virtual ~CE_Context();

    /// Returns a pointer to the singleton CE_Context object.  This function
    /// initializes attempts to initialize OpenCL if it has not yet been.
    static CE_Context *getContext(bool gl_shared=false);

    /// Returns the underlying cl::Context object.
    cl::Context getCLContext() const {return myContext;}

    /// Returns the cl::Queue object that is used to enqueue OpenCL kernels
    /// and memory transfers.
    cl::CommandQueue getQueue() const {return myQueue;}

    /// Returns the OpenCL Device object.
    cl::Device getDevice() const {return myDevice;}

    // Write OpenCL Device info to the supplied buffer.
    void getInfo(UT_WorkBuffer &buffer );
    void getExtendedInfo(UT_WorkBuffer &buffer );

    // Write info for all available OpenCL platforms to the supplied buffer.
    static void getAllPlatformsInfo(UT_WorkBuffer &buffer);

    /// Get the suggested global and local ranges for the given 1-D kernel over
    /// the specified number of items.
    void get1DRanges(const cl::Kernel &k, size_t items,
		     cl::NDRange &g, cl::NDRange &l);

    /// Loads the OpenCL program specified by progname.  This functions searches
    /// for the file in the HOUDINI_OCL_PATH environment variable.  Any compile-
    /// time options can be passed in the options parameter.  If the program
    /// load succeeds, the progname will be cached, using the progrname and
    /// options strings together as a hash value lookup.  In this way the same
    /// OpenCL program can be loaded several times with different compile-time
    /// flags.
    cl::Program loadProgram(const char *progname, const char *options = NULL,
			bool recompile = false);
    cl::Program compileProgram(const char *progtext, const char *options = NULL,
			bool recompile = false);

    /// Create an OpenCL kernel named kernelname from the program  specified by
    /// progname.  For some types of devices these kernels will be cached, as
    /// kernels can be expensive to create.   This is the recommended method
    /// for creating kernels.
    cl::Kernel loadKernel(const cl::Program &prog, const UT_StringRef &kernelname);
    cl::Kernel loadKernel(const char *progname, const UT_StringRef &kernelname,
			  const char *options = NULL)
    { return loadKernel(loadProgram(progname, options), kernelname); }

    /// Returns whether the CE_Context has been successfully initialized.
    bool isValid() const {return myIsValid;}

    /// Returns whether the singleton CE_Context has been initialized yet.  This
    /// can be used to test whether OpenCL has been initialized without calling
    /// getContext and forcing an attempt at initialization.
    static bool isInitialized(bool gl_shared=false);

    /// Returns true if the OpenCL device is running on the CPU.
    bool isCPU() const;

    /// Returns true if the OpenCL device supports double precision.
    bool hasDoubleSupport() const {return mySupportsDouble;}

    /// Block until any outstanding kernel or memory transfers on the main
    /// CommandQueue have executed.  If sweepPool is true, the context's
    /// CE_MemoryPool will sweep for any buffers that were in use when their
    /// CE_Grid's went out of scope, but that were still active in kernels.
    void finish(bool sweepPool=true);

    /// Allocate a buffer of specified size on the CE_Device.
    /// usePool= true, attempts to use the underlying CE_MemoryPool to possibly return
    /// an already allocated, unused buffer.
    /// read=true, creates a buffer that is readable inside kernels.
    /// write=true, creates a buffer that is writable inside kernels.
    /// ogl_bind, specifies an OGL buffer to bind to.
    cl::Buffer allocBuffer(int64 size, bool usePool=true, bool read=true, bool write=true, uint32 ogl_bind=SYS_UINT32_MAX);

    /// Release the specified buffer, possibly to the CE_MemoryPool.
    void releaseBuffer(const cl::Buffer &buf);

    /// Keep a map buffer to bind at render time
    /// The first time a CL::Buffer is created it can be registered to rebing to a OGL vertex buffer at drawing time.
    /// The uint returned by the register call can be attached to a detail attribute and the drawing code can convert
    /// the CL Buffer to a CL BufferGL.
    uint32 registerDelayedOGLBindBuffer(CE_DelayedOGLBindBuffer* buffer);
    void unregisterDelayedOGLBindBuffer(uint32 id);
    CE_DelayedOGLBindBuffer* lookupDelayedOGLBindBuffer( uint id );

    /// Clear the CE_MemoryPool object.
    void clearMemoryPool();

    /// Return a pointer to pinned (page-locked) host memory. On some devices
    /// (Nvidia), using this type of memory for the PCI/E host/device transfers
    /// can double the throughput.  Will return NULL if the memory can't be
    /// allocated, or if the device is not a GPU.
    fpreal32 *getPinnedBuffer(int64 size);

    cl::Buffer getXNoiseData();

    /// Standard error reporting for OpenCL exceptions.  They should generally
    /// take the form:
    /// @code
    /// try
    /// {
    ///      OpenCL calls...
    /// }
    /// catch(cl::Error &err)
    /// {
    ///      CE_Context::reportError(err);
    ///      ///cleanup
    /// }
    /// @endcode
    /// This will not capture delayed errors, however.  Instead
    /// you will need to add a callback to intercept them.
    static void reportError(const cl::Error &err);
    static void outputErrorMessage(const char *errMsg);
    static void setErrorCB(CE_ErrorCB callback, void *data);
    static void outputWarningMessage(const char *errMsg);

    static void initMainSharedGLContext( int devicetype, void* context, void* display );
    static bool useHalfNormalDelayedBindBuffer();

    /// Marks that an operation has run out of memory, allowing us
    /// to report elsewhere.
    void setOutOfMemoryFailure(bool hasfailed = true) { myOutOfMemoryFailure = true; }
    bool hasOutOfMemoryFailureHappened() const { return myOutOfMemoryFailure; }

    /// This structure holds a device name, vendor, and device number with respect to
    /// its vendor platform.
    struct DeviceDescriptor
    {
        UT_String       label;
        UT_String       vendor;
        int             number;
    };
    /// Get the vector of available devices of the given type.
    static void getDevices(UT_Array<DeviceDescriptor>&, cl_device_type t);
    /// Returns true if environment variables are set that override preferences.
    static bool isEnvironmentOverride();

protected:
    cl::Program *doCompileProgram(const char *progtext, const char *options);

    /// Initialize the context for the given device.
    void init(cl::Context &context, cl::Device &device);

    /// Releases the pinned, page-locked memory buffer.
    void releasePinnedBuffer();


    cl::Context             myContext;
    cl::CommandQueue        myQueue;
    cl::Device              myDevice;
    bool                    myIsValid;
    bool                    mySupportsDouble;
    cl::Buffer		    myXNoiseData;

    struct KernelInfo
    {
	UT_StringHolder		 name;
	cl::Kernel		*kernel;
    };

    UT_StringMap<cl::Program *>	myProgramTable;
    UT_Map<const _cl_program *, UT_Array<KernelInfo> *> myKernelTable;

    CE_MemoryPool           *myMemPool;

    // The pinned buffer is unique to the main thread.
    cl::Buffer              myPinnedBuffer;
    fpreal32                *myPinnedData;

    bool		    myOutOfMemoryFailure;

    UT_Map<uint32,CE_DelayedOGLBindBuffer*> myDelayedOGLBindBuffers;

    static void*                        theGLContext;
    static void*                        theGLDisplay;
    static int                          theGLDeviceType;
};


#endif
#endif

