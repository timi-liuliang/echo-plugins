/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_VectorT.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Vector.
 */

#ifndef __CE_Vector__
#define __CE_Vector__

#include "CE_API.h"

#include "CE_Context.h"

#ifdef CE_ENABLED

#include <UT/UT_Vector.h>

/// A simple OpenCL-based vector class that at the moment just mimics just
/// enough of UT_Vector to be used in CE_SparseMatrixELLT::solveConjugateGradient.
template <typename T>
class CE_API CE_VectorT : public CE_DelayedOGLBindBuffer
{
public:

    typedef T	 value_type;

    CE_VectorT();

    /// Initialize to given length.
    CE_VectorT(exint len, bool use_pool=false, bool read=true, bool write=true, bool delayed_ogl_bind=false, bool ogl_ocl=false);

    /// Initialize to zero in in the cases when we want to 
    /// init / not init the vector based on size    
    CE_VectorT(bool use_pool,
    	       bool read,
    	       bool write,
    	       bool delayed_ogl_bind,
    	       bool ogl_ocl);

    /// Copy constructor.
    CE_VectorT(const CE_VectorT<T> &v);

    virtual 	~CE_VectorT();

    /// Initialize to given length.
    /// NOTE- values are undefined.
    void 	init(exint len);

    /// For "compatibility" with UT_Vector.
    /// NOTE: Asserts if nl is not zero!
    void 	init(exint nl, exint nh);

    /// Initialize from a UT_Vector.
    void	initFromVector(const UT_VectorT<T> &src, bool block = true);

    /// Rebing to an OGL buffer
    virtual void rebindOGLBuffer( uint buf_obj );
    virtual void unbindOGLBuffer();
    virtual bool isBinded();
    uint	 delayedBindBufferID() const { return myDelayedBindBufferID; }

    /// Resize the provided UT_Vector to fit and copy this vector's data there.
    void	matchAndCopyToVector(UT_VectorT<T> &dst, bool block = true) const;
    /// Returns the vector length.
    exint	length() const { return myLen; }

    /// Returns the underlying OpenCL buffer.
    const cl::Buffer &buffer() const { return myBuffer; }

    /// The following functions all mimic those of the same name in UT_Vector.
    void	multAndSet(const CE_VectorT<T> &a, const CE_VectorT<T> &b);

    void	negPlus(const CE_VectorT<T> &a);

    void	scaleAddVec(T s, const CE_VectorT<T> &a);

    fpreal64	norm(int type) const;

    void	zero() { setValue(0); }

    void	addScaledVec(T s, const CE_VectorT<T> &a);

    /// Apply Blend Shapes on the current buffer
    /// bs holds all the blend shape positions concatenated together in a single buffer
    /// wi, holds the contribution of the blend shapes with index and weights.
    void	blendShapes(const CE_VectorT<T> &bs, const CE_VectorT<T>& wi, exint num_active_shapes, bool write_half);

    /// Apply Position and Normal blend shapes at the same time in a single kernel
    void	blendShapesPointNormal(CE_VectorT<T> &n, const CE_VectorT<T> &bs, const CE_VectorT<T> &bsn, const CE_VectorT<T>& wi, exint num_active_shapes, bool write_half);

    /// Apply deform shapes on the current buffer
    void        deformShape(const CE_VectorT<T> &shape, 
			    const CE_VectorT<T> &transforms,
			    const CE_VectorT<T> &weightIndices,
			    exint num_max_regions);

    void        deformShapeNormal(const CE_VectorT<T> &shape, 
				  const CE_VectorT<T> &transforms,
				  const CE_VectorT<T> &weightIndices,
				  exint num_max_regions );

    void        deformShapeVector(const CE_VectorT<T> &shape, 
				  const CE_VectorT<T> &transforms,
				  const CE_VectorT<T> &weightIndices,
				  exint num_max_regions,
				  exint num_vec_attr);

   
    void        deformShapeQuaternion(const CE_VectorT<T> &shape, 
				      const CE_VectorT<T> &xforms,
				      const CE_VectorT<T> &weightIndices,
				      exint num_max_regions,
				      exint num_vec_attr);

    void	dqDeformShape(const CE_VectorT<T> &shape,
			      const CE_VectorT<T> &transforms,
			      const CE_VectorT<T> &weightIndices,
			      exint num_max_regions);

    void	dqDeformShapeVector(const CE_VectorT<T> &shape,
				    const CE_VectorT<T> &transforms,
				    const CE_VectorT<T> &weightIndices,
				    exint num_max_regions,
				    exint num_vec_attr,
				    bool normalize_dst);

    // Need to refactor this into one chunk.. or something not so repeated
    void	dqDeformShapeQuaternion(const CE_VectorT<T> &shape,
					const CE_VectorT<T> &dqs,
					const CE_VectorT<T> &weightIndices,
					exint num_max_regions,
					exint num_quat_attr);

    void	dqLinearBlendDeformShape(const CE_VectorT<T> &shape,
					 const CE_VectorT<T> &xforms,
					 const CE_VectorT<T> &dqs,
					 const CE_VectorT<T> &weightIndices,
					 const CE_VectorT<T> &dq_linear_blend,
					 exint num_max_regions);

    void	dqLinearBlendDeformNormal(const CE_VectorT<T> &shape,
					  const CE_VectorT<T> &xforms,
					  const CE_VectorT<T> &dqs,
					  const CE_VectorT<T> &weightIndices,
					  const CE_VectorT<T> &dq_linear_blend,
					  exint num_max_regions);
    
    void	dqLinearBlendDeformVector(const CE_VectorT<T> &shape,
					  const CE_VectorT<T> &xforms,
					  const CE_VectorT<T> &dqs,
					  const CE_VectorT<T> &weightIndices,
					  const CE_VectorT<T> &dq_linear_blend,
					  exint num_max_regions,
					  exint num_attr);

    void	dqLinearBlendDeformQuaternion(const CE_VectorT<T> &shape,
					      const CE_VectorT<T> &xforms,
					      const CE_VectorT<T> &dqs,
					      const CE_VectorT<T> &weightIndices,
					      const CE_VectorT<T> &dq_linear_blend,
					      exint num_max_regions,
					      exint num_attr);
    /// Operators
    /// NOTE: operator= requires the destination be a matching size.
    CE_VectorT &operator= (const CE_VectorT<T> &v);

    /// Reductions of the vector to a single value.
    fpreal64 sum() const;
    fpreal64 sumAbs() const;
    fpreal64 sumSqr() const;
    fpreal64 min() const;
    fpreal64 minAbs() const;
    fpreal64 max() const;
    fpreal64 maxAbs() const;
    fpreal64 average() const {return sum() / length();}

    /// Returns the dot product with provided vector.
    fpreal64 dot(const CE_VectorT<T> &a) const;

    cl::KernelFunctor bind(cl::Kernel &k) const;
    void setValue(T cval);
    void copyFrom(const CE_VectorT<T> &v);
protected:

    const cl::Buffer &allocBuffer() const;
    void releaseBuffer();
    cl::KernelFunctor bind(const char *kernelName) const;
    cl::Kernel loadKernel(const char *kernelName, const char *opt = NULL) const;


    // Reduction helpers
    void getReductionRanges(const cl::Kernel &k,
                            cl::NDRange &globalRange, cl::NDRange &localRange,
                            uint &groupsize, uint &ngroups,
                            size_t &accumsize) const;
    fpreal64 reduceGroup(cl::Buffer out, uint groupsize, uint ngroups,
                        size_t accumsize, const char *reduceFlags) const;

    // Main reduction function.
    fpreal64 doReduce(const char* reduceFlags, const CE_VectorT<T> *a=NULL) const;

    mutable cl::Buffer  myBuffer;
    cl::NDRange		myGlobalRange, myLocalRange;
    exint 		myLen;

    bool		myRead, myWrite, myUsePool;
    uint32		myDelayedBindBufferID;
    uint32		myBufferObject;
    bool		myUseOGLContex;
};

template <typename T>
inline fpreal64
dot(const CE_VectorT<T> &a, const CE_VectorT<T> &b)
{
    return a.dot(b);
}


typedef CE_VectorT<fpreal32>	CE_VectorF;
typedef CE_VectorT<fpreal64>	CE_VectorD;

#else
template <typename T>
class CE_API CE_VectorT
{
};

#endif
#endif
