/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLComputeGPU.h ( RE Library, C++)
 *
 * COMMENTS:
 *    A high level interface to allow GP-GPU computing (general purpose
 *    computation on the GPU, or video card). This uses RE_OGLFramebuffer 
 *    (output), RE_Textures (array input), and RE_Shaders (GLSL or Cg program) 
 *    to perform the computation.
 *
 *    Setting up the computation is relatively expensive, so this should only
 *    be used for large volumes of data.
 * 
 *    Once you have set up the program, output and input(s), you can change one
 *    or all of them for the next run. If you call setConstant() or setArray()
 *    with the same name as an existing constant or array, the data will be
 *    updated as long as the data format and size remains the same (otherwise
 *    another constant or texture will be created for it).
 *
 *    You can use special macros to read from the input arrays (which are
 *    textures) using integer indices rather than floating point texture
 *    coords:
 *
 *      IN_array_name		    // at current output pos.
 *	ABS_array_name (int_pos)    // at an absolute position.
 *      REL_array_name (int_offset) // offset from current pos.
 *
 *	(Note: 2D Arrays will have 2 pos / offset parms)
 *
 *    You can use the first version to grab the corresponding input element
 *    only if it matches the output in size and dimensions (1d/2d). Otherwise,
 *    you'll get array bounds issues.
 */
#ifndef RE_OGLComputeGPU_h
#define RE_OGLComputeGPU_h

#include "RE_API.h"
#include "RE_Types.h"
#include <PXL/PXL_Common.h>
#include <IMG/IMG_FileTypes.h>
#include <UT/UT_ComputeGPU.h>

class UT_String;
class re_GPUConstant;
class re_GPUInputArray;
class re_GPUFrameBuffer;
class re_GPUInputArrayProxy;
class re_GPUData;
class re_ProgramRun;
class re_GPUProgram;
class RE_Render;

class RE_API RE_OGLComputeGPU : public UT_ComputeGPU
{
public:
    virtual ~RE_OGLComputeGPU();

    /// Initialize RE GPU implementation. This must be called on startup.
    static void		init();

    static void		initStandalone(bool one_per_thread);
    
    static RE_Render *	getRender();

    // 0. If you're doing multiple passes, you should set how many passes you
    //    wish to do. This is optional - it defaults to 1 pass.
    //    TODO: This is not supported yet.
    virtual void	setNumPasses(int passes);

    // If enabled, your shader is using integer bit operations. Not all cards
    // support this.
    virtual void	needBitOps(bool enable);
    virtual bool	needsBitOps() const { return myBitOpsUsed; }

    // 1. Inputs to computation.
    //    You may rewrite to existing values to begin another computation.
    //	  This class does not take ownership of the value array you pass in -
    //    you are responsible for deleting it, but do not do so until compute()
    //    has been called.
    //    The OpenGL texture size limits the maximum array size. 1D Arrays
    //    have a max of this size squared, 2D arrays must have both dimensions
    //    under this size (see RE_OGLRender::getMaxTextureSize()). setArray()
    //    will return NULL if these limits are exceeeded.

    //    Data types available for I/O
    //      - constants are usually int32's, float32's or matrices.
    //      - input and output arrays cannot be arrays of matrices (currently)

    //    Constants are usually UT_GPU_FLOAT32, UT_GPU_(U)INT32 OR
    //    UT_GPU_MATRIX[n]. They can be a small array, but GLSL will not allow
    //    you to dynamically index them (ie, [x], only [1] or using fixed range
    //    loop variables like x=1 to 10). If you need to dynamically index,
    //    use an Array. Constant arrays are very lightweight, though.
    virtual void	setConstant(const char  *name,
			    UT_GPUType	 type,
			    int		 vectorsize,
			    const void	*value,
			    int array_size = 1,
			    bool global_var = false);
    
    virtual void *	setArray(const char    *name,
			 UT_GPUType	type,
			 int		vectorsize,
			 int		size,
			 const void    *values);

    virtual void *	setArray2D(const char   *name,
			   UT_GPUType	 type,
			   int		 vectorsize,
			   int		 width,
			   int		 height,
			   const void	*values); // in rows.

    // flat arrays to vectors (interleaves to 1,2,3,4, 1,2,3,4). Not all
    // components need to be specified.
    virtual void *	setArray(const char    *name,
			 UT_GPUType	type,
			 int		vectorsize,
			 int		size,
			 const void    *values1,
			 const void    *values2,
			 const void    *values3 = NULL,
			 const void    *values4 = NULL);
    
    virtual void *	setArray2D(const char    *name,
			   UT_GPUType	type,
			   int		vectorsize,
			   int		width,
			   int		height,
			   const void    *values1,
			   const void    *values2,
			   const void    *values3 = NULL,
			   const void    *values4 = NULL);
    
    // 2. Output data.
    //   You can add multiple outputs, but they all must be the same size, type
    //   and vectorsize. 'dest_data' is where the results are written.
    
    virtual void	setOutputArray(const char *name,
			       UT_GPUType type,
			       int vectorsize,
			       int size,
			       void *dest_data);
    
    virtual void	setOutputArray2D(const char *name,
				 UT_GPUType type,
				 int vectorsize,
				 int width,
				 int height,
				 void *dest_data);

    //    Convienience method to determine the size of the array you will need
    //    for an output buffer (or need to provide for an input buffer).
    //    1D arrays only need to pass xsize.
    virtual int		getDataByteSize(UT_GPUType type, int vectorsize,
				int xsize, int ysize = 1);

    // 3. GLSL/Cg Program (false if it fails to compile)
    //    'name' is arbitrary, for debug messages.
    //    'program' is the actual program code.
    //    If 'auto_generate_framework' is true, this class prepends the GLSL
    //    constant and texture declarations (as named above), then
    //    'void main(void) {', your code, and then '}' (so all you need to do is
    //    fill in the actual code). If non-NULL, 'preamble_code' will be placed
    //    before the declarations.

    virtual void	setLanguage(UT_OGLComputeLanguage lang);
	
    virtual bool	setProgram(const char *name,
			   const char *program,
			   bool auto_generate_framework = true,
			   const char *preamble_code = NULL,
			   const char *function_code = NULL);

    //    This allows you to define macros and symbols for preprocessing.
    virtual void	defineSymbol(const char *symbol,
			     const char *value);

    // 4. Computation
    //    Runs the program on the inputs to produce the output. Returns a
    //    pointer to the result (in the output format and dimensions specified
    //    above). Returns false if an opengl error occured.
    //    Once you have called compute, you may begin another computation
    //    before fetching the results (just go back to step 1).
    
    //    'async_read' starts the DMA transfer of the results after the
    //    computation is finshed. Useful when you have more than one output,
    //    or more work to do between compute() and getResult(). Note
    //    that this call will block until the computation is done, so for long
    //    computations, you may want to take the latency hit. If pixel buffer
    //    objects are not supported, this is ignored.

    virtual bool	compute(UT_String &errors, bool async_read = false);

    // 5. Fetch Results
    //    Notify the engine that you'd like to use your results. With multiple
    //    active computations, this removes the oldest and another getResult()
    //	  will begin fetching from the next oldest computation.
    //
    //    Your results will be available _after_ this method is called. This
    //    returns false if your results could not be read. 
    virtual bool	getResults();

    //    Alternatively, you can use a deferred scheme to retrive the data.
    //    1. Call deferred fetch to grab a key to the results.
    //    2. You can optionally call beginDeferredRead() some time afterward
    //       to start the asynchronouse read.
    //    3. Call endDeferredRead() to ensure that all results have been read.
    //    The key is no longer valid after step 3.
    
    virtual void	*useDeferredFetch();

private:
    RE_OGLComputeGPU();

    /// Create a new RE_OGLComputeGPU object.
    /// The caller takes ownership of the returned object and is responsible
    /// for deleting it.
    static UT_ComputeGPU *create();

    bool		initFrameBuffers(RE_Render *r);

    re_ProgramRun	*myProgram;
    bool		 myProgramRunFlag;
    bool		 myBitOpsUsed;
};

class RE_API RE_OGLComputeGPUState : public UT_ComputeGPUState
{
public:
    virtual ~RE_OGLComputeGPUState();

    /// Initialize RE GPU implementation. This must be called on startup.
    static void		init();

    virtual void	initStandalone(bool one_per_thread = true);
    virtual void	cleanupStandalone();
    virtual RE_Render *getRender();
    virtual void getGLSLComputeDir(UT_String &dir);
    
    // Returns true if the graphics hardware has proper support to use this
    // class.
    virtual bool	hasGPUSupport();

    virtual bool	areBitOpsSupported();

    virtual void	beginDeferredRead(void *key);
    virtual bool	endDeferredRead(void *key);
    virtual void cancelDeferredRead(void *key);

    // Returns true if texture rectangles must be used (because standard 2D
    // textures do not support most higher formats on some hardware)
    virtual bool useTextureRectangles();

    // The largest sizes that this GPU can handle.
    virtual int  getMaxArraySize();
    virtual int  getMaxArraySize2D();

    // Clears all the GP-GPU allocated objects. You don't normally need to
    // call this.
    virtual void reset();

    virtual void restoreState();
    virtual void cleanupState();

private:
    RE_OGLComputeGPUState();
};

#endif
