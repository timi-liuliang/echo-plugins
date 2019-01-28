/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_Grid.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Grid.
 */

#ifndef __CE_Grid__
#define __CE_Grid__

#include "CE_API.h"

#ifdef CE_ENABLED

#include <UT/UT_VoxelArray.h>
#include <UT/UT_String.h>

/// This class represents a 3-dimensional array of float values stored on an
/// OpenCL device.  It is roughly analagous to UT_VoxelArray, although it does
/// not support tiling.  There are various convenience functions for simplifying
/// invocations of OpenCL kernels that operate on grids of values.
///
/// CE_Grid supports an arbitrary number of "ghost" cells, which
/// are cells at the boundary edges of the grid that can be set according to
/// different UT_VoxelBorderType values, as well as arbitrary number of
/// "padding" cells.  The padding cells can be used to ensure that several
/// grids with slightly different resolutions can nonetheless
/// have indentical x, y, and z stride values.  This makes writing
/// kernels that operate on grids that represent face-sampled (MAC) vector
/// array much simpler, since the index into the different grids need only be
/// calculated once.
///
/// Invoking an OpenCL kernel typically involves one of the bind() functions to
/// bind the kernel to the range of work items in the grid, assuming one OpenCL
/// work item per voxel.  There are three different variants of bind(), the
/// default assumes the grid is always 3-dimensional; bind2D() which assumes the
/// grid has a 2-dimensional axis, and bind2D3D(), which will treat a
/// 3-dimensional grid normally, but flatten a 2-d grid for optimal performance
/// on OpenCL devices that prefer a large number of work items in the first
/// dimension.  This last function is the preferred method to call if the grid
/// might represent 2D or 3D data.
///
/// A simple OpenCL kernel that doubles every value in a CE_Grid might look
/// like:
/// @code
/// void __kernel doubleit(__global float *grid,
///                        uint offset, uint xstride, uint ystride, uint zstride)
/// {
///      size_t idx = offset + get_global_id(0) * xstride +
///                            get_global_id(1) * ystride +
///                            get_global_id(2) * zstride;
///      grid[idx] *= 2;
/// }
/// @endcode
///
/// Because this kernel takes x, y, and z strides, it can be used for 2D and 3D
/// data: in the 2D case zstride will always be 0.
/// Invoking this kernel then typically looks like:
/// @code
/// CE_Context *context = CE_Context::getContext();
/// cl::Program prog = context->loadProgram("mykernels.cl");
/// cl::KernelFunctor doubleit = myGrid.bind2D3D(prog, "doubleit");
/// doubleit(myGrid.buffer(), myGrid.getOffset(),
///          myGrid.getXStride2D3D(),
///          myGrid.getYStride2D3D(),
///          myGrid.getZStride2D3D());
/// @endcode
class CE_API CE_Grid
{
public:
    CE_Grid();
    CE_Grid(const CE_Grid &src);
    virtual ~CE_Grid();

    /// Return the underlying OpenCL buffer that can be used in kernel
    /// invocations.  It allocates this buffer only on demand.
    const cl::Buffer &buffer() const
    {
        if (hasBuffer())
            return myBuffer;
        return allocBuffer();
    }

    /// If the current OpenCL buffer is valid.
    bool hasBuffer() const {return (myBuffer() != 0);}

    /// Size the grid as specified.  Note this does not actually allocate
    /// memory on the OpenCL device.
    void  size(int xres, int yres, int zres,
               int xghost = 1, int yghost = 1, int zghost = 1,
               int xpad = 1, int ypad = 1, int zpad = 1);

    int  getXRes() const { return myRes[0]; }
    int  getYRes() const { return myRes[1]; }
    int  getZRes() const { return myRes[2]; }

    int  getRes(int dim) const { return myRes[dim]; }
    UT_Vector3I getRes() const    { return myRes; }

    UT_Vector3I getGhostRes() const { return myGhostCells; }
    int  getGhostRes(int dim) const { return myGhostCells[dim]; }

    UT_Vector3I getPadding() const { return myPadding; }
    int  getPadding(int dim) const { return myPadding[dim]; }

    /// Returns the offset from the beginning of the buffer to the beginning
    /// of actual data.
    int   getOffset() const
    {
        return myGhostCells[0] +
               myGhostCells[1] * myStrides[1] +
               myGhostCells[2] * myStrides[2];
    }
    /// Returns the offset from the beginning of the buffer to the beginning
    /// of the data including ghost cells.
    int  getGhostOffset() const;

    /// Like the identically-named function in UT_VoxelArray, set the values
    /// that determine grid border behaviour.  Note that calling these functions
    /// does not actually set the ghost cell values; updateBorderCells() does
    /// that.
    void                setBorder(UT_VoxelBorderType type, fpreal32 t);
    UT_VoxelBorderType  getBorder() const { return myBorderType; }
    fpreal32            getBorderValue() const { return myBorderValue; }
    void                setBorderScale(fpreal32 scalex,
                                       fpreal32 scaley,
                                       fpreal32 scalez);
    fpreal32            getBorderScale(int axis) const
                                        { return myBorderScale[axis]; }

    /// Invoke a series of kernels to fill the ghost cells along each axis with
    /// the proper values according to the border value and type.
    void updateBorderCells() const;

    /// Returns the number of data voxels in the grid.
    int64 numVoxels() const
    { return ((int64)myRes[0]) * myRes[1] * myRes[2]; }

    /// Returns the number of total voxels in the grid, including ghost cells
    /// and padding.
    int64 numTotalVoxels() const
    {
        return  (int64)(myRes[0] + 2 * myGhostCells[0] + myPadding[0]) *
                (int64)(myRes[1] + 2 * myGhostCells[1] + myPadding[1]) *
                (int64)(myRes[2] + 2 * myGhostCells[2] + myPadding[2]);
    }

    /// Returns the memory required by the entire grid, including ghost cells
    /// and padding.
    int64 totalVoxelMemory() const
    { return sizeof(fpreal32) * numTotalVoxels(); }

    /// Initialize the CE_Grid from the supplied UT_VoxelArray.  Note that
    /// in the case that UT_VoxelArray::isConstant(), this will be very fast
    /// and avoid allocating any actual GPU memory.
    void  initFromVoxels(const UT_VoxelArrayF &src,
                         int xghost = 1, int yghost = 1, int zghost = 1,
                         int xpad = 1, int ypad = 1, int zpad = 1);

    /// Match destination UT_VoxelArray to this CE_Grid and copy data.  Note
    /// if isConstant() is true, this is very fast.  includeGhostCells can be
    /// set to include the ghost cell values in the sizing and copy operations.
    void  matchAndCopyToVoxels(UT_VoxelArrayF& dest,
                               bool includeGhostCells = false) const;

    /// Match the src CE_Grid in terms of size and border conditions.
    void match(const CE_Grid &src);

    /// Returns true if this CE_Grid matches src in terms of size and
    /// border conditions.
    bool isMatching(const CE_Grid &src) const;

    /// Returns true if this CE_Grid matches src in terms of offset and strides.
    bool isCongruent(const CE_Grid& src) const;

    int getXStride() const {return myStrides[0];}
    int getYStride() const {return myStrides[1];}
    int getZStride() const {return myStrides[2];}
    int  getStride(int dim) const { return myStrides[dim]; }

    UT_Vector3I getStrides() const { return myStrides; }

    /// Returns the stride along axis including ghost cells.
    int  getGhostStride(int axis) const;

    /// Returns whether the specified axis is 2-dimensional.
    bool isAxis2D(int axis) const
    {
        return (myRes[axis] == 1);
    }

    /// Returns the 2-dimensional axis of this grid, or -1 if there is none.
    int getAxis2D() const {return myAxis2d;}

    /// When flattening a grid to 2-dimensions, this is the axis to treat as
    /// the x-axis, usually when using bind2D.
    static int getXAxis2D(int axis2d)
    {
        return (axis2d > 0) ? 0 : 1;
    }

    /// When flattening a grid to 2-dimensions, this is the axis to treat as
    /// the y-axis, usually when using bind2D.
    static int getYAxis2D(int axis2d)
    {
        return (axis2d < 2) ? 2 : 1;
    }

    /// 2D strides.
    int  getXStride2D(int axis2d) const {return myStrides[getXAxis2D(axis2d)];}
    int  getYStride2D(int axis2d) const {return myStrides[getYAxis2D(axis2d)];}

    /// Returns the true x-stride if the grid is 3D, or the flattened x-stride
    /// if 2D.
    int  getXStride2D3D() const
    {
        if (myAxis2d == -1)
            return myStrides[0];
        return getXStride2D(myAxis2d);
    }

    /// Returns the true Y-stride if the grid is 3D, or the flattened y-stride
    /// if 2D.
    int   getYStride2D3D() const
    {
        if (myAxis2d == -1)
            return myStrides[1];
        return getYStride2D(myAxis2d);
    }

    /// Returns the true Z-stride if the grid is 3D, or 0 if 2D.
    int   getZStride2D3D() const
    {
        if (myAxis2d == -1)
            return myStrides[2];
        return 0;
    }

    /// Returns the true x resolution if the grid is 3D, or the flattened x
    /// resolution if 2D.
    int  getXRes2D3D() const
    {
        if (myAxis2d == -1)
            return myRes[0];
        return myRes[getXAxis2D(myAxis2d)];
    }

    /// Returns the true y resolution if the grid is 3D, or the flattened y
    /// resolution if 2D.
    int   getYRes2D3D() const
    {
        if (myAxis2d == -1)
            return myRes[1];
        return myRes[getYAxis2D(myAxis2d)];
    }

    /// Returns the true z resolution if the grid is 3D, or 1 if 2D.
    int   getZRes2D3D() const
    {
        if (myAxis2d == -1)
            return myRes[2];
        return 1;
    }

    /// Set this grid to the specified constant value.  This will release the
    /// underlying OpenCL buffer and store the constant value in the CE_Grid
    /// object.
    void constant(fpreal32 v);

    void zero() { constant(0); }

    /// Returns whether the grid is set to a constant value.  If checkBorders is
    /// true, checks that the border type and value are equal as well.
    bool isConstant(fpreal32 *cval = 0, bool checkBorders = false) const;

    /// Copy data from the source grid.  This requires that the grids have the
    /// same data resolution, but ghost cells, padding, borders, etc. can differ.
    void copyData(const CE_Grid &src);

    /// Assign to this, equivalent to match(src), followed by copyData(src).
    CE_Grid &operator=(const CE_Grid &src);

    /// Steal the buffer from the other grid, leaving it unitialized.
    void stealBuffer(CE_Grid &src);

    /// Add the source array values to this. Calls linearCombination() and
    /// requires isCongruent(src).
    CE_Grid &operator +=(const CE_Grid &src);

    /// Return an OpenCL NDRange comprised of the entire grid resolution,
    /// implying one OpenCL work item per voxel.
    cl::NDRange getGlobalRange() const
    {
        return cl::NDRange(getXRes(),
                           getYRes(),
                           getZRes());
    }

    /// Return an OpenCL NDRange comprised of the flattened 2D grid resolution,
    /// implying one OpenCL work item per voxel.
    cl::NDRange getGlobalRange2D(int axis2d) const
    {
        return cl::NDRange(myRes[getXAxis2D(axis2d)],
                           myRes[getYAxis2D(axis2d)]);
    }

    /// Return an OpenCL NDRange comprised of the entire 3D grid resolution if
    /// the grid is 3D, else the flattened 2D grid resolution.
    cl::NDRange getGlobalRange2D3D() const
    {
        return (myAxis2d == -1) ? getGlobalRange() :
                                  getGlobalRange2D(myAxis2d);
    }

    /// Create a local work item range for the supplied global range.
    static cl::NDRange  getLocalRange(const cl::NDRange &g);
    cl::NDRange   getLocalRange2D3D() const
    { return getLocalRange(getGlobalRange2D3D()); }

    /// Bind a 3D kernel, with one work item per voxel.
    cl::KernelFunctor bind(cl::Kernel k) const;
    cl::KernelFunctor bind(cl::Program prog,
                             const char *kernelname) const;

    /// Bind a 2D kernel, treating the provided axis as the flat one,
    /// with one work item per voxel.  This also allows calling 2d kernels along
    /// slices of a 3D grid.
    cl::KernelFunctor bind2D(int axis, cl::Kernel k) const;
    cl::KernelFunctor bind2D(int axis, cl::Program prog,
                             const char *kernelname) const;

    /// Bind a 2D-3D kernel, which should take x-, y-, and z-strides as
    /// as parameters, automatically flattening a 2D grid if necessary.
    cl::KernelFunctor bind2D3D(cl::Kernel k) const;
    cl::KernelFunctor bind2D3D(cl::Program prog,
                               const char *kernelname) const;


    /// Enqueue kernel that stores the linear combination
    /// c0 * g0 + d.
    /// Requires isCongruent(g0).
    void linearCombination(fpreal32 c0, const CE_Grid &g0, fpreal32 d);

    /// Enqueue kernel that stores the linear combination
    /// c0 * g0 +  c1 * g1 + d.
    /// Requires isCongruent(g0) && isCongruent(g1).
    void linearCombination(fpreal32 c0, const CE_Grid &g0,
                           fpreal32 c1, const CE_Grid &g1,
			   fpreal32 d);

    /// Enqueue kernel that stores the linear combination
    /// c0 * g0 + c1 * g1 + c2 * g2 + d
    /// Requires isCongruent(g0) && isCongruent(g1) && isCongruent(g2)
    void linearCombination(fpreal32 c0, const CE_Grid &g0,
                           fpreal32 c1, const CE_Grid &g1,
                           fpreal32 c2, const CE_Grid &g2,
			   fpreal32 d);

    /// Compute scale * divergence of the vector field represented by the
    /// supplied grids and voxelsize.
    void divergence(const CE_Grid &x, const CE_Grid &y, const CE_Grid &z,
                    fpreal32 scale, const UT_Vector3 voxelSize);

    /// Add scale * gradient of the supplied field along the supplied axis
    /// and voxelsize.
    void applyGradient(const CE_Grid &p, fpreal32 scale,
                       fpreal32 voxelSize, int axis);


    /// Reductions of the grid to a single value.
    fpreal64 sum() const;
    fpreal64 sumAbs() const;
    fpreal64 sumSqr() const;
    fpreal64 min() const;
    fpreal64 minAbs() const;
    fpreal64 max() const;
    fpreal64 maxAbs() const;
    fpreal64 average() const {return sum() / numVoxels();}

    fpreal64 localAverage(UT_Vector3I &radius);
    fpreal64 localSum(    UT_Vector3I &radius);
    fpreal64 localSumSqr( UT_Vector3I &radius);
    fpreal64 localSumAbs( UT_Vector3I &radius);
    fpreal64 localMin(    UT_Vector3I &radius);
    fpreal64 localMinAbs( UT_Vector3I &radius);
    fpreal64 localMax(    UT_Vector3I &radius);
    fpreal64 localMaxAbs( UT_Vector3I &radius);


    /// Compute the infinity-norm and 2-norm of the grid.
    void computeNorms(fpreal64 &norminf, fpreal64 &norm2) const;
protected:

    const cl::Buffer &allocBuffer() const;
    void releaseBuffer();

    void setValue(fpreal32 cval) const;

    // Reduction helpers
    void getReductionRanges(const cl::Kernel &k,
                            cl::NDRange &globalRange, cl::NDRange &localRange,
                            uint &groupsize, uint &ngroups,
                            size_t &accumsize) const;
    fpreal64 reduceFlat(cl::Buffer outgrid, uint groupsize, uint ngroups,
                        size_t accumsize, const char *reduceFlags) const;

    // Main reduction function.
    fpreal64 doReduce(const char* reduceFlags) const;
    bool doLocalReduce( const char * options, UT_Vector3I &radius );

    mutable cl::Buffer  myBuffer;
    mutable bool        myIsConstant;
    fpreal32            myConstantVal;
    UT_Vector3I         myRes;
    UT_Vector3I         myGhostCells;
    UT_Vector3I         myPadding;
    int                 myAxis2d;

    UT_Vector3I         myStrides;
    fpreal32            myBorderValue;
    UT_VoxelBorderType  myBorderType;
    UT_Vector3F         myBorderScale;
};


#else

class CE_API CE_Grid
{
};

#endif
#endif
